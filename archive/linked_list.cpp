// #include <xmemory>
#include "msldef.h"
#include "xmemory.hpp"
#include "list_node.hpp"
#include "list_iterator.hpp"
// #include <cinttypes>


_MSL_BEGIN
// [x] _List_simple_types
// [x] _List_iter_types
// [ ] _List_val
// [ ] list_v1
// [ ] list_v2
//     [] list_v2()
//     [] _Construct_n (Count)
//     [] list_v2 (Count)
//     [] _Construct_n(Count, Val)
//     [] list_v2 (Count, Val)
//     [] _Construct_range_unchecked_my
//     [] list_v2 (Rhs)
//     [] list_v2 (&&Rhs)
//     [] operator=
//     [] _Swap_val
//     [] push_front
//     [] push_back
//     [] insert (Where, &&Val)
//     [] emplace_front
//     [] emplace_back
//     [] emplace
//     [] _Emplace
//     [] ~list_v2
//     [] operator= 
//     [] all iters
//     [] resize (Newsize)
//     [] resize (Newsize, Val)
//     [] size()
//     [x] empty
//     [] front
//     [] back
//     [] push_front
//     [] pop_back()
//     [?] assign (First, Last)
//     [?] assing (Count, Val)
//     [?] insert (Where, Val)
//     [?] insert (Where, Count, Val)
//     [?] insert (Where, First, Last)
//     [?] erase (Where)
//     [] _Unchecked_erase (Node)
//     [] erase (First, Last)
//     [] _Unchecked_erase (First, Last)
//     [] clear()
//     [] _Tidy()
//     [] remove
//     [] remove_if
//     [] reverse
//     [] _Alloc_head_and_proxy
//     [] _Orphan_all


template <class _Value_type>
struct _List_simple_type_traits : _Simple_type_traits<_Value_type> { // _List_simple_types in STL
    using _Node    = _List_node<_Value_type>;
    using _Nodeptr = _List_node<_Value_type>*;
};


template <
    class _Value_type,
    class _Size_type,
    class _Difference_type,
    class _Pointer,
    class _Const_pointer,
    class _Reference,
    class _Const_reference,
    class _Nodeprt_type
>
struct _List_iter_traits { // _List_iter_types in STL
    using value_type      = _Value_type;
    using size_type       = _Size_type;
    using difference_type = _Difference_type;
    using pointer         = _Pointer;
    using const_pointer   = _Const_pointer;
    using _Nodeptr        = _Nodeprt_type;
};


template <class _Val_types> // NOTE: does it list for iterators that makes
                            // a base for main list?
class _List_val : public _Container_base {
public:
    using _Nodeptr = typename _Val_types::_Nodeptr;
    
    using value_type      = typename _Val_types::value_type;
    using size_type       = typename _Val_types::size_type;
    using difference_type = typename _Val_types::difference_type;
    using pointer         = typename _Val_types::pointer;
    using const_pointer   = typename _Val_types::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;    
    
    _List_val() noexcept : _Myhead(), _Mysize(0) {} // NOTE: doesn't construct _Container_base implicitly

    _List_val (const _List_val& Rhs) noexcept : _Myhead(), _Mysize(Rhs._Mysize) {} // NOTE: mb it't unneccessary

    void _Oprhan_iterators (_Nodeptr Ptr) noexcept;

    void _Orphan_non_end() noexcept;

    _Nodeptr _Unlink_node (_Nodeptr Node) noexcept; // unlink node at Where from the list

    void _Adopt_unique (_List_val& Other, _Nodeptr Node) noexcept {
        // adopt iterators pointing to spliced node
    }

    void _Adopt_all (_List_val& Other) noexcept {
        // adopt all iterators except Other.end()
    }

    void _Adopt_range (_List_val& Other, const _Nodeptr First, const _Nodeptr Last) noexcept {
        // adopt all iterators pointing to nodes in the range [First, Last) by marking nodes
    }


    _Nodeptr  _Myhead; // pointer to head node
    size_type _Mysize; // number of elements
};


template <class _Node_t>
struct _List_node_insert_v2 { // _List_node_insert_op2 in STL
    // list insert operation (which maintains exception safety)
    using value_type = _Node_t;
    using pointer    = _Node_t*;
    using size_type  = std::size_t;

    explicit _List_node_insert_v2() : _Added(0), _Head(nullptr), _Tail(nullptr) {}

