# Ordered Sets

This repository provides a straightforward C++20 header-only implementations of selected ordered set data structures.

I co-authored the SEA 2021 paper [Engineering Predecessor Data Structures for Dynamic Integer Sets](https://arxiv.org/abs/2104.06740), where previous versions of these implementations were benchmarked against different data structures, all being competitive.

### Requirements

This library is written in C++20, a corresponding compiler is required that fully supports concepts. Tests have been done only with GCC 11. Apart from that, the library as no external dependencies. For running [unit tests](#unit-tests), CMake is required.

### License

```
MIT License

Copyright (c) 2023 Patrick Dinklage

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

### Unit Tests

Using CMake, you can build and run the tests using the following chain of commands in the repository root:

```sh
mkdir build; cd build
cmake ..
make
make test
```

The tests are extremely simple. The benchmark of the paper [Engineering Predecessor Data Structures for Dynamic Integer Sets](https://arxiv.org/abs/2104.06740), where the outputs of all experiments have been verified for correctness, were the actual tests.

## Usage

The library is header only, so all you need to do is make sure it's in your include path.

In case you use CMake, you can embed this repository into yours (e.g., as a git submodule) and add it like so:

```cmake
add_subdirectory(path/to/ordered)
```

You can then link against the `ordered` interface library, which will automatically add the include directory to your target.

### B-trees

This is an implementation of plain old B-trees with no tricks or magic. Let *B* be the degree of the nodes in the B-tree. The implementation supports insertion and removal operations, as well as minimum, maximum, membership, predecessor and successor queries in base-*B* logarithmic time in the number of contained keys plus node-level scans.

The implementation is very simple, almost naïve: the maintenance of the B-tree structure is implemented according to Gonazlo Navarro's book [Compact Data Structures](https://users.dcc.uchile.cl/~gnavarro/CDSbook/). The data structure for nodes is a simple array of keys which are maintained in ascending order using simple linear-time queries and manipulation. Since nodes are kept small (think a B-tree of degree 65, storing up to 64 keys at each node), thanks to caching, this is extremely fast on modern hardware and can easily outperform much more sophisticated approaches (such as fusion or burst) in the general case. Despite the simple algorithms, the code has been engineered such that it is competitive with other data structures across the board. Because B-trees are cardinality-reducing, their memory consumption depends only on the contained number of keys and not the size of the universe these keys are drawn from.

The B-tree comes as an ordered set as well as an ordered map (associative B-tree). It only supports unique keys; multiple insertions of the same key leads to undefined behaviour.

#### Ordered Set (classic B-tree)

The alias `btree::Set<Key>` gives you the classic B-tree experience for keys of type `Key`. The only requirement for `Key` is that it must satisfy the `std::totally_ordered` concept.

Note that even though no values are associated with keys here, you will get a [QueryResult](#QueryResult) when performing predecessor or successor queries. This is simply for convenience of keeping the code succinct; the `value` field in these results is not meaningful and can simply be ignored.

##### Example

The following excerpt from the tests should tell you all you need to know about the usage.

```cpp
#include <btree.hpp>

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
```

#### Ordered Map (associative B-tree)

The associative variant `btree::Map` works just like `btree::Set`, except you associate values to each key upon insertion and the [QueryResult](#QueryResult) returned by predecessor and successor queries also contain the corresponding value. This is particularly useful when using `find`, which can be used to lookup the value associated to a key.

##### Example

The following excerpt from the tests should tell you all you need to know about the usage.

```cpp
#include <btree.hpp>

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
```

### QueryResult

Queries such as *find*, *predecessor* and *successor* return an instance of the `btree:QueryResult<Key, Value>` struct.

First, it contains the flag `exists` telling whether a result was found for the query key at all. Casting query results to `bool` will also return the `exists` flag. Second, it contains the `key` field that will hold the corresponding key, e.g., the predecessor of the query key. Last, the `value` field will hold the value associated to `key`, if any. In case you are using `btree::Set`, the `value` will always be a meaningless dummy that you may ignore.

If `exists` is `false`, the contents of `key` and `value` are undefined.