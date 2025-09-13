/* #include "BitcoinExchange.hpp"
int main(int ac, char **av)
{
    (void)av;
    if ( ac != 2 ) {std::cerr << "Error: could not open file." << std::endl; return 1;}

    std::ifstream data("data.csv");
    if ( !data ) {std::cerr << "Error opening database" << std::endl; return 1;}
    
    std::string line;
    while (getline(data, line)) 
    {
        std::cout << line << std::endl;
        key
        value
        // is issue found in key or value throw exception
        
        // if (year %4 && year % 100) ! leap year
        // if (year % 4 || year % 400) leap year
        
        month [] = {31, }
        
        // if ( year % 4 || (year % 400 && year % 100 ))

        database[key] = value;



    }

    // BitcoinExchange btc(av[1]);

    return 0;
} 
*/

#include "BitcoinExchange.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) { std::cerr << "Error: could not open file." << std::endl; return 1; }

    BitcoinExchange exchange;

    if (!exchange.isDatabaseValid()) return 1;

    exchange.processInputFile(argv[1]);
    
    return 0;
}