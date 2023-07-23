#include "block.hpp"
#include "board.hpp"
#include "search.hpp"
#include <string>
#include <deque>
#include <iostream>
#include <chrono>
#include <cassert>
using namespace std;

bool _ = ios::sync_with_stdio(false);
constexpr int WIDTH = 10, HEIGHT = 24;
using BOARD = reachability::board_t<WIDTH, HEIGHT>;
BOARD str_to_board(deque<string> &&b_str) {
  while (b_str.size() < HEIGHT) {
    b_str.emplace_front(WIDTH, ' ');
  }
  string res;
  for (auto &s : b_str) {
    assert(s.size() == WIDTH);
    res.append(s.rbegin(), s.rend());
  }
  return {res};
}
auto bench(auto f, int count=50000) {
  auto start = chrono::system_clock::now();
  for (int _ = 0; _ < count; ++_) {
    f();
  }
  auto end = chrono::system_clock::now();
  return double(chrono::duration_cast<chrono::nanoseconds>(end - start).count()) / count;
}
template <reachability::block block, bool print=false, reachability::coord start=reachability::coord{4, 20}, unsigned init_rot=0>
array<double, 2> test(const BOARD &b, const string &name) {
  using namespace reachability::search;
  cout << "BOARD " << name << endl;
  int length = block.ORIENTATIONS;
  auto binary = binary_bfs<block, start, init_rot>(b);
  auto ordinary = ordinary_bfs_without_binary(b, block, start, init_rot);
  for (int i = 0; i < length; ++i) {
    if (binary[i] != ordinary[i]) {
      cout << "  binary[" << i << "] != ordinary[" << i << "]" << endl;
      cout << to_string(binary[i], ordinary[i], b);
    } else if (print) {
      cout << "  result[" << i << "]" << endl;
      cout << to_string(binary[i], b);
    }
  }
  auto binary_time = bench([&](){binary = binary_bfs<block, start, init_rot>(b);}, 1000000);
  cout << "  binary  : " << binary_time << "ns" << endl;
  auto ordinary_time = bench([&](){ordinary = ordinary_bfs_without_binary(b, block, start, init_rot);});
  cout << "  true ord: " << ordinary_time << "ns" << endl;
  return {binary_time, ordinary_time};
}
template <bool print=false, reachability::coord start=reachability::coord{4, 20}, unsigned init_rot=0>
array<double, 2> test(const BOARD &b, const string &name, char block) {
  using namespace reachability::search;
  cout << "BOARD " << name << endl;
  cout << " BLOCK " << block << endl;
  inv_board_t<WIDTH, HEIGHT> binary[4], ordinary[4];
  binary_bfs<start, init_rot>(binary, b, block);
  ordinary_bfs_without_binary(ordinary, b, block, start, init_rot);
  for (int i = 0; i < get_orientations(block); ++i) {
    if (binary[i] != ordinary[i]) {
      cout << "  binary[" << i << "] != ordinary[" << i << "]" << endl;
      cout << to_string(binary[i], ordinary[i], b);
    } else if (print) {
      cout << "  result[" << i << "]" << endl;
      cout << to_string(binary[i], b);
    }
  }
  auto binary_time = bench([&](){binary_bfs<start, init_rot>(binary, b, block);}, 1000000);
  cout << "  binary  : " << binary_time << "ns" << endl;
  auto ordinary_time = bench([&](){ordinary_bfs_without_binary(ordinary, b, block, start, init_rot);});
  cout << "  true ord: " << ordinary_time << "ns" << endl;
  return {binary_time, ordinary_time};
}
int main() {
  double binary_sum = 0, ordinary_sum = 0;
  const deque<BOARD> boards = {
    str_to_board({
      "X         ",
      "X         ",
      "XX      XX",
      "XXX    XXX",
      "XXXX   XXX",
      "XXXX  XXXX",
      "XXX   XXXX",
      "XXXX XXXXX"
    }),
    str_to_board({
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
    str_to_board({
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
    str_to_board({
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
  const deque<string> board_names = {
    "LEMONTEA TSPIN", "LEMONTEA DT", "LEMONTEA TERRIBLE", "4T"
  };
  for (size_t i = 0; i < board_names.size(); ++i) {
    for (char block : string{"IJLOSTZ"}) {
      auto [binary_time, ordinary_time] = test(boards[i], board_names[i], block);
      binary_sum += binary_time;
      ordinary_sum += ordinary_time;
    }
  }
  cout << "TOTAL binary  : " << binary_sum << "ns" << endl;
  cout << "TOTAL true ord: " << ordinary_sum << "ns" << endl;
  using namespace reachability::blocks;
  using namespace SRS;
  test<Z+SRS_common_kick, true>(str_to_board({
    "XXXX  XXX ",
    "XXXXX XXXX",
    "      X  X",
    "          ",
    "   X      ",
    "    XX X X",
    "XXX X     ",
    "XX  X    X",
    "X    X    ",
    "XX  XXXX X",
    "X    XX   ",
    "XX XXX    ",
    "       X  ",
    "      XXX ",
    "XX X      ",
    "X    X    ",
    "X   X    X",
    "X  XX X XX",
    "XX  XXXXXX",
  }), "LZT");
  test<L+SRS_common_kick, true, reachability::coord{1, 1}, 2>(str_to_board({
    "     X    ",
    "XXX X     ",
    "         X",
    " X X  XX  ",
    " X        ",
    "    X X  X",
    "X   XXX   ",
    "X XX      ",
    "       X  ",
    "    X     ",
    "  XX X X  ",
    "X     XX  ",
    "   X X    ",
    " X      X ",
    "    X     ",
    "XX     X X",
    "    X X   ",
    "X  X      ",
    "    XX    ",
    "  X       ",
    "       X  ",
    " XXX XX X ",
  }), "FARTER 1");
  test<I+SRS_I_kick, true, reachability::coord{7, 0}, 0>(str_to_board({
    "       X  ",
    "       X  ",
    "   X   XX ",
    "        X ",
    "        X ",
    "    X   XX",
    "         X",
    "         X",
    "     X   X",
    "          ",
    "          ",
    "      X   ",
    "          ",
    "          ",
    "       X  ",
    "          ",
    "          ",
    "          "
  }), "FARTER 2");
}