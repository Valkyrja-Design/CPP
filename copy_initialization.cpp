#include <memory>
#include <string>
#include <utility>
#include <iostream>
 
struct A
{
    operator int() { return 12;}
};
 
struct B
{
    B(int) {}
};
 
int main()
{
    std::string s = "test";        // OK: constructor is non-explicit
    std::string s2 = std::move(s); // this copy-initialization performs a move
 
//  std::unique_ptr<int> p = new int(1); // error: constructor is explicit
    std::unique_ptr<int> p(new int(1));  // OK: direct-initialization
 
    int n = 3.14;    // floating-integral conversion
    const int b = n; // const doesn't matter
    int c = b;       // ...either way
 
    A a;
    B b0 = 12;
//  B b1 = a;       // < error: conversion from 'A' to non-scalar type 'B' requested
    B b2{a};        // < identical, calling A::operator int(), then B::B(int)
    B b3 = {a};     // <
    auto b4 = B{a}; // <
 
//  b0 = a;         // < error, assignment operator overload needed
 
    int x = a;      // conversion function called
    std::cout << x << '\n';

    [](...){}(c, b0, b3, b4); // pretend these variables are used
}