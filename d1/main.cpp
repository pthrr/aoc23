#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>

const std::map< std::string, int > word_map{
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

auto word2int( const std::string& word ) -> int
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
            /* std::cout << "First: " << first.value() << std::endl; */
            /* std::cout << "Last: " << last.value() << std::endl; */

            if( first.value().size() > 1 ) {
                result += 10 * word2int( first.value() );
            }
            else {
                result += 10 * std::stoi( first.value() );
            }

            if( last.value().size() > 1 ) {
                result += word2int( last.value() );
            }
            else {
                result += std::stoi( last.value() );
            }
        }
    }

    file.close();
    std::array< char, 50 > buffer{};
    sprintf( buffer.data(), "%d", result );
    std::cout << "Sum: " << buffer.data() << std::endl;
    return 0;
}
