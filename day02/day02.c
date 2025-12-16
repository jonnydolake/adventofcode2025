#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t start;
    size_t end;
} Pairs;

typedef struct {
    Pairs pairs[128];
    int count;
} Data;

typedef struct {
    int nums[128];
    int count;
} Digits;

void get_data(Data *data);
void get_digits(Digits *dig, size_t i);
int check_digits_one(const Digits dig);
int check_digits_two(Digits dig);

int main(void)
{
    Data data = {0};
    get_data(&data);

    Digits digits = {0};
    size_t result = 0;
    //print data
    for (int i = 0; i < data.count; i++)
    {
        for(size_t j = data.pairs[i].start; j <= data.pairs[i].end; j++) 
        {
            get_digits(&digits, j);
            if (check_digits_two(digits)) {
                printf("Invalid ID: %zu\n", j);
                result += j;
            }
            digits.count = 0;
        }
        // printf("start value: %zu\n", data.pairs[i].start);
        // printf("end value: %zu\n", data.pairs[i].end);
    }
    // printf("size: %d\n", data.count);
    //-------------------------

    // TODO: below, use the data

    printf("Result: %zu\n", result);

    return 0;
}

int check_digits_two(Digits dig)
{
    int half = dig.count * 0.5f;
    long n_check = 0;
    for (int i = half; i > 0; i--)
    {
        if (dig.count % i == 0) {
            int split = dig.count / i;
            for (int j = 0; j < i; j++)
            {
                for (int k = 1; k < split; k++)
                {
                    if (dig.nums[j] != dig.nums[i*k + j]) n_check++;
                }
            }
            if (n_check == 0) return 1;
            n_check = 0;
        }
    }
    return 0;
}

int check_digits_one(Digits dig)
{
    if (dig.count % 2 != 0) return 0;
    int half = dig.count * 0.5f;

    for (int i = 0; i < half; i++) 
    {
        if (dig.nums[i] != dig.nums[half + i]) return 0;
    }
    return 1;
}

void get_digits(Digits *dig, size_t i)
{
    while (i > 0) {
        dig->nums[dig->count++] = i%10;
        i /= 10;
    }
}

void get_data(Data *data)
{
    FILE *fp = fopen("data.txt", "r");

    size_t c_count = 0;
    int c;
    char start[256];
    char end[256];

    int swtch = 1;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '-') {
            swtch = 2;
            start[c_count] = '\0';
            data->pairs[data->count].start = atol(start);
            c_count = 0;
            continue;
        }
        if (c == ',') {
            swtch = 1;
            end[c_count] = '\0';
            data->pairs[data->count++].end = atol(end);
            c_count = 0;
            continue;
        }

        if (swtch == 1) start[c_count++] = c;
        else end[c_count++] = c;
    }
    end[c_count] = '\0';
    data->pairs[data->count++].end = atol(end);

    fclose(fp);
}
