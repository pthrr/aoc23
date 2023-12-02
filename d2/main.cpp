#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

using Game = std::map< std::string, int >;

auto main( int argc, char** argv ) -> int
{
    std::ifstream file{ "data.txt" };

    if( !file.is_open() ) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    Game problem{ { "red", 12 }, { "green", 13 }, { "blue", 14 } };
    int sum{ 0 };
    int power{ 0 };
    std::string line{};
    std::regex rx_id{ "Game (\\d+):" };
    std::regex rx_rgb{ "(\\d+)\\s(red|green|blue)" };

    for( ; std::getline( file, line ); ) {
        Game draw{ { "id", 0 }, { "red", 0 }, { "green", 0 }, { "blue", 0 } };
        std::smatch match;
        std::regex_search( line.cbegin(), line.cend(), match, rx_id );
        draw.at( "id" ) = std::stoi( match[1].str() );
        std::sregex_iterator line_iterator{ line.begin(), line.end(), rx_rgb };
        std::sregex_iterator line_end;

        for( ; line_iterator != line_end; ++line_iterator ) {
            const std::smatch& match = *line_iterator;
            const int num = std::stoi( match[1].str() );
            const std::string color = match[2].str();

            if( draw.at( color ) < num ) {
                draw.at( color ) = num;
            }
        }

        power += draw.at( "red" ) * draw.at( "green" ) * draw.at( "blue" );

        if( problem.at( "red" ) >= draw.at( "red" ) && problem.at( "green" ) >= draw.at( "green" ) &&
            problem.at( "blue" ) >= draw.at( "blue" ) ) {
            sum += draw.at( "id" );
        }
    }

    file.close();
    std::cout << "Sum: " << std::to_string( sum ) << std::endl;
    std::cout << "Power: " << std::to_string( power ) << std::endl;
    return 0;
}
