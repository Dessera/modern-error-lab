/// @file flex.hpp
/// @brief Flex: Simple error trait definition.

#pragma once

#include <concepts>
#include <expected>
#include <format>
#include <sstream>
#include <type_traits>
#include <utility>
#include <variant>

namespace flex {

namespace detail {

/// @brief Helper for inspecting if type parameter pack contains given type.
template <typename T, typename... Args> struct HasType {};

template <typename T, typename Arg, typename... Args>
struct HasType<T, Arg, Args...> {
    constexpr static bool VALUE =
        std::is_same_v<T, Arg> || HasType<T, Args...>::VALUE;
};

template <typename T> struct HasType<T> {
    constexpr static bool VALUE = false;
};

/// @brief Helper for inspecting if type parameter pack contains given type
/// (variable version).
template <typename T, typename... Args>
constexpr bool HasTypeV = HasType<T, Args...>::VALUE;

} // namespace detail

/// @brief Output stream for error.
template <typename CharT = char>
using OutputStream = std::basic_ostringstream<CharT>;

/// @brief Error trait.
template <typename Err, typename CharT> struct Error {};

/// @brief Check if type is Throwable.
template <typename Err, typename CharT = char>
concept Throwable = requires(Err &&err, OutputStream<CharT> out) {
    {
        Error<Err, CharT>::output(err, out)
    } -> std::same_as<OutputStream<CharT> &>;
};

template <typename... Eargs, typename CharT>
    requires(Throwable<Eargs> && ...)
struct Error<std::variant<Eargs...>, CharT> {
    constexpr static auto output(const std::variant<Eargs...> &err,
                                 OutputStream<CharT> &out)
        -> OutputStream<CharT> & {
        return std::visit(
            [&](auto &&err) -> OutputStream<CharT> & {
                return Error<std::remove_cvref_t<decltype(err)>, CharT>::output(
                    err, out);
            },
            err);
    }
};

/// @brief Error transform trait.
template <typename From, typename To> struct Transform {};

/// @brief Check if one error can be transformed into another.
template <typename From, typename To>
concept Transformable = requires(From &&from) {
    { Transform<From, To>::transform(std::move(from)) } -> std::same_as<To>;
};

template <typename Err> struct Transform<Err, Err> {
    constexpr static auto transform(Err &&from) -> Err {
        return std::move(from);
    }
};

template <typename From, typename... Tos>
    requires detail::HasTypeV<From, Tos...>
struct Transform<From, std::variant<Tos...>> {
    constexpr static auto transform(From &&from) -> std::variant<Tos...> {
        return std::move(from);
    }
};

/// @brief Helper for create a group of error.
template <typename... Eargs>
    requires(Throwable<Eargs> && ...)
using ErrorGroup = std::variant<Eargs...>;

/// @brief Helper for create a result type.
template <typename T, typename E>
    requires Throwable<E>
using Result = std::expected<T, E>;

} // namespace flex

template <typename Err, typename CharT>
    requires flex::Throwable<Err, CharT>
struct std::formatter<Err, CharT> {
    constexpr auto parse(auto &ctx) { return ctx.begin(); }

    constexpr auto format(const Err &err, auto &ctx) const {
        auto out = flex::OutputStream<CharT>();
        flex::Error<Err, CharT>::output(err, out);

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
