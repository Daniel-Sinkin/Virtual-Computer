#include "cpu.h"
#include <stdio.h>
#include <string.h>

void initialize_cpu(CPU *cpu) {
    memset(cpu, 0, sizeof(CPU));
}