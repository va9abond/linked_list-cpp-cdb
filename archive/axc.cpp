#include <iostream> 
#include <iterator>
#include <stdint.h>






#define _AXC_BEGIN namespace axc {
#define _AXC_END   }
#define _AXC       ::axc::
#define _STD       ::std::


_AXC_BEGIN // auxiliary core to replace functions from STD S
// ************ utility ************ //
	
template <class _Ty>
constexpr _Ty* addressof (_Ty& _Val) noexcept { // copy of std::addressof()
    // return __builtin_addressof(_Val);	
    return &_Val;
}

template <class _Ty>
const _Ty* addressof (const _Ty&&) = delete; // copy of std::addressof()



// STRUCT TEMPLATE remove_reference // full copy from STD
template <class _Ty>
struct remove_reference {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&> {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty&;
};

template <class _Ty>
struct remove_reference<_Ty&&> {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty&&;
};

template <class _Ty>
using remove_reference_t = typename remove_reference<_Ty>::type;

template <class _Ty>
constexpr remove_reference_t<_Ty>&& move (_Ty&& _Arg) noexcept { // forward _Arg as movable // copy from STD
    return static_cast<remove_reference_t<_Ty>&&>(_Arg);
}

template <class _Ty>
constexpr _Ty&& forward (
    remove_reference_t<_Ty>& _Arg) noexcept { // forward an lvalue as either an lvalue or an rvalue
    return static_cast<_Ty&&>(_Arg);
}

// template <class _Ty> // https://habr.com/ru/post/568306/ 
// constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) noexcept { // forward an rvalue as an rvalue
//     static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
//     return static_cast<_Ty&&>(_Arg);
// }

template <class _Ty>
void swap (_Ty& _Left, _Ty& _Right) noexcept { // *WARNING* no check if _Ty is is_nothrow_move_constructible_v && is_nothrow_move_assignable_v
	if ( addressof(_Left) != addressof(_Right) )
	{
	    _Ty _Tmp =  move(_Left);
	    _Left    =  move(_Right);
	    _Right   =  move(_Tmp);
	}
}

template <class _Ty>
void destroy_ (_Ty* _Ptr) {
	_Ptr->~_Ty();
}



// ************ Container_base_ ************ //

struct _Iterator_base;
struct _Container_base;

struct _Container_proxy { // struct to connect _Container_base and _Iterator_base
	_Container_proxy() noexcept = default;
	_Container_proxy (_Container_base* Mycont) noexcept : _Mycont(Mycont) {} 


	const   _Container_base* _Mycont      = nullptr;
	mutable _Iterator_base*  _Myfirstiter = nullptr;
};

struct _Container_base {
	
	_Container_base() noexcept = default;

	_Container_base (const _Container_base&) = delete;
	_Container_base& operator= (const _Container_base&) = delete;

	void _Orphan_all() noexcept;
	void _Swap_proxy_and_iterators(_Container_base&) noexcept;

	~_Container_base() {
		delete _Myproxy; _Myproxy = nullptr;
	}

	void Alloc_proxy () {
		_Container_proxy* const New_proxy = new _Container_proxy(this);  // TODO: *WARNING* where i need to delete _Myproxy
		_Myproxy = New_proxy;
		// New_proxy->_Mycont = this;
	}


	_Container_proxy* _Myproxy = nullptr;
};



// ************ Iterator_base_ ************ //

struct _Iterator_base { // store links to container and next iterator

	_Iterator_base() noexcept = default; // construct orphaned iterator
	
	_Iterator_base (const _Iterator_base& Right) noexcept {
		*this = Right;
	}

	_Iterator_base& operator= (const _Iterator_base& Right) noexcept {
		_Assign(Right);
		_Myproxy = Right._Myproxy;
		
		return *this;
	}

	~_Iterator_base() {
		this->_Orphan_me();
		_Myproxy    = nullptr;
		_Mynextiter = nullptr;
	}

	const _Container_base* Getcont() const noexcept {
		return _Myproxy->_Mycont != nullptr ? _Myproxy->_Mycont : nullptr;
	}


