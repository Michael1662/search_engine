cmake_minimum_required(VERSION 3.20)

project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# =========================
# nlohmann/json
# =========================

FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(nlohmann_json)

# =========================
# GoogleTest
# =========================

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

enable_testing()

# =========================
# Основная программа
# =========================

add_executable(search_engine
        main.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
)

target_link_libraries(search_engine
        PRIVATE
        nlohmann_json::nlohmann_json
)

# =========================
# Файлы, необходимые программе
# =========================

configure_file(
        ${CMAKE_SOURCE_DIR}/config.json
        ${CMAKE_BINARY_DIR}/config.json
        COPYONLY
)

configure_file(
        ${CMAKE_SOURCE_DIR}/requests.json
        ${CMAKE_BINARY_DIR}/requests.json
        COPYONLY
)

file(COPY ${CMAKE_SOURCE_DIR}/resources
        DESTINATION ${CMAKE_BINARY_DIR}
)

# =========================
# Тесты
# =========================

add_executable(search_engine_tests
        tests/tests.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
)

target_link_libraries(search_engine_tests
        PRIVATE
        nlohmann_json::nlohmann_json
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(search_engine_tests)