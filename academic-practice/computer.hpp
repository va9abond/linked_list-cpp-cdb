#include <string>
#include <typeinfo>
#include <iostream>
#include <algorithm>

namespace msl {
class Computer {
public:
    Computer (std::string model = "unknown",
              double price = -1,
              int numCores = -1,
              int ram = -1,
              double diaganal = -1
              ) noexcept :
        model(model),
        price(price),
        numCores(numCores),
        ram(ram),
        diagonal(diaganal)
    {}

    Computer (const Computer&) = default;

    Computer &operator= (const Computer&) = default;

    ~Computer() = default;


    std::string GetModel() const noexcept { return model; }

    double GetPrice() const noexcept { return price; }

    int GetNumCores() const noexcept { return numCores; }

    int GetRam() const noexcept { return ram; }

    double GetDiagonal() const noexcept {return diagonal; }



    bool operator== (const Computer &other) const noexcept {
        if (model == other.model &&
            price == other.price &&
            numCores == other.numCores &&
            ram == other.ram &&
            diagonal == other.diagonal)
        {
            return true;
        } return false;
    }

    bool operator!= (const Computer &other) const noexcept {
        return !(*this == other);
    }

    bool operator> (const Computer &other) const noexcept {
        std::string modelLowerCase = model;
        std::for_each(modelLowerCase.begin(), modelLowerCase.end(),
                      [](unsigned char c) { return std::tolower(c); });


        std::string otherModelLowerCase = other.model;
        std::for_each(otherModelLowerCase.begin(), otherModelLowerCase.end(),
                      [](unsigned char c) { return std::tolower(c); });


        if ( modelLowerCase > otherModelLowerCase ) {
            return true;
        }

        else if ( modelLowerCase == otherModelLowerCase ) {
            if ( price > other.price ) {
                return true;
            } else if ( price == other.price ) {
                if ( diagonal > other.diagonal ) {
                    return true;
                } else if ( diagonal == other.diagonal ) {
                    if ( numCores > other.numCores ) {
                        return true;
                    } else if ( ram > other.ram ) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool operator< (const Computer &other) const noexcept {
        return (!(*this > other) && !(*this != other));
    }

    bool operator>= (const Computer &other) const noexcept {
        return (*this > other || *this == other);
    }

    bool operator<= (const Computer &other) const noexcept {
        return (!(*this > other) || *this == other);
    }


    friend std::ostream &operator<< (std::ostream &output, const Computer &computer);


private:
    std::string model;
    double      price;
    int         numCores;
    int         ram;
    double      diagonal;
};

std::ostream &operator<< (std::ostream & output, const Computer & computer) {
    // if ( typeid(output).name() != typeid(std::ofstream).name() ) {
        output << "{ " <<
            computer.GetModel() << "; " <<
            computer.GetPrice() << "; " <<
            computer.GetDiagonal() << "; "<< 
            computer.GetNumCores() << "; " <<
            computer.GetRam() << " }";
    // }
    // else {
        // output << '\n' <<
        //     computer.GetModel() << " " <<
        //     computer.GetPrice() << " " <<
        //     computer.GetDiagonal() << " " <<
        //     computer.GetNumCores() << " " <<
        //     computer.GetRam() << '\n';
    // }
    return output;
}
}
