#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <iterator>
#include <memory>

namespace {
    template <class K, class V>
    class value_in_map {
    public:
        value_in_map() = default;
        value_in_map(V value, typename std::list<K>::iterator node_iterator) {
            this->value = value;
            this->node_iterator = node_iterator;
        }

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

    bool copy_data(std::shared_ptr<Data> &backup);

    std::shared_ptr<Data> data;

public:
    insertion_ordered_map();
    ~insertion_ordered_map() noexcept;
    insertion_ordered_map(insertion_ordered_map const &other);
    insertion_ordered_map(insertion_ordered_map &&other) noexcept;
    insertion_ordered_map &operator=(insertion_ordered_map other) noexcept;


    bool insert(K const &k, V const &v);
    void erase(K const &k);
    void merge(insertion_ordered_map const &other);

    V &at(K const &k);
    V const &at(K const &k) const;
    V &operator[](K const &k);
    size_t size() const;
    bool empty() const;
    void clear();
    bool contains(K const &k) const;

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

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::copy_data(std::shared_ptr<Data> &backup) {
    if(!data.unique()) {
        try {
            backup = data;
            data = std::make_shared(Data(*data));
            return true;
        }
        catch(std::exception &e) {
            data = backup;
            throw e;
        }
    }

    return false;
}

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::insert(K const &k, V const &v) {
    std::shared_ptr<Data> backup;
    bool copied = copy_data(backup);
        
    if(contains(k)) {
        data->list_of_recent_keys.splice(data->list_of_recent_keys.end(), data->list_of_recent_keys, data->elements_map[k].node_iterator);
        return false;
    }
    else {
        try {
            data->list_of_recent_keys.push_back(k);
            data->elements_map[k] = value_in_map<K, V>(v, std::prev(data->list_of_recent_keys.end()));
            return true;
        }
        catch(std::exception &e) {
            if(copied)
                data = backup;
            data->elements_map.erase(k);
            data->list_of_recent_keys.pop_back();

            throw e;
        }
    }
}

template<class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::erase(K const &k) {
    std::shared_ptr<Data> backup;
    bool copied = copy_data(backup);

    try {
        if(!contains(k))
            throw lookup_error();

        data->list_of_recent_keys.erase(data->elements_map[k].node_iterator);
        data->elements_map.erase(k);    
    }
    catch(std::exception &e) {
        if(copied)
            data = backup;
        throw e;
    }
}

template<class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::merge(insertion_ordered_map const &other) {
    std::shared_ptr<Data> backup;
    bool copied = copy_data(backup);
    Data old_data{*data};

    try {
        for(K key : other.data->list_of_recent_keys) {
            if(contains(key))
                data->list_of_recent_keys.splice(data->list_of_recent_keys.end(), data->list_of_recent_keys, data->elements_map[key].node_iterator);
            else {
                data->list_of_recent_keys.push_back(key);
                data->elements_map[key] = value_in_map<K, V>(other[key], std::prev(data->list_of_recent_keys.end()));
            }
        }
    }
    catch(std::exception &e) {
        if(copied)
            data = backup;
        else
            *data = old_data;
        throw e;
    }
}

template<class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::at(K const &k) {
    if(!contains(k))
        throw lookup_error();

    return *(data->elements_map[k].node_iterator);
}

template<class K, class V, class Hash>
V const &insertion_ordered_map<K, V, Hash>::at(K const &k) const {
    return at(k);
}

template<class K, class V, class Hash>
V &insertion_ordered_map<K, V, Hash>::operator[](K const &k) {
    if(!contains(k))
        insert(k, V());

    return at(k);
}

template<class K, class V, class Hash>
size_t insertion_ordered_map<K, V, Hash>::size() const {
    return data->list_of_recent_keys.size();
}

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::empty() const {
    return size() == 0;
}

template<class K, class V, class Hash>
void insertion_ordered_map<K, V, Hash>::clear() {
    data = std::make_shared(Data());
}

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::contains(K const &k) const {
    return data->elements_map.find(k) != data->elements_map.end();
}

#endif //INSERTION_ORDERED_MAP_H
