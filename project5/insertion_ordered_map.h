#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <iostream>

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {
public:
    insertion_ordered_map();
    insertion_ordered_map(insertion_ordered_map const &other);
    insertion_ordered_map(insertion_ordered_map &&other);
    insertion_ordered_map &operator=(insertion_ordered_map other);

    bool insert(K const &k, V const &v);
    void erase(K const &k);
};

class lookup_error{

};



#endif //INSERTION_ORDERED_MAP_H
