

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>

using namespace std;

// Define global variables
int wordLen;
string currString;
int numChances = 5;
string word;

// Begin function prototypes
vector<string> getWords();
int genRandomNum(int low, int high);
string getTarget(vector<string> words, int len);
int welcome();
void runGuess(char guess);
char getGuess();
void runGame();
// End function prototypes


/*
getWords: Read in the words from the file and save them in a string vector
*/
vector<string> getWords() {
	vector<string> retVec;
	string word;
	ifstream infile("words.txt");
	while (getline(infile, word)) {
		retVec.push_back(word);
	}

	return retVec;
}


/*
genRandomNum: generate a random number between the given range
Range will likely be range of all printable ASCII chars, but this may change based on
performance
*/
int genRandomNum(int low, int high) {

	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(low, high); // define the range

	return distr(eng);
}

/*
getWord: get a word from the list based on requested length
*/
string getTarget(vector<string> words, int len) {
	while (1) {
		int rndIndx = genRandomNum(0, words.size());
		string possWord = words[rndIndx];
		if (possWord.length() == len) {
			word = possWord;
			return possWord;
		}
		else {
			continue;
		}
	}
}












/*
welcome: welcome the player to the game and ask the number of chars
for their first word
*/
int welcome() {
	printf("****************** WELCOME TO HANGMAN ***************** \n");
	while (1) {
		string usrInpt;
		cout << "Please enter your desired word length (must be greater than 5): ";
		cin >> usrInpt;
		int len = stoi(usrInpt);
		if (len <= 5) {
			cout << "Word must be longer than 5 characters \n";
		} else {
			return len;
		}
	}
}



/*
showGame: show a current game state:
- Number characters guess correctly and their positions
- How many guesses left
Also update the current string
*/
void runGuess(char guess) {
	printf("You guessed: %c \n", guess);
	printf("******************** \n");
	int wordLen = word.length();
	for (int i = 0; i < wordLen; i++) {
		if (word[i] == guess) {
			printf("%c ", word[i]);
			currString[i] = guess;
		}
		else {
			printf("%c ",currString[i]);
		}
	}
	printf("\n");
	printf("******************** \n");
	printf("You have %d guesses left \n", numChances);


}

/*
getGuess: request a guess from the user
reject any guesses that are not in the defined ASCII range
*/
char getGuess() {
	string usrInpt;
	printf("Would you like to guess the word? (y/n) ");
	cin >> usrInpt;
	if (usrInpt == "y") {
		cout << "Please make your guess. Choose wisely: ";
		cin >> usrInpt;
		if (usrInpt == word) {
			cout << "Congragulations! You escaped with your life \n";
			exit(0);
		}
		else {
			cout << "Sorry that is incorrect. You have died \n";
			printf("The word was %s \n", word.c_str());
			exit(0);
		}
	}
	else {
		cout << "Please enter your guess: ";
		cin >> usrInpt;
		numChances--;
		char buf[1] = { '0' };
		for (int i = 0; i < usrInpt.length(); i++) {
			buf[i] = usrInpt[i];
		}
		return buf[0];
	}

}

/*
runGame: run the game
*/
void runGame() {
	int wordLen = welcome();
	// Get a word from the word list
	cout << "Please wait while we choose a word... \n";
	vector<string> wordList = getWords();
	word = getTarget(wordList, wordLen);
	currString = word;
	// Change currString to underscores
	for (int i = 0; i < wordLen; i++) {
		currString[i] = '_';
	}
	while (numChances > 0) {
		char guess = getGuess();
		runGuess(guess);
	}
	printf("Unforunately, you have died. The word was: %s \n", word.c_str());
	string usrInpt;
	cout << "Would you like to play again? (y/n): ";
	cin >> usrInpt;
	if (usrInpt == "y") {
		runGame();
	}
	else {
		exit(0);
	}
}



int main()
{
	// Testing the whole game
	runGame();

	return 0;
}


