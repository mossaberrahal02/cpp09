#include "PmergeMe.hpp"
#include <iostream>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) : _vectorContainer(other._vectorContainer), _dequeContainer(other._dequeContainer) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vectorContainer = other._vectorContainer;
        _dequeContainer = other._dequeContainer;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

double PmergeMe::getTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

bool PmergeMe::parseInput(int argc, char **argv) {
    if (argc < 2) { 
        std::cerr << "Error" << std::endl; 
        return false; 
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg.empty() || arg[0] == '-') { 
            std::cerr << "Error" << std::endl; 
            return false; 
        }

        for (size_t j = 0; j < arg.length(); ++j) {
            if (!std::isdigit(arg[j])) { 
                std::cerr << "Error" << std::endl; 
                return false; 
            }
        }

        std::stringstream ss(arg);
        int num;
        ss >> num;
        if (ss.fail() || !ss.eof() || num < 0) { 
            std::cerr << "Error" << std::endl; 
            return false; 
        }
        
        _vectorContainer.push_back(num);
        _dequeContainer.push_back(num);
    }
    
    return true;
}

void PmergeMe::printSequence(const std::vector<int>& seq, const std::string& prefix) {
    std::cout << prefix << std::endl;
    for (size_t i = 0; i < seq.size() ; ++i) {
        if (i > 0) std::cout << " ";
        std::cout << seq[i];
    }
    std::cout << std::endl;
}

std::vector<size_t> PmergeMe::generateJacobsthalNumbers(size_t n) {
    std::vector<size_t> jacobsthal;
    if (n <= 1) return jacobsthal;
    
    jacobsthal.push_back(1);
    if (n == 2) return jacobsthal;
    
    size_t prev = 1;
    size_t curr = 1;
    
    while (curr < n) {
        size_t next = curr + 2 * prev;
        if (next >= n) break;
        jacobsthal.push_back(next);
        prev = curr;
        curr = next;
    }
    
    return jacobsthal;
}

void PmergeMe::binaryInsert(std::vector<int>& arr, int value, size_t end) {
    size_t searchEnd = std::min(end, arr.size());
    std::vector<int>::iterator pos = std::lower_bound(arr.begin(), arr.begin() + searchEnd, value);
    arr.insert(pos, value);
}

void PmergeMe::binaryInsert(std::deque<int>& arr, int value, size_t end) {
    size_t searchEnd = std::min(end, arr.size());
    std::deque<int>::iterator pos = std::lower_bound(arr.begin(), arr.begin() + searchEnd, value);
    arr.insert(pos, value);
}

void PmergeMe::fordJohnsonSort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;
    
    std::vector<std::pair<int, int> > pairs;
    bool hasStraggler = false;
    int straggler = 0;
    
    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        if (arr[i] <= arr[i + 1]) {
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        } else {
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
        }
    }
    
    if (arr.size() % 2 == 1) {
        hasStraggler = true;
        straggler = arr[arr.size() - 1];
    }
    
    if (pairs.empty())
    {
        if (hasStraggler) {
            arr.clear();
            arr.push_back(straggler);
        }
        return;
    }

    std::vector<int> largerElements;
    for (size_t i = 0; i < pairs.size(); ++i) 
    {
        largerElements.push_back(pairs[i].second);
    }
    fordJohnsonSort(largerElements);
    std::vector<int> mainChain = largerElements;
    std::cout << std::endl;

    /* insert smaller elements using Jacobsthal sequence */
    std::vector<int> pendingElements;
    for (size_t i = 0; i < pairs.size(); ++i) {
        pendingElements.push_back(pairs[i].first);
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    
    if (!pendingElements.empty()) {
        binaryInsert(mainChain, pendingElements[0], mainChain.size());

        if (pendingElements.size() > 1) {
            std::vector<size_t> jacobsthal = generateJacobsthalNumbers(pendingElements.size());
            std::vector<bool> inserted(pendingElements.size(), false);
            inserted[0] = true;
            
            for (size_t i = 0; i < jacobsthal.size(); ++i) {
                size_t jacobNum = jacobsthal[i];
                size_t start = (i == 0) ? 2 : jacobsthal[i - 1] + 1;
                size_t end = std::min(jacobNum, pendingElements.size());
                
                for (size_t j = end; j >= start && j >= 1; --j) {
                    size_t idx = j - 1;
                    if (idx < pendingElements.size() && !inserted[idx]) {
                        /* find position of the corresponding larger element */
                        size_t limit = mainChain.size();
                        for (size_t k = 0; k < mainChain.size(); ++k) {
                            if (mainChain[k] == pairs[idx].second) {
                                limit = k + 1;
                                break;
                            }
                        }
                        binaryInsert(mainChain, pendingElements[idx], limit);
                        inserted[idx] = true;
                    }
                }
            }
            
            for (size_t i = 1; i < pendingElements.size(); ++i) {
                if (!inserted[i]) {
                    size_t limit = mainChain.size();
                    for (size_t k = 0; k < mainChain.size(); ++k) {
                        if (mainChain[k] == pairs[i].second) {
                            limit = k + 1;
                            break;
                        }
                    }
                    binaryInsert(mainChain, pendingElements[i], limit);
                }
            }
        }
    }
    if (hasStraggler) 
        binaryInsert(mainChain, straggler, mainChain.size());
    arr = mainChain;
}

