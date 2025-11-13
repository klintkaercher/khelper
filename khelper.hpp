#pragma once

#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#if __cplusplus >= 201703L
#include <optional>
#include <string_view>
#endif

namespace khelper {

using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;
using u64   = uint64_t;
using usize = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using String = std::string;
#if __cplusplus >= 201703L
using StringV = std::string_view;
#endif

template <typename T>
using Vec = std::vector<T>;

#if __cplusplus >= 201703L
template <typename T>
using Option = std::optional<T>;
#else
template <typename T>
using Option = KOption<T>;
#endif

using std::make_optional;

template <typename T>
using SPtr = std::shared_ptr<T>;
using std::make_shared;

template <typename T>
using UPtr = std::unique_ptr<T>;
using std::make_unique;

using std::literals::string_literals::operator""s;

/// RESULT
struct BadResultOkAccess : public std::exception {};
struct BadResultErrAccess : public std::exception {};

template <typename T, typename E>
struct Result {
    operator bool() {
        return (bool)this->value_;
    }

    auto value() -> T {
        if (this) {
            return this->value_.value();
        } else {
            throw BadResultOkAccess();
        }
    }

    auto err_value() -> E {
        if (this->err_value_) {
            return this->err_value_.value();
        } else {
            throw BadResultErrAccess();
        }
    }

    template <typename U, typename Transform>
    auto transform(Transform func) -> Result<U, E> {
        if (this) {
            return Result<U, E>{
                .value_     = func(this->value_.value()),
                .err_value_ = this->err_value_,
            };
        } else {
            return Result<U, E>{
                .value_     = {},
                .err_value_ = this->err_value_,
            };
        }
    }

    template <typename U, typename Transform>
    auto err_transform(Transform func) -> Result<T, U> {
        if (this->err_value_) {
            return Result<T, U>{
                .value_     = {},
                .err_value_ = func(this->err_value_.value()),
            };
        } else {
            return Result<T, U>{
                .value_     = this->value_,
                .err_value_ = {},
            };
        }
    }

    template <typename U>
    auto value_or(U alternative) -> U {
        if (this) {
            return this->value_.value();
        } else {
            return alternative;
        }
    }

