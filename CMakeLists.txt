#
# Bareflank Hypervisor
# Copyright (C) 2018 Assured Information Security, Inc.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

if(ENABLE_BUILD_VMM)
    vmm_extension(
        eapis
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src
    )

    vmm_extension(
        eapis_main
        DEPENDS eapis
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/main
    )

    vmm_extension(
        eapis_integration_intel_x64_cpuid
        DEPENDS eapis
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/integration/arch/intel_x64/cpuid/
    )

    vmm_extension(
        eapis_integration_intel_x64_crs
        DEPENDS eapis
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/integration/arch/intel_x64/crs/
    )

    vmm_extension(
        eapis_integration_intel_x64_drs
        DEPENDS eapis
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/integration/arch/intel_x64/drs/
    )

    vmm_extension(
        eapis_integration_intel_x64_msrs
        DEPENDS eapis
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/integration/arch/intel_x64/msrs/
    )

    vmm_extension(
        eapis_integration_intel_x64_vpid
        DEPENDS eapis
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/integration/arch/intel_x64/vpid/
    )
endif()

# if(ENABLE_BUILD_TEST)
#     test_extension(
#         eapis
#         SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/tests
#     )
# endif()