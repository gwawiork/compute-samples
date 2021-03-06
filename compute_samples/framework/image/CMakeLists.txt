#
# Copyright(c) 2018 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required(VERSION 3.8)
project(image)

set(SOURCES
    include/image/image.hpp
    src/image.cpp
    src/bmp.hpp
    src/bmp.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
	PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Boost::boost
    PNG::PNG
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER framework/${PROJECT_NAME})

set(TEST_SOURCES
    test/image_integration_tests.cpp
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
add_executable(compute_samples::${PROJECT_NAME}_tests ALIAS ${PROJECT_NAME}_tests)

target_link_libraries(${PROJECT_NAME}_tests
    PUBLIC
    ${PROJECT_NAME}
    GTest::GTest
    GTest::Main
)

set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER framework/${PROJECT_NAME})

file(COPY "${MEDIA_DIRECTORY}/png/rgb_3x1.png" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/bmp/kwkw_4x1_mono.bmp" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/bmp/rgb_3x1_argb.bmp" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME}_tests DESTINATION ".")
install(FILES "${MEDIA_DIRECTORY}/png/rgb_3x1.png" DESTINATION ".")
install(FILES "${MEDIA_DIRECTORY}/bmp/kwkw_4x1_mono.bmp" DESTINATION ".")
install(FILES "${MEDIA_DIRECTORY}/bmp/rgb_3x1_argb.bmp" DESTINATION ".")

add_test(${PROJECT_NAME}_tests, ${PROJECT_NAME}_tests)
