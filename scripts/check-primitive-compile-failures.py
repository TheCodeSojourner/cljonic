#!/usr/bin/env python3
"""Check collection primitive rejection diagnostics and admitted calls."""

from __future__ import annotations

import os
import argparse
import re
import subprocess

PRIMITIVES = {
    "take_while": {
        "diagnostic_anchor": "cljonic::take_while:",
        "diagnostic_cases": {
            "scalar": ("(void)cljonic::core::take_while([](int) noexcept { return true; }, 42);", "one cljonic source"),
            "array": ("(void)cljonic::core::take_while([](int) noexcept { return true; }, std::array<int, 2>{});", "one cljonic source"),
            "map-entry": ("(void)cljonic::core::take_while([](auto) noexcept { return true; }, cljonic::core::map_entry(1, 2));", "one cljonic source"),
            "throwing": ("(void)cljonic::core::take_while([](int) { return true; }, cljonic::core::vector(1));", "without throwing"),
            "throwing-copy": (
                "struct Predicate { Predicate() = default; Predicate(const Predicate&) noexcept(false) {} "
                "bool operator()(int) const noexcept { return true; } }; "
                "(void)cljonic::core::take_while(Predicate{}, cljonic::core::vector(1));",
                "without throwing",
            ),
            "not-bool": ("(void)cljonic::core::take_while([](int n) noexcept { return n; }, cljonic::core::vector(1));", "exactly bool"),
            "narrowing": ("(void)cljonic::core::take_while([](short) noexcept { return true; }, cljonic::core::vector(1L));", "safe"),
            "missing": ("(void)cljonic::core::take_while();", "a predicate and one cljonic source"),
            "extra": ("(void)cljonic::core::take_while([](int) noexcept { return true; }, cljonic::core::vector(1), cljonic::core::vector(2));", "one cljonic source"),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::take_while([](int) noexcept { return true; }, cljonic::core::vector(1, 2));",
            "captured": "(void)cljonic::core::take_while([limit = 3](int n) noexcept { return n < limit; }, cljonic::core::range(0, 4));",
            "nested": "const auto p = cljonic::core::take_while([](int n) noexcept { return n < 3; }, cljonic::core::repeat(2)); (void)cljonic::core::take_while([](auto) noexcept { return true; }, p);",
        },
    },
    "drop": {
        "diagnostic_anchor": "cljonic::drop:",
        "diagnostic_cases": {
            "scalar-source": ("(void)cljonic::core::drop(2, 42);", "exactly one cljonic source"),
            "map-entry-source": ("(void)cljonic::core::drop(2, cljonic::core::map_entry(1, 2));", "exactly one cljonic source"),
            "external-range": ("int values[]{1, 2}; (void)cljonic::core::drop(2, values);", "exactly one cljonic source"),
            "standard-array": ("(void)cljonic::core::drop(2, std::array<int, 2>{1, 2});", "exactly one cljonic source"),
            "standard-view": ("(void)cljonic::core::drop(2, std::ranges::iota_view{0, 3});", "exactly one cljonic source"),
            "bool-count": ("(void)cljonic::core::drop(true, cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "char-count": ("(void)cljonic::core::drop('a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "signed-char-count": ("(void)cljonic::core::drop(static_cast<signed char>(2), cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "unsigned-char-count": ("(void)cljonic::core::drop(static_cast<unsigned char>(2), cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "wide-char-count": ("(void)cljonic::core::drop(L'a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "utf8-char-count": ("(void)cljonic::core::drop(u8'a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "utf16-char-count": ("(void)cljonic::core::drop(u'a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "utf32-char-count": ("(void)cljonic::core::drop(U'a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "float-count": ("(void)cljonic::core::drop(1.5, cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "enum-count": ("enum Count { two = 2 }; (void)cljonic::core::drop(two, cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "converted-count": ("struct Count { operator int() const noexcept { return 2; } }; (void)cljonic::core::drop(Count{}, cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "missing-source": ("(void)cljonic::core::drop(2);", "exactly two arguments"),
            "missing-count": ("(void)cljonic::core::drop();", "exactly two arguments"),
            "extra-source": ("(void)cljonic::core::drop(2, cljonic::core::vector(1), cljonic::core::vector(2));", "exactly two arguments"),
            "swapped-arguments": ("(void)cljonic::core::drop(cljonic::core::vector(1, 2), 2);", "non-boolean, non-character integer"),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::drop(1, cljonic::core::vector(1, 2));",
            "map": "(void)cljonic::core::drop(1, cljonic::core::hash_map(1, 2));",
            "set": "(void)cljonic::core::drop(1, cljonic::core::hash_set(1, 2));",
            "queue": "(void)cljonic::core::drop(1, cljonic::core::queue(1, 2));",
            "string": '(void)cljonic::core::drop(1, cljonic::core::string<4>("ab"));',
            "range": "(void)cljonic::core::drop(1, cljonic::core::range(0, 3));",
            "repeat": "(void)cljonic::core::drop(1, cljonic::core::repeat(3, 7));",
            "cycle": "(void)cljonic::core::drop(1, cljonic::core::cycle(cljonic::core::vector(1, 2)));",
            "iterate": "(void)cljonic::core::drop(1, cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0));",
            "repeatedly": "(void)cljonic::core::drop(1, cljonic::core::repeatedly(3, []() noexcept { return 7; }));",
            "negative-count": "(void)cljonic::core::drop(-2, cljonic::core::vector(1, 2));",
            "unsigned-count": "(void)cljonic::core::drop(std::size_t{2}, cljonic::core::vector(1, 2));",
            "zero-capacity": "(void)cljonic::core::drop(1, cljonic::Vector<int, 0>{});",
        },
    },
    "take": {
        "diagnostic_anchor": "cljonic::take:",
        "diagnostic_cases": {
            "scalar-source": ("(void)cljonic::core::take(2, 42);", "exactly one cljonic source"),
            "map-entry-source": ("(void)cljonic::core::take(2, cljonic::core::map_entry(1, 2));", "exactly one cljonic source"),
            "external-range": ("int values[]{1, 2}; (void)cljonic::core::take(2, values);", "exactly one cljonic source"),
            "standard-array": ("(void)cljonic::core::take(2, std::array<int, 2>{1, 2});", "exactly one cljonic source"),
            "standard-view": ("(void)cljonic::core::take(2, std::ranges::iota_view{0, 3});", "exactly one cljonic source"),
            "bool-count": ("(void)cljonic::core::take(true, cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "char-count": ("(void)cljonic::core::take('a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "signed-char-count": ("(void)cljonic::core::take(static_cast<signed char>(2), cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "unsigned-char-count": ("(void)cljonic::core::take(static_cast<unsigned char>(2), cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "wide-char-count": ("(void)cljonic::core::take(L'a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "utf8-char-count": ("(void)cljonic::core::take(u8'a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "utf16-char-count": ("(void)cljonic::core::take(u'a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "utf32-char-count": ("(void)cljonic::core::take(U'a', cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "float-count": ("(void)cljonic::core::take(1.5, cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "enum-count": ("enum Count { two = 2 }; (void)cljonic::core::take(two, cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "converted-count": ("struct Count { operator int() const noexcept { return 2; } }; (void)cljonic::core::take(Count{}, cljonic::core::vector(1, 2));", "non-boolean, non-character integer"),
            "missing-source": ("(void)cljonic::core::take(2);", "exactly two arguments"),
            "missing-count": ("(void)cljonic::core::take();", "exactly two arguments"),
            "extra-source": ("(void)cljonic::core::take(2, cljonic::core::vector(1), cljonic::core::vector(2));", "exactly two arguments"),
            "swapped-arguments": ("(void)cljonic::core::take(cljonic::core::vector(1, 2), 2);", "non-boolean, non-character integer"),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::take(2, cljonic::core::vector(1, 2));",
            "map": "(void)cljonic::core::take(1, cljonic::core::hash_map(1, 2));",
            "set": "(void)cljonic::core::take(1, cljonic::core::hash_set(1, 2));",
            "queue": "(void)cljonic::core::take(1, cljonic::core::queue(1, 2));",
            "string": '(void)cljonic::core::take(1, cljonic::core::string<4>("ab"));',
            "range": "(void)cljonic::core::take(1, cljonic::core::range(0, 3));",
            "repeat": "(void)cljonic::core::take(1, cljonic::core::repeat(3, 7));",
            "cycle": "(void)cljonic::core::take(1, cljonic::core::cycle(cljonic::core::vector(1, 2)));",
            "iterate": "(void)cljonic::core::take(1, cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0));",
            "repeatedly": "(void)cljonic::core::take(1, cljonic::core::repeatedly(3, []() noexcept { return 7; }));",
            "negative-count": "(void)cljonic::core::take(-2, cljonic::core::vector(1, 2));",
            "unsigned-count": "(void)cljonic::core::take(std::size_t{2}, cljonic::core::vector(1, 2));",
            "zero-capacity": "(void)cljonic::core::take(1, cljonic::Vector<int, 0>{});",
        },
    },
    "get": {
        "diagnostic_anchor": "cljonic::get:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::get(42, 0);",
                "the first argument must be a Map, Set, Vector, or String",
            ),
            "unsupported-queue": (
                "(void)cljonic::core::get(cljonic::Queue<int, 4>{}, 0);",
                "the first argument must be a Map, Set, Vector, or String",
            ),
            "unsupported-producer": (
                "(void)cljonic::core::get(cljonic::Range<int>{0, 5}, 0U);",
                "the first argument must be a Map, Set, Vector, or String",
            ),
            "invalid-lookup-key": (
                "(void)cljonic::core::get(cljonic::Map<int, int, 4>{}, 1.5);",
                "the key must safely convert to the Map's key type or the Set's element type",
            ),
            "invalid-index": (
                "(void)cljonic::core::get(cljonic::Vector<int, 4>{}, 1.5);",
                "a Vector or String index must be a non-boolean, non-character integer type",
            ),
            "unsafe-fallback": (
                "(void)cljonic::core::get(cljonic::Vector<int, 4>{}, 0, 1.5);",
                "the fallback must safely convert to the collection's lookup result type",
            ),
        },
        "pass_cases": {
            "string-source": '(void)cljonic::core::into(cljonic::String<4>{}, cljonic::String<4>{"ab"});',
            "vector-char-source": "(void)cljonic::core::into(cljonic::String<4>{}, cljonic::Vector<char, 2>{'a', 'b'});",
            "vector-index": "(void)cljonic::core::get(cljonic::Vector<int, 4>{1, 2}, 0U);",
            "map-key": "(void)cljonic::core::get(cljonic::Map<int, long, 4>{}, short{1}, int{2});",
            "set-value": "(void)cljonic::core::get(cljonic::Set<int, 4>{1}, short{1});",
            "string-index": "(void)cljonic::core::get(cljonic::String<4>{\"ab\"}, 0U, 'x');",
        },
    },
    "count": {
        "diagnostic_anchor": "cljonic::count:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::count(42);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Cycle, Iterate, or Repeatedly value",
            ),
        },
        "pass_cases": {
            "vector-collection": "(void)cljonic::core::count(cljonic::Vector<int, 4>{1, 2});",
            "range-producer": "(void)cljonic::core::count(cljonic::Range<int>{0, 5});",
        },
    },
    "peek": {
        "diagnostic_anchor": "cljonic::peek:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::peek(42);",
                "the first argument must be a Vector or Queue",
            ),
            "unsupported-set": (
                "(void)cljonic::core::peek(cljonic::Set<int, 4>{1});",
                "the first argument must be a Vector or Queue",
            ),
            "unsupported-producer": (
                "(void)cljonic::core::peek(cljonic::Range<int>{0, 5});",
                "the first argument must be a Vector or Queue",
            ),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::peek(cljonic::Vector<int, 4>{1, 2});",
            "queue": "(void)cljonic::core::peek(cljonic::Queue<int, 4>{1, 2});",
            "empty-vector": "(void)cljonic::core::peek(cljonic::Vector<int, 4>{});",
            "empty-queue": "(void)cljonic::core::peek(cljonic::Queue<int, 4>{});",
            "zero-capacity-vector": "(void)cljonic::core::peek(cljonic::Vector<int, 0>{});",
            "zero-capacity-queue": "(void)cljonic::core::peek(cljonic::Queue<int, 0>{});",
        },
    },
    "pop": {
        "diagnostic_anchor": "cljonic::pop:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::pop(42);",
                "the argument must be a Vector or Queue collection; scalar values, producers, and other collection "
                "types are not supported",
            ),
            "unsupported-set": (
                "(void)cljonic::core::pop(cljonic::Set<int, 4>{1});",
                "the argument must be a Vector or Queue collection; scalar values, producers, and other collection "
                "types are not supported",
            ),
            "unsupported-producer": (
                "(void)cljonic::core::pop(cljonic::Range<int>{0, 5});",
                "the argument must be a Vector or Queue collection; scalar values, producers, and other collection "
                "types are not supported",
            ),
        },
        "pass_cases": {
            "queue": "(void)cljonic::core::pop(cljonic::Queue<int, 4>{1, 2});",
        },
    },
    "rest": {
        "diagnostic_anchor": "cljonic::rest:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::rest(42);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
            "unsupported-map-entry": (
                "(void)cljonic::core::rest(cljonic::MapEntry<int, int>{1, 2});",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
            "unsupported-external-range": (
                "int values[]{1, 2}; (void)cljonic::core::rest(values);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::rest(cljonic::Vector<int, 4>{1, 2});",
            "map": "(void)cljonic::core::rest(cljonic::Map<int, int, 4>{cljonic::MapEntry{1, 2}});",
            "set": "(void)cljonic::core::rest(cljonic::Set<int, 4>{1, 2});",
            "queue": "(void)cljonic::core::rest(cljonic::Queue<int, 4>{1, 2});",
            "string": "(void)cljonic::core::rest(cljonic::String<4>{\"AB\"});",
            "empty-vector": "(void)cljonic::core::rest(cljonic::Vector<int, 4>{});",
            "zero-capacity-vector": "(void)cljonic::core::rest(cljonic::Vector<int, 0>{});",
            "range": "(void)cljonic::core::rest(cljonic::Range<int>{0, 5});",
            "repeat": "(void)cljonic::core::rest(cljonic::core::repeat(3, 7));",
            "cycle": "(void)cljonic::core::rest(cljonic::core::cycle(cljonic::Vector<int, 3>{1, 2, 3}));",
            "iterate": "(void)cljonic::core::rest(cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0));",
            "repeatedly": "(void)cljonic::core::rest(cljonic::core::repeatedly(3, []() noexcept { return 7; }));",
        },
    },
    "seq": {
        "diagnostic_anchor": "cljonic::seq:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::seq(42);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Cycle, Iterate, or Repeatedly value",
            ),
            "unsupported-map-entry": (
                "(void)cljonic::core::seq(cljonic::MapEntry<int, int>{1, 2});",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Cycle, Iterate, or Repeatedly value",
            ),
            "unsupported-external-range": (
                "int values[]{1, 2}; (void)cljonic::core::seq(values);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Cycle, Iterate, or Repeatedly value",
            ),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::seq(cljonic::Vector<int, 4>{1, 2});",
            "map": "(void)cljonic::core::seq(cljonic::Map<int, int, 4>{cljonic::MapEntry{1, 2}});",
            "set": "(void)cljonic::core::seq(cljonic::Set<int, 4>{1, 2});",
            "queue": "(void)cljonic::core::seq(cljonic::Queue<int, 4>{1, 2});",
            "string": "(void)cljonic::core::seq(cljonic::String<4>{\"AB\"});",
            "empty-vector": "(void)cljonic::core::seq(cljonic::Vector<int, 4>{});",
            "zero-capacity-vector": "(void)cljonic::core::seq(cljonic::Vector<int, 0>{});",
            "range": "(void)cljonic::core::seq(cljonic::Range<int>{0, 5});",
            "repeat": "(void)cljonic::core::seq(cljonic::core::repeat(3, 7));",
            "cycle": "(void)cljonic::core::seq(cljonic::core::cycle(cljonic::Vector<int, 3>{1, 2, 3}));",
            "iterate": "(void)cljonic::core::seq(cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0));",
            "repeatedly": "(void)cljonic::core::seq(cljonic::core::repeatedly(3, []() noexcept { return 7; }));",
        },
    },
    "has_next": {
        "diagnostic_anchor": "cljonic::has_next:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::has_next(42);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
            "unsupported-map-entry": (
                "(void)cljonic::core::has_next(cljonic::MapEntry<int, int>{1, 2});",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
            "unsupported-external-range": (
                "int values[]{1, 2}; (void)cljonic::core::has_next(values);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::has_next(cljonic::Vector<int, 4>{1, 2});",
            "map": "(void)cljonic::core::has_next(cljonic::Map<int, int, 4>{cljonic::MapEntry{1, 2}});",
            "set": "(void)cljonic::core::has_next(cljonic::Set<int, 4>{1, 2});",
            "queue": "(void)cljonic::core::has_next(cljonic::Queue<int, 4>{1, 2});",
            "string": "(void)cljonic::core::has_next(cljonic::String<4>{\"AB\"});",
            "empty-vector": "(void)cljonic::core::has_next(cljonic::Vector<int, 4>{});",
            "zero-capacity-vector": "(void)cljonic::core::has_next(cljonic::Vector<int, 0>{});",
            "range": "(void)cljonic::core::has_next(cljonic::Range<int>{0, 5});",
            "repeat": "(void)cljonic::core::has_next(cljonic::core::repeat(3, 7));",
            "cycle": "(void)cljonic::core::has_next(cljonic::core::cycle(cljonic::Vector<int, 3>{1, 2, 3}));",
            "iterate": "(void)cljonic::core::has_next(cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0));",
            "repeatedly": "(void)cljonic::core::has_next(cljonic::core::repeatedly(3, []() noexcept { return 7; }));",
            "take-while": "(void)cljonic::core::has_next(cljonic::core::take_while([](int value) noexcept { return value < 3; }, cljonic::Vector<int, 3>{1, 2, 3}));",
            "drop-while": "(void)cljonic::core::has_next(cljonic::core::drop_while([](int value) noexcept { return value < 3; }, cljonic::Vector<int, 3>{1, 2, 3}));",
        },
    },
    "has_rest": {
        "diagnostic_anchor": "cljonic::has_rest:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::has_rest(42);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
            "unsupported-map-entry": (
                "(void)cljonic::core::has_rest(cljonic::MapEntry<int, int>{1, 2});",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
            "unsupported-external-range": (
                "int values[]{1, 2}; (void)cljonic::core::has_rest(values);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle value",
            ),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::has_rest(cljonic::Vector<int, 4>{1, 2});",
            "map": "(void)cljonic::core::has_rest(cljonic::Map<int, int, 4>{cljonic::MapEntry{1, 2}});",
            "set": "(void)cljonic::core::has_rest(cljonic::Set<int, 4>{1, 2});",
            "queue": "(void)cljonic::core::has_rest(cljonic::Queue<int, 4>{1, 2});",
            "string": "(void)cljonic::core::has_rest(cljonic::String<4>{\"AB\"});",
            "range": "(void)cljonic::core::has_rest(cljonic::Range<int>{0, 5});",
            "repeat": "(void)cljonic::core::has_rest(cljonic::core::repeat(3, 7));",
            "cycle": "(void)cljonic::core::has_rest(cljonic::core::cycle(cljonic::Vector<int, 3>{1, 2, 3}));",
            "iterate": "(void)cljonic::core::has_rest(cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0));",
            "repeatedly": "(void)cljonic::core::has_rest(cljonic::core::repeatedly(3, []() noexcept { return 7; }));",
        },
    },
    "is_empty": {
        "diagnostic_anchor": "cljonic::is_empty:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::is_empty(42);",
                "value must be a collection or producer",
            ),
            "unsupported-external-range": (
                "int values[]{1, 2}; (void)cljonic::core::is_empty(values);",
                "value must be a collection or producer",
            ),
            "unsupported-count-shaped-type": (
                "struct CountShaped { auto count() const noexcept -> std::size_t { return 0; } }; "
                "(void)cljonic::core::is_empty(CountShaped{});",
                "value must be a collection or producer",
            ),
            "unsupported-map-entry": (
                "(void)cljonic::core::is_empty(cljonic::MapEntry<int, int>{1, 2});",
                "value must be a collection or producer",
            ),
        },
        "pass_cases": {
            "vector-empty": "(void)cljonic::core::is_empty(cljonic::Vector<int, 4>{});",
            "map-empty": "(void)cljonic::core::is_empty(cljonic::Map<int, int, 4>{});",
            "set-empty": "(void)cljonic::core::is_empty(cljonic::Set<int, 4>{});",
            "queue-empty": "(void)cljonic::core::is_empty(cljonic::Queue<int, 4>{});",
            "string-empty": "(void)cljonic::core::is_empty(cljonic::String<4>{});",
            "empty-range-producer": "(void)cljonic::core::is_empty(cljonic::Range<int>{0, 0});",
            "nonempty-range-producer": "(void)cljonic::core::is_empty(cljonic::Range<int>{0, 5});",
            "repeat-producer": "(void)cljonic::core::is_empty(cljonic::Repeat{7});",
            "cycle-producer": "(void)cljonic::core::is_empty(cljonic::core::cycle(cljonic::Vector<int, 1>{1}));",
            "iterate-producer": (
                "(void)cljonic::core::is_empty(cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0));"
            ),
            "repeatedly-producer": "(void)cljonic::core::is_empty(cljonic::core::repeatedly([]() noexcept { return 7; }));",
        },
    },
    "empty": {
        "diagnostic_anchor": "cljonic::empty:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::empty(42);",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-producer": (
                "(void)cljonic::core::empty(cljonic::Range<int>{0, 5});",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::empty(cljonic::Vector<int, 4>{});",
            "map": "(void)cljonic::core::empty(cljonic::Map<int, int, 4>{});",
            "set": "(void)cljonic::core::empty(cljonic::Set<int, 4>{});",
            "queue": "(void)cljonic::core::empty(cljonic::Queue<int, 4>{});",
            "string": "(void)cljonic::core::empty(cljonic::String<4>{});",
        },
    },
    "not_empty": {
        "diagnostic_anchor": "cljonic::not_empty:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::not_empty(42);",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-producer": (
                "(void)cljonic::core::not_empty(cljonic::Range<int>{0, 5});",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-repeat": (
                "(void)cljonic::core::not_empty(cljonic::Repeat{1});",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-cycle": (
                "(void)cljonic::core::not_empty(cljonic::core::cycle(cljonic::Vector{1}));",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-iterate": (
                "(void)cljonic::core::not_empty(cljonic::core::iterate([](int x) noexcept { return x; }, 0));",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-repeatedly": (
                "(void)cljonic::core::not_empty(cljonic::core::repeatedly([]() noexcept { return 1; }));",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-map-entry": (
                "(void)cljonic::core::not_empty(cljonic::MapEntry{1, 2});",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-external-range": (
                "int values[]{1, 2}; (void)cljonic::core::not_empty(values);",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
            "unsupported-count-shaped-type": (
                "struct CountShaped { auto count() const noexcept -> std::size_t { return 0; } }; "
                "(void)cljonic::core::not_empty(CountShaped{});",
                "expected a Vector, Map, Set, Queue, or String value",
            ),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::not_empty(cljonic::Vector{1, 2});",
            "map": "(void)cljonic::core::not_empty(cljonic::Map{cljonic::MapEntry{1, 2}});",
            "set": "(void)cljonic::core::not_empty(cljonic::Set{1, 2});",
            "queue": "(void)cljonic::core::not_empty(cljonic::Queue{1, 2});",
            "string": "(void)cljonic::core::not_empty(cljonic::String{\"ab\"});",
            "empty": "(void)cljonic::core::not_empty(cljonic::Vector<int, 4>{});",
            "zero-capacity": "(void)cljonic::core::not_empty(cljonic::Vector<int, 0>{});",
            "nested": "(void)cljonic::core::not_empty(cljonic::Vector{cljonic::Vector{1, 2}});",
        },
    },
    "first": {
        "diagnostic_anchor": "cljonic::first:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::first(42);",
                "expected a cljonic collection, producer, or MapEntry value",
            ),
            "external-array": (
                "int values[]{1, 2}; (void)cljonic::core::first(values);",
                "expected a cljonic collection, producer, or MapEntry value",
            ),
            "standard-array": (
                "(void)cljonic::core::first(std::array<int, 2>{});",
                "expected a cljonic collection, producer, or MapEntry value",
            ),
            "standard-view": (
                "(void)cljonic::core::first(std::ranges::iota_view{0, 3});",
                "expected a cljonic collection, producer, or MapEntry value",
            ),
        },
        "pass_cases": {
            "range": "static_assert(cljonic::core::first(cljonic::core::range(7, 9)) == 7);",
            "repeat": "static_assert(cljonic::core::first(cljonic::core::repeat(7)) == 7);",
            "cycle": "static_assert(cljonic::core::first(cljonic::core::cycle(cljonic::core::vector(7))) == 7);",
            "iterate": "static_assert(cljonic::core::first(cljonic::core::iterate([](int n) noexcept { return n + 1; }, 7)) == 7);",
            "repeatedly": "static_assert(cljonic::core::first(cljonic::core::repeatedly([]() noexcept { return 7; })) == 7);",
            "take-while": "static_assert(cljonic::core::first(cljonic::core::take_while([](int) noexcept { return true; }, cljonic::core::vector(7))) == 7);",
            "drop-while": "static_assert(cljonic::core::first(cljonic::core::drop_while([](int) noexcept { return false; }, cljonic::core::vector(7))) == 7);",
            "cycle-adapter": "static_assert(cljonic::core::first(cljonic::core::cycle(cljonic::core::take_while([](int) noexcept { return true; }, cljonic::core::vector(7)))) == 7);",
            "vector": "(void)cljonic::core::first(cljonic::Vector<int, 4>{10, 20});",
            "map": "(void)cljonic::core::first(cljonic::Map<int, int, 4>{cljonic::MapEntry<int, int>{1, 100}});",
            "set": "(void)cljonic::core::first(cljonic::Set<int, 4>{7});",
            "queue": "(void)cljonic::core::first(cljonic::Queue<int, 4>{5, 6});",
            "string": "(void)cljonic::core::first(cljonic::String<4>{\"AB\"});",
            "map-entry": "(void)cljonic::core::first(cljonic::MapEntry<int, int>{1, 100});",
            "materialized-producer": (
                "static_assert(cljonic::core::first(cljonic::core::into(cljonic::core::vector<int, 1>(), "
                "cljonic::core::take_while([](int) noexcept { return true; }, cljonic::core::vector(1, 4)))) == 1);"
            ),
        },
    },
    "next": {
        "diagnostic_anchor": "cljonic::next:",
        "diagnostic_cases": {
            "unsupported-scalar": (
                "(void)cljonic::core::next(42);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle "
                "value",
            ),
            "unsupported-map-entry": (
                "(void)cljonic::core::next(cljonic::MapEntry<int, int>{1, 100});",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle "
                "value",
            ),
            "unsupported-external-range": (
                "int values[]{1, 2}; (void)cljonic::core::next(values);",
                "expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, TakeWhile, DropWhile, or supported Cycle "
                "value",
            ),
        },
        "pass_cases": {
            "vector": "(void)cljonic::core::next(cljonic::Vector<int, 4>{10, 20});",
            "map": "(void)cljonic::core::next(cljonic::Map<int, int, 4>{cljonic::MapEntry<int, int>{1, 100}});",
            "set": "(void)cljonic::core::next(cljonic::Set<int, 4>{7, 8});",
            "queue": "(void)cljonic::core::next(cljonic::Queue<int, 4>{10, 20});",
            "string": '(void)cljonic::core::next(cljonic::String<4>{"ab"});',
            "finite-repeat": "(void)cljonic::core::next(cljonic::Repeat{3U, 7});",
            "unbounded-repeat": "(void)cljonic::core::next(cljonic::Repeat{7});",
            "iterate": "(void)cljonic::core::next(cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0));",
            "finite-repeatedly": "(void)cljonic::core::next(cljonic::core::repeatedly(3U, []() noexcept { return 7; }));",
            "unbounded-repeatedly": "(void)cljonic::core::next(cljonic::core::repeatedly([]() noexcept { return 7; }));",
            "finite-range": "(void)cljonic::core::next(cljonic::Range<int>{0, 5});",
            "unbounded-range": "(void)cljonic::core::next(cljonic::Range<int>{0, 5, 0});",
            "finite-cycle": "(void)cljonic::core::next(cljonic::core::cycle(cljonic::Vector<int, 3>{1, 2, 3}));",
            "iterate-cycle": (
                "(void)cljonic::core::next(cljonic::core::cycle(cljonic::core::iterate([](int value) noexcept { return value + 1; }, 0)));"
            ),
            "unbounded-cycle": (
                "(void)cljonic::core::next(cljonic::core::cycle(cljonic::Range<int>{0, 0, 0}));"
            ),
            "empty": "(void)cljonic::core::next(cljonic::Vector<int, 4>{});",
            "zero-capacity": "(void)cljonic::core::next(cljonic::Vector<int, 0>{});",
        },
    },
    "fits_into": {
        "diagnostic_anchor": "cljonic::fits_into:",
        "diagnostic_cases": {
            "unsupported-scalar-source": (
                "(void)cljonic::core::fits_into(cljonic::Vector<int, 4>{}, 42);",
                "expected a Vector, Set, Map, Queue, or String destination and a compatible cljonic collection or producer source",
            ),
            "unsupported-destination": (
                "(void)cljonic::core::fits_into(42, cljonic::Range<int>{0, 5});",
                "expected a Vector, Set, Map, Queue, or String destination and a compatible cljonic collection or producer source",
            ),
            "lossy-source-element": (
                "(void)cljonic::core::fits_into(cljonic::Vector<short, 4>{}, cljonic::Range<int>{0, 5});",
                "expected a Vector, Set, Map, Queue, or String destination and a compatible cljonic collection or producer source",
            ),
        },
        "pass_cases": {
            "finite-range": "(void)cljonic::core::fits_into(cljonic::Vector<int, 8>{}, cljonic::Range<int>{0, 5});",
            "unbounded-range": "(void)cljonic::core::fits_into(cljonic::Vector<int, 8>{}, cljonic::Range<int>{0, 5, 0});",
            "vector-source": "(void)cljonic::core::fits_into(cljonic::Vector<int, 8>{}, cljonic::Vector<int, 4>{1, 2});",
            "populated-destination": "(void)cljonic::core::fits_into(cljonic::Vector<int, 8>{1, 2}, cljonic::Range<int>{0, 5});",
        },
    },
    "into": {
        "diagnostic_anchor": "cljonic::into:",
        "diagnostic_cases": {
            "unsupported-scalar-source": (
                "(void)cljonic::core::into(cljonic::Vector<int, 4>{}, 42);",
                "expected a Vector, Set, Map, Queue, or String destination and a compatible cljonic collection or producer source",
            ),
            "unsupported-destination": (
                "(void)cljonic::core::into(42, cljonic::Range<int>{0, 5});",
                "expected a Vector, Set, Map, Queue, or String destination and a compatible cljonic collection or producer source",
            ),
            "lossy-source-element": (
                "(void)cljonic::core::into(cljonic::Vector<short, 4>{}, cljonic::Range<int>{0, 5});",
                "expected a Vector, Set, Map, Queue, or String destination and a compatible cljonic collection or producer source",
            ),
        },
        "pass_cases": {
            "finite-range": "(void)cljonic::core::into(cljonic::Vector<int, 8>{}, cljonic::Range<int>{0, 5});",
            "unbounded-range": "(void)cljonic::core::into(cljonic::Vector<int, 8>{}, cljonic::Range<int>{0, 5, 0});",
            "vector-source": "(void)cljonic::core::into(cljonic::Vector<int, 8>{}, cljonic::Vector<int, 4>{1, 2});",
            "populated-destination": "(void)cljonic::core::into(cljonic::Vector<int, 8>{1, 2}, cljonic::Range<int>{0, 5});",
        },
    },
    "dissoc": {
        "diagnostic_anchor": "cljonic::dissoc:",
        "diagnostic_cases": {
            "non-map-zero-keys": (
                "(void)cljonic::core::dissoc(cljonic::Set<int, 4>{});",
                "collection must be a Map; only Maps support dissoc",
            ),
            "non-map-with-key": (
                "(void)cljonic::core::dissoc(cljonic::Set<int, 4>{}, 1);",
                "first argument must be a Map; only Maps support dissoc",
            ),
            "non-map-multiple-keys": (
                "(void)cljonic::core::dissoc(cljonic::Set<int, 4>{}, 1, 2);",
                "first argument must be a Map; only Maps support dissoc",
            ),
            "non-convertible-key": (
                "struct DissocNotConvertible {}; (void)cljonic::core::dissoc(cljonic::Map<int, int, 4>{}, DissocNotConvertible{});",
                "each key must convert safely to the Map's key type",
            ),
            "non-convertible-trailing-key": (
                "struct DissocNotConvertible {}; (void)cljonic::core::dissoc(cljonic::Map<int, int, 4>{}, 1, DissocNotConvertible{});",
                "each key must convert safely to the Map's key type",
            ),
        },
        "pass_cases": {
            "zero-keys": "(void)cljonic::core::dissoc(cljonic::Map<int, int, 4>{});",
            "present-map-key": "(void)cljonic::core::dissoc(cljonic::Map{cljonic::MapEntry{1, 10}}, 1);",
            "absent-map-key": "(void)cljonic::core::dissoc(cljonic::Map<int, int, 4>{}, 1);",
            "multiple-keys": "(void)cljonic::core::dissoc(cljonic::Map{cljonic::MapEntry{1, 10}, cljonic::MapEntry{2, 20}}, 1, short{2});",
        },
    },
    "disj": {
        "diagnostic_anchor": "cljonic::disj:",
        "diagnostic_cases": {
            "non-set-collection": (
                "(void)cljonic::core::disj(cljonic::Vector<int, 4>{}, 1);",
                "first argument must be a Set",
            ),
            "lossy-value": (
                "(void)cljonic::core::disj(cljonic::Set<short, 4>{}, 1);",
                "must convert safely to the set's element type",
            ),
        },
        "pass_cases": {
            "widening-value": "(void)cljonic::core::disj(cljonic::Set<int, 4>{}, short{1});",
            "zero-values": "(void)cljonic::core::disj(cljonic::Set<int, 4>{1});",
            "one-value": "(void)cljonic::core::disj(cljonic::Set<int, 4>{1}, 1);",
            "multiple-values": "(void)cljonic::core::disj(cljonic::Set<int, 4>{1, 2}, 1, 2);",
            "repeated-values": "(void)cljonic::core::disj(cljonic::Set<int, 4>{1, 2}, 1, 2, 1);",
        },
    },
    "contains": {
        "diagnostic_anchor": "cljonic::contains:",
        "diagnostic_cases": {
            "unsupported-range": (
                "(void)cljonic::core::contains(cljonic::Range<int>{0, 5}, 1U);",
                "first argument must be a Map, Set, Vector, or String",
            ),
            "unsupported-queue": (
                "(void)cljonic::core::contains(cljonic::Queue<int, 4>{}, 1);",
                "first argument must be a Map, Set, Vector, or String",
            ),
            "map-lossy-key": (
                "(void)cljonic::core::contains(cljonic::Map<short, int, 4>{}, 1);",
                "must convert safely to the map's key type or the set's element type",
            ),
            "set-lossy-element": (
                "(void)cljonic::core::contains(cljonic::Set<short, 4>{}, 1);",
                "must convert safely to the map's key type or the set's element type",
            ),
            "vector-non-integral-index": (
                "(void)cljonic::core::contains(cljonic::Vector<int, 4>{}, 1.5);",
                "index must be a non-boolean, non-character integer type",
            ),
            "string-non-integral-index": (
                "(void)cljonic::core::contains(cljonic::String<4>{}, 1.5);",
                "index must be a non-boolean, non-character integer type",
            ),
        },
        "pass_cases": {
            "map-declared-key": "(void)cljonic::core::contains(cljonic::Map<int, int, 4>{}, 1);",
            "map-widening-key": "(void)cljonic::core::contains(cljonic::Map<int, int, 4>{}, short{1});",
            "set-widening-value": "(void)cljonic::core::contains(cljonic::Set<int, 4>{}, short{1});",
            "set-declared-element": "(void)cljonic::core::contains(cljonic::Set<int, 4>{}, 1);",
            "vector-integral-index": "(void)cljonic::core::contains(cljonic::Vector<int, 4>{}, -1);",
            "string-integral-index": "(void)cljonic::core::contains(cljonic::String<4>{}, 1U);",
        },
    },
}

