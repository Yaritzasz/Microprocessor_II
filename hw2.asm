.data
array: .word 0,0,0,0,0,0,0,0,0,0

.text
.globl main

main:
    li x8, 0          # initialize loop counter i = 0
    la x9, array      # base address of array

loop:
    li x5, 10         # loop limit = 10
    bge x8, x5, done  # stop when i reaches 10

    mv x10, x8        # i into argument register
    jal fibonacci     # call fibonacci(i)

    slli x6, x8, 2    
    add  x7, x9, x6   # compute address of array[i]
    sw   x10, 0(x7)   # store fibonacci result 

    addi x8, x8, 1    # increment i
    j loop            # repeat loop

done:
    j done            # infinite loop to end program

fibonacci:
    addi x2, x2, -4   # make space on stack
    sw   x1, 0(x2)    # return address

    li x5, 1          # check base case valueis 1
    ble x10, x5, base # if n <= 1 return n

    li x6, 0          # first number = 0
    li x7, 1          # second number = 1
    li x28, 2         # start counter at 2

fib_loop:
    bgt x28, x10, fib_end  # stop when counter > n

    add x29, x6, x7   # compute next fibonacci number
    mv  x6, x7        # shift second into first
    mv  x7, x29       # update second with new value

    addi x28, x28, 1  # increment loop counter
    j fib_loop

fib_end:
    mv x10, x7        # move result into return register

base:
    lw   x1, 0(x2)    # restore return address
    addi x2, x2, 4    # restore stack pointer
    ret               # return to caller
