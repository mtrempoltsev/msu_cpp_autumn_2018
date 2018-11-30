#pragma once


#include <string>
#include <sstream>
#include <regex>


template<class T>
std::string to_str(T &&arg)
{
    std::stringstream ss;
    ss << arg;
    return ss.str();
}

template<class... Args>
std::string format(const std::string &s, Args &&...args)
{
    static const std::regex check("\\{(?!(\\d+)\\})");
    if (std::regex_search(s.cbegin(), s.cend(), check)) {
        throw std::runtime_error("");
    }

    int delta = 0;
    for (const auto &x : s) {
        delta += x == '{';
        delta -= x == '}';
    }
    if (delta != 0) {
        throw std::runtime_error("");
    }

    ssize_t max_num = -1;
    std::vector<size_t> lens, idxs, nums;
    static const std::regex number_in_brackets("\\{\\d+\\}");
    for (auto it = std::sregex_iterator(s.cbegin(), s.cend(),
            number_in_brackets); it != std::sregex_iterator(); ++it) {
        lens.push_back(it->str().length());
        idxs.push_back(it->position());

        const auto &t = it->str();
        ssize_t cur_num = 0;
        try {
            cur_num = stoll(t.substr(1, t.length() - 2));
            if (cur_num < 0) {
                throw std::runtime_error("");
            }
        } catch (std::out_of_range &) {
            throw std::runtime_error("");
        } catch (std::invalid_argument &) {
            throw std::runtime_error("");
        }

        nums.push_back(cur_num);
        max_num = std::max(max_num, cur_num);
    }

    constexpr ssize_t num_params = sizeof...(args);
    if (max_num >= num_params) {
        throw std::runtime_error("");
    }
    if (num_params == 0) {
        return s;
    }

    size_t prev_pos = 0;
    std::stringstream ss;
    std::vector<std::string> params{to_str(std::forward<Args>(args))...};
    for (size_t j = 0; j < nums.size(); ++j) {
        ss << s.substr(prev_pos, idxs[j] - prev_pos);
        prev_pos = idxs[j] + lens[j];
        ss << params[nums[j]];
    }
    if (prev_pos != s.size()) {
        ss << s.substr(prev_pos);
    }

    return ss.str();
}

