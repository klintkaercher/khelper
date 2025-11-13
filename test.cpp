#include "khelper.hpp"
#include <optional>

using namespace khelper;

const constexpr bool TEST_ALL     = false;
const constexpr bool TEST_STRING  = TEST_ALL || true;
const constexpr bool TEST_RESULT  = TEST_ALL || true;
const constexpr bool TEST_VECTOR  = TEST_ALL || true;
const constexpr bool TEST_COLOR   = TEST_ALL || true;
const constexpr bool TEST_SLICE   = TEST_ALL || true;
const constexpr bool TEST_KOPTION = TEST_ALL || true;
const constexpr bool TEST_OTHER   = TEST_ALL || true;

auto test_string() {
    String s1 = "asdf"s;
    kexpect_eq(find("a", s1), make_optional(0));
    kexpect_eq(find("s", s1), make_optional(1));
    kexpect_eq(find("df", s1), make_optional(2));
    kexpect(!find("q", s1).has_value());
    kexpect_eq(to_uppercase(s1), "ASDF"s);
    String s2 = "ASDF"s;
    kexpect_eq(to_lowercase(s2), "asdf"s);
    kexpect_eq(to_lowercase("AKSi{}"s), "aksi{}"s);
    String s3 = "One Two Three";
    kexpect_eq(slice(0, 3, s3), "One"s);
    kexpect_eq(slice(4, s3), "Two Three"s);
    kexpect_eq(replace(s3, "Two", "2"), "One 2 Three");
    kexpect_eq(replace(s3, "e", ""), "On Two Thr");
    kexpect_eq(replacen(s3, "e", "-", 2), "On- Two Thr-e");
    kexpect_eq(strip_prefix("Mr. ", "Mr. Smith"), make_optional("Smith"));
    kexpect_eq(strip_suffix(" III", "Mike Smith III"),
               std::make_optional("Mike Smith"));
    kexpect(!strip_prefix("Mrs. ", "Mr. Smith").has_value());
    kexpect(!strip_suffix(" IV", "Mike Smith III").has_value());
    Vec<String> expected = {"one", "two", "three"};
    kexpect_eq(string_break("one two three"), expected);
    kexpect_eq(string_break("one      two three"), expected);
    kexpect_eq(string_break("one two      three"), expected);
    kexpect_eq_msg(string_break("one\ttwo      three"), expected, "Using tabs");
    kexpect_eq_msg(string_break("one\t\r\ntwo      three"), expected,
                   "More complicated whitespace");
    kexpect_eq(string_break("    one\t\r\ntwo      three"), expected);

    expected = {"one", "two three"};
    kexpect_eq(string_break("one \"two three\""), expected);

    expected = {"o\\\"ne", "two three"};
    kexpect_eq(string_break("o\\\"ne \"two three\""), expected);

    expected = {"o\\\"ne", "two three"};
    kexpect_eq(string_break("o\\\"ne \"two three\""), expected);

    expected = {"--option", "--flag", "-t", "-s", "one two"};
    kexpect_eq(string_break(R"(--option --flag -t -s "one two")"), expected);
    expect_eq_helper(
        string_break(R"(--option --flag -t -s "one two")"),
        std::vector<std::string>{"--option", "--flag", "-t", "-s", "one two"});
}

auto test_result() {
    Result res1 = Ok<i32, i32>(42);
    Result res2 = Err<String, i32>(62);

    Result res3 = Ok<String, i32>("Hello");
    Result<usize, i32> res4
        = res3.transform<usize>([](auto it) { return it.size(); });
    Result res5 = res2.err_transform<bool>([](auto it) { return it % 2 == 0; });
    kexpect_eq(res5.err_value(), true);
}

