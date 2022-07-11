#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <sstream>
#include <utility>
#include <vector>

SCENARIO("At Member Test 1") {
	GIVEN("Valid non-const input") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2};

		CHECK(a.at(0) == 0.0);
		CHECK(a.at(1) == 1.1);
		CHECK(a.at(2) == 2.2);
	}
}

SCENARIO("At Member Test 2") {
	GIVEN("Valid const input") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2};

		CHECK(a.at(0) == 0.0);
		CHECK(a.at(1) == 1.1);
		CHECK(a.at(2) == 2.2);
	}
}

SCENARIO("At Member Test 3") {
	GIVEN("Invalid input: index out of bounds for const euclidean vector") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		CHECK_THROWS_AS(a.at(4), comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(a.at(4), "Index 4 is not valid for this euclidean_vector object");
	}
}

SCENARIO("At Member Test 4") {
	GIVEN("Invalid input: index out of bounds for const euclidean vector") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		CHECK_THROWS_AS(a.at(4), comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(a.at(4), "Index 4 is not valid for this euclidean_vector object");
	}
}

SCENARIO("Dimensions Member Test 1") {
	GIVEN("Valid simple input") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto const b = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		auto const c = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3, 4.4};
		CHECK(a.dimensions() == 3);
		CHECK(a.dimensions() == 4);
		CHECK(a.dimensions() == 5);
	}
}

SCENARIO("Dimensions Member Test 2") {
	GIVEN("Dimensions changing correctly after move operator") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto b = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		a = std::move(b);
		CHECK(b.dimensions() == 0);
	}
	GIVEN("Dimensions changing correctly after copy operator") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto b = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		CHECK(a.dimensions() == 3);
		a = b;
		CHECK(a.dimensions() == 4);
	}
}

SCENARIO("Dimensions Member Test 3") {
	GIVEN("Dimensions changing correctly after move constructor") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto b = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		a = comp6771::euclidean_vector(std::move(b));
		CHECK(b.dimensions() == 0);
	}
	GIVEN("Dimensions changing correctly after copy constructor") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto b = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		CHECK(a.dimensions() == 3);
		a = comp6771::euclidean_vector(b);
		CHECK(a.dimensions() == 4);
	}
}