	mutable _Container_proxy* _Myproxy    = nullptr;
	mutable _Iterator_base*   _Mynextiter = nullptr;


private:
	void _Adopt (const _Container_base* Parent) noexcept {
		if (Parent == nullptr) {
			_Orphan_me(); return;
		}

		_Container_proxy* Parent_proxy = Parent->_Myproxy;
		if (_Myproxy != Parent_proxy) { // change parentage
			if (_Myproxy != nullptr) { // already have a parent
				_Orphan_me();
			}
			_Mynextiter = Parent_proxy->_Myfirstiter; // TODO: how it works??? 
			Parent_proxy->_Myfirstiter = this;        // TODO: how it works???
			_Myproxy = Parent_proxy;
		}
	}

	void _Orphan_me() { // TODO: how it works?
		if (_Myproxy == nullptr) { // already orphaned
			return;
		}

		// adopted, remove self from list
		_Iterator_base** Pnext = &_Myproxy->_Myfirstiter;
		while (*Pnext != nullptr && *Pnext != this) {
			const auto Tmp = *Pnext;
			Pnext = &Tmp->_Mynextiter;
		}

		try {
			if (*Pnext == nullptr) {
				throw exception("ITERATOR LIST CORRUPTED!");
			}

			*Pnext   = _Mynextiter;
			_Myproxy = nullptr;
		}
		catch (_AXC exception& corrupted_list) { // TODO: why there is a warning?
			std::cerr << corrupted_list.what();
		}
	}

	void _Assign (const _Iterator_base& Right) noexcept {
		if (_Myproxy == Right._Myproxy) {
			return;
		}

		if (Right._Myproxy != nullptr) {
			this->_Adopt(Right._Myproxy->_Mycont);
		}
		else {
			this->_Orphan_me();
		}
	}

};


void _Container_base::_Orphan_all() noexcept {
	if (_Myproxy == nullptr) { // no proxy, already orphaned
		return;
	}

	// proxy allocated, drain it 
	for (auto& Pnext = _Myproxy->_Myfirstiter; Pnext != nullptr; Pnext = Pnext->_Mynextiter) {
		Pnext->_Myproxy = nullptr;
	}
	_Myproxy->_Myfirstiter = nullptr;
}

void _Container_base::_Swap_proxy_and_iterators (_Container_base& Right) noexcept {
	_Container_proxy* Tmp = _Myproxy;
	_Myproxy              = Right._Myproxy;
	Right._Myproxy        = Tmp;

	if (_Myproxy != nullptr) {
		_Myproxy->_Mycont = this; // Right._Myproxy->_Mycont = this;
	}

	if (Right._Myproxy != nullptr) {
		Right._Myproxy->_Mycont = &Right; // _Myproxy = &Right;
	}
}


template <class IterTy_>
struct iterator_traits { // traits for iterators
	using value_type        = typename IterTy_::value_type;
	using difference_type   = typename IterTy_::difference_type;
	using iterator_category = typename IterTy_::iterator_category;
	using pointer           = typename IterTy_::pointer;
	using reference         = typename IterTy_::reference;
};


template <class PtrTy_>
struct iterator_traits<PtrTy_*> { // traits for pointers
	using value_type        = PtrTy_;
	using difference_type   = std::ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;
	using pointer           = PtrTy_*;
	using reference         = PtrTy_&;
};


template <class ValTy_>
struct _Simple_types { // wraps types from allocators with simple addressing for use in iterators
                       // and other SCARY machinery
	using value_type      = ValTy_;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;
	using pointer         = value_type*;
	using const_pointer   = const value_type*;
}; // full copy from STD


template <class _Alloc>
struct _Default_allocator_traits { // traits for std::allocator
    using allocator_type = _Alloc;
    using value_type     = typename _Alloc::value_type;

    using pointer            = value_type*;
    using const_pointer      = const value_type*;
    using void_pointer       = void*;
    using const_void_pointer = const void*;

    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    template <class _Other>
    using rebind_alloc = std::allocator<_Other>;

}; // full copy from STD








_AXC_END

