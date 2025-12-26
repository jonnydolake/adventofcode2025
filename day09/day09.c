#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024
typedef struct {
    int x;
    int y;
} iVec2;

typedef struct {
    size_t size;
    iVec2 v_array[SIZE];
} Data;

void check_alloc(const void *data);
void get_data(Data *data);
void print_data(const Data *data);
int compare_top_down(const void *a, const void *b);
int compare_bot_up(const void *a, const void *b);
int64_t calc_area(const iVec2 a, const iVec2 b);

void part_one(Data *data);

int main(void)
{
    Data *data = calloc(1, sizeof(*data));
    check_alloc(data);

    get_data(data);
    // print_data(data);

    part_one(data);

    return 0;
}

void part_one(Data *data)
{
    size_t cap = data->size;
    iVec2 *top_down = calloc(data->size, sizeof(*top_down));
    check_alloc(top_down);
    memcpy(top_down, &data->v_array, data->size * sizeof(data->v_array[0]));

    iVec2 *bot_up = calloc(data->size, sizeof(*bot_up));
    check_alloc(bot_up);
    memcpy(bot_up, &data->v_array, data->size * sizeof(data->v_array[0]));

    qsort(top_down, data->size, sizeof(top_down[0]), compare_top_down);
    qsort(bot_up, data->size, sizeof(bot_up[0]), compare_bot_up);

    // for (size_t i = 0; i < data->size; i++)
    // {
    //     printf("%d,%d\n", top_down[i].x, top_down[i].y);
    // }
    // printf("\n");
    // for (size_t i = 0; i < data->size; i++)
    // {
    //     printf("%d,%d\n", bot_up[i].x, bot_up[i].y);
    // }

    int64_t result = 0;

    for (int64_t i = 0; i < 3; i++) {
        for (int64_t j = 1; j < 4; j++) {
            int64_t area_td = calc_area(top_down[i], top_down[cap-j]);
            int64_t area_bu = calc_area(bot_up[i], bot_up[cap-j]);
            if (area_td > result) result = area_td;
            else if (area_bu > result) result = area_bu;
        }
    }
    printf("Area: %lld\n", result);

    free(top_down);
    free(bot_up);
}

int64_t calc_area(const iVec2 a, const iVec2 b)
{
    int64_t w = abs(b.x - a.x + 1);
    int64_t h = abs(b.y - a.y + 1);

    // printf("%lld * %lld = %lld\n", w, h, w*h);

    return w * h;
}

int compare_top_down(const void *a, const void *b)
{
    iVec2 vec_a = *((const iVec2*)a);
    iVec2 vec_b = *((const iVec2*)b);

    if (vec_a.x + vec_a.y > vec_b.x + vec_b.y) return 1;
    if (vec_a.x + vec_a.y < vec_b.x + vec_b.y) return -1;
    return 0;
}

int compare_bot_up(const void *a, const void *b)
{
    iVec2 vec_a = *((const iVec2*)a);
    iVec2 vec_b = *((const iVec2*)b);

    if (vec_a.x - vec_a.y > vec_b.x - vec_b.y) return 1;
    if (vec_a.x - vec_a.y < vec_b.x - vec_b.y) return -1;
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
    FILE *fp = fopen("data.txt", "r");

    size_t count = 0;
    int i1, i2;
    while ((fscanf(fp, "%d,%d", &i1, &i2)) == 2)
    {
        data->v_array[count++] = (iVec2){.x = i1, .y = i2};
    }
    data->size = count;
    fclose(fp);
}

void print_data(const Data *data)
{
    for (size_t i = 0; i < data->size; i++) {
        printf("%d,%d\n", data->v_array[i].x, data->v_array[i].y);
    }
    printf("Size: %zu\n", data->size);
}