void PmergeMe::fordJohnsonSort(std::deque<int>& arr) {
    if (arr.size() <= 1) return;

    std::vector<std::pair<int, int> > pairs;
    bool hasStraggler = false;
    int straggler = 0;
    
    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        if (arr[i] <= arr[i + 1]) {
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        } else {
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
        }
    }
    
    if (arr.size() % 2 == 1) {
        hasStraggler = true;
        straggler = arr[arr.size() - 1];
    }
    
    if (pairs.empty()) {
        if (hasStraggler) {
            arr.clear();
            arr.push_back(straggler);
        }
        return;
    }

    std::deque<int> largerElements;
    for (size_t i = 0; i < pairs.size(); ++i) {
        largerElements.push_back(pairs[i].second);
    }
    
    fordJohnsonSort(largerElements);

    std::deque<int> mainChain = largerElements;

    std::vector<int> pendingElements;
    for (size_t i = 0; i < pairs.size(); ++i) {
        pendingElements.push_back(pairs[i].first);
    }
    
    if (!pendingElements.empty()) {
        binaryInsert(mainChain, pendingElements[0], mainChain.size());
        
        if (pendingElements.size() > 1) {
            std::vector<size_t> jacobsthal = generateJacobsthalNumbers(pendingElements.size());
            std::vector<bool> inserted(pendingElements.size(), false);
            inserted[0] = true;
            
            for (size_t i = 0; i < jacobsthal.size(); ++i) {
                size_t jacobNum = jacobsthal[i];
                size_t start = (i == 0) ? 2 : jacobsthal[i - 1] + 1;
                size_t end = std::min(jacobNum, pendingElements.size());
                
                for (size_t j = end; j >= start && j >= 1; --j) {
                    size_t idx = j - 1;
                    if (idx < pendingElements.size() && !inserted[idx]) {
                        size_t limit = mainChain.size();
                        for (size_t k = 0; k < mainChain.size(); ++k) {
                            if (mainChain[k] == pairs[idx].second) {
                                limit = k + 1;
                                break;
                            }
                        }
                        binaryInsert(mainChain, pendingElements[idx], limit);
                        inserted[idx] = true;
                    }
                }
            }

            for (size_t i = 1; i < pendingElements.size(); ++i) {
                if (!inserted[i]) {
                    size_t limit = mainChain.size();
                    for (size_t k = 0; k < mainChain.size(); ++k) {
                        if (mainChain[k] == pairs[i].second) {
                            limit = k + 1;
                            break;
                        }
                    }
                    binaryInsert(mainChain, pendingElements[i], limit);
                }
            }
        }
    }

    if (hasStraggler) {
        binaryInsert(mainChain, straggler, mainChain.size());
    }
    
    arr = mainChain;
}

void PmergeMe::run(int argc, char **argv) {
    if (!parseInput(argc, argv)) return;
    
    if (_vectorContainer.empty()) { 
        std::cerr << "Error" << std::endl; 
        return; 
    }
    
    printSequence(_vectorContainer, "Before:");


    double startTime = getTime();
    fordJohnsonSort(_vectorContainer);
    double endTime = getTime();
    double vectorTime = endTime - startTime;

    startTime = getTime();
    fordJohnsonSort(_dequeContainer);
    endTime = getTime();
    double dequeTime = endTime - startTime;

    printSequence(_vectorContainer, "After:");

    std::cout << std::endl
              << "Time to process a range of " << _vectorContainer.size() 
              << " elements with std::vector : " << std::fixed << std::setprecision(5) 
              << vectorTime << " us" << std::endl;
    
    std::cout << "Time to process a range of " << _dequeContainer.size() 
              << " elements with std::deque : " << std::fixed << std::setprecision(5) 
              << dequeTime << " us" << std::endl;
}
