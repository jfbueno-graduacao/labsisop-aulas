#include <stdio.h>
#include <string.h>

int main()
{
    char input[128];
    int i;

    printf("Digite algo: ");
    scanf("%s", input);

    printf("%s\n", input);

    for(i = 0; i < strlen(input); i++)
    {
        printf("%c\n", input[i]);
    }
}