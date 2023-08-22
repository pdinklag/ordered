/**
 * test_examples.cpp
 * part of pdinklag/code
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <ordered/btree.hpp>

namespace ordered::test {

TEST_SUITE("ordered") {
    TEST_CASE("btree::Set") {
        // initialize an empty tree
        btree::Set<int> tree;
        CHECK(tree.empty());

        // insert some numbers
        tree.insert(5);
        tree.insert(1);
        tree.insert(8);
        tree.insert(4);
        tree.insert(12);
        tree.insert(-5);
        CHECK(tree.size() == 6);

        // erase a number
        bool const erased = tree.erase(8);
        CHECK(erased);
        CHECK(tree.size() == 5);

        // minimum and maximum
        CHECK(tree.min() == -5);
        CHECK(tree.max() == 12);

        // membership queries
        CHECK(tree.contains(-5));
        CHECK(tree.contains(1));
        CHECK(tree.contains(12));

        CHECK(!tree.contains(0));
        CHECK(!tree.contains(3));
        CHECK(!tree.contains(13));

        // alternative membership queries
        { auto const r = tree.find(-5); CHECK(r.exists); CHECK(r.key == -5); }
        { auto const r = tree.find(0); CHECK(!r.exists); }

        // predecessor queries
        { auto const r = tree.predecessor(-6); CHECK(!r.exists); }
        { auto const r = tree.predecessor(-5); CHECK(r.exists); CHECK(r.key == -5); }
        { auto const r = tree.predecessor(3);  CHECK(r.exists); CHECK(r.key == 1); }
        { auto const r = tree.predecessor(99); CHECK(r.exists); CHECK(r.key == 12); }

        // successor queries
        { auto const r = tree.successor(-5); CHECK(r.exists); CHECK(r.key == -5); }
        { auto const r = tree.successor(-6); CHECK(r.exists); CHECK(r.key == -5); }
        { auto const r = tree.successor(3);  CHECK(r.exists); CHECK(r.key == 4); }
        { auto const r = tree.successor(99); CHECK(!r.exists); }
    }

    TEST_CASE("map") {
        // initialize an empty associative tree
        btree::Map<int, int> tree;
        CHECK(tree.empty());

        // insert some numbers with associated values
        tree.insert(5, 500);
        tree.insert(1, 100);
        tree.insert(8, 800);
        tree.insert(4, 400);
        tree.insert(12, 1200);
        tree.insert(-5, -500);
        CHECK(tree.size() == 6);

        // erase a number
        bool const erased = tree.erase(8);
        CHECK(erased);
        CHECK(tree.size() == 5);

        // minimum and maximum
        CHECK(tree.min() == -5);
        CHECK(tree.max() == 12);

        // membership queries
        CHECK(tree.contains(-5));
        CHECK(tree.contains(1));
        CHECK(tree.contains(12));

        CHECK(!tree.contains(0));
        CHECK(!tree.contains(3));
        CHECK(!tree.contains(13));

        // alternative membership queries / lookup
        { auto const r = tree.find(-5); CHECK(r.exists); CHECK(r.key == -5); CHECK(r.value == -500); }
        { auto const r = tree.find(0); CHECK(!r.exists); }

        // predecessor queries
        { auto const r = tree.predecessor(-6); CHECK(!r.exists); }
        { auto const r = tree.predecessor(-5); CHECK(r.exists); CHECK(r.key == -5); CHECK(r.value == -500); }
        { auto const r = tree.predecessor(3);  CHECK(r.exists); CHECK(r.key == 1);  CHECK(r.value == 100); }
        { auto const r = tree.predecessor(99); CHECK(r.exists); CHECK(r.key == 12); CHECK(r.value == 1200); }

        // successor queries
        { auto const r = tree.successor(-5); CHECK(r.exists); CHECK(r.key == -5); CHECK(r.value == -500); }
        { auto const r = tree.successor(-6); CHECK(r.exists); CHECK(r.key == -5); CHECK(r.value == -500); }
        { auto const r = tree.successor(3);  CHECK(r.exists); CHECK(r.key == 4);  CHECK(r.value == 400); }
        { auto const r = tree.successor(99); CHECK(!r.exists); }
    }
}

}
