    .data
    .align 2
array:
    .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   # 10 ints = 40 bytes

    .text
    .globl main

# ============================================================
# main:
# for (i=0; i<10; i++) array[i] = fibonacci(i);
# ============================================================
main:
    la   s0, array        # s0 = base address of array (callee-saved; fibonacci will preserve it)

    li   t0, 0            # i = 0
    li   t1, 10           # loop bound

main_loop:
    bge  t0, t1, main_done        # if (i >= 10) break

    mv   a0, t0                   # a0 = i (argument)
    jal  ra, fibonacci             # call fibonacci(i), return in a0

    slli t2, t0, 2                # offset = i * 4
    add  t3, s0, t2               # addr = &array[i]
    sw   a0, 0(t3)                # array[i] = fib(i)

    addi t0, t0, 1                # i++
    j    main_loop

main_done:
    ebreak                        # stop in Ripes (no OS to return to)

# ============================================================
# int fibonacci(int n)
# a0 = n on entry
# returns fib(n) in a0
#
# Uses:
#   t0 = n
#   t2 = first
#   t3 = second
#   t4 = i
#   t5 = result
# Saves/restores ra (calling convention)
# ============================================================
fibonacci:
    addi sp, sp, -16
    sw   ra, 12(sp)

    mv   t0, a0            # t0 = n
    li   t1, 1
    ble  t0, t1, fib_ret_n # if (n <= 1) return n

    li   t2, 0             # first = 0
    li   t3, 1             # second = 1
    li   t4, 2             # i = 2

fib_loop:
    bgt  t4, t0, fib_done  # if (i > n) break

    add  t5, t2, t3        # result = first + second
    mv   t2, t3            # first = second
    mv   t3, t5            # second = result
    addi t4, t4, 1         # i++
    j    fib_loop

fib_done:
    mv   a0, t5            # return result
    lw   ra, 12(sp)
    addi sp, sp, 16
    ret

fib_ret_n:
    mv   a0, t0            # return n
    lw   ra, 12(sp)
    addi sp, sp, 16
    ret
