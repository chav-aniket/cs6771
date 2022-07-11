#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <sstream>
#include <vector>

SCENARIO("Equal Friend Test") {
	GIVEN("Two vectors of different lengths") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		auto const b = comp6771::euclidean_vector{2.2, 1.1, 0.0};

		CHECK(!(a == b));
	}
	GIVEN("Two not equal vectors of the same length") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto const b = comp6771::euclidean_vector{2.2, 1.1, 0.0};

		CHECK(!(a == b));
	}
	GIVEN("Two equal vectors of the same length") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto const b = comp6771::euclidean_vector{0.0, 1.1, 2.2};

		CHECK(a == b);
	}
}

SCENARIO("Not Equal Friend Test") {
	GIVEN("Two vectors of different lengths") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2, 3.3};
		auto const b = comp6771::euclidean_vector{2.2, 1.1, 0.0};

		CHECK(a != b);
	}
	GIVEN("Two not equal vectors of the same length") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto const b = comp6771::euclidean_vector{2.2, 1.1, 0.0};

		CHECK(a != b);
	}
	GIVEN("Two equal vectors of the same length") {
		auto const a = comp6771::euclidean_vector{0.0, 1.1, 2.2};
		auto const b = comp6771::euclidean_vector{0.0, 1.1, 2.2};

		CHECK(!(a != b));
	}
}

SCENARIO("Addition Friend Test 1") {
	GIVEN("Two valid vectors") {
		auto const a = comp6771::euclidean_vector{1.782364, 2.987634, 3.982345};
		auto const b = comp6771::euclidean_vector{2.876238, 3.986235, 4.871264};
		auto const c = a + b;
		CHECK(c.at(0) == a.at(0) + b.at(0));
		CHECK(c.at(1) == a.at(1) + b.at(1));
		CHECK(c.at(2) == a.at(2) + b.at(2));
	}
}

SCENARIO("Addition Friend Test 2") {
	GIVEN("Two vectors of differing lengths") {
		auto const a = comp6771::euclidean_vector{0, 1, 2};
		auto const b = comp6771::euclidean_vector{0, 1, 2, 3};
		CHECK_THROWS_AS(a + b, comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(a + b, "Dimensions of LHS(3) and RHS(4) do not match");
	}
}

SCENARIO("Substraction Friend Test 1") {
	GIVEN("Two valid vectors") {
		auto const a = comp6771::euclidean_vector{1.782364, 2.987634, 3.982345};
		auto const b = comp6771::euclidean_vector{2.876238, 3.986235, 4.871264};
		auto const c = a - b;
		CHECK(c.at(0) == a.at(0) - b.at(0));
		CHECK(c.at(1) == a.at(1) - b.at(1));
		CHECK(c.at(2) == a.at(2) - b.at(2));
	}
}

SCENARIO("Substraction Friend Test 2") {
	GIVEN("Two vectors of differing lengths") {
		auto const a = comp6771::euclidean_vector{0, 1, 2};
		auto const b = comp6771::euclidean_vector{0, 1, 2, 3};
		CHECK_THROWS_AS(a - b, comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(a - b, "Dimensions of LHS(3) and RHS(4) do not match");
	}
}

SCENARIO("Multiplication Friend Test 1") {
	GIVEN("Valid input") {
		auto const a = comp6771::euclidean_vector{1.782364, 2.987634, 3.982345};
		auto const c = a * 7;
		CHECK(c.at(0) == a.at(0) * 7);
		CHECK(c.at(1) == a.at(1) * 7);
		CHECK(c.at(2) == a.at(2) * 7);
	}
}

SCENARIO("Multiplication Friend Test 2") {
	GIVEN("Valid input") {
		auto const a = comp6771::euclidean_vector{1.782364, 2.987634, 3.982345};
		auto const c = a * 7.7763292;
		CHECK(c.at(0) == a.at(0) * 7.7763292);
		CHECK(c.at(1) == a.at(1) * 7.7763292);
		CHECK(c.at(2) == a.at(2) * 7.7763292);
	}
}

SCENARIO("Multiplication Friend Test 3") {
	GIVEN("Valid inputs switched") {
		auto const a = comp6771::euclidean_vector{1.782364, 2.987634, 3.982345};
		auto const c = a * 7.7763292;
		CHECK(c.at(0) == 7.7763292 * a.at(0));
		CHECK(c.at(1) == 7.7763292 * a.at(1));
		CHECK(c.at(2) == 7.7763292 * a.at(2));
	}
}

SCENARIO("Division Friend Test 1") {
	GIVEN("Valid simple input") {
		auto const a = comp6771::euclidean_vector{1.782364, 2.987634, 3.982345};
		auto const b = a / 7;
		CHECK(b.at(0) == a.at(0) / 7);
		CHECK(b.at(1) == a.at(1) / 7);
		CHECK(b.at(2) == a.at(2) / 7);
	}
}

SCENARIO("Division Friend Test 2") {
	GIVEN("Valid complex input") {
		auto const a = comp6771::euclidean_vector{1.782364, 2.987634, 3.982345};
		auto const b = a / 7.7763292;
		CHECK(b.at(0) == a.at(0) / 7.7763292);
		CHECK(b.at(1) == a.at(1) / 7.7763292);
		CHECK(b.at(2) == a.at(2) / 7.7763292);
	}
}

SCENARIO("Division Friend Test 3") {
	GIVEN("Invalid input: dividing by zero") {
		auto const a = comp6771::euclidean_vector();
		CHECK_THROWS_AS(a / 0.0, comp6771::euclidean_vector_error);
		CHECK_THROWS_WITH(a / 0.0, "Invalid vector division by 0");
	}
}

SCENARIO("Output Stream Friend Test 1") {
	GIVEN("Valid simple input") {
		auto oss = std::ostringstream{};
		auto const a = comp6771::euclidean_vector{7, 11, 360};
		oss << a;
		CHECK(oss.str() == "[7 11 360]");
	}
}

SCENARIO("Output Stream Friend Test 2") {
	GIVEN("Valid complex input") {
		auto os1 = std::ostringstream{};
		auto os2 = std::ostringstream{};
		auto const a = comp6771::euclidean_vector{7.982702, 11.872349, 360.874828};
		os1 << a;
		// Because the output is meant to print accordingly to `oss << d`
		os2 << "[" << a.at(0) << " " << a.at(1) << " " << a.at(2) << "]";
		CHECK(os1.str() == os2.str());
	}
}
