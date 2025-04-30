/**
 * test_btree.cpp
 * part of pdinklag/code
 * 
 * MIT License
 * 
 * Copyright (c) Patrick Dinklage
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
#include <ordered/range_marking.hpp>

#include <random_permutation.hpp>

#include <set>

namespace ordered::test {

template<typename Set>
void test_set(Set& set, uint64_t const universe, uint64_t const seed, size_t const n) {
    // start with an empty set
    set.clear();
    CHECK(set.empty());

    // compute a permutation of n keys from the universe and insert it into the set
    // also keep it in a std::set for verification
    random_permutation::RandomPermutation perm_keys(universe, seed);

    using Key = typename Set::Key;
    std::set<Key> verification;
    {
        for(size_t i = 0; i < n; i++) {
            Key const key = perm_keys(i);
            verification.emplace(key);
            set.insert(key);
        }
    }

    // ensure that all keys in the permutation are actually contained
    CHECK(set.size() == n);
    for(size_t i = 0; i < n; i++) {
        Key const key = perm_keys(i);
        CHECK(set.contains(key));
    }

    // test successor
    random_permutation::RandomPermutation perm_query(universe, ~seed);

    {
        auto const max = *verification.rbegin();
        if(max < universe) {
            CHECK(!set.successor(max+1).exists);
        }
    }

    for(size_t i = 0; i < n; i++) {
        Key const key = perm_query(i);
        auto const r = set.successor(key);
        if(r.exists) {
            // sanity
            CHECK(r.key >= key);

            if(r.key == key) {
                // ensure that it also exists in the verification set
                CHECK(verification.contains(key));
            } else {
                // ensure that the successor in the verification set is the same
                auto const vsucc = *verification.lower_bound(key);
                CHECK(vsucc == r.key);
            }
        } else {
            // ensure that the key is greater than the maximum
            auto const max = *verification.rbegin();
            CHECK(key > max);
        }
    }
}

TEST_SUITE("ordered") {
    TEST_CASE("btree::Set") {
        ordered::btree::Set<uint64_t> set;
        test_set(set, 100'000'000ULL, 123456789, 1'000'000ULL);
    }

    TEST_CASE("range_marking::Set") {
        ordered::range_marking::Set<uint64_t> set(100'000'000ULL);
        test_set(set, 100'000'000ULL, 123456789, 1'000'000ULL);
    }
}

}
