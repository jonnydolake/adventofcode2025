#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

enum Direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT,
};

enum Position{
    TOP_LEFT = 0,
    TOP_RIGHT,
    BOT_RIGHT,
    BOT_LEFT,
};

typedef struct {
    int x;
    int y;
    int32_t index;
} iVecD;

typedef struct {
    int min_x;
    int min_y;
    bool checked_x;
    bool checked_y;
} MinChecker;

typedef struct {
    int64_t size;
    iVecD v_array[SIZE];
} Data;

void check_alloc(const void *data);
void get_data(Data *data);
void print_data(const Data *data);
int compare_top_down(const void *a, const void *b);
int compare_bot_up(const void *a, const void *b);
int compare_x(const void *a, const void *b);
int compare_y(const void *a, const void *b);
uint64_t calc_area(const iVecD a, const iVecD b);
iVecD* get_sorted_data(const Data *data, int (* _Nonnull compar)(const void *, const void *));
int get_next_direction(const iVecD a, const iVecD b);
static inline bool check_collision(iVecD i1, iVecD i2, iVecD i3);
iVecD lerp_half(iVecD a, iVecD b);

void part_one(Data *data, iVecD *top_down, iVecD *bot_up);
uint64_t part_two(iVecD *data, int64_t n);

int main(void)
{
    Data *data = calloc(1, sizeof(*data)); //BORROW:data
    check_alloc(data);

    get_data(data);
    print_data(data);

    iVecD *top_down = get_sorted_data(data, compare_top_down); //BORROW:top_down
    iVecD *bot_up = get_sorted_data(data, compare_bot_up); //BORROW:bot_up
    //
    // part_one(data, top_down, bot_up);


    int64_t result = part_two(data->v_array, data->size);


    free(top_down); //FREE:top_down
    free(bot_up); //FREE:bot_up

    printf("Result: %lld\n", result);

    free(data); //FREE:data
    return 0;
}


iVecD lerp_half(iVecD a, iVecD b)
{
    double minx = MIN(a.x, b.x);
    double maxx = MAX(a.x, b.x);
    double miny = MIN(a.y, b.y);
    double maxy = MAX(a.y, b.y);
    iVecD c = {0};
    c.x = (float)minx + 0.5f * (float)(maxx - minx);
    c.y = (float)miny + 0.5f * (float)(maxy - miny);
    return c;
}

static inline iVecD check_adjacent(iVecD i1, iVecD i2)
{
    if (i1.x == i2.x) {
        if (i1.y < i2.y) {
            return (iVecD){.x = i1.x, .y = i1.y+1};
        }
        else {
            return (iVecD){.x = i1.x, .y = i1.y-1};
        }
    }
    else if (i1.y == i2.y) {
        if (i1.x < i2.x) {
            return (iVecD){.x = i1.x +1, .y = i1.y};
        }
        else {
            return (iVecD){.x = i1.x-1, .y = i1.y};
        }
    }
    else {
        perror("[ERROR] while checking adjacent points\n");
        return (iVecD){0};
    }
}
// modulo ((a % b) + b) % b

int check_path_right(iVecD *data, iVecD i1, iVecD i2, int64_t n, int64_t size)
{
    int64_t n2 = (n + 1) % size;
    int64_t n3 = (((n - 1) % size) + size) % size;
    if (data[n].x == i2.x && data[n].y == i2.y) {
        return 1;
    }
    if (check_collision(i1, i2, lerp_half(data[n], data[n2])) ||
            check_collision(i1, i2, lerp_half(data[n], data[n3])) ||
             check_collision(i1, i2, data[n])) {
        return 0;
    }
    return check_path_right(data, i1, i2, n2, size);
}

int check_path_left(iVecD *data, iVecD i1, iVecD i2, int64_t n, int64_t size)
{
    int64_t n2 = (((n - 1) % size) + size) % size;
    int64_t n3 = (n + 1) % size;
    if (data[n].x == i2.x && data[n].y == i2.y) {
        return 1;
    }
    if (check_collision(i1, i2, check_adjacent(data[n], data[n2])) ||
            check_collision(i1, i2, check_adjacent(data[n], data[n3])) ||
            check_collision(i1, i2, data[n])) {
        return 0;
    }
    return check_path_left(data, i1, i2, n2, size);
}

static inline bool check_collision(iVecD i1, iVecD i2, iVecD i3)
{
    // printf("i3: %d,%d\n", i3.x, i3.y);
    if (i3.x > MIN(i1.x, i2.x) && i3.x < MAX(i1.x, i2.x) && i3.y > MIN(i1.y, i2.y) && i3.y < MAX(i1.y, i2.y)) {

        return true;
    }
    return false;
}

