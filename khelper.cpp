#include "khelper.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <regex>
#include <sstream>
#include <vector>

namespace khelper {
/// PARSE
auto parse_i32(const std::string &input) -> std::optional<int32_t> {
    try {
        return std::stol(input);
    } catch (...) { return {}; }
}
auto parse_i64(const std::string &input) -> std::optional<int64_t> {
    try {
        return std::stoll(input);
    } catch (...) { return {}; }
}
auto parse_u32(const std::string &input) -> std::optional<uint32_t> {
    try {
        return std::stoul(input);
    } catch (...) { return {}; }
}

auto parse_u64(const std::string &input) -> std::optional<uint64_t> {
    try {
        return std::stoull(input);
    } catch (...) { return {}; }
}

/// STRING
// clang-format off
// Can go to https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124
// if I want to implement more.
auto black(const std::string_view input)            -> std::string { return "\e[0;30m" + std::string{input} + "\e[0m"; }
auto red(const std::string_view input)              -> std::string { return "\e[0;31m" + std::string{input} + "\e[0m"; }
auto green(const std::string_view input)            -> std::string { return "\e[0;32m" + std::string{input} + "\e[0m"; }
auto yellow(const std::string_view input)           -> std::string { return "\e[0;33m" + std::string{input} + "\e[0m"; }
auto blue(const std::string_view input)             -> std::string { return "\e[0;34m" + std::string{input} + "\e[0m"; }
auto purple(const std::string_view input)           -> std::string { return "\e[0;35m" + std::string{input} + "\e[0m"; }
auto cyan(const std::string_view input)             -> std::string { return "\e[0;36m" + std::string{input} + "\e[0m"; }
auto white(const std::string_view input)            -> std::string { return "\e[0;37m" + std::string{input} + "\e[0m"; }
auto black_bold(const std::string_view input)       -> std::string { return "\e[1;30m" + std::string{input} + "\e[0m"; }
auto red_bold(const std::string_view input)         -> std::string { return "\e[1;31m" + std::string{input} + "\e[0m"; }
auto green_bold(const std::string_view input)       -> std::string { return "\e[1;32m" + std::string{input} + "\e[0m"; }
auto yellow_bold(const std::string_view input)      -> std::string { return "\e[1;33m" + std::string{input} + "\e[0m"; }
auto blue_bold(const std::string_view input)        -> std::string { return "\e[1;34m" + std::string{input} + "\e[0m"; }
auto purple_bold(const std::string_view input)      -> std::string { return "\e[1;35m" + std::string{input} + "\e[0m"; }
auto cyan_bold(const std::string_view input)        -> std::string { return "\e[1;36m" + std::string{input} + "\e[0m"; }
auto white_bold(const std::string_view input)       -> std::string { return "\e[1;37m" + std::string{input} + "\e[0m"; }
auto black_underline(const std::string_view input)  -> std::string { return "\e[4;30m" + std::string{input} + "\e[0m"; }
auto red_underline(const std::string_view input)    -> std::string { return "\e[4;31m" + std::string{input} + "\e[0m"; }
auto green_underline(const std::string_view input)  -> std::string { return "\e[4;32m" + std::string{input} + "\e[0m"; }
auto yellow_underline(const std::string_view input) -> std::string { return "\e[4;33m" + std::string{input} + "\e[0m"; }
auto blue_underline(const std::string_view input)   -> std::string { return "\e[4;34m" + std::string{input} + "\e[0m"; }
auto purple_underline(const std::string_view input) -> std::string { return "\e[4;35m" + std::string{input} + "\e[0m"; }
auto cyan_underline(const std::string_view input)   -> std::string { return "\e[4;36m" + std::string{input} + "\e[0m"; }
auto white_underline(const std::string_view input)  -> std::string { return "\e[4;37m" + std::string{input} + "\e[0m"; }
auto black_bg(const std::string_view input)         -> std::string { return "\e[40m" + std::string{input} + "\e[0m"; }
auto red_bg(const std::string_view input)           -> std::string { return "\e[41m" + std::string{input} + "\e[0m"; }
auto green_bg(const std::string_view input)         -> std::string { return "\e[42m" + std::string{input} + "\e[0m"; }
auto yellow_bg(const std::string_view input)        -> std::string { return "\e[43m" + std::string{input} + "\e[0m"; }
auto blue_bg(const std::string_view input)          -> std::string { return "\e[44m" + std::string{input} + "\e[0m"; }
auto purple_bg(const std::string_view input)        -> std::string { return "\e[45m" + std::string{input} + "\e[0m"; }
auto cyan_bg(const std::string_view input)          -> std::string { return "\e[46m" + std::string{input} + "\e[0m"; }
auto white_bg(const std::string_view input)         -> std::string { return "\e[47m" + std::string{input} + "\e[0m"; }
// clang-format on

auto args_vec(int argc, const char **argv) -> std::vector<std::string> {
    std::vector<std::string> output = {};
    for (int i = 0; i < argc; i++) {
        output.push_back(argv[i]);
    }
    return output;
}

auto string_break(const std::string_view input) -> std::vector<std::string> {
    std::vector<std::string> output = {};
    std::string tmp                 = "";
    bool in_string                  = false;
    char previous_char_is_backslash = false;

    for (size_t i = 0; i < input.size(); i++) {
        // Handling backslash
        if (input[i] == '\\') {
            // Marking an escape character.
            previous_char_is_backslash = true;
            tmp += input[i];
            continue;
        }

        // "string" = multiple whitespace-separated words.
        // "word" = singular word.
        if (input[i] == '\"' and !previous_char_is_backslash and !in_string) {
            // Starting string
            if (!tmp.empty()) {
                output.push_back(tmp);
                tmp.clear();
            }
            in_string = true;
        } else if (in_string and input[i] != '\"') {
            // In a string and continuing.
            tmp += input[i];
        } else if (in_string and previous_char_is_backslash
                   and input[i] == '\"') {
            // Found an escaped double quote in a string.
            tmp += input[i];
        } else if (in_string and !previous_char_is_backslash
                   and input[i] == '\"') {
            // Exiting a string.
            if (!tmp.empty()) {
                output.push_back(tmp);
                tmp.clear();
            }
        } else if (previous_char_is_backslash and input[i] == '\"') {
            // Found an escaped double quote.
            tmp += input[i];
        } else if (!std::isspace(input[i])) {
            // In word
            tmp += input[i];
        } else if (std::isspace(input[i]) and !tmp.empty()) {
            // Ending word
            output.push_back(tmp);
            tmp.clear();
        }
        previous_char_is_backslash = false;
    }

    if (!tmp.empty()) { output.push_back(tmp); }
    return output;
}
auto quote_string(const std::string_view input) -> std::string {
    return "\"" + std::string{input} + "\"";
}

auto split(const std::string_view input, const char &delim)
    -> std::vector<std::string> {
    std::vector<std::string> output = {};
    std::string elem                = "";
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] != delim) {
            elem += input[i];
        } else if (input[i] == delim and !elem.empty()) {
            output.push_back(elem);
            elem.clear();
        }
    }
    if (!elem.empty()) { output.push_back(elem); }
    return output;
}

