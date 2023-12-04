#include <iostream>
#include <list>
#include "list.cpp"


using msl::list_v2;
// using msl::_List_node;

// comment

template< 
    template <class... > class _Container_t,
    class _Value_t
>
void print(const _Container_t<_Value_t>& Cont) {
    auto It = Cont.begin(); std::cout << "\n" << "{ ";
    while (It != Cont.end()) {
        std::cout << *(It++) << " ";
    }
    std::cout << "};" << "\n";
}


int main() {

    // default Ctor
    //std::list<int> exmpl;
    list_v2<int> mylist;
    print(mylist);

    // list_v2 (size_type)
    list_v2<int> mylist2(4);
    print(mylist2);

    // list_v2 (size_type, const _Ty&)
    int var = 42;
    //std::list<int> exmpl(11, var);
    list_v2<int> mylist3(11, var);
    print(mylist3);

    // operator= (list_v&&)
    // mylist2 = list_v2<int>(5, var + 1);
    // print(mylist2);

    // push_front (_Ty&&)
    mylist.push_front(1);
    print(mylist);

    // push_front (const _Ty&)
    mylist.push_front(var);
    print(mylist);

    // push_back (_Ty&&);
    mylist.push_back(9);
    print(mylist);

    // push_back (const _Ty&)
    mylist.push_back(var);
    print(mylist);

    // emplace_fornt (_Ty&&)
    //mylist.emplace_front(8);
    //mylist.emplace_back(7);
    //print(mylist);

    // print(mylist2);
    // mylist.insert(++mylist.begin(), mylist2.begin(), mylist2.end());
    // print(mylist);

    // std::list<int> list1(4);
    // print(list1);
    //
    // std::list<int> list_main;
    // list_main.insert(++list_main.begin(), list1.begin(), list1.end());
    // print(list_main);

    mylist.erase(++mylist.begin());
    print(mylist);

    std::cout << "END";
    return 0;
}
