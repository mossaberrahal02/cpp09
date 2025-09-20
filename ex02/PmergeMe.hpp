#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include <sys/time.h>
#include <iomanip>

class PmergeMe {
private:
    std::vector<int> _vectorContainer;
    std::deque<int> _dequeContainer;

    bool parseInput(int argc, char **argv);
    double getTime();
    void printSequence(const std::vector<int>& seq, const std::string& prefix);

    std::vector<size_t> generateJacobsthalNumbers(size_t n);
    
    void binaryInsert(std::vector<int>& arr, int value, size_t end);
    void binaryInsert(std::deque<int>& arr, int value, size_t end);

    void fordJohnsonSort(std::vector<int>& arr);
    void fordJohnsonSort(std::deque<int>& arr);
    
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    void run(int argc, char **argv);
};

#endif
