#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

auto float_equal( double a, double b, double tol = 1e-6 ) -> bool
{
    return std::fabs( a - b ) < tol;
}

struct PositionSettings
{
    int x{};
    int y{};
};

struct Position
{
    int x{};
    int y{};

    explicit Position( const PositionSettings& settings = PositionSettings{} )
        : x( settings.x )
        , y( settings.y )
    {
    }

    auto operator-( const Position& other ) const -> double
    {
        return std::sqrt( std::pow( x - other.x, 2 ) + std::pow( y - other.y, 2 ) );
    }
};

enum class ObjectType
{
    SYMBOL,
    PART
};

struct ObjectSettings
{
    ObjectType type{};
    int value{};
    std::string name{};
    std::vector< Position > pos{};
};

class Object
{
public:
    ObjectType type{};
    int value{};
    std::string name{};
    bool used{ false };
    std::vector< Position > pos{};

    explicit Object( const ObjectSettings& settings = ObjectSettings{} )
        : type( settings.type )
        , value( settings.value )
        , name( settings.name )
        , pos( settings.pos )
    {
    }

    auto operator-( const Object& other ) -> std::optional< double >
    {
        if( pos.empty() || other.pos.empty() ) {
            return std::nullopt;
        }

        std::vector< double > tmp{};
        std::for_each( pos.begin(), pos.end(), [&]( const Position& pos1 ) {
            std::transform( other.pos.begin(),
                other.pos.end(),
                std::back_inserter( tmp ),
                [pos1]( const Position& pos2 ) { return pos1 - pos2; } );
        } );
        auto min{ std::min_element( tmp.begin(), tmp.end() ) };

        if( min == tmp.end() ) {
            return std::nullopt;
        }

        return std::optional< double >{ *min };
    }
};

auto main( int argc, char** argv ) -> int
{
    std::ifstream file{ "data_example1.txt" };

    if( !file.is_open() ) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line{};
    int pos_line{ 0 };
    std::regex rx_part{ "(\\d+)" };
    std::regex rx_symbol{ "([^a-zA-Z0-9.])" };
    std::vector< Object > objects{};

    for( ; std::getline( file, line ); ++pos_line ) {
        std::sregex_iterator line_end;
        std::sregex_iterator line_iterator_part{ line.begin(), line.end(), rx_part };

        for( ; line_iterator_part != line_end; ++line_iterator_part ) {
            const std::smatch& match{ *line_iterator_part };
            const size_t start_pos{ static_cast< size_t >( match.position() ) };
            const size_t end_pos{ start_pos + match[1].length() - 1 };
            const int value{ std::stoi( match[1].str() ) };
            std::vector< Position > pos{};

            for( int x : std::views::iota( start_pos, end_pos + 1 ) ) {
                pos.emplace_back( Position{ PositionSettings{ .x = x, .y = pos_line } } );
            }

            objects.emplace_back(
                Object{ ObjectSettings{ .type = ObjectType::PART, .value = value, .pos = pos } } );
        }

        std::sregex_iterator line_iterator_symbol{ line.begin(), line.end(), rx_symbol };

        for( ; line_iterator_symbol != line_end; ++line_iterator_symbol ) {
            const std::smatch& match{ *line_iterator_symbol };
            const size_t start_pos{ static_cast< size_t >( match.position() ) };
            const size_t end_pos{ start_pos + match[1].length() - 1 };
            const std::string name{ match[1].str() };
            std::vector< Position > pos{};

            for( int x : std::views::iota( start_pos, end_pos + 1 ) ) {
                pos.emplace_back( Position{ PositionSettings{ .x = x, .y = pos_line } } );
            }

            objects.emplace_back(
                Object{ ObjectSettings{ .type = ObjectType::SYMBOL, .name = name, .pos = pos } } );
        }
    }

    file.close();
    int sum{ 0 };
    auto is_part{ []( const Object& obj ) { return obj.type == ObjectType::PART; } };
    auto is_symbol{ []( const Object& obj ) { return obj.type == ObjectType::SYMBOL; } };

    for( auto& symbol : objects | std::views::filter( is_symbol ) ) {
        for( auto& part : objects | std::views::filter( is_part ) ) {
            std::optional< double > dist{ symbol - part };

            if( dist.has_value() &&
                ( dist.value() < std::sqrt( 2 ) || float_equal( dist.value(), std::sqrt( 2 ) ) ) ) {
                if( !part.used ) {
                    part.used = true;
                    sum += part.value;
                }
            }
        }
    }

    std::cout << "Sum: " << std::to_string( sum ) << std::endl;
    return 0;
}
