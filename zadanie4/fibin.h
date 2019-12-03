#ifndef PROJECT4_FIBIN_H
#define PROJECT4_FIBIN_H
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
    template <typename ValueType, typename List>
    using eval = Value<ValueType,
                ValueType(Fib<I-1>::template eval<ValueType,
                    List>::val + Fib<I-2>::template eval<ValueType, List>::val)>;
};

template<>
struct Fib<0> {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, 0>;
};

template<>
struct Fib<1> {
    template <typename ValueType, typename List>
    using eval = Value<ValueType, 1>;
};

template <bool B>
struct Bool {
    constexpr static bool val = B;
};

struct True {
    template <typename ValueType, typename List>
    using eval = Bool<true>;
};

struct False{
    template <typename ValueType, typename List>
    using eval = Bool<false>;
};

template <typename V>
struct Lit {
    template <typename ValueType, typename List>
    using eval = typename V::template eval<ValueType, List>;
};

/**************************************************************************/
/** EQ **/
/**************************************************************************/

template <typename T1, typename T2>
struct CheckEq{
    template <typename ValueType, typename List>
    using eval = typename False::template eval<ValueType, List>;
};

template <typename T>
struct CheckEq<T, T>{
    template <typename ValueType, typename List>
    using eval = typename True::template eval<ValueType, List>;
};

template <typename E1, typename E2>
struct Eq {
    template <typename ValueType, typename List>
    using eval = typename CheckEq<typename E1::template eval<ValueType, List>,
            typename E2::template eval<ValueType, List>>::template eval<ValueType, List>;
};

/**************************************************************************/
/** IF **/
/**************************************************************************/

template <typename T, typename Then, typename Else>
struct CheckIf {
    template <typename ValueType, typename List>
    using eval = typename Else::template eval<ValueType, List>;
};

template <typename Then, typename Else>
struct CheckIf<Bool<true>, Then, Else> {
    template <typename ValueType, typename List>
    using eval = typename Then::template eval<ValueType, List>;
};

template <typename C, typename Then, typename Else>
struct If {
    template <typename ValueType, typename List>
    using eval = typename CheckIf<typename C::template eval<ValueType, List>, Then, Else>::template eval<ValueType, List>;
};


/**************************************************************************/
/** LET **/
/**************************************************************************/

template <uint32_t var, typename V, typename E>
struct Let {
    template <typename ValueType, typename List>
    using eval = typename E::template eval<ValueType, List>; // TODO zrobić liste i dodać do niej var tutaj
};



#endif //PROJECT4_FIBIN_H
