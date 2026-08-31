#include <iostream>

#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"


int main()
{
    try
    {
        ConverterJSON converter;

        std::cout << "Starting "
                  << converter.GetEngineName()
                  << " v"
                  << converter.GetEngineVersion()
                  << "..."
                  << std::endl;

        auto documents = converter.GetTextDocuments();

        InvertedIndex index;
        index.UpdateDocumentBase(documents);

        SearchServer server(index);

        auto requests = converter.GetRequests();
        size_t limit = converter.GetResponsesLimit();

        auto answers = server.Search(requests, limit);

        converter.PutAnswers(answers);

        std::cout << "Search completed. "
                  << "Results written to answers.json."
                  << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}