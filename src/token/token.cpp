#include "token/token.hpp"
#include "token/error.hpp"

#include <cctype>
#include <iterator>
#include <string_view>

namespace token {

auto name(const Token &token) -> std::string_view {
    return Token::NAMES.at(token.type);
}

Tokenizer::Tokenizer(std::string_view input)
    : _input(input), _pos(_input.begin()) {}

auto Tokenizer::next() -> error::TokenResult<Token> {
    _skip(isspace);

    if (_pos >= _input.end()) {
        return Token{.type = Token::Eoi, .data = ""};
    }

    if (std::iscntrl(*_pos) != 0) {
        return std::unexpected(
            error::InvalidCharError{.chr = *_pos, .pos = spos()});
    }

    if (std::isdigit(*_pos) != 0) {
        return _digest(Token::Number, isdigit);
    }

    if (std::isalpha(*_pos) != 0) {
        return _digest(Token::Word, isalpha);
    }

    if (std::ispunct(*_pos) != 0) {
        return _digest(Token::Punct, 1);
    }

    return std::unexpected(
        error::UnexpectedEoiError{.pos = spos(), .len = _input.size()});
}

auto Tokenizer::pos() const -> std::string_view::const_iterator { return _pos; }

auto Tokenizer::spos() const -> std::size_t {
    return std::distance(_input.begin(), _pos);
}

auto Tokenizer::_digest(Token::Type type, std::size_t len) -> Token {
    return _digest(type, [&](auto && /*chr*/) -> bool {
        if (len != 0) {
            --len;
            return true;
        }
        return false;
    });
}

} // namespace token
