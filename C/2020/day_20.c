#ifndef DAY_20
#define DAY_20

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int HASH_SIZE = 103;
static const char DRAGON[3][21] = {"                  # ", "#    ##    ##    ###",
                                   " #  #  #  #  #  #   "};

typedef struct {
    char x[10][11];
} grid;
typedef struct {
    size_t x[4];
} neighbor_list;

typedef struct {
    size_t id;
    grid g;
    void* next;
} table_entry;

typedef struct {
    size_t id;
    neighbor_list e;
    void* next;
} graph_entry;

typedef struct {
    size_t id;
    void* next;
} set_entry;

void orient(grid* g, short type) {
    switch (type) {
    case 0:
        // 不转
        break;

    case 1:
        //顺时针
        orient(g, 4);
        orient(g, 7);
        break;

    case 2:
        //逆时针
        orient(g, 5);
        orient(g, 7);
        break;

    case 3:
        // 180度旋转
        orient(g, 4);
        orient(g, 5);
        break;

    case 4:
        //反射（横轴）
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 10; ++j) {
                char t = g->x[i][j];
                g->x[i][j] = g->x[9 - i][j];
                g->x[9 - i][j] = t;
            }
        }
        break;

    case 5:
        //反射（竖轴）
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 5; ++j) {
                char t = g->x[i][j];
                g->x[i][j] = g->x[i][9 - j];
                g->x[i][9 - j] = t;
            }
        }
        break;

    case 6:
        // 反射（轴为右上到左下）
        orient(g, 1);
        orient(g, 4);
        break;

    case 7:
        // 反射（轴为左上到右下）
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < i; ++j) {
                char t = g->x[i][j];
                g->x[i][j] = g->x[j][i];
                g->x[j][i] = t;
            }
        }
        break;

    default:
        break;
    }
}

int matches(const grid* g1, const grid* g2) {
    char temp1[4][11];
    char temp2[4][11];

    for (int i = 0; i < 10; ++i) {
        temp1[0][i] = g1->x[0][i];
        temp1[1][i] = g1->x[9][i];
        temp1[2][i] = g1->x[i][0];
        temp1[3][i] = g1->x[i][9];

        temp2[0][i] = g2->x[0][i];
        temp2[1][i] = g2->x[9][i];
        temp2[2][i] = g2->x[i][0];
        temp2[3][i] = g2->x[i][9];
    }
    for (int i = 0; i < 4; ++i)
        temp1[i][10] = temp2[i][10] = '\0';

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!strcmp(temp1[i], temp2[j]))
                return 8 * i + 2 * j;
            bool reversed = true;
            for (int k = 0; reversed && k < 10; ++k)
                if (temp1[i][k] != temp2[j][9 - k])
                    reversed = false;
            if (reversed)
                return 8 * i + 2 * j + 1;
        }
    }
    return -1;
}

table_entry* get_entry(table_entry** table, size_t id) {
    int bucket = id % HASH_SIZE;
    table_entry* curr = table[bucket];
    while (curr) {
        if (curr->id == id)
            break;
        curr = (table_entry*) curr->next;
    }
    return curr;
}

graph_entry* get_node(graph_entry** graph, size_t id) {
    int bucket = id % HASH_SIZE;
    graph_entry* curr = graph[bucket];
    while (curr) {
        if (curr->id == id)
            break;
        curr = (graph_entry*) curr->next;
    }

    return curr;
}

set_entry* contains(set_entry** set, size_t id) {
    int bucket = id % HASH_SIZE;
    set_entry* curr = set[bucket];
    while (curr) {
        if (curr->id == id)
            break;
        curr = (set_entry*) curr->next;
    }

    return curr;
}

size_t find_dragons(char* grid, size_t dim_x, size_t dim_y) {
    size_t result = 0;
    for (size_t i = 0; i <= dim_x - 3; ++i) {
        for (size_t j = 0; j <= dim_y - 20; ++j) {
            bool matches = true;
            for (size_t m = 0; matches && m < 3; ++m)
                for (size_t n = 0; matches && n < 20; ++n)
                    if (DRAGON[m][n] != ' ' && DRAGON[m][n] != grid[(i + m) * dim_y + j + n])
                        matches = false;

            if (matches)
                ++result;
        }
    }
    return result;
}

