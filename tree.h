/*
 *		@brief: This header is an extension of the libstdc++ project, specifically adding a tree collection to c++2a.
 *
 *		This file is part of CXX (https://github.com/seanhobeck/cxx).
 *		Copyright (c) 2024 Sean Hobeck.
 *
 *		This program is free software: you can redistribute it and/or modify
 *		it under the terms of the GNU General Public License as published by
 *		the Free Software Foundation, either version 3 of the License, or
 *		(at your option) any later version.
 *
 *		This program is distributed in the hope that it will be useful, but
 *		WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *		General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *		@date    05 / 09 / 24
 *
 */

#ifndef CXX_TREE_H
#define CXX_TREE_H

#if __cplusplus >= 202002L

/// @uses: std::vector<?>
#include <vector>

/// @uses: std::stack<?>
#include <stack>

/// @uses: std::iterator<?>
#include <iterator>

/// @uses: std::convertible_to<?>
#include <concepts>

/// @uses: std::shared_ptr<?>
#include <memory>

namespace std
_GLIBCXX_VISIBILITY(default) {
	/// @note: concept to make sure that the type provided is comparable in a struct.
	template<typename _ty>
	concept __is_cmp = requires(_ty const &a, _ty const &b)
	{
		/// eq & ne checks only.
		{ a == b } -> std::convertible_to<bool>;
		{ a != b } -> std::convertible_to<bool>;
	};

	/// @note: class for tree-like containers.
	template<typename _ty> requires __is_cmp<_ty>
	class tree {
	public:
		/// @note: internal class for nodes.
		class node {
			/// @field: data held in each node.
			_ty _data;

			/// @field: shared ptr to the parent node.
			std::shared_ptr<node> _parent;

			/// @field: vector container of all children nodes.
			std::vector<node> _children {};

		public:
			/// @note: constructor for a tree node.
			_GLIBCXX20_CONSTEXPR node() : _parent(nullptr) {};
			_GLIBCXX20_CONSTEXPR explicit node(_ty const &data) _GLIBCXX_NOEXCEPT
				: _data(data), _parent(nullptr) {
			}
			_GLIBCXX20_CONSTEXPR node(_ty const &data, std::shared_ptr<node> parent) _GLIBCXX_NOEXCEPT
				: _data(data), _parent(parent) {
			}

			/// @fn: getter for the nodes children.
			_GLIBCXX_NODISCARD
			std::vector<node> children() _GLIBCXX_CONST { return this->_children; }

			/// @fn: getter for the nodes parent.
			_GLIBCXX_NODISCARD
			std::shared_ptr<node> parent() _GLIBCXX_CONST { return this->_parent; }

			/// @fn: getter for the nodes data.
			_GLIBCXX_NODISCARD
			_ty data() _GLIBCXX_CONST { return this->_data; }

			/// @fn: adds a child to this node.
			void append(node const &child) {
				child.parent = std::make_shared<node>(this);
				this->_children.push_back(child);
			}

			/// @fn: removes a child from this node.
			void remove(node const &child) {
				this->_children.remove(child);
			}

			/// @fn: getting the index of a child node from this tree's root node.
			unsigned int index(node const& child) {
				return std::find(this->_children.begin(), this->_children.end(), child);
			};

			/// @fn: checks if this node is a leaf (has no children).
			_GLIBCXX_NODISCARD
			bool is_leaf() _GLIBCXX_CONST {
				return this->_children.empty();
			}

			/// @fn: checks if this node is the root of the tree (has no parent).
			_GLIBCXX_NODISCARD
			bool is_root() _GLIBCXX_CONST {
				return this->_parent == nullptr;
			}

			/// @fn: overload operator to compare if it is eq.
			_GLIBCXX_NODISCARD
			bool operator==(node const &o) _GLIBCXX_CONST {
				return this->_data == o._data;
			}

			/// @fn: overload operator to compare if it is ne.
			_GLIBCXX_NODISCARD
			bool operator!=(node const &o) _GLIBCXX_CONST {
				return this->_data != o._data;
			}

			/// @fn: overload operator for indexing the children nodes.
			_GLIBCXX_NODISCARD
			std::shared_ptr<node> operator[](unsigned int idx) _GLIBCXX_CONST {
				if (idx >= this->_children.size())
					throw std::out_of_range("index out of range");

				return std::make_shared<node>(this->_children[idx]);
			};
		};

	protected:
		/// @note: internal iterator for pre-order iterating a tree.
		class _iterator {
			/// @usings: for readability...
			using _iter_cat = std::forward_iterator_tag;
			using _val_ty = _ty;
			using _diff_ty = std::ptrdiff_t;
			using _ptr = _ty *;
			using _ref = _ty &;

			/// @field: stack of the current iterated nodes.
			std::stack<node> _nodes;

		public:
			/// @note: construction for the tree containers iterator.
			_GLIBCXX20_CONSTEXPR explicit _iterator(node node) _GLIBCXX_NOEXCEPT {
				if (node) this->_nodes.push(node);
			}

			/// @fn: returns the stack of nodes used in the current iter.
			_GLIBCXX_NODISCARD
			std::stack<node> nodes() { return this->_nodes; };

			/// @fn: overload operators for comparison on looping (eq)
			_GLIBCXX_NODISCARD
			bool operator==(_iterator const &other) _GLIBCXX_CONST {
				return this->_nodes == other.nodes;
			}

			/// @fn: overload operators for comparison on looping (ne)
			_GLIBCXX_NODISCARD
			bool operator!=(_iterator const &other) _GLIBCXX_CONST {
				return !(*this == other);
			}

			/// @fn: overload operator for references on de-reference of the iter.
			_GLIBCXX_NODISCARD
			_ref operator*() _GLIBCXX_CONST { return this->_nodes.top()->data; }

			/// @fn: overload operator for pointer grabbing the iter.
			_GLIBCXX_NODISCARD
			_ptr operator->() _GLIBCXX_CONST { return &this->_nodes.top()->data; }

			/// @fn: overload operator for iteration.
			_iterator &operator++() {
				auto current = this->_nodes.top();
				this->_nodes.pop();
				for (auto it = current->children().rbegin(); it != current->children().rend(); ++it)
					this->_nodes.push(*it);
				return *this;
			}

			/// @fn: overload operator for iteration.
			_iterator operator++(int) {
				_iterator tmp = *this;
				++(*this);
				return tmp;
			}
		};

	private:
		/// @field: root node shared ptr.
		node _root;

	public:
		/// @note: constructor for a tree container.
		_GLIBCXX20_CONSTEXPR explicit tree(_ty data) {
			this->_root = node(data);
		};
		_GLIBCXX20_CONSTEXPR explicit tree(node root) : _root(root) {
		};

		/// @fn: getting the beginning iterator.
		_GLIBCXX_NODISCARD
		_iterator begin() { return _iterator(_root); }

		/// @fn: getting the ending iterator.
		_GLIBCXX_NODISCARD
		_iterator end() { return _iterator(nullptr); }

		/// @fn: function made to search the entire tree for a _ty (data template that matches the provided)
		_GLIBCXX_NODISCARD
		std::shared_ptr<node> search(_ty const &data) _GLIBCXX_CONST {
			std::stack<node> node_stack;
			node_stack.push(this->_root);
			while (!node_stack.empty()) {
				node current_node = node_stack.top();
				node_stack.pop();

				if (current_node.data() == data)
					return std::make_shared<node>(current_node);
				for (const node child: current_node.children())
					node_stack.push(child);
			}
			return nullptr;
		}

		/// @fn: adds a child to this tree's root node.
		void append(node const &child) {
			child.parent = std::make_shared<node>(this->_root);
			this->_root.push_back(child);
		}

		/// @fn: removes a child from this tree's root node.
		void remove(node const &child) {
			this->_root.remove(child);
		}

		/// @fn: getting the index of a child node from this tree's root node.
		unsigned int index(node const& child) {
			return std::find(this->_root.children.begin(), this->_root.children.end(), child);
		};

		/// @fn: overload operator for indexing the root node.
		_GLIBCXX_NODISCARD
		std::shared_ptr<node> operator[](unsigned int idx) _GLIBCXX_CONST {
			return std::make_shared<node>(this->_root[idx]);
		};
	};
};
#endif
#endif
