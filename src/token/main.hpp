/// @file main.hpp
/// @brief Tokenizer main utils.

#pragma once

#include <flex/flex.hpp>
#include <ostream>
#include <string_view>
#include <token/error.hpp>
#include <token/token.hpp>

/// @brief Application error type.
using AppError = flex::ErrorGroup<token::error::TokenError>;

/// @brief Application result type.
template <typename T> using AppResult = flex::Result<T, AppError>;

/// @brief Get tokenize summay for a string.
auto tokenize(std::string_view input, std::ostream &out) -> AppResult<void>;
