# Coding Principles

## Code Quality & Robustness
1. **Multiple compilers** (e.g., gcc, clang, msvc) are run against the code, with as many (reasonable) warnings enabled and multiple optimization levels should be used (e.g., lowest level for debugging, speed-based and/or space-based max optimization for release)
2. **Multiple static analysis tools** (e.g., cppcheck, gcc's -fanalyzer, clang-tidy, clang static analyzer, coverity) are run against the code, configured as much as possible against MISRA C 2023 and CERT C 2016
3. Debug and test builds shall include **multiple sanitizers** to help check runtime bugs
4. **Memory profile tools** (e.g., valgrind) are run against the debug, test, and release builds to detect memory bugs
5. A list of _banned functions / code constructs are checked_ by home brewed tools
6. **Unit test** the code to verify that the program behaves correctly through most conceivable scenarios (see the unit testing principles below)
7. **Fuzz test** (e.g., [afl](https://lcamtuf.coredump.cx/afl/)) to catch the crazy edge cases that cause the program to misbehave
8. Use **asserts** on assumptions made at various points of execution. These asserts should abort the program on internal paradoxes.

## Unit Testing
1. 100% function and statement code coverage is **required**, _however, keep in mind sub-point 2 below
2. The code should be treated as a **black box** :black_large_square. Do **not** look into it as you write your tests! Focus on how the code _should behave_.
3. Strive for [MC/DC](https://en.wikipedia.org/wiki/Modified_condition/decision_coverage) coverage but 100% coverage here is not expected, because it tends to lead to violations of sub-point 2
4. Unit tests should be short and simple. If you find yourself re-setting up an environment before running the function-under-test, split it out to another test case.
5. Follow the [Four-Phase Test pattern](http://xunitpatterns.com/Four%20Phase%20Test.html).
6. Whenever practical, test a _range_ of values rather than a single value in a test case.
   - This may seem redundant but helps catch unexpected edge cases.
   - If the range of values causes this single test case to take more than a few seconds to run, use a smaller subset.

## Encapsulation
1. Don't expose the underlying implementation to the user. Use [opaque types](https://stackoverflow.com/questions/2301454/what-defines-an-opaque-type-in-c-and-when-are-they-necessary-and-or-useful).

## API
1. APIs should be orthogonal except when there are extremely common functionalities for which providing an extra non-orthogonal function would be significantly more convenient.
   - There doesn't seem to be a common definition for an "orthogonal API", so I'll explain what I mean here in the follow two points.
   - The API is orthogonal if its functions do not overlap in behavior. Another way to look at it is the API is _not_ orthogonal if the behavior of one function can actually be done by another function.
   - An example of non-orthogonal API functions might be an `InsertAt(idx, val)` and `InsertAtEnd(val)` when a `Length()` method exists, because you could technically do `InsertAtEnd()` using `InsertAt( Length() )` (within a 0-indexed scheme).
   - The above `InsertAtEnd` example is one of the non-orthogonal functions I actually do include within the [`vector` API](./vector/vector.h), simply for its clear convenience.