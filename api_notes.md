# API Note
## C++
### std::string
Annoyingly lacks remove_prefix.

### std::string_view
| Function        | Notes |
|-----------------|-------|
| remove_prefix() | void means that you can't chain this :(.    |

### std::map

## Rust
### String
### str
remove_prefix()
| Function        | Notes |
|-----------------|-------|
| strip_prefix()  | This returns optional for checking if the provided prefix was actually there. |
| remove_prefix() | This always succeeds for sake of chaining. |
| strip_suffix()  | This returns optional for checking if the provided prefix was actually there. |
| remove_suffix() | This always succeeds for sake of chaining. |

### Functions to migrate
