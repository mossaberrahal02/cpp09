#include "BitcoinExchange.hpp"
#include <cstdlib>

BitcoinExchange::BitcoinExchange() : _databaseValid(false)
{
    _databaseValid = loadDatabase("data.csv");
}

BitcoinExchange::BitcoinExchange(const std::string& databaseFile) : _databaseValid(false)
{
    _databaseValid = loadDatabase(databaseFile);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) 
    : _exchangeRates(other._exchangeRates), _databaseValid(other._databaseValid)
{
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
    {
        _exchangeRates = other._exchangeRates;
        _databaseValid = other._databaseValid;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

bool BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open()) { std::cerr << "Error: could not open file." << std::endl; return false; }

    std::string line;

    if (!std::getline(file, line)) 
        { std::cerr << "Error: invalid database format." << std::endl; file.close(); return false; }
    
    // Parse header line with same flexibility as data lines
    size_t commaPos = line.find(',');
    if (commaPos == std::string::npos) 
        { std::cerr << "Error: invalid database format." << std::endl; file.close(); return false; }
    
    std::string headerDate = trim(line.substr(0, commaPos));
    std::string headerRate = trim(line.substr(commaPos + 1));
    
    if (headerDate != "date" || headerRate != "exchange_rate")
        { std::cerr << "Error: invalid database format." << std::endl; file.close(); return false; }

    int lineNumber = 1;
    bool hasAtLeastOneEntrie = false;

    while (std::getline(file, line))
    {
        lineNumber++;
        if (trim(line).empty())
            continue;
        hasAtLeastOneEntrie = true;
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos)
        {
            std::cerr << "Error: invalid format at line " << lineNumber << ": missing comma" << std::endl;
            file.close();
            return false;
        }
        if (line.find(',', commaPos + 1) != std::string::npos)
        {
            std::cerr << "Error: invalid format at line " << lineNumber << ": multiple commas" << std::endl;
            file.close();
            return false;
        }
        std::string date = trim(line.substr(0, commaPos));
        std::string rateStr = trim(line.substr(commaPos + 1));
        if (!isValidDate(date))
            { std::cerr << "Error: invalid date at line " << lineNumber << ": " << date << std::endl; file.close(); return false; }
        if (!isValidValue(rateStr))
            { std::cerr << "Error: invalid rate at line " << lineNumber << ": " << rateStr << std::endl; file.close(); return false; }
        double rate = stringToDouble(rateStr);
        if (rate < 0) { std::cerr << "Error: negative rate at line " << lineNumber << ": " << rate << std::endl; file.close(); return false; }
        if (_exchangeRates.find(date) != _exchangeRates.end())
        {
            std::cerr << "Error: duplicate date at line " << lineNumber << ": " << date << std::endl;
            file.close();
            return false;
        }
        _exchangeRates[date] = rate;
    }
    file.close();
    if (!hasAtLeastOneEntrie) { std::cerr << "Error: database contains no entries." << std::endl; return false; }
    if (_exchangeRates.empty()) { std::cerr << "Error: database contains no valid entries." << std::endl; return false; }
    return true;
}

bool BitcoinExchange::isDatabaseValid() const { return _databaseValid; }

