This program is designed to find Vampire Numbers in a given range using multiple threads. A Vampire Number is a composite natural number with an even number of digits, that can be factored into two integers with the same number of digits as the original number and whose product is the original number.


Input :


My program take input from the infile.txt which should have
first number : Number of thread you want to provide.
second number: Number below which you have to find Vampire Number.


for example it should be like

In infile.txt

8 5000

My main program read(via file pointer ptr) and it store in m and N which I declare globally.


Output:


for printing in output I declare global array which help me to print in outfile.txt whenever I want to print .
I first store vampire number found by each thread in local buffer and then put in global buffer index(i.e vampire number) using local buffer 
founded value then corresponding to these index i store thread ID who found the these vampire number.
for example: thread ID 10 found 1260 as vampire number 
then it will be store as global_buffer[1260]=10,
so it will be easy for me to to directly print all non-zero index in file.(to be noted initially I initialised global_buffer all value as 0).
Also I declare vampire_count globally which keep track of total number of vampire when my if condition in runner satisfies I am doing vampire_count++
which will also be printed in outfile.txt

so if my m=8 and N=5000

then my output will look like:

1260 : Found By thread :4
1395 : Found By thread :3
1435 : Found By thread :3
1530 : Found By thread :2
1827 : Found By thread :3
2187 : Found By thread :3
Total Vampire Number : 6 

