#pragma once
#include <vector>
#include <string>
#include <utility>
#include "SearchServer.h"

class ConverterJSON {
public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
    void PutAnswers(const std::vector<std::vector<RelativeIndex>>& answers);

    std::string GetEngineName() const;
    std::string GetEngineVersion() const;
};