PRIMITIVES["drop_while"] = {
    "diagnostic_anchor": "cljonic::drop_while:",
    "diagnostic_cases": {
        name: (body.replace("take_while", "drop_while"), message)
        for name, (body, message) in PRIMITIVES["take_while"]["diagnostic_cases"].items()
    },
    "pass_cases": {
        name: body.replace("take_while", "drop_while")
        for name, body in PRIMITIVES["take_while"]["pass_cases"].items()
    },
}

for adapter in ("take_while", "drop_while"):
    statement = f"const auto p = cljonic::core::{adapter}([](int) noexcept {{ return true; }}, cljonic::core::vector(1));"
    for api in ("has_next", "has_rest"):
        PRIMITIVES[api]["pass_cases"][f"{adapter}-producer"] = statement + f" (void)cljonic::core::{api}(p);"
        PRIMITIVES[api]["pass_cases"][f"{adapter}-cycle-producer"] = (
            statement + f" (void)cljonic::core::{api}(cljonic::core::cycle(p));"
        )
    for tail in ("next", "rest"):
        PRIMITIVES[tail]["pass_cases"][f"{adapter}-tail"] = (
            f"const auto p = cljonic::core::{adapter}([limit = 3](int n) noexcept {{ return n < limit; }}, "
            "cljonic::core::vector(1, 2, 4, 1));"
            f" static_assert(std::same_as<decltype(cljonic::core::{tail}(p)), std::remove_cv_t<decltype(p)>>);"
            f" (void)cljonic::core::{tail}(cljonic::core::cycle(cljonic::core::cycle(p)));"
        )

