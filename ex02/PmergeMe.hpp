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
    
    // Jacobsthal numbers generation
    std::vector<size_t> generateJacobsthalNumbers(size_t n);
    
    // Ford-Johnson algorithm for std::vector
    void fordJohnsonSort(std::vector<int>& container);
    std::vector<int> fordJohnsonSortRecursive(const std::vector<int>& arr);
    void insertInSortedVector(std::vector<int>& sorted, int value, size_t maxPos);
    
    // Ford-Johnson algorithm for std::deque
    void fordJohnsonSort(std::deque<int>& container);
    std::deque<int> fordJohnsonSortRecursive(const std::deque<int>& arr);
    void insertInSortedDeque(std::deque<int>& sorted, int value, size_t maxPos);
    
    // Utility functions
    bool parseInput(int argc, char **argv);
    double getTime();
    void printSequence(const std::vector<int>& seq, const std::string& prefix);
    
public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();
    
    void run(int argc, char **argv);
};

#endif
