#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int16_t  i16;
typedef int32_t  i32;

typedef uint16_t u16;
typedef uint32_t u32;

typedef struct {
    u32 base;
    u32 base_size;
    u32 input[32];
    u32 input_count;
    u32 joltage[32];
} Items;

typedef struct {
    u32 count;
    Items *item;
} Data;

void print_bits_u32(u32 num);
void get_data(Data *dt);

int main(void)
{
    u32 test = 15;
    u32 test2 = 0;
    test2 |= (1<<1);
    test ^= test2;

    print_bits_u32(test);

    Data data = {0};
    data.item = calloc(1024, sizeof(*data.item));

    get_data(&data);

    print_bits_u32(data.item[0].base);

    return 0;
}

void get_data(Data *dt)
{
    FILE *fp = fopen("data.txt", "r");
    int c;
    char buf[8] = {0};
    u32 count = 0;

    u32 item = 0;
    u32 input_count = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        switch (item) {
            case 0:
                if (c == ']') {
                    for (i32 i = 0; i < (i32)count; i++) {
                        if (buf[i] == '#') dt->item[dt->count].base |= (1<<i);
                    }
                    dt->item[dt->count].base_size = count;
                    count = 0;
                    item++;
                }
                else if (c == '.' || c == '#') buf[count++] = c;
                break;
            //------------------------------------------
            case 1:
                if (c == '{') {
                    item++;
                    break;
                }
                if (isdigit(c)) buf[count++] = c;
                else if (c == ')'){
                    for (i32 i = 0; i < (i32)count; i++) {
                        dt->item[dt->count].input[input_count] |= (1<<i);
                    }
                    dt->item[dt->count].input_count = ++input_count;
                    count = 0;
                }
                break;
            //------------------------------------------
            case 2:
                break;
        }

    }
    buf[count] = '\n';
    fclose(fp);
}

void print_bits_u32(u32 num)
{
    for (i32 i = sizeof(i)*8-1; i >= 0; i--)
    {
        printf("%dt", (num >> i) & 1);
        if (i % (sizeof(i)*8/2) == 0) printf(" ");
    }
    printf("\n");
}
