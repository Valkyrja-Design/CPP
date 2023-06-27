#include <iostream>
#include <array>
 
struct S
{
    static const int c;
};
 
const int d = 10 * S::c; // not a constant expression: S::c has no preceding
                         // initializer, this initialization happens after const
const int S::c = 5;      // constant initialization, guaranteed to happen first
 
int main()
{
    std::cout << "d = " << d << '\n';
    std::array<int, S::c> a1; // OK: S::c is a constant expression
//  std::array<int, d> a2;    // error: d is not a constant expression
}