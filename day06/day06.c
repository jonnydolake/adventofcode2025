#include <complex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int width;
    int height;
    int matrix[8][1024];
    char signs[1024];
} Data;

typedef struct {
    int width[1024];
    int height;
    int matrix[1024][8];
    char signs[1024];
} DataTwo ;

typedef struct {
    int width;
    int height;
    char data[8][4000];
} InputData;

void get_data_one(Data *data);
void get_data_two(InputData *data);
void print_data(const Data *data);
size_t part_one(Data *data);
void part_two_sort(InputData *id, DataTwo *data);
size_t part_two(DataTwo *data);


int main(void)
{
    // Data data = {0};
    DataTwo data = {0};
    InputData i_data = {0};

    // get_data_one(&data);
    get_data_two(&i_data);
    // print_data(&data);

    for (int i = 0; i < i_data.height; i++)
    {
        printf("%s\n", i_data.data[i]);
    }
    printf("\n");

    part_two_sort(&i_data, &data);

    for (int i = 0; i < data.height; i++)
    {
        for (int j = 0; j < data.width[i]; j++)
        {
            printf("%d ", data.matrix[i][j]);
        }
        printf("%c\n", data.signs[i]);
    }
    // printf("Width: %d\n", data.width);
    printf("Height: %d\n", data.height);

    // size_t result = part_one(&data);
    size_t result = part_two(&data);
    printf("Result: %zu\n", result);

    return 0;
}
size_t part_two(DataTwo *data)
{
    size_t result = 0;

    for (int i = 0; i < data->height; i++)
    {
        size_t temp = data->matrix[i][0];
        for (int j = 1; j < data->width[i]; j++)
        {
            if (data->signs[i] == '+') {
                temp += data->matrix[i][j];
            }
            else if (data->signs[i] == '*') {
                temp *= data->matrix[i][j];
            }
        }
        result += temp;
    }
    return result;
}

void part_two_sort(InputData *id, DataTwo *data)
{
    size_t result = 0;
    char num[8] = {0};
    int count = 0;
    int width = 0;

    for (int i = id->width -1; i >= 0; i--)
    {
        for (int j = 0; j < id->height; j++)
        {
            if (id->data[j][i] == ' ') {
                if (count) {
                    num[count] = '\0';
                    data->matrix[data->height][width++] = atoi(num);
                    count = 0;
                }
                continue;
            }
            if (id->data[j][i] == '+' || id->data[j][i] == '*') {
                if (count) {
                    num[count] = '\0';
                    data->matrix[data->height][width++] = atoi(num);
                    count = 0;
                }
                data->signs[data->height] = id->data[j][i];
                data->width[data->height++] = width;
                width = 0;
                if (i) i--;
                continue;
            }
            num[count++] = id->data[j][i];
        }
    }
}

void get_data_two(InputData *data)
{
    FILE *fp = fopen("data.txt", "r");
    int c;
    int width = 0;
    bool got_width = false;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n') {
            data->data[data->height++][width] = '\0';
            if (!got_width) {
                data->width = width;
                got_width = true;
            }
            width = 0;
            continue;
        }
        data->data[data->height][width++] = c;
    }
    fclose(fp);
}

size_t part_one(Data *data)
{
    size_t result = 0;
    for (int i = 0; i < data->width; i++)
    {
        size_t temp = data->matrix[0][i];
        for (int j = 1; j < data->height; j++)
        {
            if (data->signs[i] == '+') {
                temp += data->matrix[j][i];
            }
            else if (data->signs[i] == '*') {
                temp *= data->matrix[j][i];
            }
        }
        result += temp;
    }
    return result;
}

void get_data_one(Data *data)
{
    FILE *fp = fopen("data.txt", "r");
    int c;
    char num[16];
    int count = 0;
    int width = 0;
    bool end_of_num = false;

    while ((c = fgetc(fp)) != EOF)
    {
        if (!end_of_num && (c == '*' || c == '+')) {
            end_of_num = true;
        }
        if (c == ' ' && !end_of_num) {
            if (count) {
                num[count] = '\0';
                data->matrix[data->height][width++] = atoi(num);
                count = 0;
            }
            continue;
        }
        else if (c == '\n' && !end_of_num) {
            if (count) {
                num[count] = '\0';
                data->matrix[data->height++][width++] = atoi(num);
                data->width = width;
                count = 0;
                width = 0;
            }
            else {
                data->height++;
                width = 0;
            }
            continue;
        }
        else if (c == ' ') {
            continue;
        }

        if (end_of_num) {
            data->signs[width++] = c; 
            continue;
        }
        num[count++] = c;
    }
    data->signs[width] = '\0';
    fclose(fp);
}

void print_data(const Data *data)
{
    for (int i = 0; i < data->height; i++)
    {
        for (int j = 0; j < data->width; j++)
        {
            printf("%d ", data->matrix[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < data->width; i++)
    {
        printf("%c ", data->signs[i]);
    }
    printf("\n");
    printf("width: %d\n", data->width);
    printf("height: %d\n", data->height);
}
