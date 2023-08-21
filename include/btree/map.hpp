/**
 * btree/map.hpp
 * part of pdinklag/btree
 * 
 * MIT License
 * 
 * Copyright (c) 2023 Patrick Dinklage
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _BTREE_MAP_HPP
#define _BTREE_MAP_HPP

#include "btree/btree_impl.hpp"
#include "btree/internal/linear_search_map.hpp"

namespace btree {

/**
 * \brief An associative B-tree
 * 
 * The degree must be an odd number.
 * 
 * \tparam Key the key type
 * \tparam Value the value type
 * \tparam degree the B-tree node degree
 */
template<std::totally_ordered Key, typename Value, size_t degree = 65>
using Map = BTree<internal::LinearSearchMap<Key, Value, degree - 1>>;

}

#endif