auto find(const std::string_view needle, const std::string_view haystack)
    -> std::optional<size_t> {
    size_t out = haystack.find(needle);
    if (out == std::string::npos) { return {}; }
    return out;
}

// This one is really starting to push that line between just an aesthetic
// wrapper.
auto slice(const size_t start, const std::string &input) -> std::string {
    return input.substr(start);
}

auto slice(const size_t start, const size_t end, const std::string &input)
    -> std::string {
    return input.substr(start, end - start);
}

auto replace(const std::string_view input, const std::string_view from,
             const std::string_view to) -> std::string {
    std::string output{};
    for (usize i = 0; i < input.size();) {
        if (starts_with(from, &input[i])) {
            i += from.size();
            output += to;
        } else {
            output += input[i];
            i++;
        }
    }
    return output;
}

auto replacen(const std::string_view input, const std::string_view from,
              const std::string_view to, const size_t max_count)
    -> std::string {
    std::string output{};
    size_t remaining_replacements = max_count;

    for (usize i = 0; i < input.size();) {
        if (remaining_replacements > 0 && starts_with(from, &input[i])) {
            i += from.size();
            output += to;
            remaining_replacements--;
        } else {
            output += input[i];
            i++;
        }
    }
    return output;
}

auto lines(const std::string &input) -> std::vector<std::string> {
    std::vector<std::string> output = {};
    size_t start                    = 0;

    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\n') {
            output.push_back(input.substr(start, i - start));
            if (i + 1 < input.size()) { start = i + 1; }
        }
    }
    return output;
}

auto starts_with(const std::string_view needle, const std::string_view haystack)
    -> bool {
    // clang-format off
    if (needle.size() == 0)              return true;
    if (needle.size() > haystack.size()) return false;
    if (haystack == needle)              return true;
    // clang-format on

    for (size_t i = 0; i < needle.size(); i++) {
        if (haystack[i] != needle[i]) { return false; }
    }

    return true;
}

auto ends_with(const std::string_view needle, const std::string_view haystack)
    -> bool {
    // clang-format off
    if (needle.size() == 0)              return true;
    if (needle.size() > haystack.size()) return false;
    if (haystack == needle)              return true;
    // clang-format on

    for (size_t i = 0; i < needle.size(); i++) {
        if (haystack[haystack.size() - needle.size() + i] != needle[i]) {
            return false;
        }
    }

    return true;
}

auto to_lowercase(const std::string_view input) -> std::string {
    std::string output{input};
    for (auto &it : output) {
        it = tolower(it);
    }
    return output;
}

auto to_uppercase(const std::string_view input) -> std::string {
    std::string output{input};
    for (auto &it : output) {
        it = toupper(it);
    }
    return output;
}

auto strip_prefix(const std::string_view prefix, const std::string_view input)
    -> std::optional<std::string> {
    if (starts_with(prefix, input)) {
        return slice(prefix.size(), std::string{input});
    }
    return {};
}
auto strip_suffix(const std::string_view suffix, const std::string_view input)
    -> std::optional<std::string> {
    if (ends_with(suffix, input)) {
        return slice(0, input.size() - suffix.size(), std::string{input});
    }
    return {};
}

/// VECTOR
auto flag_value(const std::string_view lead_value,
                const std::vector<std::string> &input)
    -> std::optional<std::string> {
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == lead_value and i < input.size() - 1) {
            return make_optional(std::string{input[i + 1]});
        }
    }
    return {};
}

/// PRINTING
auto println() -> void {
    std::cout << std::endl;
}
auto eprintln() -> void {
    std::cerr << std::endl;
}

auto operator<<(std::ostream &os, const std::vector<uint8_t> &input)
    -> std::ostream & {
    os << "Vec { ";
    for (size_t i = 0; i < input.size(); i++) {
        os << std::to_string(input[i]);
        if (i != input.size() - 1) { os << ", "; }
    }
    os << " }";
    return os;
}

/// MISC
auto re_search(const std::string &re, const std::string &input) -> bool {
    return std::regex_search(input, std::regex(re));
}

auto lines_from_file(const std::string &input) -> std::vector<std::string> {
    std::ifstream file(input);
    std::ostringstream input_ss;
    input_ss << file.rdbuf();
    return lines(input_ss.str());
}
} // namespace khelper
