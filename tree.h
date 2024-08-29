/*
 * This file is part of the XXX distribution (https://github.com/xxxx or http://xxx.github.io).
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 *      @author  Sean Hobeck
 *       @date 2024-08-29
 *
 *
 **/
#ifndef CXX_TREE_H
#define CXX_TREE_H

/// @uses: std::vector
#include <vector>

/// @uses: std::shared_ptr
#include <memory>

/// @uses: std::function
#include <functional>

/// @uses: std::stack
#include <stack>

/// @uses: std::iterator
#include <iterator>

namespace std {
    /// @class: class for tree-like containers.
    template<typename _ty>
    class tree {
    protected:
    	/// @class: internal class for nodes.
    	class _node {
    		/// @field: data held in each node.
    		_ty _data;

    		/// @field: shared ptr to the parent node.
    		std::shared_ptr<_node<_ty>> _parent;

    		/// @field: vector container of all children nodes.
    		std::vector<std::shared_ptr<_node<_ty>>> _children;

    	public:
    		/// @note: constructor for a tree node.
    		explicit _node(const _ty& data)
				: _data(data), _parent(nullptr) {}


    		/// @fn: getter for the nodes children.
    		std::vector<_node<_ty>> children() const { return this->children; }

    		/// @fn: getter for the nodes parent.
    		_node<_ty> parent() const { return this->parent; }

    		/// @fn: getter for the nodes data.
    		_ty data() const { return this->data; }

    		/// @brief Adds a child to this node.
    		/// @param child The child node to be added.
    		void append(const std::shared_ptr<_node<_ty>>& child) {
    			child->parent = std::shared_ptr<_node<_ty>>(this);
    			this->_children.push_back(child);
    		}

    		/// @fn: removes a child from this node.
    		/// @param: child The child node to be removed.
    		void remove(const std::shared_ptr<_node<_ty>>& child) {
    			this->_children.remove(child);
    		}

    		/// @fn: checks if this node is a leaf (has no children).
    		[[nodiscard]] bool is_leaf() const {
    			return this->_children.empty();
    		}

    		/// @fn: checks if this node is the root of the tree (has no parent).
    		[[nodiscard]] bool is_root() const {
    			return this->_parent == nullptr;
    		}
    	};

		/// @class: internal iterator for pre-order iterating a tree.
    	class _iterator {
    		/// @usings: for readability...
    		using _iter_cat = std::forward_iterator_tag;
    		using _val_ty = _ty;
    		using _diff_ty = std::ptrdiff_t;
    		using _ptr = _ty*;
    		using _ref = _ty&;

    		/// @field: stack of the current iterated nodes.
    		std::stack<std::shared_ptr<_node>> _nodes;

    	public:
			/// @note: construction for the tree containers iterator.
    		explicit _iterator(std::shared_ptr<_node> node) {
    			if (node) this->_nodes.push(node);
    		}

    		/// @fn: returns the stack of nodes used in the current iter.
			[[nodiscard]] std::stack<std::shared_ptr<_node>> nodes() { return this->_nodes; }

    		/// @fn: overload operators for comparison on looping (eq)
    		bool operator==(const _iterator& other) const {
    			return this->_nodes == other.nodes;
    		}
    		/// @fn: overload operators for comparison on looping (ne)
    		bool operator!=(const _iterator& other) const {
    			return !(*this == other);
    		}

    		/// @fn: overload operator for references on de-reference of the iter.
    		_ref operator*() const { return this->_nodes.top()->data; }
    		/// @fn: overload operator for pointer grabbing the iter.
    		_ptr operator->() const { return &(this->_nodes.top()->data); }

			/// @fn: overload operator for iteration.
    		_iterator& operator++() {
    			auto current = this->_nodes.top();
    			this->_nodes.pop();
    			for (auto it = current->children().rbegin(); it != current->children().rend(); ++it) {
    				this->_nodes.push(*it);
    			}
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
    	std::shared_ptr<_node> _root;

    public:
    	/// @note: constructor for a tree container.
    	explicit tree() : _root(nullptr) {};
    	explicit tree(std::shared_ptr<_node> root) : _root(root) {};

    	/// @fn: getting the beginning iterator.
		_iterator begin() { return _iterator(_root); }

    	/// @fn: getting the ending iterator.
    	_iterator end() { return _iterator(nullptr); }
    };
};
#endif
