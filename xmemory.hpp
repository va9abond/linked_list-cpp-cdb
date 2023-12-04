#ifndef XMEMORY_HPP
#define XMEMORY_HPP


#include "msldef.h"


_MSL_BEGIN
// [x] _Container_proxy
// [x] _Container_base
//         [x] _Orphan_all
//         [x] _Swap_proxy_and_iterators
//         [x] _Alloc_proxy
//         [-] _Reload_proxy
// [x] _Iterator_base
//         [x] operator=
//         [x] ~_Iterator_base
//         [x] _Adopt_me
//         [x] _Getcont
//         [x] _Assign
//         [?] _Orphan_me_v1
//         [?] _Orphan_me_v2


template <class _Alloc, class _Value_type>
using _Rebind_alloc_t = typename std::allocator_traits<_Alloc>::template rebind_alloc<_Value_type>;

// template <class _Alloc> // tests if allocator has simple addressing
// inline constexpr bool _Is_simple_alloc_v =
//     std::is_same_v<typename std::allocator_traits<_Alloc>::size_type, size_t>
//     && std::is_same_v<typename std::allocator_traits<_Alloc>::difference_type, ptrdiff_t>
//     && std::is_same_v<typename std::allocator_traits<_Alloc>::pointer, typename _Alloc::value_type*>
//     && std::is_same_v<typename std::allocator_traits<_Alloc>::const_pointer, const typename _Alloc::value_type*>;

template <class _Value_type>
struct _Simple_type_traits { // _Simple_types in STL
    using value_type      = _Value_type;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
};


struct _Iterator_base;
struct _Container_base;


struct _Container_proxy {
    _Container_proxy() noexcept = default;
    _Container_proxy (_Container_base* Mycont) noexcept : _Mycont(Mycont) {}


    const   _Container_base* _Mycont      = nullptr;
    mutable _Iterator_base*  _Myfirstiter = nullptr;
};


struct _Container_base {

    _Container_base() noexcept = default;

    _Container_base (const _Container_base&)            = delete;
    _Container_base& operator= (const _Container_base&) = delete;

    ~_Container_base() {
        _Orphan_all();
        _Free_proxy();
    }

    void _Orphan_all() noexcept;
    void _Swap_proxy_and_iterators (_Container_base&) noexcept;

    void _Alloc_proxy() {
        _Myproxy = new _Container_proxy(this);
        // _Myproxy = static_cast<_Container_proxy*>(::operator new(sizeof(_Container_proxy)));
        // _Myproxy->_Mycont = this;
    }

    void _Free_proxy() noexcept {
        if (_Myproxy) {
            _Myproxy->_Mycont = nullptr; _Myproxy->_Myfirstiter = nullptr;
            delete _Myproxy; _Myproxy = nullptr;
        }
    }


    _Container_proxy* _Myproxy = nullptr;
};


struct _Iterator_base {
public:
    _Iterator_base() noexcept = default; // construct orphaned iterator

    _Iterator_base (const _Iterator_base& Rhs) noexcept {
        *this = Rhs;
    }

    _Iterator_base& operator= (const _Iterator_base& Rhs) noexcept {
        _Assign(Rhs);
        return *this;
    }

    ~_Iterator_base() noexcept { // NOTE: mb i need to free allocated memory
        _Orphan_me_v2();
    }

    void _Adopt_by_cont(const _Container_base* Parent) noexcept {
        return _Adopt_me(Parent);
    }

    const _Container_base* _Getcont() const noexcept {
        return _Myproxy ? _Myproxy->_Mycont : nullptr;
    }


    mutable _Container_proxy* _Myproxy    = nullptr;
    mutable _Iterator_base*   _Mynextiter = nullptr;

private:
    // Assign self to other container by given iterator
    // from other container
    void _Assign (const _Iterator_base& Rhs) noexcept {
        if (_Myproxy == Rhs._Myproxy) { return; }
        if (Rhs._Myproxy) { // != nullptr => do adoption
            _Adopt_me(Rhs._Myproxy->_Mycont);
        } else { // == nullptr => no parent container now
            _Orphan_me_v2();
        }
    }

