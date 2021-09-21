# Copyright 2021 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 17)

project(dawnnode C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(DAWN_ENABLE_DESKTOP_GL OFF)
set(DAWN_ENABLE_OPENGLES OFF)
set(DAWN_BUILD_EXAMPLES OFF)
set(DAWN_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
set(DAWN_JINJA2_DIR "${DAWN_THIRD_PARTY_DIR}/jinja/src/jinja2")
set(DAWN_SPIRV_HEADERS_DIR "${DAWN_THIRD_PARTY_DIR}/spirv-headers")
set(DAWN_SPIRV_TOOLS_DIR "${DAWN_THIRD_PARTY_DIR}/spirv-tools")
set(TINT_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
set(NODE_ADDON_API_DIR   "${TINT_THIRD_PARTY_DIR}/node-addon-api")
set(NODE_API_HEADERS_DIR "${TINT_THIRD_PARTY_DIR}/node-api-headers/include")
set(WEBGPU_IDL_PATH "${TINT_THIRD_PARTY_DIR}/gpuweb/webgpu.idl")

add_subdirectory(third_party/dawn ${CMAKE_BINARY_DIR}/dawn)

################################################################################
set(GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/gen")
set(INTEROP_WEBGPU_H  "${GEN_DIR}/interop/webgpu.h")
set(INTEROP_WEBGPU_CC "${GEN_DIR}/interop/webgpu.cc")
add_custom_command(
    COMMAND "go" "run" "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmd/idlgen/main.go"
            "--template" "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/webgpu.h.tmpl"
            "--output" "${INTEROP_WEBGPU_H}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/browser.idl"
            "${WEBGPU_IDL_PATH}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/browser.idl"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/webgpu.h.tmpl"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/webgpu_common.tmpl"
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmd/idlgen/main.go"
            "${WEBGPU_IDL_PATH}"
    OUTPUT "${INTEROP_WEBGPU_H}"
    COMMENT "Generating ${INTEROP_WEBGPU_H}"
)
add_custom_command(
    COMMAND "go" "run" "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmd/idlgen/main.go"
            "--template" "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/webgpu.cc.tmpl"
            "--output" "${INTEROP_WEBGPU_CC}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/browser.idl"
            "${WEBGPU_IDL_PATH}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/browser.idl"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/webgpu.cc.tmpl"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/interop/webgpu_common.tmpl"
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmd/idlgen/main.go"
            "${WEBGPU_IDL_PATH}"
    OUTPUT "${INTEROP_WEBGPU_CC}"
    COMMENT "Generating ${INTEROP_WEBGPU_CC}"
)
################################################################################

set(WEBIDL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/webidl-napi)

include_directories(${CMAKE_JS_INC})
set(SRC_FILES
    "src/bindings/async_runner.cc"
    "src/bindings/async_runner.h"
    "src/bindings/convert.cc"
    "src/bindings/convert.h"
    "src/bindings/errors.cc"
    "src/bindings/errors.h"
    "src/bindings/gpu.cc"
    "src/bindings/gpu.h"
    "src/bindings/gpuadapter.cc"
    "src/bindings/gpuadapter.h"
    "src/bindings/gpubindgroup.cc"
    "src/bindings/gpubindgroup.h"
    "src/bindings/gpubindgrouplayout.cc"
    "src/bindings/gpubindgrouplayout.h"
    "src/bindings/gpubuffer.cc"
    "src/bindings/gpubuffer.h"
    "src/bindings/gpucommandbuffer.cc"
    "src/bindings/gpucommandbuffer.h"
    "src/bindings/gpucommandencoder.cc"
    "src/bindings/gpucommandencoder.h"
    "src/bindings/gpucomputepassencoder.cc"
    "src/bindings/gpucomputepassencoder.h"
    "src/bindings/gpucomputepipeline.cc"
    "src/bindings/gpucomputepipeline.h"
    "src/bindings/gpudevice.cc"
    "src/bindings/gpudevice.h"
    "src/bindings/gpupipelinelayout.cc"
    "src/bindings/gpupipelinelayout.h"
    "src/bindings/gpuqueryset.cc"
    "src/bindings/gpuqueryset.h"
    "src/bindings/gpuqueue.cc"
    "src/bindings/gpuqueue.h"
    "src/bindings/gpurenderbundle.cc"
    "src/bindings/gpurenderbundle.h"
    "src/bindings/gpurenderbundleencoder.cc"
    "src/bindings/gpurenderbundleencoder.h"
    "src/bindings/gpurenderpassencoder.cc"
    "src/bindings/gpurenderpassencoder.h"
    "src/bindings/gpurenderpipeline.cc"
    "src/bindings/gpurenderpipeline.h"
    "src/bindings/gpusampler.cc"
    "src/bindings/gpusampler.h"
    "src/bindings/gpushadermodule.cc"
    "src/bindings/gpushadermodule.h"
    "src/bindings/gpusupportedlimits.cc"
    "src/bindings/gpusupportedlimits.h"
    "src/bindings/gputexture.cc"
    "src/bindings/gputexture.h"
    "src/bindings/gputextureview.cc"
    "src/bindings/gputextureview.h"
    "src/interop/core.cc"
    "src/interop/core.h"
    "src/webgpu.cc"
    "${INTEROP_WEBGPU_H}"
    "${INTEROP_WEBGPU_CC}"
)
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} dawn_native dawncpp dawn_proc)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${NODE_API_HEADERS_DIR}
    ${NODE_ADDON_API_DIR}
    ${WEBIDL_SRC}
    ${GEN_DIR}
)