    std::optional<T> value_     = std::nullopt;
    std::optional<E> err_value_ = std::nullopt;
};

template <typename T, typename E>
auto Ok(T input) -> Result<T, E> {
    return Result<T, E>{.value_ = std::make_optional(input)};
}

template <typename T, typename E>
auto Err(E input) -> Result<T, E> {
    return Result<T, E>{.err_value_ = std::make_optional(input)};
}

/// PARSE
auto parse_i32(const std::string &input) -> std::optional<int32_t>;
auto parse_i64(const std::string &input) -> std::optional<int64_t>;
auto parse_u32(const std::string &input) -> std::optional<uint32_t>;
auto parse_u64(const std::string &input) -> std::optional<uint64_t>;

/// STRING
template <typename Predicate>
auto find_char(Predicate p, const std::string_view input)
    -> std::optional<std::pair<size_t, char>> {
    for (size_t i = 0; i < input.size(); i++) {
        if (p(input[i])) { return std::make_pair(i, input[i]); }
    }
    return {};
}

auto black(const std::string_view input) -> std::string;
auto red(const std::string_view input) -> std::string;
auto green(const std::string_view input) -> std::string;
auto yellow(const std::string_view input) -> std::string;
auto blue(const std::string_view input) -> std::string;
auto purple(const std::string_view input) -> std::string;
auto cyan(const std::string_view input) -> std::string;
auto white(const std::string_view input) -> std::string;
auto black_bold(const std::string_view input) -> std::string;
auto red_bold(const std::string_view input) -> std::string;
auto green_bold(const std::string_view input) -> std::string;
auto yellow_bold(const std::string_view input) -> std::string;
auto blue_bold(const std::string_view input) -> std::string;
auto purple_bold(const std::string_view input) -> std::string;
auto cyan_bold(const std::string_view input) -> std::string;
auto white_bold(const std::string_view input) -> std::string;
auto black_underline(const std::string_view input) -> std::string;
auto red_underline(const std::string_view input) -> std::string;
auto green_underline(const std::string_view input) -> std::string;
auto yellow_underline(const std::string_view input) -> std::string;
auto blue_underline(const std::string_view input) -> std::string;
auto purple_underline(const std::string_view input) -> std::string;
auto cyan_underline(const std::string_view input) -> std::string;
auto white_underline(const std::string_view input) -> std::string;
auto black_bg(const std::string_view input) -> std::string;
auto red_bg(const std::string_view input) -> std::string;
auto green_bg(const std::string_view input) -> std::string;
auto yellow_bg(const std::string_view input) -> std::string;
auto blue_bg(const std::string_view input) -> std::string;
auto purple_bg(const std::string_view input) -> std::string;
auto cyan_bg(const std::string_view input) -> std::string;
auto white_bg(const std::string_view input) -> std::string;

auto args_vec(int argc, const char **argv) -> std::vector<std::string>;
auto string_break(const std::string_view input) -> std::vector<std::string>;
auto quote_string(const std::string_view input) -> std::string;
auto split(const std::string_view input, const char &delim)
    -> std::vector<std::string>;
auto find(const std::string_view needle, const std::string_view haystack)
    -> std::optional<size_t>;
auto replace(const std::string_view input, const std::string_view from,
             const std::string_view to) -> std::string;
auto replacen(const std::string_view input, const std::string_view from,
              const std::string_view to, const size_t max_count) -> std::string;
auto slice(const size_t start, const std::string &input) -> std::string;
auto slice(const size_t start, const size_t end, const std::string &input)
    -> std::string;
auto lines(const std::string &input) -> std::vector<std::string>;
auto starts_with(const std::string_view needle, const std::string_view haystack)
    -> bool;
auto ends_with(const std::string_view needle, const std::string_view haystack)
    -> bool;
auto to_lowercase(const std::string_view input) -> std::string;
auto to_uppercase(const std::string_view input) -> std::string;
auto strip_prefix(const std::string_view prefix, const std::string_view input)
    -> std::optional<std::string>;
auto strip_suffix(const std::string_view suffix, const std::string_view input)
    -> std::optional<std::string>;

/// FORMATTING
template <typename T>
auto format(const std::string_view fmt_string, T input) -> std::string {
    if (!find("{}", fmt_string)) { return std::string{fmt_string}; }

    std::ostringstream oss;
    if (auto it = find("{}", fmt_string); it) {
        oss << fmt_string.substr(0, it.value());
        oss << input;
        oss << ((it.value() + 2 < fmt_string.size())
                    ? fmt_string.substr(it.value() + 2)
                    : "");
    }
    return oss.str();
}

template <typename T, typename... Args>
auto format(const std::string_view fmt_string, T first, Args... rest)
    -> std::string {
    std::string processed = format(fmt_string, first);

    return format(processed, rest...);
}

/// SLICE
template <typename T>
struct Slice {
    size_t start_idx;
    size_t end_idx;
    const std::vector<T> &data;

    auto to_vec() -> std::vector<T> {
        std::vector<T> output;
        for (size_t i = this->start_idx; i < this->end_idx; i++) {
            output.push_back(this->data[i]);
        }
        return output;
    }

    auto size() -> size_t {
        return this->end_idx - this->start_idx;
    }

    auto begin() {
        return data.begin() + this->start_idx;
    }

    auto end() {
        return data.begin() + this->end_idx;
    }

    bool operator==(const Slice<T> &rhs) {
        return this->start_idx == rhs.start_idx and this->end_idx == rhs.end_idx
            and &this->data == &rhs.data;
    };