auto test_vector() {
    Vec<u32> v1 = {1, 2, 3, 4, 5, 6};
    // clang-format off
    Vec<u32> expected1 = {6, 9};
    kexpect_eq(take_while([](auto it)     { return it < 10; },
               fmap<u32, u32>([](auto it) { return it * 3;  },
               filter([](auto it)         { return it >= 2; },
                      v1))), expected1);
    // clang-format on

    kexpect_eq(find(4, v1), make_optional(3));
    kexpect_eq(find(4.0, v1), make_optional(3));
    Vec<String> v2 = {"one", "two", "three"};
    kexpect_eq(find("two", v2), make_optional(1));

    String input = R"(One
Two
Three
Four)";

    Vec<String> expected2 = {"Two", "Three"};
    kexpect_eq(
        filter([](auto it) { return re_search("T[wh]", it); }, lines(input)),
        expected2);

    expect_eq_helper(flag_value("--source-dir", {"--source-dir", "src/"}),
                     "src/");
    expect_helper(
        !flag_value("--destination-dir", {"--source-dir", "src/"}).has_value());
    expect_helper(!flag_value("src/", {"--source-dir", "src/"}).has_value());
    expect_helper(flag_value("Two", expected2).has_value());
}

auto test_color() {
    println(black("This text is black."));
    println(red("This text is red."));
    println(green("This text is green."));
    println(yellow("This text is yellow."));
    println(blue("This text is blue."));
    println(purple("This text is purple."));
    println(cyan("This text is cyan."));
    println(white("This text is white."));
    println(black_bold("This text is black bold."));
    println(red_bold("This text is red bold."));
    println(green_bold("This text is green bold."));
    println(yellow_bold("This text is yellow bold."));
    println(blue_bold("This text is blue bold."));
    println(purple_bold("This text is purple bold."));
    println(cyan_bold("This text is cyan bold."));
    println(white_bold("This text is white bold."));
    println(black_underline("This text is black underline."));
    println(red_underline("This text is red underline."));
    println(green_underline("This text is green underline."));
    println(yellow_underline("This text is yellow underline."));
    println(blue_underline("This text is blue underline."));
    println(purple_underline("This text is purple underline."));
    println(cyan_underline("This text is cyan underline."));
    println(white_underline("This text is white underline."));
    println(black_bg("This text is black bg."));
    println(red_bg("This text is red bg."));
    println(green_bg("This text is green bg."));
    println(yellow_bg("This text is yellow bg."));
    println(blue_bg("This text is blue bg."));
    println(purple_bg("This text is purple bg."));
    println(cyan_bg("This text is cyan bg."));
    println(white_bg("This text is white bg."));
}

auto test_other() {
    try {
        Option<i32> o2 = {};
        i32 _          = expect("This will throw.", o2);
        // We should not get to this line.
        kexpect(false);
    } catch (ExpectedOptionalValue e) {
        // Eating the exception we're expecting.
    }

    kexpect_eq(parse_i32("1234").value(), 1234);
    kexpect(!parse_i32("asdf").has_value());
    kexpect_eq(format("hello {} and {}!", "Alice", "Bob"),
               "hello Alice and Bob!"s);
    kexpect_eq(format("hello {}", 42), "hello 42");
}

/// TODO: add more to this
auto test_slice() {
    Vec<String> v1 = {"one", "two", "three", "four", "five"};

    Vec<String> actual   = Slice<String>{0, 3, v1}.to_vec();
    Vec<String> expected = {"one", "two", "three"};
    kexpect_eq(actual, expected);
    actual   = Slice<String>{2, 5, v1}.to_vec();
    expected = {"three", "four", "five"};
    kexpect_eq(actual, expected);

    Slice s1 = slice(2, 5, v1).value();
    Slice s2 = slice(2, 5, v1).value();

    kexpect_eq(s1, s2);
}

