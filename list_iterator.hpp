#ifndef LISTITERATOR_HPP
#define LISTITERATOR_HPP


#include "list_node.hpp"


_MSL_BEGIN
// [x] _List_unchecked_const_iterator
//          [x] operator*
//          [x] operator->
//          [x] operator bool
//          [x] operator++
//          [x] operator++(int)
//          [x] operator--
//          [x] opeartor--(int)
//          [x] operator==
//          [x] operator!=
// [x] _List_unchecked_iterator
//          [x] operator*
//          [x] operator->
//          [x] operator bool
//          [x] operator++
//          [x] operator++(int)
//          [x] operator--
//          [x] opeartor--(int)
// [x] _List_const_iterator
//          [x] operator*
//          [x] operator->
//          [x] operator bool
//          [x] operator++
//          [x] operator++(int)
//          [x] operator--
//          [x] opeartor--(int)
//          [x] operator==
//          [x] operator!=
//          [x] _Verify_range
//          [x] _Unwrapped
//          [-] _Seek_to
// [x] _List_iterator
//          [x] operator*
//          [x] operator->
//          [x] operator bool
//          [x] operator++
//          [x] operator++(int)
//          [x] operator--
//          [x] opeartor--(int)
//          [x] _Unwrapped


template <
    class _Mylist,
    class _Base = _Iterator_base
>
class _List_unchecked_const_iterator : public _Base {
public:
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mylist::_Nodeptr;
    using value_type      = typename _Mylist::value_type;
    using difference_type = typename _Mylist::difference_type;
    using pointer         = typename _Mylist::const_pointer;
    using reference       = const value_type&;

    _List_unchecked_const_iterator() noexcept : _Myptr() {}

    _List_unchecked_const_iterator (_Nodeptr Parent_node, const _Mylist* Parent_list) noexcept : _Myptr(Parent_node) {
        _Base::_Adopt_by_cont(Parent_list);
    }

    reference operator*() const noexcept {
        return _Myptr->_Myval;
    }

    pointer operator->() const noexcept { // TODO: impl
        return _Myptr;
    }

    explicit operator bool() const noexcept { // type conversation iterator to bool
                                              // _Myptr == nullptr ? true : false;
        return _Myptr == nullptr;
    }

    _List_unchecked_const_iterator& operator++() noexcept {
        _Myptr = _Myptr->_Next;
        return *this;
    }

    _List_unchecked_const_iterator operator++(int) noexcept {
        _List_unchecked_const_iterator Temp = *this;
        _Myptr = _Myptr->_Next;
        return Temp;
    }

    _List_unchecked_const_iterator& operator--() noexcept {
        _Myptr = _Myptr->_Prev;
        return *this;
    }

    _List_unchecked_const_iterator operator--(int) noexcept {
        _List_unchecked_const_iterator Temp = *this;
        _Myptr = _Myptr->_Prev;
        return Temp;
    }

    bool operator== (const _List_unchecked_const_iterator& Rhs) const noexcept {
        return _Myptr == Rhs._Myptr;
    }

    bool operator!= (const _List_unchecked_const_iterator& Rhs) const noexcept {
        return _Myptr != Rhs._Myptr;
    }


    _Nodeptr _Myptr; // pointer to node
};


template <
    class _Mylist
>
class _List_unchecked_iterator : public _List_unchecked_const_iterator<_Mylist> {
public:
    using _Mybase = _List_unchecked_const_iterator<_Mylist>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mylist::_Nodeptr;
    using value_type      = typename _Mylist::value_type;
    using difference_type = typename _Mylist::difference_type;
    using pointer         = typename _Mylist::pointer;
    using reference       = value_type&;

    using _Mybase::_Mybase; // TODO: how it works?

    reference operator*() const noexcept {
        return const_cast<reference>(_Mybase::operator*);
    }

    pointer operator->() const noexcept {
        return _Mybase::_Myptr;
    }

    explicit operator bool() const noexcept {
        return _Mybase::operator bool();
    }

    _List_unchecked_iterator& operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    _List_unchecked_iterator operator++(int) noexcept {
        _List_unchecked_iterator Temp = *this;
        _Mybase::operator++();
        return Temp;
    }

    _List_unchecked_iterator& operator--() noexcept {
        _Mybase::operator++();
        return *this;
    }

    _List_unchecked_iterator operator--(int) noexcept {
        _List_unchecked_iterator Temp = *this;
        _Mybase::operator++();
        return Temp;
    }
};


template <
    class _Mylist
>
class _List_const_iterator : public _List_unchecked_const_iterator<_Mylist> {
public:
    using _Mybase = _List_unchecked_const_iterator<_Mylist>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mylist::_Nodeptr;
    using value_type      = typename _Mylist::value_type;
    using difference_type = typename _Mylist::difference_type;
    using pointer         = typename _Mylist::const_pointer;
    using reference       = const value_type&;

