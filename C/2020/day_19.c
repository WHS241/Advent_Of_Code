#ifndef DAY_19
#define DAY_19

#include <ctype.h>
#include <stdio.h>

const size_t NUM_RULES = 138;
const size_t STRING_SIZE = 100;

int day_19_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    int rules[200][3][2];          // rule[number] -> the rules
    int single_char_match[2][200]; // match[i][j] -> rule j can be matched by single char ('a' + i)

    for (int i = 0; i < 200; ++i) {
        single_char_match[0][i] = single_char_match[1][i] = 0;
        rules[i][0][0] = rules[i][0][1] = -1;
        rules[i][1][0] = rules[i][1][1] = -1;
        rules[i][2][0] = rules[i][2][1] = -1;
    }

    char next_char;
    int curr_num;

    while (1) {
        next_char = fgetc(reader);
        if (isalpha(next_char) || next_char == '\n')
            break;
        else
            ungetc(next_char, reader);

        fscanf(reader, "%d: ", &curr_num);
        next_char = fgetc(reader);
        if (next_char == '"') {
            char c;
            fscanf(reader, "%c\" ", &c);
            single_char_match[c - 'a'][curr_num] = 1;
        } else {
            ungetc(next_char, reader);
            fscanf(reader, "%d", &rules[curr_num][0][0]);
            next_char = fgetc(reader);
            if (next_char == '\n')
                continue;
            next_char = fgetc(reader);
            if (next_char != '|') {
                ungetc(next_char, reader);
                fscanf(reader, "%d", &rules[curr_num][0][1]);
                next_char = fgetc(reader);
                if (next_char == '\n')
                    continue;
                while (next_char != '|')
                    next_char = fgetc(reader);
            }

            fscanf(reader, "%d", &rules[curr_num][1][0]);
            next_char = fgetc(reader);
            if (next_char != '\n')
                fscanf(reader, "%d ", &rules[curr_num][1][1]);
        }
    }

    ungetc(next_char, reader);

    // remove one-char definitions
    for (int i = 0; i < 200; ++i) {
        if (rules[i][0][0] != -1 && rules[i][0][1] == -1) {
            single_char_match[0][i] =
              single_char_match[0][i] || single_char_match[0][rules[i][0][0]];
            single_char_match[1][i] =
              single_char_match[1][i] || single_char_match[1][rules[i][0][0]];

            int replace_rule = rules[i][0][0];
            rules[i][0][0] = rules[replace_rule][0][0];
            rules[i][0][1] = rules[replace_rule][0][1];

            int place = 1;
            if (rules[i][place][0] != -1)
                ++place;

            rules[i][place][0] = rules[replace_rule][1][0];
            rules[i][place][1] = rules[replace_rule][1][1];
        }

        if (rules[i][1][0] != -1 && rules[i][1][1] == -1) {
            single_char_match[0][i] =
              single_char_match[0][i] || single_char_match[0][rules[i][1][0]];
            single_char_match[1][i] =
              single_char_match[1][i] || single_char_match[1][rules[i][1][0]];
        }
    }

    // CYK
    char buffer[100];
    int matches[100][100][200]; // matches[first][last][rule]
    int size;

    int num_matches = 0;

    while (fscanf(reader, "%s", buffer) != EOF) {
        // base case for DP: character by character
        for (size = 0; buffer[size] != '\0'; ++size)
            for (int i = 0; i < 200; ++i)
                matches[size][size][i] = single_char_match[buffer[size] - 'a'][i];

        for (int subsize = 2; subsize <= size; ++subsize) {
            for (int i = 0; i <= size - subsize; ++i) {
                for (int r = 0; r < 200; ++r) {
                    if (rules[r][0][0] != -1) {
                        matches[i][i + subsize - 1][r] = 0;
                        for (int j = i; j < i + subsize - 1 && !matches[i][i + subsize - 1][r];
                             ++j) {
                            if ((matches[i][j][rules[r][0][0]]
                                 && matches[j + 1][i + subsize - 1][rules[r][0][1]])
                                || (rules[r][1][0] != -1 && matches[i][j][rules[r][1][0]]
                                    && matches[j + 1][i + subsize - 1][rules[r][1][1]])
                                || (rules[r][2][0] != -1 && matches[i][j][rules[r][2][0]]
                                    && matches[j + 1][i + subsize - 1][rules[r][2][1]])) {
                                matches[i][i + subsize - 1][r] = 1;
                            }
                        }
                    }
                }
            }
        }

        num_matches += matches[0][size - 1][0];
    }

    printf("%d\n", num_matches);

    return 0;
}

#endif // DAY_19
