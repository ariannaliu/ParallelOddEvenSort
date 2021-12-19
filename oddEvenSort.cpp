#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <random>
#include <iomanip>
using namespace std;


void mpi_sort(int *local_array, int global_number, int p, int basic, int remainder, int my_rank){

    // local_number: how many number are in this process
    // base_number: how many number are in front of this process
    int local_number, base_num;
    local_number = (my_rank < remainder) ? basic+1 : basic;
    base_num = (my_rank < remainder) ? (basic+1) * my_rank : (basic+1)*remainder + basic*(my_rank-remainder);
    int last_p = (global_number >= p) ? p : global_number;

    int last_index = base_num + local_number -  1;
    int first_index = base_num;
    int s1,r1;
    for (int i=0; i<global_number; i++){
        // transport inside the process
        for(int j=0; j<local_number-1; j++){
            int tar_index = base_num + j;
            if((tar_index+i)%2 == 0){
                if(local_array[j] > local_array[j+1]){
                    int temp = local_array[j];
                    local_array[j] = local_array[j+1];
                    local_array[j+1] = temp;
                }
            }
        }

        s1=0; 
        r1=0;

        if (my_rank == 0){

            if((last_index+i)%2 == 0){
                MPI_Recv(&r1, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                s1 = (local_array[local_number-1] > r1) ? local_array[local_number-1] : r1;
                local_array[local_number-1] = (local_array[local_number-1] > r1) ? r1 : local_array[local_number-1];
                MPI_Send(&s1, 1, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD);
            }
        }else if(my_rank == last_p-1){
            if((first_index+i)%2 != 0){
                s1 = local_array[0];
                MPI_Send(&s1, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD);
                MPI_Recv(&r1, 1, MPI_INT, my_rank-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_array[0] = r1;
            }
        }else if (my_rank>0 && my_rank < last_p-1){
            if((first_index+i)%2 != 0){
                s1 = local_array[0];
                MPI_Send(&s1, 1, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD);
                MPI_Recv(&r1, 1, MPI_INT, my_rank-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_array[0] = r1;
            }
            if((last_index+i)%2 == 0){
                MPI_Recv(&r1, 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                s1 = (local_array[local_number-1] > r1) ? local_array[local_number-1] : r1;
                local_array[local_number-1] = (local_array[local_number-1] > r1) ? r1 : local_array[local_number-1];
                MPI_Send(&s1, 1, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD);
            }
        }

    }

}

int main(int argc, char** argv){

    // validate user inputs
    // user should input: How many numbers are needed to be sorted
    // user should run the file like this: mpirun a.out #size_of_array
    int global_num;
    if (argc < 2){
        // if user did not provide the size of array
        // the defualt value is set to be 100
        global_num = 100;
    }else{
        global_num = atoi(argv[1]);
    }


    // init MPI
    int my_rank, p, ierr;
    MPI_Comm comm;
    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &p);
    ierr = MPI_Comm_rank(comm, &my_rank);
    if (MPI_SUCCESS != ierr) {
            throw std::runtime_error("failed to get MPI world rank");
        }

    int basic = global_num / p;
    int remainder = global_num % p;
    int global_array[global_num];
    if (my_rank == 0){
        // alloc space and randomnly generate new numbers for the global array
        // global_array = (int*)malloc(sizeof(int)*global_num);
        std::cout << "Name: Zhixuan Liu" << endl;
        std::cout << "Student ID: 118010202" << endl;
        std::cout << "Assignment 1, Paralleled-Odd-Even Sort" << endl;
        std::cout << "The total number of element is: " << global_num << "." << std::endl;
        std::cout << "The array data before sorting is: " << std::endl;
        srand(time(0));
        for(int i=0; i<global_num; i++){
            global_array[i] = rand() % 1000;
            if (i < 20 ) {
                std::cout << global_array[i] << " ";
            }
        }
        std::cout<<"."<<std::endl;
    }

    // begin the odd even sorting algorithm

    // time setting
    clock_t startT, finishT, time_need;
    startT = clock();

    // dealing with the problem if # cannot be divided by # of process
    int scatter_count[p];
    int scatter_dsp[p];
    int ccount = 0;
    for (int j=0; j<p ; j++){
        if( j <= remainder-1){
            scatter_count[j] = basic+1;
        }else{
            scatter_count[j] = basic;
        }
        scatter_dsp[j] = ccount;
        ccount += scatter_count[j];
    }

    // allocate memory for local array
    int local_array[scatter_count[my_rank]]; 

    // ierr = MPI_Bcast(&localn,1,MPI_INT,0,MPI_COMM_WORLD);
    ierr = MPI_Scatterv(global_array,scatter_count, scatter_dsp, MPI_INT, local_array, scatter_count[my_rank], MPI_INT, 0, MPI_COMM_WORLD);
    mpi_sort(local_array, global_num, p, basic, remainder, my_rank);
    ierr = MPI_Gatherv(local_array, scatter_count[my_rank], MPI_INT, global_array, scatter_count, scatter_dsp, MPI_INT, 0, MPI_COMM_WORLD);


    finishT = clock();
    time_need = finishT - startT; 
    double duration = ((double)time_need)/CLOCKS_PER_SEC;
    if(my_rank==0){
        std::cout<<"final sorted data: "<<std::endl;
        for(int k=0;k<global_num;k++){
            if (k<20){
                std::cout << global_array[k] << " ";
            }
        }
        std::cout << "." << std::endl;
        cout << "Time used to sort " << global_num << " numbers is: " <<setiosflags(ios::fixed)<<duration << "."<<endl;
    }
    ierr = MPI_Finalize();


}