    bool operator!=(const Slice<T> &rhs) {
        return !(*this == rhs);
    };
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Slice<T> &rhs) {
    os << "Slice { .start_idx  = " << rhs.start_idx
       << ", .end_idx = " << rhs.end_idx << ", .data = " << &rhs.data << " }";
    return os;
};

/// VECTOR
template <typename T>
auto nth(const size_t index, const std::vector<T> &input) -> std::optional<T> {
    if (index < input.size()) { return std::make_optional(input[index]); }
    return {};
}

template <typename Transform, typename T, typename U>
auto flat_map(Transform func, std::vector<T> input) -> std::vector<U> {
    std::vector<U> output = {};
    for (const auto &it : input) {
        if (auto elem = func(it); elem) { output.push_back(elem.value()); }
    }
    return output;
}

/// F is f(acc, elem) -> new_acc.
template <typename T, typename U, typename F>
auto fold(U init, F func, std::vector<T> input) -> U {
    U acc{init};
    for (const auto &it : input) {
        acc = func(acc, it);
    }
    return acc;
}

/// F is f(acc, elem) -> <Some type that converts to bool>.
template <typename T, typename U, typename F>
auto try_fold(U init, F func, std::vector<T> input) -> U {
    U acc{init};
    for (const auto &it : input) {
        if (auto new_acc = func(acc, it); new_acc) {
            acc = new_acc;
        } else {
            return acc;
        }
    }
    return acc;
}

template <typename T>
auto slice(size_t start, size_t end, const std::vector<T> &input)
    -> std::optional<Slice<T>> {
    if (end < start or start >= input.size()) { return {}; }
    return Slice<T>{start, end, input};
}

template <typename T>
auto slice(size_t start, const std::vector<T> input)
    -> std::optional<Slice<T>> {
    return slice(start, input.size(), input);
}

template <typename Predicate, typename T>
auto take_while(Predicate pred, const std::vector<T> &input) -> std::vector<T> {
    std::vector<T> output = {};
    for (const auto &it : input) {
        if (pred(it)) {
            output.push_back(it);
        } else {
            break;
        }
    }

    return output;
}

// `destination` is being extended by `source`.
template <typename T, typename U>
auto append(std::vector<T> &dest, const std::vector<U> &src) {
    dest.reserve(dest.size() + src.size());
    dest.insert(dest.end(), src.begin(), src.end());
}

// Takes two vectors and returns them concatenated.
// While it can take two types, the output type is determined by the type of the
// first vector.
template <typename T, typename U>
auto concat(const std::vector<T> &first, const std::vector<U> &second)
    -> std::vector<T> {
    std::vector<T> output(first.size() + second.size());
    for (size_t i = 0; i < first.size(); i++) {
        output.push_back(first[i]);
    }
    for (size_t i = 0; i < second.size(); i++) {
        output.push_back(second[i]);
    }
    return output;
}

template <typename T>
auto flatten(const std::vector<std::optional<T>> &input) -> std::vector<T> {
    std::vector<T> output = {};
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i]) { output.push_back(input[i].value()); }
    }
    return output;
}

template <typename T>
auto flatten(const std::vector<std::vector<T>> &input) -> std::vector<T> {
    std::vector<T> output = {};
    for (const auto &it : input) {
        for (const auto &elem : it) {
            output.push_back(elem);
        }
    }
    return output;
}

template <typename T, typename U, typename Transform>
auto fmap(Transform func, const std::vector<T> &input) -> std::vector<U> {
    std::vector<T> output = {};
    for (const auto &it : input) {
        output.push_back(func(it));
    }
    return output;
}

template <typename T, typename Predicate>
auto filter(Predicate pred, const std::vector<T> &input) -> std::vector<T> {
    std::vector<T> output = {};
    for (const auto &it : input) {
        if (pred(it)) { output.push_back(it); }
    }
    return output;
}

template <typename T, typename Predicate>
auto retain(Predicate pred, std::vector<T> &input) -> std::vector<T> {
    std::vector<size_t> remove_indices = {};
    for (size_t i = 0; i < input.size(); i++) {
        if (!pred(input[i])) { remove_indices.push_back(i); }
    }

    for (size_t i = 0; i < remove_indices.size(); i++) {
        input.erase(remove_indices[remove_indices.size() - 1 - i]);
    }
}

