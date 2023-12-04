#include "../../list.cpp"

_MSL_BEGIN
/* ------------------------------------------------- */

template <
    class Val_t
>
class stack : public list_v2<Val_t> {
public:
    using Mybase = list_v2<Val_t>;
    using Mybase::Mybase;

    using iterator = typename Mybase::iterator;
    using const_iterator = typename Mybase::const_iterator;


    iterator insert (Val_t&& Val) { // insert at end
        return Mybase::insert(Mybase::cend(), std::move(Val));
    }

    iterator insert (const Val_t& Val) { // insert at end
        return Mybase::insert(Mybase::cend(), Val);
    }

    decltype(auto) emplace (Val_t&& Val) { // emplace object by Val at end
        return Mybase::emplace_end(std::move(Val));
    }

    iterator erase () { // erase element at begin
        return Mybase::erase(Mybase::cend());
    }
};

/* ------------------------------------------------- */
_MSL_END
