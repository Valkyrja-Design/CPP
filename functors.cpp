#include <iostream>
#include <set>

struct data{
    int x;
    int y;
};

class Functor{  
    public:
        bool operator()(const data& first, const data& second) const {
            if (first.x != second.x) return first.x < second.x;
            return first.y > second.y;
        }
};

int main(){

    Functor func{};
    auto comp = [](const data& first, const data& second){
        if (first.x != second.x) return first.x < second.x;
        return first.y > second.y;
    };
    std::set<data, decltype(comp)> s1(comp);    // need to pass lambda as constructor 
                                                // arg because otherwise lambda will be 
                                                // constructed in the constructor but
                                                // lambda doesn't have a default constructor
    // or 
    std::set<data, Functor> s2;

    for (int i=0;i<2;i++){
        for (int j=2;j>0;j--){
            s1.insert({i, j});
            s2.insert({i, j});
        }
    }
    
    std::cout << "Example 1\n";
    for (const auto& d : s1) std::cout << "(" << d.x << ", " << d.y << "), ";
    
    std::cout << "\nExample 2\n";
    for (const auto& d : s2) std::cout << "(" << d.x << ", " << d.y << "), ";

}