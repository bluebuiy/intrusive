
#define INTRUSIVE_EXPOSE_INTERNALS
#include "heap.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

struct HeapItem
{
    int i = 0;

    intrusive::heap_inject<HeapItem> hd;
};

bool operator<(HeapItem const & a, HeapItem const & b)
{
    return a.i < b.i;
}


template <typename T, typename Chain>
void print_tree(intrusive::heap<T, Chain>& heap)
{
    std::vector<T*> queue;
    queue.push_back(heap.peek());
    unsigned i = 1;
    while (!queue.empty())
    {
        if (std::popcount(i+1) == 1)
        {
            std::cout << std::endl;
        }
        T* n = queue.front();
        queue.erase(queue.begin());
        if (n)
        {
            std::cout << n->i << "  ";
            queue.push_back(Chain::resolve(n)->c1);
            queue.push_back(Chain::resolve(n)->c0);
            i += 2;
        }
    }
}

template <typename T, typename MembChain>
void print_node(T* n)
{
    if (n == nullptr)
    {
        std::cout << "{ NULL }" << std::endl;
        return;
    }
    auto* c0 = MembChain::resolve(n)->c0;
    auto* c1 = MembChain::resolve(n)->c1;
    auto* p = MembChain::resolve(n)->parent;
    if (p)
    {
        std::cout << "{" << p->i << "} -> ";
    }
    else
    {
        std::cout << "{ NULL } -> ";
    }
    std::cout << n->i << ": {";
    if (c0)
    {
        std::cout << c0->i;
    }
    else
    {
        std::cout << "NULL";
    }
    std::cout << ", ";
    if (c1)
    {
        std::cout << c1->i;
    }
    else
    {
        std::cout << "NULL";
    }
    std::cout << "}" << std::endl;
}

template <typename T, typename Chain>
void print_tree_details(intrusive::heap<T, Chain>& heap)
{
    std::vector<T*> queue;
    queue.push_back(heap.peek());
    std::cout << "r   ";
    print_node<T, Chain>(queue.front());

    unsigned c = 1;
    while (!queue.empty())
    {
        if (std::popcount(c + 1) == 1)
        {
            std::cout << std::endl;
        }
        ++c;
        T* n = queue.front();
        queue.erase(queue.begin());
        if (n)
        {
            queue.push_back(Chain::resolve(n)->c0);
            queue.push_back(Chain::resolve(n)->c1);

            std::cout << "0   ";
            if (Chain::resolve(n)->c0) { print_node<T, Chain>(Chain::resolve(n)->c0); } else { std::cout << "NULL" << std::endl; }
            std::cout << "1   ";
            if (Chain::resolve(n)->c1) { print_node<T, Chain>(Chain::resolve(n)->c1); } else { std::cout << "NULL" << std::endl; }
            ++c;
        }
    }
    std::cout << std::endl;
}

int main(void)
{

    intrusive::heap<HeapItem,
        intrusive::member_chain<
            intrusive::member_pointer<HeapItem, decltype(HeapItem::hd), &HeapItem::hd>
        >
    > heap;

    std::vector<HeapItem> items;

    for (int i = 0; i < 200; ++i)
    {
        items.push_back({(i * 43) % 15});
        //items.push_back({ i });
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(items.begin(), items.end(), g);

    for (int i = 0; i < items.size(); ++i)
    {
        heap.insert(&items[i]);
        //print_tree_details(heap);
    }

    print_tree_details(heap);

    //heap.swap(
    //    heap.resolve(heap.resolve(heap.root)->c0)->c0,
    //    //heap.resolve(heap.root)->c0
    //    heap.root
    //);
    
    //print_tree_details(heap);

    while (heap.peek())
    {
        HeapItem* item = heap.pop();
        std::cout << item->i << std::endl;

        //print_tree_details(heap);
    }

}


