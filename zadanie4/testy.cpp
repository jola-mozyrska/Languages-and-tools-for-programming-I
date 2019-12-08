#include <iostream>
#include <array>
#include <type_traits>
#include "fibin.h"

template<unsigned n>
using L = Lit<Fib<n>>;
using FB = Fibin<int>;

void test_fib() {

    static_assert(0 == Fibin<int>::eval<L<0>>());
    static_assert(1 == Fibin<int>::eval<L<1>>());
    static_assert(1 == Fibin<int>::eval<L<2>>());
    static_assert(2 == Fibin<int>::eval<L<3>>());
    static_assert(3 == Fibin<int>::eval<L<4>>());
    static_assert(5 == Fibin<int>::eval<L<5>>());
    static_assert(8 == Fibin<int>::eval<L<6>>());
    static_assert(13 == Fibin<int>::eval<L<7>>());
    static_assert(21 == Fibin<int>::eval<L<8>>());
    static_assert(34 == Fibin<int>::eval<L<9>>());
    static_assert(55 == Fibin<int>::eval<L<10>>());
    static_assert(89 == Fibin<int>::eval<L<11>>());
    static_assert(144 == Fibin<int>::eval<L<12>>());
    static_assert(233 == Fibin<int>::eval<L<13>>());
    static_assert(377 == Fibin<int>::eval<L<14>>());
    static_assert(610 == Fibin<int>::eval<L<15>>());
    static_assert(987 == Fibin<int>::eval<L<16>>());
    static_assert(1597 == Fibin<int>::eval<L<17>>());
    static_assert(2584 == Fibin<int>::eval<L<18>>());
    static_assert(4181 == Fibin<int>::eval<L<19>>());
    static_assert(6765 == Fibin<int>::eval<L<20>>());
    static_assert(10946 == Fibin<int>::eval<L<21>>());
    static_assert(17711 == Fibin<int>::eval<L<22>>());
    static_assert(28657 == Fibin<int>::eval<L<23>>());
    static_assert(46368 == Fibin<int>::eval<L<24>>());
    static_assert(75025 == Fibin<int>::eval<L<25>>());
    static_assert(121393 == Fibin<int>::eval<L<26>>());
    static_assert(196418 == Fibin<int>::eval<L<27>>());
    static_assert(317811 == Fibin<int>::eval<L<28>>());
    static_assert(514229 == Fibin<int>::eval<L<29>>());


    static_assert(0 == Fibin<uint8_t>::eval<L<0>>());
    static_assert(1 == Fibin<uint8_t>::eval<L<1>>());
    static_assert(1 == Fibin<uint8_t>::eval<L<2>>());
    static_assert(2 == Fibin<uint8_t>::eval<L<3>>());
    static_assert(3 == Fibin<uint8_t>::eval<L<4>>());
    static_assert(5 == Fibin<uint8_t>::eval<L<5>>());
    static_assert(8 == Fibin<uint8_t>::eval<L<6>>());
    static_assert(13 == Fibin<uint8_t>::eval<L<7>>());
    static_assert(21 == Fibin<uint8_t>::eval<L<8>>());
    static_assert(34 == Fibin<uint8_t>::eval<L<9>>());
    static_assert(55 == Fibin<uint8_t>::eval<L<10>>());
    static_assert(89 == Fibin<uint8_t>::eval<L<11>>());
    static_assert(144 == Fibin<uint8_t>::eval<L<12>>());
    static_assert(233 == Fibin<uint8_t>::eval<L<13>>());
    static_assert(121 == Fibin<uint8_t>::eval<L<14>>());
    static_assert(98 == Fibin<uint8_t>::eval<L<15>>());
    static_assert(219 == Fibin<uint8_t>::eval<L<16>>());
    static_assert(61 == Fibin<uint8_t>::eval<L<17>>());
    static_assert(24 == Fibin<uint8_t>::eval<L<18>>());
    static_assert(85 == Fibin<uint8_t>::eval<L<19>>());
    static_assert(109 == Fibin<uint8_t>::eval<L<20>>());
    static_assert(194 == Fibin<uint8_t>::eval<L<21>>());
    static_assert(47 == Fibin<uint8_t>::eval<L<22>>());
    static_assert(241 == Fibin<uint8_t>::eval<L<23>>());
    static_assert(32 == Fibin<uint8_t>::eval<L<24>>());
    static_assert(17 == Fibin<uint8_t>::eval<L<25>>());
    static_assert(49 == Fibin<uint8_t>::eval<L<26>>());
    static_assert(66 == Fibin<uint8_t>::eval<L<27>>());
    static_assert(115 == Fibin<uint8_t>::eval<L<28>>());
    static_assert(181 == Fibin<uint8_t>::eval<L<29>>());
}

