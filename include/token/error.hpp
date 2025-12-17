/// @file error.hpp
/// @brief Tokenizer error types.

#pragma once

#include <cstddef>
#include <flex/flex.hpp>
#include <ios>
#include <string_view>

namespace token::error {

/// @brief Error for invalid character in input stream.
struct InvalidCharError {
    char chr;
    std::size_t pos;
};

/// @brief Error for unexpected end of input.
struct UnexpectedEoiError {
    std::size_t pos;
    std::size_t len;
};

} // namespace token::error

template <typename CharT>
struct flex::Error<token::error::InvalidCharError, CharT> {
    constexpr static auto output(const token::error::InvalidCharError &err,
                                 OutputStream<CharT> &out)
        -> OutputStream<CharT> & {
        out << std::basic_string_view<CharT>("Invalid character at ") << err.pos
            << std::basic_string_view<CharT>(", which value is ") << std::hex
            << static_cast<int>(err.chr);
        return out;
    }
};

template <typename CharT>
struct flex::Error<token::error::UnexpectedEoiError, CharT> {
    constexpr static auto output(const token::error::UnexpectedEoiError &err,
                                 OutputStream<CharT> &out)
        -> OutputStream<CharT> & {
        out << std::basic_string_view<CharT>("Unexpected end of line at ")
            << err.pos
            << std::basic_string_view<CharT>(", while the length of input is ")
            << err.len;
        return out;
    }
};

namespace token::error {

/// @brief Tokenizer error type.
using TokenError = flex::ErrorGroup<InvalidCharError, UnexpectedEoiError>;

/// @brief Tokenizer result type.
template <typename T> using TokenResult = flex::Result<T, TokenError>;

} // namespace token::error
