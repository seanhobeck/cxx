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
	concept __is_cmp = requires(_ty a, _ty b)
	{
		/// eq & ne checks only.
		{ a == b } -> std::convertible_to<bool>;
		{ a != b } -> std::convertible_to<bool>;
	};

	/// @note: class for tree-like containers.
	template<typename _ty> requires __is_cmp<_ty>
	class tree {
	protected:
		/// @note: internal class for nodes.
		class _node {
			/// @field: data held in each node.
			_ty _data;

			/// @field: shared ptr to the parent node.
			_node _parent;

			/// @field: vector container of all children nodes.
			std::vector<_node> _children;

		public:
			/// @note: constructor for a tree node.
			_GLIBCXX20_CONSTEXPR _node(const _ty &data) _GLIBCXX_NOEXCEPT
				: _data(data), _parent(nullptr) {
			}

			/// @fn: getter for the nodes children.
			_GLIBCXX_NODISCARD
			std::vector<_node> children() _GLIBCXX_CONST { return this->children; }

			/// @fn: getter for the nodes parent.
			_GLIBCXX_NODISCARD
			_node parent() _GLIBCXX_CONST { return this->parent; }

			/// @fn: getter for the nodes data.
			_GLIBCXX_NODISCARD
			_ty data() _GLIBCXX_CONST { return this->data; }

			/// @brief Adds a child to this node.
			/// @param child The child node to be added.
			void append(const _node &child) {
				child->parent = this;
				this->_children.push_back(child);
			}

			/// @fn: removes a child from this node.
			/// @param: child The child node to be removed.
			void remove(const _node &child) {
				this->_children.remove(child);
			}

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
			bool operator==(_node a, _node b) _GLIBCXX_CONST {
				return a._data == b._data;
			}

			/// @fn: overload operator to compare if it is ne.
			_GLIBCXX_NODISCARD
			bool operator!=(_node a, _node b) _GLIBCXX_CONST {
				return !(a._data == b._data);
			}
		};

		/// @note: internal iterator for pre-order iterating a tree.
		class _iterator {
			/// @usings: for readability...
			using _iter_cat = std::forward_iterator_tag;
			using _val_ty = _ty;
			using _diff_ty = std::ptrdiff_t;
			using _ptr = _ty *;
			using _ref = _ty &;

			/// @field: stack of the current iterated nodes.
			std::stack<_node> _nodes;

		public:
			/// @note: construction for the tree containers iterator.
			_GLIBCXX20_CONSTEXPR _iterator(_node node) _GLIBCXX_NOEXCEPT {
				if (node) this->_nodes.push(node);
			}

			/// @fn: returns the stack of nodes used in the current iter.
			_GLIBCXX_NODISCARD
			std::stack<_node> nodes() { return this->_nodes; };

			/// @fn: overload operators for comparison on looping (eq)
			_GLIBCXX_NODISCARD
			bool operator==(const _iterator &other) _GLIBCXX_CONST {
				return this->_nodes == other.nodes;
			}

			/// @fn: overload operators for comparison on looping (ne)
			_GLIBCXX_NODISCARD
			bool operator!=(const _iterator &other) _GLIBCXX_CONST {
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
		_node _root;

	public:
		/// @note: constructor for a tree container.
		_GLIBCXX20_CONSTEXPR tree() {
			this->_root = _node();
		};
		_GLIBCXX20_CONSTEXPR tree(_node root) : _root(root) {
		};

		/// @fn: getting the beginning iterator.
		_GLIBCXX_NODISCARD
		_iterator begin() { return _iterator(_root); }

		/// @fn: getting the ending iterator.
		_GLIBCXX_NODISCARD
		_iterator end() { return _iterator(nullptr); }

		/// @fn: overload operator for indexing a node in the entire tree.
		_GLIBCXX_NODISCARD
		std::shared_ptr<_node> operator[](const _ty &data) _GLIBCXX_CONST {
			if (!this->_root)
				return nullptr;

			std::stack<_node> node_stack;
			node_stack.push(this->_root);
			while (!node_stack.empty()) {
				_node current_node = node_stack.top();
				node_stack.pop();

				if (current_node->_data == data)
					return std::make_shared<current_node>;
				for (const _node child: current_node->_children)
					node_stack.push(child);
			}
			return nullptr;
		}
	};
};
#endif
