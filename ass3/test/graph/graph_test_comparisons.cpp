#include "gdwg/graph.hpp"

#include <algorithm>
#include <catch2/catch.hpp>
#include <initializer_list>

TEST_CASE("operator== success test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(5, 3, "5->3");
	auto g1 = gdwg::graph<int, std::string>(g);
	CHECK(g == g1);
}

TEST_CASE("operator== fail test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(5, 3, "5->3");
	auto g1 = gdwg::graph<int, std::string>{1, 2, 3};
	g.insert_edge(1, 2, "1->2");
	CHECK(g != g1);
}

TEST_CASE("operator== same graph test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(5, 3, "5->3");
	CHECK(g == g);
}