for operation in ("into", "fits_into"):
    materialization = PRIMITIVES[operation]
    for label, statement in {
        "producer-destination": f"(void)cljonic::core::{operation}(cljonic::Range<int>{{0, 2}}, cljonic::Range<int>{{0, 2}});",
        "external-source": f"int values[]{{1, 2}}; (void)cljonic::core::{operation}(cljonic::Vector<int, 4>{{}}, values);",
        "lossy-set-source": f"(void)cljonic::core::{operation}(cljonic::Set<short, 4>{{}}, cljonic::Range<int>{{0, 2}});",
        "lossy-queue-source": f"(void)cljonic::core::{operation}(cljonic::Queue<short, 4>{{}}, cljonic::Range<int>{{0, 2}});",
        "map-non-entry-source": (
            f"(void)cljonic::core::{operation}(cljonic::Map<int, int, 4>{{}}, cljonic::Vector<int, 4>{{1, 2}});"
        ),
        "map-mismatched-entry-source": (
            f"(void)cljonic::core::{operation}(cljonic::Map<long, int, 4>{{}}, "
            "cljonic::Vector<cljonic::MapEntry<int, int>, 1>{cljonic::MapEntry{1, 2}});"
        ),
    }.items():
        materialization["diagnostic_cases"][label] = (
            statement,
            "expected a Vector, Set, Map, Queue, or String destination and a compatible cljonic collection or producer source",
        )
    for label, statement in {
        "set-destination": f"(void)cljonic::core::{operation}(cljonic::Set<int, 4>{{}}, cljonic::Range<int>{{0, 2}});",
        "queue-destination": f"(void)cljonic::core::{operation}(cljonic::Queue<int, 4>{{}}, cljonic::Range<int>{{0, 2}});",
        "map-destination": (
            f"(void)cljonic::core::{operation}(cljonic::Map<int, int, 4>{{}}, "
            "cljonic::Map<int, int, 4>{cljonic::MapEntry{1, 2}});"
        ),
        "map-entry-producer": (
            f"(void)cljonic::core::{operation}(cljonic::Map<int, int, 4>{{}}, cljonic::core::repeat(3, cljonic::MapEntry{{1, 2}}));"
        ),
        "string-destination": (
            f'(void)cljonic::core::{operation}(cljonic::String<4>{{}}, cljonic::String<4>{{"ab"}});'
        ),
        "string-vector-char-source": (
            f"(void)cljonic::core::{operation}(cljonic::String<4>{{}}, cljonic::Vector<char, 2>{{'a', 'b'}});"
        ),
        "set-widening-source": (
            f"(void)cljonic::core::{operation}(cljonic::Set<long long, 4>{{}}, cljonic::Vector<short, 2>{{short{{1}}}});"
        ),
        "queue-widening-source": (
            f"(void)cljonic::core::{operation}(cljonic::Queue<long long, 4>{{}}, cljonic::Range<int>{{0, 2}});"
        ),
        "zero-capacity-map": (
            f"(void)cljonic::core::{operation}(cljonic::Map<int, int, 0>{{}}, cljonic::core::repeat(0, cljonic::MapEntry{{1, 2}}));"
        ),
    }.items():
        materialization["pass_cases"][label] = statement


