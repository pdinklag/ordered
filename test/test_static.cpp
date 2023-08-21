#include <cstdint>
#include <limits>

#include <btree/internal/linear_search_map.hpp>
#include <btree/internal/linear_search_set.hpp>

constexpr size_t small = 255;
constexpr size_t large = 256;

using Key = uint32_t;
using Value = uint32_t;

constexpr size_t key_size = sizeof(Key);
constexpr size_t value_size = sizeof(Value);

static_assert(sizeof(btree::internal::LinearSearchSet<Key, small>) == (small * key_size + sizeof(uint8_t)));
static_assert(sizeof(btree::internal::LinearSearchSet<Key, large>) == (large * key_size + sizeof(uint16_t)));

static_assert(sizeof(btree::internal::LinearSearchMap<Key, Value, small>) == (small * (key_size + value_size) + sizeof(uint8_t)));
static_assert(sizeof(btree::internal::LinearSearchMap<Key, Value, large>) == (large * (key_size + value_size) + sizeof(uint16_t)));

int main() {
    // we compiled, so we passed
    return 0;
}
