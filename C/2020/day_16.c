#ifndef DAY_16
#define DAY_16

#include <stdio.h>

const int NUM_FIELDS = 20;
const int FIELD_SIZE = 20;

int day_16_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");
    char fields[NUM_FIELDS][FIELD_SIZE];
    int ranges[NUM_FIELDS][4];

    for (int i = 0; i < NUM_FIELDS; ++i) {
        char c;
        int j = 0;
        while ((c = fgetc(reader)) != ':')
            fields[i][j++] = c;

        fields[i][j] = '\0';

        fscanf(reader, "%d-%d or %d-%d\n", ranges[i], ranges[i] + 1, ranges[i] + 2, ranges[i] + 3);
    }

    fscanf(reader, "%*s %*s\n");

    int my_ticket[NUM_FIELDS];
    for (int i = 0; i < NUM_FIELDS; ++i)
        fscanf(reader, "%d%*c", my_ticket + i);

    fscanf(reader, " %*s %*s\n");
    int is_valid[NUM_FIELDS][NUM_FIELDS]; // is_valid[pos][field]

    for (int i = 0; i < NUM_FIELDS; ++i)
        for (int j = 0; j < NUM_FIELDS; ++j)
            is_valid[i][j] = 1;

    int invalid = 0;
    for (int last_code = 0; last_code != EOF;) {
        int curr_ticket[NUM_FIELDS];
        for (int i = 0; i < NUM_FIELDS && last_code != EOF; ++i)
            last_code = fscanf(reader, "%d%*c", curr_ticket + i);
        if (last_code == EOF)
            break;

        // Validate
        int found_invalid = 0;
        for (int i = 0; i < NUM_FIELDS && !found_invalid; ++i) {
            found_invalid = 1;
            for (int j = 0; j < NUM_FIELDS && found_invalid; ++j)
                if ((ranges[j][0] <= curr_ticket[i] && curr_ticket[i] <= ranges[j][1])
                    || (ranges[j][2] <= curr_ticket[i] && curr_ticket[i] <= ranges[j][3]))
                    found_invalid = 0;

            if (found_invalid) {
                invalid += curr_ticket[i];
            }
        }

        // Filter
        if (!found_invalid)
            for (int i = 0; i < NUM_FIELDS; ++i)
                for (int j = 0; j < NUM_FIELDS; ++j)
                    if (is_valid[i][j]
                        && (curr_ticket[i] < ranges[j][0]
                            || (ranges[j][1] < curr_ticket[i] && curr_ticket[i] < ranges[j][2])
                            || ranges[j][3] < curr_ticket[i]))
                        is_valid[i][j] = 0;
    }

    // Deduce via Hopcroft-Karp
    int map[NUM_FIELDS];
    int rev_map[NUM_FIELDS];
    int remaining[NUM_FIELDS];

    for (int i = 0; i < NUM_FIELDS; ++i) {
        map[i] = rev_map[i] = NUM_FIELDS;
        remaining[i] = i;
    }
    int num_remaining = NUM_FIELDS;
    while (num_remaining) {
        int path_length = 2 * NUM_FIELDS;
        int layer_lookup[NUM_FIELDS][2];
        int layer_size[2 * NUM_FIELDS];
        int layer[2 * NUM_FIELDS][NUM_FIELDS];

        layer_size[0] = num_remaining;
        layer_size[NUM_FIELDS] = 0;
        layer_lookup[0][1] = NUM_FIELDS;
        for (int i = 1; i < NUM_FIELDS; ++i) {
            layer_size[i] = layer_size[i + NUM_FIELDS] = 0;
            layer_lookup[i][0] = layer_lookup[i][1] = NUM_FIELDS;
        }

        for (int i = 0; i < num_remaining; ++i) {
            layer_lookup[remaining[i]][0] = 0;
            layer[0][i] = remaining[i];
        }

        // BFS
        for (int i = 0; i < path_length && layer_size[i]; ++i) {
            if (i % 2) {
                for (int j = 0; j < layer_size[i]; ++j) {
                    layer_lookup[rev_map[layer[i][j]]][0] = i + 1;
                    layer[i + 1][layer_size[i + 1]++] = rev_map[layer[i][j]];
                }
            } else {
                for (int j = 0; j < layer_size[i]; ++j) {
                    for (int k = 0; k < NUM_FIELDS; ++k) {
                        if (map[layer[i][j]] != k && layer_lookup[k][1] == NUM_FIELDS
                            && is_valid[layer[i][j]][k]) {
                            layer_lookup[k][1] = i + 1;
                            layer[i + 1][layer_size[i + 1]++] = k;
                            if (rev_map[k] == NUM_FIELDS)
                                path_length = i + 1;
                        }
                    }
                }
            }
        }

        // Clear non-matched
        int terminals = 0;
        for (int i = 0; i < layer_size[path_length]; ++i)
            if (rev_map[layer[path_length][i]] == NUM_FIELDS)
                layer[path_length][terminals++] = layer[path_length][i];

        // Match with DFS
        int path[2 * NUM_FIELDS];
        for (int i = 0; i < num_remaining; ++i) {
            path[0] = layer[0][i];
            int j = 0;
            int indices[2 * NUM_FIELDS];
            for (int k = 0; k <= path_length; ++k)
                indices[k] = 0;
            indices[0] = i;

            while (path[0] != NUM_FIELDS) {
                int k;
                for (k = indices[j + 1]; k < layer_size[j + 1]; ++k)
                    if (layer[j + 1][k] != NUM_FIELDS && is_valid[path[j]][layer[j + 1][k]])
                        break;

                if (k != layer_size[j + 1]) {
                    path[j + 1] = layer[j + 1][k];
                    indices[j + 1] = indices[j + 2] = k;
                    if (j + 1 != path_length) {
                        path[j + 2] = layer[j + 2][k];
                        j += 2;
                    } else {
                        break;
                    }
                } else {
                    // no possible path forward
                    layer[j][indices[j]] = NUM_FIELDS;
                    path[j] = NUM_FIELDS;
                    if (j) {
                        layer[j - 1][indices[j]] = NUM_FIELDS;
                        j -= 2;
                    }
                }
            }

            if (path[0] != NUM_FIELDS) {
                for (int i = 0; i < path_length; i += 2) {
                    map[path[i]] = path[i + 1];
                    rev_map[path[i + 1]] = path[i];
                    layer[i][indices[i]] = layer[i + 1][indices[i + 1]] = NUM_FIELDS;
                }
            }
        }

        // Check for remaining unmatched
        num_remaining = 0;
        for (int i = 0; i < NUM_FIELDS; ++i)
            if (map[i] == NUM_FIELDS)
                remaining[num_remaining++] = i;
    }

    printf("%d\n", invalid);

    for (int i = 0; i < NUM_FIELDS; ++i)
        printf("%s: %d\n", fields[i], my_ticket[rev_map[i]]);

    return 0;
}

#endif // DAY_16
