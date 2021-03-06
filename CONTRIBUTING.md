# Contributing to Intel® GPU Compute Samples
Thanks for your time! Below you can find guidelines for contributing to the project.

## Pull Requests
We are open for all kind of contributions including bug fixes, enhancements and documentation. Before creating a pull request please make sure that:
* Your changes can be successfully built.
* All tests are passing.
* Source code is formatted using [clang-format](https://clang.llvm.org/docs/ClangFormat.html) with the [defined style](.clang-format).

## Issues and enhancements
All bug reports and feature requests are welcomed and should be reported to the [issue tracker](https://github.com/intel/compute-samples/issues). Even more welcomed are pull requests fixing these issues.

## Testing
Testing instructions can be found in the [README](README.md).

## Directory structure
* `build` - build artifacts. Directory should be created by a user.
* `cmake` - custom CMake scripts.
* `compute_samples` - source code.
* `compute_samples/applications` - source code of the applications.
* `compute_samples/framework` - source code of the framework's modules.
* `compute_samples/launcher` - source code of the main binary.
* `docs` - documentation and other resources about the project.
* `mediadata` - directory with media data. It contains merged `internal` and `external` media data. This way applications don't need to know whether a media file is external or internal.
* `mediadata/external` - media files downloaded from external sources. Directory is generated by CMake.
* `mediadata/downloads` - temporary CMake files required for downloading external media data. Directory is generated by CMake.
* `mediadata/internal` - small media files stored in the repository.
* `third_party` - external dependencies. Directory is generated by CMake.
* `third_party/downloads` - temporary CMake files required for downloading third party projects. Directory is generated by CMake.

## How to add a new application?
1. Prepare directory structure:
    1. Create directory for `your_app` in `compute_samples/applications`.
    1. Create `include/your_app` directories for public header files in `compute_samples/applications/your_app`.
    1. Create `src` directory for source files and private headers in `compute_samples/applications/your_app`.
    1. Create `test` directory for test files in `compute_samples/applications/your_app`.
    1. All files which are not a source code e.g. video files should be placed in `mediadata/internal` directory or if they are too big they should be downloaded into `mediadata/external` directory.

    Example directory structure for `compute_samples/applications/your_app`:
    ```
    ├── CMakeLists.txt
    ├── include
    │   └── your_app
    │       └── your_app.hpp
    ├── your_app_kernel.cl
    ├── src
    │   └── your_app.cpp
    └── test
        ├── your_app_integration_tests.cpp
        ├── your_app_system_tests.cpp
        └── your_app_unit_tests.cpp
    ```
1. Prepare CMake:
    1. Add subdirectory with `your_app` to the `compute_samples/applications/CMakeLists.txt`.
    1. Create a `CMakeLists.txt` file in `compute_samples/applications/your_app`.
        1. Create `your_app` library.
        1. Create `your_app_tests` binary.
        1. Add custom post build command to copy files to the binary directory. This technique should be used only for files which may be modified between the builds.
        1. Copy static files to the binary directory. This technique should be used only for files which won't be modified between the builds. Path to the common media directory is available in `MEDIA_DIRECTORY`.
        1. Install all required files in the root directory (`.`).
        1. Add test command lines.

        Example `CMakeLists.txt`
        ```CMake
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
        project(your_app)

        set(SOURCES
            include/your_app/your_app.hpp
            src/your_app.cpp
            your_app_kernel.cl
        )

        add_library(${PROJECT_NAME} ${SOURCES})
        add_library(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

        target_include_directories(${PROJECT_NAME}
            PUBLIC
            ${PROJECT_SOURCE_DIR}/include
        )

        target_link_libraries(${PROJECT_NAME}
            PUBLIC
            compute_samples::application
            Boost::program_options
        )

        target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

        file(COPY "${MEDIA_DIRECTORY}/png/your_app_media.png" DESTINATION ${CMAKE_BINARY_DIR})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/your_app_kernel.cl" ${CMAKE_BINARY_DIR})

        install(FILES "${PROJECT_SOURCE_DIR}/your_app_kernel.cl" DESTINATION ".")

        source_group("Kernel Files" FILES your_app_kernel.cl)
        set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER applications/${PROJECT_NAME})

        set(TEST_SOURCES
            test/your_app_system_tests.cpp
        )

        add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
        add_executable(compute_samples::${PROJECT_NAME}_tests ALIAS ${PROJECT_NAME}_tests)

        target_link_libraries(${PROJECT_NAME}_tests
            PUBLIC
            ${PROJECT_NAME}
            compute_samples::framework
            GTest::GTest
            GTest::Main
        )

        set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER applications/${PROJECT_NAME})

        add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/your_app_kernel.cl" ${CMAKE_CURRENT_BINARY_DIR})

        install(TARGETS ${PROJECT_NAME}_tests DESTINATION ".")

        add_test(${PROJECT_NAME}_tests, ${PROJECT_NAME}_tests)
        ```
1. Prepare the source code:
    1. Create `YourApplication` class which is derived from `Application` interface available in `application/application.hpp` header.
    1. Implement all methods required by the `Application` interface e.g. `Application::run`.
    1. Add tests using [Google Test](https://github.com/google/googletest) to the `compute_samples/applications/your_app/test` directory.
1. Add `your_app` to the framework:
    1. Link `your_app` with the framework in `compute_samples/framework/framework/CMakeLists.txt`.
    1. Include `your_app/your_app.hpp` header in `compute_samples/framework/framework/src/framework.cpp`.
    1. Add `YourApplication` to the application factory named `applications` with `your_app` id in `compute_samples/framework/framework/src/framework.cpp`.
