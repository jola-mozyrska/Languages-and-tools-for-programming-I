#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <iterator>

namespace {
    template <class K, class V>
    class value_in_map {
        const V value;
        typename std::list<K>::iterator node_iterator;
    };

    class lookup_error : std::exception {

    };
}

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {
private:
    class Data{
    public:
        Data();
        Data(Data const &other_data) {
            elements_map = {};
            list_of_recent_keys = {};
        }

        std::unordered_map<K, value_in_map<K, V>, Hash> elements_map;
        std::list<K> list_of_recent_keys;
    };

    std::shared_ptr<Data> data;

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
    };

    iterator begin();
    iterator begin() const;
    iterator end();
    iterator end() const;

};

//  constructors
template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map() {
    data->list_of_recent_keys = std::make_shared<std::list<K>>(new std::list<K>);

    std::unordered_map<K, value_in_map<K, V>, Hash> m = {};
    data->elements_map = std::make_shared<std::unordered_map<K, value_in_map<K, V>, Hash>>(&m);
}




#endif //INSERTION_ORDERED_MAP_H