template <typename T, typename U>
auto find(const U &needle, const std::vector<T> &haystack)
    -> std::optional<size_t> {
    for (size_t i = 0; i < haystack.size(); i++) {
        if (haystack[i] == needle) { return i; }
    }

    return {};
}

auto flag_value(const std::string_view lead_value,
                const std::vector<std::string> &input)
    -> std::optional<std::string>;

/// OPTIONAL
struct ExpectedOptionalValue : public std::exception {
    explicit ExpectedOptionalValue(const char *input) : value_(input) {
    }
    const char *what() const noexcept {
        return value_;
    }
    const char *value_;
};

/// KOPTION
// Maybe this should use heap allocations and a raw pointer for the value?
// So None values can just have a nullptr?
template <typename T>
struct KOption {
    KOption() {
    }

    KOption(T input) : value_(input), has_value_(true) {
    }

    bool has_value() const {
        return this->has_value_;
    }

    auto expect(std::string error_msg) const -> T {
        return (this) ? this->value_
                      : throw ExpectedOptionalValue(error_msg.c_str());
    }

    auto value() const -> T {
        return (this->has_value_) ? this->value_
                                  : throw std::bad_optional_access();
    }

    auto value_or(const T &input) const -> T {
        return (this->has_value_) ? this->value_ : input;
    }

    // Right now this assumes that `call()` returns KOption<T>.
    template <typename Callable>
    auto or_else(Callable call) -> KOption<T> {
        return (*this) ? KOption<T>{this->value()} : call();
    }

    template <typename U = T, typename Transform>
    auto transform(Transform func) -> KOption<U> {
        return (*this) ? KOption<U>{func(this->value_)} : KOption<U>{};
    }

    template <typename U = T, typename Transform>
    auto and_then(Transform func) -> KOption<U> {
        return (*this) ? func(this->value_) : KOption<U>{};
    }

    auto reset() {
        this->value_     = T();
        this->has_value_ = false;
    }

    auto to_optional() -> std::optional<T> {
        return *this ? std::make_optional(this->value_) : std::nullopt;
    }

    operator bool() const {
        return this->has_value_;
    }

    // KOPTION COMPARISONS
    template <typename U>
    bool operator==(const KOption<U> &rhs) {
        return (this->has_value() != rhs.has_value())
            ? false
            : (this->has_value() == false ? true
                                          : (this->value_ == rhs.value()));
    }

    template <typename U>
    bool operator!=(const KOption<U> &rhs) {
        return !(*this == rhs);
    }

    template <typename U>
    bool operator<(const KOption<U> &rhs) {
        return !rhs ? false
                    : (!this->has_value() ? true : this->value_ < rhs.value());
    }

    template <typename U>
    bool operator<=(const KOption<U> &rhs) {
        return !rhs ? false
                    : (!this->has_value() ? true : this->value_ <= rhs.value());
    }

    template <typename U>
    bool operator>(const KOption<U> &rhs) {
        return !this->has_value()
            ? false
            : (!rhs ? true : (this->value_ > rhs.value()));
    }

    template <typename U>
    bool operator>=(const KOption<U> &rhs) {
        return !rhs
            ? true
            : (!this->has_value() ? false : (this->value_ >= rhs.value()));
    }

    // // TODO: NEXT THING These might be goofed with how the template. Maybe we
    // // need comparisons with std::optional as well? specialization goes.
    // VALUE
    // // COMPARISONS
    bool operator==(const T &rhs) {
        return *this && this->value() == rhs;
    }

    friend bool operator==(const T &lhs, const KOption<T> &rhs) {
        return rhs ? rhs.value() == lhs : false;
    }

    bool operator!=(const T &rhs) {
        return !(*this == rhs);
    }

    friend bool operator!=(const T &lhs, const KOption<T> &rhs) {
        return !(lhs == rhs);
    }

    bool operator<(const T &rhs) {
        return *this ? this->value() < rhs : true;
    }

