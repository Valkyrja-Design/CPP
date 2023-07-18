#include <iostream>

int main(){
    int x = 5;
    auto f1 = [=](){
        // x = 25;     // error: lambda call operator is const
                       
    };

    auto f2 = [=]() mutable {
        x = 25;         // works now
    };

    auto f3 = [&](auto& f){
        // return 1;        // works if this 
        // f(f);               // error: calling f before deduction
                            // of return type (sort of like using
                            // incomplete object)        
        if (x == 2) return x + 1;
        else return x + 25;
    };

    f3(f3); 

    auto answer = [](int n){    // constexpr implicitly in C++17 and later
        // std::cout << "Invoked\n";
        return 32 + n;
    };

    constexpr int response = answer(10);
    auto response_ = answer(x);

    std::cout << response << ' ' << response_ << '\n';
}