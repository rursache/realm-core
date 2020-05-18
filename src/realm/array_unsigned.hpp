/*************************************************************************
 *
 * Copyright 2016 Realm Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 **************************************************************************/

#ifndef REALM_ARRAY_UNSIGNED_HPP
#define REALM_ARRAY_UNSIGNED_HPP

#include <realm/node.hpp>

namespace realm {

// Array holding unsigned values only
class ArrayUnsigned : public Node {

public:
    ArrayUnsigned(Allocator& allocator)
        : Node(allocator)
    {
    }

    // Will create an uninitialized array of size 'initial_size'
    // Has a width big enough to hold values smaller than 'ubound_value'
    void create(size_t initial_size, uint64_t ubound_value);

    void init_from_ref(ref_type ref) noexcept
    {
        REALM_ASSERT_DEBUG(ref);
        char* header = m_alloc.translate(ref);
        init_from_mem(MemRef(header, ref, m_alloc));
    }

    bool update_from_parent(size_t old_baseline) noexcept;

    size_t lower_bound(uint64_t value) const noexcept;
    size_t upper_bound(uint64_t value) const noexcept;

    void add(uint64_t value)
    {
        insert(m_size, value);
    }
    //  insert  value at  index (moving successive  elements  1 position  forwards)
    void insert(size_t ndx, uint64_t value);
    // delete value at index
    void erase(size_t ndx);
    //  return  value at  index
    uint64_t get(size_t index) const;
    //  return  tagged value at  index
    // override value at index
    void set(size_t ndx, uint64_t value);

    void adjust(size_t begin, size_t end, int64_t diff)
    {
        if (diff != 0) {
            // FIXME: Should be optimized
            for (size_t i = begin; i < end; ++i)
                adjust(i, diff); // Throws
        }
    }

    void truncate(size_t ndx);

    /// The meaning of 'width' depends on the context in which this
    /// array is used.
    size_t get_width() const noexcept
    {
        return m_width;
    }

private:
    uint_least8_t m_width = 0; // Size of an element (meaning depend on type of array).
    uint64_t m_ubound;         // max number that can be stored with current m_width

    void init_from_mem(MemRef mem) noexcept
    {
        Node::init_from_mem(mem);
        set_width(get_width_from_header(get_header()));
    }

    void adjust(size_t ndx, int64_t diff)
    {
        if (diff != 0) {
            set(ndx, get(ndx) + diff); // Throws
        }
    }

    void alloc(size_t init_size, size_t new_width)
    {
        Node::alloc(init_size, new_width);
        set_width(uint8_t(new_width));
    }

    void set_width(uint8_t width);
    uint8_t bit_width(uint64_t value);

    void _set(size_t ndx, uint8_t width, uint64_t value);
    uint64_t _get(size_t ndx, uint8_t width) const;
};

class ClusterKeyArray : public ArrayUnsigned {
public:
    using ArrayUnsigned::ArrayUnsigned;

    uint64_t get(size_t ndx) const
    {
        return (m_data != nullptr) ? ArrayUnsigned::get(ndx) : uint64_t(ndx);
    }
};


} // namespace realm

#endif /* REALM_ARRAY_UNSIGNED_HPP */
