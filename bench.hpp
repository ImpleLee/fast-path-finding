#pragma once
#include <array>
#include <initializer_list>
#include <string_view>
#include <chrono>
#include "board.hpp"

constexpr int WIDTH = 10, HEIGHT = 24;
using BOARD = reachability::board_t<WIDTH, HEIGHT>;
constexpr auto merge_str(std::initializer_list<std::string_view> &&b_str) {
  std::array<char, WIDTH*HEIGHT> res = {};
  unsigned pos = 0;
  if (b_str.size() < HEIGHT) {
    for (pos = 0; pos < WIDTH * (HEIGHT - b_str.size()); ++pos) {
      res[pos] = ' ';
    }
  }
  for (auto &s : b_str) {
    for (unsigned i = 0; i < WIDTH; ++i) {
      res[pos + i] = s[WIDTH - i - 1];
    }
    pos += WIDTH;
  }
  return BOARD::convert_to_array(std::string_view{res.data(), res.size()});
}

inline constexpr std::array boards = {
  merge_str({
    "X         ",
    "X         ",
    "XX      XX",
    "XXX    XXX",
    "XXXX   XXX",
    "XXXX  XXXX",
    "XXX   XXXX",
    "XXXX XXXXX"
  }),
  merge_str({
    "        XX",
    "XXXX  XXXX",
    "XXX   XXXX",
    "XXX XXXXXX",
    "XXX  XXXXX",
    "XX   XXXXX",
    "XXX XXXXXX",
    "XXX XXXXXX",
    "XXXX XXXXX"
  }),
  merge_str({
    "XXXXXXXX  ",
    "XXXXXXXX  ",
    "X         ",
    "X         ",
    "X  XXXXXXX",
    "X  XXXXXXX",
    "X        X",
    "X        X",
    "XXXXXXXX X",
    "XXXXXXXX X",
    "         X",
    "         X"
  }),
  merge_str({
    "   XX     ",
    "X XX    XX",
    "XXXX XXXXX",
    "XXXX   XXX",
    "XXX     XX",
    "XXXXXX XXX",
    "XXXX   XXX",
    "XXXXX XXXX",
    "XXXX XXXXX",
    "XXXX XXXXX"
  })
};
inline constexpr std::array board_names = {
  "LEMONTEA TSPIN", "LEMONTEA DT", "LEMONTEA TERRIBLE", "4T"
};

auto bench(auto f, int count=50000) {
  auto start = std::chrono::system_clock::now();
  for (int _ = 0; _ < count; ++_) {
    f();
  }
  auto end = std::chrono::system_clock::now();
  return double(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / count;
}