#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

// Global variable
int global_var = 10;

void func() {
    // Automatic variable
    int automatic_var = 20;

    // Register variable
    register int register_var = 30;

    // Static variable
    static int static_var = 40;

    // Volatile variable
    volatile int volatile_var = 50;

    printf("Automatic variable: %d\n", automatic_var);
    printf("Global variable: %d\n", global_var);
    printf("Register variable: %d\n", register_var);
    printf("Static variable: %d\n", static_var);
    printf("Volatile variable: %d\n", volatile_var);

    // Modify the variables
    automatic_var = 100;
    global_var = 200;
    register_var = 300;
    static_var = 400;
    volatile_var = 500;

    // Use longjmp to return to the setjmp point
    longjmp(env, 1);
}

int main() {
    // Setjmp to save the execution context
    int ret_val = setjmp(env);

    if (ret_val == 0) {
        // This is the initial execution
        printf("Initial execution:\n");
        func();
    } else {
        // We returned from longjmp
        printf("Returned from longjmp:\n");
    }

    // Variables after longjmp
    printf("Automatic variable: undefined\n");
    printf("Global variable: %d\n", global_var);
    printf("Register variable: undefined\n");
    printf("Static variable: %d\n", global_var);
    printf("Volatile variable: undefined\n");

    return 0;
}
