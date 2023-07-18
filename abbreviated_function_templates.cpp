#include <iostream>

// this is 
void foo(auto x, auto y){
    std::cout << "Unspecialized\n";
}

// // same as this
// template<typename T, typename U>
// int foo(T x, U y){

// }

// can also specialize abbreviated function templates
// just like normal function templates
template<>
void foo(int x, double y){
    std::cout << "Specialized\n";
}

int main(){
    foo(2, 2);
    foo(2, 52.2);
}