    using _Mybase::_Mybase;

    reference operator*() const noexcept {
        // this->_Getcont return const _Container_base*
        // static_cast convert const pointer to parent class _Container_base
        // to const pointer to one of the derived classes like _List_val in our
        // case, so, in other words, there is a conversation from parent type
        // class pointer to derived type class pointer
        // (*) nullptr has type std::nullptr_t, which convertible to any
        // raw pointer type
        const _Mylist* Mycont = static_cast<const _Mylist*>(this->_Mybase::_Getcont());
        _MSL_VERIFY_f(Mycont, "cannot dereference value-initialized list iterator");
        _MSL_VERIFY_f(this->_Myptr != Mycont->_Myhead, "cannot dereference end list iterator");

        return _Mybase::_Myptr->_Myval;
    }

    pointer operator->() const noexcept {
        return _Mybase::_Myptr;
    }

    explicit operator bool() const noexcept {
        const auto Mycont = static_cast<const _Mylist*>(this->_Getcont());
        return (this->_Myptr == Mycont->_Myhead) ? false : this->_Myptr;
    }

    _List_const_iterator& operator++() noexcept {
        const auto Mycont = static_cast<const _Mylist*>(this->_Getcont());
        _MSL_VERIFY_f(Mycont, "cannot increment value_initialized list iterator");
        _MSL_VERIFY_f(this->_Myptr != Mycont->_Myhead, "cannot increment end list iterator");

        this->_Myptr = this->_Myptr->_Next;
        return *this;
    }

    _List_const_iterator operator++(int) noexcept {
        _List_const_iterator Temp = *this;
        ++(*this);
        return Temp;
    }

     _List_const_iterator& operator--() noexcept {
        const auto New_ptr = this->_Myptr->_Prev;
        const auto Mycont = static_cast<const _Mylist*>(this->_Getcont());
        _MSL_VERIFY_f(Mycont, "cannot decrement value_initialized list iterator");
        _MSL_VERIFY_f(New_ptr != Mycont->_Myhead, "cannot decrement begin list iterator");

        this->_Myptr = New_ptr;
        return *this;
    }

    _List_const_iterator operator--(int) noexcept {
        _List_const_iterator Temp = *this;
        --*this;
        return Temp;
    }

    bool operator== (const _List_const_iterator& Rhs) const noexcept {
        _MSL_VERIFY_f(this->_Getcont() == Rhs._Getcont(), "list iterators incompatible");
        return this->_Myptr == Rhs._Myptr;
    }

    bool operator!= (const _List_const_iterator& Rhs) const noexcept {
        return !(*this == Rhs);
    }

    friend void _Verify_range (const _List_const_iterator& First, const _List_const_iterator& Last) noexcept {
        _MSL_VERIFY_f(First->_Getcont() == Last->_Getcont(), "list iterators in range are from different containers");
    }

    _List_unchecked_const_iterator<_Mylist> _Unwrapped() const noexcept { // make other unchecked iterator base on this
        return _List_unchecked_const_iterator<_Mylist>(this->_Myptr, static_cast<const _Mylist*>(this->_Getcont()));
    }

    // void _Seek_to (const _List_unchecked_const_iterator<_Mylist> It) noexcept {
    //     this->_Myptr = It._Myptr;
    // }
};


template <
    class _Mylist
>
class _List_iterator : public _List_const_iterator<_Mylist> {
public:
    using _Mybase = _List_const_iterator<_Mylist>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mylist::_Nodeptr;
    using value_type      = typename _Mylist::value_type;
    using difference_type = typename _Mylist::difference_type;
    using pointer         = typename _Mylist::pointer;
    using reference       = value_type&;

    using _Mybase::_Mybase;

    reference operator*() const noexcept {
        return const_cast<reference>(_Mybase::operator*());
    }  

    pointer operator->() const noexcept { // TODO: i'm not sure, probably its wrong
        return _Mybase::_Myptr;
    }

    _List_iterator& operator++() noexcept {
        _Mybase::operator++(); 
        return *this;
    }

    _List_iterator operator++(int) noexcept {
        _List_iterator Temp = *this;
        _Mybase::operator++();
        return Temp;
    }

    _List_iterator& operator--() noexcept {
        _Mybase::operator--(); 
        return *this;
    }

    _List_iterator operator--(int) noexcept {
        _List_iterator Temp = *this;
        _Mybase::operator--();
        return Temp;
    }

    _List_unchecked_iterator<_Mylist> _Unwrapped() const noexcept {
        return _List_unchecked_iterator<_Mylist>(this->_Myptr, static_cast<const _Mylist*>(this->_Getcont()));
    }
};

_MSL_END
#endif // LISTITERATOR_HPP
