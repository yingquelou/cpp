#include <stdio.h>
#include <compiler_detect.h>
int main(int argc, char const *argv[])
{
    printf("%d", C_STANDARD);
    return 0;
}
