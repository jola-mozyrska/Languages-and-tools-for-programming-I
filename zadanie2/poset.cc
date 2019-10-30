#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <climits>

#include "poset.h"

#ifndef NDEBUG
const bool debug = true;
#else
const bool debug = false;
#endif

namespace {
    std::unordered_map<unsigned long, size_t> nr_of_elements_in_poset;
    unsigned long max_poset_id = 0;
    long long max_element_id = 0;

    std::unordered_map<unsigned long, std::unordered_map<long long, std::unordered_set<long long>>> neighbours_in_poset;
    std::unordered_map<std::string, long long> map_of_values_ids;

    std::string get_string_from_pointer(char const *value) {
        if(value == nullptr)
            return "NULL";
        return std::string(value);
    }

    //poset_del: poset 0, relation ("E", "F") cannot be deleted
    void debug_relation(const std::string &function_name, unsigned long id,
                        const std::string &element_1,
                        const std::string &element_2,
                        const std::string &message) {
        if (debug)
            std::cerr << function_name << ": poset " << id << ", relation (\""
                      << element_1 << "\", \"" << element_2 << "\") " << message
                      << std::endl;
    }

    //  poset_del: invalid value1 (NULL)
    bool debug_invalid_value(const std::string &function_name,
                             const std::string &value_number) {
        if (debug)
            std::cerr << function_name << ": invalid value" << value_number
                      << " (NULL)" << std::endl;
        return false;
    }

    //poset_del: poset 0, element "D" or "C" does not exist
    void debug_two_elements_do_not_exist(const std::string &function_name,
                                    unsigned long id,
                                    const std::string &element_1,
                                    const std::string &element_2) {
        if (debug) {
            std::string message = "does not exist";
            std::cerr << function_name << ": poset " << id << ", element \""
                      << element_1 << "\" or \"" << element_2 << "\" "
                      << message
                      << std::endl;
        }
    }

    //poset_remove: poset 0, element "A" removed
    void debug_one_element_message(const std::string &function_name,
                                   unsigned long id,
                                   const std::string &element,
                                   const std::string &message) {
        if (debug)
            std::cerr << function_name << ": poset " << id << ", element \""
                      << element << "\" " << message << std::endl;
    }

    //poset_insert: poset 1 does not exist
    void debug_poset_does_not_exist(const std::string &function_name,
                                    unsigned long id) {
        if (debug) {
            std::string message = "does not exist";
            std::cerr << function_name << ": poset " << id << " " << message
                      << std::endl;
        }
    }

    //poset_size: poset 0 contains 3 element(s)
    void debug_poset_size(unsigned long id, long long number_of_elements) {
        if (debug)
            std::cerr << "poset_size: poset " << id << " contains "
                      << number_of_elements << " element(s)" << std::endl;
    }

    //poset_new: poset 0 created
    void debug_poset_message(const std::string &function_name, unsigned long id,
                             const std::string &message) {
        if (debug)
            std::cerr << function_name << ": poset " << id << " " << message
                      << std::endl;
    }

    bool check_value(const std::string &function_name, char const *value,
                     const std::string &value_number) {
        if (value == nullptr)
            return debug_invalid_value(function_name, value_number);
        return true;
    }

    bool check_existance(const std::string &function_name, unsigned long id,
                         const std::string &element_name_1,
                         const std::string &element_name_2) {
        if (!map_of_values_ids.count(element_name_1) ||
            !map_of_values_ids.count(element_name_2)) {
            debug_two_elements_do_not_exist(function_name, id, element_name_1,
                                            element_name_2);
            return false;
        }

        if (!neighbours_in_poset.count(id)) {
            debug_poset_does_not_exist(function_name, id);
            return false;
        }

        auto element_id_1 = map_of_values_ids[element_name_1];
        auto element_id_2 = map_of_values_ids[element_name_2];
        if (!neighbours_in_poset[id].count(element_id_1) ||
            !neighbours_in_poset[id].count(element_id_2)) {
            debug_two_elements_do_not_exist(function_name, id, element_name_1,
                                            element_name_2);
            return false;
        }
        return true;
    }