int day_20_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    FILE* reader = fopen(argv[1], "r");

    table_entry* tiles[HASH_SIZE];
    graph_entry* adjacencies[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i) {
        tiles[i] = NULL;
        adjacencies[i] = NULL;
    }

    // 读取tiles
    while (1) {
        int id;
        if (fscanf(reader, "%*s %d:\n", &id) == EOF)
            break;

        table_entry* tile = malloc(sizeof(table_entry));
        tile->id = id;
        tile->next = tiles[id % HASH_SIZE];
        tiles[id % HASH_SIZE] = tile;

        for (int i = 0; i < 10; ++i)
            fscanf(reader, "%s\n", &tile->g.x[i][0]);
    }

    for (int i = 0; i < HASH_SIZE; ++i) {
        for (table_entry* curr = tiles[i]; curr; curr = curr->next) {
            graph_entry* node = malloc(sizeof(graph_entry));
            node->id = curr->id;
            node->next = adjacencies[i];
            adjacencies[i] = node;

            for (int j = 0; j < 4; ++j)
                node->e.x[j] = 0;
            int size = 0;

            for (int j = 0; j < HASH_SIZE; ++j)
                for (table_entry* target = tiles[j]; target; target = target->next)
                    if (target->id != curr->id && matches(&curr->g, &target->g) != -1)
                        node->e.x[size++] = target->id;
        }
    }

    size_t corner_id, prod = 1;
    for (int i = 0; i < HASH_SIZE; ++i) {
        for (graph_entry* node = adjacencies[i]; node; node = node->next) {
            if (!node->e.x[2]) {
                corner_id = node->id;
                prod *= node->id;
            }
        }
    }
    printf("%zu\n", prod);

    // 固定第一角的方向
    table_entry* first_corner = get_entry(tiles, corner_id);
    graph_entry* neighbors = get_node(adjacencies, corner_id);
    size_t orientation =
      4 * (matches(&first_corner->g, &get_entry(tiles, neighbors->e.x[0])->g) / 8)
      + (matches(&first_corner->g, &get_entry(tiles, neighbors->e.x[1])->g) / 8);
    switch (orientation) {
    case 2:
    case 8:
        orient(&first_corner->g, 6);
        break;

    case 3:
    case 12:
        orient(&first_corner->g, 4);
        break;

    case 6:
    case 9:
        orient(&first_corner->g, 5);
        break;

    case 7:
    case 13:
        break;

    default:
        printf("ORIENT ERROR");
        return 1;
    }

    // 判断图的大小
    set_entry* first_row = malloc(sizeof(set_entry));
    first_row->id = corner_id;
    first_row->next = NULL;
    set_entry* first_col = malloc(sizeof(set_entry));
    first_col->id = corner_id;
    first_col->next = NULL;

    size_t dim_x = 0;
    for (set_entry* curr = first_row; curr; curr = curr->next) {
        ++dim_x;
        int id = curr->id;
        table_entry* curr_tile = get_entry(tiles, id);
        graph_entry* neighbors = get_node(adjacencies, id);

        //找出右边的，若存在
        for (int i = 0; i < 4; ++i) {
            if (neighbors->e.x[i] == 0)
                break;
            table_entry* nearby_tile = get_entry(tiles, neighbors->e.x[i]);
            int match_key = matches(&curr_tile->g, &nearby_tile->g);
            if (match_key / 8 == 3) {
                // 修正定向
                short orient_key;
                switch (match_key % 8) {
                case 0:
                    orient_key = 7;
                    break;

                case 1:
                    orient_key = 2;
                    break;

                case 2:
                    orient_key = 1;
                    break;

                case 3:
                    orient_key = 6;
                    break;

                case 4:
                    orient_key = 0;
                    break;

                case 5:
                    orient_key = 4;
                    break;

                case 6:
                    orient_key = 5;
                    break;

                case 7:
                    orient_key = 3;
                    break;
                }
                orient(&nearby_tile->g, orient_key);

                // 存下
                curr->next = malloc(sizeof(set_entry));
                ((set_entry*) curr->next)->id = nearby_tile->id;
                ((set_entry*) curr->next)->next = NULL;
                break;
            }
        }
    }

    size_t dim_y = 0;
    for (set_entry* curr = first_col; curr; curr = curr->next) {
        ++dim_y;
        int id = curr->id;
        table_entry* curr_tile = get_entry(tiles, id);
        graph_entry* neighbors = get_node(adjacencies, id);

        //找出下边的，若存在
        for (int i = 0; i < 4; ++i) {
            if (neighbors->e.x[i] == 0)
                break;
            table_entry* nearby_tile = get_entry(tiles, neighbors->e.x[i]);
            int match_key = matches(&curr_tile->g, &nearby_tile->g);
            if (match_key / 8 == 1) {
                // 修正定向
                short orient_key;
                switch (match_key % 8) {
                case 0:
                    orient_key = 0;
                    break;

                case 1:
                    orient_key = 5;
                    break;

                case 2:
                    orient_key = 4;
                    break;

                case 3:
                    orient_key = 3;
                    break;

                case 4:
                    orient_key = 7;
                    break;

                case 5:
                    orient_key = 1;
                    break;

                case 6:
                    orient_key = 2;
                    break;

                case 7:
                    orient_key = 6;
                    break;
                }
                orient(&nearby_tile->g, orient_key);

                // 存下
                curr->next = malloc(sizeof(set_entry));
                ((set_entry*) curr->next)->id = nearby_tile->id;
                ((set_entry*) curr->next)->next = NULL;
                break;
            }
        }
    }

    char* grid = malloc(dim_x * dim_y * 8 * 8 * sizeof(char));
    // 填满第一行
    set_entry* curr = first_row;
    for (size_t i = 0; i < dim_x; ++i, curr = curr->next) {
        table_entry* tile = get_entry(tiles, curr->id);
        for (size_t j = 0; j < 8; ++j)
            for (size_t k = 0; k < 8; ++k)
                grid[j * (8 * dim_x) + 8 * i + k] = tile->g.x[j + 1][k + 1];
    }
    // 填满剩下的
    set_entry* curr_row_first = first_col->next;
    for (size_t i = 1; i < dim_y; ++i, curr_row_first = curr_row_first->next) {
        table_entry* row_first_tile = get_entry(tiles, curr_row_first->id);
        for (size_t j = 0; j < 8; ++j)
            for (size_t k = 0; k < 8; ++k)
                grid[(8 * i + j) * 8 * dim_x + k] = row_first_tile->g.x[j + 1][k + 1];

        // 后面的块
        size_t last = curr_row_first->id;
        for (size_t j = 1; j < dim_x; ++j) {
            table_entry* last_tile = get_entry(tiles, last);
            graph_entry* neighbors = get_node(adjacencies, last);

            //找出右边的
            for (int k = 0; k < 4; ++k) {
                if (neighbors->e.x[k] == 0)
                    break;
                table_entry* nearby_tile = get_entry(tiles, neighbors->e.x[k]);
                int match_key = matches(&last_tile->g, &nearby_tile->g);
                if (match_key / 8 == 3) {
                    // 修正定向
                    short orient_key;
                    switch (match_key % 8) {
                    case 0:
                        orient_key = 7;
                        break;

                    case 1:
                        orient_key = 2;
                        break;

                    case 2:
                        orient_key = 1;
                        break;

                    case 3:
                        orient_key = 6;
                        break;

                    case 4:
                        orient_key = 0;
                        break;

                    case 5:
                        orient_key = 4;
                        break;

                    case 6:
                        orient_key = 5;
                        break;

                    case 7:
                        orient_key = 3;
                        break;
                    }
                    orient(&nearby_tile->g, orient_key);

                    // 存下
                    last = nearby_tile->id;
                    for (int m = 0; m < 8; ++m)
                        for (int n = 0; n < 8; ++n)
                            grid[(8 * i + m) * 8 * dim_x + 8 * j + n] =
                              nearby_tile->g.x[m + 1][n + 1];
                    break;
                }
            }
        }
    }

    dim_x *= 8;
    dim_y *= 8;
    size_t num_signals = 0;
    for (size_t i = 0; i < dim_x; ++i)
        for (size_t j = 0; j < dim_y; ++j)
            if (grid[i * dim_x + j] == '#')
                ++num_signals;

    // 搜出海蛇
    bool flipped_dir = false;
    // 横轴 + 斜轴 -> 四个方向，加上反射
    for (int i = 0; i < 8; ++i) {
        size_t found = find_dragons(grid, dim_x, dim_y);
        if (found) {
            printf("%zd\n", num_signals - found * 15);
            break;
        }
        if (flipped_dir) {
            for (size_t j = 0; j < dim_x; ++j) {
                for (size_t k = 0; k < dim_y / 2; ++k) {
                    char temp = grid[j * dim_x + k];
                    grid[j * dim_x + k] = grid[j * dim_x + dim_y - k - 1];
                    grid[j * dim_x + dim_y - k - 1] = temp;
                }
            }
        } else {
            for (size_t j = 0; j < dim_x; ++j) {
                for (size_t k = j; k < dim_y; ++k) {
                    char temp = grid[j * dim_x + k];
                    grid[j * dim_x + k] = grid[k * dim_x + j];
                    grid[k * dim_x + j] = temp;
                }
            }
        }
        flipped_dir = !flipped_dir;
    }

    return 0;
}

#endif // DAY_20
