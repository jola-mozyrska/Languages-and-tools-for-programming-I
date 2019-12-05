#ifndef PROJECT4_FIBIN_H
#define PROJECT4_FIBIN_H
#include <iostream>

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


constexpr uint32_t Var(const char *N) {
    return N[0]; // TODO funkcja hashująca
}

template <uint32_t I>
struct Ref {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, I>; // TODO znajdowanie w liście wartości
};

template <unsigned I>
struct Fib {
    template <typename ValueType>
    using eval = Value<ValueType,
                static_cast<ValueType>(Fib<I-1>::template eval<ValueType
                    >::val + Fib<I-2>::template eval<ValueType>::val)>;
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

template <bool B>
struct Bool {
    constexpr static bool val = B;
};

struct True {
    template <typename ValueType>
    using eval = Bool<true>;
};

struct False{
    template <typename ValueType>
    using eval = Bool<false>;
};

template <typename V>
struct Lit {
    template <typename ValueType, typename List>
    using eval = typename V::template eval<ValueType>;
};


/**************************************************************************/
/** LET **/
/**************************************************************************/

template <uint32_t var, typename V, typename E>
struct Let {
    template <typename ValueType, typename List>
    using eval = typename E::template eval<ValueType, List>; // TODO zrobić liste i dodać do niej var tutaj
};

/**************************************************************************/
/** Operacje arytmetyczne **/
/**************************************************************************/

template <typename T>
struct Inc1 {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, static_cast<ValueType>(T::template eval<ValueType, List>::val + Fib<1>::template eval<ValueType>::val)>;
};

template <typename T>
struct Inc10 {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, static_cast<ValueType>(T::template eval<ValueType, List>::val + Fib<10>::template eval<ValueType>::val)>;
};

template <typename... T>
struct Sum {};

template <typename T1, typename... T2>
struct Sum<T1, T2...> {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, static_cast<ValueType>(T1::template eval<ValueType, List>::val + Sum<T2...>::template eval<ValueType, List>::val)>;
};

template <>
struct Sum<> {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, 0>;
};

/**************************************************************************/
/** EQ **/
/**************************************************************************/

template <typename T1, typename T2>
struct CheckEq{
    template <typename ValueType, typename List>
    using eval = Bool<false>;
};

template <typename T>
struct CheckEq<T, T>{
    template <typename ValueType, typename List>
    using eval = Bool<true>;
};

template <typename E1, typename E2> // TODO może powinno być eval<>::val - zmienić jak coś przy testowaniu
struct Eq {
    template <typename ValueType, typename List>
    using eval = typename CheckEq<typename E1::template eval<ValueType, List>,
            typename E2::template eval<ValueType, List>>::template eval<ValueType, List>;
};

/**************************************************************************/
/** IF **/
/**************************************************************************/

template <bool B, typename Then, typename Else>
struct CheckIf {
    template <typename ValueType, typename List>
    using eval = typename Else::template eval<ValueType, List>;
};

template <typename Then, typename Else>
struct CheckIf<true, Then, Else> {
    template <typename ValueType, typename List>
    using eval = typename Then::template eval<ValueType, List>;
};

template <typename C, typename Then, typename Else>
struct If {
    template <typename ValueType, typename List>
    using eval = typename CheckIf<C::template eval<ValueType, List>::val, Then, Else>::template eval<ValueType, List>;
};

template <uint32_t I, typename T>
struct Lambda {
    template <typename ValueType, typename List>
    using eval = decltype([] {T::template eval<ValueType, List>;}); // TODO tutaj jakoś dodać to I
};

#endif //PROJECT4_FIBIN_H
