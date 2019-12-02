#ifndef PROJECT4_FIBIN_H
#define PROJECT4_FIBIN_H

template<typename T1>
class Fibin {
    // static List

    template<typename T2>


    template <typename T>
    constexpr int eval() {}

    template <Lit<typename T>>
    constexpr int eval() {

    }

    struct Var {
        const char *name;
        Var(const char *N): name(N) {}
    };

    template <Ref<typename T>>
    constexpr int eval() {}

    template <Let<Var V, typename Val, typename E>>
    constexpr int eval() {
        return Let<V::name, Val, E>.fun();
        //Let<V::name, Val, E, Environment>.fun();
    }
};

template <const char* N, typename Val, typename E>//, List Env>
struct Let {
    constexpr int fun() {return 1;}

    // Env.append(
};

/*
class VarClass {
    const char* name;
    auto val;

    VarClass(const char* name): name(name) {}

    template<typename T>
    void set(T t) const{
        val = t;
    }
}*/


template <const char *N>
struct Ref {
    template <typename T>
    struct {
        static T val;
    };
/*
    constexpr static int get() const {return val;}

    constexpr static void set(T v) {val = v;}
*/
};

template <int I, typename T t, typename E>
class Let {
    constexpr static E fun() {
        E<Ref<I, T>::val = t > result;
    }
}

#endif //PROJECT4_FIBIN_H
