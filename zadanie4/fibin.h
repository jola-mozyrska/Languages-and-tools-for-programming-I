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

template <typename ValueType>
constexpr ValueType Fibo(unsigned I) {
    return I; // TODO i-ta liczba fibonacciego
}

template <unsigned I>
struct Fib {
    template <typename ValueType>
    using eval = Value<ValueType, Fibo<ValueType>(I)>;
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
