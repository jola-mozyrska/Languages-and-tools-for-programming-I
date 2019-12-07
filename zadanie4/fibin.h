//  TODO: enable_if - dokończyć
//  TODO: everything is public, we may want to hide it in namespace

#ifndef FIBIN_FIBIN_H
#define FIBIN_FIBIN_H

#include <iostream>

template <typename T, T i>
struct Value {
    constexpr static T val = i;
};

struct EmptyList;

template <uint64_t I, typename V, typename Tail=EmptyList>
struct _List {
    using tail = Tail;
};

template <uint64_t I, typename List>
struct Find {
    using result = typename Find<I, typename List::tail>::result;
};

template <uint64_t I, typename Value, typename Tail>
struct Find<I, _List<I, Value, Tail>> {
    using result =  Value;
};

constexpr uint64_t Var(const char *N) {
    size_t i = 0;
    uint64_t hash = 0;
    uint64_t temp = 0;
    const uint64_t BASE = 100;
    while (N[i] != '\0') {
        hash *= BASE;
        temp = static_cast<int64_t>(('A' <= N[i] && N[i] <= 'Z') ? N[i] :
                                    (('a' <= N[i] && N[i] <= 'z') ? N[i] - 'a' + 'A' :
                                     (('0' <= N[i] && N[i] <= '9') ? N[i] : 0)));
        if (temp == 0)
            return 0;
        hash += static_cast<uint64_t>(temp);
        ++i;
    }

    return hash;
}

template <uint64_t I>
struct Ref {
    template <typename ValueType, typename List>
    using eval = typename Find<I, List>::result;
};

template<typename T>
struct Fibin {
    template <typename E>
    static constexpr typename std::enable_if<std::is_integral<T>::value, T>::type eval() {
        return E::template eval<T, EmptyList>::val;
    }
};


/**************************************************************************/
/** Fib, Lit, Bool **/
/**************************************************************************/

template <unsigned I, typename ValueType>
struct _Fib {
    static constexpr ValueType val = static_cast<ValueType>(_Fib<I-1, ValueType>::val + _Fib<I-2, ValueType>::val);
};

template <typename ValueType>
struct _Fib<0, ValueType> {
    static constexpr ValueType val = static_cast<ValueType>(0);
};

template <typename ValueType>
struct _Fib<1, ValueType> {
    static constexpr ValueType val = static_cast<ValueType>(1);
};

template <unsigned I>
struct Fib {
    template <typename ValueType>
    using eval = Value<ValueType, _Fib<I, ValueType>::val>;
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

template <uint64_t var, typename V, typename E>
struct Let {
    static_assert(var > 0);

    template <typename ValueType, typename List>
    using eval = typename E::template eval<ValueType, _List<var, typename V::template eval<ValueType, List>, List>>;
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

template <typename T1, typename T2, typename... T3>
struct Sum {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, static_cast<ValueType>(T1::template eval<ValueType, List>::val + Sum<T2, T3...>::template eval<ValueType, List>::val)>;
};

template <typename T1, typename T2>
struct Sum<T1, T2> {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, static_cast<ValueType>(T1::template eval<ValueType, List>::val + T2::template eval<ValueType, List>::val)>;
};

/**************************************************************************/
/** EQ **/
/**************************************************************************/

template <typename T1, typename T2>
struct _Eq{
    template <typename ValueType, typename List>
    using eval = Bool<false>;
};

template <typename T>
struct _Eq<T, T>{
    template <typename ValueType, typename List>
    using eval = Bool<true>;
};

template <typename E1, typename E2> // TODO może powinno być eval<>::val - zmienić jak coś przy testowaniu
struct Eq {
    template <typename ValueType, typename List>
    using eval = typename _Eq<typename E1::template eval<ValueType, List>,
            typename E2::template eval<ValueType, List>>::template eval<ValueType, List>;
};

/**************************************************************************/
/** IF **/
/**************************************************************************/

template <bool B, typename Then, typename Else>
struct _If {
    template <typename ValueType, typename List>
    using eval = typename Else::template eval<ValueType, List>;
};

template <typename Then, typename Else>
struct _If<true, Then, Else> {
    template <typename ValueType, typename List>
    using eval = typename Then::template eval<ValueType, List>;
};

template <typename C, typename Then, typename Else>
struct If {
    template <typename ValueType, typename List>
    using eval = typename _If<C::template eval<ValueType, List>::val, Then, Else>::template eval<ValueType, List>;
};

template <uint64_t I, typename Body>
struct Lambda {};

template <typename Body, typename Param>
struct Invoke {
    template <typename ValueType, typename List>
    using eval = typename Invoke<typename Body::template eval<ValueType, List>, Param>::template eval<ValueType, List>;
};

template <uint64_t I, typename Body, typename Param>
struct Invoke<Lambda<I, Body>, Param> {
    static_assert(I > 0);
    template <typename ValueType, typename List>
    using eval = typename Body::template eval<ValueType, _List<I, typename Param::template eval<ValueType, List>, List>>;
};

#endif //FIBIN_FIBIN_H
