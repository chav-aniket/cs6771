#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include "helper.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		// ========================== PUBLIC MEMBERS =========================
		mutable double norm = -1;

		// ========================== CONSTRUCTORS =========================
		// DEFAULT CONSTRUCTOR
		euclidean_vector();
		// SINGLE-ARGUMENT CONSTRUCTOR
		explicit euclidean_vector(int dim);
		// CONSTRUCTOR
		explicit euclidean_vector(int dim, double mag);
		// VECTOR CONSTRUCTOR
		explicit euclidean_vector(std::vector<double>::const_iterator b,
		                          std::vector<double>::const_iterator e);
		// INITIALISER LIST CONSTRUCTOR
		euclidean_vector(std::initializer_list<double> list);
		// COPY CONSTRUCTOR
		euclidean_vector(euclidean_vector const& to_copy);
		// MOVE CONSTRUCTOR
		euclidean_vector(euclidean_vector&& to_move) noexcept;
		// DESTRUCTOR
		~euclidean_vector() = default;

		// =========================== OPERATORS ===========================
		// COPY OPERATOR
		auto operator=(euclidean_vector const& to_copy) -> euclidean_vector&;
		// MOVE OPERATOR
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&;
		// SUBSCRIPT OPERATOR
		auto operator[](int i) -> double;
		// CONST SUBSCRIPT OPERATOR
		auto operator[](int i) const -> double;
		// UNARY PLUS OPERATOR
		auto operator+() -> euclidean_vector;
		// NEGATION OPERATOR
		auto operator-() -> euclidean_vector;
		// COMPOUND ADDITION OPERATOR
		auto operator+=(euclidean_vector const& ev) -> euclidean_vector&;
		// COMPOUND SUBTRACTION OPERATOR
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		// COMPOUND MULTIPLICATION OPERATOR
		auto operator*=(double) -> euclidean_vector&;
		// COMPOUND DIVISION OPERATOR
		auto operator/=(double) -> euclidean_vector&;
		// VECTOR TYPE CONVERSION OPERATOR
		explicit operator std::vector<double>();
		// VECTOR TYPE CONVERSION OPERATOR
		explicit operator std::vector<double>() const;
		// LIST TYPE CONVERSION OPERATOR
		explicit operator std::list<double>();
		// LIST TYPE CONVERSION OPERATOR
		explicit operator std::list<double>() const;

		// =========================== MEMBER FUNCTIONS ===========================
		// AT METHOD
		[[nodiscard]] auto at(int i) -> double {
			if (i < 0 or i >= this->dimensions()) {
				auto err_msg =
				   "Index " + std::to_string(i) + " is not valid for this euclidean_vector object";
				throw euclidean_vector_error(err_msg);
			}
			return magnitudes_[itos(i)];
		}

		// AT CONST METHOD
		[[nodiscard]] auto at(int i) const -> double& {
			if (i < 0 or i >= this->dimensions()) {
				auto err_msg =
				   "Index " + std::to_string(i) + " is not valid for this euclidean_vector object";
				throw euclidean_vector_error(err_msg);
			}
			return magnitudes_[itos(i)];
		}

		// DIMENSIONS METHOD
		[[nodiscard]] auto dimensions() const -> int {
			return static_cast<int>(dimensions_);
		}

		// BEGIN METHOD
		[[nodiscard]] auto begin() const -> std::__wrap_iter<double*> {
			auto s = std::span(this->magnitudes_.get(), this->dimensions_);
			return s.begin();
		}

		// END METHOD
		[[nodiscard]] auto end() const -> std::__wrap_iter<double*> {
			auto s = std::span(this->magnitudes_.get(), this->dimensions_);
			return s.end();
		}

		// =========================== FRIENDS ===========================
		// EQUAL FRIEND
		friend auto operator==(euclidean_vector const& ev1, euclidean_vector const& ev2) -> bool {
			return std::equal(ev1.begin(), ev1.end(), ev2.begin());
		}

		// NOT EQUAL FRIEND
		friend auto operator!=(euclidean_vector const& ev1, euclidean_vector const& ev2) -> bool {
			return !std::equal(ev1.begin(), ev1.end(), ev2.begin());
		}

		// ADDITION FRIEND
		friend auto operator+(euclidean_vector const& ev1, euclidean_vector const& ev2)
		   -> euclidean_vector {
			auto r = ev1;
			r += ev2;
			return r;
		}

		// SUBTRACTION FRIEND
		friend auto operator-(euclidean_vector const& ev1, euclidean_vector const& ev2)
		   -> euclidean_vector {
			auto r = ev1;
			return r -= ev2;
		}

		// MULTIPLICATION FRIENDS
		friend auto operator*(euclidean_vector const& ev, double d) -> euclidean_vector {
			auto r = euclidean_vector(ev.dimensions());
			auto sr = std::span(r.magnitudes_.get(), r.dimensions_);
			auto s1 = std::span(ev.magnitudes_.get(), ev.dimensions_);
			std::transform(s1.begin(), s1.end(), sr.begin(), [&](double& a) { return a * d; });
			return r;
		}

		friend auto operator*(double d, euclidean_vector const& ev) -> euclidean_vector {
			auto r = euclidean_vector(ev.dimensions());
			auto sr = std::span(r.magnitudes_.get(), r.dimensions_);
			auto s1 = std::span(ev.magnitudes_.get(), ev.dimensions_);
			std::transform(s1.begin(), s1.end(), sr.begin(), [&](double& a) { return a * d; });
			return r;
		}

		// DIVISION FRIEND
		friend auto operator/(euclidean_vector const& ev, double d) -> euclidean_vector {
			if (d == 0.0) {
				const auto* err_msg = "Invalid vector division by 0";
				throw euclidean_vector_error(err_msg);
			}
			auto r = euclidean_vector(ev.dimensions());
			auto sr = std::span(r.magnitudes_.get(), r.dimensions_);
			auto s1 = std::span(ev.magnitudes_.get(), ev.dimensions_);
			std::transform(s1.begin(), s1.end(), sr.begin(), [&](double& a) { return a / d; });
			return r;
		}

		// OUTPUT STREAM FRIEND
		friend auto operator<<(std::ostream& os, euclidean_vector const& ev) -> std::ostream& {
			auto s = std::span(ev.magnitudes_.get(), ev.dimensions_);
			os << '[' << ev.at(0);
			std::for_each (std::next(s.begin()), s.end(), [&](double& d) { os << ' ' << d; });
			return os << "]";
		}

	private:
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;
		std::size_t dimensions_;
	};

	// =========================== UTILITY ===========================
	// EUCLIDEAN NORMAL
	auto euclidean_norm(euclidean_vector const& v) -> double;

	// UNIT
	auto unit(euclidean_vector const& v) -> euclidean_vector;

	// DOT PRODUCT
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;
} // namespace comp6771

#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
