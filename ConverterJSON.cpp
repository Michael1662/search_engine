#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream file("config.json");
    if (!file.is_open()) {
        throw std::runtime_error("config file is missing");
    }
    json dict;
    file >> dict;
    if (!dict.contains("config") || dict["config"].empty()) {
        throw std::runtime_error("config file is empty");
    }

    std::vector<std::string> documents;
    if (dict.contains("files")) {
        for (const auto& p : dict["files"]) {
            std::string filepath = p;
            std::ifstream doc_file(filepath);
            if (!doc_file.is_open()) {
                std::cerr << "Warning: File not found -> " << filepath << std::endl;
                continue;
            }
            std::string content((std::istreambuf_iterator<char>(doc_file)),
                                std::istreambuf_iterator<char>());
            documents.push_back(content);
        }
    }
    return documents;
}

int ConverterJSON::GetResponsesLimit() {
    std::ifstream file("config.json");
    if (!file.is_open()) return 5;
    json dict;
    file >> dict;
    if (dict["config"].contains("max_responses")) {
        return dict["config"]["max_responses"];
    }
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream file("requests.json");
    if (!file.is_open()) {
        throw std::runtime_error("requests file is missing");
    }
    json dict;
    file >> dict;
    std::vector<std::string> requests;
    if (dict.contains("requests")) {
        for (const auto& req : dict["requests"]) {
            requests.push_back(req);
        }
    }
    return requests;
}

void ConverterJSON::PutAnswers(const std::vector<std::vector<RelativeIndex>>& answers) {
    json root;
    json answers_obj;

    for (size_t i = 0; i < answers.size(); ++i) {
        std::string req_id = "request" + std::string(3 - std::to_string(i + 1).length(), '0') + std::to_string(i + 1);
        json req_result;

        if (answers[i].empty()) {
            req_result["result"] = false;
        } else {
            req_result["result"] = true;
            json relevance_arr = json::array();
            for (const auto& p : answers[i]) {
                json rel_obj;
                rel_obj["docid"] = p.doc_id;
                rel_obj["rank"] = p.rank;
                relevance_arr.push_back(rel_obj);
            }
            req_result["relevance"] = relevance_arr;
        }
        answers_obj[req_id] = req_result;
    }

    root["answers"] = answers_obj;
    std::ofstream file("answers.json");
    file << std::setw(4) << root << std::endl;
}

std::string ConverterJSON::GetEngineName() const {
    std::ifstream file("config.json");
    if (!file.is_open()) throw std::runtime_error("config file is missing");
    json dict;
    file >> dict;
    if (!dict.contains("config") || !dict["config"].contains("name")) {
        throw std::runtime_error("config file is empty");
    }
    return dict["config"]["name"];
}

std::string ConverterJSON::GetEngineVersion() const {
    std::ifstream file("config.json");
    if (!file.is_open()) throw std::runtime_error("config file is missing");
    json dict;
    file >> dict;
    return dict["config"]["version"];
}