#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>

TEST_CASE("basic constructor test") {
	auto g = gdwg::graph<int, std::string>();
	CHECK(g.empty());
}

TEST_CASE("initializer list constructor test") {
	auto g = gdwg::graph<int, std::string>{2, 4, 6};
	CHECK(g.is_node(2));
	CHECK(g.is_node(4));
	CHECK(g.is_node(6));
}

TEST_CASE("iterator constructor test") {
	auto const v = std::vector<int>{2, 4, 6};
	auto g = gdwg::graph<int, std::string>(v.begin(), v.end());
	CHECK(g.is_node(2));
	CHECK(g.is_node(4));
	CHECK(g.is_node(6));
}

TEST_CASE("move constructor test") {
	auto g = gdwg::graph<int, std::string>{2, 4, 6};
	auto g1 = gdwg::graph<int, std::string>(std::move(g));
	CHECK(g.empty());
	CHECK(g1.is_node(2));
	CHECK(g1.is_node(4));
	CHECK(g1.is_node(6));
}

TEST_CASE("move operator test") {
	auto g = gdwg::graph<int, std::string>{2, 4, 6};
	auto g1 = std::move(g);
	CHECK(g.empty());
	CHECK(g1.is_node(2));
	CHECK(g1.is_node(4));
	CHECK(g1.is_node(6));
}

TEST_CASE("copy constructor test") {
	auto g = gdwg::graph<int, std::string>{2, 4};
	auto g1 = gdwg::graph<int, std::string>(g);
	CHECK(g.is_node(2));
	CHECK(g.is_node(4));
	CHECK(g1.is_node(2));
	CHECK(g1.is_node(4));
	CHECK(g.nodes() == g1.nodes());
	CHECK(&g != &g1);
}

TEST_CASE("copy operator test") {
	auto g = gdwg::graph<int, std::string>{2, 4};
	auto g1 = g;
	CHECK(g.is_node(2));
	CHECK(g.is_node(4));
	CHECK(g1.is_node(2));
	CHECK(g1.is_node(4));
	CHECK(g.nodes() == g1.nodes());
	CHECK(&g != &g1);
}
