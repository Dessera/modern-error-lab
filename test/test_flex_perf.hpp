/// @file test_flex_perf.hpp
/// @brief Flex performance test utils.

#pragma once

#include <chrono>
#include <exception>
#include <flex/flex.hpp>
#include <string_view>

/// @brief Error for zero division (expected version).
struct ExpectedDivideByZeroError {
    int dividend;
};

template <typename CharT> struct flex::Error<ExpectedDivideByZeroError, CharT> {
    constexpr static auto output(const ExpectedDivideByZeroError &err,
                                 OutputStream<CharT> &out)
        -> OutputStream<CharT> & {
        out << std::basic_string_view<CharT>("Divide number ") << err.dividend
            << std::basic_string_view<CharT>(" by zero");
        return out;
    }
};

/// @brief Error for arthmetic calculation.
using ExpectedArithmeticError = flex::ErrorGroup<ExpectedDivideByZeroError>;

/// @brief Error for zero division (exception version).
class ExceptionDivideByZeroError : public std::exception {
  private:
    int _dividend;
    mutable std::string _cache;

  public:
    explicit ExceptionDivideByZeroError(int dividend);

    [[nodiscard]] auto what() const noexcept -> const char * override;
};

/// @brief Performance test helper.
template <typename D = std::chrono::milliseconds,
          typename Clock = std::chrono::high_resolution_clock>
auto record(auto &&func, auto &&...args) -> decltype(auto) {
    using Ret = std::invoke_result_t<decltype(func), decltype(args)...>;

    auto start = Clock::now();
    if constexpr (std::is_same_v<Ret, void>) {
        std::forward<decltype(func)>(func)(
            std::forward<decltype(args)>(args)...);
        return std::make_tuple(
            std::chrono::duration_cast<D>(Clock::now() - start));
    } else {
        auto ret = std::forward<decltype(func)>(func)(
            std::forward<decltype(args)>(args)...);
        return std::make_tuple(
            std::chrono::duration_cast<D>(Clock::now() - start),
            std::move(ret));
    }
}

/// @brief Divide tester (expected version).
auto divide_expected(int lhs, int rhs)
    -> flex::Result<int, ExpectedArithmeticError>;

/// @brief Divide tester (exception version).
auto divide_exception(int lhs, int rhs) -> int;
