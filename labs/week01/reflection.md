# Week 1 reflection

Write concise answers in your own words.

## 1. Source and executable

What is the difference between `src/hello.cpp` and `build/manual/hello` after compilation?

Answer: `src/hello.cpp` is a plain text file with C++ source code that I can read and edit. `build/manual/hello` is the compiled machine-code executable that g++ produced from it. Linux can run the executable, but not the .cpp file. `file` shows the first as "C++ source, ASCII text" and the second as an "ELF 64-bit executable". If I edit the source, I have to recompile before the executable changes.

## 2. Compiler warnings

What is the purpose of `-Wall -Wextra -Wpedantic`?

Answer: They turn on extra compiler warnings. `-Wall` enables the common ones, `-Wextra` adds more, and `-Wpedantic` warns when code uses non-standard compiler extensions. The program can still compile with warnings, but they point to likely mistakes (e.g. the unused variable in `warning.cpp`), so it is better to keep code warning-clean.

## 3. Value and reference parameters

What is the difference between these declarations?

```cpp
void f(std::vector<int> values);
void f(std::vector<int>& values);
```

Answer: The first takes the vector by value, so the function gets its own copy. Changes inside the function do not affect the caller's vector, and copying a big vector costs time and memory. The second takes it by reference, so `values` is just another name for the caller's vector: no copy is made and changes are visible in the caller. I saw this in `sequence.cpp`: with the by-value version of `add_to_all`, the vector in `main()` stayed the same.

## 4. Const reference

Why can this parameter form be useful?

```cpp
void print(const std::vector<int>& values);
```

Answer: It avoids copying the vector (fast, like a reference) but `const` stops the function from changing it. So it is the best choice for functions that only need to read the data, like printing or summing, and it also makes the intent clear to whoever reads the code.

## 5. Linux navigation

Which command shows the current working directory?

Answer: `pwd`

## 6. Git state

Which command shows modified files?

Answer: `git status` (and `git diff` shows the actual changes inside them).
