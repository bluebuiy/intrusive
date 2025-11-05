
#include "llist.h"
#include <vector>
#include <iostream>
#include <memory>

struct intrusive_int
{
    intrusive::list_inject<intrusive_int> list1;

    int i = 0;

    intrusive_int(int i)
        : i(i)
    {

    }

};

using list_memb_chain = intrusive::member_chain<
    intrusive::member_pointer<intrusive_int, intrusive::list_inject<intrusive_int>, &intrusive_int::list1>
>;


std::ostream& operator<<(std::ostream& out, intrusive_int const & v)
{
    out << v.i;
    return out;
}

std::unique_ptr<intrusive_int> make_value(int i)
{
    return std::make_unique<intrusive_int>(i);
}

template <typename T, typename MembChain>
void print_list(intrusive::list<T, MembChain> & list)
{
    T* current = list.head;
    while (current != nullptr)
    {
        std::cout << *current << std::endl;
        current = list.resolve(current)->right;
    }
    std::cout << std::endl;
}

template <typename T, typename MembChain>
void print_list_back(intrusive::list<T, MembChain> & list)
{
    T* current = list.tail;
    while (current != nullptr)
    {
        std::cout << *current << std::endl;
        current = list.resolve(current)->left;
    }
    std::cout << std::endl;
}

void test_insert_after()
{

    intrusive::list<intrusive_int, list_memb_chain> list;

    std::vector<std::unique_ptr<intrusive_int>> elements;

    for (int i = 0; i < 7; ++i)
    {
        elements.emplace_back(make_value(i));
        list.push_back(elements[i].get());
    }

    auto val = make_value(10);
    list.insert_after(elements[3].get(), val.get());

    print_list(list);
    std::cout << std::endl;
    print_list_back(list);

}



void test_insert_before()
{
    intrusive::list<intrusive_int, list_memb_chain> list;

    std::vector<std::unique_ptr<intrusive_int>> elements;

    for (int i = 0; i < 7; ++i)
    {
        elements.emplace_back(make_value(i));
        list.push_back(elements[i].get());
    }

    auto val = make_value(10);
    list.insert_before(elements[3].get(), val.get());

    print_list(list);
    std::cout << std::endl;
    print_list_back(list);

}

void test_stuff()
{
    std::cout << "push_back" << std::endl;
    intrusive::list<intrusive_int, list_memb_chain> list;

    std::vector<std::unique_ptr<intrusive_int>> elements;
    for (int i = 0; i < 10; ++i)
    {
        elements.emplace_back(make_value(i));
        list.push_back(elements[i].get());
    }

    print_list(list);
    std::cout << std::endl;
    print_list_back(list);

    std::cout << "Remove test:" << std::endl;
    
    list.remove(elements[4].get());
    elements.erase(elements.begin() + 4);


    print_list(list);
    std::cout << std::endl;
    print_list_back(list);

    for (int i = 0; i < elements.size(); ++i)
    {
        list.remove(elements[i].get());
    }

    print_list(list);
    std::cout << std::endl;
    print_list_back(list);

    std::cout << list.head << " " << list.tail << std::endl;
}

int main(void)
{
    test_stuff();
    test_insert_after();
    test_insert_before();
}