auto unhappy_test_koption() {

    // Comparisons
    KOption<i32> k1_4{4};
    KOption<i32> k2_4{4};
    KOption<i32> k3_5{5};
    KOption<i32> k4_null{};
    KOption<i32> k5_null{};

    kexpect(k1_4 == k1_4);
    kexpect(k1_4 == k2_4);
    kexpect(!(k1_4 == k3_5));
    kexpect(!(k4_null == k3_5));
    kexpect(k1_4 == 4);
    kexpect(4 == k1_4);
    kexpect(!(k1_4 == 5));
    kexpect(!(k4_null == 5));

    kexpect(k1_4 != k3_5);
    kexpect(k1_4 != k4_null);
    kexpect(k4_null != k2_4);
    kexpect(k1_4 != 5);
    kexpect(4 != k4_null);
    kexpect(k4_null != 2);

    kexpect(k1_4 < k3_5);
    kexpect(!(k1_4 < k4_null));
    kexpect(!(k4_null < k5_null));
    kexpect(k1_4 < 5);
    kexpect(!(4 < k4_null));
    kexpect(k4_null < 2);

    kexpect(k1_4 <= k3_5);
    kexpect(k1_4 <= k2_4);
    kexpect(!(k1_4 <= k4_null));
    kexpect(!(k4_null <= k5_null));
    kexpect(k1_4 <= 5);
    kexpect(!(4 <= k4_null));
    kexpect(k4_null <= 2);

    kexpect(k3_5 > k1_4);
    kexpect(k1_4 > k4_null);
    kexpect(!(k4_null > k1_4));
    kexpect(!(k4_null > k5_null));
    kexpect(!(k1_4 > 5));
    kexpect(4 > k4_null);
    kexpect(!(k4_null > 2));

    kexpect(k3_5 >= k1_4);
    kexpect(k1_4 >= k2_4);
    kexpect(k1_4 >= k4_null);
    kexpect(!(k4_null >= k1_4));
    kexpect(k4_null >= k5_null);
    kexpect(!(k1_4 >= 5));
    kexpect(4 >= k4_null);
    kexpect(!(k4_null >= 2));

    // Interface
    auto double_it                = [](i32 input) { return input * 2; };
    auto larger_than_5            = [](i32 input) { return input > 5; };
    auto double_it_with_threshold = [](i32 input) {
        return (input >= 4) ? KOption<i32>{input * 2} : KOption<i32>{};
    };
    auto larger_than_5_opt = [](i32 input) {
        return (input > 5) ? KOption<i32>{input} : KOption<i32>{};
    };

    KOption<i32> k6{4};
    kexpect_eq(k6, KOption{4});

    k6.reset();
    kexpect_eq(k6, KOption<i32>{});

    kexpect_eq(k6.value_or(8), 8);
    KOption<i32> k7 = k6.or_else([] { return KOption{8}; });
    kexpect_eq(k7.value(), 8);

    k6 = KOption<i32>{4};
    k7 = k6.transform(double_it);
    kexpect_eq(k7.value(), 8);
    k7 = k6.transform(larger_than_5);
    kexpect_eq(k7.value(), false);
    k7 = k6.or_else([] { return KOption{7}; });
    kexpect_eq(k7.value(), 4);
    kexpect_eq(k7.expect("There should be a value here."), 4);

    k7 = k6.and_then(double_it_with_threshold);
    kexpect_eq(k7.value(), 8);
    k7 = k6.and_then(larger_than_5_opt);
    kexpect(!k7.has_value());

    // Optional Conversions
    std::optional<i32> o1 = k6.to_optional();
    kexpect_eq(o1, make_optional(4));
    std::optional<i32> o2 = k7.to_optional();
    kexpect(!o2.has_value());
}

auto foo() -> KOption<i32> {
    return KOption<i32>{};
}

int main() {
    // clang-format off
    if (TEST_STRING)  { test_string();  }
    if (TEST_RESULT)  { test_result();  }
    if (TEST_VECTOR)  { test_vector();  }
    if (TEST_COLOR)   { test_color();   }
    if (TEST_SLICE)   { test_slice();   }
    if (TEST_KOPTION) { unhappy_test_koption(); }
    if (TEST_OTHER)   { test_other();   }
    // clang-format on

    // SCRATCH TESTING STARTS HERE
    return 0;
}