template<unsigned n>struct E{using _ = Sum<L<n%10>, typename E<n/10>::_>;};
template<>struct E<0> {using _ = L<0>;};

constexpr std::array<unsigned, 55> v {0,1,3,4,14,5,15,35,6,16,36,46,146,7,17,37,47,147,57,157,357,8,18,38,48,148,58,158,358,68,168,368,468,1468,9,19,39,49,149,59,159,359,69,169,369,469,1469,79,179,379,479,1479,579,1579,3579};

template<uint64_t n>struct W {using _ = Inc10<typename W<n-55>::_>;};

#define M(i) template<>struct W<(i)>{using _ = typename E<v[(i)]>::_;};
M(0);M(1);M(2);M(3);M(4);M(5);M(6);M(7);M(8);M(9);M(10);M(11);M(12);M(13);M(14);M(15);M(16);M(17);M(18);M(19);M(20);M(21);M(22);M(23);M(24);M(25);M(26);M(27);M(28);M(29);M(30);M(31);M(32);M(33);M(34);M(35);M(36);M(37);M(38);M(39);M(40);M(41);M(42);M(43);M(44);M(45);M(46);M(47);M(48);M(49);M(50);M(51);M(52);M(53);M(54);

template<uint64_t n>
using t = typename W<n>::_;

template<typename Fn, typename ...Ts>
struct InvokeVariadic {};

template<typename Fn, typename T, typename ...Ts>
struct InvokeVariadic<Fn, T, Ts...> {
    using _ = typename InvokeVariadic<Invoke<Fn, T>, Ts...>::_;
};

template<typename Fn>
struct InvokeVariadic<Fn> {
    using _ = Fn;
};

using Ycombinator =
Lambda<
        Var("f"),
        Invoke<
                Lambda<
                        Var("x"),
                        Invoke<
                                Ref<Var("x")>,
                                Ref<Var("x")>
                        >
                >,
                Lambda<
                        Var("x"),
                        Invoke<
                                Ref<Var("f")>,
                                Lambda<
                                        Var("args"),
                                        Invoke<
                                                Invoke<
                                                        Ref<Var("x")>,
                                                        Ref<Var("x")>
                                                >,
                                                Ref<Var("args")>>>>>>>;
/*
Invoke<
        Lambda<88,
                Invoke<
                        Ref<70>,
                        Lambda<6,
                                Invoke<Invoke<Ref<88>,
                                        Ref<88>
                                        >,
                                        Ref<6>
                                        >,
                                        EmptyList>
                                        >,
                        EmptyList>,
        Ref<88> >
*/
/*

Invoke<Invoke<
        Lambda<70,
                Invoke<
                        Lambda<88, Invoke<Ref<88>, Ref<88> >, EmptyList>,
                        Lambda<88, Invoke<Ref<70>, Lambda<65827183, Invoke<Invoke<Ref<88>, Ref<88> >, Ref<65827183> >, EmptyList> >, EmptyList> >, EmptyList>, Lambda<70, Lambda<78, If<Eq<Sum<Lit<Fib<4> >, Lit<Fib<0> >>, Ref<78> >, Ref<78>, Sum<Ref<78>, Invoke<Ref<70>, Sum<Ref<78>, Lit<Fib<3> >> >> >, EmptyList>, EmptyList> >,
        Lit<Fib<1> > >
*/
template<typename F, typename ...Args>
using Exec = typename InvokeVariadic<Ycombinator, F, Args...>::_;

