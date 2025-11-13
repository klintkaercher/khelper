# khelper

## GOAL
A library that provides the utility functionality of C++20 and beyond where possible in C++17, with a C++11 mode with a hand-rolled `std::optional` equivalent.
Primary focuses are `std::string`, `std::vector`, `std::optional`, and a custom `Result` that operates like `std::expected`.
If there's functionality given in Rust for similar data structures that is not in C++, then we will try to implement it here, where possible.
Opinionated alternatives for existing APIs are also present. `slice()` for `std::string`s, for example, uses the common `[start, end)` for indices
to provide an alternative to the `(start, length)` of `substr()`.

We'll use C++ nomenclature, Rust nomenclature, and custom nomenclature in that order where it exists.

The `test.cpp` file shows the style of code that can be written using khelper.

### USE CASES
1. Build with `./build.sh` to get the header and `.so` file. Install with `./install.sh`. Use like any other system-installed library.
2. Copy the `khelper.hpp` and `khelper.cpp` files into your project and add them into your build process.
3. Copy the specific functions into your source code. I'm not sure how strong this use case will be,
    so it's likely you might be expected to couple a few functions to get specific functionality.

## PORTABILITY
The code is written in a way where functions can easily be copied and pasted out and into other source files to eliminate dependencies or reduce a testing surface.
Functions may depend on a couple other functions in order for implementation.

We are not using derived classes, which might offer better method-call ergonomics, in support of portability. The end user is not expected to:
```cpp
void foo(std::string input) {
    khelper::String{input};
    if (input.contains("bar")) {
        // Do thing.
    }
}
```

Rather:
```cpp
void foo(std::string input) {
    if (khelper::find("bar", input)) {
        // Do thing.
    }
}
```

## SIMPLICITY
The code written here is not complicated when taken function by function.
The goal is to have so much of it that the sum of the parts is worth the price of admission.
We're not going to have dependencies. You can look at the `build.sh` and `install.sh` scripts to see how simple they are.
We're going to try to keep everything in two files, and we will organize the files to make navigation easy.

## PERFORMANCE
A functional style of read-only params and new outputs is preferred.
An `fmap` function for vectors that does not return a new vector is just `for (auto& elem : input) f(elem)`.

Performance should be pretty clean for these functions. It's unknown how much the templates really add to compile time at this time.

## FUNCTION SIGNATURES
Functions will be written in a "Subject Last" order with the "subject" of the function as the last parameter.
At the beginning, I started with the "subject up front" style that was reasonable for C-style method calls,
but realized it separated the not-obvious parameters from the function name, which became more and more
an issue depending on function nesting.

"Subject First" order:
```cpp
// Break a string into a vector of lines, filter out with a known predicate `p()`, and take the first three entries.
slice(filter(lines(v1), p, 0, 3);

```
"Subject Last" order:
```cpp
// Break a string into a vector of lines, filter out with a known predicate `p()`, and take the first three entries.
slice(0, 3, filter(p, lines(v1));
```
With a bit of formatting, you can get _almost_ a ranges-style pipeline, but in reverse.
```cpp
// Break a string into a vector of lines, filter out with a known predicate `p()`, and take the first three entries.
auto output = slice(0, 3,
              fmap(f,
              filter(p,
              lines(v1))));
```

## PLANNED FEATURES
- Assessment of missing functions.
- Investigation of potential helper functions for `std::map`.
- Color formatting for println.
- Create `expect` macros like `assert` and then make `assert` throw exceptions.
- Create `enumerate` for `std::vector`.


