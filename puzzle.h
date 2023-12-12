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

struct RecordRow {
    std::vector<SpringStatus> records{};
    std::vector<int> damaged_groups{};

    uint64_t get_possible_arrangements();

private:
    std::vector<size_t> get_possibilities_for_group(size_t start_from, size_t group_index);
};

int puzzle_sample_1(const std::string &base_file_path);

int puzzle_sample_2(const std::string &base_file_path);

int puzzle_1(const std::string &base_file_path);

int puzzle_2(const std::string &base_file_path);

int do_puzzle_1(std::ifstream &file);

int do_puzzle_2(std::ifstream &file);

#endif //PUZZLE_H
