#include <utility>
#include <cstring>
#include <iostream>

class rule_of_five
{
    char* cstring; // raw pointer used as a handle to a dynamically-allocated memory block
public:
    rule_of_five(const char* s = "") : cstring(nullptr)
    { 
        std::cout << "Constructor called!\n";
        if (s)
        {
            std::size_t n = std::strlen(s) + 1;
            cstring = new char[n];      // allocate
            std::memcpy(cstring, s, n); // populate 
        } 
    }
 
    ~rule_of_five()
    {
        std::cout << "Destructor called!\n";
        delete[] cstring; // deallocate
    }
 
    rule_of_five(const rule_of_five& other) // copy constructor
    : rule_of_five(other.cstring) {
        std::cout << "Copy constructor called!\n";
    }
 
    rule_of_five(rule_of_five&& other) noexcept // move constructor
    : cstring(std::exchange(other.cstring, nullptr)) {
        std::cout << "Move constructor called!\n";
    }
 
    rule_of_five& operator=(const rule_of_five& other) // copy assignment
    {
        std::cout << "Copy assignment operator called!\n";
        return *this = rule_of_five(other); // why does this call move assignment operator?
    }
 
    rule_of_five& operator=(rule_of_five&& other) noexcept // move assignment
    {
        std::cout << "Move assignment operator called!\n";
        std::swap(cstring, other.cstring);
        return *this;
    }
 
// alternatively, replace both assignment operators with 
//  rule_of_five& operator=(rule_of_five other) noexcept
//  {
//      std::swap(cstring, other.cstring);
//      return *this;
//  }
};

rule_of_five foo(){
    std::cout << "foo begin\n";
    rule_of_five rof{"rof_foo"};
    std::cout << "foo end\n";
    return rof;
}

int main(){
    rule_of_five rof1{"rof1"};
    rule_of_five rof2{"rof2"};
    rule_of_five rof3;
    rof3 = rof2;
    std::cout << "61\n";
    rof3 = std::move(rof2);
    std::cout << "63\n";
    rule_of_five rof4{std::move(rof1)};
    std::cout << "65\n";
    auto rof5 = foo();      // nothing gets called! Because Copy Elision!
    std::cout << "66\n";
    rof5 = foo();       // but it does here! 
}