template<uint64_t n>
using SquareN =
Lambda<
        Var("f"),
        Lambda<
                Var("n"),
                If<
                        Eq<
                                typename W<n*2-1>::_,
                                Ref<Var("n")>>,
                        Ref<Var("n")>,
                        Sum<
                                Ref<Var("n")>,
                                Invoke<
                                        Ref<Var("f")>,
                                        Sum<
                                                Ref<Var("n")>,
                                                Lit<Fib<3>>>>>>>>;




template <typename M, typename N>
using Multiply =
Let<
        Var("n"),
        N,
        Let<
                Var("m"),
                M,
                Lambda<
                        Var("f"),
                        Lambda<
                                Var("i"),
                                If<
                                        Eq<
                                                Ref<Var("i")>,
                                                Ref<Var("m")>>,
                                        Lit<Fib<0>>,
                                        Sum<
                                                Ref<Var("n")>,
                                                Invoke<
                                                        Ref<Var("f")>,
                                                        Inc1<
                                                                Ref<Var("i")>>>>>>>>>;


template <typename M, typename N>
using Modulo =
Let<
        Var("m"),
        M,
        Let<
                Var("n"),
                N,
                Lambda<
                        Var("f"),
                        Lambda<
                                Var("mod"),
                                Lambda<
                                        Var("cnt"),
                                        If<
                                                Eq<
                                                        Ref<Var("cnt")>,
                                                        Ref<Var("m")>>,
                                                Ref<Var("mod")>,
                                                InvokeVariadic<
                                                        Ref<Var("f")>,
                                                        If<
                                                                Eq<
                                                                        Inc1<Ref<Var("mod")>>,
                                                                        Ref<Var("n")>>,
                                                                Lit<Fib<0>>,
                                                                Inc1<Ref<Var("mod")>>>,
                                                        Inc1<Ref<Var("cnt")>>>::_>>>>>>;



