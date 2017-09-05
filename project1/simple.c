/*
Project 1: Linux Kernal Module
Class: CPSC 351, Tue/Thur 1:00pm - 2:15pm
Professor: William McCarthy
Author: Fernando Cuevas
E-mail: fzavala@csu.fullerton.edu
Due Date: 9/5/2017
*/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

// Birthday Linked List
struct birthday{
  int day;
  int month;
  int year;
  struct list_head list; // linux kernal version where LIST_HEAD
                        // contains both previuse and next pointers
};
LIST_HEAD(birthday_list); // getting warning about mixed code

/*This function is called when the module is loaded */
int simple_init(void)
{
  printk(KERN_INFO "SIMPLE: Loading simple module\n \t use 'sudo rmmod simple' to remove module\n");
 //-----------------------------------------------------------------------------
//-------------------------------STRUCT CREATION--------------------------------
  printk(KERN_INFO "SIMPLE: Creating birthday struct ...");
  struct birthday *person;

  /*
  FILLING THE STRUCT WITH DATA
  For loop used to fill the valuers of the birthday struct with valuers
  was unable to get rand() to work with the make file
  */
  int i;
  for (i = 0; i < 5; i++){
    person = kmalloc(sizeof(*person), GFP_KERNEL);
    person->day = 5 + i; //generate a number btw 1 and 31
    person->month = 9 + i; // ^^ between 1 and 12
    person->year = 2017 + i; //any year upt to and including 2017
    if(i == 0)
      INIT_LIST_HEAD(&person->list);
    list_add_tail(&person->list, &birthday_list);
  }

  /*
  PRINT OUT THE STRUCT
  after we have a full struct print them out to dmesg using printk
  the list_for_each_entry from the list.h lib goes through each value without
  me haveing to use a loop, very convienent, this function takes in
  the position, head of list, and name of list
  */
  printk(KERN_INFO "SIMPLE: Printing birthdays, (day/month/year)\n");
  struct birthday *ptr;   // getting warning about mixed code
  list_for_each_entry(ptr, &birthday_list, list){
    // formated with %d which is used for decimals aka ints and such
    printk(KERN_INFO "SIMPLE: %d/%d/%d\n", ptr->day, ptr->month, ptr->year);
  }
  printk(KERN_INFO "SIMPLE: Finished printing birthdays\n"); // odd not
//-------------------------------STRUCT CREATION--------------------------------
//------------------------------------------------------------------------------
  return 0;
}

/* Thia function is called when the modue is removed */
void simple_exit(void)
{
  //------------------------------------------------------------------------------
  //-------------------------------STRUCT DELETION--------------------------------
    struct birthday *ptr, *next;
    printk(KERN_INFO "SIMPLE: deleting contents of struct...");
    // will automatiaclly iterate for me and
    // will give the ptr that will  be used to loop
    // next is a temp and bday list is the head with list being the list name
    //
    list_for_each_entry_safe(ptr, next, &birthday_list, list){
      // remove contents of struct

      list_del(&ptr->list);
      kfree(ptr);
    }
    //-------------------------------STRUCT DELETION--------------------------------
    //------------------------------------------------------------------------------

    printk(KERN_INFO "Struct memory freed");
    printk(KERN_INFO "Simple: Removing simple Module\n \t use 'sudo insmod simple.ko' to add module\n");
}

/*Macros for registering module entry and exit points*/
module_init(simple_init);
module_exit(simple_exit);

// other
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG, Fernando Cuevas");
