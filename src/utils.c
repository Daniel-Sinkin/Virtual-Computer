#include "utils.h"
#include <ctype.h>

bool is_printable_char(char c) {
    return isprint(c);
}