#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <ctime>
#include <numeric>

#include "bank.h"

using namespace std;

// Begin helper function definitions
bool isPrime(int num);
vector<int> genPrimes(int maxPrime);
int cLookup();
Customer *verifyCustomer();
bool chkPsswrd(string password);
void showMainMenu();
void showAccountsMenu();
void showTrxMenu();
void addNewAccount(Customer c);
void addNewCust();
void chkDetails();
void handleDW(Customer *c, int choice);
void handleTrnsfr(Customer *c);
void runBank();
// End helper function definitions

// Begin global variable definitions
vector<Customer> clients;
// End global variable definitions

//***************** BEGIN HELPER FUNCTION IMPLEMENTATIONS ******************
/*
isPrime: Check if given number is prime
*/
bool isPrime(int num){
  if ((num % 2) == 0 && num != 2){
    return false;
  } else {
    int start = floor(num / 2);
    for (int i = start; i > 1; i--){
      if ((num % i) == 0){
        return false;
      }
    }
    return true;
  }
}

/*
genPrimes: Generate a vector of all primes up to given
maxPrime
*/
vector<int> genPrimes(int maxPrime){
  vector<int> myPrimes;
  for (int i = 2; i <= maxPrime; i++){
    if (isPrime(i)){
      myPrimes.push_back(i);
    }
  }
  return myPrimes;
}

/*
addNewCust: Add a new customer to the global customer vector
*/
void addNewCust(){
  Customer new_cust;
  new_cust.get_info();
  new_cust.set_n_accounts();
  clients.push_back(new_cust);
}


/*
cLookup: Given a last name, return the index of the customer if
he/she is in the clients vector, or return -1 if he/she is not
*/
int cLookup(string lname, string password){
  int num_custs = clients.size();
  for (int i = 0; i < num_custs; i++){
    if (clients[i].get_lname() == lname && clients[i].get_password() == password){
      return i;
    }
  }
  return -1;

}



/*
verifyCustomer: Have the user input his/her last name
If the user is in the customer records, prompt the user for the password
If the pasword is correct, return the Customer object
Limit the customer to five password guesses
*/
Customer *verifyCustomer(){
  while (1){
    string usrName;
    string usrPword;
    int found;
    cout << "Please input your last name: ";
    cin >> usrName;
    cout << "Please input your password: ";
    cin >> usrPword;
    found = cLookup(usrName, usrPword);
    if (found < 0){
      cout << "Customer not found. Either password or last name are incorrect \n";
    } else {
      return &clients[found];
    }
  }
}



/*
chkDetails: Receive account number from the user display account info
If account is not found continually prompt user for correct account number
*/

void chkDetails(Customer *c){
  while (1){
    string usrImpt;
    cout << "Please input the account number of the account you would like to check: ";
    cin.ignore();
    cin >> usrImpt;
    //printf("'%s' \n", usrImpt.c_str());
    unsigned long int usrNmbr = stol(usrImpt);
    //printf("%lu \n", usrNmbr);
    //printf("Number of accounts in chkDetails func is: %d \n", c->get_n_accounts());
    int exists = c->account_lookup(usrNmbr);
    //printf("%d \n", exists);
    if (exists < 0){
      cout << "Sorry, that account does not exist in our records \n";
    } else {
      c->get_accounts()[exists].show_account();
      return;
    }
  }
}

/*
addNewAccount: Prompt the user for information and crete an account accordingly
*/
void addNewAccount(Customer *c){
  Account new_account;
  string usrImpt;
  cout << "Please specify the type of account you would like to create: ";
  cin >> usrImpt;
  new_account.set_type(usrImpt);
  cout << "Please give the amount you would like in the account: ";
  cin >> usrImpt;
  new_account.set_amount(stoi(usrImpt));
  new_account.set_id();
  cout << "New account was sucessfully created \n";
  printf("Your account number is: %lu \n", new_account.get_id());
  c->add_account(new_account);
  c->set_n_accounts();
  //printf("New number of accounts is: %d \n", c->get_n_accounts());
  cout << "Enter 1 to create another accounnt \n";
  cout << "Enter 2 to exit \n";
  cout << "Please enter your choice: ";
  cin >> usrImpt;
  if (stoi(usrImpt) == 1){
    addNewAccount(c);
  } else {
    return;
  }
}

/*
handleDpst: Ask the user for an account and amount to deposit and
update the account information accordingly
*/
void handleDW(Customer *c, int choice){
  string usrInpt;
  int n;
  Account acnt;
  printf("Please enter the number of the account you would like to %s: ", choice == 1 ? "depsit into" : "withdraw from");
  cin >> usrInpt;
  unsigned long int usrNmbr = stol(usrInpt);
  if ((n = c->account_lookup(usrNmbr)) < 0){
    cout << "Sorry, that account does not exist in your records \n";
  } else {
    acnt = c->get_account(n);
  }
  Account *acntPtr = &acnt;
  if (choice == 1){
    cin.ignore();
    acntPtr->deposit();
  } else {
    cin.ignore();
    acntPtr-> withdraw();
  }
  return;
}

