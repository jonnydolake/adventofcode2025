//ugly fucking code, but it worked
//i need to do better tho...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void)
{
    int position = 50;
    char s[8];
    int count = 0;

    FILE *file = fopen("rotations.txt", "r");
    while(fgets(s, sizeof(s), file))
    {
        printf("Current position: %d\n%d", position, position);
        printf(" + %s", s);
        char sn[8];
        for(int i = 1; s[i] != '\0'; i++) {
            sn[i-1] = s[i];
            if(s[i + 1] == '\0') sn[i] = '\0';
        }
        int n = atoi(sn);
        bool pos_zero = position == 0;
        printf("(%d) ", n);

        while(n >= 100) {
            n -= 100;
            count++;
        }

        if(s[0] == 'R') {
            position += n;
        }
        else if(s[0] == 'L') {
            position -= n;
        }

        if(position == 0) count++;
        while (position < 0 || position >= 100)
        {
            if(position < 0) {
                position = 100 + position;
                if (!pos_zero) count++;
            }
            else if(position >= 100) {
                position = position - 100;
                if (!pos_zero) count++;
            }
        }
        printf("= %d\n", position);
        printf("Count: %d\n", count);

    }
    printf("%d\n", count);

    return 0;
}
