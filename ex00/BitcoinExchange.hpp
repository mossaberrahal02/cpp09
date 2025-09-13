#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cfloat>
#include <cerrno>
#include <cmath>

class BitcoinExchange
{
    private:
        std::map<std::string, double> _exchangeRates;
        bool _databaseValid;

        bool isValidDate(const std::string& date) const;
        bool isValidValue(const std::string& value) const;
        bool isLeapYear(int year) const;
        bool isValidDateValues(int year, int month, int day) const;
        double stringToDouble(const std::string& str) const;
        std::string trim(const std::string& str) const;
        std::string findClosestDate(const std::string& date) const;
        bool loadDatabase(const std::string& filename);
        bool hasOverflow(double value, double rate) const;

    public:
        BitcoinExchange();
        BitcoinExchange(const std::string& databaseFile);
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();

        void processInputFile(const std::string& filename) const;
        double getExchangeRate(const std::string& date) const;
        bool isDatabaseValid() const;
};
