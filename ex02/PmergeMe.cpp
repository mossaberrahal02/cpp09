#include "PmergeMe.hpp"

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
    if (argc < 2) { std::cerr << "Error: No arguments provided" << std::endl; return false; }

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg.empty() || arg[0] == '-') { std::cerr << "Error" << std::endl; return false; }

        for (size_t j = 0; j < arg.length(); ++j) 
        {
            if (!std::isdigit(arg[j])) { std::cerr << "Error" << std::endl; return false; }
        }

        std::stringstream ss(arg);
        int num;
        ss >> num;
        if (ss.fail() || !ss.eof() || num < 0) { std::cerr << "Error" << std::endl; return false; }
        _vectorContainer.push_back(num);
        _dequeContainer.push_back(num);
    }
    
    return true;
}

void PmergeMe::printSequence(const std::vector<int>& seq, const std::string& prefix) 
{
    std::cout << prefix;
    size_t size = 7;
    size_t printCount = std::min(seq.size(), size);
    for (size_t i = 0; i < printCount; ++i) 
    {
        if (i > 0) std::cout << " ";
        std::cout << seq[i];
    }
    if (seq.size() > 5) std::cout << " [...]" << std::endl;
}

std::vector<size_t> PmergeMe::generateJacobsthalNumbers(size_t n) {
    std::vector<size_t> jacobsthal;
    if (n == 0) return jacobsthal;
    
    jacobsthal.push_back(1);
    if (n == 1) return jacobsthal;
    
    size_t prev = 0;
    size_t curr = 1;
    
    while (curr < n) 
    {
        size_t next = curr + 2 * prev;
        jacobsthal.push_back(next);
        prev = curr;
        curr = next;
    }
    
    return jacobsthal;
}

