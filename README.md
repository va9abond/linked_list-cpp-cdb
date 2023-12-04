# msldef.h

| defines     | token string    |
| ----------- | --------------- |
| \_MSL_BEGIN | namespace msl { |
| \_MSL_END   | }               |
| \_MSL       | ::msl::         |
| \_STD       | ::std::         |

| keyword / feature | return | name                 | args               | notes |
| ----------------- | ------ | -------------------- | ------------------ | ----- |
| inline / noexcept | void   | \_MSL_REPORT_ERROR_f | const char\*       | [x]   |
| inline / noexcept | void   | \_MSL_VERIFY_f       | bool, const char\* | [x]   |

# xmemory.hpp

### struct \_Simple_type_traits

| \_Simple_type_traits<\_Value_type> |                    |
| ---------------------------------- | ------------------ |
| value_type                         | \_Value_type       |
| size_type                          | std::size_t        |
| difference_type                    | std::ptrdiff_t     |
| pointer                            | value_type\*       |
| const_pointer                      | const value_type\* |

### struct \_Container_proxy

| \_Container_proxy |                    |
| ----------------- | ------------------ |
| const             | \_Container_base\* |
| mutable           | \_Iterator_base\*  |

| keyword / feature | return            | name                       | args               | notes |
| ----------------- | ----------------- | -------------------------- | ------------------ | ----- |
| noexcept          | \_Container_proxy | default Ctor [ = default ] |                    | [x]   |
| noexcept          | \_Container_proxy | Ctor [ = delete ]          | \_Container_base\* | [x]   |

### struct \_Container_base

| \_Container_base  |
| ----------------- |
| \_Container_proxy |

| keyword / feature | return            | name                       | args                    | notes                 |
| ----------------- | ----------------- | -------------------------- | ----------------------- | --------------------- |
| noexcept          | \_Container_base  | default Ctor [ = default ] |                         | [x]                   |
|                   | \_Container_base  | Ctor [ = delete ]          | const \_Container_base& | [x]                   |
|                   | \_Container_base& | Ctor [ = delete ]          | const \_Container_base& | [x]                   |
| \--------         | \--------         | \--------                  | \--------               | \--------             |
| noexcept          | void              | \_Orphan_all               |                         | [x]                   |
|                   | void              | \_Swap_proxy_and_iterators | \_Container_base&       | [x]                   |
|                   | void              | \_Alloc_proxy              |                         | [x]                   |
| noexcept          | void              | \_Free_proxy               |                         | Doest it really need? |

### struct \_Iterator_base

| \_Iterator_base |                     |              |
| --------------- | ------------------- | ------------ |
| mutable         | \_Container_proxy\* | \_Myproxy    |
| mutable         | \_Iterator_base\*   | \_Mynextiter |

| keyword / feature | return             | name                       | args                     | notes     |
| ----------------- | ------------------ | -------------------------- | ------------------------ | --------- |
| noexcept          | \_Iterator_base    | default Ctor [ = default ] |                          | [x]       |
| noexcept          | \_Iterator_base    | Ctor                       | const \_Iterator_base&   | [x]       |
| noexcept          | \_Iterator_base&   | operator=                  | const \_Iterator_base&   | [x]       |
| noexcept          |                    | Dtor                       |                          | [x]       |
| \--------         | \--------          | \--------                  | \--------                | \-------- |
| noexcept          | \_Container_base\* | \_Get_cont                 |                          | [x]       |
| noexcept          | void               | \_Assign                   | const \_Iterator_base&   | [x]       |
| noexcept          | void               | \_Adopt_me                 | const \_Container_base\* | [x]       |
| noexcept          | void               | \_Orphan_me_v1             |                          | [x]       |
| noexcept          | void               | \_Orphan_me_v2             |                          | [x]       |

# list_node.hpp

### struct \_List_node

| \_List_node\<Value_type\> |                             |
| ------------------------- | --------------------------- |
| value_type                | \_Value_type                |
| \_Nodeptr                 | \_List_node<\_Value_type>\* |

| keyword / feature   | return        | name                       | args                                    | notes                  |
| ------------------- | ------------- | -------------------------- | --------------------------------------- | ---------------------- |
| explicit / template | \_List_node   | Ctor                       | \_Arg_t&&, \_Nodeptr, \_Nodeptr         | \_Arg_t --> value_type |
|                     | \_List_node   | default Ctor [ = default ] |                                         | [x]                    |
|                     | \_List_node   | Ctor [ = delete ]          | const \_List_node&                      | [x]                    |
|                     | \_List_node&  | operator= [ = delete ]     | const \_List_node&                      | [x]                    |
| \--------           | \--------     | \--------                  | \--------                               | \--------              |
| static              | \_Nodeptr     | \_Buy_head_node            |                                         | [x]                    |
| static              | void          | \_Freenode0                | \_Nodeptr                               | [x]                    |
| static              | void          | \_Freenode                 | \_Nodeptr                               | [x]                    |
| static              | void          | \_Free_non_head            | \_Nodeptr                               | [x]                    |
| \--------           | \--------     | \--------                  | \--------                               | \--------              |
| friend              | std::ostream& | operator<<                 | std::ostream&, const \_List_node<\_Ty>& | [x]                    |

