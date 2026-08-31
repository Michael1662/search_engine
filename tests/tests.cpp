#include <gtest/gtest.h>

#include "InvertedIndex.h"
#include "SearchServer.h"

#include <string>
#include <vector>


TEST(InvertedIndexTest, UpdateDocumentBase)
{
    InvertedIndex index;

    std::vector<std::string> documents = {
        "milk sugar salt",
        "milk milk water"
    };

    index.UpdateDocumentBase(documents);

    auto milk = index.GetWordCount("milk");

    ASSERT_EQ(milk.size(), 2);

    EXPECT_EQ(milk[0].doc_id, 0);
    EXPECT_EQ(milk[0].count, 1);

    EXPECT_EQ(milk[1].doc_id, 1);
    EXPECT_EQ(milk[1].count, 2);
}


TEST(InvertedIndexTest, UnknownWord)
{
    InvertedIndex index;

    std::vector<std::string> documents = {
        "milk sugar salt"
    };

    index.UpdateDocumentBase(documents);

    auto result = index.GetWordCount("banana");

    EXPECT_TRUE(result.empty());
}


TEST(SearchServerTest, SearchExistingWord)
{
    InvertedIndex index;

    std::vector<std::string> documents = {
        "milk sugar salt",
        "milk milk water"
    };

    index.UpdateDocumentBase(documents);

    SearchServer server(index);

    std::vector<std::string> requests = {
        "milk"
    };

    auto result = server.Search(requests, 5);

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].size(), 2);

    EXPECT_EQ(result[0][0].doc_id, 1);
    EXPECT_FLOAT_EQ(result[0][0].rank, 1.0f);

    EXPECT_EQ(result[0][1].doc_id, 0);
    EXPECT_FLOAT_EQ(result[0][1].rank, 0.5f);
}


TEST(SearchServerTest, SearchUnknownWord)
{
    InvertedIndex index;

    std::vector<std::string> documents = {
        "milk sugar salt",
        "milk milk water"
    };

    index.UpdateDocumentBase(documents);

    SearchServer server(index);

    std::vector<std::string> requests = {
        "banana"
    };

    auto result = server.Search(requests, 5);

    ASSERT_EQ(result.size(), 1);
    EXPECT_TRUE(result[0].empty());
}


TEST(SearchServerTest, SearchLimit)
{
    InvertedIndex index;

    std::vector<std::string> documents = {
        "milk",
        "milk",
        "milk"
    };

    index.UpdateDocumentBase(documents);

    SearchServer server(index);

    std::vector<std::string> requests = {
        "milk"
    };

    auto result = server.Search(requests, 2);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].size(), 2);
}