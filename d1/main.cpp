#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>

const std::map< std::string, int > word_map{
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

auto str2int( const std::string& word ) -> int
{
    return word_map.at( word );
}

auto main( int argc, char** argv ) -> int
{
    std::ifstream file( "data.txt" );

    if( !file.is_open() ) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    int result{ 0 };
    std::string line{};
    std::regex digits_and_words( "(\\d|one|two|three|four|five|six|seven|eight|nine)" );

    while( std::getline( file, line ) ) {
        auto line_begin = std::sregex_iterator( line.begin(), line.end(), digits_and_words );
        auto line_end = std::sregex_iterator();
        std::optional< std::string > first;
        std::optional< std::string > last;

        for( auto i = line_begin; i != line_end; ++i ) {
            const std::smatch& match = *i;
            std::string match_str = match.str();

            if( !first.has_value() ) {
                first = match_str;
            }

            last = match_str;
        }

        if( first.has_value() ) {
            result += 10 * str2int( first.value() ) + str2int( last.value() );
        }
    }

    file.close();
    std::cout << "Sum: " << std::to_string( result ) << std::endl;
    return 0;
}
