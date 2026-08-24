#include "SearchServer.h"
#include <sstream>
#include <set>
#include <map>
#include <algorithm>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, size_t max_responses) {
    std::vector<std::vector<RelativeIndex>> result;

    for (const auto& query : queries_input) {
        std::stringstream ss(query);
        std::string word;
        std::set<std::string> unique_words;

        while (ss >> word) {
            unique_words.insert(word);
        }

        if (unique_words.empty()) {
            result.push_back({});
            continue;
        }

        std::vector<std::pair<std::string, size_t>> sorted_words;
        for (const auto& w : unique_words) {
            auto entries = index.GetWordCount(w);
            size_t total_count = 0;
            for (const auto& e : entries) {
                total_count += e.count;
            }
            if (entries.empty()) {
                sorted_words.clear();
                break;
            }
            sorted_words.push_back({w, total_count});
        }

        if (sorted_words.empty()) {
            result.push_back({});
            continue;
        }

        std::sort(sorted_words.begin(), sorted_words.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        std::set<size_t> suitable_docs;
        auto first_word_entries = index.GetWordCount(sorted_words[0].first);
        for (const auto& e : first_word_entries) {
            suitable_docs.insert(e.doc_id);
        }

        for (size_t i = 1; i < sorted_words.size(); ++i) {
            std::set<size_t> current_doc_set;
            auto entries = index.GetWordCount(sorted_words[i].first);
            for (const auto& e : entries) {
                current_doc_set.insert(e.doc_id);
            }
            std::set<size_t> intersection;
            std::set_intersection(suitable_docs.begin(), suitable_docs.end(),
                                  current_doc_set.begin(), current_doc_set.end(),
                                  std::inserter(intersection, intersection.begin()));
            suitable_docs = intersection;
        }

        if (suitable_docs.empty()) {
            result.push_back({});
            continue;
        }

        std::map<size_t, float> absolute_relevance;
        float max_abs_rel = 0.0f;

        for (size_t doc_id : suitable_docs) {
            float abs_rel = 0.0f;
            for (const auto& sw : sorted_words) {
                auto entries = index.GetWordCount(sw.first);
                for (const auto& e : entries) {
                    if (e.doc_id == doc_id) {
                        abs_rel += static_cast<float>(e.count);
                    }
                }
            }
            absolute_relevance[doc_id] = abs_rel;
            if (abs_rel > max_abs_rel) {
                max_abs_rel = abs_rel;
            }
        }

        std::vector<RelativeIndex> rel_indexes;
        for (const auto& pair : absolute_relevance) {
            float rel = (max_abs_rel > 0) ? (pair.second / max_abs_rel) : 0.0f;
            rel_indexes.push_back({pair.first, rel});
        }

        std::sort(rel_indexes.begin(), rel_indexes.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            if (a.rank != b.rank) {
                return a.rank > b.rank;
            }
            return a.doc_id < b.doc_id;
        });

        if (rel_indexes.size() > max_responses) {
            rel_indexes.resize(max_responses);
        }

        result.push_back(rel_indexes);
    }

    return result;
}