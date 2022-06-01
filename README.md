# Modified-Moving-Average
An MMA for embedded projects

# mma.c
A simple library to calculate modified moving averages (MMA).  This library is meant for embedded projects, so there is no dynamic memory.  The number of averages to use is assigned at compile time through the definition [MAX_AVERAGES].

## Usage
### Setup
There are two definitions in **[mma.h](src/mma.h?raw=1)** that need to be configured before using the library:

```c
//Number of values to use for the moving average
#define NUMBER_OF_SAMPLES   8
//Number of averages needed by external modules - This sets the maximum IDs that can be used.
#define MAX_AVERAGES 2 
```

Note: This was used for an AVR MCU so the following line is included in the .c file
```c
#include <avr/io.h>
```
Change to stdint.h if required
```c
#include <stdint.h>
```

### Register
Register for an ID by calling the function mma_error_t avg_register(uint8_t *avgID)
```c
uint8_t myID;

if (avg_register(&myID) < 0){  
  //error
}
```
Ensure you check the return value of the function before reading avgID.  This ID is a unique MMA that can be updated by using the provided functions with this ID.

To add a new value to the average, call the following function and provide the ID and new value as parameters
```c
uint16_t measurement = 123;
if (avg_moveTheAverage(myID, measurement) < 0){
  //error
 }
```
To get the current average, call the following function:
```c
int16_t average;
if (avg_getAverage(myID, &average) < 0){
  //error
}
```

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.

