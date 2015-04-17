# MPU6050
MPU6050 accelerometer and gyroscope drivers for STM32f3

## Usage
One can open this repository using CooCox CoIDE and program/debug stm32 F3 discovery board. If there's a problem with
compiling the file, there is a link to math.h library missing to the compiler.
Go to Right click on project -> Configuration -> Link and Add "m" to the Linked libraries.

Code in this branch (master) includes everything to read (calculated) data from the sensors. There's a complementary filter
implemented in this repository, in the branch -b MPU6050-Complementary_filter.

