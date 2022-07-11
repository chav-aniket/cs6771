#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <stdexcept>

TEST_CASE("insert_node test") {
	auto g = gdwg::graph<int, std::string>{};
	auto n = 5;
	g.insert_node(n);
	CHECK(g.is_node(n));
}

TEST_CASE("insert_edge test") {
	auto g = gdwg::graph<int, std::string>{5, 7, 9};
	CHECK(g.insert_edge(5, 9, "5 -> 9"));
	CHECK(g.insert_edge(5, 9, "5 -> -> 9"));
	CHECK(g.insert_edge(7, 9, "7 -> 9"));
	CHECK(g.is_connected(5, 9));
	CHECK(g.is_connected(7, 9));
}

TEST_CASE("insert_edge error throw test") {
	auto g = gdwg::graph<int, std::string>{5, 7, 9};
	const auto* m = "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does "
	                "not exist";
	CHECK_THROWS_WITH(g.insert_edge(5, 12, "hi"), m);
	CHECK_THROWS_WITH(g.insert_edge(10, 5, "hello"), m);
	CHECK_THROWS_WITH(g.insert_edge(10, 12, "hello"), m);
}

TEST_CASE("replace_node test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 5, "2->5");
	g.replace_node(2, 3);
	CHECK(g.is_node(3));
	CHECK(!g.is_node(2));
	CHECK(g.is_connected(1, 3));
	CHECK(g.is_connected(3, 4));
	CHECK(g.is_connected(3, 5));
}

TEST_CASE("replace_node error throw test") {
	auto g = gdwg::graph<int, std::string>{5, 7, 9};
	const auto* m = "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist";
	CHECK_THROWS_WITH(g.replace_node(10, 12), m);
}

TEST_CASE("merge_replace_node test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 3, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(3, 1, "3->1");
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 5, "2->5");
	g.merge_replace_node(2, 3);
	CHECK(g.is_node(3));
	CHECK(!g.is_node(2));
	CHECK(g.is_connected(1, 3));
	CHECK(g.is_connected(3, 1));
	CHECK(g.is_connected(3, 4));
	CHECK(g.is_connected(3, 5));
}

TEST_CASE("merge_replace_node error throw test") {
	auto g = gdwg::graph<int, std::string>{5, 7, 9};
	const auto* m = "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they "
	                "don't exist in the graph";
	CHECK_THROWS_WITH(g.merge_replace_node(5, 12), m);
	CHECK_THROWS_WITH(g.merge_replace_node(10, 5), m);
	CHECK_THROWS_WITH(g.merge_replace_node(10, 12), m);
}

TEST_CASE("erase_node test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 5, "2->5");
	g.erase_node(2);
	CHECK(!g.is_node(2));
	CHECK(g.connections(1).empty());
}

TEST_CASE("erase_edge test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 5, "2->5");
	CHECK(g.connections(2).size() == 2);
	g.erase_edge(2, 5, "2->5");
	CHECK(g.connections(2).size() == 1);
}

TEST_CASE("erase_edge error throw test") {
	auto g = gdwg::graph<int, std::string>{5, 7, 9};
	const auto* m = "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in "
	                "the graph";
	CHECK_THROWS_WITH(g.erase_edge(5, 12, "hi"), m);
	CHECK_THROWS_WITH(g.erase_edge(10, 5, "hello"), m);
	CHECK_THROWS_WITH(g.erase_edge(10, 12, "hey"), m);
}

TEST_CASE("clear test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 5, "2->5");
	g.clear();
	CHECK(g.empty());
}

TEST_CASE("erase_edge one iterator param test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 5, "2->5");
	auto iter = g.begin();
	++iter;
	const auto& [from, to, weight] = *g.erase_edge(iter);
	CHECK(g.connections(2).size() == 1);
	CHECK(from == 2);
	CHECK(to == 5);
	CHECK(weight == "2->5");
}

TEST_CASE("erase_edge two iterator param test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 5, "2->5");
	auto i = g.erase_edge(g.begin(), ++g.begin());
	CHECK(i == --g.end());
	CHECK(g.connections(1).empty());
	CHECK(g.connections(2).size() == 1);
}

TEST_CASE("erase_edge two iterator param test2") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4, 5};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(2, 4, "2->4");
	g.insert_edge(2, 5, "2->5");
	g.insert_edge(4, 5, "4->5");
	auto i = g.erase_edge(g.begin(), g.end());
	CHECK(i == g.end());
	CHECK(g.connections(1).empty());
	CHECK(g.connections(2).empty());
	CHECK(g.connections(4).empty());
}
