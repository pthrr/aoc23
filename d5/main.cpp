#include <fstream>
#include <iostream>
#include <regex>
#include <string>

auto main( int argc, char** argv ) -> int
{
    std::ifstream file{ "data_example1.txt" };

    if( !file.is_open() ) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{};
    std::regex rx_num{ "(\\d+)" };
    std::regex rx_map{ "^([a-zA-Z])-to-([a-zA-Z])\\smap:$" };
    std::regex rx_seeds{ R"(^seeds:\s(\d+)(\s\d+)*$)" };
    std::sregex_iterator line_end;
    std::vector< int > seeds{};

    for( ; std::getline( file, line ); ) {
        std::sregex_iterator line_iterator_seeds{ line.begin(), line.end(), rx_seeds };

        for( ; line_iterator_seeds != line_end; ++line_iterator_seeds ) {
            const std::smatch& matches{ *line_iterator_seeds };

            for( auto match : matches ) {
                seeds.emplace_back( std::stoi( match ) );
            }
        }

        std::sregex_iterator line_iterator_map{ line.begin(), line.end(), rx_map };

        for( ; line_iterator_map != line_end; ++line_iterator_map ) {
            std::cout << ( *line_iterator_map ).str() << std::endl;
        }

        std::sregex_iterator line_iterator_num{ line.begin(), line.end(), rx_num };

        for( ; line_iterator_num != line_end; ++line_iterator_num ) {
            /* std::cout << ( *line_iterator_num ).str() << std::endl; */
        }
    }

    file.close();
    return 0;
}
