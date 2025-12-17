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

void get_data(Data *data);
void print_data(const Data *data);
size_t part_one(Data *data);

int main(void)
{
    Data data = {0};

    get_data(&data);
    print_data(&data);

    size_t result = part_one(&data);
    printf("Result: %zu\n", result);

    return 0;
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

void get_data(Data *data)
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
