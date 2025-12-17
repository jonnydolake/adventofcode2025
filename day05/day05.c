#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint64_t uint64 ;

typedef struct {
    uint64 start;
    uint64 end;
} Range;

typedef struct {
    size_t count;
    Range range[512];
} DataRanges;

typedef struct {
    size_t count;
    uint64 array[1024];
} Data;

void get_data(DataRanges *values, Data *data);
void print_data(DataRanges values, Data data, bool prt_val, bool prt_data);
int compare_values(const void *a, const void *b);
void organize_values(DataRanges *values);

int main(void)
{

    DataRanges values = {0};
    Data data = {0};

    get_data(&values, &data);

    qsort(values.range, values.count, sizeof(Range), compare_values);
    print_data(values, data, true, false);
    organize_values(&values);

    size_t result = 0;
    for (size_t i = 0; i < data.count; i++)
    {
        for (size_t j = 0; j < values.count; j++)
        {
            if (data.array[i] >= values.range[j].start && data.array[i] <= values.range[j].end) {
                result++;
                break;
            }
        }
    }

    size_t result2 = 0;

    for (size_t i = 0; i < values.count; i++)
    {
        result2 += (values.range[i].end - values.range[i].start + 1);
    }

    print_data(values, data, true, false);
    printf("Result: %zu\n", result);
    printf("Result2: %zu\n", result2);

    return 0;
}

void organize_values(DataRanges *values)
{
    DataRanges new_values = {0};
    // *values = new_values;
    new_values.range[new_values.count++] = values->range[0];
    for (size_t i = new_values.count; i < values->count; i++)
    {
        if (values->range[i].start <= values->range[i-1].end) {
            if (values->range[i-1].end <= values->range[i].end) {
                new_values.range[new_values.count-1].end = values->range[i].end;
            }
        }
        else {
            new_values.range[new_values.count++] = values->range[i];
        }
    }
    printf("New_values: %zu\n", new_values.count);
    *values = new_values;
}

int compare_values(const void *a, const void *b)
{
    Range range_a = *((const Range*)a);
    Range range_b = *((const Range*)b);

    if (range_a.start < range_b.start) return -1;
    if (range_a.start > range_b.start) return 1;
    return 0;
}

void get_data(DataRanges *values, Data *data)
{
    FILE *fp = fopen("data.txt", "r");

    int c;
    int count = 0;
    char num[32];

    bool new_line = true;
    bool split = false;

    while ((c = fgetc(fp)) != EOF)
    {
        if (new_line) {
            if (c == '\n') {
                split = true;
                continue;
            }
            else new_line = false;
        }

        if (c == '\n') {
            if (!split) {
                num[count] = '\0';
                new_line = true;
                values->range[values->count++].end = atoll(num);
                count = 0;
                // first = true;
                continue;
            }
            else {
                num[count] = '\0';
                new_line = true;
                data->array[data->count++] = atoll(num);
                count = 0;
                continue;
            }
        }
        else if (c == '-') {
            num[count] = '\0';
            values->range[values->count].start = atoll(num);
            count = 0;
            continue;
            // first = false; 
        }
        else {
            new_line = false;
            num[count++] = c;
        }
    }
    fclose(fp);
}

void print_data(DataRanges values, Data data, bool prt_val, bool prt_data)
{
    if (prt_val) {
        for (size_t i = 0; i < values.count; i++)
        {
            printf("Start: %llu | End: %llu\n", values.range[i].start, values.range[i].end);
        }
    }
    if (prt_data) {
        for (size_t i = 0; i < data.count; i++)
        {
            printf("%llu\n", data.array[i]);
        }
    }
    printf("Values count: %zu\n", values.count);
    printf("Data count: %zu\n", data.count);
}
