#ifndef HELLO_ALGORITHM_H
#define HELLO_ALGORITHM_H

#include <vector>
#include <algorithm>
#include <numeric>

namespace hello_algorithm {

    class HelloAlgorithm {
    public: 
        explicit HelloAlgorithm();
        ~HelloAlgorithm();
        void test() const;
        void greet() const;
    };

} // namespace hello_algorithm

#endif // HELLO_ALGORITHM_H
