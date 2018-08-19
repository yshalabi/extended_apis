//
// Bareflank Extended APIs
// Copyright (C) 2018 Assured Information Security, Inc.
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

#include <mutex>

#include <bfvmm/vcpu/vcpu_factory.h>
#include <bfvmm/memory_manager/arch/x64/unique_map.h>

#include <eapis/hve/arch/intel_x64/vcpu.h>
using namespace eapis::intel_x64;

// -----------------------------------------------------------------------------
// vCPU
// -----------------------------------------------------------------------------

namespace test
{

std::once_flag flag;
ept::mmap g_guest_map;

void
test_hlt_delegate(bfobject *obj)
{
    bfignored(obj);
}

class vcpu : public eapis::intel_x64::vcpu
{
public:

    explicit vcpu(vcpuid::type id) :
        eapis::intel_x64::vcpu{id}
    {
        std::call_once(flag, [&] {
            ept::identity_map(
                g_guest_map,
                MAX_PHYS_ADDR,
                ept::mmap::attr_type::read_write
            );
        });

        this->add_hlt_delegate(
            hlt_delegate_t::create<test_hlt_delegate>()
        );

        eapis()->add_ept_execute_violation_handler(
            ept_violation_handler::handler_delegate_t::create<vcpu, &vcpu::test_execute_violation_handler>(this)
        );

        eapis()->set_eptp(g_guest_map);
    }

    bool
    test_execute_violation_handler(
        gsl::not_null<vmcs_t *> vmcs, ept_violation_handler::info_t &info)
    {
        bfignored(vmcs);
        bfignored(info);

        bfdebug_info(0, "disabling EPT");
        eapis()->disable_ept();

        return true;
    }

    vcpu(vcpu &&) = delete;
    vcpu &operator=(vcpu &&) = delete;
    vcpu(const vcpu &) = delete;
    vcpu &operator=(const vcpu &) = delete;
};

}

// -----------------------------------------------------------------------------
// vCPU Factory
// -----------------------------------------------------------------------------

namespace bfvmm
{

std::unique_ptr<vcpu>
vcpu_factory::make_vcpu(vcpuid::type vcpuid, bfobject *obj)
{
    bfignored(obj);
    return std::make_unique<test::vcpu>(vcpuid);
}

}
