// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <span>

namespace comp6771 {
	// ========================== CONSTRUCTORS ==========================
	// DEFAULT CONSTRUCTOR
	euclidean_vector::euclidean_vector()
	: magnitudes_{prep_mag(1)}
	, dimensions_{itos(1)} {
		this->magnitudes_[0] = 0.0;
	};

	// SINGLE-ARGUMENT CONSTRUCTOR
	euclidean_vector::euclidean_vector(int dim)
	: magnitudes_{prep_mag(dim)}
	, dimensions_{itos(dim)} {
		std::fill(this->begin(), this->end(), 0.0);
	}

	// CONSTRUCTOR
	euclidean_vector::euclidean_vector(int dim, double mag)
	: magnitudes_{prep_mag(dim)}
	, dimensions_{itos(dim)} {
		auto s = std::span(this->magnitudes_.get(), this->dimensions_);
		std::fill(s.begin(), s.end(), mag);
	}

	// VECTOR CONSTRUCTOR
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator b,
	                                   std::vector<double>::const_iterator e)
	: magnitudes_{prep_mag(static_cast<int>(std::distance(b, e)))}
	, dimensions_{static_cast<std::size_t>(std::distance(b, e))} {
		auto s = std::span(this->magnitudes_.get(), this->dimensions_);
		std::copy(b, e, this->magnitudes_.get());
	}

	// INITIALISER LIST CONSTRUCTOR
	euclidean_vector::euclidean_vector(std::initializer_list<double> list)
	: magnitudes_{prep_mag(static_cast<int>(list.size()))}
	, dimensions_{list.size()} {
		std::copy(list.begin(), list.end(), this->magnitudes_.get());
	}

	// COPY CONSTRUCTOR
	euclidean_vector::euclidean_vector(euclidean_vector const& to_copy)
	: norm{to_copy.norm}
	, magnitudes_{prep_mag(to_copy.dimensions())}
	, dimensions_{to_copy.dimensions_} {
		std::copy_n(to_copy.magnitudes_.get(), to_copy.dimensions(), this->magnitudes_.get());
	}

	// MOVE CONSTRUCTOR
	euclidean_vector::euclidean_vector(euclidean_vector&& to_move) noexcept
	: norm{to_move.norm}
	, magnitudes_{std::move(to_move.magnitudes_)}
	, dimensions_{to_move.dimensions_} {
		to_move.magnitudes_ = nullptr;
		to_move.dimensions_ = 0;
		to_move.norm = -1;
	}

	// =========================== OPERATORS ===========================
	// COPY OPERATOR
	auto euclidean_vector::operator=(euclidean_vector const& to_copy) -> euclidean_vector& {
		if (this != &to_copy) {
			this->dimensions_ = to_copy.dimensions_;
			this->magnitudes_ = prep_mag(to_copy.dimensions());
			this->norm = to_copy.norm;
			std::copy_n(to_copy.begin(), to_copy.dimensions(), this->begin());
		}
		return *this;
	}

	// MOVE OPERATOR
	auto euclidean_vector::operator=(euclidean_vector&& to_move) noexcept -> euclidean_vector& {
		this->magnitudes_ = std::move(to_move.magnitudes_);
		this->dimensions_ = to_move.dimensions_;
		this->norm = to_move.norm;
		to_move.magnitudes_ = nullptr;
		to_move.dimensions_ = 0;
		to_move.norm = -1;
		return *this;
	}

	// SUBSCRIPT OPERATOR
	auto euclidean_vector::operator[](int i) -> double {
		assert(i >= 0 and i < this->dimensions());
		return this->magnitudes_[itos(i)];
	}

	// CONST SUBSCRIPT OPERATOR
	auto euclidean_vector::operator[](int i) const -> double {
		assert(i >= 0 and i < this->dimensions());
		return this->magnitudes_[itos(i)];
	}

	// UNARY PLUS OPERATOR
	auto euclidean_vector::operator+() -> euclidean_vector {
		return euclidean_vector(*this);
	}

	// NEGATION OPERATOR
	auto euclidean_vector::operator-() -> euclidean_vector {
		auto ev = euclidean_vector(*this);
		std::for_each (ev.begin(), ev.end(), [&](double& d) { d *= -1; });
		return ev;
	}

	// COMPOUND ADDITION OPERATOR
	auto euclidean_vector::operator+=(euclidean_vector const& ev) -> euclidean_vector& {
		if (this->dimensions_ != ev.dimensions_) {
			auto err_msg = "Dimensions of LHS(" + std::to_string(this->dimensions()) + ") and RHS("
			               + std::to_string(ev.dimensions()) + ") do not match";
			throw euclidean_vector_error(err_msg);
		}
		std::transform(this->begin(), this->end(), ev.begin(), this->begin(), [&](double& a, double& b) {
			return a + b;
		});
		return *this;
	}

	// COMPOUND SUBTRACTION OPERATOR
	auto euclidean_vector::operator-=(euclidean_vector const& ev) -> euclidean_vector& {
		auto new_ev = ev;
		*this += -new_ev;
		return *this;
	}

	// COMPOUND MULTIPLICATION OPERATOR
	auto euclidean_vector::operator*=(double d) -> euclidean_vector& {
		std::for_each (this->begin(), this->end(), [&](double& a) { a *= d; });
		return *this;
	}

	// COMPOUND DIVISION OPERATOR
	auto euclidean_vector::operator/=(double d) -> euclidean_vector& {
		if (d == 0) {
			const auto* err_msg = "Invalid vector division by 0";
			throw euclidean_vector_error(err_msg);
		}
		std::for_each (this->begin(), this->end(), [&](double& a) { a /= d; });
		return *this;
	}

	// VECTOR TYPE CONVERSION OPERATOR
	euclidean_vector::operator std::vector<double>() {
		auto v = std::vector<double>();
		v.reserve(this->dimensions_);
		auto s = std::span(this->magnitudes_.get(), this->dimensions_);
		std::for_each (s.begin(), s.end(), [&](double& a) { v.push_back(a); });
		return v;
	}

	// VECTOR TYPE CONVERSION CONST OPERATOR
	euclidean_vector::operator std::vector<double>() const {
		auto v = std::vector<double>();
		v.reserve(this->dimensions_);
		auto s = std::span(this->magnitudes_.get(), this->dimensions_);
		std::for_each (s.begin(), s.end(), [&](double& a) { v.push_back(a); });
		return v;
	}

	// LIST TYPE CONVERSION OPERATOR
	euclidean_vector::operator std::list<double>() {
		auto l = std::list<double>();
		auto s = std::span(this->magnitudes_.get(), this->dimensions_);
		std::for_each (s.begin(), s.end(), [&](double& a) { l.push_back(a); });
		return l;
	}

	// LIST TYPE CONVERSION CONST OPERATOR
	euclidean_vector::operator std::list<double>() const {
		auto l = std::list<double>();
		auto s = std::span(this->magnitudes_.get(), this->dimensions_);
		std::for_each (s.begin(), s.end(), [&](double& a) { l.push_back(a); });
		return l;
	}

	// =========================== UTILITY ===========================
	auto euclidean_norm(euclidean_vector const& v) -> double {
		if (v.dimensions() == 0) {
			return 0.0;
		}
		if (v.norm == -1) {
			auto total = 0.0;
			std::for_each (v.begin(), v.end(), [&](double& a) { total += a * a; });
			v.norm = std::sqrt(total);
		}
		return v.norm;
	}

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			const auto* err_msg = "euclidean_vector with no dimensions does not have a unit vector";
			throw euclidean_vector_error(err_msg);
		}
		if (euclidean_norm(v) == 0.0) {
			const auto* err_msg = "euclidean_vector with zero euclidean normal does not have a unit "
			                      "vector";
			throw euclidean_vector_error(err_msg);
		}
		auto ev = euclidean_vector(v);
		auto norm = euclidean_norm(ev);
		std::for_each (ev.begin(), ev.end(), [&](double& a) { a /= norm; });
		return ev;
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (x.dimensions() != y.dimensions()) {
			auto err_msg = "Dimensions of LHS(" + std::to_string(x.dimensions()) + ") and RHS("
			               + std::to_string(y.dimensions()) + ") do not match";
			throw euclidean_vector_error(err_msg);
		}
		auto ev = euclidean_vector(x.dimensions());
		std::transform(x.begin(), x.end(), y.begin(), ev.begin(), [&](double& a, double& b) {
			return a * b;
		});
		return std::accumulate(ev.begin(), ev.end(), 0.0);
	}
} // namespace comp6771
