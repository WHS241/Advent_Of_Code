#ifndef DAY_4
#define DAY_4

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

typedef bool (*check_string)(char*);

bool check_byr(char* s) {
    for (int i = 0; i < 4; ++i)
        if (!isdigit(s[i]))
            return false;
    int yr = atoi(s);
    return yr >= 1920 && yr <= 2002;
}

bool check_iyr(char* s) {
    for (int i = 0; i < 4; ++i)
        if (!isdigit(s[i]))
            return false;
    int yr = atoi(s);
    return yr >= 2010 && yr <= 2020;
}

bool check_eyr(char* s) {
    for (int i = 0; i < 4; ++i)
        if (!isdigit(s[i]))
            return false;
    int yr = atoi(s);
    return yr >= 2020 && yr <= 2030;
}

bool check_hgt(char* s) {
    char num_buf[4];
    int i = 0;
    while (i < 4 && isdigit(s[i])) {
        num_buf[i] = s[i];
        ++i;
    }
    if (i == 4 || i == 0)
        return false;

    num_buf[i] = '\0';
    int val = atoi(num_buf);
    if (i == 2) {
        return !strcmp(s + 2, "in") && val >= 59 && val <= 76;
    }
    if (i == 3) {
        return !strcmp(s + 3, "cm") && val >= 150 && val <= 193;
    }
    return false;
}

bool check_hcl(char* s) {
    for (int i = 1; i < 7; ++i)
        if (!isxdigit(s[i]))
            return false;
    return s[7] == '\0' && s[0] == '#';
}

bool check_ecl(char* s) {
    const char VALID[7][4] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    for (int i = 0; i < 7; ++i)
        if (!strcmp(s, VALID[i]))
            return true;

    return false;
}

bool check_pid(char* s) {
    for (int i = 0; i < 9; ++i)
        if (!isdigit(s[i]))
            return false;
    return s[9] == '\0';
}

int day_4_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    const char CHECK_FIELDS[7][4] = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    const check_string FUNCS[7] = {&check_byr, &check_iyr, &check_eyr, &check_hgt,
                                   &check_hcl, &check_ecl, &check_pid};

    char buffer[200];
    int count_fields_1 = 0, count_fields_2 = 0, count_valids_1 = 0, count_valids_2 = 0;
    while (fgets(buffer, 200, reader)) {
        if (buffer[0] == '\n') {
            if (count_fields_1 == 7)
                ++count_valids_1;
            if (count_fields_2 == 7)
                ++count_valids_2;
            count_fields_1 = count_fields_2 = 0;
            continue;
        }
        int size = 0;
        while (buffer[size] != '\n' && buffer[size] != '\0')
            ++size;

        for (int i = 0; i <= size; ++i) {
            if (isspace(buffer[i]))
                buffer[i] = '\0';
        }

        int j = 0;
        while (j < size) {
            for (int i = 0; i < 7; ++i) {
                if (!strncmp(buffer + j, CHECK_FIELDS[i], 3) && buffer[j + 3] == ':') {
                    ++count_fields_1;
                    if (FUNCS[i](buffer + j + 4))
                        ++count_fields_2;
                }
            }

            while (buffer[j++] != '\0')
                ;
        }
    }
    if (count_fields_1 == 7)
        ++count_valids_1;
    if (count_fields_2 == 7)
        ++count_valids_2;

    printf("%d\n%d\n", count_valids_1, count_valids_2);

    return 0;
}

#endif // DAY_0