# list_iterator.hpp

| Iterators                       |
| ------------------------------- |
| \_List_unchecked_const_iterator |
| \_List_const_iterator           |
| \_List_const_iterator           |
| \_List_iterator                 |

### struct \_List_unchecked_const_iterator

| \_List_unchecked_const_iterator<\_Mylist, \_Base> |                                 |
| ------------------------------------------------- | ------------------------------- |
| iterator_category                                 | std::bidirectional_iterator_tag |
| \_Nodeptr                                         | \_Mylist::\_Nodeptr             |
| value_type                                        | \_Mylist::value_type            |
| difference_type                                   | \_Mylist::difference_type       |
| pointer                                           | \_Mylist::const_pointer         |
| reference                                         | const value_type&               |

| \_List_unchecked_const_iterator |         |
| ------------------------------- | ------- |
| \_Nodeptr                       | \_Myptr |

| keyword / feature           | return                          | name            | args                                  | notes     |
| --------------------------- | ------------------------------- | --------------- | ------------------------------------- | --------- |
| noexcept                    | \_List_unchecked_const_iterator | default Ctor    |                                       |           |
| noexcept                    | \_List_unchecked_const_iterator | Ctor            | \_Nodeptr, const \_Mylist\*           |           |
| \--------                   | \--------                       | \--------       | \--------                             | \-------- |
| const / noexcept            | reference                       | operator\*      |                                       |           |
| const / noexcept            | pointer                         | operator->      |                                       |           |
| explicit / const / noexcept | bool                            | operator bool   |                                       |           |
| noexcept                    | \_List_unchecked_const_iterator | operator++      |                                       |           |
| noexcept                    | \_List_unchecked_const_iterator | operator++(int) |                                       |           |
| noexcept                    | \_List_unchecked_const_iterator | operator--      |                                       |           |
| noexcept                    | \_List_unchecked_const_iterator | operator--(int) |                                       |           |
| const / noexcept            | bool                            | operator==      | const \_List_unchecked_const_iterator |           |
| const / noexcept            | bool                            | operator!=      | const \_List_unchecked_const_iterator |           |

### struct \_List_const_iterator

| \_List_const_iterator | : public \_List_unchecked_const_iterator |
| --------------------- | ---------------------------------------- |
| \_Mybase              | \_List_unchecked_const_iterator          |
| iterator_category     | std::bidirectional_iterator_tag          |
| \_Nodeptr             | \_Mylist::\_Nodeptr                      |
| value_type            | \_Mylist::value_type                     |
| difference_type       | \_Mylist::difference_type                |
| pointer               | \_Mylist::const_pointer                  |
| reference             | const value_type&                        |

| keyword / feature           | return                          | name            | args                                                     | notes     |
| --------------------------- | ------------------------------- | --------------- | -------------------------------------------------------- | --------- |
| const / noexcept            | reference                       | operator\*      |                                                          |           |
| const / noexcept            | pointer                         | operator->      |                                                          |           |
| explicit / const / noexcept | bool                            | operator bool   |                                                          |           |
| noexcept                    | \_List_const_iterator           | operator++      |                                                          |           |
| noexcept                    | \_List_const_iterator           | operator++(int) |                                                          |           |
| noexcept                    | \_List_const_iterator           | operator--      |                                                          |           |
| noexcept                    | \_List_const_iterator           | operator--(int) |                                                          |           |
| const / noexcept            | bool                            | operator==      | const \_List_const_iterator                              |           |
| const / noexcept            | bool                            | operator!=      | const \_List_const_iterator                              |           |
| \--------                   | \--------                       | \--------       | \--------                                                | \-------- |
| friend                      | void                            | \_Verify_range  | const \_List_const_iterator, const \_List_const_iterator |           |
|                             | \_List_unchecked_const_iterator | \_Unwrapped     |                                                          |           |

# list.cpp

### struct \_List_simple_type_traits

| \_List_simple_type_traits<\_Value_type> | : public \_Simple_type_traits |
| --------------------------------------- | ----------------------------- |
| \_Node                                  | \_List_node<\_Value_type>     |
| \_Nodeptr                               | \_List_node<\_Value_type>\*   |

### class \_List_val

