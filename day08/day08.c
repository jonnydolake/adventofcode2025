#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define SIZE 1024

typedef double f64;

typedef struct {
    int x;
    int y;
    int z;
} Coord;

typedef struct {
    size_t size;
    Coord coord[SIZE];
} Data;

typedef struct {
    Coord coord1;
    Coord coord2;
    f64 distance;
} Distance;

//prototypes
void get_data(Data *data);
void print_data(const Data *data);
f64 calc_distance(Coord c1, Coord c2);
void print_coord(Coord coord);

int main(void)
{
    Data data = {0};

    get_data(&data);
    print_data(&data);

    f64 temp = 1000000;
    f64 temp_r = 1000000;
    size_t index = 0;
    for (size_t i = 1; i < data.size; i++)
    {
        if ((temp_r = calc_distance(data.coord[0], data.coord[i])) < temp) {
            temp = temp_r;
            index = i;
        }
    }
    printf("test result: %lf\n", temp);
    print_coord(data.coord[0]);
    print_coord(data.coord[index]);

    return 0;
}

void part_one(const Data *data)
{

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
    for (size_t i = 0; i < data->size; i++)
    {
        printf("%d %d %d\n", data->coord[i].x, data->coord[i].y, data->coord[i].z);
    }
    printf("Size: %zu\n", data->size);
}

void print_coord(Coord coord)
{
    printf("%d,%d,%d\n", coord.x, coord.y, coord.z);
}
