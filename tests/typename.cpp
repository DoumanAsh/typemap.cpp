#include <catch.hpp>

#include <iostream>
#include <typemap.hpp>

TEST_CASE("it works") {
    type::Map map;

    REQUIRE(map.get<int>() == nullptr);
    REQUIRE(!map.has<int>());

    REQUIRE(!map.emplace<int>(1).has_value());
    REQUIRE(map.get<int>() != nullptr);
    REQUIRE(*map.get<int>() == 1);
    REQUIRE(map.has<int>());

    REQUIRE(!map.emplace<char>('a').has_value());

    REQUIRE(map.get<char>() != nullptr);
    REQUIRE(*map.get<char>() == 'a');

    REQUIRE(*map.emplace<char>('b') == 'a');
    REQUIRE(*map.remove<char>() == 'b');

    const type::Map& alias = map;
    REQUIRE(alias.get<int>() != nullptr);
}
