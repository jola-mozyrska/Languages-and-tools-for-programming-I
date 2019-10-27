//  TODO: is int_32 a good option?
//  TODO: tests
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <climits>

#include "poset.h"

std::unordered_map<unsigned long, size_t> nr_of_elements_in_poset;
unsigned long max_poset_id = 0;
uint32_t max_element_id = 0;

std::unordered_map<unsigned long, std::unordered_map<uint32_t, std::unordered_set<uint32_t>>> neighbours_in_poset;
std::unordered_map<std::string, uint32_t> map_of_values_ids;

bool poset_test(unsigned long id, char const *value1, char const *value2) {
    std::string element_name_1(value1);
    if (!map_of_values_ids.count(element_name_1))
        return false;

    std::string element_name_2(value2);
    if (!map_of_values_ids.count(element_name_2))
        return false;

    if (element_name_1 == element_name_2)
        return true;

    auto value_id_1 = map_of_values_ids[element_name_1];
    auto value_id_2 = map_of_values_ids[element_name_2];
    if (neighbours_in_poset.count(id) &&
        neighbours_in_poset[id].count(value_id_1) &&
        neighbours_in_poset[id][value_id_1].count(value_id_2)) {
        return true;
    }
    return false;
}

unsigned long poset_new(void) {
    neighbours_in_poset.insert(max_poset_id, {});
    nr_of_elements_in_poset[max_poset_id] = 0;
    ++max_poset_id;
    return max_poset_id - 1;
}

void poset_delete(unsigned long id) {
    if (!neighbours_in_poset.count(id))
        return;

    nr_of_elements_in_poset[id] = 0;
    neighbours_in_poset.erase(id);
}

size_t poset_size(unsigned long id) {
    if (!neighbours_in_poset.count(id))
        return 0;
    return nr_of_elements_in_poset[id];
}

bool poset_insert(unsigned long id, char const *value) {
    if (neighbours_in_poset.count(id)) {
        std::string element_name(value);
        //  the value is new
        if (!map_of_values_ids.count(element_name)) {
            map_of_values_ids[element_name] = max_element_id++;
        }
        auto value_id = map_of_values_ids[element_name];
        neighbours_in_poset[id].insert(value_id, {});
    }
    return false;
}

bool poset_remove(unsigned long id, char const *value) {
    if (neighbours_in_poset.count(id)) {
        std::string element_name(value);
        if (map_of_values_ids.count(element_name)) {
            auto element_id = map_of_values_ids[element_name];
            //  element exists in this poset
            if (neighbours_in_poset[id].count(element_id)) {
                neighbours_in_poset[id].erase(element_id);
                //  remove every connections to this element
                for (auto predecessor : neighbours_in_poset[id])
                    neighbours_in_poset[id][predecessor.first].erase(
                            element_id);

                return true;
            }
        }
    }
    return false;
}

bool poset_add(unsigned long id, char const *value1, char const *value2) {
    if (poset_test(id, value1, value2))
        return false;
    if (poset_test(id, value2, value1))
        return false;

    if (neighbours_in_poset.count(id)) {
        std::string element_name_1(value1);
        std::string element_name_2(value2);

        auto value_id_1 = map_of_values_ids[element_name_1];
        auto value_id_2 = map_of_values_ids[element_name_2];

        if (!neighbours_in_poset[id].count(value_id_1) ||
            !neighbours_in_poset.count(value_id_2))
            return false;

        //  relation is reciprocal
        if (element_name_1 == element_name_2)
            return true;

        //  add relation to poset
        neighbours_in_poset[id][value_id_1].insert(value_id_2);

        for (auto const&[value_1_father, set_of_sons] : neighbours_in_poset[id]) {
            if (set_of_sons.find(value_id_1) != set_of_sons.end()) {
                neighbours_in_poset[id][value_1_father].insert(value_id_2);
            }
        }

        return true;
    }
    return false;
}

bool poset_del(unsigned long id, char const *value1, char const *value2) {
    //  poset exists
    if (!poset_test(id, value1, value2))
        return false;

    std::string element_name_1(value1);
    std::string element_name_2(value2);

    if (element_name_1 == element_name_2)
        return false;

    auto value_id_1 = map_of_values_ids[element_name_1];
    auto value_id_2 = map_of_values_ids[element_name_2];

    //  neighbours_in_poset[id] does not contain loops
    for (auto child : neighbours_in_poset[id][value_id_1]) {
        if (neighbours_in_poset[id][child].count(value_id_2)) {
            //  we can not remove the edge because relation between value1 and value2 won't be
            //  transitional
            return false;
        }
    }

    neighbours_in_poset[id][value_id_1].erase(value_id_2);
    return true;
}

void poset_clear(unsigned long id) {
    if (neighbours_in_poset.count(id))
        neighbours_in_poset.erase(id);
}
