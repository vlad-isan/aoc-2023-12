//
// Created by Vlad Isan on 05/12/2023.
//

#include "puzzle.h"

uint64_t puzzle_sample_1(const std::string &base_file_path) {
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

uint64_t puzzle_sample_2(const std::string &base_file_path) {
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

uint64_t puzzle_1(const std::string &base_file_path) {
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

uint64_t puzzle_2(const std::string &base_file_path) {
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

uint64_t do_puzzle_1(std::ifstream &file) {
    std::string line;
    std::vector<RecordRow> records{};
    uint64_t sum = 0;

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

        sum += record_row.get_count_memoization(0, 0);
        records.push_back(record_row);
    }

    return sum;
}

uint64_t do_puzzle_2(std::ifstream &file) {
    std::string line;
    std::vector<RecordRow> records{};
    uint64_t sum = 0;
    uint64_t line_count = 0;

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

        record_row.unfold();

        sum += record_row.get_count_memoization(0, 0);

        records.push_back(record_row);
    }

    return sum;
}

void RecordRow::unfold() {
    std::vector<SpringStatus> temp_records;
    std::vector<int> temp_damaged_groups;

    for (size_t i = 0; i < 5; ++i) {
        if (i != 0) {
            temp_records.emplace_back(SpringStatus::UNKNOWN);
        }

        std::copy(this->records.begin(), this->records.end(), std::back_inserter(temp_records));
        std::copy(this->damaged_groups.begin(), this->damaged_groups.end(), std::back_inserter(temp_damaged_groups));
    }

    this->records = temp_records;
    this->damaged_groups = temp_damaged_groups;
}

uint64_t RecordRow::get_count(size_t record_index, size_t group_index) {
    if (record_index >= this->records.size()) {
        if (group_index == this->damaged_groups.size()) {
            return 1;
        } else {
            return 0;
        }
    }

    if (group_index == this->damaged_groups.size()) {
        if (std::ranges::count_if(this->records.begin() + record_index, this->records.end(),
                                  [](auto &x) { return x == SpringStatus::DAMAGED; }) == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    uint64_t result = 0;

    if (this->records[record_index] == SpringStatus::OPERATIONAL || this->records[record_index] == SpringStatus::UNKNOWN) {
        result += get_count(record_index + 1, group_index);
    }

    if (this->records[record_index] == SpringStatus::DAMAGED || this->records[record_index] == SpringStatus::UNKNOWN) {
        if (this->damaged_groups[group_index] + record_index <= this->records.size()
    && std::ranges::count_if(this->records.begin() + record_index, this->records.begin() + record_index + this->damaged_groups[group_index],
                                     [](auto &x) { return x == SpringStatus::OPERATIONAL; }) == 0
                                     && (this->damaged_groups[group_index] + record_index == this->records.size()
                                         || this->records[this->damaged_groups[group_index] + record_index] != SpringStatus::DAMAGED)
        ) {
            result += get_count(record_index + this->damaged_groups[group_index] + 1, group_index + 1);
        }
    }

    return result;
}

std::map<std::pair<std::vector<SpringStatus>, std::vector<int>>, uint64_t> memoization_map{};

uint64_t RecordRow::get_count_memoization(size_t record_index, size_t group_index) {

    if (record_index >= this->records.size()) {
        if (group_index == this->damaged_groups.size()) {
            return 1;
        } else {
            return 0;
        }
    }

    if (group_index == this->damaged_groups.size()) {
        if (std::ranges::count_if(this->records.begin() + record_index, this->records.end(),
                                  [](auto &x) { return x == SpringStatus::DAMAGED; }) == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    std::pair<std::vector<SpringStatus>, std::vector<int>> key{{this->records.begin() + record_index, this->records.end()}, {this->damaged_groups.begin() + group_index, this->damaged_groups.end()}};

    if (memoization_map.contains(key)) {
        return memoization_map[key];
    }

    uint64_t result = 0;

    if (this->records[record_index] == SpringStatus::OPERATIONAL || this->records[record_index] == SpringStatus::UNKNOWN) {
        result += get_count_memoization(record_index + 1, group_index);
    }

    if (this->records[record_index] == SpringStatus::DAMAGED || this->records[record_index] == SpringStatus::UNKNOWN) {
        if (this->damaged_groups[group_index] + record_index <= this->records.size()
            && std::ranges::count_if(this->records.begin() + record_index, this->records.begin() + record_index + this->damaged_groups[group_index],
                                     [](auto &x) { return x == SpringStatus::OPERATIONAL; }) == 0
            && (this->damaged_groups[group_index] + record_index == this->records.size()
                || this->records[this->damaged_groups[group_index] + record_index] != SpringStatus::DAMAGED)
                ) {
            result += this->get_count_memoization(record_index + this->damaged_groups[group_index] + 1, group_index + 1);
        }
    }

    memoization_map[key] = result;

    return result;
}
