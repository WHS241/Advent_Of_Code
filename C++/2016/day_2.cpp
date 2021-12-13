#ifndef DAY_2
#define DAY_2

#include <fstream>
#include <iostream>
#include <sstream>

constexpr std::size_t NUM_ROWS_P1 = 3;
constexpr std::size_t NUM_COLS_P1 = 3;
constexpr std::size_t START = 5;

constexpr std::size_t NUM_COLS_P2 = 5;
constexpr std::size_t NUM_ROWS_P2 = 5;
constexpr std::size_t values[5][5] = {
  {0, 0, 1, 0, 0}, {0, 2, 3, 4, 0}, {5, 6, 7, 8, 9}, {0, 10, 11, 12, 0}, {0, 0, 13, 0, 0}};
constexpr std::size_t START_ROW = 2;
constexpr std::size_t START_COL = 0;

int day_2_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::ostringstream password_p1, password_p2;
    password_p2 << std::hex << std::uppercase;

    std::size_t curr = START;
    std::size_t row(START_ROW), col(START_COL);

    for (std::string line; std::getline(reader, line);) {
        for (char c : line) {
            if (c == 'U') {
                if (curr > NUM_COLS_P1)
                    curr -= NUM_COLS_P1;
                if (row > 0 && values[row - 1][col] != 0)
                    --row;
            } else if (c == 'D') {
                if (curr <= (NUM_ROWS_P1 - 1) * NUM_COLS_P1)
                    curr += NUM_COLS_P1;
                if (row < NUM_ROWS_P2 - 1 && values[row + 1][col] != 0)
                    ++row;
            } else if (c == 'L') {
                if (curr % NUM_COLS_P1 != 1)
                    --curr;
                if (col > 0 && values[row][col - 1] != 0)
                    --col;
            } else if (c == 'R') {
                if (curr % NUM_COLS_P1 != 0)
                    ++curr;
                if (col < NUM_COLS_P2 - 1 && values[row][col + 1] != 0)
                    ++col;
            }
        }
        password_p1 << curr;
        password_p2 << values[row][col];
    }

    std::cout << password_p1.str() << '\n' << password_p2.str() << std::endl;

    return 0;
}

#endif // DAY_2
