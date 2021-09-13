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

add_definitions(-DNAPI_VERSION=10)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(DAWN_ENABLE_DESKTOP_GL OFF)
set(DAWN_ENABLE_OPENGLES OFF)
set(DAWN_BUILD_EXAMPLES OFF)
set(DAWN_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
set(DAWN_JINJA2_DIR "${DAWN_THIRD_PARTY_DIR}/jinja/src/jinja2")
set(TINT_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
add_subdirectory(third_party/dawn ${CMAKE_BINARY_DIR}/dawn)

################################################################################
# Get the npm binary directory
execute_process(COMMAND npm bin
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NPM_BIN
)
string(REPLACE "\n" "" NPM_BIN ${NPM_BIN})
string(REPLACE "\"" "" NPM_BIN ${NPM_BIN})
################################################################################

################################################################################
# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
)
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
################################################################################

################################################################################
set(WEBGPU_INTEROP_DIR "${CMAKE_CURRENT_BINARY_DIR}/gen")
set(WEBGPU_INTEROP_H "${WEBGPU_INTEROP_DIR}/webgpu_interop.h")
set(WEBGPU_INTEROP_CC "${WEBGPU_INTEROP_DIR}/webgpu_interop.cc")
add_custom_command(
    COMMAND "go" "run" "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmd/idl2cpp/main.go"
            "--template" "${CMAKE_CURRENT_SOURCE_DIR}/src/webgpu_interop.h.tmpl"
            "--output" "${WEBGPU_INTEROP_H}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/webgpu.idl"
    DEPENDS "src/webgpu.idl"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/webgpu_interop.h.tmpl"
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmd/idl2cpp/main.go"
    OUTPUT "${WEBGPU_INTEROP_H}"
    COMMENT "Generating ${WEBGPU_INTEROP_H}"
)
add_custom_command(
    COMMAND "go" "run" "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmd/idl2cpp/main.go"
            "--template" "${CMAKE_CURRENT_SOURCE_DIR}/src/webgpu_interop.cc.tmpl"
            "--output" "${WEBGPU_INTEROP_CC}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/webgpu.idl"
    DEPENDS "src/webgpu.idl"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/webgpu_interop.cc.tmpl"
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmd/idl2cpp/main.go"
    OUTPUT "${WEBGPU_INTEROP_CC}"
    COMMENT "Generating ${WEBGPU_INTEROP_CC}"
)
################################################################################

set(WEBIDL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/webidl-napi)

include_directories(${CMAKE_JS_INC})
set(SRC_FILES
    "src/bindings/convert.h"
    "src/bindings/gpu.cc"
    "src/bindings/gpu.h"
    "src/bindings/gpuadapter.cc"
    "src/bindings/gpuadapter.h"
    "src/bindings/gpubuffer.cc"
    "src/bindings/gpubuffer.h"
    "src/bindings/gpucommandbuffer.cc"
    "src/bindings/gpucommandbuffer.h"
    "src/bindings/gpucommandencoder.cc"
    "src/bindings/gpucommandencoder.h"
    "src/bindings/gpudevice.cc"
    "src/bindings/gpudevice.h"
    "src/bindings/gpuqueue.cc"
    "src/bindings/gpuqueue.h"
    "src/bindings/gputexture.cc"
    "src/bindings/gputexture.h"
    "src/core_interop.cc"
    "src/core_interop.h"
    "src/webgpu_bindings.cc"
    "${WEBGPU_INTEROP_H}"
    "${WEBGPU_INTEROP_CC}"
)
add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} dawn_native dawncpp dawn_proc)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${NODE_ADDON_API_DIR}
    ${WEBIDL_SRC}
    ${WEBGPU_INTEROP_DIR}
)
