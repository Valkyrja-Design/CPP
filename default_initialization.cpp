#include <iostream>
#include <string>

/* Default initialization of non-class variables with automatic and dynamic storage duration produces objects with indeterminate values */
/* References and const scalar objects cannot be default-initialized */

int foo(int b){
    int x;               // OK: the value of x is indeterminate (garbage)
    int y = x;           // undefined behavior
    unsigned char c;     // OK: the value of c is indeterminate
    unsigned char d = c; // OK: the value of d is indeterminate but behavior is not undefined
    int e = d;           // undefined behavior
    return b ? d : 0;    // undefined behavior if b is true
}

struct T1 { int mem; };
 
struct T2
{
    int mem;
    T2(); // "mem" is not in the initializer list
};
 
T2 :: T2() = default;

int n; // static non-class, a two-phase initialization is done:
       // 1) zero initialization initializes n to zero
       // 2) default initialization does nothing, leaving n being zero

int main(){
    int n;            // non-class, the value is indeterminate
    std::string s;    // class, calls default ctor, the value is "" (empty string)
    std::string a[2]; // array, default-initializes the elements, the value is {"", ""}
//  int& r;           // error: a reference
//  const int n;      // error: a const non-class
//  const T1 t1;      // error: const class with implicit default ctor
    T1 t1;            // class, calls implicit default ctor (t1.mem is default-initialized (to indeterminate value))
    const T2 t2;      // const class, calls the user-provided default ctor
                      // t2.mem is default-initialized (to indeterminate value)
    int *x;           // indeterminate
    
}