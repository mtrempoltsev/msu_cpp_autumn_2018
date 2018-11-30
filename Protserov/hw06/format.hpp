#include <string>
#include <sstream>
#include <regex>

template<class T>
std::string argtostr(T&& arg)
{
    std::stringstream sstr;
    sstr << arg;

    return sstr.str();
}

template<class... ArgsT>
std::string format(const std::string& fmt, ArgsT&& ... args)
{
    constexpr ssize_t params = sizeof...(args);
    static const std::regex braced_num{"\\{\\d+\\}"};
    static const std::regex valid{"\\{(?!(\\d+)\\})"};
    if (std::regex_search(fmt.cbegin(), fmt.cend(), valid)) {
        throw std::runtime_error("");
    }
    size_t openbr = 0;
    size_t closebr = 0;
    for (auto x : fmt) {
        if (x == '{') {
            ++openbr;
        } else if (x == '}') {
            ++closebr;
        }
    }
    if (openbr != closebr) {
        throw std::runtime_error("");
    }
    std::stringstream sstr;
    std::vector<size_t> match_idxs;
    std::vector<size_t> match_lens;
    std::vector<size_t> argnums;
    ssize_t max_argnum = -1;
    for (auto it = std::sregex_iterator(fmt.cbegin(), fmt.cend(), braced_num);
            it != std::sregex_iterator(); ++it) {
        match_idxs.push_back(it->position());
        match_lens.push_back(it->str().length());
        const auto& tmp = it->str();
        ssize_t argnum{0};
        try {
            argnum = stoll(tmp.substr(1, tmp.length() - 2));
            if (argnum < 0) {
                throw std::runtime_error("");
            }
        } catch (std::invalid_argument&) {
            throw std::runtime_error("");
        } catch (std::out_of_range&) {
            throw std::runtime_error("");
        }
        if (argnum > max_argnum) {
            max_argnum = argnum;
        }
        argnums.push_back(argnum);
    }
    const size_t places = argnums.size();
    if (max_argnum >= params) {
        throw std::runtime_error("");
    }
    if (params == 0) {
        return fmt;
    }
    std::vector<std::string> paramstrs{argtostr(std::forward<ArgsT>(args))...};
    size_t lastpos = 0;
    for (size_t j = 0; j < places; ++j) {
        sstr << fmt.substr(lastpos, match_idxs[j] - lastpos);
        lastpos = match_idxs[j] + match_lens[j];
        sstr << paramstrs[argnums[j]];
    }
    if (lastpos != fmt.size()) {
        sstr << fmt.substr(lastpos);
    }
    return sstr.str();
}
