#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int m; //number of threads
int N; //Number below which we have to find vampire number.

FILE *fileptr2; //Globally defining file pointer to write in file anywhere we want.  

int *global_buffer;//Global  buffer to store thread ID in corresponding founded vampire number index.
int vampire_count = 0;//To keep track of total number of vampire number found by all the thread created.

void *runner(void *var); /* threads call this function */



/*Below (NumberofDigit) function is to calculate number of digits in any number.
here i used the concept of math that any number can be express in terms of 10's powers
*/
int NumberofDigit(int num) 
{
    int count = 0;
    while (num != 0)
    {
        num = num / 10;
        count++;
    }
    return count;
}


/*Below (powerfunc) function here instead of using math.h library 
(for pow) i created a function to take out value of base^exp.
*/
int powerfunc(int base, int exp) 
{
    int value = 1;
    for (int i = 0; i < exp; i++)
    {
        value = base * value;
    }
    return value;
}


/*Below (ifodd) function is of int type 
which return 1 if digit count of any number is odd
we required these as for odd number vampire not possible
*/
int ifodd(int digiCount)
{
    int r;
    r = digiCount % 2;
    if (r == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}



/*Below (VampireNum) Function is of type boolean here in these function i am returning
true if a number is boolean and false otherwise.here in these function basically first 
counting the number of digits in the num if it odd or 2 then returning false
otherwise initially i am creating array initialising with 0 all (array_original)
 with size 10 which keep track of the which number does 
our original num(our argument) have by simply incrementing corresponding index by 1 if found 
that index in num.
now to check it is vampire first we find divisor(div) of size digitcount/2 which
give remainder 0 and store digits of quotient(quot) and divisor(div) similarly as for num in 
array and check if the count of both is equal to 10(i.e both quotient and divisor mathces same
number as num)
*/ 
bool VampireNum(int num)
{
    int digiCount;
    digiCount = NumberofDigit(num);
    int array_original[10];

    int array[10];

    for (int i = 0; i < 10; i++)
    {
        array[i] = 0;
        array_original[i] = 0;
    }

    if (ifodd(digiCount) == 1)//if digicount have odd number then we dont get vampire number
    {
        return false;
    }

    if (digiCount == 2)
    {
        return false;
    }

    else
    {
        int x;
        int dumm = num;
        while (dumm > 0)
        {

            x = dumm % 10;
            array_original[x]++;
            dumm = dumm / 10;
        }
        int div;
        int quot;
        int p;
        int q;
        int div_digit;
        int quot_digit;
        int dumm_div;
        int dumm_quot;

        for (int i = powerfunc(10, digiCount / 2 - 1); i < powerfunc(10, digiCount / 2); i++)
        {
            for (int i = 0; i < 10; i++)
            {
                array[i] = 0;
            }
            if (num % i == 0)
            {
                div = i;
                quot = num / div;
                div_digit = NumberofDigit(div);
                quot_digit = NumberofDigit(quot);
                dumm_div = div;
                dumm_quot = quot;
                while (div > 0)
                {
                    p = div % 10;
                    array[p]++;
                    div = div / 10;
                }
                while (quot > 0)
                {
                    q = quot % 10;
                    array[q]++;
                    quot = quot / 10;
                }
            }

            if (div_digit == digiCount / 2 && quot_digit == digiCount / 2)
            {

                int count = 0;
                for (int i = 0; i < 10; i++)
                {
                    if (array[i] == array_original[i])
                    {
                        count++;
                    }
                }

/*below check is important as it is only vampire if all the digits of divisor and
quotient matches with the num
other check is likely intended to ensure
 that both the divisor and quotient do not end with a zero
checks if both the last digits of dumm_div and dumm_quot are equal to 0.
*/
                if (count == 10 && !(dumm_div % 10 == 0 && dumm_quot % 10 == 0))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
/*below file pointer to read a value of m(Number of thread) and N(number below
which we have to find all vampire number)*/    
    FILE *ptr = fopen("infile.txt", "r");
    if (ptr == NULL)
    {
        printf("\nFile not found\n\n");
        return 0;
    }
    fscanf(ptr, "%d", &m);
    fscanf(ptr, "%d", &N);

//here below when we read N from infile.txt we use it to declare size of global buffer
    global_buffer = (int *)malloc((N + 1) * sizeof(int));
    for (int i = 0; i < N + 1; i++)//initially all value is 0.
    {
        global_buffer[i] = 0;
    }


//here below we have open the file outfile.txt in write mode to write our result.
    fileptr2 = fopen("outfile.txt", "w");
    if (fileptr2 == NULL)
    {
        perror("Error opening outfile.txt");
        return 0;
    }

    pthread_t tid[m]; /* the thread identifier 
    This declares an array of pthread_t identifiers with a size of m
    These identifiers will be used to keep track of the threads that will be created.*/

   
/*&tid[i]: a pointer to the thread identifier where the ID of the newly created thread will be stored.
NULL: attributes for the new thread (default attributes are used).
&runner: the function to be executed by the thread.
start: a parameter passed to the runner function
pthread_create called to create thread*/
    for (int i = 0; i < m; i++)
    {

        int *start = malloc(sizeof(int));
        *start = i + 1;

        if (pthread_create(&tid[i], NULL, &runner, start) != 0)
        {
            perror("Failed to create thread");
        }
    }

//this loop waits for each thread to finish using pthread_join   
    for (int i = 0; i < m; i++)
    {

        if (pthread_join(tid[i], NULL) != 0)
        {
            perror("Failed to create thread");
        }
    }

/*her using global buffer i am printing vampire and corresponding thread ID
 in file(outfile.txt) using file pointer(fileptr2) which we declare globally*/
    for (int i = 0; i < N + 1; i++)
    {
        if (global_buffer[i] != 0)
        {
            fprintf(fileptr2, "%d : Found By thread :%d\n", i, global_buffer[i]);
        }
    }
    fprintf(fileptr2, "Total Vampire Number : %d", vampire_count);


    pthread_exit(NULL);

    return 0;
}


/*below function takes a single argument arg of type void*
It's a common practice to pass parameters to threads by casting them to void*
*/
void *runner(void *arg)
{
    int index = *(int *)arg;//extracts the integer index from the argument passed to the thread
    
/*It allocates a local buffer (localbuffer) to store the vampire numbers found by the current thread*/
    int *localbuffer = malloc(N / m * sizeof(int));
    for (int i = 0; i < N / m; i++)//It initializes all elements of the local buffer to zero
    {
        localbuffer[i] = 0;
    }

    int j = 0;
    for (int i = index; i <= N; i += m)
    {
/*using vampireNum function we get vampire number and I am using such kind of
condition in loop to partition the set of number below N to mulitple thread
each check N/m set of number and after these i am storing it in localbuffer.*/
        if (VampireNum(i) == true)
        {
            localbuffer[j] = i;
            j = j + 1;


/*using the vampire number as index in global buffer we are storing the corresponding 
thread which have found it which we use in main thread for printing it in outfile.txt*/
            for (int i = 0; i < j; i++)
            {
                global_buffer[localbuffer[i]] = index;
            }
            vampire_count++;//to keep track of total number of vampire.
        }
    }
    free(localbuffer);//It frees the dynamically allocated memory for the local buffer.
    pthread_exit(NULL);//It exits the thread.
}