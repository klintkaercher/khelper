= TODO
+ Double check `<<` operators are using const &.
+ Maybe wrap `debug` in a macro for file name and line numbers?


= Roadmap to 0.1.0
+ Assert statements changed to expect statements and assert statements made that will throw exceptions.
  - DONE
+ Another pass on the parameters being const and ref correct.
+ slice() function properly produces a Slice instance.
  - DONE
+ KOption class should be equivalent in 90% of use cases for std::optional.
  + No `->` or `*` operators. No iterators. No swap, or emplace. 
  + DONE
    + operator bool
    + reset()
    + has_value()
    + value()
    + value_or()
    + or_else()
    + and_then()
    + transform()
    + `==`, `!=`, `<`, `<=`, `>`, and `>=` operators for other KOptions
  + TODO
    + `==`, `!=`, `<`, `<=`, `>`, and `>=` operators for raw values
