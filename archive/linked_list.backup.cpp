#include "listIterator.cpp"
#include <initializer_list>
#include <limits>
#include <memory>



_AXC_BEGIN

template <class Ty_>
struct _List_simple_types : _Simple_types<Ty_> {
    using _Node    = _List_node<Ty_>;
    using _Nodeptr = _List_node<Ty_>*;
};


template < 
    class Value_type_,
    class Size_type_,
    class Difference_type_,
    class Pointer_,
    class Const_pointer_,
    class Reference_,
    class Const_reference,
    class Nodeptr_type
>
struct _List_iter_types {
    using value_type      = Value_type_;
    using size_type       = Size_type_;
    using difference_type = Difference_type_;
    using pointer         = Pointer_;
    using const_pointer   = Const_pointer_;
    using _Nodeptr        = Nodeptr_type;
};


template <class Val_types_>
class _List_val : public _Container_base {
public:
    using _Nodeptr = typename Val_types_::_Nodeptr;

    using value_type      = typename Val_types_::value_type;
    using size_type       = typename Val_types_::size_type; // ?????????
    using difference_type = typename Val_types_::difference_type;
    using pointer         = typename Val_types_::pointer;
    using const_pointer   = typename Val_types_::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;


    _List_val() noexcept : _Myhead(nullptr), _Mysize(0) {} // initialize data

    void _Orphan_iterator (_Nodeptr Ptr) noexcept { // orphan iterator from node it contains to
        _Iterator_base* ItNext = this->_Myfirstiter;
        const auto head = _Myhead;

        while(ItNext != nullptr) {
            _Iterator_base* ItNextNext = ItNext->_Mynextiter;
            const auto PtrNext = static_cast<_List_const_iterator<_List_val>&>(ItNext)._Ptr;
            
            if (PtrNext == head || PtrNext != Ptr) {
                // ItNext == end() or ItNext doesn't point at the one we are orphaning, move on
                ItNext = ItNextNext;
            } else { // orphan the iterator
                ItNext->_Myproxy = nullptr;
                ItNext           = ItNextNext;
                // break;
            }
        }
    }

    _Nodeptr _Unlinknode (_Nodeptr Pnode) noexcept { // unlink Pnode from the list

        Pnode->_Prev->_Next = Pnode->_Next;
        Pnode->_Next->_Prev = Pnode->_Prev;
        --_Mysize;
        return Pnode;
    }


    _Nodeptr  _Myhead; // pointer to head node
    size_type _Mysize; // number of elements
};


template <
    class Ty_
>
class linked_list { // biderectional double linked list
private:
    // *WARNING*
    // I provide only std::allocator and _Default_allocator_traits,
    // because implement _Normal_allocator_traits quite complicated,
    // that's why I excluded _Alloc from templates parametrs for linked_list. 
    using _Alloc = _STD allocator<Ty_>;



    using _Alty          = typename _AXC _Default_allocator_traits<_Alloc>::template rebind_alloc<Ty_>;
    using _Alty_traits   = _AXC _Default_allocator_traits<_Alty>;
    using _Node          = _List_node<Ty_>;
    using _Alnode        = typename _AXC _Default_allocator_traits<_Alloc>::template rebind_alloc<_Node>;
    using _Alnode_traits = _AXC _Default_allocator_traits<_Alnode>;
    using _Nodeptr       = typename _Alnode_traits::pointer;

    using _Val_types = _List_iter_types <
        Ty_,
        typename _Alty_traits::size_type,
        typename _Alty_traits::difference_type,        
        typename _Alty_traits::pointer,
        typename _Alty_traits::const_pointer,
        Ty_&,
        const Ty_&,
        _Nodeptr
    >;
    using _Scary_val = _List_val<_Val_types>;

public:
    using value_type      = Ty_;
    using allocator_type  = _Alloc;
    using size_type       = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;
    using pointer         = typename _Alty_traits::pointer;
    using const_pointer   = typename _Alty_traits::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator                  = _List_iterator<_Scary_val>;
    using const_iterator            = _List_const_iterator<_Scary_val>;
    using _Unchecked_iterator       = _List_unchecked_iterator<_Scary_val>;
    using _Unchecked_const_iterator = _List_unchecked_const_iterator<_Scary_val>;

    using reverse_iterator       = _STD reverse_iterator<iterator>;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;


    /* Ctors, Dtor & Assignment Operators */

    linked_list() noexcept : _Mycont()
    {
        _ALLOCATE_HEAD_AND_PROXY();
    }