    bool _remove_element(unsigned long id, const std::string& element_name) {
        if (map_of_values_ids.count(element_name)) {
            auto element_id = map_of_values_ids[element_name];
            //  element exists in this poset
            if (neighbours_in_poset[id].count(element_id)) {
                neighbours_in_poset[id].erase(element_id);
                //  remove every connections to this element
                for (auto predecessor : neighbours_in_poset[id])
                    neighbours_in_poset[id][predecessor.first].erase(
                            element_id);

                debug_one_element_message("poset_remove", id, element_name,
                                          "removed");
                nr_of_elements_in_poset[id]--;
                return true;
            }
        }
        debug_one_element_message("poset_remove", id, element_name,
                                  "does not exist");
        return false;
    }

    bool _add_element(unsigned long id, const std::string& element_name_1, const std::string& element_name_2) {
        auto element_id_1 = map_of_values_ids[element_name_1];
        auto element_id_2 = map_of_values_ids[element_name_2];

        //  add relation to poset
        neighbours_in_poset[id][element_id_1].insert(element_id_2);

        for (auto const&[value_1_father, set_of_sons] : neighbours_in_poset[id]) {
            if (set_of_sons.find(element_id_1) != set_of_sons.end()) {
                neighbours_in_poset[id][value_1_father].insert(element_id_2);
            }
        }

        debug_relation("poset_add", id, element_name_1, element_name_2, "added");
        return true;
    }
}

namespace jnp1 {
    bool poset_test(unsigned long id, char const *value1, char const *value2) {
        std::string element_name_1 = get_string_from_pointer(value1);
        std::string element_name_2 = get_string_from_pointer(value2);

        if (debug)
            std::cerr << "poset_test(" << id << ", \"" << element_name_1
                      << "\", \""
                      << element_name_2 << "\")" << std::endl;

        if (!check_value("poset_test", value1, "1") ||
            !check_value("poset_test", value2, "2"))
            return false;

        if(!map_of_values_ids.count(element_name_1))
            std::cerr << "tu";

        if (!check_existance("poset_test", id, element_name_1, element_name_2))
            return false;

        auto element_id_1 = map_of_values_ids[element_name_1];
        auto element_id_2 = map_of_values_ids[element_name_2];

        if (!neighbours_in_poset[id][element_id_1].count(element_id_2)) {
            debug_relation("poset_test", id, element_name_1, element_name_2,
                           "does not exist");
            return false;
        }

        debug_relation("poset_test", id, element_name_1, element_name_2,
                       "exists");
        return true;
    }

    unsigned long poset_new(void) {
        if (debug)
            std::cerr << "poset_new()" << std::endl;

        neighbours_in_poset[max_poset_id] = {};
        nr_of_elements_in_poset[max_poset_id] = 0;
        ++max_poset_id;

        debug_poset_message("poset_new", max_poset_id - 1, "created");
        return max_poset_id - 1;
    }

    void poset_delete(unsigned long id) {
        if (debug)
            std::cerr << "poset_delete(" << id << ")" << std::endl;

        if (!neighbours_in_poset.count(id)) {
            debug_poset_does_not_exist("poset_delete", id);
            return;
        }

        nr_of_elements_in_poset[id] = 0;
        neighbours_in_poset.erase(id);
        debug_poset_message("poset_delete", id, "deleted");
    }

    size_t poset_size(unsigned long id) {
        if (debug)
            std::cerr << "poset_size(" << id << ")" << std::endl;

        if (!neighbours_in_poset.count(id)) {
            debug_poset_does_not_exist("poset_size", id);
            return 0;
        }

        auto s = nr_of_elements_in_poset[id];
        debug_poset_size(id, s);
        return s;
    }

