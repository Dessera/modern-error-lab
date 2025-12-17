#include <iostream>
#include <print>
#include <token/token.hpp>

auto main() -> int {
    auto tokenizer = token::Tokenizer(
        "Lorem ipsum dolor, sit amet consectetur adipisicing elit. Obcaecati "
        "voluptatum necessitatibus blanditiis quam recusandae quaerat quia "
        "quod temporibus itaque enim voluptas, laboriosam non laborum "
        "corrupti, vero nesciunt veniam suscipit illo?");

    while (true) {
        auto res = tokenizer.next();
        if (!res.has_value()) {
            std::println(std::cerr, "Error: {}", res.error());
            break;
        }

        std::println("Token: {:8} {}", token::name(*res), res->data);
        if (res->type == token::Token::Eoi) {
            break;
        }
    }
}