void PmergeMe::insertInSortedVector(std::vector<int>& sorted, int value, size_t maxPos) {
    if (sorted.empty() || maxPos == 0) {
        sorted.insert(sorted.begin(), value);
        return;
    }
    
    size_t searchEnd = std::min(maxPos, sorted.size());
    
    // Binary search within the allowed range
    size_t left = 0;
    size_t right = searchEnd;
    
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (sorted[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    sorted.insert(sorted.begin() + left, value);
}

std::vector<int> PmergeMe::fordJohnsonSortRecursive(const std::vector<int>& arr) {
    if (arr.size() <= 1) {
        return arr;
    }
    
    if (arr.size() == 2) {
        std::vector<int> result = arr;
        if (result[0] > result[1]) {
            std::swap(result[0], result[1]);
        }
        return result;
    }
    
    // Step 1: Pairing and comparing
    std::vector<int> smaller, larger;
    int unpaired = -1;
    
    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        if (arr[i] <= arr[i + 1]) {
            smaller.push_back(arr[i]);
            larger.push_back(arr[i + 1]);
        } else {
            smaller.push_back(arr[i + 1]);
            larger.push_back(arr[i]);
        }
    }
    
    // Handle odd element
    if (arr.size() % 2 == 1) {
        unpaired = arr[arr.size() - 1];
    }
    
    // Step 2: Recursively sort larger elements
    std::vector<int> sortedLarger = fordJohnsonSortRecursive(larger);
    
    // Step 3: Insert smaller elements using Jacobsthal sequence
    std::vector<int> result = sortedLarger;
    
    if (!smaller.empty()) {
        // Always insert first element first
        insertInSortedVector(result, smaller[0], result.size());
        
        if (smaller.size() > 1) {
            std::vector<size_t> jacobsthal = generateJacobsthalNumbers(smaller.size());
            std::vector<bool> inserted(smaller.size(), false);
            inserted[0] = true; // First element already inserted
            
            // Insert according to Jacobsthal sequence
            for (size_t i = 0; i < jacobsthal.size(); ++i) {
                size_t jacobNum = jacobsthal[i];
                
                // Insert elements in reverse order within each Jacobsthal group
                size_t start = (i == 0) ? 2 : jacobsthal[i - 1] + 1;
                size_t end = std::min(jacobNum, smaller.size());
                
                for (size_t j = end; j >= start && j >= 1; --j) {
                    size_t idx = j - 1;
                    if (idx < smaller.size() && !inserted[idx]) {
                        // Find the position of corresponding larger element
                        size_t maxPos = result.size();
                        for (size_t k = 0; k < result.size(); ++k) {
                            if (result[k] == larger[idx]) {
                                maxPos = k;
                                break;
                            }
                        }
                        
                        insertInSortedVector(result, smaller[idx], maxPos);
                        inserted[idx] = true;
                    }
                }
            }
            
            // Insert any remaining elements
            for (size_t i = 1; i < smaller.size(); ++i) {
                if (!inserted[i]) {
                    size_t maxPos = result.size();
                    for (size_t k = 0; k < result.size(); ++k) {
                        if (result[k] == larger[i]) {
                            maxPos = k;
                            break;
                        }
                    }
                    insertInSortedVector(result, smaller[i], maxPos);
                }
            }
        }
    }
    
    // Insert unpaired element if exists
    if (unpaired != -1) {
        insertInSortedVector(result, unpaired, result.size());
    }
    
    return result;
}

void PmergeMe::fordJohnsonSort(std::vector<int>& container)
{
    container = fordJohnsonSortRecursive(container);
}

void PmergeMe::insertInSortedDeque(std::deque<int>& sorted, int value, size_t maxPos) {
    if (sorted.empty() || maxPos == 0) {
        sorted.insert(sorted.begin(), value);
        return;
    }
    
    size_t searchEnd = std::min(maxPos, sorted.size());
    
    // Binary search within the allowed range
    size_t left = 0;
    size_t right = searchEnd;
    
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (sorted[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    sorted.insert(sorted.begin() + left, value);
}

std::deque<int> PmergeMe::fordJohnsonSortRecursive(const std::deque<int>& arr) {
    if (arr.size() <= 1) return arr;

    if (arr.size() == 2) 
    {
        std::deque<int> result = arr;
        if (result[0] > result[1]) std::swap(result[0], result[1]);
        return result;
    }
    
    // Step 1: Pairing and comparing
    std::deque<int> smaller, larger;
    int unpaired = -1;
    
    for (size_t i = 0; i + 1 < arr.size(); i += 2)
    {
        if (arr[i] <= arr[i + 1])
        {
            smaller.push_back(arr[i]);
            larger.push_back(arr[i + 1]);
        }
        else
        {
            smaller.push_back(arr[i + 1]);
            larger.push_back(arr[i]);
        }
    }

    // Handle odd element
    if (arr.size() % 2 == 1) unpaired = arr[arr.size() - 1];

    // Step 2: Recursively sort larger elements
    std::deque<int> sortedLarger = fordJohnsonSortRecursive(larger);
    
    // Step 3: Insert smaller elements using Jacobsthal sequence
    std::deque<int> result = sortedLarger;
    
    if (!smaller.empty()) {
        // Always insert first element first
        insertInSortedDeque(result, smaller[0], result.size());

        if (smaller.size() > 1)
        {
            std::vector<size_t> jacobsthal = generateJacobsthalNumbers(smaller.size());
            std::vector<bool> inserted(smaller.size(), false);
            inserted[0] = true; // first element inserted

            // Insert according to Jacobsthal sequence
            for (size_t i = 0; i < jacobsthal.size(); ++i) {
                size_t jacobNum = jacobsthal[i];
                
                // Insert elements in reverse order within each Jacobsthal group
                size_t start = (i == 0) ? 2 : jacobsthal[i - 1] + 1;
                size_t end = std::min(jacobNum, smaller.size());
                
                for (size_t j = end; j >= start && j >= 1; --j) 
                {
                    size_t idx = j - 1;
                    if (idx < smaller.size() && !inserted[idx]) 
                    {
                        // Find the position of corresponding larger element
                        size_t maxPos = result.size();
                        for (size_t k = 0; k < result.size(); ++k)
                        {
                            if (result[k] == larger[idx]) 
                            {
                                maxPos = k;
                                break;
                            }
                        }
                        insertInSortedDeque(result, smaller[idx], maxPos);
                        inserted[idx] = true;
                    }
                }
            }
            
            // Insert any remaining elements
            for (size_t i = 1; i < smaller.size(); ++i) 
            {
                if (!inserted[i]) 
                {
                    size_t maxPos = result.size();
                    for (size_t k = 0; k < result.size(); ++k) 
                    {
                        if (result[k] == larger[i]) 
                        {
                            maxPos = k;
                            break;
                        }
                    }
                    insertInSortedDeque(result, smaller[i], maxPos);
                }
            }
        }
    }
    
    // Insert unpaired element if exists
    if (unpaired != -1) {
        insertInSortedDeque(result, unpaired, result.size());
    }
    
    return result;
}

void PmergeMe::fordJohnsonSort(std::deque<int>& container)
{
    container = fordJohnsonSortRecursive(container);
}

void PmergeMe::run(int argc, char **argv)
{
    if (!parseInput(argc, argv)) return;
    
    if (_vectorContainer.empty()) { std::cerr << "Error: No valid integers provided" << std::endl; return; }
    printSequence(_vectorContainer, "Before: ");

    double startTime = getTime();
    fordJohnsonSort(_vectorContainer);
    double endTime = getTime();
    double vectorTime = endTime - startTime;

    startTime = getTime();
    fordJohnsonSort(_dequeContainer);
    endTime = getTime();
    double dequeTime = endTime - startTime;

    printSequence(_vectorContainer, "After: ");

    std::cout << "Time to process a range of " << _vectorContainer.size() 
              << " elements with std::vector : " << std::fixed << std::setprecision(5) 
              << vectorTime << " us" << std::endl;
    
    std::cout << "Time to process a range of " << _dequeContainer.size() 
              << " elements with std::deque : " << std::fixed << std::setprecision(5) 
              << dequeTime << " us" << std::endl;
}
