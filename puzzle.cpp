//
// Created by Vlad Isan on 05/12/2023.
//

#include "puzzle.h"

int puzzle_sample_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

int puzzle_sample_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

int puzzle_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

int puzzle_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

int do_puzzle_1(std::ifstream &file) {
    std::string line;
    std::vector<RecordRow> records{};

    while (std::getline(file, line)) {
        RecordRow record_row{};

        std::istringstream iss(line);
        std::string conditions;

        std::getline(iss, conditions, ' ');

        for (auto &condition: conditions) {
            record_row.records.push_back(spring_status_map.at(condition));
        }

        std::string groups;
        std::string group;
        std::getline(iss, groups, ' ');
        std::istringstream iss_groups(groups);

        while (std::getline(iss_groups, group, ',')) {
            record_row.damaged_groups.push_back(std::stoi(group));
        }

        records.push_back(record_row);
    }

    uint64_t sum = 0;

    for (auto &record: records) {
        sum += record.get_possible_arrangements();
    }

    return sum;
}

int do_puzzle_2(std::ifstream &file) {
    std::string line;

    while (std::getline(file, line)) {
        fmt::println("{}", line);
    }

    return 0;
}

uint64_t RecordRow::get_possible_arrangements() {
    std::vector<std::vector<size_t>> group_possibilities{};

    for (size_t i = 0; i < this->damaged_groups.size(); i++) {
        if (i > 0) {
            std::vector<size_t> temp_possibilities{};

            for (auto &start: group_possibilities[i - 1]) {
                auto end = start + this->damaged_groups[i - 1] + 1;
                auto possibilities = get_possibilities_for_group(end, i);

                temp_possibilities.insert_range(temp_possibilities.end(), possibilities);
            }

            group_possibilities.emplace_back(temp_possibilities);
        } else {
            auto possibilities = get_possibilities_for_group(0, i);
            group_possibilities.emplace_back(possibilities);
        }
    }

    auto super_valid = std::count_if(group_possibilities.back().begin(), group_possibilities.back().end(), [&](auto &x) {
        bool valid = true;

        for (size_t i = x + this->damaged_groups.back(); i < this->records.size(); i++) {
            if (this->records[i] == SpringStatus::DAMAGED) {
                valid = false;
                break;
            }
        }

        return valid;
    });

    return super_valid;
}

std::vector<size_t> RecordRow::get_possibilities_for_group(size_t start_from, size_t group_index) {
    std::vector<size_t> possibilities{};

    for (size_t i = start_from; i < this->records.size(); i++) {
        if (this->records[i] != SpringStatus::OPERATIONAL) {
            bool all_valid = true;

            for (size_t j = i; j < this->damaged_groups[group_index] + i; ++j) {
                if (this->records[j] == SpringStatus::OPERATIONAL) {
                    all_valid = false;
                    break;
                }
            }

            size_t end_index = i + this->damaged_groups[group_index];

            if (all_valid && (end_index <= this->records.size())) {
                if (end_index < this->records.size() && this->records[end_index] == SpringStatus::DAMAGED) {
                    all_valid = false;
                }

                if (all_valid) {
                    possibilities.emplace_back(i);
                }
            }

            if (this->records[i] == SpringStatus::DAMAGED) {
                break;
            }
        }
    }

    return possibilities;
}