    // TODO: need check
    // **be careful** type Ty_ must have default ctor
    explicit linked_list (size_t size, const_reference Value = Ty_() ) : _Mycont() {
        try {
            if (size <= 0 || size > 4294967295) { // UINT_MAX = 4294967295, to not include <limits.h>
                _Mycont._Mysize = 0;
                throw _AXC exception("Invalid size"); // TODO: invalid_argument
            }

            _ALLOCATE_HEAD_AND_PROXY();
            _Mycont._Myhead->_Prev = _Mycont._Myhead->_Next = new _Node(Value, _Mycont._Myhead, _Mycont._Myhead);
            
            while (size --> 0) { 
                _EMPLACE_BACK( std::move( Ty_(Value) ) ); 
                // there I use std::move instead of std::forward, because
                // Ty_(Value) return rvalue of type Ty_ so perfect forwarding not possible
            }
        }
        catch (_AXC exception& invalid_size) { // TODO: invalid_argument
            std::cerr << invalid_size.what();
        }
    }

protected:
    //TODO: need to check
    void _ALLOCATE_HEAD_AND_PROXY() noexcept {
        _Mycont.Alloc_proxy();
        _ALLOCATE_HEAD();
    }

    //TODO: need to check
    void _ALLOCATE_HEAD() noexcept {
        _Mycont._Myhead = static_cast<_Nodeptr>(::operator new(sizeof(_Node)));
        // ::operator new return void*, i need to cast void* to _Nodeptr by static_cast
        
        _Mycont._Myhead->_Prev = _Mycont._Myhead;
        _Mycont._Myhead->_Next = _Mycont._Myhead;
    }

    // TODO: need check
    _Nodeptr _EMPLACE (_Nodeptr Where, Ty_&& Value) noexcept { // create element at Where
        try {
            
            if (_Mycont._Mysize == max_size()) {
                throw _AXC exception("list too long"); // TODO: lenght_error
            }

            _Nodeptr new_node = new _Node(std::forward<Ty_>(Value), Where->_Prev, Where);
            Where->_Prev->_Next = new_node;
            Where->_Prev = new_node;

            _Mycont._Mysize++;
            return new_node;
        }
        catch (_AXC exception& length_error) {
            std::cerr << length_error.what();
        }
    }

    _Nodeptr _EMPLACE_BACK (Ty_&& Value) noexcept {
        return _EMPLACE(_Mycont._Myhead, std::forward<Ty_>(Value));
    }

    _Nodeptr _EMPLACE_FRONT (Ty_&& Value) noexcept {
        return _EMPLACE(_Mycont._Myhead->_Next, std::forward<Ty_>(Value));
    }

public:
    // TODO: need check
    linked_list (const linked_list& Right) noexcept : _Mycont() { // deep copy
        if (Right._Mycont._Mysize != 0)
        {
            _ALLOCATE_HEAD_AND_PROXY();
            
            auto Right_Nodeptr = Right._Mycont._Myhead->_Next;
            while (Right_Nodeptr != nullptr) 
            {
                _EMPLACE_BACK( Ty_(Right_Nodeptr->_data) );
                Right_Nodeptr = Right_Nodeptr->_Next;
            }
        }
    }

    // TODO: create this
    ~linked_list() { 
        this->_TIDY();
        // 1. deallocate linked_list
        // 2. deallocate _List_val
        // 3. deallocate _Container_proxy
        // 4. deallocate _Container_base
    }

private:
    void _TIDY() noexcept {
        _Mycont._Orphan_all();
        _Node::_Free_non_head(_Mycont._Myhead);
        _Node::_Freenode0(_Mycont._Myhead);
    }

    void _COPY_ASSIGN(const linked_list& Right) {
        assign(Right._Unchecked_begin(), Right._Unchecked_end());
    }

public:
    // TODO: check
    linked_list& operator= (const linked_list& Right) noexcept {
        if (std::addressof(*this) != std::addressof(Right)) {
            _COPY_ASSIGN(Right);
        }

        return *this;
    }

    // TODO: need check
    linked_list (linked_list&& Right) noexcept { // shallow copy
        _ALLOCATE_HEAD_AND_PROXY();
        _SWAP_VAL(Right); // ?? why not _Swap_val(move(Right));
    }

