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

    REQUIRE(map.emplace<int>(1).second);
    REQUIRE(map.get<int>() != nullptr);
    REQUIRE(*map.get<int>() == 1);
    REQUIRE(map.has<int>());

    REQUIRE(!map.empty());

    REQUIRE(map.emplace<char>('a').second);
    REQUIRE(map.emplace<std::string>("lolka").second);

    REQUIRE(map.get<char>() != nullptr);
    REQUIRE(*map.get<char>() == 'a');
    REQUIRE(*map.get<std::string>() == "lolka");

    REQUIRE(map.emplace<char>('b').first == 'a');
    REQUIRE(!map.emplace<char>('b').second);
    REQUIRE(*map.remove<char>() == 'a');

    const type::Map& alias = map;
    REQUIRE(alias.get<int>() != nullptr);
}
