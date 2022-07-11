#include "gdwg/graph.hpp"

#include <algorithm>
#include <catch2/catch.hpp>
#include <initializer_list>

TEST_CASE("is_node test") {
	auto g = gdwg::graph<int, std::string>{2, 4, 6};
	CHECK(g.is_node(2));
	CHECK(g.is_node(4));
	CHECK(g.is_node(6));
	CHECK(!g.is_node(7));
	g.insert_node(7);
	CHECK(g.is_node(7));
	g.erase_node(4);
	CHECK(!g.is_node(4));
	auto const g1 = gdwg::graph<int, std::string>{2, 4, 6};
	CHECK(g1.is_node(4));
}

TEST_CASE("empty test") {
	auto g = gdwg::graph<int, std::string>{};
	CHECK(g.empty());
	auto g1 = gdwg::graph<int, std::string>{1, 2};
	g1.insert_edge(1, 2, "test!");
	g1.clear();
	CHECK(g1.empty());
	auto const g2 = gdwg::graph<int, std::string>{};
	CHECK(g2.empty());
}

TEST_CASE("is_connected test") {
	auto g = gdwg::graph<int, std::string>{1, 2};
	g.insert_edge(1, 2, "test!");
	CHECK(g.is_connected(1, 2));
	CHECK(!g.is_connected(2, 1));
	auto const g1 = gdwg::graph<int, std::string>{1, 2};
	CHECK(g.is_connected(1, 2));
}

TEST_CASE("is_connected error throw test") {
	auto g = gdwg::graph<int, std::string>{5, 7, 9};
	const auto* m = "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in "
	                "the graph";
	CHECK_THROWS_WITH(g.is_connected(5, 12), m);
	CHECK_THROWS_WITH(g.is_connected(10, 5), m);
	CHECK_THROWS_WITH(g.is_connected(10, 12), m);
}

TEST_CASE("nodes test") {
	auto v = std::vector<int>{2, 4, 6};
	auto g = gdwg::graph<int, std::string>{v.begin(), v.end()};
	CHECK(g.nodes() == v);
	g.insert_node(8);
	v.push_back(8);
	std::sort(v.begin(), v.end());
	CHECK(g.nodes() == v);
	g.erase_node(4);
	auto v1 = std::vector<int>{2, 6, 8};
	CHECK(g.nodes() == v1);
	auto const g1 = gdwg::graph<int, std::string>{v.begin(), v.end()};
	CHECK(g1.nodes() == v);
}

TEST_CASE("weights test") {
	auto g = gdwg::graph<int, std::string>{2, 4, 6};
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 6, "2->6");
	g.insert_edge(6, 4, "6->4");
	auto v = std::vector<std::string>{"s", "e", "t"};
	for (const auto& c : v) {
		g.insert_edge(4, 2, c);
	}
	std::sort(v.begin(), v.end());
	CHECK(g.weights(4, 2) == v);
	auto v1 = std::vector<std::string>{"2->6"};
	CHECK(g.weights(2, 6) == v1);
	g.erase_edge(2, 6, "2->6");
	auto v3 = std::vector<std::string>{};
	CHECK(g.weights(2, 6) == v3);
	auto const g1 = g;
	CHECK(g.weights(2, 6) == v3);
}

TEST_CASE("weights error throw test") {
	auto g = gdwg::graph<int, std::string>{5, 7, 9};
	const auto* m = "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in the "
	                "graph";
	CHECK_THROWS_WITH(g.weights(5, 12), m);
	CHECK_THROWS_WITH(g.weights(10, 5), m);
	CHECK_THROWS_WITH(g.weights(10, 12), m);
}

TEST_CASE("find success test") {
	auto g = gdwg::graph<int, std::string>{2, 4, 6};
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 6, "2->6");
	g.insert_edge(6, 4, "6->4");
	const auto& [from, to, weight] = *g.find(2, 6, "2->6");
	CHECK(from == 2);
	CHECK(to == 6);
	CHECK(weight == "2->6");
	auto const g1 = g;
	const auto& [from1, to1, weight1] = *g1.find(2, 6, "2->6");
	CHECK(from1 == 2);
	CHECK(to1 == 6);
	CHECK(weight1 == "2->6");
}

TEST_CASE("find fail test") {
	auto g = gdwg::graph<int, std::string>{2, 4, 6};
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 6, "2->6");
	g.insert_edge(6, 4, "6->4");
	auto i = g.find(6, 2, "6->2");
	auto j = g.find(2, 6, "2->4");
	CHECK(i == g.end());
	CHECK(j == g.end());
}

TEST_CASE("connections test") {
	auto g = gdwg::graph<int, std::string>{2, 4, 6};
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 6, "2->6");
	g.insert_edge(4, 6, "4->6");
	CHECK(g.connections(2) == std::vector<int>{4, 6});
	g.erase_edge(4, 6, "4->6");
	auto v1 = std::vector<int>{};
	CHECK(g.connections(4) == v1);
	auto const g1 = g;
	CHECK(g1.connections(4) == v1);
}

TEST_CASE("connections error throw test") {
	auto g = gdwg::graph<int, std::string>{5, 7, 9};
	const auto* m = "Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph";
	CHECK_THROWS_WITH(g.connections(12), m);
}
