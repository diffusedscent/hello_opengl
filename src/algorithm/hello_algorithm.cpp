
#include "algorithm/hello_algorithm.h"
#include <iostream>


namespace hello_algorithm {

HelloAlgorithm::HelloAlgorithm() {}

HelloAlgorithm::~HelloAlgorithm() {}

void HelloAlgorithm::test() const {
    // Test code here
    this->greet();
}

void HelloAlgorithm::greet() const {
    std::cout << "Hello, Algorithm!" << std::endl;
}
}