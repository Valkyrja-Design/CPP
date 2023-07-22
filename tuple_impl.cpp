// Source: https://stackoverflow.com/questions/4041447/how-is-stdtuple-implemented

#include <iostream>

// Contains the actual value for one item in the tuple. The 
// template parameter `i` allows the
// `Get` function to find the value in O(1) time
template<std::size_t i, typename Item>
struct TupleLeaf {
    Item value;
};

// TupleImpl is a proxy for the final class that has an extra 
// template parameter `i`.
template<std::size_t i, typename... Items>
struct TupleImpl;

// Base case: empty tuple
template<std::size_t i>
struct TupleImpl<i>{};

// Recursive specialization
template<std::size_t i, typename HeadItem, typename... TailItems>
struct TupleImpl<i, HeadItem, TailItems...> :
    public TupleLeaf<i, HeadItem>, // This adds a `value` member of type HeadItem
    public TupleImpl<i + 1, TailItems...> // This recurses
    {};

// Obtain a reference to i-th item in a tuple
template<std::size_t i, typename HeadItem, typename... TailItems>
HeadItem& Get(TupleImpl<i, HeadItem, TailItems...>& tuple) {
    // Fully qualified name for the member, to find the right one 
    // (they are all called `value`).
    return tuple.TupleLeaf<i, HeadItem>::value;
}

// Templated alias to avoid having to specify `i = 0`
template<typename... Items>
using Tuple = TupleImpl<0, Items...>;

int main(int argc, char** argv) {
    Tuple<int, float, std::string> tuple;
    Get<0>(tuple) = 5;
    Get<1>(tuple) = 8.3;
    Get<2>(tuple) = "Foo";
    std::cout << Get<0>(tuple) << std::endl;
    std::cout << Get<1>(tuple) << std::endl;
    std::cout << Get<2>(tuple) << std::endl;
    return 0;
}