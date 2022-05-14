#include <iostream>
#include <string>
#include <cstring>
#include <cassert>

#include "my_vector.hpp"

namespace product
{

    class Product
    {
    public:
        Product(const char *name = "without name", int quantity = 0, double price = 0) : name_((char *)name), quantity_(quantity), price_(price){};
        explicit Product(const Product &other) : Product(other.name_, other.quantity_, other.price_){};
        friend std::ostream &operator<<(std::ostream &os, const Product &prod);
        bool operator==(const Product &other) const
        {
            if (quantity_ != other.quantity_)
                return false;
            if (price_ != other.price_)
                return false;
            if (std::strcmp(name_, other.name_))
                return false;
            return true;
        }
        bool operator!=(const Product &other) const
        {
            return !(*this == other);
        }

    private:
        char *name_;
        int quantity_;
        double price_;
    };

    std::ostream &operator<<(std::ostream &os, const Product &prod)
    {
        return os << prod.name_ << " " << prod.quantity_ << " " << prod.price_;
    }

    void print(const containers::my_vector<Product> &v)
    {
        std::cout << v << std::endl;
    }
}
using namespace product;

class Test
{
public:
    Test() = delete;
    Test(int) {}
    Test(const Test &) = default;
};

class Test1
{
public:
    Test1() : size_(0), data_(nullptr){};
    explicit Test1(int size) : size_(size)
    {
        if (!size)
            Test1();
        else
        {
            data_ = new int[size_];
            for (int i = 0; i < size_; ++i)
                data_[i] = i;
        }
    }

    ~Test1()
    {
        if (size_)
            delete[] data_;
    }

    Test1(const Test1 &other)
    {
        if (other.size_ == 0)
        {
            size_ = 0;
            data_ = nullptr;
        }
        else
        {
            size_ = other.size_;
            data_ = new int[size_];
            for (int i = 0; i < size_; ++i)
                data_[i] = other.data_[i];
        }
    }
    int get_size()
    {
        return size_;
    }
    friend std::ostream &operator<<(std::ostream &os, const Test1 &t);

private:
    int size_;
    int *data_;
};


std::ostream &operator<<(std::ostream &os, const Test &t)
{
    (void)t;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Test1 &t)
{
    os << t.size_ << " ";
    return os;
}

template <typename T>
void test_my_vector(const T &x, const T &y)
{
    containers::my_vector<T> v;
    v.push_back(x);
    v.push_back(y);
    containers::my_vector<T> v1(v);
    assert(v == v1);
    assert(v.size() == 2);
    v.pop_back();
    assert(v.size() == 1);
    assert(!v.empty());
    v.clear();
    assert(v.empty());
    v = v1;
    assert(v == v1);
    std::cout << "test_my_vector finished" << std::endl;
}


template <typename T>
void test_my_vector_default_constrictable(const T &x, const T &y)
{
    test_my_vector<T>(x, y);
    containers::my_vector<T> v1(7);
    containers::my_vector<T> v2(5);
    assert(v1.size() == 7);
    assert(v2.size() == 5);
    assert(v1 != v2);
    v2.resize(4);
    v1.resize(4);
    assert(v1 == v2);
    std::cout << "test_my_vector_dafault_constrictable finished" << std::endl;
}


void test_test_class()
{
    containers::my_vector<Test> vec;
    vec.push_back(Test(10));
    vec.push_back(Test(20));
    vec.push_back(Test(40));
    vec.clear();
    std::cout << "test_tes_class finished" << std::endl;
}



void test_push_back()
{
    containers::my_vector<Test1> vec;
    for (int i = 0; i < 100; ++i)
        vec.push_back(Test1(i + 1));
    vec.resize(10);
    std::cout << vec << std::endl;
    containers ::my_vector<Test1> vec1(vec);
    for (int i = 0; i < 9998; ++i)
        vec.pop_back();
    vec.reserve(20);
    for (int i = vec.size(); i <= 20; ++i)
        vec.push_back(Test1(6));
    vec.clear();
    std::cout << vec1 << std::endl;
    std::cout << "test_push_back finished" << std::endl;
}

void test_test_class1()
{
    containers::my_vector<Test1> vec(100);
    vec.clear();
    std::cout << "test_test_class1 finished" << std::endl;
}

void test_index_operator()
{
    containers::my_vector<Test1> vec;
    for (int i = 0; i < 10; ++i)
        vec.push_back(Test1(i));
    for (int i = 0; i < 10; ++i)
        assert(vec[i].get_size() == i);
    std::cout << "test_index_operator finished" << std::endl;
}

void init_test()
{
    containers::my_vector<int> v_int;
    containers::my_vector<char> v_char;
    containers::my_vector<std::string> v_str;
    containers::my_vector<Product> v_prod;
    containers::my_vector<Test> v_test;
    containers::my_vector<Test1> v_test1;
    containers::my_vector<int> v_int_(5);
    containers::my_vector<char> v_char_(5);
    containers::my_vector<std::string> v_str_(5);
    containers::my_vector<Product> v_prod_(5);
    containers::my_vector<Test1> v_test1_(5);
    containers::my_vector<int> v_int_c(v_int);
    containers::my_vector<char> v_char_c(v_char);
    containers::my_vector<std::string> v_str_c(v_str);
    containers::my_vector<Product> v_prod_c(v_prod);
    containers::my_vector<Test> v_test_c(v_test);
    containers::my_vector<Test1> v_test1_c(v_test1);
    containers::my_vector<int> v_int_c2(v_int_);
    containers::my_vector<char> v_char_c2(v_char_);
    containers::my_vector<std::string> v_str_c2(v_str_);
    containers::my_vector<Product> v_prod_c2(v_prod_);
    containers::my_vector<Test1> v_test1_c2(v_test1_);
    std::cout << "init_test accepted" << std::endl;
}

int main() {
	containers::my_vector<int> v;
	v.push_back(2);
	const int n{3};
	v.push_back(n);
	std::cout << v << std::endl;
    init_test();
    std::cout << "Hello world" << std::endl;
    test_my_vector<int>(5, 10);
    test_my_vector<char>('a', 'b');
    test_my_vector<double>(2.3453, 23.424);
    test_my_vector_default_constrictable<Product>(Product("asdf", 4, 12.0), Product("qwe", -1, 7.5));
    test_test_class();
    test_test_class1();
    test_push_back();
    test_index_operator();
    return 0;
}