    _List_node_insert_v2 (const _List_node_insert_v2&) = delete;
    _List_node_insert_v2& operator= (const _List_node_insert_v2&) = delete;

    void _Append_n (size_type Count) { // Append Count elements
        if (Count <= 0) { return; }

        if (_Added == 0) {
            pointer Newnode = static_cast<pointer>(::operator new(sizeof(value_type)));
            _Head = Newnode; _Tail = Newnode;
            ++_Added;
            --Count;
        }

        for (; Count > 0; --Count) {
            auto Append_node = static_cast<pointer>(::operator new(sizeof(value_type)));
            _Tail->_Next = Append_node;
            Append_node->_Prev = _Tail;
            _Tail = Append_node;
            ++_Added;
        }
    }
    
    // template <class _CArgT>
    // void _Append_n (size_type Count, const _CArgT& _Carg);
    
    template <class _Val_types>
    pointer _Attach_before (_List_val<_Val_types>& List, const pointer Insert_before) noexcept { // NOTO: check this!!
        // Attach the elements from *this in List before Where
        // If *this is empty, returns Where; otherwise returns a pointer to first inserted list node
        // Resets *this to the default-initialized state
    
        if (_Added == 0) { return Insert_before; }

        const auto Local_head = _Head;
        const auto Local_tail = _Tail;
        const auto Insert_after = Insert_before->_Prev;

        Insert_after->_Next = Local_head;
        Local_head->_Prev = Insert_after;
        Insert_before->_Prev = Local_tail;
        Local_tail->_Next = Insert_before;
        List._Mysize += _Added;

        _Head = nullptr; _Tail = nullptr; _Added = 0;

        return Local_head;
    }

    template <class _Val_types>
    void _Attach_at_end (_List_val<_Val_types>& List) noexcept {
        _Attach_before(List, List._Myhead);
    }

    template <class _Val_types>
    void _Attach_head (_List_val<_Val_types>& List) { // 
        // Create within *this list head pointer and transfer new whole insert_list in empty List
        pointer Newhead = static_cast<pointer>(::operator new(sizeof(value_type)));
        // create new head
        if (_Added == 0) { 
            Newhead->_Next = Newhead;
            Newhead->_Prev = Newhead;
        } else {
            const auto Local_head = _Head;
            const auto Local_tail = _Tail;
            Newhead->_Next = Local_head;
            Newhead->_Prev = Local_tail;
            Local_head->_Prev = Newhead;
            Local_tail->_Next = Newhead;
        }
       
        List._Mysize = _Added;
        List._Myhead = Newhead; 
        _Head = nullptr; _Tail = nullptr; _Added = 0;
    }

    ~_List_node_insert_v2() { // TODO: impl
        if (_Added == 0) { return; }

        _Head->_Prev = nullptr;
        _Tail->_Next = nullptr;
        pointer Ptr = _Head;
        while (Ptr != nullptr) {
            
        }
    }

private:
    size_type _Added; // if 0, the values of _Head and _Tail are indeterminate
    pointer   _Head; // points to the first appended element; it doesn't have _Prev constructed
    pointer   _Tail; // points to most recently appended element; id doesn't have _Next constructed
};

template <
    class _Ty,
    class _Alloc = std::allocator<_Ty>
>
class list_v1 {
private:
    using _Alty          = _Rebind_alloc_t<_Alloc, _Ty>;
    using _Alty_traits   = std::allocator_traits<_Alty>;
    using _Node          = _List_node<_Ty>;
    using _Alnode        = _Rebind_alloc_t<_Alloc, _Node>;
    using _Alnode_traits = std::allocator_traits<_Alnode>;
    using _Nodeptr       = typename _Alnode_traits::pointer;

