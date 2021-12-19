#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <random>
#include <iomanip>
using namespace std;


void oddEvenSeq(int * global_array, int global_number){
    for (int i=0; i<global_number; i++){
        // transport inside the process
        for(int j=0; j<global_number-1; j++){
            int tar_index = j;
            if((tar_index+i)%2 == 0){
                if(global_array[j] > global_array[j+1]){
                    int temp = global_array[j];
                    global_array[j] = global_array[j+1];
                    global_array[j+1] = temp;
                }
            }
        }
    }
}


int main(int argc, char** argv){

    int global_num;
    if (argc < 2){
        // if user did not provide the size of array
        // the defualt value is set to be 100
        global_num = 100;
    }else{
        global_num = atoi(argv[1]);
    }
    std::cout << "Name: Zhixuan Liu" << endl;
    std::cout << "Student ID: 118010202" << endl;
    std::cout << "Assignment 1, Seqential-Odd-Even Sort" << endl;
    std::cout << "The number of element is: " << global_num << "." << std::endl;
    std::cout << "The array data before sorting is: " << std::endl;
    srand(time(0));

    int global_array[global_num];

    for(int i=0; i<global_num; i++){
        global_array[i] = rand() % 1000;
        if(i < 20){
        std::cout << global_array[i] << " ";
        }
    }
    std::cout<<"."<<std::endl;

    // begin the sqeuential odd even sorting algorithm
    // time setting
    clock_t startT, finishT, time_need;
    startT = clock();
    oddEvenSeq(global_array, global_num);

    finishT = clock();
    time_need = finishT - startT; 
    double duration = ((double)time_need)/CLOCKS_PER_SEC;

    std::cout<<"final sorted data: "<<std::endl;
    for(int k=0;k<global_num;k++){
        if(k < 20){
            std::cout << global_array[k] << " ";
        }
    }
    std::cout << "." << std::endl;
    cout << "Time used to sort " << global_num << " numbers is: " <<setiosflags(ios::fixed)<<duration << "."<<endl;

}