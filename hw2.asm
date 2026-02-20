.data
ARRAY: .word 0,0,0,0,0,0,0,0,0,0

.text
.globl MAIN

MAIN:
    li x8, 0          # initialize loop counter i = 0
    la x9, ARRAY      # base address of array

LOOP:
    li x5, 10         # loop limit = 10
    bge x8, x5, DONE  # stop when i reaches 10

    mv x10, x8        # i into argument register
    jal FIBONACCI     # call fibonacci(i)

    slli x6, x8, 2    
    add  x7, x9, x6   # compute address of array[i]
    sw   x10, 0(x7)   # store fibonacci result 

    addi x8, x8, 1    # increment i
    j LOOP            # repeat loop

DONE:
    j DONE            # infinite loop to end program

FIBONACCI:
    addi x2, x2, -4   # make space on stack
    sw   x1, 0(x2)    # return address

    li x5, 1          # check base case valueis 1
    ble x10, x5, BASE # if n <= 1 return n

    li x6, 0          # first number = 0
    li x7, 1          # second number = 1
    li x28, 2         # start counter at 2

F_LOOP:
    bgt x28, x10, FIB_FINISH  # stop when counter > n

    add x29, x6, x7   # compute next fibonacci number
    mv  x6, x7        # shift second into first
    mv  x7, x29       # update second with new value

    addi x28, x28, 1  # increment loop counter
    j F_LOOP

FIB_FINISH:
    mv x10, x7        # move result into return register

BASE:
    lw   x1, 0(x2)    # restore return address
    addi x2, x2, 4    # restore stack pointer
    ret               # return to caller
