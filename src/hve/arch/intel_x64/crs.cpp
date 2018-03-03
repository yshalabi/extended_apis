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

#include <bfdebug.h>
#include <hve/arch/intel_x64/crs.h>

namespace eapis
{
namespace intel_x64
{

static bool
default_handler(
    gsl::not_null<vmcs_t *> vmcs, crs::info_t &info)
{ bfignored(vmcs); bfignored(info); return true; }

crs::crs(gsl::not_null<exit_handler_t *> exit_handler) :
    m_exit_handler{exit_handler}
{
    using namespace vmcs_n;

    m_exit_handler->add_handler(
        exit_reason::basic_exit_reason::control_register_accesses,
        handler_delegate_t::create<crs, &crs::handle_crs>(this)
    );

    this->add_wrcr0_handler(
        wrcr0_handler_delegate_t::create<default_handler>()
    );

    this->add_rdcr3_handler(
        rdcr3_handler_delegate_t::create<default_handler>()
    );

    this->add_wrcr3_handler(
        wrcr3_handler_delegate_t::create<default_handler>()
    );

    this->add_wrcr4_handler(
        wrcr4_handler_delegate_t::create<default_handler>()
    );

    this->add_rdcr8_handler(
        rdcr8_handler_delegate_t::create<default_handler>()
    );

    this->add_wrcr8_handler(
        wrcr8_handler_delegate_t::create<default_handler>()
    );
}

crs::~crs()
{
    if (!ndebug && this->is_logging_enabled()) {
        dump_log();
    }
}

// -----------------------------------------------------------------------------
// CR0
// -----------------------------------------------------------------------------

void
crs::add_wrcr0_handler(wrcr0_handler_delegate_t &&d)
{ m_wrcr0_handlers.push_front(std::move(d)); }

void
crs::add_rdcr3_handler(rdcr3_handler_delegate_t &&d)
{ m_rdcr3_handlers.push_front(std::move(d)); }

void
crs::add_wrcr3_handler(wrcr3_handler_delegate_t &&d)
{ m_wrcr3_handlers.push_front(std::move(d)); }

void
crs::add_wrcr4_handler(wrcr4_handler_delegate_t &&d)
{ m_wrcr4_handlers.push_front(std::move(d)); }

void
crs::add_rdcr8_handler(rdcr8_handler_delegate_t &&d)
{ m_rdcr8_handlers.push_front(std::move(d)); }

void
crs::add_wrcr8_handler(wrcr8_handler_delegate_t &&d)
{ m_wrcr8_handlers.push_front(std::move(d)); }

void
crs::enable_wrcr0_trapping(vmcs_n::value_type mask, vmcs_n::value_type shadow)
{
    using namespace vmcs_n;

    cr0_guest_host_mask::set(mask);
    cr0_read_shadow::set(shadow);
}

void
crs::enable_rdcr3_trapping()
{
    using namespace vmcs_n;
    primary_processor_based_vm_execution_controls::cr3_store_exiting::enable();
}

void
crs::enable_wrcr3_trapping()
{
    using namespace vmcs_n;
    primary_processor_based_vm_execution_controls::cr3_load_exiting::enable();
}

void
crs::enable_wrcr4_trapping(vmcs_n::value_type mask, vmcs_n::value_type shadow)
{
    using namespace vmcs_n;

    cr4_guest_host_mask::set(mask);
    cr4_read_shadow::set(shadow);
}

void
crs::enable_rdcr8_trapping()
{
    using namespace vmcs_n;
    primary_processor_based_vm_execution_controls::cr8_store_exiting::enable();
}

void
crs::enable_wrcr8_trapping()
{
    using namespace vmcs_n;
    primary_processor_based_vm_execution_controls::cr8_load_exiting::enable();
}

// -----------------------------------------------------------------------------
// Debug
// -----------------------------------------------------------------------------

void
crs::dump_log()
{
    if (!m_cr0_log.empty()) {
        bfdebug_transaction(0, [&](std::string * msg) {
            bfdebug_lnbr(0, msg);
            bfdebug_info(0, "cr0 log", msg);
            bfdebug_brk2(0, msg);

            for (const auto &record : m_cr0_log) {
                bfdebug_info(0, "record", msg);
                bfdebug_subnhex(0, "val", record.val, msg);
                bfdebug_subnhex(0, "shadow", record.shadow, msg);
                bfdebug_subbool(0, "out", record.out, msg);
                bfdebug_subbool(0, "dir", record.dir, msg);
            }

            bfdebug_lnbr(0, msg);
        });
    }

    if (!m_cr3_log.empty()) {
        bfdebug_transaction(0, [&](std::string * msg) {
            bfdebug_lnbr(0, msg);
            bfdebug_info(0, "cr3 log", msg);
            bfdebug_brk2(0, msg);

            for (const auto &record : m_cr3_log) {
                bfdebug_info(0, "record", msg);
                bfdebug_subnhex(0, "val", record.val, msg);
                bfdebug_subnhex(0, "shadow", record.shadow, msg);
                bfdebug_subbool(0, "out", record.out, msg);
                bfdebug_subbool(0, "dir", record.dir, msg);
            }

            bfdebug_lnbr(0, msg);
        });
    }

    if (!m_cr4_log.empty()) {
        bfdebug_transaction(0, [&](std::string * msg) {
            bfdebug_lnbr(0, msg);
            bfdebug_info(0, "cr4 log", msg);
            bfdebug_brk2(0, msg);

            for (const auto &record : m_cr4_log) {
                bfdebug_info(0, "record", msg);
                bfdebug_subnhex(0, "val", record.val, msg);
                bfdebug_subnhex(0, "shadow", record.shadow, msg);
                bfdebug_subbool(0, "out", record.out, msg);
                bfdebug_subbool(0, "dir", record.dir, msg);
            }

            bfdebug_lnbr(0, msg);
        });
    }

    if (!m_cr8_log.empty()) {
        bfdebug_transaction(0, [&](std::string * msg) {
            bfdebug_lnbr(0, msg);
            bfdebug_info(0, "cr8 log", msg);
            bfdebug_brk2(0, msg);

            for (const auto &record : m_cr8_log) {
                bfdebug_info(0, "record", msg);
                bfdebug_subnhex(0, "val", record.val, msg);
                bfdebug_subnhex(0, "shadow", record.shadow, msg);
                bfdebug_subbool(0, "out", record.out, msg);
                bfdebug_subbool(0, "dir", record.dir, msg);
            }

            bfdebug_lnbr(0, msg);
        });
    }
}

// -----------------------------------------------------------------------------
// Handlers
// -----------------------------------------------------------------------------

bool
crs::handle_crs(gsl::not_null<vmcs_t *> vmcs)
{
    using namespace vmcs_n::exit_qualification::control_register_access;

    switch (control_register_number::get()) {
        case 0:
            return handle_wrcr0(vmcs);

        case 3:
            return handle_cr3(vmcs);

        case 4:
            return handle_wrcr4(vmcs);

        case 8:
            return handle_cr8(vmcs);

        default:
            throw std::runtime_error("crs::handle_crs: invalid cr number");
    }
}

bool
crs::handle_cr3(gsl::not_null<vmcs_t *> vmcs)
{
    using namespace vmcs_n::exit_qualification::control_register_access;

    switch (access_type::get()) {
        case access_type::mov_from_cr:
            return handle_rdcr3(vmcs);

        case access_type::mov_to_cr:
            return handle_wrcr3(vmcs);

        default:
            throw std::runtime_error("crs::handle_cr3: invalid access type");
    }
}

bool
crs::handle_cr8(gsl::not_null<vmcs_t *> vmcs)
{
    using namespace vmcs_n::exit_qualification::control_register_access;

    switch (access_type::get()) {
        case access_type::mov_from_cr:
            return handle_rdcr8(vmcs);

        case access_type::mov_to_cr:
            return handle_wrcr8(vmcs);

        default:
            throw std::runtime_error("crs::handle_cr8: invalid access type");
    }
}

bool
crs::handle_wrcr0(gsl::not_null<vmcs_t *> vmcs)
{
    struct info_t info = {
        this->emulate_rdgpr(vmcs),
        vmcs_n::cr0_read_shadow::get(),
        false,
        false
    };

    if (!ndebug && this->is_logging_enabled()) {
        add_record(m_cr0_log, {
            info.val, info.shadow, false, false
        });
    }

    for (const auto &d : m_wrcr0_handlers) {
        if (d(vmcs, info)) {

            if (!ndebug && this->is_logging_enabled()) {
                add_record(m_cr0_log, {
                    info.val, info.shadow, true, false
                });
            }

            if (!info.ignore_write) {
                vmcs_n::guest_cr0::set(info.val);
                vmcs_n::cr0_read_shadow::set(info.shadow);
            }

            if (!info.ignore_advance) {
                return advance(vmcs);
            }

            return true;
        }
    }

    return false;
}

bool
crs::handle_rdcr3(gsl::not_null<vmcs_t *> vmcs)
{
    struct info_t info = {
        vmcs_n::guest_cr3::get(),
        0,
        false,
        false
    };

    if (!ndebug && this->is_logging_enabled()) {
        add_record(m_cr3_log, {
            info.val, info.shadow, false, true
        });
    }

    for (const auto &d : m_rdcr3_handlers) {
        if (d(vmcs, info)) {

            if (!ndebug && this->is_logging_enabled()) {
                add_record(m_cr3_log, {
                    info.val, info.shadow, true, true
                });
            }

            if (!info.ignore_write) {
                this->emulate_wrgpr(vmcs, info.val);
            }

            if (!info.ignore_advance) {
                return advance(vmcs);
            }

            return true;
        }
    }

    return false;
}

bool
crs::handle_wrcr3(gsl::not_null<vmcs_t *> vmcs)
{
    struct info_t info = {
        this->emulate_rdgpr(vmcs),
        0,
        false,
        false
    };

    if (!ndebug && this->is_logging_enabled()) {
        add_record(m_cr3_log, {
            info.val, info.shadow, false, false
        });
    }

    for (const auto &d : m_wrcr3_handlers) {
        if (d(vmcs, info)) {

            info.val &= 0x7FFFFFFFFFFFFFFF;

            if (!ndebug && this->is_logging_enabled()) {
                add_record(m_cr3_log, {
                    info.val, info.shadow, true, false
                });
            }

            if (!info.ignore_write) {
                vmcs_n::guest_cr3::set(info.val);
            }

            if (!info.ignore_advance) {
                return advance(vmcs);
            }

            return true;
        }
    }

    return false;
}

bool
crs::handle_wrcr4(gsl::not_null<vmcs_t *> vmcs)
{
    struct info_t info = {
        this->emulate_rdgpr(vmcs),
        vmcs_n::cr4_read_shadow::get(),
        false,
        false
    };

    if (!ndebug && this->is_logging_enabled()) {
        add_record(m_cr4_log, {
            info.val, info.shadow, false, false
        });
    }

    for (const auto &d : m_wrcr4_handlers) {
        if (d(vmcs, info)) {

            if (!ndebug && this->is_logging_enabled()) {
                add_record(m_cr4_log, {
                    info.val, info.shadow, true, false
                });
            }

            if (!info.ignore_write) {
                vmcs_n::guest_cr4::set(info.val);
                vmcs_n::cr4_read_shadow::set(info.shadow);
            }

            if (!info.ignore_advance) {
                return advance(vmcs);
            }

            return true;
        }
    }

    return false;
}

bool
crs::handle_rdcr8(gsl::not_null<vmcs_t *> vmcs)
{
    struct info_t info = {
        0,
        0,
        false,
        false
    };

    if (!ndebug && this->is_logging_enabled()) {
        add_record(m_cr8_log, {
            info.val, info.shadow, false, true
        });
    }

    for (const auto &d : m_rdcr8_handlers) {
        if (d(vmcs, info)) {

            if (!ndebug && this->is_logging_enabled()) {
                add_record(m_cr8_log, {
                    info.val, info.shadow, true, true
                });
            }

            if (!info.ignore_write) {
                this->emulate_wrgpr(vmcs, info.val);
            }

            if (!info.ignore_advance) {
                return advance(vmcs);
            }

            return true;
        }
    }

    return false;
}

bool
crs::handle_wrcr8(gsl::not_null<vmcs_t *> vmcs)
{
    struct info_t info = {
        this->emulate_rdgpr(vmcs),
        0,
        false,
        false
    };

    if (!ndebug && this->is_logging_enabled()) {
        add_record(m_cr0_log, {
            info.val, info.shadow, false, false
        });
    }

    for (const auto &d : m_wrcr8_handlers) {
        if (d(vmcs, info)) {

            if (!ndebug && this->is_logging_enabled()) {
                add_record(m_cr8_log, {
                    info.val, info.shadow, true, false
                });
            }

            if (!info.ignore_advance) {
                return advance(vmcs);
            }

            return true;
        }
    }

    return false;
}

}
}