#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <sstream>
#include <vector>

SCENARIO("Constructor Test 1") {
	GIVEN("Constructor with no arguments") {
		auto a = comp6771::euclidean_vector();

		CHECK(a.dimensions() == 1);
		CHECK(a.at(0) == 0.0);
	}
}

SCENARIO("Constructor Test 1.5") {
	GIVEN("Const Constructor with no arguments") {
		auto const a = comp6771::euclidean_vector();

		CHECK(a.dimensions() == 1);
		CHECK(a.at(0) == 0.0);
	}
}

SCENARIO("Constructor Test 2") {
	GIVEN("Constructor with 1 argument") {
		auto a = comp6771::euclidean_vector(3);

		CHECK(a.dimensions() == 3);
		CHECK(a.at(0) == 0.0);
		CHECK(a.at(1) == 0.0);
		CHECK(a.at(2) == 0.0);
	}
}

SCENARIO("Constructor Test 2.5") {
	GIVEN("Const Constructor with 1 argument") {
		auto const a = comp6771::euclidean_vector(3);

		CHECK(a.dimensions() == 3);
		CHECK(a.at(0) == 0.0);
		CHECK(a.at(1) == 0.0);
		CHECK(a.at(2) == 0.0);
	}
}

// Wanted to use a GIVEN but complexity went above limit >:(
SCENARIO("Constructor Test 3: Constructor with 2 arguments passed") {
	auto a = comp6771::euclidean_vector(3, 4.2);

	CHECK(a.dimensions() == 3);
	CHECK(a.at(0) == 4.2);
	CHECK(a.at(1) == 4.2);
	CHECK(a.at(2) == 4.2);
}

SCENARIO("Constructor Test 3.5: Const Constructor with 2 arguments passed") {
	auto const a = comp6771::euclidean_vector(3, 4.2);

	CHECK(a.dimensions() == 3);
	CHECK(a.at(0) == 4.2);
	CHECK(a.at(1) == 4.2);
	CHECK(a.at(2) == 4.2);
}

SCENARIO("Constructor Test 4: Constructing from vector") {
	auto v = std::vector<double>{1.1, 6.9, 4.2, 2.7, 7.11};
	auto a = comp6771::euclidean_vector(v.begin(), v.end());

	CHECK(a.dimensions() == 5);
	CHECK(a.at(0) == 1.1);
	CHECK(a.at(1) == 6.9);
	CHECK(a.at(2) == 4.2);
	CHECK(a.at(3) == 2.7);
	CHECK(a.at(4) == 7.11);
}

SCENARIO("Constructor Test 4.5: Const Constructing from vector") {
	auto v = std::vector<double>{1.1, 6.9, 4.2, 2.7, 7.11};
	auto const a = comp6771::euclidean_vector(v.begin(), v.end());

	CHECK(a.dimensions() == 5);
	CHECK(a.at(0) == 1.1);
	CHECK(a.at(1) == 6.9);
	CHECK(a.at(2) == 4.2);
	CHECK(a.at(3) == 2.7);
	CHECK(a.at(4) == 7.11);
}

SCENARIO("Constructor Test 5: Constructing from initialiser list") {
	auto l = std::initializer_list<double>{1.1, 6.9, 4.2, 2.7, 7.11};
	auto a = comp6771::euclidean_vector(l);

	CHECK(a.dimensions() == 5);
	CHECK(a.at(0) == 1.1);
	CHECK(a.at(1) == 6.9);
	CHECK(a.at(2) == 4.2);
	CHECK(a.at(3) == 2.7);
	CHECK(a.at(4) == 7.11);
}

SCENARIO("Constructor Test 5.5: Const Constructing from initialiser list") {
	auto l = std::initializer_list<double>{1.1, 6.9, 4.2, 2.7, 7.11};
	auto const a = comp6771::euclidean_vector(l);

	CHECK(a.dimensions() == 5);
	CHECK(a.at(0) == 1.1);
	CHECK(a.at(1) == 6.9);
	CHECK(a.at(2) == 4.2);
	CHECK(a.at(3) == 2.7);
	CHECK(a.at(4) == 7.11);
}

SCENARIO("Constructor Test 6: Copy Constructor") {
	auto a = comp6771::euclidean_vector{4.3, 7.1};
	auto b = comp6771::euclidean_vector(a);

	CHECK(a.dimensions() == 2);
	CHECK(a.at(0) == 4.3);
	CHECK(a.at(1) == 7.1);
	CHECK(b.dimensions() == 2);
	CHECK(b.at(0) == 4.3);
	CHECK(b.at(1) == 7.1);
}

SCENARIO("Constructor Test 6.5: Const Copy Constructor") {
	auto a = comp6771::euclidean_vector{4.3, 7.1};
	auto const b = comp6771::euclidean_vector(a);

	CHECK(a.dimensions() == 2);
	CHECK(a.at(0) == 4.3);
	CHECK(a.at(1) == 7.1);
	CHECK(b.dimensions() == 2);
	CHECK(b.at(0) == 4.3);
	CHECK(b.at(1) == 7.1);
}

SCENARIO("Constructor Test 7: Move Constructor") {
	auto a = comp6771::euclidean_vector{4.3, 7.1};
	auto b = comp6771::euclidean_vector(std::move(a));
	CHECK(b.dimensions() == 2);
	CHECK(b.at(0) == 4.3);
	CHECK(a.dimensions() == 0);
	CHECK_THROWS_MATCHES(a.at(0),
	                     comp6771::euclidean_vector_error,
	                     Catch::Matchers::Message("Index 0 is not valid for this euclidean_vector "
	                                              "object"));
}

SCENARIO("Constructor Test 7.5: Const Move Constructor") {
	auto a = comp6771::euclidean_vector{4.3, 7.1};
	auto const b = comp6771::euclidean_vector(std::move(a));
	CHECK(b.dimensions() == 2);
	CHECK(b.at(0) == 4.3);
	CHECK(a.dimensions() == 0);
	CHECK_THROWS_MATCHES(a.at(0),
	                     comp6771::euclidean_vector_error,
	                     Catch::Matchers::Message("Index 0 is not valid for this euclidean_vector "
	                                              "object"));
}
