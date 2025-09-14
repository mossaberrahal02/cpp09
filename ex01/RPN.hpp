#pragma once

#include <string>
#include <stack>
#include <sstream>
#include <cstdlib>
#include <iostream>

class RPN {
private:
    std::stack<int> Stack;
    
    bool isOperator(const std::string& token) const;
    bool isValidNumber(const std::string& token) const;
    int performOperation(const std::string& op, int b, int a) const;
    
public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();  

    int evaluate(const std::string& expression);
};