def build_source(header: str, body: str) -> str:
    return "\n".join((f'#include "{header}"', "int main() {", f"  {body}", "  return 0;", "}"))


def compile_source(compiler: list[str], include_dir: str, source: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [*compiler, "-std=c++23", "-fsyntax-only", "-I", include_dir, "-x", "c++", "-"],
        input=source,
        text=True,
        capture_output=True,
        check=False,
    )


def normalized(text: str) -> str:
    return re.sub(r"\s+", " ", text)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--primitive", choices=PRIMITIVES)
    parser.add_argument("--list-primitives", action="store_true")
    args = parser.parse_args()
    if args.list_primitives:
        print("\n".join(PRIMITIVES))
        return 0
    compiler = os.environ.get("CXX", "c++").split()
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    configurations = (
        ("modular", os.path.join(root, "src"), "cljonic-core.hpp"),
        ("single-header", root, "cljonic.hpp"),
    )
    failures: list[str] = []

    for configuration, include_dir, header in configurations:
        for primitive, cases in PRIMITIVES.items():
            if args.primitive and primitive != args.primitive:
                continue
            anchor = cases["diagnostic_anchor"]
            for name, (body, message) in cases["diagnostic_cases"].items():
                result = compile_source(compiler, include_dir, build_source(header, body))
                if result.returncode == 0:
                    failures.append(f"{configuration}/{primitive}/{name}: expected compile failure, but compiled")
                    continue
                output = normalized(result.stdout + result.stderr)
                if anchor not in output or normalized(message) not in output:
                    failures.append(f"{configuration}/{primitive}/{name}: targeted diagnostic content not reported")

            for name, body in cases["pass_cases"].items():
                result = compile_source(compiler, include_dir, build_source(header, body))
                if result.returncode != 0:
                    failures.append(f"{configuration}/{primitive}/{name}: expected to compile, but failed")

    if failures:
        print("primitive-compile-fail:failures:")
        print("\n".join(f"  {failure}" for failure in failures))
        return 1

    print("primitive-compile-fail:ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())