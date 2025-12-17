#include "main.hpp"

#include <expected>
#include <iostream>
#include <print>
#include <string>
#include <string_view>
#include <token/token.hpp>

constexpr static std::string_view DEFAULT_CONTENT =
    "Lorem ipsum dolor, sit amet consectetur adipisicing elit. Obcaecati "
    "voluptatum necessitatibus blanditiis quam recusandae quaerat quia "
    "quod temporibus itaque enim voluptas, laboriosam non laborum "
    "corrupti, vero nesciunt veniam suscipit illo?";

auto main(int argc, char **argv) -> int {
    auto content = std::string();
    if (argc <= 1) {
        content = DEFAULT_CONTENT;
    } else {
        for (int idx = 1; idx < argc; ++idx) {
            content += argv[idx];
        }
    }

    if (auto res = tokenize(content, std::cout); !res.has_value()) {
        std::println(std::cerr, "Error Occurred: {}", res.error());
        return 1;
    }

    return 0;
}

auto tokenize(std::string_view input, std::ostream &out) -> AppResult<void> {
    auto tokenizer = token::Tokenizer(input);

    while (true) {
        auto res = tokenizer.next();
        if (!res.has_value()) {
            return std::unexpected(res.error());
        }

        std::println(out, "Token: {:8} {}", token::name(*res), res->data);
        if (res->type == token::Token::Eoi) {
            break;
        }
    }

    return {};
}
