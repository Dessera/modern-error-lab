#include "test_flex_perf.hpp"

#include <cstddef>
#include <exception>
#include <expected>
#include <flex/flex.hpp>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <print>
#include <sstream>
#include <string>
#include <string_view>

constexpr static std::size_t DEFAULT_NLOOP = 10000;

// NOLINTNEXTLINE
auto main(int argc, char **argv) -> int {
    volatile std::size_t nloop = DEFAULT_NLOOP;
    if (argc >= 2) {
        if (std::string_view(argv[1]) == "help") {
            std::println("Usage: {} [<nloop>]", argv[0]);
            return 0;
        }

        try {
            nloop = std::stoll(argv[1]);
        } catch (const std::exception &err) {
            std::println(std::cerr, "Failed to get nloop in argv: {}",
                         err.what());
        }
    }

    auto out = std::ostringstream();
    auto [time_expected] = record(
        [](int nloop) -> void {
            while (nloop != 0) {
                auto res = divide_expected(nloop, 0);
                if (!res.has_value()) {
                    (void)res;
                }
                --nloop;
            }
        },
        nloop);
    auto [time_expected_io] = record(
        [](int nloop, std::ostringstream &out) -> void {
            while (nloop != 0) {
                auto res = divide_expected(nloop, 0);
                if (!res.has_value()) {
                    std::print(out, "{}", res.error());
                }
                --nloop;
            }
        },
        nloop, std::ref(out));
    auto [time_expected_nothrow] = record(
        [](int nloop) -> void {
            while (nloop != 0) {
                auto res = divide_expected(nloop, 1);
                --nloop;
            }
        },
        nloop);
    auto [time_exception] = record(
        [](int nloop) -> void {
            while (nloop != 0) {
                try {
                    auto res = divide_exception(nloop, 0);
                } catch (const ExceptionDivideByZeroError &err) {
                    (void)err;
                }
                --nloop;
            }
        },
        nloop);
    auto [time_exception_io] = record(
        [](int nloop, std::ostringstream &out) -> void {
            while (nloop != 0) {
                try {
                    auto res = divide_exception(nloop, 0);
                } catch (const ExceptionDivideByZeroError &err) {
                    std::print(out, "{}", err.what());
                }
                --nloop;
            }
        },
        nloop, std::ref(out));
    auto [time_exception_nothrow] = record(
        [](int nloop) -> void {
            while (nloop != 0) {
                try {
                    auto res = divide_exception(nloop, 1);
                } catch (const ExceptionDivideByZeroError &err) {
                    (void)err;
                }
                --nloop;
            }
        },
        nloop);
    auto [time_nothrow] = record(
        [](int nloop) -> void {
            while (nloop != 0) {
                auto res = divide_nothrow(nloop, 0);
                --nloop;
            }
        },
        nloop);

    std::println("Performance Test for Flex:");
    std::println("Nloop               : {}", static_cast<std::size_t>(nloop));
    std::println("Expected (No IO)    : {}", time_expected);
    std::println("Expected            : {}", time_expected_io);
    std::println("Expected (Nothrow)  : {}", time_expected_nothrow);
    std::println("Exception (No IO)   : {}", time_exception);
    std::println("Exception           : {}", time_exception_io);
    std::println("Exception (Nothrow) : {}", time_exception_nothrow);
    std::println("Nothrow             : {}", time_nothrow);
}

ExceptionDivideByZeroError::ExceptionDivideByZeroError(int dividend)
    : _dividend(dividend) {}

auto ExceptionDivideByZeroError::what() const noexcept -> const char * {
    _cache = std::format("Divide number {} by zero", _dividend);
    return _cache.c_str();
}

auto divide_expected(int lhs, int rhs)
    -> flex::Result<int, ExpectedArithmeticError> {
    if (rhs == 0) {
        return std::unexpected(ExpectedDivideByZeroError{.dividend = lhs});
    }

    return lhs / rhs;
}

auto divide_exception(int lhs, int rhs) -> int {
    if (rhs == 0) {
        throw ExceptionDivideByZeroError(lhs);
    }

    return lhs / rhs;
}

auto divide_nothrow(int lhs, int rhs) noexcept -> int {
    if (rhs == 0) {
        return lhs >= 0 ? std::numeric_limits<int>::max()
                        : std::numeric_limits<int>::min();
    }

    return lhs / rhs;
}
