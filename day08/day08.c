#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024
#define TARGET 1000

typedef double f64;

typedef struct {
    int x;
    int y;
    int z;
} Coord;

typedef struct {
    Coord coord[SIZE];
    int size;
} Data;

typedef struct {
    f64 distance;
    int i1;
    int i2;
} Distance;

typedef struct {
    uint32_t size;
    Distance *array;
} DistArray;


typedef struct Circuit {
    uint32_t size;
    uint32_t array[SIZE];
    bool removed;
} Circuit;

typedef struct {
    uint32_t c_count;
    uint32_t total;
    uint32_t buffer[SIZE];
    uint32_t circuit[SIZE];
} CircuitGroups;

//prototypes
void get_data(Data *data);
void print_data(const Data *data);
f64 calc_distance(Coord c1, Coord c2);
void print_coord(Coord coord);
int compare_data_values(const void *a, const void *b);
void part_one(const DistArray *dist, CircuitGroups *cg);
int compare_results(const void *a, const void *b);

int main(void)
{
    Data data = {0};

    get_data(&data);
    print_data(&data);

    DistArray dist = {0};
    dist.array = calloc(SIZE * SIZE, sizeof(*dist.array));
    if (!dist.array) {
        perror("failed to alloc mem for Distance struct\n");
        return 1;
    }

    for (int i = 0; i < data.size - 1; i++)
    {
        for (int j = i + 1; j < data.size; j++)
        {
            dist.array[dist.size].distance = calc_distance(data.coord[i], data.coord[j]);
            dist.array[dist.size].i1 = i;
            dist.array[dist.size].i2 = j;
            dist.size++;
        }
    }

    qsort(dist.array, dist.size, sizeof(dist.array[0]), compare_data_values);

    for (size_t i = 0; i < dist.size; i++)
    {
        printf("%d,%d\n", dist.array[i].i1, dist.array[i].i2);
    }
    printf("Size: %d\n", dist.size);

    CircuitGroups *cg = calloc(1, sizeof(*cg));

    part_one(&dist, cg);
    //
    qsort(cg->circuit, TARGET+1, sizeof(cg->circuit[0]), compare_results);

    for (int i = 0; i < TARGET+1; i++)
    {
        printf("%d\n", cg->circuit[i]);
    }

    return 0;
}

void part_one(const DistArray *dist, CircuitGroups *cg)
{
    cg->c_count = 1;
    cg->buffer[dist->array[0].i1] = cg->c_count;
    cg->buffer[dist->array[0].i2] = cg->c_count;
    // cg->total++;
    for (uint32_t i = 1; i < TARGET+1; i++)
    {
        uint32_t i1 = dist->array[i].i1;
        uint32_t i2 = dist->array[i].i2;
        uint32_t c1 = cg->buffer[i1];
        uint32_t c2 = cg->buffer[i2];
        if (c1 && c2) {
            if (c1 == c2) continue;
            for (uint32_t j = 0; j < dist->size; j++)
            {
                if (cg->buffer[j] == c2) {
                    cg->buffer[j] = c1;
                }
            }
        }
        else if (c1) {
            cg->buffer[i2] = c1;
        }
        else if (c2) {
            cg->buffer[i1] = c2;
        }
        else {
            cg->c_count++;
            cg->buffer[i1] = cg->c_count;
            cg->buffer[i2] = cg->c_count;
        }
    }

    for (uint32_t i = 0; i < TARGET+1; i++)
    {
        if (cg->buffer[i]) {
            // printf("%d\n", cg->buffer[i]);
            cg->circuit[cg->buffer[i]]++;
        }
    }
    // for (size_t i = 0; i <= cg->c_count; i++)
    // {
    //         printf("%d\n", cg->circuit[i]);
    // }
}

int compare_results(const void *a, const void *b)
{
    uint32_t cg_a = *((const uint32_t*)a);
    uint32_t cg_b = *((const uint32_t*)b);

    if (cg_a < cg_b) return -1;
    if (cg_a > cg_b) return 1;
    return 0;
}

int compare_data_values(const void *a, const void *b)
{
    Distance dist_a = *((const Distance*)a);
    Distance dist_b = *((const Distance*)b);

    if (dist_a.distance < dist_b.distance) return -1;
    if (dist_a.distance > dist_b.distance) return 1;
    return 0;
}

f64 calc_distance(Coord c1, Coord c2)
{
    int64_t r1 = (c1.x - c2.x);
    int64_t r2 = (c1.y - c2.y);
    int64_t r3 = (c1.z - c2.z);
    f64 result = r1*r1 + r2*r2 + r3*r3;
    return sqrtl(result);
}

void get_data(Data *data)
{
    FILE *fp = fopen("data.txt", "r");
    int num1, num2, num3;
    num1 = num2 = num3 = 0;
    if (fp) {
        while ((fscanf(fp, "%d,%d,%d", &num1, &num2, &num3)) == 3)
        {
            data->coord[data->size++] = (Coord){.x = num1, .y = num2, .z = num3};
        }
        fclose(fp);
        return;
    }
    perror("Failed to open data.txt\n");
}

void print_data(const Data *data)
{
    for (int i = 0; i < data->size; i++)
    {
        printf("%d %d %d\n", data->coord[i].x, data->coord[i].y, data->coord[i].z);
    }
    printf("Size: %d\n", data->size);
}

void print_coord(Coord coord)
{
    printf("%d,%d,%d\n", coord.x, coord.y, coord.z);
}