| \_List_val<\_Val_types> | : public \_Container_base   |
| ----------------------- | --------------------------- |
| \_Nodeptr               | \_Val_type::\_Nodeptr       |
| value_type              | \_Val_type::value_type      |
| size_type               | \_Val_type::size_type       |
| defference_type         | \_Val_type::difference_type |
| pointer                 | \_Val_type::pointer         |
| const_pointer           | \_Val_type::const_pointer   |
| reference               | value_type&                 |
| const_reference         | const value_type&           |

| \_List_val | : public \_Container_base |
| ---------- | ------------------------- |
| \_Nodeptr  | \_Myhead                  |
| size_type  | \_Mysize                  |

| keyword / feature | return     | name                 | args      | notes     |
| ----------------- | ---------- | -------------------- | --------- | --------- |
|                   | \_List_val | default Ctor         |           |           |
| \--------         | \--------  | \--------            | \-------- | \-------- |
| noexcept          | void       | \_Orphan_iter_on_ptr | \_Nodeptr |           |
| noexcept          | void       | \_Orphan_non_end     |           |           |
| noexcept          | \_Nodeptr  | \_Unlink_node        | \_Nodeptr |           |

### struct \_List_node_emplace_v2

| \_List_node_emplace_v2<\_Node_t> |            |
| -------------------------------- | ---------- |
| pointer                          | \_Node_t\* |

| \_List_node_emplace_v2 |          |
| ---------------------- | -------- |
| pointer                | \_Mynode |

| keyword / feature   | return                 | name                   | args          | notes     |
| ------------------- | ---------------------- | ---------------------- | ------------- | --------- |
| template / explicit | \_List_node_emplace_v2 | Ctor                   | \_Arg_t&&     |           |
|                     |                        | Dtor                   |               |           |
|                     | \_List_node_emplace_v2 | Ctor [ = delete ]      |               |           |
|                     | \_List_node_emplace_v2 | operator= [ = delete ] |               |           |
| \--------           | \--------              | \--------              | \--------     | \-------- |
| noexcept            | pointer                | \_Transfer_before      | const pointer |           |

### struct \_List_node_insert_v2

| \_List_node_insert_v2<\_Node_t> |             |
| ------------------------------- | ----------- |
| value_type                      | \_Node_t    |
| pointer                         | \_Node_t\*  |
| size_type                       | std::size_t |

| \_List_node_insert_v2 |         |
| --------------------- | ------- |
| size_type             | \_Added |
| pointer               | \_Head  |
| pointer               | \_Tail  |

| keyword / feature | return                | name                     | args                                   | notes     |
| ----------------- | --------------------- | ------------------------ | -------------------------------------- | --------- |
| explicit          | \_List_node_insert_v2 | default Ctor             |                                        |           |
|                   | \_List_node_insert_v2 | Ctor [ = delete ]        |                                        |           |
|                   | \_List_node_insert_v2 | Ctor [ = delete ]        |                                        |           |
|                   |                       | Dtor                     |                                        |           |
| \--------         | \--------             | \--------                | \--------                              | \-------- |
|                   | void                  | \_Append_n               | size_type                              |           |
| template          | void                  | \_Append_range_unchecked | \_Iter_t1, \_Iter_t2                   |           |
| template          | pointer               | \_Attach_before          | \_List_val<\_Val_type>&, const pointer |           |
| template          | void                  | \_Attach_at_end          | \_List_val<\_Val_type>&                |           |
| template          | void                  | \_Attach_head            | \_List_val<\_Val_type>&                |           |

### class list_v2

| list_v2<\_Ty>              |                                                   |
| -------------------------- | ------------------------------------------------- |
| \_Alloc                    | std::allocator<\_Ty>                              |
| \_Node                     | \_List_node<\_Ty>                                 |
| \_Nodeptr                  | \_List_node<\_Ty>\*                               |
| \--------                  | \--------                                         |
| \_Val_types                | \_List_simple_type_traits<\_Ty>                   |
| \_List_SCARY_val           | \_List_val<\_Val_types>                           |
| \_List_node_insert         | \_List_node_insert_v2<\_Node>                     |
| \_List_node_emplace        | \_List_node_emplace_v2<\_Node>                    |
| \--------                  | \--------                                         |
| value_type                 | \_Ty                                              |
| allocator_type             | \_Alloc                                           |
| size_type                  | std::size_t                                       |
| difference_type            | std::ptrdiff_t                                    |
| pointer                    | \_Ty\*                                            |
| const_pointer              | const \_Ty\*                                      |
| reference                  | value_type&                                       |
| const_reference            | const value_type&                                 |
| \--------                  | \--------                                         |
| iterator                   | \_List_iterator<\_List_SCARY_val>                 |
| const_iterator             | \_List_const_iterator<\_List_SCARY_val>           |
| \_Unchecked_iterator       | \_List_unchecked_iterator<\_List_SCARY_val>       |
| \_Unchecked_const_iterator | \_List_const_unchecked_iterator<\_List_SCARY_val> |
| \--------                  | \--------                                         |
| reverse_iterator           | std::reverse_iterator<iterator>                   |
| const_reverse_iterator     | std::reverse_iterator<const_iterator>             |

