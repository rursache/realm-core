/*************************************************************************
 *
 * Copyright 2019 Realm Inc.
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

#include <realm.hpp>
#include <realm/array_object_id.hpp>

#include "test.hpp"


using namespace realm;

TEST(ObjectId_Basics)
{
    std::string init_str("000123450000ffbeef91906c");
    ObjectId id0(init_str.c_str());
    CHECK_EQUAL(id0.to_string(), init_str);
    Timestamp t0(0x12345, 0);
    ObjectId id1(t0, 0xff0000, 0xefbe);
    CHECK_EQUAL(id1.to_string().substr(0, 18), init_str.substr(0, 18));
    CHECK_EQUAL(id1.get_timestamp(), t0);
    ObjectId id2(Timestamp(0x54321, 0));
    CHECK_GREATER(id2, id1);
    CHECK_LESS(id1, id2);

    ObjectId id_null;
    CHECK(id_null.is_null());
}

TEST(ObjectId_Array)
{
    const char str0[] = "0000012300000000009218a4";
    const char str1[] = "000004560000000000170232";
    const char str2[] = "0000078900000000002999f3";

    ArrayObjectId arr(Allocator::get_default());
    arr.create();

    arr.add({str0});
    arr.add({str1});
    arr.insert(1, {str2});

    ObjectId id2(str2);
    CHECK_EQUAL(arr.get(0), ObjectId(str0));
    CHECK_EQUAL(arr.get(1), id2);
    CHECK_EQUAL(arr.get(2), ObjectId(str1));
    CHECK_EQUAL(arr.find_first(id2), 1);

    arr.erase(1);
    CHECK_EQUAL(arr.get(1), ObjectId(str1));

    ArrayObjectId arr1(Allocator::get_default());
    arr1.create();
    arr.move(arr1, 1);

    CHECK_EQUAL(arr.size(), 1);
    CHECK_EQUAL(arr1.size(), 1);
    CHECK_EQUAL(arr1.get(0), ObjectId(str1));

    arr.destroy();
    arr1.destroy();
}
