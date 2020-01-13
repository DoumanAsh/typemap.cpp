#include <catch.hpp>

#include <typemap.hpp>

#include <string>
#include <array>

TEST_CASE("it works") {
    type::Map map;

    REQUIRE(map.empty());
    REQUIRE(map.get<int>() == nullptr);
    REQUIRE(!map.has<int>());

    REQUIRE(map.get_or_default<std::vector<int>>().size() == 0);

    REQUIRE(!map.emplace<int>(1).has_value());
    REQUIRE(map.get<int>() != nullptr);
    REQUIRE(*map.get<int>() == 1);
    REQUIRE(map.has<int>());

    REQUIRE(!map.empty());

    REQUIRE(!map.emplace<char>('a').has_value());
    REQUIRE(!map.emplace<std::string>("lolka").has_value());

    REQUIRE(map.get<char>() != nullptr);
    REQUIRE(*map.get<char>() == 'a');
    REQUIRE(*map.get<std::string>() == "lolka");

    REQUIRE(*map.emplace<char>('b') == 'a');
    REQUIRE(*map.remove<char>() == 'b');

    const type::Map& alias = map;
    REQUIRE(alias.get<int>() != nullptr);
}