uint64_t part_two(iVecD *data, int64_t n)
{
    uint64_t result = 0;
    for (int64_t i = 0; i < n -1; i++)
    {
        for (int64_t j = i+1; j < n; j++)
        {
            if (check_path_right(data, data[i], data[j], i+1, n) &&
                    check_path_left(data, data[i], data[j], (((i - 1) % n) + n) % n, n)) {
                uint64_t area = calc_area(data[i], data[j]);
                // printf("points: %d,%d | %d,%d = %lld\n",
                //         sorted_d[i].x, sorted_d[i].y, sorted_d[j].x, sorted_d[j].y, area);
                result = MAX(result, area);
            }
        }
    }
    printf("Result: %llu\n", result);
    return result;
}

int get_next_direction(const iVecD a, const iVecD b)
{
    if (a.x == b.x) {
        if (a.y - b.y > 0) return UP;
        else return DOWN;
    }
    else if (a.y == b.y) {
        if (a.x - b.x > 0) return LEFT;
        else return RIGHT;
    }
    return -1;
}

void part_one(Data *data, iVecD *top_down, iVecD *bot_up)
{
    int64_t cap = data->size;
    int64_t result = 0;

    for (int64_t i = 0; i < 2; i++) {
        for (int64_t j = 1; j < 3; j++) {
            int64_t area_td = calc_area(top_down[i], top_down[cap-j]);
            int64_t area_bu = calc_area(bot_up[i], bot_up[cap-j]);
            if (area_td > result) result = area_td;
            else if (area_bu > result) result = area_bu;
        }
    }
    printf("Area: %lld\n", result);
}

iVecD* get_sorted_data(const Data *data, int (* _Nonnull compar)(const void *, const void *))
{
    iVecD *s_data = calloc(data->size, sizeof(*s_data)); //BORROW!
    check_alloc(s_data);
    memcpy(s_data, &data->v_array, data->size * sizeof(data->v_array[0]));
    qsort(s_data, data->size, sizeof(s_data[0]), compar);

    // for (int64_t i = 0; i < data->size; i++) {
    //     printf("%d,%d\n", s_data[i].x, s_data[i].y);
    // }
    // printf("\n");

    return s_data;
}

uint64_t calc_area(const iVecD a, const iVecD b)
{
    uint64_t w = MAX(a.x, b.x) - MIN(a.x, b.x) + 1;
    uint64_t h = MAX(a.y, b.y) - MIN(a.y, b.y) + 1;

    // printf("%lld * %lld = %lld\n", w, h, w*h);

    return w * h;
}

int compare_top_down(const void *a, const void *b)
{
    iVecD vec_a = *((const iVecD*)a);
    iVecD vec_b = *((const iVecD*)b);

    if (vec_a.x + vec_a.y > vec_b.x + vec_b.y) return 1;
    if (vec_a.x + vec_a.y < vec_b.x + vec_b.y) return -1;
    return 0;
}

int compare_bot_up(const void *a, const void *b)
{
    iVecD vec_a = *((const iVecD*)a);
    iVecD vec_b = *((const iVecD*)b);

    if (vec_a.x - vec_a.y > vec_b.x - vec_b.y) return 1;
    if (vec_a.x - vec_a.y < vec_b.x - vec_b.y) return -1;
    return 0;
}

int compare_x(const void *a, const void *b)
{
    iVecD vec_a = *((const iVecD*)a);
    iVecD vec_b = *((const iVecD*)b);

    if (vec_a.x > vec_b.x) return 1;
    if (vec_a.x < vec_b.x) return -1;
    return 0;
}

int compare_y(const void *a, const void *b)
{
    iVecD vec_a = *((const iVecD*)a);
    iVecD vec_b = *((const iVecD*)b);

    if (vec_a.y > vec_b.y) return 1;
    if (vec_a.y < vec_b.y) return -1;
    return 0;
}

void check_alloc(const void *data)
{
    if (!data) {
        perror("failed to calloc data\n");
        exit(EXIT_FAILURE);
    }
}

void get_data(Data *data)
{
    FILE *fp = fopen("data.txt", "r"); //BORROW:fp

    int64_t count = 0;
    uint32_t index = 0;
    int i1, i2;
    while ((fscanf(fp, "%d,%d", &i1, &i2)) == 2)
    {
        data->v_array[count++] = (iVecD){.index = index++, .x = i1, .y = i2};
    }
    data->size = count;
    fclose(fp); //FREE:fp
}

void print_data(const Data *data)
{
    for (int64_t i = 0; i < data->size; i++) {
        printf("i%d: %d,%d\n", data->v_array[i].index, data->v_array[i].x, data->v_array[i].y);
    }
    printf("Size: %lld\n", data->size);
}
