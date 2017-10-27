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

/* request / release functions used by threads (customers)  */

// will check to see if requested_resources will deadlock system 
// return 0 if it will not, else -1 if it will. 
// release_resource releases the allocated resources that the customer 
// has been allocated  similar return to above 
// these fucntions will be accessed through the bank_customer function
int request_resources(int customer_num, int request[]);
int release_resource(int customer_num, int release[]);

void bank_customer(int customer_num);

/* other set up functions */

// create the appropriate tables and pupulate them with data
// available table is what is given to use by user input or file input.
// max table will be given 
// allocation table will be given to us as well.
// need table will be [ # of customer}[max - allocated]
void create_AvaliableTable();
void create_MaxTable();
void create_AllocationTable();
void create_NeedTable();


int main(int argc, char* argv[]){
	
	// create the tables based on avalable and other such things 
	// possibly make it read a file for easy testing, lookm at java code for more 
	// refined help, currently seems good but there might be at least 3 funcs missing.
	
	std::thread th_Customers[NUM_CUSTOMERS] // array of customer threads;
	
	// create and launch the customer thread 
	for(int customer = 0; customer < NUM_CUSTOMERS; customer++){
		th_Customers[customer] = std::thread(bank_customer, customer);
	}
	std::cout << "Customers Are entering the bank\n";
	
	// join the customer threads once done.
	for(int customer = 0; customer < NUM_CUSTOMERS; customer++){
		th_Customers[customer].join();
	}
	std::cout << "Customers are leaving\n";
	return 0;
}

int request_resources(int customer_num, int request[]){
	// do something 
	std::cout << "Customer # " << customer_num << " made a resource request.\n";
	return 0; 
}
int release_resources(int customer_num, int request[]){
	// do something 
	std::cout << "Customer # " << customer_num << " made a release request.\n";
	return 0; 
}

void bank_customer(int customer_num){
	// requestes and such will be made here 
	std::cout << "Customer # " << customer_num
	//says we should have a while loop but wo bu dong!
}
