#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <iterator>
#include <memory>

namespace {
    class lookup_error : std::exception {
        virtual const char* what() const throw()
        {
            return "Element does not exit in the map";
        }
    };
}

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {
private:

    class hash_ptr {
        Hash hash;

        std::size_t operator()(const K* &k) const noexcept {
            return hash(*k);
        }
    };

    class compare_ptr {
        bool operator()(const K* &k1, const K* &k2) const noexcept {
            return *k1 == *k2;
        }
    };

    using un_map_type = std::unordered_map<K *, typename std::list<K,V>::iterator, hash_ptr, compare_ptr>;
    using pair_in_list = std::pair<K, V>;

    class Data {
        un_map_type elements_map;
        std::list<K,V> list_of_recent_elements;

        Data() : elements_map({}), list_of_recent_elements({}) {}

        Data(Data const &other) : elements_map({}), list_of_recent_elements({}){
            elements_map = un_map_type();
            list_of_recent_elements = other.element_list;
            for (auto it = list_of_recent_elements.begin(); it != list_of_recent_elements.end(); ++it) {
                elements_map[&(it->first)] = it;
            }
        }

        Data(Data &&other) noexcept :
                elements_map(std::move(other.elements_map)),
                list_of_recent_elements(std::move(other.list_of_recent_elements)) {}
    };

    std::shared_ptr<Data> data;

    bool copy_data(std::shared_ptr<Data> &backup);
    //  add sharable to functions

public:
    using iterator = typename pair_in_list::const_iterator;

    insertion_ordered_map() noexcept;
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

    iterator begin() const {
        return data == nullptr ? iterator() : data->element_list.begin();
    }

    iterator end() const {
        return data == nullptr ? iterator() : data->element_list.end();
    }
};

//  CONSTRUCTORS

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map() noexcept{
    data = std::make_shared<data>();
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map const &other) {
    if (other.data_->markUnshareable)
        data = other.data;
    else
        data = std::make_shared<data>(*(other.data));
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map &&other) noexcept
: data(std::move(other.data)) {}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::~insertion_ordered_map() noexcept {
    delete data;
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash> &
insertion_ordered_map<K, V, Hash>::operator=(insertion_ordered_map<K, V, Hash> other) noexcept {
    if(this != &other)
        other.swap(*this);
    // Old resources released when the destructor of 'other' is called.
    return *this;
}

//  OPERATIONS ON MAP

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::copy_data(std::shared_ptr<Data> &backup) {
    if(!data.unique()) {
        try {
            backup = data;
            data = std::make_shared<Data>(data);
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
                data->elements_map[key] = value_in_map<K, V>(other.at(key), std::prev(data->list_of_recent_keys.end()));
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

    return data->elements_map[k].value;
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
    data = std::make_shared<Data>(Data());
}

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::contains(K const &k) const {
    return data->elements_map.find(k) != data->elements_map.end();
}

#endif //INSERTION_ORDERED_MAP_H
