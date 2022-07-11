#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <sstream>
#include <vector>

SCENARIO("Euclidean Normal Utility Test") {
	GIVEN("Valid input") {
		auto const a = comp6771::euclidean_vector{1, 2, 3};
		auto norm = comp6771::euclidean_norm(a);
		CHECK(norm == sqrt(1 * 1 + 2 * 2 + 3 * 3));
	}
	GIVEN("Dimension of 0 should return 0") {
		auto const a = comp6771::euclidean_vector(0);
		auto norm = comp6771::euclidean_norm(a);
		CHECK(norm == 0);
	}
}

SCENARIO("Unit Utility Test 1") {
	GIVEN("Valid input") {
		auto const a = comp6771::euclidean_vector{1, 2, 3};
		auto const b = comp6771::unit(a);
		auto norm = comp6771::euclidean_norm(a);
		CHECK(b.at(0) == a.at(0) / norm);
		CHECK(b.at(1) == a.at(1) / norm);
		CHECK(b.at(2) == a.at(2) / norm);
	}
}

SCENARIO("Unit Utility Test 2") {
	GIVEN("Checking for invalid input: vector with dimension 0") {
		auto const a = comp6771::euclidean_vector(0);
		CHECK_THROWS_AS(comp6771::unit(a), comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(comp6771::unit(a),
		                  "euclidean_vector with no dimensions does not have a unit vector");
	}
}

SCENARIO("Unit Utility Test 3") {
	GIVEN("Checking for invalid input: norm is 0") {
		auto const a = comp6771::euclidean_vector(3);
		CHECK_THROWS_AS(comp6771::unit(a), comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(comp6771::unit(a),
		                  "euclidean_vector with zero euclidean normal does not have a unit vector");
	}
}

SCENARIO("Dot Utility Test 1") {
	GIVEN("Valid simple input") {
		auto a = comp6771::euclidean_vector{1, 2, 3};
		auto b = comp6771::euclidean_vector{1, 2, 3};
		auto const c = comp6771::dot(a, b);
		CHECK(c == 1 * 1 + 2 * 2 + 3 * 3);
	}
}

SCENARIO("Dot Utility Test 2") {
	GIVEN("Valid complex input") {
		auto const a = comp6771::euclidean_vector{1.7894309, 2.872935, 3.972354};
		auto const b = comp6771::euclidean_vector{1.7894309, 2.872935, 3.972354};
		auto const c = comp6771::dot(a, b);
		CHECK(c == 1.7894309 * 1.7894309 + 2.872935 * 2.872935 + 3.972354 * 3.972354);
	}
}

SCENARIO("Dot Utility Test 3") {
	GIVEN("Invalid input: dimensions not the same") {
		auto const a = comp6771::euclidean_vector{1.7894309, 2.872935, 3.972354};
		auto const b = comp6771::euclidean_vector{1.7894309, 2.872935, 3.972354, 5.723392};
		CHECK_THROWS_AS(comp6771::dot(a, b), comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(comp6771::dot(a, b), "Dimensions of LHS(3) and RHS(4) do not match");
	}
}

TEST_CASE("Dot Product 3") {
	auto const a = comp6771::euclidean_vector{1.12345, 2.23432, 3.432423};
	auto const b = comp6771::euclidean_vector{432432.234, 2.432, 3.5435};
	auto const c = comp6771::dot(a, b);
	CHECK_THAT(c - (1.12345 * 432432.234 + 2.23432 * 2.432 + 3.432423 * 3.5435),
	           Catch::Matchers::WithinAbs(0, 0.01));
}
