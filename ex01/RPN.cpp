#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN& other) : Stack(other.Stack) {}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {
        Stack = other.Stack;
    }
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(const std::string& token) const {
    return (token == "+" || token == "-" || token == "*" || token == "/");
}

bool RPN::isValidNumber(const std::string& token) const {
    if (token[0] == '-')
    {
        if (token.length() != 2) return false;
        if (!std::isdigit(token[1])) return false;
        return true;
    }
    if (token.empty()) return false;
    if (token.length() != 1) return false;
    if (!std::isdigit(token[0])) return false;
    return true;
}

int RPN::performOperation(const std::string& op, int b, int a) const {
    if (op == "+")
        return a + b;
    else if (op == "-")
        return a - b;
    else if (op == "*")
        return a * b;
    else if (op == "/") {
        if (b == 0)
            throw std::runtime_error("Division by zero");
        return a / b;
    }
    throw std::runtime_error("Invalid operator");
}
int RPN::evaluate(const std::string& expression) {
    while (!Stack.empty())
        Stack.pop();
    std::istringstream iss(expression);
    std::string token;
    // ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
    // 55
    while (iss >> token) {
        if (token.find('(') != std::string::npos || token.find(')') != std::string::npos)
            throw std::runtime_error("Error");
        
        if (isValidNumber(token)) {
            int num = std::atoi(token.c_str());
            Stack.push(num);
        }
        else if (isOperator(token))
        {
            if (Stack.size() < 2)
                throw std::runtime_error("Error");
            int b = Stack.top();
            Stack.pop();
            int a = Stack.top();
            Stack.pop();

            int result = performOperation(token, b, a);
            Stack.push(result);
        }
        else
            throw std::runtime_error("Error");
    }
    if (Stack.size() != 1)
        throw std::runtime_error("Error");    
    return Stack.top();
}