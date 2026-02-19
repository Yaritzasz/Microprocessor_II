    .data
    .align 2

/* array[10] = 40 bytes */
array:
    .word 0,0,0,0,0,0,0,0,0,0


    .text
    .globl main

/* Entry point */
_start:
    j main


/* ------------------------------------------------------------
   fibonacci(n)
   input : x10 = n
   output: x10 = fib(n)

   register use:
     x10 = n / return value
     x5  = n copy
     x6  = 1 constant
     x7  = first
     x28 = second
     x29 = i
     x30 = result
     x2  = sp
     x1  = ra
------------------------------------------------------------ */
fibonacci:
    addi x2, x2, -16
    sw   x1, 12(x2)

    addi x5,  x10, 0        /* x5 = n */
    addi x6,  x0,  1        /* x6 = 1 */

    bge  x6,  x5,  FibRetN  /* if 1 >= n => n <= 1 return n */

    addi x7,  x0,  0        /* first  = 0 */
    addi x28, x0,  1        /* second = 1 */
    addi x29, x0,  2        /* i = 2 */

FibTest:
    blt  x5,  x29, FibDone  /* if n < i => i > n stop */

    add  x30, x7,  x28      /* result = first + second */
    addi x7,  x28, 0        /* first = second */
    addi x28, x30, 0        /* second = result */
    addi x29, x29, 1        /* i++ */
    j    FibTest

FibDone:
    addi x10, x30, 0        /* return result */
    lw   x1,  12(x2)
    addi x2,  x2,  16
    jalr x0,  0(x1)

FibRetN:
    addi x10, x5,  0        /* return n */
    lw   x1,  12(x2)
    addi x2,  x2,  16
    jalr x0,  0(x1)


/* ------------------------------------------------------------
   main:
   for (i=0; i<10; i++) array[i] = fibonacci(i);

   register use:
     x8  = base address of array
     x5  = i
     x6  = bound (10)
     x7  = offset (i*4)
     x28 = address (&array[i])
     x10 = argument/return (a0)
------------------------------------------------------------ */
main:
    la   x8, array          /* x8 = &array[0] */

    addi x5, x0, 0          /* i = 0 */
    addi x6, x0, 10         /* bound = 10 */

MainTest:
    bge  x5, x6, MainDone   /* if i>=10 exit */

    addi x10, x5, 0         /* x10 = i (argument) */
    jal  x1, fibonacci      /* call fibonacci(i), result in x10 */

    slli x7,  x5, 2         /* offset = i*4 */
    add  x28, x8, x7        /* &array[i] */
    sw   x10, 0(x28)        /* array[i] = fib(i) */

    addi x5, x5, 1          /* i++ */
    j    MainTest

MainDone:
    ebreak                  /* stop in Ripes */