    using _Val_types = 
        std::conditional_t<
            _Is_simple_alloc_v<_Alnode>,
                _List_simple_type_traits<_Ty>, // simple alloc
                _List_iter_traits< // complex alloc
                    _Ty,
                    typename _Alty_traits::size_type,
                    typename _Alty_traits::difference_type,
                    typename _Alty_traits::pointer,
                    typename _Alty_traits::const_pointer,
                    _Ty&,
                    const _Ty&,
                    _Nodeptr
                >
        >;
    using _List_scary_val = _List_val<_Val_types>; // _Scary_val in STL

public:
    static_assert(std::is_same_v<_Ty, typename _Alloc::value_type>,
                  "list<T, Allocator> and T MISMATCHED ALLOCATOR");
    static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids containers of non-object types "
                                         "because of [container.requirements].");

    using value_type      = _Ty;
    using allocator_type  = _Alloc;
    using size_type       = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;
    using pointer         = typename _Alty_traits::pointer;
    using const_pointer   = typename _Alty_traits::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator                  = _List_iterator<_List_scary_val>;
    using const_iterator            = _List_const_iterator<_List_scary_val>;
    using _Unchecked_iterator       = _List_unchecked_iterator<_List_scary_val>;
    using _Unchecked_const_iterator = _List_unchecked_const_iterator<_List_scary_val>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // _Compressed_pair<_Alnode, _List_scary_val> _Mypair;
    _List_scary_val _Mycont;
    _Alnode         _Myalnode;
};

template < 
    class _Ty,
    class _Alloc = std::allocator<_Ty>
>
class list_v2 { // same class as list_v1, but no memory self miantain
private:
    using _Node        = _List_node<_Ty>;
    using _Nodeptr     = _List_node<_Ty>*;

    using _Val_types = _List_simple_type_traits<_Ty>;
    using _List_SCARY_val = _List_val<_Val_types>; // not SCARY to tell the truth
                                                   // because _Val_type doesn't provide SCARY
    using _List_node_insert = _List_node_insert_v2<_List_node<_Ty>>;

public:
    static_assert(std::is_same_v<_Ty, typename _Alloc::value_type>,
                  "list<T, Allocator> and T MISMATCHED ALLOCATOR");
    static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids containers of non-object types "
                                         "because of [container.requirements].");

    using value_type      = _Ty;
    using allocator_type  = _Alloc;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = _Ty*;
    using const_pointer   = const _Ty*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator                  = _List_iterator<_List_SCARY_val>;
    using const_iterator            = _List_const_iterator<_List_SCARY_val>;
    using _Unchecked_iterator       = _List_unchecked_iterator<_List_SCARY_val>;
    using _Unchecked_const_iterator = _List_unchecked_const_iterator<_List_SCARY_val>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    _List_SCARY_val _Mycont;
   
    list_v2() noexcept : _Mycont() {
        _Alloc_head_and_proxy();
    } 

private:
    void _Construct_n (size_type Count) {
        // construct and apply Proxy for _Mycont
        _Mycont._Container_base::_Alloc_proxy();
        // append n elements
        _List_node_insert Appended;
        Appended._Append_n(Count);
        Appended._Attach_head(_Mycont);
    }

public:
    explicit list_v2 (size_type Count) : _Mycont() {
        _Construct_n(Count);
    }

private:
    void _Construct_n (size_type Count, const _Ty& Val) {
        // construct and apply Proxy for _Mycont
        _Mycont._Container_base::_Alloc_proxy();
        // append n elements
        _List_node_insert Appended;
        Appended._Append_n(Count, Val);
        Appended._Attach_head(_Mycont);
    }

public:
    list_v2 (size_type Count, const _Ty& Val) : _Mycont() {
        _Construct_n(Count, Val);
    }

// private:
//     template <class _Iter_t>
//     void _Construct_range_unchecked_my (_Iter_t First, _Iter_t Last);

public:
    // list_v2 (const list_v2& Rhs) : _Mycont(Rhs._Mycont) {
    //     // _Ald_verify_range();
    //     _Construct_range_unchecked_my(Rhs._Unchecked_begin(), Rhs._Unchecked_end());
    // }

    list_v2 (list_v2&& Rhs) : _Mycont(std::move(Rhs._Mycont)) {
        _Alloc_head_and_proxy();
        _Swap_val(Rhs);
    }

    list_v2& operator= (list_v2&& Rhs) noexcept {
        if (this == std::addressof(Rhs)) { return *this; }

        clear(); _Swap_val(Rhs);
        return *this;
    }

private:
    void _Swap_val (list_v2& Rhs) noexcept { // swap with Rhs
        auto& Mycont = _Mycont;
        auto& Rhscont = Rhs._Mycont;
        Mycont._Swap_proxy_and_iterators(Rhscont);
        auto* Temp = _Mycont._Myhead;
        _Mycont._Myhead = Rhs._Mycont._Myhead;
        Rhs._Mycont._Myhead = Temp;
        std::swap(Mycont._Mysize, Rhscont._Mysize);
    }

