#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <iostream>
#include <unordered_map>

namespace {
    template <class K>
    class node {
    public:
        const K key;

        node(const K key, node<K> prev = nullptr,
             node<K> next = nullptr) : key(key), prev(prev), next(next) {}
    private:
        node* next;
        node* prev;
    };

    template <class K, class V>
    class value_in_map {
        const V value;
        node<K>* node_in_list;
    };

    class lookup_error : std::exception{

    };
}

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {
private:
    std::shared_ptr<std::unordered_map<K, value_in_map<K, V>, Hash>> elements_ptr;
    std::shared_ptr<node<K>> first_node_ptr;

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
        node<K>* operator->() const;
        node<K>& operator*() const;
    };

    iterator begin();
    iterator begin() const;
    iterator end();
    iterator end() const;

};

// TODO: when to copy? when p1.use_count() > 1 and we want to change sth

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map() {
    first_node_ptr = std::make_shared<node<K>>();
    first_node_ptr = nullptr;
    std::unordered_map<K, value_in_map<K, V>, Hash> m = {};
    // TODO:&m
    elements_ptr = std::make_shared<std::unordered_map<K, value_in_map<K, V>, Hash>>(&m);
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map const &other) {
    //  TODO: how to check if other is valid besides this, does it throw except?
    if(other == this) return;

    first_node_ptr = std::make_shared<node<K>>(other.first_node_ptr);
    elements_ptr = std::make_shared<std::unordered_map<K, value_in_map<K, V>, Hash>>(other.elements_ptr);
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map &&other) noexcept {
    //  TODO: what to do when other == this?
    if(other == this) return;

    first_node_ptr = other.first_node_ptr;
    elements_ptr = other.elements_ptr;

    other.first_node_ptr = nullptr;
    other.elements_ptr = nullptr;
}

//  TODO: why not & before other?
// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap
template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash> &
insertion_ordered_map<K, V, Hash>::operator=(insertion_ordered_map<K, V, Hash> other) {
    if(this != &other) {
        //can i use cpy constructor for map?
        // copy each node and join them?
    }

    return *this;
}

//  iterator






#endif //INSERTION_ORDERED_MAP_H
