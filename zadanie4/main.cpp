#include "fibin.h"
#include <cstdint>
#include <iostream>

int main() {
    static_assert(Fibin<uint32_t>::eval<Let<Var("A"), Lit<Fib<1>>, Ref<Var("A")>>>() == 1);


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


    static_assert(Fibin<uint64_t>::eval<Eq<If<Lit<True>, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<1>>>>());
    static_assert(Fibin<uint64_t>::eval<Eq<If<Lit<False>, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<0>>>>());

    // Błędy kompilacji:
    /*static_assert(Fibin<uint64_t>::eval<Eq<If<False, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<0>>>>());
    static_assert(Fibin<uint64_t>::eval<Eq<If<True, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<1>>>>());
    static_assert(Fibin<uint64_t>::eval<Eq<If<Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<0>>>, Lit<Fib<1>>>>());
*/
    static_assert((int)Fibin<char>::eval<Sum<Lit<Fib<11>>,Lit<Fib<11>>,Lit<Fib<11>>,Lit<Fib<11>>,Lit<Fib<11>>>>() == -67);

    
    /*
    // Testing: lambda(x) {x + (Fib(1) + Fib(10)) + Fib(2)}(Fib(3))
    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
    static_assert(59 == Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"),
                  Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>>>, Lit<Fib<3>>>>());

    // Testing: if False then Fib(0) else Fib(1)
    static_assert(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());

    // Testing: let z = Fib(0) in {z + Fib(1)}
    static_assert(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());

    // Prints out to std::cout: "Fibin doesn't support: PKc"
    Fibin<const char*>::eval<Lit<Fib<0>>>();

    std::cout << "Fibin works fine!" << std::endl;
    */
}