    // adopt self by other parent container
    void _Adopt_me (const _Container_base* Other_parent) noexcept {
        if (!Other_parent) { // other parent container is nullptr, no parent cont now
            _Orphan_me_v2();
            return;
        }

        // do adoption
        _Container_proxy* Other_parent_proxy = Other_parent->_Myproxy;
        if (_Myproxy != Other_parent_proxy) { // change parentage
            if (_Myproxy) { // already adopted, remove self from current list
                _Orphan_me_v2();
            }

            // insert at the beginning of list
            _Mynextiter = Other_parent_proxy->_Myfirstiter;
            Other_parent_proxy->_Myfirstiter = this;
            _Myproxy = Other_parent_proxy;
        }
    }

    // remove self from parent container
    void _Orphan_me_v1() noexcept {
        if (!_Myproxy) { return; } // already orphaned

        // remove self from current list
        _Iterator_base** Pnext = &_Myproxy->_Myfirstiter;
        // _Iterator_base** because _Myfirsiter may equals nullptr
        while (*Pnext && (*Pnext)->_Mynextiter != this) {
            Pnext = &(*Pnext)->_Mynextiter;
        }

        _MSL_VERIFY_f(*Pnext, "ITERATOR LIST CORRUPTED");
        (*Pnext)->_Mynextiter = _Mynextiter;
        _Mynextiter = nullptr; _Myproxy = nullptr;
    }

    void _Orphan_me_v2() noexcept {
        if (!_Myproxy) { return; } // already orphaned

        // remove self from current list
        _Iterator_base** Pnext = &_Myproxy->_Myfirstiter;
        while (*Pnext && *Pnext != this) {
            Pnext = &(*Pnext)->_Mynextiter;
        }

        _MSL_VERIFY_f(*Pnext, "ITERATOR LIST CORRUPTED");
        *Pnext = _Mynextiter; // <=> this = _Mynextiter, but we need there Pnext
                              // to make assign, expression (this = _Mynextiter)
                              // or (this = *_Mynextiter) cause error(idk why) 
                              // INCORRECT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        // this and *Pnext point to same memory cell, and all those pointers that
        // point to the memory cell this alse point to *Pnext, and if we change 
        // *Pnext to another address, than all pointers will point to the cell
        // corressponding to new adress // TODO: how it works?
        _Myproxy = nullptr; // why _Myproxy should be nullptr now, this points to
                            // _Mynextiter, doesn't it? And _Mynextiter still
                            // child of current container
                            // INCORRECT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    }
};


inline void _Container_base::_Orphan_all() noexcept {
    if (!_Myproxy) { return; } // no any iterators

    _Iterator_base* Pnext = _Myproxy->_Myfirstiter; _Myproxy->_Myfirstiter = nullptr;
    while (Pnext) {
        Pnext->_Myproxy = nullptr;
        Pnext = Pnext->_Mynextiter;
    }
    // NOTE: after that whihe loop we have iterators list somewhere in memory,
    // they don't rely with any container, but it still a connected list.
    // Firstly, if we have some iterator we can go to next one throught _Mynextiter,
    // Secondly, does we still can to get access to data throught these iterators?
}

// swap owners of proxy and iterators
inline void _Container_base::_Swap_proxy_and_iterators(_Container_base& Rhs) noexcept {
    // swap proxy
    _Container_proxy* Temp = _Myproxy;
    _Myproxy = Rhs._Myproxy;
    Rhs._Myproxy = Temp;

    // swap proxy owners
    if (_Myproxy) { _Myproxy->_Mycont = this; }
    if (Rhs._Myproxy) { Rhs._Myproxy->_Mycont = &Rhs; }
}

_MSL_END
#endif // XMEMORY_HPP
