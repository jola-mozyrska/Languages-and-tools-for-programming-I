#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <iostream>

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {
private:
    std::unordered_set<K, Hash> elements;

public:
    insertion_ordered_map();
    ~insertion_ordered_map() noexcept;
    insertion_ordered_map(insertion_ordered_map const &other);
    insertion_ordered_map(insertion_ordered_map &&other) noexcept;
    insertion_ordered_map &operator=(insertion_ordered_map other);


    bool insert(K const &k, V const &v);
    void erase(K const &k);

    class iterator{
        iterator();
        iterator(const iterator &other);
        iterator& operator++();
        bool operator==(iterator &other) const;
        bool operator!=(iterator &other) const;
        node<K, V>* operator->() const;
        node<K, V>& operator*() const;
    };

    iterator begin();
    iterator begin() const;
    iterator end();
    iterator end() const;

};

class lookup_error{

};



#endif //INSERTION_ORDERED_MAP_H
