#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <__functional_base>
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>

// TODO(unknown): Make this graph generic
//       ... this won't just compile
//       straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
		class iterator;

	public:
		struct value_type {
			N from;
			N to;
			E weight;
		};

		// ====================== CONSTRUCTORS ======================
		graph();
		graph(std::initializer_list<N> il);
		template<typename InputIt>
		graph(InputIt first, InputIt last);
		graph(graph&& other) noexcept;
		auto operator=(graph&& other) noexcept -> graph&;
		graph(graph const& other);
		auto operator=(graph const& other) -> graph&;

		// ====================== MODIFIERS ======================
		auto insert_node(N const& value) -> bool;
		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool;
		auto replace_node(N const& old_data, N const& new_data) -> bool;
		auto merge_replace_node(N const& old_data, N const& new_data) -> void;
		auto erase_node(N const& value) -> bool;
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool;
		auto erase_edge(iterator i) -> iterator;
		auto erase_edge(iterator i, iterator s) -> iterator;
		auto clear() noexcept -> void;

		// ====================== ACCESSORS ======================
		[[nodiscard]] auto is_node(N const& value) const -> bool;
		[[nodiscard]] auto empty() const -> bool;
		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool;
		[[nodiscard]] auto nodes() const -> std::vector<N>;
		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E>;
		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator;
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N>;

		// ====================== ITERATOR ACCESS ======================
		[[nodiscard]] auto begin() const -> iterator;
		[[nodiscard]] auto end() const -> iterator;

		// ====================== COMPARISONS ======================
		[[nodiscard]] auto operator==(graph const& other) const -> bool;

		// ====================== EXTRACTOR ======================
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			for (auto node : g.nodes_) {
				os << *node.first << " (\n";
				for (const auto& [dst, weights] : node.second.out) {
					for (auto weight : weights) {
						auto edge_val = dst.lock();
						os << "  " << *edge_val << " | " << weight << "\n";
					}
				}
				os << ")\n";
			}
			return os;
		}

	private:
		struct node_compare;
		struct map_compare;

		struct node {
			std::set<std::weak_ptr<N>, node_compare> in;
			std::map<std::weak_ptr<N>, std::set<E>, node_compare> out;
		};

		std::map<std::shared_ptr<N>, node, map_compare> nodes_;

		struct node_compare {
			using is_transparent = void;
			auto operator()(const std::weak_ptr<N>& left, const std::weak_ptr<N>& right) const -> bool {
				auto left_val = left.lock();
				auto right_val = right.lock();
				return *left_val < *right_val;
			}
			auto operator()(const std::weak_ptr<N>& a, const N& b) const -> bool {
				auto tmp = a.lock();
				return *tmp < b;
			}

			auto operator()(const N& a, const std::weak_ptr<N>& b) const -> bool {
				auto tmp = b.lock();
				return a < *tmp;
			}
		};

		struct map_compare {
			using is_transparent = void;
			auto operator()(const std::shared_ptr<N>& left, const std::shared_ptr<N>& right) const
			   -> bool {
				return *left < *right;
			}
			auto operator()(const std::shared_ptr<N>& a, const N& b) const -> bool {
				return *a < b;
			}

			auto operator()(const N& a, const std::shared_ptr<N>& b) const -> bool {
				return a < *b;
			}
		};

		// ====================== HELPER FUNCTIONS ======================
		auto create_node() -> node;
	};

	template<typename N, typename E>
	class graph<N, E>::iterator {
	public:
		using value_type = graph<N, E>::value_type;
		using reference = value_type;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		using src_t = typename std::map<std::shared_ptr<N>, node, map_compare>::const_iterator;
		using dst_t = typename std::map<std::weak_ptr<N>, std::set<E>, node_compare>::const_iterator;
		using weight_t = typename std::set<E>::const_iterator;

		// Iterator constructors
		iterator();
		iterator(src_t curr, src_t first, src_t last);

		// Iterator source
		auto operator*() -> reference;

		// Iterator traversal
		auto operator++() -> iterator&;
		auto operator++(int) -> iterator;
		auto operator--() -> iterator&;
		auto operator--(int) -> iterator;

		// Iterator comparison
		auto operator==(iterator const& other) const -> bool;

	private:
		// explicit iterator(unspecified);
		friend class graph;
		src_t first_{};
		src_t last_{};
		src_t src_{};
		dst_t dst_{};
		weight_t weight_{};
	};

	////////////////////////////////////////// GRAPH CLASS //////////////////////////////////////////

	// ====================================== HELPER FUNCTIONS ======================================

	template<typename N, typename E>
	auto graph<N, E>::create_node() -> node {
		auto new_node = node{};
		new_node.in = std::set<std::weak_ptr<N>, node_compare>{};
		new_node.out = std::map<std::weak_ptr<N>, std::set<E>, node_compare>{};
		return new_node;
	}

	// ======================================= CONSTRUCTORS =======================================

	// No-parameter Constructor
	template<typename N, typename E>
	graph<N, E>::graph()
	: nodes_(){};

	// Initializer list Constructor
	template<typename N, typename E>
	graph<N, E>::graph(std::initializer_list<N> il)
	: nodes_() {
		std::for_each_n(il.begin(), il.size(), [this](auto& a) { insert_node(a); });
	};

	// Iterator Constructor
	template<typename N, typename E>
	template<typename InputIt>
	graph<N, E>::graph(InputIt first, InputIt last)
	: nodes_() {
		std::for_each(first, last, [this](auto& a) { insert_node(a); });
	};

	// Move Constructor
	template<typename N, typename E>
	graph<N, E>::graph(graph&& other) noexcept
	: nodes_() {
		this->nodes_ = std::move(other.nodes_);
		other.clear();
	}

	// Move Operator
	template<typename N, typename E>
	auto graph<N, E>::operator=(graph&& other) noexcept -> graph& {
		this->nodes_ = std::move(other.nodes_);
		other.clear();
		return *this;
	}

	// Copy Constructor
	template<typename N, typename E>
	graph<N, E>::graph(graph const& other)
	: nodes_() {
		auto other_nodes = other.nodes();
		std::for_each(other_nodes.begin(), other_nodes.end(), [this](auto& a) { insert_node(a); });
		for (auto n : other_nodes) {
			auto other_conns = other.connections(n);
			for (auto c : other_conns) {
				auto other_weights = other.weights(n, c);
				for (auto w : other_weights) {
					this->insert_edge(n, c, w);
				}
			}
		}
	}

	// Copy Operator
	template<typename N, typename E>
	auto graph<N, E>::operator=(graph const& other) -> graph& {
		if (this == other) {
			return *this;
		}
		auto other_nodes = other.nodes();
		std::for_each(other_nodes.begin(), other_nodes.end(), [this](auto& a) { insert_node(a); });
		for (auto n : other_nodes) {
			auto other_conns = other.connections(n);
			for (auto c : other_conns) {
				auto other_weights = other.weights(n, c);
				for (auto w : other_weights) {
					this->insert_edge(n, c, w);
				}
			}
		}
		return *this;
	}

	// ======================================== MODIFIERS ========================================

	// MODIFIER: insert_node
	template<typename N, typename E>
	auto graph<N, E>::insert_node(N const& value) -> bool {
		if (!is_node(value)) {
			auto key = std::make_shared<N>(value);
			auto val = create_node();
			this->nodes_.insert({key, val});
			return true;
		}
		return false;
	}

	// MODIFIER: insert_edge
	template<typename N, typename E>
	auto graph<N, E>::insert_edge(N const& src, N const& dst, E const& weight) -> bool {
		if (!is_node(src) or !is_node(dst)) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or "
			                         "dst node does not exist");
		}
		auto src_ptr = this->nodes_.find(src)->first;
		auto src_node = this->nodes_.find(src)->second;
		auto dst_ptr = this->nodes_.find(dst)->first;
		auto dst_node = this->nodes_.find(dst)->second;
		auto ret = false;

		if (is_connected(src, dst)) {
			auto res = src_node.out.find(dst)->second.insert(weight);
			ret = res.second;
		}
		else {
			src_node.out.insert({dst_ptr, std::set<E>{weight}});
			ret = true;
		}
		dst_node.in.insert(src_ptr);
		return ret;
	}

	// MODIFIER: replace_node
	template<typename N, typename E>
	auto graph<N, E>::replace_node(N const& old_data, N const& new_data) -> bool {
		if (!is_node(old_data)) {
			const auto* m = "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist";
			throw std::runtime_error(m);
		}
		if (is_node(new_data)) {
			return false;
		}
		auto element = this->nodes_.extract(this->nodes_.find(old_data));
		*element.key() = new_data;
		this->nodes_.insert(std::move(element));
		return true;
	};

	// MODIFIER: merge_replace_node
	template<typename N, typename E>
	auto graph<N, E>::merge_replace_node(N const& old_data, N const& new_data) -> void {
		if (!is_node(old_data) or !is_node(new_data)) {
			const auto* m = "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
			                "they don't exist in the graph";
			throw std::runtime_error(m);
		}
		auto old_elem = this->nodes_.find(old_data);
		auto new_elem = this->nodes_.find(new_data);
		for (auto inlink : old_elem->second.in) {
			auto inlink_val = inlink.lock();
			auto inlink_elem = this->nodes_.find(*inlink_val);
			auto conn_edges = inlink_elem->second.out.find(old_data);
			auto extracted = inlink_elem->second.out.extract(conn_edges);
			extracted.key() = new_elem->first;
			inlink_elem->second.out.insert(std::move(extracted));
		}
		for (const auto& [dst_ptr, weights] : old_elem->second.out) {
			auto dst_val = dst_ptr.lock();
			for (auto weight : weights) {
				insert_edge(new_data, *dst_val, weight);
			}
		}
		this->nodes_.erase(this->nodes_.find(old_data));
	};

	// MODIFIER: erase_node
	template<typename N, typename E>
	auto graph<N, E>::erase_node(N const& value) -> bool {
		if (is_node(value)) {
			auto val = this->nodes_.find(value);
			for (auto inlink : val->second.in) {
				auto inlink_val = inlink.lock();
				auto target = this->nodes_.find(*inlink_val)->second.out.find(value);
				this->nodes_.find(*inlink_val)->second.out.erase(target);
			}
			this->nodes_.erase(val);
			return true;
		}
		return false;
	};

	// MODIFIER: erase_edge
	template<typename N, typename E>
	auto graph<N, E>::erase_edge(N const& src, N const& dst, E const& weight) -> bool {
		if (!is_node(src) or !is_node(dst)) {
			const auto* m = "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
			                "exist in the graph";
			throw std::runtime_error(m);
		}
		if (!is_connected(src, dst)) {
			return false;
		}
		auto src_elem = this->nodes_.find(src);
		auto dst_elem = this->nodes_.find(dst);
		auto conns = src_elem->second.out.find(dst);
		conns->second.erase(conns->second.find(weight));
		if (conns->second.empty()) {
			src_elem->second.out.erase(src_elem->second.out.find(dst));
			dst_elem->second.in.erase(dst_elem->second.in.find(src));
		}
		return true;
	};

	// MODIFIER: erase_edge (1 iterator)
	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i) -> iterator {
		auto val = *i;
		++i;
		erase_edge(val.from, val.to, val.weight);
		return i;
	};

	// MODIFIER: erase_edge (2 iterator)
	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i, iterator s) -> iterator {
		if (s != this->end()) {
			++s;
		}
		auto iter = i;
		while (iter != s and iter != this->end()) {
			auto val = *iter;
			++iter;
			erase_edge(val.from, val.to, val.weight);
		}
		return s;
	};

	// MODIFIER: clear
	template<typename N, typename E>
	auto graph<N, E>::clear() noexcept -> void {
		this->nodes_.clear();
	};

	// ======================================== ACCESSORS ========================================

	// ACCESSOR: is_node
	template<typename N, typename E>
	auto graph<N, E>::is_node(N const& value) const -> bool {
		return (this->nodes_.find(value) != this->nodes_.end());
	}

	// ACCESSOR: empty
	template<typename N, typename E>
	auto graph<N, E>::empty() const -> bool {
		return (this->nodes_.size() == 0);
	}

	// ACCESSOR: is_connected
	template<typename N, typename E>
	auto graph<N, E>::is_connected(N const& src, N const& dst) const -> bool {
		if (!is_node(src) or !is_node(dst)) {
			const auto* m = "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
			                "exist in the graph";
			throw std::runtime_error(m);
		}
		auto src_node = this->nodes_.find(src)->second;
		// Checks to see if src_node->out has a map key to dst
		return src_node.out.find(dst) != src_node.out.end();
	}

	template<typename N, typename E>
	auto graph<N, E>::nodes() const -> std::vector<N> {
		auto keys = std::vector<N>{};
		for (auto node_elem : this->nodes_) {
			keys.push_back(*node_elem.first);
		}
		return keys;
	}

	// ACCESSOR: weights
	template<typename N, typename E>
	auto graph<N, E>::weights(N const& src, N const& dst) const -> std::vector<E> {
		if (is_node(src) and is_node(dst)) {
			auto ret = std::vector<E>{};
			auto src_node = this->nodes_.find(src)->second;
			// Find the dst connection in the src node and use its second element (set of weights)
			// Iterate through weights and push to vector
			if (!is_connected(src, dst)) {
				return ret;
			}
			auto weights = src_node.out.find(dst)->second;
			std::for_each(weights.begin(), weights.end(), [&](auto& w) { std::cout << w << " "; });
			for (auto weight : weights) {
				ret.push_back(weight);
			};
			return ret;
		}
		const auto* m = "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
		                "the graph";
		throw std::runtime_error(m);
	}

	// ACCESSOR: find
	template<typename N, typename E>
	auto graph<N, E>::find(N const& src, N const& dst, E const& weight) const -> iterator {
		auto w_vec = this->weights(src, dst);
		if (std::find(w_vec.begin(), w_vec.end(), weight) == w_vec.end()) {
			return this->end();
		}
		for (auto iter = this->begin(); iter != this->end(); ++iter) {
			auto val = *iter;
			if (val.from == src and val.to == dst and val.weight == weight) {
				return iter;
			}
		}
		return this->end();
	}

	// ACCESSOR: connections
	template<typename N, typename E>
	auto graph<N, E>::connections(N const& src) const -> std::vector<N> {
		if (!is_node(src)) {
			const auto* m = "Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the "
			                "graph";
			throw std::runtime_error(m);
		}
		auto src_node = this->nodes_.find(src)->second;
		auto conns = std::vector<N>{};
		// Iterate through the map of connections and get the first element (key) to append to vector
		for (auto conn : src_node.out) {
			auto conn_val = conn.first.lock();
			conns.push_back(*conn_val);
		}
		return conns;
	}

	// ===================================== ITERATOR ACCESS =====================================

	// ITERATOR: BEGIN
	template<typename N, typename E>
	auto graph<N, E>::begin() const -> iterator {
		for (auto iter = this->nodes_.begin(); iter != this->nodes_.end(); ++iter) {
			if (!iter->second.out.empty()) {
				return iterator{iter, this->nodes_.begin(), this->nodes_.end()};
			}
		}
		return iterator{this->nodes_.end(), this->nodes_.begin(), this->nodes_.end()};
	}

	// ITERATOR: END
	template<typename N, typename E>
	auto graph<N, E>::end() const -> iterator {
		return iterator{this->nodes_.end(), this->nodes_.begin(), this->nodes_.end()};
	}

	// ======================================== COMPARISONS ========================================

	// ACCESSOR: == operator
	template<typename N, typename E>
	auto graph<N, E>::operator==(graph const& other) const -> bool {
		if (&this->nodes_ == &other.nodes_) {
			return true;
		}
		auto this_nodes = this->nodes();
		auto other_nodes = other.nodes();
		// They must have the same nodes
		if (this_nodes != other_nodes) {
			return false;
		}
		for (auto n : other_nodes) {
			auto this_conns = this->connections(n);
			auto other_conns = other.connections(n);
			if (this_conns != other_conns) {
				return false;
			}
			for (auto c : other_conns) {
				auto this_weights = this->weights(n, c);
				auto other_weights = other.weights(n, c);
				if (this_weights != other_weights) {
					return false;
				}
			}
		}
		return true;
	}

	////////////////////////////////////// ITERATOR CLASS //////////////////////////////////////

	// ======================================== CONSTRUCTORS ========================================

	template<typename N, typename E>
	graph<N, E>::iterator::iterator()
	: first_()
	, last_()
	, src_()
	, dst_()
	, weight_(){};

	template<typename N, typename E>
	graph<N, E>::iterator::iterator(src_t curr, src_t first, src_t last)
	: first_(first)
	, last_(last)
	, src_(curr) {
		if (curr != last) {
			dst_ = curr->second.out.begin();
			weight_ = dst_->second.begin();
		}
		else {
			dst_ = first->second.out.end();
			weight_ = dst_->second.end();
		}
	};

	// =========================================== SOURCE ===========================================

	template<typename N, typename E>
	auto graph<N, E>::iterator::operator*() -> reference {
		const N& from = *src_->first;
		auto dst_val = dst_->first.lock();
		const N& to = *dst_val;
		const E& weight = *weight_;
		return {from, to, weight};
	}

	// ======================================== TRAVERSAL ========================================

	template<typename N, typename E>
	auto graph<N, E>::iterator::operator++() -> iterator& {
		if (src_ != last_) {
			++weight_;
			if (weight_ == dst_->second.end()) {
				++dst_;
				if (dst_ == src_->second.out.end()) {
					++src_;
					while (src_ != last_) {
						if (src_->second.out.begin() != src_->second.out.end()) {
							break;
						}
						++src_;
					}
					if (src_ == last_) {
						dst_ = first_->second.out.end();
						weight_ = dst_->second.end();
						return *this;
					}
					dst_ = src_->second.out.begin();
				}
				weight_ = dst_->second.begin();
			}
		}
		return *this;
	}

	template<typename N, typename E>
	auto graph<N, E>::iterator::operator++(int) -> iterator {
		auto tmp = *this;
		++*this;
		return tmp;
	}

	template<typename N, typename E>
	auto graph<N, E>::iterator::operator--() -> iterator& {
		if (src_ == last_ or weight_ == dst_->second.begin()) {
			if (src_ == last_ or dst_ == src_->second.out.begin()) {
				while (src_ != first_) {
					--src_;
					if (src_->second.out.begin() != src_->second.out.end()) {
						break;
					}
				}
				dst_ = src_->second.out.end();
			}
			--dst_;
			weight_ = dst_->second.end();
		}
		--weight_;
		return *this;
	}

	template<typename N, typename E>
	auto graph<N, E>::iterator::operator--(int) -> iterator {
		auto tmp = *this;
		--*this;
		return tmp;
	}

	template<typename N, typename E>
	auto graph<N, E>::iterator::operator==(iterator const& other) const -> bool {
		return (src_ == other.src_ and dst_ == other.dst_ and weight_ == other.weight_);
	}
} // namespace gdwg

#endif // GDWG_GRAPH_HPP
