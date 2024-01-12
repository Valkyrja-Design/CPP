// Source: https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/64490578#64490578

#include <string_view>
#include <iostream>

template <typename T>
constexpr std::string_view type_name();

template <>
constexpr std::string_view type_name<void>(){
    return "void";
}

namespace detail
{

    using type_name_prober = void;

    template <typename T>
    constexpr std::string_view wrapped_type_name(){
        
#ifdef __clang__
        return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
        return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
        return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
    }

    constexpr std::size_t wrapped_type_name_prefix_length(){
        return wrapped_type_name<type_name_prober>().find(type_name<type_name_prober>());
    }

    constexpr std::size_t wrapped_type_name_suffix_length(){
        return wrapped_type_name<type_name_prober>().length() - wrapped_type_name_prefix_length() - type_name<type_name_prober>().length();
    }

} // namespace detail

template <typename T>
constexpr std::string_view type_name(){
    constexpr auto wrapped_name = detail::wrapped_type_name<T>();
    constexpr auto prefix_length = detail::wrapped_type_name_prefix_length();
    constexpr auto suffix_length = detail::wrapped_type_name_suffix_length();
    constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;
    return wrapped_name.substr(prefix_length, type_name_length);
}

template<typename T, T v>
void func(){
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

struct A{
    int x;
    static int y;
};

void foo(){}

A a;
const A* ptr;

struct B{
    static int x;
    int y;
    int& lvalref = x;
    int&& rvalref = std::move(x);
    int& otherlvalref = y;
    int&& otherrvalref = 5;
};

int B::x = 5;
B goo(){ return B{}; }

int main(){
    constexpr int x = 2;
    func<int, x>();
    
    std::cout << type_name<decltype(ptr->x)>() << '\n'
              << type_name<decltype((ptr->x))>() << '\n'
              << type_name<decltype(B::lvalref)>() << '\n'  
              << type_name<decltype((B::lvalref))>() << '\n'
              << type_name<decltype(B::rvalref)>() << '\n'
              << type_name<decltype((B::rvalref))>() << '\n'
              << type_name<decltype(B::otherlvalref)>() << '\n'  
              << type_name<decltype((B::otherlvalref))>() << '\n'
              << type_name<decltype(B::otherrvalref)>() << '\n'
              << type_name<decltype((B::otherrvalref))>() << '\n'
              << type_name<decltype(B{}.lvalref)>() << '\n'
              << type_name<decltype((B{}.lvalref))>() << '\n'
              << type_name<decltype(B{}.rvalref)>() << '\n'
              << type_name<decltype((B{}.rvalref))>() << '\n'
              << type_name<decltype(B{}.otherlvalref)>() << '\n'
              << type_name<decltype((B{}.otherlvalref))>() << '\n'
              << type_name<decltype(B{}.otherrvalref)>() << '\n'
              << type_name<decltype((B{}.otherrvalref))>() << '\n'
              << type_name<decltype((B{}.x))>() << '\n'
              << type_name<decltype((B{}.y))>() << '\n'
              << type_name<decltype(goo().lvalref)>() << '\n'
              << type_name<decltype((goo().lvalref))>() << '\n'
              << type_name<decltype(goo().rvalref)>() << '\n'
              << type_name<decltype((goo().lvalref))>() << '\n'
              << type_name<decltype((goo().x))>() << '\n'
              << type_name<decltype((goo().y))>() << '\n';
}