    friend bool operator<(const T &lhs, const KOption<T> &rhs) {
        return rhs ? lhs < rhs.value() : false;
    }

    bool operator<=(const T &rhs) {
        return *this ? this->value() <= rhs : true;
    }

    friend bool operator<=(const T &lhs, const KOption<T> &rhs) {
        return rhs ? lhs <= rhs.value() : false;
    }

    bool operator>(const T &rhs) {
        return *this ? this->value() > rhs : false;
    }

    friend bool operator>(const T &lhs, const KOption<T> &rhs) {
        return rhs ? lhs > rhs.value() : true;
    }

    friend bool operator>=(const KOption<T> &lhs, const T &rhs) {
        return lhs ? lhs.value() >= rhs : false;
    }

    friend bool operator>=(const T &lhs, const KOption<T> &rhs) {
        return rhs ? lhs >= rhs.value() : true;
    }

  private:
    // Need the trailing underscores to have the `value()` function.
    T value_{};
    bool has_value_ = false;
};

// TODO: This might not work with the Result function
template <typename T>
auto Some(T input) -> KOption<T> {
    KOption<T>{.value_ = input, .has_value_ = true};
}

// TODO: This might not work with the Result function
template <typename T>
auto None() -> Option<T> {
    return KOption{.value_ = T(), .has_value_ = false};
}

struct bad_optional_access : std::exception {
    explicit bad_optional_access() {
    }
};

template <typename T>
auto expect(std::string error_msg, const std::optional<T> &input) -> T {
    if (input) { return input.value(); }
    throw ExpectedOptionalValue(error_msg.c_str());
}

// Right now this assumes that `f()` returns some sort of std::optional<T>.
template <typename T, typename U, typename Callable>
auto or_else(const std::optional<T> &input, Callable call) -> U {
    return (input) ? input.value() : call();
}

// Right now this assumes that `f()` returns some sort of std::optional<T>.
// Calls `fmap` under the hood.
template <typename T, typename U, typename Transform>
auto transform(const std::optional<T> &input, Transform func)
    -> std::optional<U> {
    return (input) ? func(input.value()) : std::nullopt;
}

template <typename T, typename U, typename Transform>
auto and_then(const std::optional<T> &input, Transform func) -> U {
    return (input) ? func(input.value()) : std::nullopt;
}

// PRINTING
template <typename T>
auto operator<<(std::ostream &os, const KOption<T> &rhs) -> std::ostream & {
    if (rhs) {
        os << "Some(" << rhs.value() << ")";
    } else {
        os << "None";
    }
    return os;
}

template <typename T>
auto operator<<(std::ostream &os, const std::optional<T> &rhs)
    -> std::ostream & {
    if (rhs) {
        os << "Some(" << rhs.value() << ")";
    } else {
        os << "None";
    }
    return os;
}

template <typename T>
auto operator<<(std::ostream &os, const std::vector<T> &rhs) -> std::ostream & {
    os << "Vec { ";
    for (size_t i = 0; i < rhs.size(); i++) {
        os << rhs[i];
        if (i != rhs.size() - 1) { os << ", "; }
    }
    os << " }";
    return os;
}

template <typename T>
auto to_string(const std::optional<T> &input) -> std::string {
    std::ostringstream os;
    os << input;
    return os.str();
}

template <typename T, typename E>
auto operator<<(std::ostream &os, const Result<T, E> &input) -> std::ostream & {
    if (input) {
        os << "Ok(" << input.value() << ")";
    } else {
        os << "Err(" << input.err_value() << ")";
    }
    return os;
}

template <typename T, typename E>
auto to_string(const Result<T, E> &input) -> std::string {
    std::ostringstream oss;
    oss << input;
    return oss.str();
}

auto operator<<(std::ostream &os, const std::vector<uint8_t> &input)
    -> std::ostream &;

