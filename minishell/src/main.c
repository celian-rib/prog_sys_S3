#include "Shell.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    struct Shell shell;
    shell_init(&shell);
    shell_run(&shell);
    shell_free(&shell);
    return (EXIT_SUCCESS);
    (void)argc;
    (void)argv;
}
