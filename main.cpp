#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    try {
        ConverterJSON converter;
        std::cout << "Starting " << converter.GetEngineName() << " v" << converter.GetEngineVersion() << "..." << std::endl;

        auto docs = converter.GetTextDocuments();
        InvertedIndex index;
        index.UpdateDocumentBase(docs);

        SearchServer srv(index);
        auto requests = converter.GetRequests();
        size_t limit = converter.GetResponsesLimit();

        auto answers = srv.search(requests, limit);
        converter.PutAnswers(answers);
        std::cout << "Search completed. Results written to answers.json." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Запуск юнит-тестов Google Test
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}