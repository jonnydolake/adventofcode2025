#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256

typedef struct {
    char batteries[SIZE][SIZE];
    int count;
} Data;

typedef struct {
    int position1;
    int position2;
    char num_s[16];
} Battery;

void get_data(Data *data);
uint64_t part_one(const Data data, Battery *batt);
uint64_t part_two(const Data data, Battery *batt);

int main(void)
{
    Data data = {0};
    get_data(&data);

    Battery battery[SIZE] = {0};

    uint64_t result = part_two(data, battery);

    printf("Result: %llu\n", result);
    printf("Long: %zu\n", sizeof(long));

    return 0;
}

uint64_t part_two(const Data data, Battery *batt)
{
    uint64_t result = 0;

    for(int i = 0; i < data.count; i++)
    {
        int len = strlen(data.batteries[i]);
        printf("len: %d\n", len);
        int b_len = 12;
        int position = 0;
        for (int a = 0; a < 12; a++)
        {
            for (int j = position; j < (len - b_len); j++)
            {
                if (data.batteries[i][j] > batt[i].num_s[a]) {
                    batt[i].num_s[a] = data.batteries[i][j];
                    position = j + 1;
                }
                if (batt[i].num_s[a] == 9) break;
            }
            b_len--;
        }
        batt[i].num_s[12] = '\0';
        result += atol(batt[i].num_s);
        printf("Battery: %s\n", batt[i].num_s);
    }

    return result;
}

uint64_t part_one(const Data data, Battery *batt)
{
    uint64_t result = 0;

    for(int i = 0; i < data.count; i++)
    {
        int len = strlen(data.batteries[i]);
        for (int j = 0; j < (len - 2); j++)
        {
            if (data.batteries[i][j] > batt[i].num_s[0]) {
                batt[i].num_s[0] = data.batteries[i][j];
                batt[i].position1 = j;
            }
            if (batt[i].num_s[0] == 9) break;
        }
        for (int j = batt[i].position1 + 1; j < len - 1; j++)
        {
            if (data.batteries[i][j] > batt[i].num_s[1]) {
                batt[i].num_s[1] = data.batteries[i][j];
                batt[i].position2 = j;
            }
            if (batt[i].num_s[1] == 9) break;
        }
        batt[i].num_s[2] = '\0';
        result += atoi(batt[i].num_s);
        printf("Battery: %s\n", batt[i].num_s);
    }
    return result;
}

void get_data(Data *data)
{
    FILE *fp = fopen("data.txt", "r");

    while (fgets(data->batteries[data->count], SIZE, fp))
    {
        // printf("%s", data->batteries[data->count]);
        data->count++;
    }

    printf("Count: %d\n", data->count);

    fclose(fp);
}
