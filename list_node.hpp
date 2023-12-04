#ifndef LISTNODE_HPP
#define LISTNODE_HPP


#include "xmemory.hpp"

_MSL_BEGIN
// [x] _List_node
//       [x] _List_node()
//       [x] _List_node (const _List_node&) = delete
//       [x] _List_node& operator= (const _List_node&) = delete
//       [x] _Buy_head_node // construct head node
//       [x] _Freenode0 // destroy pointer members and deallocate
//       [x] _Freenode
//       [x] operator<<


template <
	class _Value_type
>
struct _List_node { // list node
    using value_type      = _Value_type;
    using _Nodeptr        = _List_node<_Value_type>*;


    template <class _Arg_t>
    explicit _List_node (
    	_Arg_t&& Value,
    	_Nodeptr Prev = nullptr,
    	_Nodeptr Next = nullptr
    ) noexcept :
    	_Myval(std::forward<_Arg_t>(Value)), // need type conversion from _Arg_t to value_type
    	_Prev(Prev),
    	_Next(Next)
    {}

    _List_node() = default;
    _List_node (const _List_node&) = delete;
    _List_node& operator= (const _List_node&) = delete;

    static _Nodeptr _Buy_head_node() {
        auto Result = new _List_node();
        Result->_Next = Result; Result->_Prev = Result;
        return Result;
    }

    static void _Freenode0 (_Nodeptr Ptr) noexcept {
        // destroy pointer members
        Ptr->_Next = nullptr; Ptr->_Prev = nullptr;
        delete Ptr; Ptr = nullptr;
    }

    static void _Freenode (_Nodeptr Ptr) noexcept {
        // destroy all members in Ptr
        Ptr->_Myval.~_Value_type();
        _Freenode0(Ptr);
    }

    static void _Free_non_head (_Nodeptr Head) noexcept { 
        // free a list starting at Head and terminate at nullptr
        Head->_Prev->_Next = nullptr;
        auto Pnode = Head->_Next;
        for (_Nodeptr Pnext; Pnode; Pnode = Pnext) {
            Pnext = Pnode->_Next;
            _Freenode(Pnode);
        }
    }


    template <class Ty_>
    friend std::ostream& operator<< (std::ostream& _Output, const _List_node<Ty_>& Node);


    _Value_type _Myval; // the stored value, unused if head
    _Nodeptr    _Prev;  // successor node, or first element if head
    _Nodeptr    _Next;  // the stored value, unused if head
};

template <
    class _Value_type
>
std::ostream& operator<< (std::ostream& _Output, const _List_node<_Value_type>& Node) {
    _Output << Node;
    return _Output;
};

_MSL_END
#endif // LISTNODE_HPP
