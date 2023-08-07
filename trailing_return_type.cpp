
class A{
    using ll = long long;
    ll foo();
};

// ll A::foo(){} // doesn't work

// need to specify return type as trailing if 
// we want to use the alias from A
auto A::foo() -> ll {}

// also useful for recursive lambdas 
auto dfs = [](int u, const auto& dfs) -> void {};

// and template return type 
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

int main(){
    auto x = add(1.0, 1);   // double
    auto y = add(1, 1);     // int
}