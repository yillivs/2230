#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char buffer[10] = {0};

int main(int argc, const char **argv)
{
    printf("%d %s %s\n", argc, argv[0], argv[1]);

    int number = atoi(argv[1]);

    int remainder = number % 2;
    buffer[8] = remainder + '0';

    printf("%d %d\n", number, remainder);

    number /= 2;
    remainder = number % 2;
    buffer[7] = remainder + '0';

    printf("%d %d\n", number, remainder);

    number /= 2;
    remainder = number % 2;
    buffer[6] = remainder + '0';

    printf("%d %d\n", number, remainder);

    number /= 2;
    remainder = number % 2;
    buffer[5] = remainder + '0';

    printf("%d %d\n", number, remainder);

    printf("Number: %s\n", &buffer[5]);

    return 0;
}