template <typename T>
auto operator<<(std::ostream &os, const std::set<T> &input) -> std::ostream & {
    os << "Set { ";
    std::string tmp = "";
    for (const auto &it : input) {
        tmp += it + ", ";
    }
    os << tmp.substr(0, tmp.size() - 2);
    os << " }";
    return os;
}

template <typename T>
auto to_string(const std::set<T> &input) -> std::string {
    std::ostringstream oss;
    oss << input;
    return oss.str();
}

auto println() -> void;
auto eprintln() -> void;

template <typename T>
auto println(const T &input) {
    std::cout << input << std::endl;
}

template <typename... Args>
auto println(const std::string_view fmt_string, Args... args) {
    std::cout << format(fmt_string, args...) << std::endl;
}

template <typename T>
auto eprintln(const T &input) {
    std::cerr << input << std::endl;
}

template <typename T>
auto debug(const T &input) -> const T & {
    println("debug: `{}`", input);
    return input;
}

template <typename T>
auto debug(T &input) -> T & {
    println(input);
    return input;
}

// TESTING
#define kexpect(x) expect_helper(x, "", __FILE__, __LINE__)
#define kexpect_msg(x, y) expect_helper(x, y, __FILE__, __LINE__)
#define kexpect_eq(x, y) expect_eq_helper(x, y, "", __FILE__, __LINE__)
#define kexpect_eq_msg(x, y, z) expect_eq_helper(x, y, z, __FILE__, __LINE__)

template <typename T>
auto expect_helper(T val, const std::string &error_msg = "",
                   const std::string &file_name = "",
                   const uint32_t line_number   = 0) {
    if (!val) {
        std::ostringstream oss;
        if (!file_name.empty() and line_number != 0) {
            oss << "In file: " << file_name << "\n"
                << "On line: " << line_number << "\n";
        }
        oss << "Expected true\nReceived `" << val << "`";
        if (!error_msg.empty()) { oss << "\n" << error_msg; }
        oss << "\n---";
        eprintln(oss.str());
    }
}

template <typename T, typename U>
auto expect_eq_helper(T lhs, U rhs, const std::string &err_msg = "",
                      const std::string_view file_name = "",
                      const uint32_t line_number       = 0) {
    if (lhs != rhs) {
        std::ostringstream oss;
        // clang-format off
        if (!file_name.empty() and line_number != 0) {
            oss << "In file: " << file_name << "\n"
                << "On line: " << line_number << "\n";
        }
        oss << "Expected `" << rhs << "`\n"
            << "Received `" << lhs << "`";
        if (!err_msg.empty()) oss << "\n" << err_msg;
        oss << "\n---";
        // clang-format on
        eprintln(oss.str());
    }
}

#define kassert(x) assert_helper(x, "", __FILE__, __LINE__)
#define kassert_msg(x, y) assert_helper(x, y, __FILE__, __LINE__)
#define kassert_eq(x, y) assert_eq_helper(x, y, "", __FILE__, __LINE__)
#define kassert_eq_msg(x, y, z) assert_eq_helper(x, y, z, __FILE__, __LINE__)

struct KhelperBadAssert : public std::exception {
    explicit KhelperBadAssert(const char *input) : value_(input) {
    }
    KhelperBadAssert(const std::string &input) : value_(input.c_str()) {
    }

    explicit KhelperBadAssert() : value_("") {
    }

    const char *what() const noexcept {
        return value_;
    }
    const char *value_;
};

template <typename T>
auto assert_helper(T val, const std::string &error_msg = "",
                   const std::string_view &file_name = "",
                   const uint32_t line_number        = 0) {
    if (!val) { throw KhelperBadAssert(error_msg); }
}

template <typename T, typename U>
auto assert_eq_helper(T lhs, U rhs, const std::string &error_msg = "",
                      const std::string_view file_name = "",
                      const uint32_t line_number       = 0) {
    if (lhs != rhs) { throw KhelperBadAssert(error_msg); }
}

/// MISC
auto re_search(const std::string &re, const std::string &input) -> bool;

auto lines_from_file(const std::string &input) -> std::vector<std::string>;

} // namespace khelper