int main(){
    cerr << Fibin<uint64_t>::eval<Lit<Fib<30>>>() << endl;
    static_assert(Fibin<int>::eval<Example<ID>>() == 89);
    static_assert(Fibin<int>::eval<Example<InfiniteLoop>>() == 89);


    static_assert(Fibin<uint64_t>::eval<Lit<Fib<12>>>() == 144);
    static_assert(Fibin<uint16_t>::eval<Lit<Fib<25>>>() == 9489);
    static_assert(Fibin<bool>::eval<Lit<True>>);

    static_assert(Fibin<uint8_t>::eval<Lit<Fib<12>>>() == 144);


    static_assert(Fibin<uint32_t>::eval<Inc1<Lit<Fib<3>>>>() == 3);
    static_assert(Fibin<uint32_t>::eval<Inc10<Lit<Fib<3>>>>() == 57);
    static_assert(Fibin<uint32_t>::eval<Sum<Lit<Fib<0>>, Lit<Fib<1>>, Inc10<Lit<Fib<3>>>>>() == 58);

    static_assert(Fibin<uint32_t>::eval<Let<Var("A"), Lit<Fib<1>>, Ref<Var("A")>>>() == 1);
    static_assert(Fibin<uint32_t>::eval<Let<Var("A"), Let<Var("A"), Lit<Fib<3>>, Ref<Var("A")>>, Ref<Var("A")>>>() == 2);

    static_assert(Fibin<uint64_t>::eval<Eq<Lit<Fib<0>>, Lit<Fib<1>>>>() == 0);
    static_assert(Fibin<uint64_t>::eval<Eq<Lit<Fib<0>>, Lit<True>>>() == 0);
    static_assert(Fibin<uint64_t>::eval<Eq<Lit<Fib<0>>, Lit<False>>>() == 0);
    static_assert(Fibin<uint64_t>::eval<Eq<Lit<Fib<1>>, Lit<Fib<2>>>>() == 1);
    static_assert(Fibin<uint64_t>::eval<Eq<Lit<Fib<15>>, Lit<Fib<2>>>>() == 0);

    static_assert(Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"), Ref<Var("x")>>, Lit<Fib<0>>>>() == 0);
    static_assert(Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"), Ref<Var("x")>>, Lit<Fib<3>>>>() == 2);

//    cout << Fibin<uint32_t>::eval<Invoke<Lambda<Var("x"), Ref<Var("x")>>, Ref<Var("x")>>>();

    static_assert(Fibin<uint64_t>::eval<Eq<If<Lit<True>, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<1>>>>());
    static_assert(Fibin<uint64_t>::eval<Eq<If<Lit<False>, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<0>>>>());

    // Błędy kompilacji:
    /*static_assert(Fibin<uint64_t>::eval<Eq<If<False, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<0>>>>());
    static_assert(Fibin<uint64_t>::eval<Eq<If<True, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<1>>>>());
    static_assert(Fibin<uint64_t>::eval<Eq<If<Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<1>>>>());
*/
    //static_assert((int)Fibin<char>::eval<Sum<Lit<Fib<11>>,Lit<Fib<11>>,Lit<Fib<11>>,Lit<Fib<11>>,Lit<Fib<11>>>>() == -67);

    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
    static_assert(59 == Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"),
            Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>>>, Lit<Fib<3>>>>());
    // Testing: if False then Fib(0) else Fib(1)
    static_assert(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());
    // Testing: let z = Fib(0) in {z + Fib(1)}
    static_assert(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());

    test_fib();

    static_assert(FB::eval<If<Lit<False>, Lit<Fib<10>>, Lit<Fib<11>>>>() == 89);
    static_assert(FB::eval<If< Eq<L<1>, L<2>>, L<10>, L<11> >>() == 55);
    static_assert(FB::eval<If< Eq<L<1>, L<3>>, L<10>, L<11> >>() == 89);
    static_assert(Fibin<uint8_t>::eval<If< Eq<L<32>, L<5>>, L<10>, L<11> >>() == 55);


    static_assert(FB::eval<If<If<Lit<False>, Lit<True>, Lit<False>>, Lit<Fib<2>>, Lit<Fib<3>>>>() == 2);
    static_assert(FB::eval<Sum<Lit<Fib<6>>, Lit<Fib<3>>>>() == 10);
    static_assert(FB::eval<Sum<L<6>, L<3>>>() == 10);
    static_assert(FB::eval<Sum<L<6>, L<3>, L<3>>>() == 12);
    static_assert(FB::eval<Sum<L<6>, L<3>, L<3>, L<6>>>() == 20);
    static_assert(FB::eval<Sum<L<4>, L<4>>>() == 6);
    static_assert(FB::eval<Inc1<L<4>>>() == 4);
    static_assert(FB::eval<Inc10<L<4>>>() == 58);


    //using Variable as bool
    static_assert(5 == Fibin<int16_t>::eval< Let<Var("true"), Lit<True>,
            If<Ref<Var("true")>, L<5>, L<3> > >
    >());

    //summing two Variables
    static_assert(57 == Fibin<int16_t>::eval<Let<Var("A"), L<10>,
            Let<Var("asffss"), L<3>, Sum<Ref<Var("A")>, Ref<Var("asffss")>>>>>());



    // Testing: if False then Fib(0) else Fib(1)
    static_assert(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());

    // Testing: let z = Fib(0) in {z + Fib(1)}
    static_assert(1 == Fibin<int16_t>::eval< Let<Var("z"), L<0>, Inc1<Ref<Var("Z")>> >>());


    // Testing: let z = Fib(0) in {z + Fib(1)}
    static_assert(1 == Fibin<int16_t>::eval< Let<Var("z"), L<0>, Inc1<Ref<Var("Z")>> >>());

    using Scoping =
    Let<
            Var("const"),
            Lit<Fib<9>>,
            Let<
                    Var("f"),
                    Lambda<
                            Var("x"),
                            Sum<
                                    Ref<Var("const")>,
                                    Ref<Var("x")>
                            >
                    >,
                    Let<
                            Var("const"),
                            Lit<Fib<10>>,
                            Invoke<
                                    Ref<Var("f")>,
                                    Lit<Fib<0>>
                            >
                    >
            >
    >;

    static_assert(34 == FB::eval<Scoping>());

    static_assert(2 == FB::eval<Invoke<Let<Var("x"), Lit<Fib<1>>, Lambda<Var("x"), Ref<Var("x")> > >, Lit<Fib<3>> > >());

    using lambdy =
    Let<
            Var("f"),
            Lit<Fib<3>>,
            Invoke<
                    Invoke <
                            Lambda<
                                    Var("x"),
                                    Lambda<
                                            Var("s"),
                                            Sum<Ref<Var("f")>, Ref<Var("s")>, Ref<Var("x")>>
                                    >
                            >,
                            Lit<Fib<4>>
                    >,
                    Lit<Fib<5>>
            >
    >;

    static_assert(FB::eval<lambdy>() == 10);

    using tescik =
    Let<
            Var("const"),
            Lit<Fib<1>>,
            Let<
                    Var("f"),
                    Lambda<
                            Var("x"),
                            Sum<
                                    Ref<Var("const")>,
                                    Ref<Var("x")
                                    >
                            >>,
                            Let<
                                    Var("const"),
                                    Lit<Fib<3>>,
                                    Invoke<
                                            Ref<Var("f")>,
                                            Lit<Fib<0>>
                                    >
                            >
                    >
            >;

    static_assert(FB::eval<tescik>() == 1);

    using FunctionComparison =
    Let<
            Var("f"),
            Lambda<
                    Var("x"),
                    Ref<Var("x")>
            >,
            If<
                    Eq<
                            Ref<Var("f")>,
                            Ref<Var("F")>
                    >,
                    L<10>,
                    L<11>
            >
    >;
    static_assert(55 == FB::eval<FunctionComparison>());

    using zero = t<0>;
    using pietnascie = t<15>;
    using dwadziesciajeden = t<21>;
    using tysionce = t<10000>;
    static_assert(
            FB::eval<tysionce>() == 10000);

    //std::cout << FB::eval<Exec<SquareN<3>, Lit<Fib<1>> >>();

    static_assert(
            FB::eval<Exec<SquareN<6>, Lit<Fib<1>> >>() == 6*6);

    static_assert(
            FB::eval<Exec<SquareN<45>, Lit<Fib<1>> >>() == 45*45);

    static_assert(
            FB::eval<Exec<Multiply<dwadziesciajeden, Lit<Fib<4>>>, zero>>() == 21*3);

    static_assert(
            FB::eval<Exec<Modulo<dwadziesciajeden, pietnascie>, zero, zero>>() == 6);

    static_assert(
            FB::eval<Exec<Modulo<L<10>, t<11>>, zero, zero>>() == 0);
            
  // Ten test się wywala na głębokości rekurencji
//static_assert(
      //      FB::eval<Exec<Modulo<L<18>, t<11>>, zero, zero>>() == 10);

    // Prints out to std::cout: "Fibin doesn't support: PKc"
    Fibin<const char*>::eval<Lit<Fib<0>>>();
    Fibin<Multiply<dwadziesciajeden, pietnascie>>::eval<Lit<Fib<0>>>();
    Fibin<Multiply<Multiply<dwadziesciajeden, pietnascie>, dwadziesciajeden>>::eval<Lit<Fib<0>>>();//*/
}
