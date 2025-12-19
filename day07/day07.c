#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

typedef struct {
    int width;
    int height;
    char matrix[SIZE][SIZE];
} Data;

// typedef struct Node {
//     int x;
//     int y;
//     struct Node *next;
// } Node;
//
// typedef struct {
//     int size;
//     Node *head;
// } TreeStack;

void get_data(Data *data);
void print_data(Data *data);
int get_start_index(Data *data);
int get_tree1(const Data *data, uint8_t buffer[SIZE][SIZE], int x, int y);
int get_tree2(const Data *data, uint8_t buffer[SIZE][SIZE], int x, int y);

int main(void)
{
    Data *data = calloc(1, sizeof(*data)); 
    if (!data) {
        printf("failed to allocate data\n");
        return 1;
    }

    get_data(data);
    print_data(data);

    int start;
    if ((start = get_start_index(data)) == -1) return 1;
    printf("Start: %d\n", start);
    uint8_t (*buffer)[SIZE] = calloc(SIZE, sizeof(*buffer));

    int result = get_tree2(data, buffer, start, 0);

    printf("Result: %d\n", result);

    return 0;
}

int get_tree2(const Data *data, uint8_t buffer[SIZE][SIZE], int x, int y)
{
    if (y == data->height) {
        return 1;
    }
    if (buffer[y][x] != 0) {
        buffer[y][x]++;
        return buffer[y][x] + buffer[y][x];
    }
    if (data->matrix[y][x] == '^') {
        return get_tree2(data, buffer, x -1, y + 1) + get_tree2(data, buffer, x+1, y+1);
        buffer[y][x] += 2;
    }
    else {
        return get_tree2(data, buffer, x, y + 1);
    }
}

int get_tree1(const Data *data, uint8_t buffer[SIZE][SIZE], int x, int y)
{
    if (y == data->height || buffer[y][x] == 1) {
        return 0;
    }
    if (data->matrix[y][x] == '^') {
        buffer[y][x] = 1;
        return 1 + get_tree1(data, buffer, x -1, y + 1) + get_tree1(data, buffer, x+1, y+1);
    }
    else {
        return get_tree1(data, buffer, x, y + 1);
    }
}

int get_start_index(Data *data)
{
    for (int i = 0; i < data->width; i++)
    {
        if (data->matrix[0][i] == 'S') return i;
    }
    printf("failed to find START index\n");
    return -1;
}

void get_data(Data *data)
{
    FILE *fp = fopen("data.txt", "r");
    int c;
    int width = 0;
    bool got_width = false;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n') {
            data->matrix[data->height++][width] = '\0';
            if (!got_width) {
                data->width = width;
                got_width = true;
            }
            width = 0;
            continue;
        }
        data->matrix[data->height][width++] = c;
    }
    fclose(fp);
}

void print_data(Data *data)
{
    for (int i = 0; i < data->height; i++)
    {
        printf("%s\n", data->matrix[i]);
    }
    printf("Width: %d\n", data->width);
    printf("Height: %d\n", data->height);
}

// void stack_pop(TreeStack *stack)
// {
//     if (stack->size == 0) {
//         printf("ERROR: nothing to pop, stack size is zero\n");
//         return;
//     }
//     else {
//         Node *temp = stack->head;
//         stack->head = stack->head->next;
//         free(temp);
//         return;
//     }
// }
//
// void stack_push(TreeStack *stack, int x, int y)
// {
//     if (stack->head == NULL) {
//         stack->head = malloc(sizeof(*stack->head));
//         if (!stack->head) {
//             printf("failed to allocate memory for node\n");
//             return;
//         }
//         stack->head->x = x;
//         stack->head->y = y;
//         stack->head->next = NULL;
//         stack->size++;
//         return;
//     }
//     else {
//         Node *temp = malloc(sizeof(*temp));
//         if (!temp) {
//             printf("failed to allocate memory for node\n");
//             return;
//         }
//         temp->x = x;
//         temp->y = y;
//         temp->next = stack->head;
//         stack->head = temp;
//         stack->size++;
//     }
// }
