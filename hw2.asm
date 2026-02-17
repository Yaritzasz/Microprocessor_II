    .data
    .align 2

/* array[10] = 10 words = 40 bytes */
array:
    .word 0,0,0,0,0,0,0,0,0,0


    .text
    .globl main

/* main: for(i=0;i<10;i++) array[i]=fibonacci(i); */
main:
    la      s0, array              /* s0 = base addr of array */
    li      t0, 0                  /* i = 0 */
    li      t1, 10                 /* bound = 10 */
    addi    x0, x0, 0              /* NOP */

main_for_test:
    bge     t0, t1, main_done      /* if i>=10 break */

main_for_body:
    mv      a0, t0                 /* arg n = i */
    jal     ra, fibonacci          /* a0 = fib(i) */

    slli    t2, t0, 2              /* offset = i*4 */
    add     t3, s0, t2             /* &array[i] */
    sw      a0, 0(t3)              /* array[i] = a0 */

main_for_step:
    addi    t0, t0, 1              /* i++ */
    j       main_for_test

main_done:
    ebreak                         /* stop in Ripes */


/* int fibonacci(int n)
   in:  a0=n
   out: a0=fib(n)
*/
fibonacci:
    addi    sp, sp, -16
    sw      ra, 12(sp)

    mv      t0, a0                 /* t0 = n */
    li      t1, 1
    ble     t0, t1, fib_ret_n      /* if n<=1 return n */

    li      t2, 0                  /* first  = 0 */
    li      t3, 1                  /* second = 1 */
    li      t4, 2                  /* i = 2 */

fib_for_test:
    bgt     t4, t0, fib_done       /* if i>n break */

fib_for_body:
    add     t5, t2, t3             /* result = first+second */
    mv      t2, t3                 /* first = second */
    mv      t3, t5                 /* second = result */
    addi    t4, t4, 1              /* i++ */
    j       fib_for_test

fib_done:
    mv      a0, t5                 /* return result */
    lw      ra, 12(sp)
    addi    sp, sp, 16
    ret

fib_ret_n:
    mv      a0, t0                 /* return n */
    lw      ra, 12(sp)
    addi    sp, sp, 16
    ret
