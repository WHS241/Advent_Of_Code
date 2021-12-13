#ifndef DAY_12
#define DAY_12

#include <fstream>
#include <iostream>

int day_12_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);

    long p1_x(0), p1_y(0);
    int orient(1);

    long p2_x(0), p2_y(0);
    long w_x(10), w_y(1); // waypoint rel. coords

    for (std::string line; std::getline(reader, line);) {
        char direction = line.front();
        std::size_t mag = std::stoul(line.substr(1));

        std::size_t rotate = 0; // for rotating waypoint

        switch (direction) {
        case 'N':
            p1_y += mag;
            w_y += mag;
            break;

        case 'S':
            p1_y -= mag;
            w_y -= mag;
            break;

        case 'E':
            p1_x += mag;
            w_x += mag;
            break;

        case 'W':
            p1_x -= mag;
            w_x -= mag;
            break;

        case 'L':
            orient = (orient + 3 * (mag / 90)) % 4;
            rotate = 3 * mag / 90 % 4;
            break;

        case 'R':
            orient = (orient + mag / 90) % 4;
            rotate = mag / 90 % 4;
            break;

        case 'F':
            switch (orient) {
            case 0:
                p1_y += mag;
                break;

            case 1:
                p1_x += mag;
                break;

            case 2:
                p1_y -= mag;
                break;

            case 3:
                p1_x -= mag;
                break;
            }
            p2_x += w_x * mag;
            p2_y += w_y * mag;
            break;
        }

        switch (rotate) {
        case 1:
            std::swap(w_x, w_y);
            w_y *= -1;
            break;

        case 2:
            w_x *= -1;
            w_y *= -1;
            break;

        case 3:
            std::swap(w_x, w_y);
            w_x *= -1;
            break;

        default:
            break;
        }
    }

    std::cout << '(' << p1_x << ',' << p1_y << ")\n"
              << '(' << p2_x << ',' << p2_y << ')' << std::endl;

    return 0;
}

#endif // DAY_12
