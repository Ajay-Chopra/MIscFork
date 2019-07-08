#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <ctime>
#include <numeric>

using namespace std;

/*
HEADER FILE:
Contains class and struct definitions
*/

#ifndef _BANK_H_
#define _BANK_H_

/*
Define an Account class
*/
class Account {
private:
  // Private attributes
  unsigned int amount;
  unsigned long int account_id;
  string type;
public:
  //Public method definitions

  unsigned int get_amount(){return amount;}
  void set_amount(unsigned int value){amount = value;}

  unsigned long int get_id(){return account_id;}
  void set_id();
  void set_id_manual(unsigned long int man_id){account_id = man_id;}

  string get_type(){return type;}
  void set_type(string acc_type){type = acc_type;}

  void show_account();

  void deposit();
  void withdraw();
  void transfer();
};


/*
Define a Customer class
*/
class Customer {
private:
  string lname;
  unsigned long int phone_num;
  string address;
  string pword;
  vector<Account> accounts;
  unsigned int num_accounts;
public:

  void get_info();
  void show_info();

  string get_lname(){return lname;}
  void set_lname(string name){lname = name;}

  unsigned long int get_phone_num(){return phone_num;}
  void set_phone(unsigned long int num){phone_num = num;}

  string get_address(){return address;}
  void set_address(string add){address = add;}

  string get_password(){return pword;}
  void set_password(string password){pword = password;}

  int account_lookup(unsigned long int id);
  void add_account(Account acc){
    accounts.push_back(acc);
    num_accounts = accounts.size();
  }


  int get_n_accounts(){return num_accounts;}
  void set_n_accounts(){num_accounts = accounts.size();}

  Account get_account(int n){return accounts[n];}
  vector<Account> get_accounts(){return accounts;}

  void transfer(int n1, int n2);

};




#endif