    // TODO: need check
    linked_list& operator= (linked_list&& Right) noexcept {
        if (std::addressof(*this) != std::addressof(Right)) {
            _MOVE_ASSIGN(move(Right));
        }

        return *this;
    }

private:
    void _SWAP_VAL (linked_list& Right) noexcept { // swap with Right, same allocator
        auto Rightcont = Right._Mycont;
        _Mycont._Swap_proxy_and_iterators(Rightcont);
        _AXC swap(_Mycont._Mycont._Myhead, Rightcont._Myhead); // *WARNING* no _Swap_adl
        _AXC swap(_Mycont._Mycont._Mysize, Rightcont._Mysize);
    }

    void _MOVE_ASSIGN (linked_list&& Right) {

    }

public:
    // TODO: need check
    template <
        class T_,
        size_t size
    >
    linked_list (T_ (&&init_list)[size]) noexcept {
        if (size > 0) {
            _ALLOCATE_HEAD_AND_PROXY();
            _Mycont._Myhead->_Prev = _Mycont._Myhead->_Next = new _Node(std::move(init_list[0]), _Mycont._Myhead, _Mycont._Myhead);
            
            for (size_t i = 1; i < size; i++) {
                _EMPLACE_BACK(std::move(init_list[i]));
            }
        }
    }


    /* Iterators */


    iterator begin() noexcept {
        return iterator(_Mycont._Myhead->_Next, _AXC addressof(_Mycont));
    }

    const_iterator begin() const noexcept {
        return const_iterator(_Mycont._Myhead->_Next, _AXC addressof(_Mycont));
    }

    iterator end() noexcept {
        return iterator(_Mycont._Myhead, _AXC addressof(_Mycont));
    }

    const_iterator end() const noexcept {
        return const_iterator(_Mycont._Myhead, _AXC addressof(_Mycont));
    }

    _Unchecked_iterator _Unchecked_begin() noexcept {
        return _Unchecked_iterator(_Mycont._Myhead->_Next, nullptr);
    }

    _Unchecked_iterator _Unchecked_begin() const noexcept {
        return _Unchecked_const_iterator(_Mycont._Myhead->_Next, nullptr);
    }

    _Unchecked_iterator _Unchecked_end() noexcept {
        return _Unchecked_iterator(_Mycont._Myhead, nullptr);
    }

    _Unchecked_iterator _Unchecked_end() const noexcept {
        return _Unchecked_const_iterator(_Mycont._Myhead, nullptr);
    }


protected:
    iterator _MAKE_ITER (_Nodeptr Where_) const noexcept {
        return iterator(Where_, _AXC addressof(_Mycont));
    }

    const_iterator _MAKE_CONST_ITER (_Nodeptr Where_) const noexcept {
        return const_iterator(Where_, _AXC addressof(_Mycont));
    }

public:
    // TODO: reverse iterators

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    // TODO: const reverse iterators


    /* Accessors & Mutators */


    // Ty_& operator[] (size_t index) const {
    //     try {
    //         if (index < 0 || index >= _Mycont._Mysize) {
    //             throw _AXC exception("Invalid index"); // TODO: out_of_range
    //         }

    //         auto node_it = _Mycont._Myhead->_Next;
    //         for (size_t i = 0; i < index; i++) {
    //             node_it = node_it->_Next;
    //         }

    //         return node_it->_data;
    //     }
    //     catch (_AXC exception& invalid_index) {
    //         std::cerr << invalid_index.what();
    //     }
    // }

    size_type size() const noexcept {
        return _Mycont._Mysize;
    }

    size_type max_size() const noexcept {
        return static_cast<size_type>(std::numeric_limits<difference_type>::max());
    }

    bool empty() const noexcept {
        return _Mycont._Mysize == 0;
    }

    reference front() { 
        try {
            if ( _Mycont._Mysize == 0 ) {
                throw _AXC exception("front() called on empty list"); // TODO: out_of_range
            }

            return _Mycont._Myhead->_Next->_Myval;     
        }
        catch (_AXC exception& length_error) {
            std::cerr << length_error.what();
        }
    }  
        
    const_reference front() const {
        try {
            if ( _Mycont._Mysize == 0 ) {
                throw _AXC exception("front() called on empty list"); // TODO: out_of_range
            }

            return _Mycont._Myhead->_Next->_Myval;     
        }
        catch (_AXC exception& length_error) {
            std::cerr << length_error.what();
        }
    }


    reference back() { 
        try { 
            if ( _Mycont._Mysize == 0 ) {
                throw _AXC exception("back() called on empty list"); // TODO: out_of_range
            }

            return _Mycont._Myhead->_Prev->_Myval;     
        }
        catch (_AXC exception& length_error) {
            std::cerr << length_error.what();
        }
    }
        
