#pragma once

#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;


// Create a matrix class
class Matrix {
public:

	// Attributes
	int rows;
	int columns;
	double** matrix;

	// Methods
	void init(int r, int c);
	void randomize();
	void free();
	void scalarOp(double a, char op);
	void eWise(Matrix M, char op);
	void show();
	double* toArray();
	


};

// Create a Neural Network class

class Network {
public:


	//****** Attributes ********

	// Number of inputs, hidden nodes and outputs
	int numInputs;
	int numHidden;
	int numOutputs;

	// Define biases
	double biasH;
	double biasO;

	// Define learning rate
	double alpha;

	// Define matrices that contain weights from layer to layer
	Matrix weightsIH;
	Matrix weightsHO;

	// Define a matrix to hold results
	Matrix hiddenResults; // results from input->hidden
	Matrix outputResults; // results from hidden->output

	// Define matrices for output and hidden layer neuron errors
	Matrix outputError;
	Matrix hiddenError;

	// Define matrices for the delta of each of the weigths
	Matrix deltaWH; // deltaW for hidden layer
	Matrix deltaWI; // deltaW for the input layer 

	double mse;

	
	// ****** Methods ********
	void init(int inputs, int hidden, int outputs, double bias_h, double bias_o, double alphaVal);
	Matrix guessH(Matrix inputs);
	Matrix guessO(Matrix hidden);
	void feedForward(Matrix inputs);
	void calcError(Matrix targets); // Calculate the total output error
	void calcHError(); // calculate the error for the hidden layer
	void calcDeltaWH(); // calculate the deltaW for the hidden layer
	void calcDeltaWI(Matrix inputs); // calculate the deltaW for the input layer
	void adjustWeights(); // adjust the weights for both input->hidden and hidden->output
	void train(Matrix inputData, Matrix targets, int dataLen); // Train the model

};