| list_v2          |          |
| ---------------- | -------- |
| \_List_SCARY_val | \_Mycont |

| keyword / feature | return          | name                        | args                                       | notes                                        |
| ----------------- | --------------- | --------------------------- | ------------------------------------------ | -------------------------------------------- |
|                   | list_v2         | default Ctor                |                                            |                                              |
| explicit          | list_v2         | Ctor                        | size_type                                  | [x]                                          |
|                   | list_v2         | Ctor                        | size_type, const \_Ty&                     | [x]                                          |
|                   | list_v2         | Ctor                        | const list_v2&                             | [x]                                          |
|                   | list_v2         | Ctor                        | list_v2&&                                  | ??                                           |
|                   | list_v2         | Ctor                        | std::init_list                             |                                              |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
|                   | void            | \_Construct_n               | size_type                                  | [x]                                          |
|                   | void            | \_Construct_n               | size_type, const \_Ty&                     | [x]                                          |
| template          | void            | \_Construct_range_unchecked | \_Iter_t1, const \_Iter_t2                 |                                              |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
|                   | list_v2&        | operator=                   | list_v2&&                                  | TODO:                                        |
|                   | list_v2&        | operator=                   | std::init_list                             | TODO: assign (\_Iter_t1, \_Iter_t2)          |
|                   | list_v2&        | operator=                   | const list_v2&                             | TODO:                                        |
|                   | void            | assign                      | std::init_list                             | TODO: assign (\_Iter_t1, \_Iter_t2)          |
|                   |                 | Dtor                        |                                            |                                              |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
| noexcept          | void            | \_Swap_val                  | list_v2&                                   |                                              |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
|                   | void            | push_front                  | \_Ty&&                                     | [x]                                          |
|                   | void            | push_back                   | \_Ty&&                                     | [x]                                          |
|                   | void            | push_front                  | \_Ty&                                      |                                              |
|                   | void            | push_back                   | \_Ty&                                      | [x]                                          |
|                   | decltype(auto)  | emplace_front               | \_Ty&&                                     | [x]                                          |
|                   | decltype(auto)  | emplace_back                | \_Ty&&                                     | [x]                                          |
|                   | iterator        | emplace                     | const_iterator, \_Ty&&                     | TODO: template                               |
|                   | \_Nodeptr       | emplace                     | const \_Nodeptr, \_Ty&&                    | TODO: template                               |
|                   | iterator        | insert                      | const_iterator, \_Ty&&                     |                                              |
|                   | iterator        | insert                      | const_iterator, std::init_list             |                                              |
|                   | iterator        | insert                      | const_iterator, const \_Ty&                |                                              |
|                   | iterator        | insert                      | const_iterator, size_type, const \_Ty&     | TODO:                                        |
| template          | iterator        | insert                      | \_Iter_t, \_Iter_t                         | TODO: verify range; 2 iter types in template |
|                   | void            | pop_front                   |                                            | [x]                                          |
|                   | void            | pop_back                    |                                            | [x]                                          |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
|                   | all iterators   |                             |                                            |                                              |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
|                   | void            | resize                      | size_type                                  | [x]                                          |
|                   | void            | resize                      | size_type, const \_Ty&                     | [x]                                          |
| const / noexcept  | size_type       | size                        |                                            | [x]                                          |
| const / noexcept  | size_type       | max_size                    |                                            | [x]                                          |
| const / noexcept  | bool            | empty                       |                                            | [x]                                          |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
| noexcept          | reference       | front                       |                                            | [x]                                          |
| const / noexcept  | const_reference | front                       |                                            | [x]                                          |
| noexcept          | reference       | back                        |                                            | [x]                                          |
| const / noexcept  | const_reference | back                        |                                            | [x]                                          |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
| noexcept          | iterator        | erase                       | const const_iterator                       |                                              |
| noexcept          | iterator        | erase                       | const const_iterator, const const_iterator | TODO: verify range                           |
| noexcept          | \_Nodeptr       | \_Unchecked_erase           | const \_Nodeptr                            |                                              |
| noexcept          | \_Nodeptr       | \_Unchecked_erase           | \_Nodeptr, const \_Nodeptr                 |                                              |
| noexcept          | void            | clear                       |                                            | [x]                                          |
| noexcept          | void            | \_Tidy                      |                                            | [x]                                          |
| noexcept          | swap            | list_v2&                    |                                            |                                              |
| \--------         | \--------       | \--------                   | \--------                                  | \--------                                    |
|                   | void            | \_Alloc_head_and_proxy      |                                            | [x]                                          |
| noexcept          | void            | \_Orphan_all                |                                            | [x]                                          |
