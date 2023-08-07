#include <iostream>


///////////////////////////////////////////////////////////////////////

template<typename T>
void foo(T x){} // add = delete to check deduced type in compiler error
// foo("string"); // will be deduced as const char* 

template<typename T>
std::pair<T, int> goo(T& x){
    
}
// goo("string") // will be deduced as const char [7]

////////////////////////////////////////////////////////////////////////



int main(){ 
    goo("string");
}