/*
handleTrnsfr: Prompt user for accounts to transfer to/from and update
account info accordingly
*/
void handleTrnsfr(Customer *c){
  string usrInpt;
  int n1, n2;
  Account to, from;
  cout << "Please enter the account number of the account you would like to transfer to: ";
  cin >> usrInpt;
  unsigned long int usrNmbr = stol(usrInpt);
  if ((n1 = c->account_lookup(usrNmbr)) < 0){
    cout << "Sorry, that account does not exist in your records \n";
  } else {
    to = c->get_account(n1);
  }

  cout << "Please enter the account number of the account you would like to transfer from: ";
  cin.ignore();
  cin >> usrInpt;
  usrNmbr = stol(usrInpt);
  if ((n2 = c->account_lookup(usrNmbr)) < 0){
    cout << "Sorry, that account does not exist in your records \n";
  } else {
    from = c->get_account(n2);
  }

  c->transfer(n1, n2);
  return;
}


/*
showMainMenu: Display the main menu detailing various operations that customer
can perform
*/
void showMainMenu(){
  cout << "======================== WELCOME TO THE MAIN MENU ==================== \n";
  cout << "Please choose one \n";
  cout << "********************\n";
  cout << "1. Manage accounts \n";
  cout << "2. For transactions \n";
  cout << "3. Create new customer profile \n";
  cout << "4. Exit \n";
  cout << "********************\n";
  cout << "Please enter your choice: ";
  return;
}


/*
showAccountsMenu: Display the operations available for accounts
*/
void showAccountsMenu(){
  cout << "======================== WELCOME TO THE ACCOUNTS MENU ==================== \n";
  cout << "Please choose one \n";
  cout << "********************\n";
  cout << "1. Check details of existing account \n";
  cout << "2. Create new account \n";
  cout << "3. Exit \n";
  cout << "********************\n";
  cout << "Please enter your choice: ";
  return;
}

/*
showTrxMenu: Display the operations available for transactions
*/
void showTrxMenu(){
  cout << "======================== WELCOME TO THE TRANSACTIONS MENU ==================== \n";
  cout << "Please choose one \n";
  cout << "********************\n";
  cout << "1. Deposit \n";
  cout << "2. Withdraw \n";
  cout << "3. Transfer \n";
  cout << "4. Exit \n";
  cout << "********************\n";
  cout << "Please enter your choice: ";
  return;
}


//***************** END HELPER FUNCTION IMPLEMENTATIONS ******************

//***************** BEGIN ACCOUNT CLASS METHODS IMPLEMENTATIONS *********************

/*
set_id: Create a relatively secure account_id by multiplying
two random primes between 2 and 2000 together
*/
void Account::set_id(){
  vector<int> primeVec = genPrimes(2000);
  int high = primeVec.size();
  //printf("%d \n", high);
  int f_indx = rand()%((303 - 1)+1) +1;
  int s_indx = rand()%((303 - 1)+1) +1;
  //printf("%d, %d, \n", f_indx, s_indx);
  account_id = primeVec[f_indx] * primeVec[s_indx];
}

/*
show_account: Display account information to the user
*/
void Account::show_account(){
  printf("====================DISPLAYING INFORMATION FOR ACCOUNT NO.%lu=============== \n",
  account_id);
  printf("********************************************* \n");
  printf("ACCOUNT TYPE: %s \n", type.c_str());
  printf("ACCOUNT BALANCE: %d \n", amount);
  printf("********************************************* \n");
}

/*
withdraw: Ask user for integer input
If there is enough money in the account, substract it and print a summary
Else, prompt the user until they input a substractable amount
*/
void Account::withdraw(){
  while(1){
    string input;
    cout << "Please input a value to withdraw: ";
    getline(cin, input);
    int usrAmt = stoi(input);
    if (usrAmt > amount){
      cout << "Sorry, that amount exceeds your account's funds \n";
    } else {
      printf("Withdrawing %d from account No.%lu \n", usrAmt, account_id);
      amount -= usrAmt;
      printf("ACCOUNT BALANCE: %d \n", amount);
      return;
    }
  }

}

/*
deposit: Ask user for integer input and add the amount to the account
*/
void Account::deposit(){
  string usrImpt;
  cout << "Please enter the amount you would like to deposit: ";
  getline(cin, usrImpt);
  int depValue = stoi(usrImpt);
  printf("Depositing %d into account No.%lu \n", depValue, account_id);
  amount += depValue;
  printf("ACCOUNT BALANCE: %d \n", amount);
  return;
}



//***************** END ACCOUNT CLASS METHODS IMPLEMENTATIONS *********************

//***************** BEGIN CUSTOMEER CLASS METHODS IMPLEMENTATIONS ***************

