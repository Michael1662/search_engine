# Search Engine

Поисковый движок для корпоративного портала, разработанный на C++.

## Возможности

- загрузка документов из файлов;
- построение инвертированного индекса;
- многопоточная обработка документов;
- поиск по поисковым запросам;
- расчёт относительной релевантности документов;
- ограничение количества результатов;
- сохранение результатов поиска в JSON.

## Требования

- C++17
- CMake 3.20 или выше
- GoogleTest
- nlohmann/json

## Структура проекта

```text
search_engine/
├── CMakeLists.txt
├── main.cpp
├── ConverterJSON.h
├── ConverterJSON.cpp
├── InvertedIndex.h
├── InvertedIndex.cpp
├── SearchServer.h
├── SearchServer.cpp
├── config.json
├── requests.json
├── answers.json
├── resources/
│   ├── file001.txt
│   └── file002.txt
└── tests/
    └── tests.cpp