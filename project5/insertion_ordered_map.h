#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <iterator>
#include <memory>
#include <algorithm>

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

    class compare_ptr {
    public:
        bool operator()(const K* const &k1, const K* const &k2) const noexcept {
            return *k1 == *k2;
        }
    };

    class hash_ptr {
    public:
        Hash hash;
        hash_ptr() = default;

        std::size_t operator()(const K* const &k) const noexcept {
            return hash(*k);
        }
    };

    using pair_in_list = std::pair<K, V>;
    using un_map_type = std::unordered_map<const K *, typename std::list<pair_in_list>::iterator, hash_ptr, compare_ptr>;

    class Data {
    public:
        un_map_type elements_map;
        std::list<pair_in_list> list_of_recent_elements;
        bool unshareable;

        Data() : elements_map({}), list_of_recent_elements({}), unshareable(false) {}

        Data(Data const &other) : unshareable(false) {
            elements_map = un_map_type();
            list_of_recent_elements = other.list_of_recent_elements;
            for (auto it = list_of_recent_elements.begin(); it != list_of_recent_elements.end(); ++it) {
                elements_map[&(it->first)] = it;
            }
        }

        Data(Data &&other) noexcept :
                elements_map(std::move(other.elements_map)),
                list_of_recent_elements(std::move(other.list_of_recent_elements)),
                unshareable(other.unshareable) {}

        Data& operator=(Data &other) {
            elements_map = other.elements_map;
            list_of_recent_elements = other.list_of_recent_elements;
            return *this;
        }
    };

    std::shared_ptr<Data> data;
    bool copy_data(std::shared_ptr<Data> &backup);

public:
    using iterator = typename std::list<pair_in_list>::const_iterator;

    insertion_ordered_map() noexcept;
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
        return data == nullptr ? iterator() : data->list_of_recent_elements.begin();
    }

    iterator end() const {
        return data == nullptr ? iterator() : data->list_of_recent_elements.end();
    }
};

//  CONSTRUCTORS

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map() noexcept{
    data = std::make_shared<Data>();
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map const &other) {
     data = std::make_shared<Data>(*(other.data));
}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash>::insertion_ordered_map(insertion_ordered_map &&other) noexcept
: data(std::move(other.data)) {}

template <class K, class V, class Hash>
insertion_ordered_map<K, V, Hash> &
insertion_ordered_map<K, V, Hash>::operator=(insertion_ordered_map<K, V, Hash> other) noexcept {
    if(this != &other)
        data.swap(other.data);
    // Old resources released when the destructor of 'other' is called.
    return *this;
}

//  OPERATIONS ON MAP

template<class K, class V, class Hash>
bool insertion_ordered_map<K, V, Hash>::copy_data(std::shared_ptr<Data> &backup) {
    if(!data.unique()) {
        try {
            backup = data;
            data = std::make_shared<Data>(*data);
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
        data->list_of_recent_elements.splice(data->list_of_recent_elements.end(), data->list_of_recent_elements, data->elements_map[&k]);
        return false;
    }
    else {
        try {
            data->list_of_recent_elements.push_back(std::make_pair(k, v));
            data->elements_map[&k] = std::prev(data->list_of_recent_elements.end());
            return true;
        }
        catch(std::exception &e) {
            if(copied)
                data = backup;
            data->elements_map.erase(&k);
            data->list_of_recent_elements.pop_back();

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

        data->list_of_recent_elements.erase((data->elements_map).at(&k));
        data->elements_map.erase(&k);    
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
        for(pair_in_list &pair : other.data->list_of_recent_elements) {
            const K &key = pair.first;
            if(contains(key))
                data->list_of_recent_elements.splice(data->list_of_recent_elements.end(), data->list_of_recent_elements, data->elements_map[&key]);
            else {
                data->list_of_recent_elements.push_back(std::make_pair(key, other.at(key)));
                data->elements_map[&key] = std::prev(data->list_of_recent_elements.end());
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

    return (data->elements_map[&k])->second;
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
    return data->list_of_recent_elements.size();
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
    return data->elements_map.find(&k) != data->elements_map.end();
}

#endif //INSERTION_ORDERED_MAP_H
