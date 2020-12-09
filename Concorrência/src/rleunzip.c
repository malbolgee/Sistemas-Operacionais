#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    FILE *compacted_file = fopen(argv[1], "rb");
    
    char c;
    int i = 0;
    char num[6] = {0};

    while (fread(&c, sizeof(char), 1, compacted_file) == 1)
    {
        if (isdigit(c))
            num[i++] = c;
        else
        {

            i = 0;
            int intNum = strtol(num, NULL, 10);
            num[0] = 0;
            for (int count = 0; count < intNum; count++)
                putchar_unlocked(c);
        }
    }

    puts("");
    return 0;
}