public:
    void push_front (_Ty&& Val) { // insert element at beginning
        _Emplace(_Mycont._Myhead->_Next, std::move(Val)); 
    }

    void push_back (_Ty&& Val) { // insert element at end
        _Emplace(_Mycont._Myhead, std::move(Val));
    }

    iterator insert (const_iterator Where, _Ty&& Val) { // Val at Where
        return emplace(Where, std::move(Val)); 
    }

    decltype(auto) emplace_front (_Ty&& Val) { // insert element at beggining
        reference Result = _Emplace(_Mycont._Myhead->_Next, std::move(Val)); 
        return Result;
    }

    decltype(auto) emplace_back (_Ty&& Val) { // insert element at end
        reference Result = _Emplace(_Mycont._Myhead, std::move(Val));
        return Result;
    }

    iterator emplace (const const_iterator Where, _Ty&& Val) { // insert element at Where
        

    }

    _Nodeptr _Emplace (const _Nodeptr Where, _Ty&& Val); // insert element at Where

    // list_v2 (initializer_list<_Ty> Ilist) : _Mycont() {
    //     _Construct_range_unchecked_my(Ilist.begin(), Ilist.end());
    // }

    ~list_v2() noexcept;
    
    list_v2& operator= (const list_v2& Rhs);

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    _Unchecked_iterator _Unchecked_begin() noexcept;

    _Unchecked_const_iterator _Unchecked_begin() const noexcept;

    _Unchecked_iterator _Unchecked_end() noexcept;

    _Unchecked_const_iterator _Unchecked_end() const noexcept;

    iterator _Make_iter(_Nodeptr Where) const noexcept;

    const_iterator _Make_const_iterator (_Nodeptr Where) const noexcept;

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    void resize (size_type Newsize); // determine new length, padding with _Ty() elements as needed

    void resize (size_type Newsize, const _Ty& Val); // determine new length, padding with Val elements as needed

    size_type size() const noexcept {
        return _Mycont._Mysize;
    }

    bool empty() const noexcept {
        return _Mycont._Mysize == 0;
    } 
    
    reference front() noexcept;

    const_reference front() const noexcept;

    reference back() noexcept;

    const_reference back() const noexcept;

    void push_front (const _Ty& Val);

    void pop_back() noexcept; 

    template <class _Iter_t>
    void assign (_Iter_t First, _Iter_t Last);

    void assign (size_type Count, const _Ty& Val); // assign Count * Val
    
    iterator insert (const_iterator Where, const _Ty& Val); // insert Val at Where
    
    iterator insert (const_iterator Where, size_type Count, const _Ty& Val);
    // insert Count * Val before Where
   
    template <class _Iter_t>
    iterator insert (const const_iterator Where, _Iter_t First, _Iter_t Last); // insert [First, Last) before Where
    
    iterator erase (const const_iterator Where) noexcept;

private:
    _Nodeptr _Unchecked_erase (const _Nodeptr Node) noexcept; // erase element at Node

public:
    iterator erase (const const_iterator First, const const_iterator Last) noexcept;

private:
    _Nodeptr _Unchecked_erase (_Nodeptr First, const _Nodeptr Last) noexcept; // erase [First, Last)

public:
    void clear() noexcept; // erase all
    
private:
    void _Tidy() noexcept;

public:
    auto remove (const _Ty& Val); // erase each element matching Val

    template <class _Pr_t>
    auto remove_if (_Pr_t Pred); // erase each element satisfying Pred

    void reverse() noexcept; // reverse sequence

private:
    void _Alloc_head_and_proxy() { // NOTE: _Mycont doesn't exist yet in ctor list_v2()
        // _Container_proxy* Newproxy = new _Container_proxy(&_Mycont); // TODO: delete
        _Mycont._Container_base::_Alloc_proxy();
        _Nodeptr Newhead = static_cast<_Nodeptr>(::operator new(sizeof(_Node))); // TODO: operator delete
        _Mycont._Myhead = Newhead;
        Newhead->_Next = Newhead; Newhead->_Prev = Newhead;
    }
    
    void _Orphan_all() noexcept {
        _Mycont._Orphan_all();
    } 
};
    
_MSL_END
