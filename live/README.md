# Live Debugging

##### `#0: ` What is the memory address of the start of the function `fac()`?

<br><br><br>

##### `#1: ` When `fac(1)` is called from main, what value is in register `r15`? What does this value represent?

<br><br><br>

##### `#2: ` There are two function calls inside of `fac()`. One is `fac()` itself, but there is another one. What is the memory address of _that_ function and what is its name? How many parameters does it accept?

<br><br><br><br>

##### `#3: ` How large is the stack frame (in bytes) for the function `fac()`? How many local variables do you think this function has? Write their offsets from the stack pointer `r1` and describe how each is being used.

<br><br><br><br><br><br>

##### `#4: ` Observe the third assembly instruction in the function `fac()` both in the source file and the debugger. What is the encoding for this instruction? What could you say about these two instructions?

<br><br><br><br>

##### `#5: ` On the _last_ recursive call to `fac()` from the initial call in main of `fac(6)`, what is the value of the stack pointer `r1`? (_after_ `sub #4, r1` was already executed)

<br><br><br>

##### `#6: ` _Right before_ `ret` is executed during this call, what value is being pointed to by the stack pointer `r1`? Explain why this value is there. (It may help to use the format specifier `/hx` to view 16-bits as a hex value)

