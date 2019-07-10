#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <random>
#include <ctime>

using namespace std;


// ********************** BEGIN FUNCTION PROTOTYPES *************************
// Setup functions
int genRandomNum(int low, int high);
string genRandomStr(int low, int high);
double randZeroOne();
void initPop(int n);

// Fitness functions
double calcFitness(string member);
void genFitness();

// Reproduction functions
string performCrossover(string parent1, string parent2);
string mutate(string child);
vector<string> genMatingPool();
void evolve();

// Miscellanious functions
double avgFitness();
bool checkPop();
string bestString();
// ********************** END FUNCTION PROTOTYPES ****************************

// ********************* BEGIN GLOBAL VARIABLE INITIALIZATIONS ****************
vector<string> population; // vector of strings that contains the population
vector<double> fitness; // vector of doubles containing fitness values for each string
string target = "they were roommates"; // target phrase
double mutationRate = 1;
int generations = 0;




// *********************** BEGIN SETUP FUNCTION IMPLEMENTATIONS ******************************
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
genRandomStr: generate a random string of length with the ASCII value of each char between low and high
*/
string genRandomStr(int low, int high, int length) {
	string randStr;
	for (int i = 0; i < length; i++) {
		randStr.push_back((char)(genRandomNum(low, high)));
	}

	return randStr;
}


/*
randZeroOne: generate a random double between 0 and 1
*/
double randZeroOne() {
	srand((unsigned)time(NULL));
	return ((double)rand() / (double)RAND_MAX);
}





/*
initPop: initialize a population of n elements, each of which is a string containing 20 random chars
*/
void initPop(int n) {
	string element;
	for (int i = 0; i < n; i++) {
		element = genRandomStr(31, 123, 20);
		population.push_back(element);
	}

}

// ***************************** END SETUP FUNCTION IMPLEMENTATIONS *****************************

// **************************** BEGIN FITNESS FUNCTION IMPLEMENTATIONS ***************************

/*
calcFitness: calculate the fitness of one member of the population by calculating the percetage
of chars that match the target phrase
*/
double calcFitness(string member) {
	int score = 0;
	int len = target.length();
	for (int i = 0; i < len; i++) {
		if ((int)member[i] == (int)target[i]) {
			score++;
		}
	}

	return (double)score / (double)len;
}

/*
genFitness: calculate fitness scores for every member of the population and save them into global vector
*/
void genFitness() {
	fitness.clear();
	int n = population.size();
	for (int i = 0; i < n; i++) {
		double fitnessScore = calcFitness(population[i]);
		fitness.push_back(fitnessScore);
	}
}






// *************************** END FITNESS FUNCTION IMPLEMENTATIONS *******************************

// ************************** BEGIN REPRODUCTION FUNCTION IMPLEMENTATIONS *************************

/*
performCrossover: given two strings, pick a random midpoint and take half from one string and
half from another
*/
string performCrossover(string parent1, string parent2) {
	int midpoint = genRandomNum(0, target.length());
	int len = target.length();
	string child;
	for (int i = 0; i < len; i++) {
		if (i < midpoint) {
			child.push_back(parent1[i]);
		}
		else {
			child.push_back(parent2[i]);
		}
	}

	return child;
}


/*
mutate: given a mutation rate, if a random number is below the mutation rate, then replace one of the chars
in the child string with a random char
*/
string mutate(string child) {
	int len = child.length();
	for (int i = 0; i < len; i++) {
		if (genRandomNum(0,200) < mutationRate) {
			child[i] = (char)(genRandomNum(31, 123));
		}
	}

	return child;

}

/*
genMatingPool: generate the mating pool based of the fitness of the population
members which are more fit will be placed into the mating pool more frequently, hence
increasing their chances of being chosen for reproduction
*/
vector<string>genMatingPool() {
	vector<string> mPool;
	int popSize = population.size();
	for (int i = 0; i < popSize; i++) {
		double memFitness = calcFitness(population[i]);
		int n = round((memFitness * 100));
		for (int j = 0; j < n; j++) {
			mPool.push_back(population[i]);
		}
	}
	return mPool;
}

/*
evolve: fill in the population vector with the next generation by performing crossover and mutation
*/
void evolve() {
	vector<string> mPool = genMatingPool();
	int popSize = population.size();
	for (int i = 0; i < popSize; i++) {
		int a = genRandomNum(0, mPool.size() - 1);
		int b = genRandomNum(0, mPool.size() - 1);
		string parentA = mPool[a];
		string parentB = mPool[b];
		string child = performCrossover(parentA, parentB);
		child = mutate(child);
		population[i] = child;
	}
	generations++;
}

// ************************** BEGIN REPRODUCTION FUNCTION IMPLEMENTATIONS *************************



// *************************** BEGIN HELPER FUNCTION IMPLEMENTATIONS ******************************
/*
avgFitness: calculate the average fitness of the population
*/
double avgFitness() {
	double totalFitness = 0.0;
	genFitness();
	int len = fitness.size();
	for (int i = 0; i < len; i++) {
		totalFitness += fitness[i];
	}

	return (double)totalFitness / (double)len;
}


/*
checkPop: check the population for a match
*/
bool checkPop() {
	int popSize = population.size();
	for (int i = 0; i < popSize; i++) {
		if (population[i] == target) {
			return true;
		}
	}
	return false;
}

/*
bestString: return the member of the population with the highest fitness
*/
string bestString() {

	double maxFitness = 0.0;
	int maxIndx = 0;
	int len = fitness.size();
	for (int i = 0; i < len; i++) {
		if (fitness[i] > maxFitness) {
			maxFitness = fitness[i];
			maxIndx = i;
		}
	}

	return population[maxIndx];
}








int main()
{

	// Create the initial population
	initPop(1000);

	// Begin main loop
	bool fullyEvolved = false;
	string finalString;
	double finalFitness = 0.0;
	while (!fullyEvolved) {
		printf("GENERATION: %d, AVG FITNESS: %f, BEST STRING: %s\n", generations, avgFitness(),
			bestString().c_str());

		evolve();

		if (checkPop()) {
			printf("FOUND IT!!!!! \n");
			fullyEvolved = true;
			finalString = bestString();
			finalFitness = avgFitness();
		}

	}

	printf("FINAL GENERATION: %d, FINAL AVG FITNESS: %f, FINAL STRING: %s\n", generations, finalFitness,
		target.c_str());

	
	


	return 0;
}

