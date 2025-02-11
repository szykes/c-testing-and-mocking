# c-testing-and-mocking
A simple testing and mocking framework for AVR8 (or any other simple microcontollers) based C projects. Thus, this is not a `googletet` repilcation in C, this is a lightweight but still effective framework.

It supports:
- asserting on equity
- asserting on true
- marking function call(s) as uninterested, so they won't be evaulated
- skipping function parameter check
- returning and passing pointers
- traceable log in case of failed test case/step

Some example projects:
- [nixie-tube-clock](https://github.com/szykes/nixie-tube-clock)
- [smart-timed-plug](https://github.com/szykes/smart-timed-plug)

## Building static lib
```
cmake -S . -B cmake
```

```
cmake --build cmake --target c-testing-and-mocking
```

## Running test of framework
```
cmake -DC_TESTING_AND_MOCKING_TEST=True -S . -B cmake-test
```

```
cmake --build cmake-test --target c-testing-and-mocking-test
```

> [!NOTE]
> The test covers only happy cases.
