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

// int&& badrvalref(int x){       // bad since x is local 
//     return std::move(x);        
// }

int&& goodrvalref(int&& x){
    return std::move(x);
}

int main(){ 
    goo("string");
    
    // int&& y = badrvalref(5);
    int&& x = goodrvalref(5);   // lifetime of 5 is extended till x goes out of scope

}