//
// Bareflank Extended APIs
//
// Copyright (C) 2015 Assured Information Security, Inc.
// Author: Rian Quinn        <quinnr@ainfosec.com>
// Author: Brendan Kerrigan  <kerriganb@ainfosec.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include <catch/catch.hpp>

#include <bfbitmanip.h>
#include <vmcs/ept_entry_intel_x64.h>

using epte_type = ept_entry_intel_x64::integer_pointer;

TEST_CASE("ept_entry_intel_x64: read access")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_read_access(true);
    CHECK(epte->read_access());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 0));

    epte->set_read_access(false);
    CHECK(!epte->read_access());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: write access")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_write_access(true);
    CHECK(epte->write_access());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 1));

    epte->set_write_access(false);
    CHECK(!epte->write_access());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: execute access")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_execute_access(true);
    CHECK(epte->execute_access());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 2));

    epte->set_execute_access(false);
    CHECK(!epte->execute_access());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: memory type")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_read_access(true);
    epte->set_write_access(true);
    epte->set_memory_type(6UL);
    CHECK(epte->read_access());
    CHECK(epte->write_access());
    CHECK(epte->memory_type() == 6UL);

    epte->set_memory_type(0x1004);
    CHECK(epte->memory_type() == 4UL);

    epte->set_read_access(true);
    epte->set_write_access(true);
    epte->set_memory_type(0x0);
    CHECK(epte->read_access());
    CHECK(epte->write_access());
    CHECK(epte->memory_type() == 0x0);
}

TEST_CASE("ept_entry_intel_x64: ignore pat")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_ignore_pat(true);
    CHECK(epte->ignore_pat());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 6));

    epte->set_ignore_pat(false);
    CHECK(!epte->ignore_pat());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: entry type")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_entry_type(true);
    CHECK(epte->entry_type());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 7));

    epte->set_entry_type(false);
    CHECK(!epte->entry_type());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: accessed")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_accessed(true);
    CHECK(epte->accessed());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 8));

    epte->set_accessed(false);
    CHECK(!epte->accessed());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: dirty")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_dirty(true);
    CHECK(epte->dirty());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 9));

    epte->set_dirty(false);
    CHECK(!epte->dirty());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: execute access user")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_execute_access_user(true);
    CHECK(epte->execute_access_user());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 10));

    epte->set_execute_access_user(false);
    CHECK(!epte->execute_access_user());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: physical address")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_read_access(true);
    epte->set_write_access(true);
    epte->set_phys_addr(0x0000ABCDEF123000);
    CHECK(epte->read_access());
    CHECK(epte->write_access());
    CHECK(epte->phys_addr() == 0x0000ABCDEF123000);

    epte->set_phys_addr(0x0000ABCDEF123010);
    CHECK(epte->phys_addr() == 0x0000ABCDEF123000);

    epte->set_read_access(true);
    epte->set_write_access(true);
    epte->set_phys_addr(0x0);
    CHECK(epte->read_access());
    CHECK(epte->write_access());
    CHECK(epte->phys_addr() == 0x0);
}

TEST_CASE("ept_entry_intel_x64: suppress ve")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->set_suppress_ve(true);
    CHECK(epte->suppress_ve());
    CHECK(num_bits_set(entry) == 1);
    CHECK(is_bit_set(entry, 63));

    epte->set_suppress_ve(false);
    CHECK(!epte->suppress_ve());
    CHECK(num_bits_set(entry) == 0);
}

TEST_CASE("ept_entry_intel_x64: trap on access")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->trap_on_access();
    CHECK(!epte->read_access());
    CHECK(!epte->write_access());
    CHECK(!epte->execute_access());
}

TEST_CASE("ept_entry_intel_x64: pass through access")
{
    epte_type entry = 0;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->pass_through_access();
    CHECK(epte->read_access());
    CHECK(epte->write_access());
    CHECK(epte->execute_access());
}

TEST_CASE("ept_entry_intel_x64: clear")
{
    epte_type entry = 0xFFFFFFFFFFFFFFFF;
    auto epte = std::make_unique<ept_entry_intel_x64>(&entry);

    epte->clear();
    CHECK(entry == 0);
}