    bool poset_insert(unsigned long id, char const *value) {
        std::string element_name = get_string_from_pointer(value);
        if (debug)
            std::cerr << "poset_insert(" << id << ", \"" << element_name
                      << "\")"
                      << std::endl;

        if (!check_value("poset_insert", value, ""))
            return false;

        if (neighbours_in_poset.count(id)) {
            //  the value is new
            if (!map_of_values_ids.count(element_name)) {
                map_of_values_ids[element_name] = max_element_id++;
            }
            auto element_id = map_of_values_ids[element_name];

            if (neighbours_in_poset[id].count(element_id)) {
                debug_one_element_message("poset_insert", id, element_name,
                                          "already exists");
                return false;
            }

            neighbours_in_poset[id][element_id] = {element_id};
            nr_of_elements_in_poset[id]++;
            debug_one_element_message("poset_insert", id, element_name,
                                      "inserted");
            return true;
        } else
            debug_poset_does_not_exist("poset_insert", id);

        return false;
    }

    bool poset_remove(unsigned long id, char const *value) {
        std::string element_name = get_string_from_pointer(value);

        if (debug)
            std::cerr << "poset_remove(" << id << ", \"" << element_name
                      << "\")"
                      << std::endl;

        if (neighbours_in_poset.count(id))
            return _remove_element(id, element_name);
        else {
            debug_poset_does_not_exist("poset_remove", id);
            return false;
        }
    }

    bool poset_add(unsigned long id, char const *value1, char const *value2) {
        std::string element_name_1 = get_string_from_pointer(value1);
        std::string element_name_2 = get_string_from_pointer(value2);

        if (debug)
            std::cerr << "poset_add(" << id << ", " << element_name_1 << ", "
                      << element_name_2 << ")" << std::endl;

        if (!check_value("poset_add", value1, "1") ||
            !check_value("poset_add", value2, "2"))
            return false;

        if (!check_existance("poset_add", id, element_name_1, element_name_2))
            return false;

        auto element_id_1 = map_of_values_ids[element_name_1];
        auto element_id_2 = map_of_values_ids[element_name_2];

        if (neighbours_in_poset[id][element_id_1].count(element_id_2) ||
            neighbours_in_poset[id][element_id_2].count(element_id_1)) {
            debug_relation("poset_add", id, element_name_1, element_name_2,
                           "cannot be added");
            return false;
        }

        return _add_element(id, element_name_1, element_name_2);
    }

    bool poset_del(unsigned long id, char const *value1, char const *value2) {
        std::string element_name_1 = get_string_from_pointer(value1);
        std::string element_name_2 = get_string_from_pointer(value2);

        if (debug)
            std::cerr << "poset_del(" << id << ", " << element_name_1 << ", "
                      << element_name_2 << ")" << std::endl;

        if (!check_value("poset_add", value1, "1") ||
            !check_value("poset_add", value2, "2"))
            return false;

        if (!check_existance("poset_del", id, element_name_1, element_name_2))
            return false;

        auto element_id_1 = map_of_values_ids[element_name_1];
        auto element_id_2 = map_of_values_ids[element_name_2];

        if (element_name_1 == element_name_2 ||
            !neighbours_in_poset[id][element_id_1].count(element_id_2)) {
            debug_relation("poset_del", id, element_name_1, element_name_2,
                           "cannot be deleted");
            return false;
        }

        for (auto child : neighbours_in_poset[id][element_id_1]) {
            if (child != element_id_1 && child != element_id_2 &&
                neighbours_in_poset[id][child].count(element_id_2)) {
                //  we can not remove the edge because relation between value1 and value2 won't be
                //  transitional
                if (debug)
                    std::cerr << child << "tu";
                debug_relation("poset_del", id, element_name_1, element_name_2,
                               "cannot be deleted");
                return false;
            }
        }

        neighbours_in_poset[id][element_id_1].erase(element_id_2);
        debug_relation("poset_del", id, element_name_1, element_name_2,
                       "deleted");
        return true;
    }

    void poset_clear(unsigned long id) {
        if (debug)
            std::cerr << "poset_clear(" << id << ")" << std::endl;

        if (neighbours_in_poset.count(id)) {
            neighbours_in_poset.erase(id);
            neighbours_in_poset[id] = {};
            nr_of_elements_in_poset[id] = 0;
            debug_poset_message("poset_clear", id, "cleared");
        } else
            debug_poset_message("poset_clear", id, "does not exist");
    }
}
