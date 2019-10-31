//  TODO: tests 3.
//  TODO: zmien ll na unsigned long
// komentarze
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
    //  Ensures std::cerr has been initialized before using it
    //  Otherwise static initialization might break something
    std::ostream &safe_cerr() {
        static std::ios_base::Init mInitializer;
        return std::cerr;
    }

    unsigned long &get_max_poset_id() {
        static unsigned long max_poset_id = 0;
        return max_poset_id;
    }

    long long &get_max_element_id() {
        static long long max_element_id = 0;
        return max_element_id;
    }

    std::unordered_map<unsigned long, std::unordered_map<long long, std::unordered_set<long long>>> &
    get_neighbours_in_poset() {
        static std::unordered_map<unsigned long, std::unordered_map<long long, std::unordered_set<long long>>> neighbours_in_poset;
        return neighbours_in_poset;
    }

    std::unordered_map<std::string, long long> &get_map_of_values_ids() {
        static std::unordered_map<std::string, long long> map_of_values_ids;
        return map_of_values_ids;
    }

    std::string get_string_from_pointer(char const *value) {
        if (value == nullptr)
            return "NULL";
        return std::string(value);
    }

    //  prints debug message about relation in a poset
    void debug_relation(const std::string &function_name, unsigned long id,
                        const std::string &element_1,
                        const std::string &element_2,
                        const std::string &message) {
        if (debug)
            safe_cerr() << function_name << ": poset " << id << ", relation (\""
                        << element_1 << "\", \"" << element_2 << "\") "
                        << message
                        << std::endl;
    }

    //  prints debug message about null value
    void debug_invalid_value(const std::string &function_name,
                             const std::string &value_number) {
        if (debug)
            safe_cerr() << function_name << ": invalid value" << value_number
                        << " (NULL)" << std::endl;
    }

    //  prints debug message about absence of elements
    void debug_two_elements_do_not_exist(const std::string &function_name,
                                         unsigned long id,
                                         const std::string &element_1,
                                         const std::string &element_2) {
        if (debug) {
            std::string message = "does not exist";
            safe_cerr() << function_name << ": poset " << id << ", element \""
                        << element_1 << "\" or \"" << element_2 << "\" "
                        << message
                        << std::endl;
        }
    }

    //  prints debug message connected to the element
    void debug_one_element_message(const std::string &function_name,
                                   unsigned long id,
                                   const std::string &element,
                                   const std::string &message) {
        if (debug)
            safe_cerr() << function_name << ": poset " << id << ", element \""
                        << element << "\" " << message << std::endl;
    }

    //  prints debug message about absence of poset
    void debug_poset_does_not_exist(const std::string &function_name,
                                    unsigned long id) {
        if (debug) {
            std::string message = "does not exist";
            safe_cerr() << function_name << ": poset " << id << " " << message
                        << std::endl;
        }
    }

    //  prints debug message about poset's size
    void debug_poset_size(unsigned long id, long long number_of_elements) {
        if (debug)
            safe_cerr() << "poset_size: poset " << id << " contains "
                        << number_of_elements << " element(s)" << std::endl;
    }

    //  prints debug message about poset
    void debug_poset_message(const std::string &function_name, unsigned long id,
                             const std::string &message) {
        if (debug)
            safe_cerr() << function_name << ": poset " << id << " " << message
                        << std::endl;
    }

    bool check_value(const std::string &function_name, char const *value,
                     const std::string &value_number) {
        if (value == nullptr) {
            debug_invalid_value(function_name, value_number);
            return false;
        }
        return true;
    }

    //  checks presence of poset and 2 elements
    bool check_existance(const std::string &function_name, unsigned long id,
                         const std::string &element_name_1,
                         const std::string &element_name_2) {
        if (!get_neighbours_in_poset().count(id)) {
            debug_poset_does_not_exist(function_name, id);
            return false;
        }

        //  checks presence of id for 2 elements
        if (!get_map_of_values_ids().count(element_name_1) ||
            !get_map_of_values_ids().count(element_name_2)) {
            debug_two_elements_do_not_exist(function_name, id, element_name_1,
                                            element_name_2);
            return false;
        }

        auto element_id_1 = get_map_of_values_ids()[element_name_1];
        auto element_id_2 = get_map_of_values_ids()[element_name_2];
        if (!get_neighbours_in_poset()[id].count(element_id_1) ||
            !get_neighbours_in_poset()[id].count(element_id_2)) {
            debug_two_elements_do_not_exist(function_name, id, element_name_1,
                                            element_name_2);
            return false;
        }
        return true;
    }

    //  helper function for poset_remove
    bool _remove_element(unsigned long id, const std::string &element_name) {
        if (get_map_of_values_ids().count(element_name)) {
            auto element_id = get_map_of_values_ids()[element_name];
            //  element exists in this poset
            if (get_neighbours_in_poset()[id].count(element_id)) {
                get_neighbours_in_poset()[id].erase(element_id);
                //  remove every connections to this element
                for (auto predecessor : get_neighbours_in_poset()[id])
                    get_neighbours_in_poset()[id][predecessor.first].erase(
                            element_id);

                debug_one_element_message("poset_remove", id, element_name,
                                          "removed");
                return true;
            }
        }
        debug_one_element_message("poset_remove", id, element_name,
                                  "does not exist");
        return false;
    }

    //  helper function for poset_add
    bool _add_relation(unsigned long id, const std::string &element_name_1,
                       const std::string &element_name_2) {
        auto element_id_1 = get_map_of_values_ids()[element_name_1];
        auto element_id_2 = get_map_of_values_ids()[element_name_2];

        //  add relation to poset
        get_neighbours_in_poset()[id][element_id_1].insert(element_id_2);

        for (auto const&[value_1_father, set_of_sons] : get_neighbours_in_poset()[id]) {
            if (set_of_sons.find(element_id_1) != set_of_sons.end()) {
                get_neighbours_in_poset()[id][value_1_father].insert(
                        element_id_2);
            }
        }

        debug_relation("poset_add", id, element_name_1, element_name_2,
                       "added");
        return true;
    }
}

