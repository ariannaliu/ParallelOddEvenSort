# ParallelOddEvenSort
Using MPI library to implement Odd Even Sort, also attach a sequential version
Description :
In this homework, you are required to write a parallel odd-even transposition sort by using MPI. A a parallel odd-even transposition sort is performed as follows:

     /* Initially,  m numbers are distributed to n processes, respectively.*/

For each process with odd rank P, send its number to the process with rank P-1.

For each process with rank P-1, compare its number with the number sent by the process with rank P and send the larger one back to the process with rank P.

For each process with even rank Q, send its number to the process with rank Q-1.

For each process with rank Q-1, compare its number with the number sent by the process with rank Q and send the larger one back to the process with rank Q.

Repeat 1-4 until the numbers are sorted.

Where and What to Turn in Your Homework :
1. Please turn in a report (at least 10 pages) includes

   - The design approaches

   - Performance analysis

   - The source code of your programs (sequential and parallel versions)
