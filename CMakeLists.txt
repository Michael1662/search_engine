cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

if(POLICY CMP0000)
    cmake_policy(SET CMP0000 NEW)
endif()
if(POLICY CMP0174)
    cmake_policy(SET CMP0174 NEW)
endif()
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

include(FetchContent)

# Подключение библиотеки для работы с JSON
FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

# Подключение Google Test
set(gtest_disable_pthreads ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(search_engine
        main.cpp
        ConverterJSON.h
        ConverterJSON.cpp
        InvertedIndex.h
        InvertedIndex.cpp
        SearchServer.h
        SearchServer.cpp
)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json gtest_main)

include(GoogleTest)
gtest_discover_tests(search_engine)