#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int width;
    int height;
    char array[512][512];
} Data;

void get_data(Data *data);
void print_data(const Data data);
bool check_data_one(Data *data, int wi, int hi);
bool check_data_two(Data *data, int wi, int hi);
int part_two(Data *data);

int main(void)
{

    Data data = {0};

    get_data(&data);
    print_data(data);

    int count = 0;
    int buffer;

    while ((buffer = part_two(&data)) > 0)
    {
        count += buffer;
    }



    print_data(data);
    printf("Result: %d\n", count);

    return 0;
}

int part_two(Data *data)
{
    int count = 0;
    for (int i = 0; i < data->height; i++)
    {
        for (int j = 0; j < data->width; j++)
        {
            if (check_data_two(data, j, i)) 
            {
                count++;
            }
        }
    }
    return count;
}

bool check_data_two(Data *data, int wi, int hi)
{
    if (data->array[hi][wi] != '@') return false;
    int count = 0;
    for (int i = (hi - 1); i <= (hi + 1); i++)
    {
        for (int j = (wi - 1); j <= (wi + 1); j++)
        {
            if (i == hi && j == wi) continue;
            if (i < 0 || i >= data->height || j < 0 || j >= data->width) {
                continue;
            }
            if (data->array[i][j] == '@') {
                count++;
            }
            if (count >= 4) return false;
        }
    }
    data->array[hi][wi] = 'x';
    return true;
}

bool check_data_one(Data *data, int wi, int hi)
{
    if (data->array[hi][wi] != '@') return false;
    int count = 0;
    for (int i = (hi - 1); i <= (hi + 1); i++)
    {
        for (int j = (wi - 1); j <= (wi + 1); j++)
        {
            if (i == hi && j == wi) continue;
            if (i < 0 || i >= data->height || j < 0 || j >= data->width) {
                continue;
            }
            if (data->array[i][j] == '@' || data->array[i][j] == 'x') {
                count++;
            }
            if (count >= 4) return false;
        }
    }
    data->array[hi][wi] = 'x';
    return true;
}

void get_data(Data *data)
{
    FILE *fp = fopen("data.txt", "r");
    int width_n = 0;
    int c;
    bool got_width = false;

    while ((c = fgetc(fp)) != EOF) 
    {
        if (c == '\n') {
            data->array[data->height][width_n] = '\0';
            data->height++;
            if (!got_width) {
                data->width = width_n;
                got_width = true;
            }
            width_n = 0;
        }
        else {
            data->array[data->height][width_n] = c;
            width_n++;
        }
    }
    fclose(fp);
}

void print_data(const Data data)
{
    for (int i = 0; i < data.height; i++) 
    {
        printf("%s\n", data.array[i]);
    }
    printf("Width: %d\n", data.width);
    printf("Height: %d\n", data.height);
    printf("\n");
}
