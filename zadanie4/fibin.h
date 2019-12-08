#ifndef FIBIN_FIBIN_H
#define FIBIN_FIBIN_H

#include <iostream>

namespace details {

    // Struktury możliwych przyjmowanych wartości

    template<typename T, T i>
    struct Value {
        constexpr static T val = i;
    };

    template<unsigned I, typename ValueType>
    struct _Fib {
        static constexpr ValueType val = static_cast<ValueType>(_Fib<I - 1, ValueType>::val +
                                                                _Fib<I - 2, ValueType>::val);
    };

    template<typename ValueType>
    struct _Fib<0, ValueType> {
        static constexpr ValueType val = static_cast<ValueType>(0);
    };

    template<typename ValueType>
    struct _Fib<1, ValueType> {
        static constexpr ValueType val = static_cast<ValueType>(1);
    };

    template<unsigned I>
    struct Fib {
        template<typename ValueType>
        using eval = Value<ValueType, _Fib<I, ValueType>::val>;
    };

    template<bool B>
    struct Bool {
        constexpr static bool val = B;
    };

    struct True {
        template<typename ValueType>
        using eval = Bool<true>;
    };

    struct False {
        template<typename ValueType>
        using eval = Bool<false>;
    };

    template<typename V>
    struct Lit {
        template<typename ValueType, typename List>
        using eval = typename V::template eval<ValueType>;
    };


    // Zmienne, do których istnieje dostęp pamiętane są w liście,
    // tworzy ona środowisko, w którym wykonują się funkcje.
    struct EmptyList {
    };

    template<uint64_t I, typename V, typename Tail=EmptyList>
    struct ListEnvironment {
        using tail = Tail;
    };

    // Struktura, która rekurencyjnie szuka w liście wartości o danym kluczu
    template<uint64_t I, typename List>
    struct Find {
        using result = typename Find<I, typename List::tail>::result;
    };

    template<uint64_t I, typename Value, typename Tail>
    struct Find<I, ListEnvironment<I, Value, Tail>> {
        using result =  Value;
    };

    // Funkcja hashująca dany ciąg znaków
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
            if (temp == 0 || i >= 6)
                return 0;
            hash += static_cast<uint64_t>(temp);
            ++i;
        }

        return hash;
    }

    template<uint64_t I>
    struct Ref {
        template<typename ValueType, typename List>
        using eval = typename Find<I, List>::result;
    };

    template<uint64_t var, typename Val, typename Expr>
    struct Let {
        static_assert(var > 0);

        template<typename ValueType, typename List>
        using eval = typename Expr::template eval<ValueType, ListEnvironment<var, typename Val::template eval<ValueType, List>, List>>;
    };

    // Operacje arytmetyczne
    template<typename T>
    struct Inc1 {
        template<typename ValueType, typename List>
        using eval = Value<ValueType, static_cast<ValueType>(T::template eval<ValueType, List>::val +
                                                             Fib<1>::template eval<ValueType>::val)>;
    };

    template<typename T>
    struct Inc10 {
        template<typename ValueType, typename List>
        using eval = Value<ValueType, static_cast<ValueType>(T::template eval<ValueType, List>::val +
                                                             Fib<10>::template eval<ValueType>::val)>;
    };

    template<typename T1, typename T2, typename... T3>
    struct Sum {
        template<typename ValueType, typename List>
        using eval = Value<ValueType, static_cast<ValueType>(T1::template eval<ValueType, List>::val +
                                                             Sum<T2, T3...>::template eval<ValueType, List>::val)>;
    };

    template<typename T1, typename T2>
    struct Sum<T1, T2> {
        template<typename ValueType, typename List>
        using eval = Value<ValueType, static_cast<ValueType>(T1::template eval<ValueType, List>::val +
                                                             T2::template eval<ValueType, List>::val)>;
    };

/**************************************************************************/
/** EQ **/
/**************************************************************************/

    template<typename T1, typename T2>
    struct _Eq {
        template<typename ValueType, typename List>
        using eval = Bool<false>;
    };

    template<typename T>
    struct _Eq<T, T> {
        template<typename ValueType, typename List>
        using eval = Bool<true>;
    };

    template<typename Left, typename Right>
    struct Eq {
        template<typename ValueType, typename List>
        using eval = typename _Eq<typename Left::template eval<ValueType, List>,
                typename Right::template eval<ValueType, List>>::template eval<ValueType, List>;
    };

    template<bool B, typename Then, typename Else>
    struct _If {
        template<typename ValueType, typename List>
        using eval = typename Else::template eval<ValueType, List>;
    };

    template<typename Then, typename Else>
    struct _If<true, Then, Else> {
        template<typename ValueType, typename List>
        using eval = typename Then::template eval<ValueType, List>;
    };

    template<typename Condition, typename Then, typename Else>
    struct If {
        template<typename ValueType, typename List>
        using eval = typename _If<Condition::template eval<ValueType, List>::val, Then, Else>::template eval<ValueType, List>;
    };

    template<uint64_t I, typename Body, typename List>
    struct _Lambda {
    };

    template<uint64_t I, typename Body>
    struct Lambda {
        template<typename ValueType, typename List>
        using eval = _Lambda<I, Body, List>;
    };

    template<typename Fun, typename Param>
    struct Invoke {
        template<typename ValueType, typename List>
        using eval = typename Invoke<typename Fun::template eval<ValueType, List>, Param>::template eval<ValueType, List>;
    };

    template<uint64_t I, typename Body, typename L, typename Param>
    struct Invoke<_Lambda<I, Body, L>, Param> {
        static_assert(I > 0);
        template<typename ValueType, typename List>
        using eval = typename Body::template eval<ValueType, ListEnvironment<I, typename Param::template eval<ValueType, List>, L>>;
    };
}


template<typename T>
struct Fibin {
    template <typename E, typename X = T, std::enable_if_t<std::is_integral<X>::value, int> = 0>
    static constexpr T eval() {
        return E::template eval<T, details::EmptyList>::val;
    }

    template <typename E, typename X = T, std::enable_if_t<!std::is_integral<X>::value, int> = 0>
    static constexpr void eval() {
        std::cout << "Fibin doesn't support: " << typeid(T).name() << std::endl;
    }
};

#endif //FIBIN_FIBIN_H
