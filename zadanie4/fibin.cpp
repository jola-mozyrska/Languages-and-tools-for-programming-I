#include<iostream>

#include "fibin.h"
//using namespace std;

template<class T>
class Fibin {
////Klasa Fibin udostępnia publiczną metodę szablonową:
////
////* ValueType eval<Expr> - metoda dostępna tylko wtedy, gdy ValueType jest typem liczbowym.
////Oblicza w trakcie kompilacji wartość wyrażenia Expr i zwraca tę wartość.
////
////* void eval<Expr> - metoda dostępna tylko wtedy, gdy ValueType NIE jest typem liczbowym.
////Wyświetla informację, że Fibin nie wspiera danego typu.
//
////public:
////    template<class T> void sort(Array<T>& v) { /*...*/ };
////    template<> void sort<char*>(Array<char*>&) {};
////
////    void eval<Expr> (){};
////    T eval<Expr>//liczbowy
//
};

//a friend of Fibin?

template<class T>
        class Fib {
        //  jak tu specjalizowac
        };

template<class T>
class Lit{};

template<>
class Lit <std::string> {

};

template<>
class Lit <Fib<int>> {

};



//template <class T>
//class mycontainer {
//    T element;
//public:
//    mycontainer (T arg) {element=arg;}
//    T increase () {return ++element;}
//};
//
//// class template specialization:
//template <>
//class mycontainer <char> {
//    char element;
//public:
//    mycontainer (char arg) {element=arg;}
//    char uppercase ()
//    {
//        if ((element>='a')&&(element<='z'))
//            element+='A'-'a';
//        return element;
//    }
//};

///////////////////////////////////

//template <class T>
//void foo() {
//    typename T::iterator * iter;
//    ...
//}

////////////////////////////////

int main() {

}

