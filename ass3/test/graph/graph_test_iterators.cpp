#include "gdwg/graph.hpp"

#include <algorithm>
#include <catch2/catch.hpp>
#include <initializer_list>

TEST_CASE("iterator accessor test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4};
	CHECK(g.begin() == g.end());
}

TEST_CASE("iterator increment test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(1, 4, "1->4");
	g.insert_edge(4, 1, "4->1");
	auto iter = g.begin();
	CHECK((*iter).from == 1);
	CHECK((*iter).to == 2);
	CHECK((*iter).weight == "1->2");
	++iter;
	CHECK((*iter).from == 1);
	CHECK((*iter).to == 4);
	CHECK((*iter).weight == "1->4");
	++iter;
	CHECK((*iter).from == 4);
	CHECK((*iter).to == 1);
	CHECK((*iter).weight == "4->1");
	++iter;
	CHECK(iter == g.end());
}

TEST_CASE("iterator decrement test") {
	auto g = gdwg::graph<int, std::string>{1, 2, 4};
	g.insert_edge(1, 2, "1->2");
	g.insert_edge(1, 4, "1->4");
	g.insert_edge(4, 2, "4->2");
	auto iter = g.end();
	--iter;
	CHECK((*iter).from == 4);
	CHECK((*iter).to == 2);
	CHECK((*iter).weight == "4->2");
	--iter;
	CHECK((*iter).from == 1);
	CHECK((*iter).to == 4);
	CHECK((*iter).weight == "1->4");
	--iter;
	CHECK((*iter).from == 1);
	CHECK((*iter).to == 2);
	CHECK((*iter).weight == "1->2");
}
