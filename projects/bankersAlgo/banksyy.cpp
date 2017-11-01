/*-------------------------------------------
Author: Fernando
Class:
Professor:
Project: Multi-threaded Bankers Algorithm
Due:
File name: banksy.cpp
Status: WIP
---------------------------------------------*/

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <chrono>

#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3
// http://www.cplusplus.com/reference/mutex/mutex/ see this for mutex lock usage.

/* Global Vars for Banker */

// available amount of each of the resources
// max demand of each customer
// max demand of each customer
// amount allocated to each customer
// need of each customer
int available[NUM_RESOURCES];
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];
int need[NUM_CUSTOMERS][NUM_RESOURCES];

std::mutex transaction_lock;

/* request / release functions used by threads (customers)  */

// will check to see if requested_resources will deadlock system
// return 0 if it will not, else -1 if it will.
// release_resource releases the allocated resources that the customer
// has been allocated  similar return to above
// these fucntions will be accessed through the bank_customer function
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);


void bank_customer(int customer_num);

/* other set up functions */

// create the appropriate tables and pupulate them with data
// available table is what is given to use by user input or file input.
// max table will be given
// allocation table will be given to us as well.
// need table will be [ # of customer}[max - allocated]
// sleep thread used to make it seem that the thread is doing work
void create_Tables(char* given[]);
void show_Table(); // for matric array
void show_Vector(int vect[]); // for 1d array
void show_All();
void sleep_thread();


int main(int argc, char* argv[]){

    // create the tables based on avalable and other such things
    // possibly make it read a file for easy testing, lookm at java code for more
    // refined help, currently seems good but there might be at least 3 funcs missing.
    create_Tables(argv);

    std::cout << "Customers Are entering the bank\n";

    std::thread th_Customers[NUM_CUSTOMERS]; // array of customer threads;

    // create and launch the customer thread
    for(int customer = 0; customer < NUM_CUSTOMERS; customer++){
        th_Customers[customer] = std::thread(bank_customer, customer);
    }

    // join the customer threads once done.
    for(int customer = 0; customer < NUM_CUSTOMERS; customer++){
        th_Customers[customer].join();
    }
    std::cout << "Customers are leaving\n";
    return 0;
}
//create and populate all the tables for the bankers algorithm
void create_Tables(char* given[]){
    for(int i = 0; i < NUM_RESOURCES; i++){
        std::cout << given[i] << std::endl; // given[1..3] has the numbers given of 0 has text so start at i+1
        available[i] = atoi(given[i+1]); // set the avaliable to the one given by the user
    }
    std::cout << available[0] << " " << available[1] << " " << available[2] << std::endl;
    std::fstream readFile;
    readFile.open("/home/dauser/CLionProjects/bank_algo/infile.txt");

    if(!readFile) {std::cout << "error opening file!"; exit(1);}
    std::string temp;
    while (readFile >> temp){ //raw reading figure out how to parse the data
        std::cout << temp << " ";
    }
}


//----------------------------------------------------------------------------------------

int request_resources(int customer_num, int request[]){
    // do something
    std::cout << request << std::endl;
    std::cout << "Customer # " << customer_num << " made a resource request.\n";
    return 0;
}

int release_resources(int customer_num, int release[]){
    // do something
    std::cout << "Customer # " << customer_num << " made a release request.\n";
    return 0;
}

void sleep_thread(){
    std::chrono::milliseconds sleepDuration(5000);
    std::this_thread::sleep_for(sleepDuration);
}

void show_Vector(int vect[]){
    for (int i = 0; i < NUM_RESOURCES; i++){
        std::cout << vect[i] << " ";
    }
    std::cout << std::endl;
}
//----------------------------------------------------------------------------------------

void bank_customer(int customer_num){
    // requestes and such will be made here
    int running = 1;
    int customer_request[NUM_RESOURCES];

    while(running) {
        sleep_thread();
        transaction_lock.lock();
        //make a resource request make it random use java file.
        for(int req = 0; req < NUM_RESOURCES; req++){ customer_request[req] = maximum[customer_num][req]+1;}
        show_Vector(customer_request);
        // if request was succesful exit then stop the loop
        // if not keep it as 1
        running = request_resources(customer_num, nullptr);
        transaction_lock.unlock();
    }
    // release resources back to avaliable
    transaction_lock.lock();
    release_resources(customer_num, nullptr);
    transaction_lock.unlock();

    //
}

