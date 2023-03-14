cmake_minimum_required(VERSION 3.23)
project(dgulevich-compiler)

# --------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

# --------------------------------------------------------------------

find_package(fmt REQUIRED)

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.13.9
)
FetchContent_MakeAvailable(Catch2)

# --------------------------------------------------------------------

add_subdirectory(src)

add_subdirectory(app)

add_subdirectory(tests)

# --------------------------------------------------------------------