void BitcoinExchange::processInputFile(const std::string& filename) const
{
    if (!_databaseValid) { std::cerr << "Error: database is not valid, cannot process input." << std::endl; return; }
    std::ifstream file(filename.c_str());
    if (!file.is_open()) { std::cerr << "Error: could not open file." << std::endl; return; }
    std::string line;
    if (!std::getline(file, line)) 
        { std::cerr << "Error: invalid input file format." << std::endl; file.close(); return; }
    
    // Parse header line with same flexibility as data lines
    size_t pipePos = line.find('|');
    if (pipePos == std::string::npos) 
        { std::cerr << "Error: invalid input file format." << std::endl; file.close(); return; }
    
    std::string headerDate = trim(line.substr(0, pipePos));
    std::string headerValue = trim(line.substr(pipePos + 1));
    
    if (headerDate != "date" || headerValue != "value")
        { std::cerr << "Error: invalid input file format." << std::endl; file.close(); return; }
    while (std::getline(file, line))
    {
        if (trim(line).empty()) continue;
        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos) { std::cerr << "Error: bad input => " << trim(line) << std::endl; continue; }
        std::string date = trim(line.substr(0, pipePos));
        std::string valueStr = trim(line.substr(pipePos + 1));
        if (!isValidDate(date)) { std::cerr << "Error: bad input => " << date << std::endl; continue; }
        if (!isValidValue(valueStr))
        {
            if (valueStr.empty() || valueStr[0] == '-')
                std::cerr << "Error: not a positive number." << std::endl;
            else
                std::cerr << "Error: too large a number." << std::endl;
            continue;
        }
        double value = stringToDouble(valueStr);
        if (value < 0) { std::cerr << "Error: not a positive number." << std::endl; continue; }
        if (value > 1000) { std::cerr << "Error: too large a number." << std::endl; continue; }
        double rate = getExchangeRate(date);
        // if (rate == 0.0)
        //     continue;
        if (hasOverflow(value, rate)) { std::cerr << "Error: calculation overflow." << std::endl; continue; }
        double result = value * rate;
        std::cout << date << " => " << value << " = " << result << std::endl;
    }
    file.close();
}

double BitcoinExchange::getExchangeRate(const std::string& date) const
{
    if (!_databaseValid)
        return 0.0;
    std::map<std::string, double>::const_iterator it = _exchangeRates.find(date);
    if (it != _exchangeRates.end())
        return it->second;
    std::string closestDate = findClosestDate(date);
    if (!closestDate.empty())
        return _exchangeRates.find(closestDate)->second;
    return 0.0;
}

std::string BitcoinExchange::findClosestDate(const std::string& date) const
{
    std::string closest = "";
    for (std::map<std::string, double>::const_iterator it = _exchangeRates.begin(); it != _exchangeRates.end(); ++it)
    {
        if (it->first < date)
        {
            if (closest.empty() || it->first > closest)
                closest = it->first;
        }
        else if (it->first == date)
            return it->first;
    }
    return closest;
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
    if (date.length() != 10)
        return false;

    if (date[4] != '-' || date[7] != '-')
        return false;

    std::string yearStr = date.substr(0, 4);
    std::string monthStr = date.substr(5, 2);
    std::string dayStr = date.substr(8, 2);

    for (int i = 0; i < 4; ++i)
    {
        if (yearStr[i] < '0' || yearStr[i] > '9')
            return false;
    }
    for (int i = 0; i < 2; ++i)
    {
        if (monthStr[i] < '0' || monthStr[i] > '9')
            return false;
        if (dayStr[i] < '0' || dayStr[i] > '9')
            return false;
    }
    int year = (int)(stringToDouble(yearStr));
    int month = (int)(stringToDouble(monthStr));
    int day = (int)(stringToDouble(dayStr));

    return isValidDateValues(year, month, day);
}

bool BitcoinExchange::isLeapYear(int year) const
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool BitcoinExchange::isValidDateValues(int year, int month, int day) const
{
    if (year < 1000 || year > 9999)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year))
        daysInMonth[1] = 29;//fubrary
    if (day > daysInMonth[month - 1])
        return false;
    return true;
}

bool BitcoinExchange::hasOverflow(double value, double rate) const
{
    if (rate == 0.0)
        return false;
    double maxVal = std::numeric_limits<double>::max();
    if (value > maxVal / rate)
        return true;
    return false;
}

bool BitcoinExchange::isValidValue(const std::string& value) const
{
    if (value.empty())
        return false;
    std::string trimmedValue = trim(value);
    if (trimmedValue.empty())
        return false;
    char* endPtr;
    double val = strtod(trimmedValue.c_str(), &endPtr);
    (void)val;
    if (*endPtr != '\0')
        return false;    
    return true;
}

double BitcoinExchange::stringToDouble(const std::string& str) const
{
    std::stringstream ss(str);
    double result;
    ss >> result;
    return result;
}

std::string BitcoinExchange::trim(const std::string& str) const
{
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos)
        return "";
    
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}