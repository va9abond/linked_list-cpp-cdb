#include <cfloat>
#include <cstdlib>
#include <iostream>
#include <stdexcept>


namespace msl {
double generateDouble(double value_min, double value_max) {
    double value = (double)rand() / RAND_MAX;
    return value_min + value * (value_max - value_min);
}


template <
    template<class...> class TContainer,
    class TObject
>
void push (TContainer<TObject> &container, const TObject &object) {
    auto itt = container.begin();

    while ( itt != container.end() ) {
        if ( *itt >= object ) { break; }
        itt++;
    }

    container.insert(itt, object);
}


template <
    template<class...> class TContainer,
    class TObject
>
typename TContainer<TObject>::iterator pop (TContainer<TObject> &container, const int &pos) {
    if (pos >= static_cast<int>(container.size()) || pos < 0) {
        throw std::out_of_range("function pop: Index out of range");
    }

    auto objectToRemove = std::next(container.begin(), pos);
    return container.erase(objectToRemove);

}



template <
    template<class...> class TContainer,
    class TObject
>
TObject POP (TContainer<TObject> &container, const int &pos) {
    if (pos >= static_cast<int>(container.size()) || pos < 0) {
        throw std::out_of_range("function pop: Index out of range");
    }

    auto objectToRemove = std::next(container.begin(), pos);
    container.erase(objectToRemove);

    auto objectToReturn = container.back();
    return objectToReturn;

}



template< 
    template <class... > class _Container_t,
    class _Value_t
>
void print(const _Container_t<_Value_t>& Cont) {
    auto It = Cont.begin(); std::cout << "\n" << "{ ";
    while (It != Cont.end()) {
        std::cout << *(It++) << " ";
    }
    std::cout << "};" << "\n";
}



bool EvaluateFractionalPart (const double p, const double value) {
    int value_ceil = int(value);
    double diff = value - value_ceil;
    return (diff - p < DBL_EPSILON);
}


template <
    template<class...> class TContainer,
    class TObject
>
TContainer<TObject> filter (const TContainer<TObject> &container,
                            bool (*func_key)(const TObject, const TObject),
                            double param)
{
    TContainer<TObject> containerResult;

    for (auto object : container) {
        if ( func_key(param, object) == true ) {
            containerResult.push_back(object);
        }
    }
    return containerResult;
}

template <
    template<class...> class Cont_input_t,
    template<class...> class Cont_output_t, // should be stack or queue or general_container
    class Val_t
>
Cont_output_t<Val_t> general_filter (const Cont_input_t<Val_t> &Cont,
                                     bool (*func_key)(Val_t)) {
    Cont_output_t<Val_t> Cont_output;
    for (Val_t value : Cont) {
        if (func_key(value) == true) {
            Cont_output.insert(value);
        }
    }
    return Cont_output;
}
}
