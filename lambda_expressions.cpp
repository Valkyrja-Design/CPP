#include <iostream>

void foo(){}

int global_var = 2;

int main(){
    int x = 5;
    auto f1 = [=](){
        // x = 25;                     // error: lambda call operator is const
    };

    auto f2 = [=]() mutable {
        x = 25;                        // works now
    };

    auto f3 = [&](auto& f){
        // return 1;                   // works if this 
        // f(f);                       // error: calling f before deduction
                                       // of return type (sort of like using
                                       // incomplete object)        
        if (x == 2) return x + 1;
        else return x + 25;
    };

    f3(f3); 

    static int static_int = 24;
    constexpr int cexpr = 25;
    const int cint1 = 25;
    const int cint2 = x;

    auto f4 = [](){         
        global_var = 3;                 // can use static storage duration variables
        static_int = 51;
        std::cout << cexpr << '\n';     // can read constexpr variables 
        std::cout << cint1 << '\n';     // and const int/enum type initialized with constant expr
        // std::cout << cint2 << '\n';  // but not otherwise
        foo();
    };

    auto answer = [](int n){            // constexpr implicitly in C++17 and later
        // std::cout << "Invoked\n";
        return 32 + n;
    };

    constexpr int response = answer(10);
    auto response_ = answer(x);

    std::cout << response << ' ' << response_ << '\n';
}