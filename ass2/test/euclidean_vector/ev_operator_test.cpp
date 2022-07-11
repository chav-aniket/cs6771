#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <sstream>
#include <vector>

SCENARIO("Copy Operator Test 1") {
	GIVEN("A non-const euclidean vector") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		auto b = comp6771::euclidean_vector{2.2, 1.1, 0.0};

		a = b;
		CHECK(a.dimensions() == 3);
		CHECK(a.at(0) == 2.2);
	}
}

SCENARIO("Copy Operator Test 2") {
	GIVEN("A const euclidean vector") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		auto const b = comp6771::euclidean_vector{2.2, 1.1, 0.0};

		a = b;
		CHECK(a.dimensions() == 3);
		CHECK(a.at(0) == 2.2);
	}
}

SCENARIO("Move Operator Test") {
	GIVEN("A valid move between two vectors") {
		auto a = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		auto b = comp6771::euclidean_vector{2.2, 1.1, 0.0};

		a = std::move(b);
		CHECK(a.dimensions() == 3);
		CHECK(a.at(0) == 2.2);
		CHECK(a.at(1) == 1.1);
		CHECK(a.at(2) == 0.0);
	}
}

SCENARIO("Subscript Operator Test 1") {
	GIVEN("A non-const euclidean vector") {
		auto a = comp6771::euclidean_vector{0.0, 1.1};

		CHECK(a[1] == 1.1);
	}
}

SCENARIO("Subscript Operator Test 2") {
	GIVEN("A const euclidean vector") {
		auto const b = comp6771::euclidean_vector{0.0, 1.1};

		CHECK(b[1] == 1.1);
	}
}

SCENARIO("Unary Plus Operator Test 1") {
	auto a = comp6771::euclidean_vector{2.2, 1.1};
	auto b = +a;
	// We check that they don't have the same reference as they are copies
	CHECK(&a != &b);
	// We ensure the operator hasn't mutated our original vector
	CHECK(a.dimensions() == 2);
	CHECK(a.at(0) == 2.2);
	CHECK(b.dimensions() == 2);
	CHECK(b.at(0) == 2.2);
	CHECK(b.at(1) == 1.1);
}

SCENARIO("Unary Plus Operator Test 2") {
	GIVEN("A valid euclidean vector") {
		auto a = comp6771::euclidean_vector{2.2, 1.1};
		auto b = +a;
		// We ensure the vector has been successfully copied over
		CHECK(b.dimensions() == 2);
		CHECK(b.at(0) == 2.2);
		CHECK(b.at(1) == 1.1);
	}
}

SCENARIO("Negation Operator Test") {
	GIVEN("Valid input") {
		auto a = comp6771::euclidean_vector{-6, 1};
		auto b = comp6771::euclidean_vector{6, -1};
		auto c = -b;

		CHECK(&b != &c);
		CHECK(a == c);
	}
}

SCENARIO("Compound Addition Operator Test 1") {
	GIVEN("Valid input") {
		auto a = comp6771::euclidean_vector{-6.54321, 1.23456};
		auto b = comp6771::euclidean_vector{6.54321, -1.23456};
		a += b;
		CHECK(a.dimensions() == 2);
		CHECK(a.at(0) == 0);
		CHECK(a.at(1) == 0);
	}
}

SCENARIO("Compound Addition Operator Test 2") {
	GIVEN("Invalid input: different dimensions") {
		auto a = comp6771::euclidean_vector{1, 2, 3};
		auto b = comp6771::euclidean_vector{6, -1};
		CHECK_THROWS_AS(a += b, comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(a -= b, "Dimensions of LHS(3) and RHS(2) do not match");
	}
}

SCENARIO("Compound Subtraction Operator Test 1") {
	GIVEN("Valid input") {
		auto a = comp6771::euclidean_vector{-6, 1};
		auto b = comp6771::euclidean_vector{6, -1};
		a -= b;
		CHECK(a.dimensions() == 2);
		CHECK(a.at(0) == -12);
		CHECK(a.at(1) == 2);
	}
}

SCENARIO("Compound Subtraction Operator Test 2") {
	GIVEN("Invalid input: different dimensions") {
		auto a = comp6771::euclidean_vector{1, 2, 3};
		auto b = comp6771::euclidean_vector{6, -1};
		CHECK_THROWS_AS(a -= b, comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(a -= b, "Dimensions of LHS(3) and RHS(2) do not match");
	}
}

SCENARIO("Compound Multiplication Operator Test") {
	auto a = comp6771::euclidean_vector{-6, 1};
	a *= 3;
	CHECK(a.dimensions() == 2);
	CHECK(a.at(0) == -18);
	CHECK(a.at(1) == 3);
}

SCENARIO("Compound Sivision Operator Test") {
	auto a = comp6771::euclidean_vector{-6, 3};
	a /= 3;
	CHECK(a.dimensions() == 2);
	CHECK(a.at(0) == -2);
	CHECK(a.at(1) == 1);
}

SCENARIO("Vector Type Conversion Test") {
	auto v1 = std::vector<double>{0.0, 1.0, 2.0};
	auto a = comp6771::euclidean_vector(v1.begin(), v1.end());
	auto v2 = static_cast<std::vector<double>>(a);
	CHECK(v1 == v2);
}

SCENARIO("Vector Type Const Conversion Test") {
	auto const v1 = std::vector<double>{0.0, 1.0, 2.0};
	auto const a = comp6771::euclidean_vector(v1.begin(), v1.end());
	auto const v2 = static_cast<std::vector<double>>(a);
	CHECK(v1 == v2);
}

SCENARIO("List Type Conversion Test") {
	auto l1 = std::initializer_list<double>{0.0, 1.0, 2.0};
	auto a = comp6771::euclidean_vector(l1);
	auto l2 = static_cast<std::list<double>>(a);
	CHECK(static_cast<std::list<double>>(l1) == l2);
}

SCENARIO("List Type Const Conversion") {
	auto l1 = std::initializer_list<double>{0.0, 1.0, 2.0};
	auto const a = comp6771::euclidean_vector{0.0, 1.0, 2.0};
	auto const l2 = static_cast<std::list<double>>(a);
	CHECK(static_cast<std::list<double>>(l1) == l2);
}
