#include <cstddef>
#include <cstdlib>
#include <utility>
#include <iostream>

template<typename T>
class vector{
    public:
        vector(int n, T m = T{});

        void push_back(const T&);
        void push_back(T&&);

        void pop_back();

        std::size_t size() const {
            return sz;
        }

        std::size_t capacity() const {
            return cap;
        }

        void print() const {
            for (int i=0;i<sz;++i){
                std::cout << *(p + i) << ' ';
            }
            std::cout << '\n';
        }

        ~vector();

    private:
        T* p;
        std::size_t cap;
        std::size_t sz;
};

template<typename T>
vector<T>::vector(int n, T m)
    : sz(n), cap(n)
{
    // allocating storage
    p = static_cast<T*>(std::malloc(n * sizeof(T)));
    
    for (int i=0;i<n;++i){
        new(p + i) T{m};
    }
}

template<typename T>
vector<T>::~vector(){
    for (int i=0;i<sz;++i){
        (p + i)->~T();
    }

    std::free(p);
}

template<typename T>
void vector<T>::push_back(const T& x){
    if (sz == cap){
        cap *= 2;
        p = static_cast<T*>(std::realloc(p, cap * sizeof(T)));
    }

    new(p + sz) T{x};
    ++sz;
}

template<typename T>
void vector<T>::push_back(T&& x){
    if (sz == cap){
        cap *= 2;
        p = static_cast<T*>(std::realloc(p, cap * sizeof(T)));
    }

    new(p + sz) T{std::move(x)};
    ++sz;
}

template<typename T>
void vector<T>::pop_back(){
    --sz;
    (p + sz)->~T();

    if (2 * sz < cap){
        cap /= 2;
        p = static_cast<T*>(std::realloc(p, cap * sizeof(T)));
    }
}

int main(){
    vector<int> v1{2};
    vector<int> v2{4, 2};

    v1.print();
    v2.print();

    v1.push_back(2);
    v1.push_back(515);
    v1.push_back(151);

    std::cout << v1.size() << ' ' << v1.capacity() << '\n';
    v1.print();
    std::cout << v1.size() << ' ' << v1.capacity() << '\n';

    v1.pop_back();
    v1.pop_back();
    v1.pop_back();
    
    v1.print();
    std::cout << v1.size() << ' ' << v1.capacity() << '\n';
}