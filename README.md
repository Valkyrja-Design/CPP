# Initialization

- [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)
- [Aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization)
- [Value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)
- [List initialization](https://en.cppreference.com/w/cpp/language/list_initialization)
- [Zero initialization](https://en.cppreference.com/w/cpp/language/zero_initialization)
- [Copy initialization](https://en.cppreference.com/w/cpp/language/copy_initialization)

# Rule of five/three/zero

- [cppreference](https://en.cppreference.com/w/cpp/language/rule_of_three)

# Constant folding

Optimization performed by the compiler where it works out the result of an expression at compile time and directly inserts that into the generated code. For example:
```
#define ONE 1
#define TWO 2 

int main(){
    int x = (TWO ? 10 : 5) + ONE + TWO;  // will directly result in 13
}
```
- [constant-folding](https://devblogs.microsoft.com/cppblog/optimizing-c-code-constant-folding/)

# As-if Rule

Basically the compiler can perform any optimization as long as the *observable behavior* of the program doesn't change. For example:
```
int& preinc(int& n) { return ++n; }
int add(int n, int m) { return n+m; }
 
// volatile input to prevent [constant folding](#constant-folding)
volatile int input = 7;
 
// volatile output to make the result a visible side-effect
volatile int result;
 
int main()
{
    int n = input;
// using built-in operators would invoke undefined behavior
//    int m = ++n + ++n;
// but using functions makes sure the code executes as-if 
// the functions were not overlapped
    int m = add(preinc(n), preinc(n));
    result = m;
}
```
Here, the compiler can remove all calls to ```preinc``` and ```add``` can be eliminated to just the single expression  
```
result = 2*input + 3;
```

- [cppreference](https://en.cppreference.com/w/cpp/language/as_if)
- [stackoverflow](https://stackoverflow.com/questions/15718262/what-exactly-is-the-as-if-rule)

# Aggregates

- [stackoverflow](https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/7189821#7189821)
