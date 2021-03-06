/*-------------------------------------------
Authors: Fernando 
       : Mattew 
Class: CPSC 351
Professor: William McCarthy
Project: Multi-threaded Bankers Algorithm
Due: 11/3/2017
File name: banksy.cpp
Status: Tested with infile.txt, need to test more with others...
NOTE: MAKE SURE TO CHANGE FILE PATH IN LINE 98
      compiled/ran with:
            g++ -std=c++11 banksy.cpp -pthread -o wells_fargo
		./wells_fargo 10 5 7
---------------------------------------------*/

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>

#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3


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

std::mutex transaction_lock; //lock

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
    if(argc != 4){
      std::cout << "Insuffecent arguments passed\n\tUsage: ./a.out <number> <number> <number>\n";
      return -1;
    }

    // for some reason create tables was acting funny when threads were running, putting here just in case
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
      //  std::cout << atoi(given[i+1]) << " ";
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
    int maxLoc[5]   = {3, 9, 15, 21, 27};
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
    // this allocation should be done in the bankers algo where it will test if allocation is safe
    // avliable = avaliable - request
    // allocation  = allocation + request
    // need = need - request
    std::cout << "CUSTOMER  # " << customer_num  << " is REQUESTING resources."<< std::endl;
    for (int i = 0; i < NUM_RESOURCES; i++){
        if(request[i] <= need[customer_num][i]){
            if(request[i] <= available[i]){
                std::cout <<  "\trequest of " << request[i] << " is valid request\n";
            } else{
                std::cout << "\tREQUEST DENIED: ";
                show_Vector(request);
                return 1;
            }
        } else{
            std::cout << "\tREQUEST DENIED: ";
            show_Vector(request);
            return 1;
        }
    }
    std::cout << "Customer # " << customer_num << " made a VALID resource request.\n";
    std::cout << "Checking to make sure request leaves machine is safe state! Take a look at the OLD STATE while you wait!\n";
    show_State();
    if(inSafeState(customer_num, request)){ //in safe state returns true if safe else it returns false: not safe
        std::cout << "\tNEW STATE - all is SAFE\n";
        show_State();
    }else{
        std::cout << "WARNING: request leaves machine in unsafe state, cancelling request!!\n";
        for(int i = 0; i < NUM_RESOURCES; i++){ //get back resources
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        return 1; //looki here
    }

    return 0;

}

bool inSafeState(int customer_num, int request[]){
    // bankers algo to check if in safe state...

    for(int i = 0; i < NUM_RESOURCES; i++){ //pretend to give resources
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    int work[NUM_RESOURCES]; //step 1
    bool finish[NUM_CUSTOMERS];
    for(int set = 0; set < NUM_CUSTOMERS; set++){finish[set] = false;} //set finish all to false
    for(int set = 0; set < NUM_RESOURCES; set++){
        work[set] = available[set];
        //std::cout << work[set] << " ";
    } // set work == avaliable

 // the banker
    for(int i = 0; i < NUM_CUSTOMERS; i++){ // go though procees m * n^2 times :: NUM_RESOURCES x NUM_CUSTOMERS^2
        for(int j = 0; j < NUM_CUSTOMERS; j++){ // if after that still find a process that is false then unsafe, see last for loop
            if(!finish[j]){  // if find an unfinished process
                bool canFinish = true;
                for(int k = 0; k < NUM_RESOURCES; k++){
                    if(need[j][k] > work[k]){  // check if need <= work if greater then set to false
                        canFinish = false; // if we find one instance of
                    }
                }
                if(canFinish){
                    finish[j] = true;
                    for(int x = 0; x < NUM_RESOURCES; x++){
                        work[x] += allocation[j][x];
                    }
                }
            }
        }
    }

    for(int i = 0; i < NUM_CUSTOMERS; i++){  // if all true safe, else not safe
        if (!finish[i]){
            std::cout << "unsafe" << "\n";
            return false;
        }
    }
    //safe
    return true;

}

int release_resources(int customer_num, int release[]){
    // release all of the allocated resources back to avaliable so avaliable += allocated
    std::cout << "Customer # " << customer_num << " made a RELEASE REQUEST.\n";

    for(int i = 0; i < NUM_RESOURCES; i++){
        available[i] += allocation[customer_num][i];
        allocation[customer_num][i] -= allocation[customer_num][i];
        //allocation[customer_num][i] -= release[i];
    }
    std::cout << "\tState after resrouce request\n";
    show_State();
    std::cout << "\n" << std::endl;

    return 0;
}

void sleep_thread(){
    std::chrono::milliseconds sleepDuration(2000);
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
        std::cout << "\nC " << customer << "\t\t    | ";
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
        transaction_lock.lock();
        for(int req = 0; req < NUM_RESOURCES; req++){ customer_request[req] = rand() % (maximum[customer_num][req]+1);}
        transaction_lock.unlock();

        transaction_lock.lock();
        if(request_resources(customer_num, customer_request) == 0){
            sleep_thread(); //make it look likes its using the resources
            //show_State();
            release_resources(customer_num, customer_request); //implement
            running = 0;
        }

        transaction_lock.unlock();
    }
}
