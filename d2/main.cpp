#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <string>

struct GameSettings
{
    int id{ 0 };
    int red{ 0 };
    int green{ 0 };
    int blue{ 0 };
};

class Game
{
public:
    int id;
    int red;
    int green;
    int blue;

    explicit Game( const GameSettings& settings = GameSettings{} )
        : id( settings.id )
        , red( settings.red )
        , green( settings.green )
        , blue( settings.blue )
    {
        m_setter_map["red"] = [this]( int val ) { this->red = val; };
        m_setter_map["green"] = [this]( int val ) { this->green = val; };
        m_setter_map["blue"] = [this]( int val ) { this->blue = val; };
        m_getter_map["red"] = [this]() { return this->red; };
        m_getter_map["green"] = [this]() { return this->green; };
        m_getter_map["blue"] = [this]() { return this->blue; };
    }

    auto get( const std::string& key ) const -> int
    {
        return m_getter_map.at( key )();
    }

    auto set( const std::string& key, int val ) const -> void
    {
        m_setter_map.at( key )( val );
    }

    auto operator>=( const Game& other ) const -> bool
    {
        return red >= other.red && green >= other.green && blue >= other.blue;
    }

private:
    std::map< std::string, std::function< void( int ) > > m_setter_map;
    std::map< std::string, std::function< int() > > m_getter_map;
};

auto main( int argc, char** argv ) -> int
{
    std::ifstream file{ "data.txt" };

    if( !file.is_open() ) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    Game problem{ GameSettings{ .red = 12, .green = 13, .blue = 14 } };
    int sum{ 0 };
    int power{ 0 };
    std::string line{};
    std::regex rx_id{ "Game (\\d+):" };
    std::regex rx_rgb{ "(\\d+)\\s(red|green|blue)" };

    for( ; std::getline( file, line ); ) {
        Game draw{};
        std::smatch match;
        std::regex_search( line.cbegin(), line.cend(), match, rx_id );
        draw.id = std::stoi( match[1].str() );
        std::sregex_iterator line_iterator{ line.begin(), line.end(), rx_rgb };
        std::sregex_iterator line_end;

        for( ; line_iterator != line_end; ++line_iterator ) {
            const std::smatch& match{ *line_iterator };
            const int num{ std::stoi( match[1].str() ) };
            const std::string color{ match[2].str() };

            if( draw.get( color ) < num ) {
                draw.set( color, num );
            }
        }

        power += draw.red * draw.green * draw.blue;

        if( problem >= draw ) {
            sum += draw.id;
        }
    }

    file.close();
    std::cout << "Sum: " << std::to_string( sum ) << std::endl;
    std::cout << "Power: " << std::to_string( power ) << std::endl;
    return 0;
}
