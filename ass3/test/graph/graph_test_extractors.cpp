#include "gdwg/graph.hpp"

#include <algorithm>
#include <catch2/catch.hpp>
#include <initializer_list>
#include <sstream>

TEST_CASE("basic extractor test") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{
	   {4, 1, -4},
	   {3, 2, 2},
	   {2, 4, 2},
	   {2, 1, 1},
	   {6, 2, 5},
	   {6, 3, 10},
	   {1, 5, -1},
	   {3, 6, -8},
	   {4, 5, 3},
	   {5, 2, 7},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		g.insert_edge(from, to, weight);
	}

	g.insert_node(64);
	auto out = std::ostringstream{};
	out << g;

	auto os1 = std::ostringstream{};
	os1 << g;
	auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
	CHECK(os1.str() == expected_output);
}