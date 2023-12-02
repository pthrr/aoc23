#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>

const std::map< std::string, int > str2int_map{
    { "1", 1 },
    { "2", 2 },
    { "3", 3 },
    { "4", 4 },
    { "5", 5 },
    { "6", 6 },
    { "7", 7 },
    { "8", 8 },
    { "9", 9 },
    { "one", 1 },
    { "two", 2 },
    { "three", 3 },
    { "four", 4 },
    { "five", 5 },
    { "six", 6 },
    { "seven", 7 },
    { "eight", 8 },
    { "nine", 9 },
};

auto str2int( const std::string& str ) -> int
{
    return str2int_map.at( str );
}

auto main( int argc, char** argv ) -> int
{
    std::ifstream file{ "data.txt" };

    if( !file.is_open() ) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    int sum{ 0 };
    std::string line{};
    /* std::regex rx_digits{ "(\\d)" }; // part1 */
    std::regex rx_digits{ "(\\d|one|two|three|four|five|six|seven|eight|nine)" }; // part2
    std::optional< std::string > first;
    std::optional< std::string > last;

    for( ; std::getline( file, line ); ) {
        first.reset();
        last.reset();

        for( size_t pos{ 0 }; pos < line.length(); ++pos ) {
            auto start{ line.cbegin() + static_cast< long >( pos ) };
            auto end{ line.cend() };
            std::smatch match;

            if( std::regex_search( start, end, match, rx_digits ) ) {
                if( !first.has_value() ) {
                    first = match[1].str();
                }

                last = match[1].str();
            }
        }

        if( first.has_value() ) {
            sum += 10 * str2int( first.value() ) + str2int( last.value() );
        }
    }

    file.close();
    std::cout << "Sum: " << std::to_string( sum ) << std::endl;
    return 0;
}
