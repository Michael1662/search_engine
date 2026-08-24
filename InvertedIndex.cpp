#include "InvertedIndex.h"
#include <sstream>
#include <thread>
#include <algorithm>

void InvertedIndex::ProcessDocument(size_t doc_id, const std::string& text) {
    std::map<std::string, size_t> local_freq;
    std::stringstream ss(text);
    std::string word;
    while (ss >> word) {
        local_freq[word]++;
    }

    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& pair : local_freq) {
        freq_dictionary[pair.first].push_back({doc_id, pair.second});
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    freq_dictionary.clear();

    std::vector<std::thread> threads;
    threads.reserve(docs.size());

    for (size_t i = 0; i < docs.size(); ++i) {
        threads.emplace_back(&InvertedIndex::ProcessDocument, this, i, docs[i]);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    std::lock_guard<std::mutex> lock(mtx);
    if (freq_dictionary.find(word) != freq_dictionary.end()) {
        return freq_dictionary[word];
    }
    return {};
}