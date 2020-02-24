# Assignment One: Parameter Passing

You are to write a simple assembly function that will take a numerator and a denominator as function parameters and return the quotient and remainder. For example:

    $ divd 13 2

    6 1

    $ divd 126 7

    18 0

    $ divd 42 16

    2 10

1. You should strive to complete this functionality in as _few instructions as possible_.
1. Only write your solution in the provided `divd.S` file.
1. Use _repeated subtraction_ to calculate the quotient. You will need to construct a loop yourself!
1. Do not simply generate assembly using the compiler and submit it. **I will know**.

### Parameter Passing

As we talked about in class, two parameters are being passed to the `divd()` function, the numerator is in `r15` and the denominator is in `r14`. When the microcontroller executes the `ret` instruction at the end of your function, the quotient should be in `r15` and the remainder should be in `r14`.

### Assembly Instructions

Here are some useful instructions you might need:

```
mov     src, dst    ; move the contents of src to dst
sub     src, dst    ; subtract the value in src from dst and store the result in dst
add     src, dst    ; add the value in src to the value in dst and store the result in dst
clr     dst         ; zero out destination (same as mov #0, dst)
jn      label       ; jump to label if N is set (result of previous instruction set N)
jmp     label       ; jump to label unconditionally (always)
```

A more complete listing can be found on page 56 of the [Family User's Guide](http://www.ti.com/lit/ug/slau144j/slau144j.pdf).

### Registers

You can use any of the registers `r5 - r15`, however do _not_ use registers `r0 - r4` for _anything_ in your function. These registers serve special purposes on the **MSP430**, which we will talk about next week.

