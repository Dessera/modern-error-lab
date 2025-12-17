/// @file token.hpp
/// @brief Simple string tokenizer.

#pragma once

#include "token/error.hpp"
#include "token_export.h"

#include <cstddef>
#include <cstdint>
#include <flex/flex.hpp>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace token {

/// @brief Token definition.
struct Token {
    /// @brief Token type definition.
    enum Type : uint8_t {
        Word,   /// Single word.
        Number, /// Number.
        Punct,  /// Punctuation.
        Eoi,    /// End of input.
    };

    /// @brief Token type names.
    static inline const std::unordered_map<Type, std::string_view> NAMES = {
        {Word, "WORD"},
        {Number, "NUMBER"},
        {Punct, "PUNCT"},
        {Eoi, "EOI"},
    };

    Type type;
    std::string_view data;
};

/// @brief Get name of token.
TOKEN_LIB_EXPORT auto name(const Token &token) -> std::string_view;

/// @brief Simple tokenizer which split string into words and numbers.
class TOKEN_LIB_EXPORT Tokenizer {
  private:
    std::string_view _input;
    std::string_view::const_iterator _pos;

  public:
    explicit Tokenizer(std::string_view input);

    /// @brief Get next token.
    auto next() -> error::TokenResult<Token>;

    /// @brief Get the current position of the tokenizer.
    [[nodiscard]] auto pos() const -> std::string_view::const_iterator;

    /// @brief Get the current position of the tokenizer as an index.
    [[nodiscard]] auto spos() const -> std::size_t;

  private:
    template <typename Pred>
        requires std::is_invocable_v<Pred, char>
    void _skip(Pred &&pred) {
        while (_pos != _input.end() && std::forward<Pred>(pred)(*_pos)) {
            ++_pos;
        }
    }

    template <typename Pred>
    auto _digest(Token::Type type, Pred &&pred)
        requires std::is_invocable_v<Pred, char>
    {
        std::string_view::const_iterator start = _pos;
        _skip(std::forward<Pred>(pred));

        return Token{.type = type, .data = std::string_view(start, _pos)};
    }

    auto _digest(Token::Type type, std::size_t len) -> Token;
};

} // namespace token