    const_reference back() const { 
        try {
            if ( _Mycont._Mysize == 0 ) {
                throw _AXC exception("back() called on empty list"); // TODO: out_of_range
            }

            return _Mycont._Myhead->_Prev->_Myval;     
        }
        catch (_AXC exception& length_error) {
            std::cerr << length_error.what();
        }
    }


    /* Modifiers */


    void push_front (value_type&& Value) {
        _EMPLACE(_Mycont._Myhead->_Next, move(Value));
    }

    void push_back (value_type&& Value) {
        _EMPLACE(_Mycont._Myhead, move(Value));
    }

    void push_front (const_reference Value) {
        _EMPLACE(_Mycont._Myhead->_Next, Value);
    }

    void push_back (const_reference Value) {
        _EMPLACE(_Mycont._Myhead, Value);
    }

    void pop_front() {
        try {
            if (_Mycont._Mysize == 0) {
                throw _AXC exception("pop_front() called on empty list"); // TODO: out_of_range
            }

            _UNCHECKED_ERASE(_Mycont._Myhead->_Next);
        }
        catch (_AXC exception& length_error) {
            std::cerr << length_error.what();
        }
    }

    void pop_back() {
        try {
            if (_Mycont._Mysize == 0) {
                throw _AXC exception("pop_back() called on empty list"); // TODO: out_of_range
            }

            _UNCHECKED_ERASE(_Mycont._Myhead->_Prev);
        }
        catch (_AXC exception& length_error) {
            std::cerr << length_error.what();
        }
    }

    iterator emplace (const const_iterator Where_, value_type&& data) { // insert element at Where_
        return _MAKE_ITER(_EMPLACE(Where_._Ptr, std::forward<value_type>(data)));
    }

    reference emplace_front (value_type&& data) { // insert element at beginning
        reference Result_ = _EMPLACE(_Mycont._Myhead->_Next, std::forward<value_type>(data))->_data;
        return Result_;
    }

    reference emplace_back (value_type&& data) { // insert element at end
        reference Result_ = _EMPLACE(_Mycont._Myhead, std::forward<value_type>(data))->_data;
    }

    iterator insert (const_iterator Where_, std::initializer_list<Ty_> Ilist_) { // insert initializer_list
        return insert(Where_, Ilist_.begin(), Ilist_.end());
    }

    iterator insert (const_iterator Where_, const_reference data) { // insert data at Where_
        return _MAKE_ITER(_EMPLACE(Where_._Ptr, data));
    }

    iterator insert (const_iterator where, int count, const_reference data) { // insert count * data before where

    }

    template <
        class Iter_
    >
    iterator insert (const_iterator where, Iter_ First_, Iter_ Last_) { // insert [First_, Last_) before where

    }

    iterator erase (const_iterator Where_) { // erase element at Where_
        // TODO: check if iterator outside list 
        const auto Result_ = Where_._Ptr->_Next;
        _FREENODE(_UNLINK_NODE(Where_._Ptr));
    }

protected:

    _Nodeptr _UNLINK_NODE (_Nodeptr Dnode_) { // detach Dnode_ from list   
        Dnode_->_Prev->_Next = Dnode_->_Next;
        Dnode_->_Next->_Prev = Dnode_->_Prev;

        Dnode_->_Prev = Dnode_->_Next = nullptr;
        return Dnode_;
    }

    void _FREENODE (_Nodeptr Pnode_) { // DANGEROUS //
        delete Pnode_;
    }
    
    _Nodeptr _UNCHECKED_ERASE (const _Nodeptr Pnode_) noexcept { // erase element at Pnode_
        const auto Result_ = Pnode_->_Next;
        // TODO: orphan iterators with specified node pointers      
        Pnode_->_Prev->_Next = Result_;
        Result_->_Prev       = Pnode_->_Prev;
        
        _FREENODE(Pnode_);
        _Mycont._Mysize--;
        return Result_;
    }

public:

    iterator erase (const const_iterator First_, const const_iterator Last_) noexcept {

    }

protected:

    _Nodeptr _UNCHECKED_ERASE (_Nodeptr First_, const _Nodeptr Last_) noexcept {

    }

public:

    auto remove (const_reference data) { // erase each element matching data

    }

    template <
        class PredTy_
    >
    auto remove_if (PredTy_ Pred_) { // erase each element satisfying Pred_

    }

    void clear() noexcept {

    }

    void resize (size_t newSize) {

    }

    void resize (size_t newSize, const_reference data) {

    }



protected:
    _Scary_val _Mycont;
};


_AXC_END
