#ifndef MYCONT_HPP
#define MYCONT_HPP

#include "../../list.cpp"

_MSL_BEGIN
/* ------------------------------------------------- */

template <
    class Val_t
>
class general_container : public list_v2<Val_t> {
public:
    using Mybase = list_v2<Val_t>;
    // using Mybase::Mybase;

    using iterator = typename Mybase::iterator;
    using const_iterator = typename Mybase::const_iterator;

    iterator insert (Val_t&& Val) { // insert by order
        iterator pos = Mybase::begin();
        while (pos != Mybase::end()) {
            if (*pos < Val) { // ascending order
                ++pos;
            } else {
                break;
            }
        }
        return Mybase::insert(pos, std::move(Val));
    }

    iterator erase () { // erase element at begin
        return Mybase::erase(Mybase::cbegin());
    }

};
/* ------------------------------------------------- */
}
#endif
