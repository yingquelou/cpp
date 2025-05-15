#include "compiler_detect.h"
#include <stdio.h>

int main() {
    printf("Compiler: %s\n", COMPILER_NAME);
    printf("Version: %d.%d\n", COMPILER_VERSION_MAJOR, COMPILER_VERSION_MINOR);

    printf("C++ standard version: %ld\n", CPP_STANDARD);

    return 0;
}