namespace jnp1 {
    bool poset_test(unsigned long id, char const *value1, char const *value2) {
        std::string element_name_1 = get_string_from_pointer(value1);
        std::string element_name_2 = get_string_from_pointer(value2);

        if (debug)
            safe_cerr() << "poset_test(" << id << ", \"" << element_name_1
                        << "\", \""
                        << element_name_2 << "\")" << std::endl;

        check_value("poset_test", value1, "1");
        check_value("poset_test", value2, "2");
        if (value1 == nullptr || value2 == nullptr)
            return false;

        if (!get_map_of_values_ids().count(element_name_1))
            safe_cerr() << "tu";

        if (!check_existance("poset_test", id, element_name_1, element_name_2))
            return false;

        auto element_id_1 = get_map_of_values_ids()[element_name_1];
        auto element_id_2 = get_map_of_values_ids()[element_name_2];

        if (!get_neighbours_in_poset()[id][element_id_1].count(element_id_2)) {
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
            safe_cerr() << "poset_new()" << std::endl;

        get_neighbours_in_poset()[get_max_poset_id()] = {};
        ++get_max_poset_id();

        debug_poset_message("poset_new", get_max_poset_id() - 1, "created");
        return get_max_poset_id() - 1;
    }

    void poset_delete(unsigned long id) {
        if (debug)
            safe_cerr() << "poset_delete(" << id << ")" << std::endl;

        if (!get_neighbours_in_poset().count(id)) {
            debug_poset_does_not_exist("poset_delete", id);
            return;
        }

        get_neighbours_in_poset().erase(id);
        debug_poset_message("poset_delete", id, "deleted");
    }

    size_t poset_size(unsigned long id) {
        if (debug)
            safe_cerr() << "poset_size(" << id << ")" << std::endl;

        if (!get_neighbours_in_poset().count(id)) {
            debug_poset_does_not_exist("poset_size", id);
            return 0;
        }

        auto s = get_neighbours_in_poset()[id].size();
        debug_poset_size(id, s);
        return s;
    }

    bool poset_insert(unsigned long id, char const *value) {
        std::string element_name = get_string_from_pointer(value);
        if (debug)
            safe_cerr() << "poset_insert(" << id << ", \"" << element_name
                        << "\")"
                        << std::endl;

        if (!check_value("poset_insert", value, ""))
            return false;

        if (get_neighbours_in_poset().count(id)) {
            //  the value is new
            if (!get_map_of_values_ids().count(element_name)) {
                get_map_of_values_ids()[element_name] = get_max_element_id();
                get_max_element_id()++;
            }
            auto element_id = get_map_of_values_ids()[element_name];

            if (get_neighbours_in_poset()[id].count(element_id)) {
                debug_one_element_message("poset_insert", id, element_name,
                                          "already exists");
                return false;
            }

            get_neighbours_in_poset()[id][element_id] = {element_id};
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
            safe_cerr() << "poset_remove(" << id << ", \"" << element_name
                        << "\")"
                        << std::endl;

        if (get_neighbours_in_poset().count(id))
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
            safe_cerr() << "poset_add(" << id << ", \"" << element_name_1
                        << "\", \""
                        << element_name_2 << "\")" << std::endl;

        check_value("poset_add", value1, "1");
        check_value("poset_add", value2, "2");
        if (value1 == nullptr || value2 == nullptr)
            return false;

        if (!check_existance("poset_add", id, element_name_1, element_name_2))
            return false;

        auto element_id_1 = get_map_of_values_ids()[element_name_1];
        auto element_id_2 = get_map_of_values_ids()[element_name_2];

        if (get_neighbours_in_poset()[id][element_id_1].count(element_id_2) ||
            get_neighbours_in_poset()[id][element_id_2].count(element_id_1)) {
            debug_relation("poset_add", id, element_name_1, element_name_2,
                           "cannot be added");
            return false;
        }

        return _add_relation(id, element_name_1, element_name_2);
    }

    bool poset_del(unsigned long id, char const *value1, char const *value2) {
        std::string element_name_1 = get_string_from_pointer(value1);
        std::string element_name_2 = get_string_from_pointer(value2);

        if (debug)
            safe_cerr() << "poset_del(" << id << ", \"" << element_name_1
                        << "\", \""
                        << element_name_2 << "\")" << std::endl;

        check_value("poset_del", value1, "1");
        check_value("poset_del", value2, "2");
        if (value1 == nullptr || value2 == nullptr)
            return false;

        if (!check_existance("poset_del", id, element_name_1, element_name_2))
            return false;

        auto element_id_1 = get_map_of_values_ids()[element_name_1];
        auto element_id_2 = get_map_of_values_ids()[element_name_2];

        if (element_name_1 == element_name_2 ||
            !get_neighbours_in_poset()[id][element_id_1].count(element_id_2)) {
            debug_relation("poset_del", id, element_name_1, element_name_2,
                           "cannot be deleted");
            return false;
        }

        for (auto child : get_neighbours_in_poset()[id][element_id_1]) {
            if (child != element_id_1 && child != element_id_2 &&
                get_neighbours_in_poset()[id][child].count(element_id_2)) {
                //  we can not remove the edge because relation between value1 and value2 won't be
                //  transitional
                debug_relation("poset_del", id, element_name_1, element_name_2,
                               "cannot be deleted");
                return false;
            }
        }

        get_neighbours_in_poset()[id][element_id_1].erase(element_id_2);
        debug_relation("poset_del", id, element_name_1, element_name_2,
                       "deleted");
        return true;
    }

    void poset_clear(unsigned long id) {
        if (debug)
            safe_cerr() << "poset_clear(" << id << ")" << std::endl;

        if (get_neighbours_in_poset().count(id)) {
            get_neighbours_in_poset().erase(id);
            get_neighbours_in_poset()[id] = {};
            debug_poset_message("poset_clear", id, "cleared");
        } else
            debug_poset_message("poset_clear", id, "does not exist");
    }
}
