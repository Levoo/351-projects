/*-------------------------------------------
Author: Fernando
Class:
Professor:
Project: Multi-threaded Bankers Algorithm
Due:
File name: banksy.cpp
Status: WIP
 todo: -request / release functions
       -bankers algo
       -array of bools to keep track of done processes
       -handle accepted / denied requests?
       -handel unsafe state -> should exit whole program and print current state and which process was requesting and what
       it was requesting


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
bool inSafeState(int customer_num, int request[]); //call this after a resource is released to see if we are in a safe state


void bank_customer(int customer_num);

/* other set up functions */

// create the appropriate tables and pupulate them with data
// available table is what is given to use by user input or file input.
// max table will be given
// allocation table will be given to us as well.
// need table will be [ # of customer}[max - allocated]
// sleep thread used to make it seem that the thread is doing work
void create_Tables(char* given[]);
void show_Table(int table[][NUM_RESOURCES]); // for matric array
void show_Vector(int vect[]); // for 1d array
void show_State();
void sleep_thread();


int main(int argc, char* argv[]){

    // create the tables based on avalable and other such things
    // possibly make it read a file for easy testing, lookm at java code for more
    // refined help, currently seems good but there might be at least 3 funcs missing.
    transaction_lock.lock();
    create_Tables(argv);
    transaction_lock.unlock();

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
//-----------------------------------------------------------------
//create and populate all the tables for the bankers algorithm
void create_Tables(char* given[]){
    for(int i = 0; i < NUM_RESOURCES; i++){
        //std::cout << given[i] << std::endl; // given[1..3] has the numbers given of 0 has text so start at i+1
        available[i] = atoi(given[i+1]); // set the avaliable to the one given by the user
    }
    std::fstream readFile;
    readFile.open("/home/osc-vm/CLionProjects/banker_algo/infile.txt"); //change for other sys

    if(!readFile) {std::cout << "error opening file!"; exit(1);}
    std::string temp;
    int raw_data[30];
    int i = 0;
    while (readFile >> temp){ //raw reading figure out how to parse the data
        raw_data[i] = stoi(temp);
        i++;
    }
    // allocation and max table
    int allocLoc[5] = {0, 6, 12, 18, 24};  // too cheesy figure out how to do with loops
    int maxLoc[5]   = {3, 4, 15, 21, 27};
    for(int customer = 0; customer < NUM_CUSTOMERS; customer++){ //move raw data into arrays
        for(int data = 0; data < 3; data++){
            allocation[customer][data] = raw_data[allocLoc[customer] + data];
            maximum[customer][data]    = raw_data[maxLoc[customer] + data];
        }

    }
    // need table
    for(int customer = 0; customer < NUM_CUSTOMERS; customer++){
        for(int data = 0; data < 3; data++){
            need[customer][data] = maximum[customer][data] - allocation[customer][data];
        }
    }
    //get true avaliablity, have al the columns added up and subtract frm total resources provided by user
    int r_temp[NUM_RESOURCES] = {0};
    for(int customer = 0; customer < NUM_CUSTOMERS; customer++){
        for(int data = 0; data < 3; data++){
            r_temp[data] += allocation[customer][data];
        }
    }

    for(int data = 0; data < 3; data++){
        available[data] -= r_temp[data];
    }
}


//----------------------------------------------------------------------------------------
// other functions used to check

int request_resources(int customer_num, int request[]){
    // do something
    //std::cout << request << std::endl;
    //1.) if request <= need go to step 2
    //2.) if request <= avaliable go to 3
    //3.) have sys prentent to give resources to process
    // avliable = avaliable - request
    // allocation  = allocation + request
    // need = need - request
    std::cout << "Customer # " << customer_num << " made a resource request.\n";
    return 0;

}

int release_resources(int customer_num, int release[]){
    // do something
    //std::lock_guard<std::mutex> guard(transaction_lock);
    // release all of the allocated resources back to avaliable so avaliable += allocated
    std::cout << "Customer # " << customer_num << " made a release request.\n";
    return 0;
}

void sleep_thread(){
    std::chrono::milliseconds sleepDuration(5000);
    std::this_thread::sleep_for(sleepDuration);
}

void show_Table(int table[][NUM_RESOURCES]){
    for(int customer = 0; customer < NUM_CUSTOMERS; customer++){
        for(int data = 0; data < 3; data++){
            std::cout << table[customer][data] << " ";
        }
    }
    std::cout << std::endl;
}

void show_Vector(int vect[]){
    //std::lock_guard<std::mutex> guard(transaction_lock);
    for (int i = 0; i < NUM_RESOURCES; i++){
        std::cout << vect[i] << " ";
    }
    std::cout << std::endl;
}

void show_State(){
    bool printed = false;
    std::cout << "\t\t\tCurrent State\n";
    std::cout << "Customer    | Allocated  | Max       | Need    | Available ";
    for(int customer = 0; customer < NUM_CUSTOMERS; customer++){ //move raw data into arrays
        std::cout << "\nC " << customer+1 << "\t\t    | ";
        for(int data = 0; data < 3; data++){
            std::cout << allocation[customer][data] <<  " ";
        }
        std::cout << "\t | ";
        for(int data = 0; data < 3; data++){
            std::cout << maximum[customer][data] <<  " ";
        }

        std::cout << "\t | ";
        for(int data = 0; data < 3; data++){
            std::cout << need[customer][data] <<  " ";
        }

        if (!printed){
            std::cout << "  |> ";
            for(int data = 0; data < 3; data++){
                std::cout << available[data] <<  " ";
            }
            printed = true;
        }

    }
    std::cout << std::endl;
}
//----------------------------------------------------------------------------------------
// function threads use

void bank_customer(int customer_num){
    // requestes and such will be made here
    int running = 1;
    int customer_request[NUM_RESOURCES];

    while(running) {
        sleep_thread();
        //transaction_lock.lock();
        //make a resource request make it random use java file.
        transaction_lock.lock();
        show_State(); //put an assert to make sure we get no errors in random generator
        for(int req = 0; req < NUM_RESOURCES; req++){ customer_request[req] = rand() % (maximum[customer_num][req]+1);}
        show_Vector(customer_request);
        transaction_lock.unlock();
        //show_State();
        // if request was succesful exit then stop the loop
        // if not keep it as 1
        transaction_lock.lock();
        //show_State();
        if(request_resources(customer_num, customer_request) == 0){
            sleep_thread(); //make it look likes its using the resources
            release_resources(customer_num, customer_request); //implement
            running = 0;
            // have array of bools and set this process to done (true)
            // possiby have statment saying thread # n requested this and show new state after release?
        }
        transaction_lock.unlock();
       // release_resources(customer_num, nullptr);
    }
    // release resources back to avaliable
}
