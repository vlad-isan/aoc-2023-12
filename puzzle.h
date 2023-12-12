//
// Created by Vlad Isan on 05/12/2023.
//

#ifndef PUZZLE_H
#define PUZZLE_H

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <thread>

#include "fmt/core.h"

enum class SpringStatus {
    OPERATIONAL,
    DAMAGED,
    UNKNOWN
};

const std::unordered_map<char, SpringStatus> spring_status_map{
        {'.', SpringStatus::OPERATIONAL},
        {'#', SpringStatus::DAMAGED},
        {'?', SpringStatus::UNKNOWN}
};

extern std::map<std::pair<std::vector<SpringStatus>, std::vector<int>>, uint64_t> memoization_map;

struct RecordRow {
    std::vector<SpringStatus> records{};
    std::vector<int> damaged_groups{};

    uint64_t get_count(size_t record_index, size_t group_index);
    uint64_t get_count_memoization(size_t record_index, size_t group_index);
    void unfold();
};

uint64_t puzzle_sample_1(const std::string &base_file_path);

uint64_t puzzle_sample_2(const std::string &base_file_path);

uint64_t puzzle_1(const std::string &base_file_path);

uint64_t puzzle_2(const std::string &base_file_path);

uint64_t do_puzzle_1(std::ifstream &file);

uint64_t do_puzzle_2(std::ifstream &file);

#endif //PUZZLE_H