/*
transfer: Given two account indicies, prompt user for transfer
amount and update informaton accordingly
*/
void Customer::transfer(int n1, int n2){
  string usrInpt;
  cout << "Please give the amount you would like to transfer: ";
  cin >> usrInpt;
  int usrAmnt = stoi(usrInpt);
  Account *to = &(accounts[n1]);
  Account *from = &(accounts[n2]);
  to->set_amount(to->get_amount() + usrAmnt);
  from->set_amount(from->get_amount() - usrAmnt);
  printf("Transferring %d from account NO.%lu to account NO.%lu \n",
        usrAmnt, from->get_id(), to->get_id());

  printf("ACCOUNT NO. %lu BALANCE: %d \n", to->get_id(), to->get_amount());
  printf("ACCOUNT NO. %lu BALANCE: %d \n", from->get_id(), from->get_amount());


  return;
}



/*
chkPsswrd: Given a password string, check to see if it meets the following conditions:
1. At least 10 characters long
2. Contains both upper and lower case letters
3. Contains at least one symbol
4. Contains at least one digit
*/
bool chkPsswrd(string password){
  int plen = password.length();
  if (plen < 10){
    return false;
  }
  int lower_flag = 0;
  int upper_flag = 0;
  int symbol_flag = 0;
  int digit_flag = 0;
  for (int i = 0; i < plen; i++){
    if ((int)password[i] >= 97 && (int)password[i] <= 122){
      lower_flag = 1;
    }
    if((int)password[i] >= 65 && (int)password[i] <= 90){
      upper_flag = 1;
    }
    if (((int)password[i] >= 33 && (int)password[i] <= 47) ||
        ((int)password[i] >= 58 && (int)password[i] <= 64)){
          symbol_flag = 1;
        }
    if ((int)password[i] >= 48 && (int)password[i] <= 57){
      digit_flag = 1;
    }
  }

  if ((lower_flag + upper_flag + symbol_flag + digit_flag) != 4){
    return false;
  } else {
    return true;
  }
}

/*
account_lookup: Locate an account based on a given id
Return the index of the account if it is found, return
-1 if it is not
*/
int Customer::account_lookup(unsigned long int id){
  //printf("%d \n", num_accounts);
  for (int i = 0; i < num_accounts; i++){
    //printf("Account: %d has ID: %lu \n", i, accounts[i].get_id());
    if (accounts[i].get_id() == id){
      return i;
    }
  }
  return -1;
}


/*
get_info(): Prompt the user for their information and put it in the
proper locations in the Customer class
*/
void Customer::get_info(){
  string input;
  // Get last name
  cout << "Please input your last name: ";
  cin >> input;
  lname = input;

  // Get phone number
  cout << "Please input your phone number: ";
  cin >> input;
  phone_num = stol(input);

  // Get address
  string usrAddrs;
  cout << "Please input your current address: ";
  cin.ignore();
  getline(cin, usrAddrs);
  address = usrAddrs;

  // Get Password
  while(1){
    cout << "Create a password \n";
    cout << "Your passowrd must: \n";
    cout << "1. Be at least 10 characters long\n";
    cout << "2. Contain at least one lower case letter \n";
    cout << "3. Contain at least one upper case letter \n";
    cout << "4. Contain at least one symbol \n";
    cout << "5. Contain at least one digit \n";
    cout << "Please enter your password: ";
    cin >> input;
    //printf("%s \n", input.c_str());
    if (chkPsswrd(input)){
      string first_password = input;
      cout << "Please input your password again: ";
      cin >> input;
      if (first_password == input){
        pword = input;
        return;
      } else {
        cout << "Sorry, those passwords do not match \n";
      }
    } else {
      cout << "Sorry, that password does not meet the requirments. Plese try again \n";
    }
  }
}


/*
show_info: Display the Customer's information
*/
void Customer::show_info(){
  printf("==================== DISPLAYING INFORMATION FOR %s ================== \n",
  lname.c_str());

  printf("************************** \n");
  printf("PHONE NUMBER: %lu \n", phone_num);
  printf("ADDRESS: %s \n", address.c_str());
  printf("PASSWORD: %s \n", pword.c_str());
  printf("NUMBER OF ACCOUNTS: %d \n", num_accounts);
  printf("************************** \n");
}


int main(){

  while(1){
    string inpt;
    showMainMenu();
    cin >> inpt;
    int usrChoice = stoi(inpt);
    if (usrChoice == 1){
      showAccountsMenu();
      cin >> inpt;
      Customer *c = verifyCustomer();
      int accChoice = stoi(inpt);
      if (accChoice == 1){
        chkDetails(c);
      } else if (accChoice == 2){
        addNewAccount(c);
      }
    } else if (usrChoice == 2) {
      showTrxMenu();
      cin >> inpt;
      Customer *c = verifyCustomer();
      int trxChoice = stoi(inpt);
      if (trxChoice == 1 || trxChoice == 2){
        handleDW(c, trxChoice);
      } else if (trxChoice == 3){
        handleTrnsfr(c);
      } else {
        continue;
      }
    } else if (usrChoice == 3){
      addNewCust();
    } else if (usrChoice == 4){
      cout << "EXITING PROGRAM \n";
      exit(0);
    } else {
      cout << "Sorry, that is an improper request";
    }
  }

  return 0;
}
