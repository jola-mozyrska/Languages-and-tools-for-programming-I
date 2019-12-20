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

    public:
        value_in_map() = default;
        value_in_map(V value, typename std::list<K>::iterator node_iterator) {
            this->value = value;
            this->node_iterator = node_iterator;
        }
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
            for(auto it = other_data.elements_map.begin(); it != other_data.elements_map.end(); ++it) {
                K key(it->first);
                V value(it->second.value);
                value_in_map value_in_map_cpy(value, nullptr);
                elements_map[key] = value_in_map_cpy;
            }

//            list_of_recent_keys = std::list<K>(other_data.list_of_recent_keys);
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
    insertion_ordered_map &operator=(insertion_ordered_map other) noexcept;


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
    data->list_of_recent_keys = std::make_shared<std::list<K>>(std::list<K>());

    std::unordered_map<K, value_in_map<K, V>, Hash> m = {};
    data->elements_map = std::make_shared<std::unordered_map<K, value_in_map<K, V>, Hash>>(m);
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map const &other) {
    //  TODO: should it throw except instead of return?
    if(other == this) return;
    data = std::make_shared<std::list<K>>(other.data);
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map &&other) noexcept
: data(other.data) {
    other.data = nullptr;
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::~insertion_ordered_map() noexcept {
    delete data;
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash> &
insertion_ordered_map<K, V, Hash>::operator=(insertion_ordered_map<K, V, Hash> other) noexcept {
    if(this != &other)
        other.swap(*this);
    // Old resources released when destructor of other is called.
    return *this;
}







#endif //INSERTION_ORDERED_MAP_H
