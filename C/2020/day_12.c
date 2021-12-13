#ifndef DAY_12
#define DAY_12

#include <stdio.h>
#include <stdlib.h>

void move_1(int dir, int mag, int* x, int* y) {
    switch (dir) {
    case 0:
        *x += mag;
        break;

    case 1:
        *y += mag;
        break;

    case 2:
        *x -= mag;
        break;

    case 3:
        *y -= mag;
        break;
    }
}

void rotate(int degree, int* x, int* y) {
    int temp;
    switch (degree) {
    case 90:
        temp = *x;
        *x = -*y;
        *y = temp;
        break;

    case 180:
        *x *= -1;
        *y *= -1;
        break;

    case 270:
        temp = *x;
        *x = *y;
        *y = -temp;
        break;

    default:
        break;
    }
}

int day_12_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    char action;
    int mag;

    int curr_x_1 = 0;
    int curr_y_1 = 0;
    int curr_x_2 = 0;
    int curr_y_2 = 0;

    int target_x = 10;
    int target_y = 1;

    int dir = 0;
    while (fscanf(reader, "%c%d\n", &action, &mag) != EOF) {
        int curr_dir_1 = 0;
        switch (action) {
        case 'N':
            curr_dir_1 = 1;
            target_y += mag;
            break;

        case 'S':
            curr_dir_1 = 3;
            target_y -= mag;
            break;

        case 'E':
            curr_dir_1 = 0;
            target_x += mag;
            break;

        case 'W':
            curr_dir_1 = 2;
            target_x -= mag;
            break;

        case 'R':
            dir = (dir + 3 * mag / 90) % 4;
            rotate(360 - mag, &target_x, &target_y);
            mag = 0;
            break;

        case 'L':
            dir = (dir + mag / 90) % 4;
            rotate(mag, &target_x, &target_y);
            mag = 0;
            break;

        case 'F':
            curr_dir_1 = dir;
            curr_x_2 += mag * target_x;
            curr_y_2 += mag * target_y;
            break;

        default:
            break;
        }
        move_1(curr_dir_1, mag, &curr_x_1, &curr_y_1);
    }

    printf("%d\n", abs(curr_x_1) + abs(curr_y_1));
    printf("%d\n", abs(curr_x_2) + abs(curr_y_2));

    return 0;
}

#endif // DAY_12
