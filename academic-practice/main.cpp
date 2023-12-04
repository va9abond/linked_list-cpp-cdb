/* -------------------------------------- */
// STL includes
#include <iterator>
#include <list>

/* -------------------------------------- */
// STUDENT includes
#include "computer.hpp"
#include "utils.cpp"

#include "queue/queue.hpp"
#include "my_container/general_container.hpp"

/* -------------------------------------- */


#define _STD  ::std::
#define _msl  ::msl::



int main() {
    // -------------- Задание 1.1 --------------
    std::cout << '\n' << "-------------- Задание 1.1 --------------" << '\n';

    // Демонстрация работы функции push()
    std::list<double> doubleList;
    for (int i = 0; i < 5; i++) {
        _msl push<std::list, double>(doubleList, _msl generateDouble(0, 100));
    }
    _msl print<std::list, double>(doubleList);

    // Демонстрация работы функции pop()
    _msl pop(doubleList, 3);
    _msl print<std::list, double>(doubleList);

    // Демонстрация работы функции filter()
    auto listResult = _msl filter<std::list, double>(doubleList, &msl::EvaluateFractionalPart, 0.45);
    _msl print(listResult); std::cout << '\n';



    // -------------- Задание 1.2 --------------
    std::cout << '\n' << "-------------- Задание 1.2 --------------" << '\n';

    std::list<msl::Computer> computerList;

    msl::Computer Apple  ((std::string) "apple",  1000, 16, 64, 17.8);
    msl::Computer Lenovo ((std::string) "lenovo", 500,  16, 16, 15.0);
    msl::Computer Huawei ((std::string) "huawei", 400,  16, 32, 16.7);
    msl::Computer HP     ((std::string) "hp",     350,   4,  8, 14.5);
    msl::Computer Dell   ((std::string) "dell",   200,   4,  8, 16.2);

    _msl push(computerList, Apple);
    _msl push(computerList, Lenovo);
    _msl push(computerList, Huawei);
    _msl push(computerList, HP);
    _msl push(computerList, Dell);

    _msl print(computerList);

    auto computerHvalue = _msl POP(computerList, 2);
    std::cout << '\n' << '\n' << computerHvalue << '\n';

    _msl print(computerList); std::cout << '\n';



    // -------------- Задание 1.3 --------------
    std::cout << '\n' << "-------------- Задание 1.3 --------------" << '\n';
    msl::queue<int> my_queue;

    my_queue.insert(42); my_queue.insert(24); my_queue.insert(8);
    _msl print(my_queue);

    my_queue.erase(); _msl print(my_queue);
    my_queue.erase(); _msl print(my_queue);


    msl::general_container<double> my_container;
    for (int i = 0; i < 10; ++i) {
        my_container.insert(msl::generateDouble(1.0f, 100.0f));
    }
    print(my_container);

    msl::queue<double> res_queue = msl::general_filter<msl::general_container, msl::queue, double>(
        my_container,
        [](double val){ return val > 50; }
    );
    print(res_queue);

    // -------------- Задание 1.4 --------------
    std::cout << "\n" << "Task 1.4 already complete, check iterators for class `msl::list_v2` and function `msl::print<...>(...)`" << "\n";


    // -------------- Задание 1.5 --------------
    std::cout << "\n" << "Task 1.5 already complete, cause class `msl::queue` is a template, it can hols objects of type `msl::Computer` and insert + erase for implemented with iterators in `msl::list_v2`\n";
}

