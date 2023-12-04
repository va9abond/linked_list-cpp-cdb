#include "listNode.cpp"



_AXC_BEGIN

template <
    class Mylist_,
    class Base_ = _Iterator_base
>
class _List_unchecked_const_iterator : public Base_ {
public: 
    using iterator_category = std::bidirectional_iterator_tag; 

    using _Nodeptr        = typename Mylist_::_Nodeptr;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&;



    _List_unchecked_const_iterator() noexcept : _Ptr() {}

    _List_unchecked_const_iterator (_Nodeptr Pnode, const Mylist_* Plist) noexcept : 
        _Ptr(Pnode)
    {
        this->_Adopt(Plist);
    }

    reference operator*() const noexcept {
        return _Ptr->_Myval;
    }

    pointer operator->() const noexcept { 
        return this;
    }

    explicit operator bool() const {
        return _Ptr; // is nullptr
    }

    _List_unchecked_const_iterator& operator++ () noexcept {
        _Ptr = _Ptr->_Next; 
        return *this;
    }

    _List_unchecked_const_iterator operator++(int) noexcept {
        auto tmp = *this;
        _Ptr     = _Ptr->_Next;
        return tmp;
    }

    _List_unchecked_const_iterator& operator--() noexcept {
        _Ptr = _Ptr->_Prev;
        return *this;
    }

    _List_unchecked_const_iterator operator--(int) noexcept {
        auto tmp = *this;
        _Ptr     = _Ptr->_Prev;
        return tmp;
    }

    bool operator== (const _List_unchecked_const_iterator& right) const noexcept {
        return _Ptr == right._Ptr;
    }
   
    bool operator!= (const _List_unchecked_const_iterator& right) const noexcept {
        return !(*this == right);
    }


    _Nodeptr _Ptr; // pointer to node
};


template <
    class Mylist_
>
class _List_unchecked_iterator : public _List_unchecked_const_iterator<Mylist_> {
public:
    using Mybase           = _List_unchecked_const_iterator<Mylist_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename Mylist_::_Nodeptr;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&;

    using Mybase::Mybase;



    reference operator*() const noexcept {
        return const_cast<reference>(Mybase::operator*());
    }    

    pointer operator->() const noexcept {
        return this;
    }

    _List_unchecked_iterator& operator++() {
        Mybase::operator++();
        return *this;
    }

    _List_unchecked_iterator operator++(int) {
        auto tmp = *this;
        Mybase::operator++();
        return tmp;
    }

    _List_unchecked_iterator& operator--() {
        Mybase::operator--();
        return *this;
    }

    _List_unchecked_iterator operator--(int) {
        auto tmp = *this;
        Mybase::operator--();
        return tmp;
    }
};


template <
    class Mylist_
>
class _List_const_iterator : public _List_unchecked_const_iterator<Mylist_> {
public:
    using Mybase = _List_unchecked_const_iterator<Mylist_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename Mylist_::_Nodeptr;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&; 

    using Mybase::Mybase;



    reference operator*() const {
        try {
            const auto _Mycont = static_cast<const Mylist_*>(this->Getcont());
            if (_Mycont == nullptr) {
                throw _AXC exception("Cannot dereference value-initialized list iterator");
            }

            if (this->_Ptr == _Mycont->_Myfirstiter->_Ptr) {
                throw _AXC exception("Cannot dereference end list iterator");
            }

            return this->_Ptr->_Myval; 
        }
        catch (_AXC exception& general_exception) {
            std::cerr << general_exception.what();
        }
    }

    pointer operator->() const noexcept {
        return this;
    }

    _List_const_iterator& operator++() {
        try {
            const auto _Mycont = static_cast<const Mylist_*>(this->Getcont());
            if (_Mycont == nullptr) {
                throw _AXC exception("Cannot increment value-initialized list iterator");
            }

            if (this->_Ptr == _Mycont->_Myfirstiter->_Ptr) {
                throw _AXC exception("Cannot increment end list iterator");
            } 
            this->_Ptr = this->_Ptr->_Next;
            return *this;
        }   
        catch (_AXC exception& general_exception) {
            std::cerr << general_exception.what();
        }
    }

    _List_const_iterator operator++(int) {
        _List_const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    _List_const_iterator& operator--() {
        try {
            const auto _Mycont = static_cast<const Mylist_*>(this->Getcont());
            const auto New_ptr = this->_Ptr->_Prev;
            if (_Mycont == nullptr) {
                throw _AXC exception("Cannot decrement value-initialized list iterator");
            }

            if (New_ptr == _Mycont->_Myfirstiter->_Ptr) {
                throw _AXC exception("Cannot decrement end list iterator");
            } 
            this->_Ptr = New_ptr;
            return *this;
        }   
        catch (_AXC exception& general_exception) {
            std::cerr << general_exception.what();
        }
    }

    _List_const_iterator operator--(int) {
        _List_const_iterator tmp = *this;
        --*this;
        return tmp;
    }

    bool operator== (const _List_const_iterator& Right) const {
        try {
            if ( (this->Getcont() =! Right.Getcont()) ) {
                throw _AXC exception("List iterators incompatible");
            }

            return this->_Ptr == Right._Ptr; 
        }
        catch (_AXC exception& incorrect_Right) {
            std::cerr << incorrect_Right.what();
        }
    }

    bool operator!= (const _List_const_iterator& Right) const {
        return !(*this == Right);
    }

    friend bool Verify_range_ (const _List_const_iterator& First, const _List_const_iterator& Last) noexcept {
        return First.Getcont() == Last.Getcont();
    }

    _List_unchecked_const_iterator<Mylist_> Unwrapped_() const noexcept {
        return _List_unchecked_const_iterator<Mylist_>(this->_Ptr, static_cast<const Mylist_*>(this->Getcont())); 
    }
};


template <
    class Mylist_
>
class _List_iterator : public _List_const_iterator<Mylist_> {
public:
    using Mybase = _List_unchecked_const_iterator<Mylist_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename Mylist_::_Nodeptr;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&; 

    using Mybase::Mybase;



    reference operator*() const {
        return const_cast<reference>(Mybase::operator*());
    }

    pointer operator->() const noexcept {
        return const_cast<pointer>(this);
    }

    _List_iterator& operator++() {
        Mybase::operator++();
        return *this;
    }

    _List_iterator operator++(int) {
        _List_iterator tmp = *this;
        Mybase::operator++();
        return tmp;
    }

    _List_iterator& operator--() {
        Mybase::operator--();
        return *this;
    }

    _List_iterator operator--(int) {
        _List_iterator tmp = *this;
        Mybase::operator--();
        return tmp;
    }

    _List_unchecked_iterator<Mylist_> Unwrapped_() const noexcept {
        return _List_unchecked_iterator<Mylist_>(this->_Ptr, static_cast<const Mylist_*>(this->Getcont()));
    }
};

_AXC_END