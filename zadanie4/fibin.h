#ifndef PROJECT4_FIBIN_H
#define PROJECT4_FIBIN_H

#include <iostream>
#include "fibin.h"
struct EmptyList {

};

template<typename T>
struct Fibin {
    template <typename E>
    static constexpr T eval() {
        return E::template eval<T, EmptyList>::val; // TODO pusta lista
    }
};

template <typename T, T i>
struct Value {
    constexpr static T val = i;
};

/*
constexpr uint32_t Var() {

}*/

template <unsigned I>
struct Fib {
    template <typename ValueType>
    using eval = Value<ValueType, ValueType(Fib<I-1>::template eval<ValueType>::val + Fib<I-2>::template eval<ValueType>::val)>;
};

template<>
struct Fib<0> {
    template <typename ValueType>
    using eval = Value<ValueType, 0>;
};

template<>
struct Fib<1> {
    template <typename ValueType>
    using eval = Value<ValueType, 1>;
};

template <typename V>
struct Lit {
    template <typename ValueType, typename List>
    using eval = typename V::template eval<ValueType>;
};

template <uint32_t var, typename V, typename E>
struct Let {
    template <typename ValueType, typename List>
    using eval = typename E::template eval<ValueType, List>; // TODO zrobić liste i dodać do niej var tutaj
};

#endif //PROJECT4_FIBIN_H
