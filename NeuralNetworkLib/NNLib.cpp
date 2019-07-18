#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "NNLib.h"


using namespace std;

// Begin helper function prototypes
Matrix mMultiply(Matrix M, Matrix N, double bias);
Matrix transpose(Matrix M);
double randDouble(double low, double high);
double sigmoid(double x);
Matrix toColumn(double* arr, int len);
Matrix createData(int dataLen);
Matrix createTargets(Matrix data, int dataLen);

// End helper function prototypes


// ******************* BEGIN HELPER FUNCTION IMPLEMENTATIONS *******************

/*
mMultiply: multiply two matrices after checking to make sure they are of
proper dimensions
*/
Matrix mMultiply(Matrix M, Matrix N, double bias) {
	
	if (M.columns != N.rows) {
		cout << "Error in mMultiply: Matrices do not have correct dimensions \n";
		exit(1);
	}
	
	Matrix newMatrix;
	newMatrix.init(M.rows, N.columns);
	for (int i = 0; i < newMatrix.rows; i++) {
		for (int j = 0; j < newMatrix.columns; j++) {
			double sum = 0.0;
			for (int k = 0; k < M.columns; k++) {
				sum += M.matrix[i][k] * N.matrix[k][j];
			}
			newMatrix.matrix[i][j] = sum + bias;
		}
	}

	return newMatrix;
}


/*
transpose: return a new matrix that is the transpose of M
*/
Matrix transpose(Matrix M) {
	Matrix MT;
	MT.init(M.columns, M.rows);
	for (int i = 0; i < M.rows; i++) {
		for (int j = 0; j < M.columns; j++) {
			MT.matrix[j][i] = M.matrix[i][j];
		}
	}

	return MT;

}

/*
randDouble: generate a random double between low and high bounds 
 */
double randDouble(double low, double high) {

	double f = (double)rand() / RAND_MAX;
	return low + f * (high - low);
}


/*
sigmoid: return result of sigmoid normalization function on x
*/
double sigmoid(double x) {
	return x / (1 + abs(x));
}

/*
toColumn: convert an array of values into a column matrix
*/
Matrix toColumn(double* arr, int len) {
	Matrix column;
	column.init(len, 1);
	for (int i = 0; i < len; i++) {
		column.matrix[i][0] = arr[i];
	}
	return column;
}

/*
createData: create a dummy data set for testing
*/
Matrix createData(int dataLen) {
	Matrix dataMtx;
	dataMtx.init(dataLen, 2);
	for (int i = 0; i < dataLen; i++) {
		double rand1 = randDouble(0, 1);
		double rand2 = randDouble(0, 1);
		if (rand1 > 0.5) {
			dataMtx.matrix[i][0] = 1;
		}
		else {
			dataMtx.matrix[i][0] = 0;
		}

		if (rand2> 0.5) {
			dataMtx.matrix[i][1] = 1;
		}
		else {
			dataMtx.matrix[i][1] = 0;
		}

	}

	return dataMtx;

}

/*
createTargets: create known XOR targets based on the data
*/
Matrix createTargets(Matrix data, int dataLen) {
	Matrix target;
	target.init(dataLen, 1);
	for (int i = 0; i < dataLen; i++) {
		target.matrix[i][0] = (int)data.matrix[i][0] ^ (int)data.matrix[i][1];
		target.matrix[i][0] = (double)target.matrix[i][0];
	}
	return target;


}








// ******************* END HELPER FUNCTION IMPLEMENTATIONS *********************

// ******************* BEGIN MATRIX CLASS METHOD IMPLEMENTATIONS **********************

/*
init: initialize a new matrix by allocating space on the heap
*/
void Matrix::init(int r, int c) {
	rows = r;
	columns = c;
	matrix = new double* [r];
	for (int i = 0; i < r; i++) {
		matrix[i] = new double[c];
	}
}

/*
free: free the space allocated for the matrix
*/
void Matrix::free() {
	for (int i = 0; i < rows; i++) {
		delete matrix[i];
	}

	delete matrix;
}

/*
scalar: perform a scalar operation on the matrix
*/
void Matrix::scalarOp(double a, char op) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (op == '+') {
				matrix[i][j] += a;
			}
			else {
				matrix[i][j] *= a;
			}
		}
	}

}


/*
addElements: add another matrix element-wise after first checking to make sure
that it is the same dimensions as current matrix
*/
void Matrix::eWise(Matrix M, char op) {
	
	if ((M.rows != rows) || (M.columns != columns)) {
		cout << "Error in addElements: Matrices are not the same dimension \n";
		return;
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (op == '+') {
				matrix[i][j] += M.matrix[i][j];
			}
			else {
				matrix[i][j] *= M.matrix[i][j];
			}
		}
	}
}


/*
showMatrix: print Matrix data in a clean, understandable way
*/
void Matrix::show() {
	printf("ROWS: %d, COLUMNS: %d \n", rows, columns);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			printf("%f ", matrix[i][j]);
		}
		printf("\n");
	}

}

/*
randomize: fill matrix with random doubles
*/
void Matrix::randomize() {
	srand(time(0));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
		
			double d = randDouble(0, 1);
			matrix[i][j] = d;
		}
	}
}


// ************************* END MATRIX CLASS METHOD IMPLEMENTATIONS *******************


// ************************ BEGIN NETWORK CLASS METHOD IMPLEMENTATIONS *******************
/*
init: initialize a neural network with given number of inputs, hidden nodes, ouputs, as well
as biases bewteen each layer
*/
void Network::init(int inputs, int hidden, int outputs, double bias_h, double bias_o, double alphaVal) {
	numInputs = inputs;
	numHidden = hidden;
	numOutputs = outputs;
	biasH = bias_h;
	biasO = bias_o;
	alpha = alphaVal;
	
	// Initialize and randomize the weights
	weightsIH.init(numHidden, numInputs);
	weightsHO.init(numOutputs, numHidden);
	outputResults.init(numOutputs, 1);
	outputError.init(numOutputs, 1);
	hiddenError.init(numHidden, 1);
	hiddenError.init(numHidden, 1);
	weightsIH.randomize();
	weightsHO.randomize();
}


/*
guessH: given the vector of inputs, generate the vector of outputs that is the hidden layer
*/
Matrix Network::guessH(Matrix inputs) {
	Matrix hValues;
	hValues = mMultiply(weightsIH, inputs, biasH);
	
	for (int i = 0; i < hValues.rows; i++) {
		for (int j = 0; j < hValues.columns; j++) {
			hValues.matrix[i][j] = sigmoid(hValues.matrix[i][j]);
		}
	}
	hiddenResults = hValues;
	return hValues;
}

/*
guessO: given the column matrix of hidden layer values return the final output
*/
Matrix Network::guessO(Matrix hidden) {
	Matrix oValues;
	oValues = mMultiply(weightsHO, hidden, biasO);
	for (int i = 0; i < oValues.rows; i++) {
		for (int j = 0; j < oValues.columns; j++) {
			oValues.matrix[i][j] = sigmoid(oValues.matrix[i][j]);
		}
	}

	return oValues;
}

/*
feedForward: perform entire feed forward algorithm for one data point
*/
void Network::feedForward(Matrix inputs) {

	Matrix hidden = guessH(inputs);
	Matrix outputs = guessO(hidden);
	outputResults = outputs;
}



/*
calcError: calculate the total output error based on target matrix and output matrix
*/
void Network::calcError(Matrix target) {
	for (int i = 0; i < outputResults.rows; i++) {
		for (int j = 0; j < outputResults.columns; j++) {
			outputError.matrix[i][j] = target.matrix[i][j] - outputResults.matrix[i][j];
		}
	}
}


/*
calcHError: calculate the error for the weights from hidden layer to output layer
*/
void Network::calcHError() {
	for (int k = 0; k < numHidden; k++) {
		double errorTotal = 0.0;
		for (int i = 0; i < weightsHO.rows; i++) {
			double wSum = 0.0;
			for (int j = 0; j < weightsHO.columns; j++) {
				wSum += weightsHO.matrix[i][j];
			}
			double wRatio = weightsHO.matrix[i][k] / wSum;
			wRatio *= outputError.matrix[i][0];
			errorTotal += wRatio;
		}
		hiddenError.matrix[k][0] = errorTotal;
	}

}




/*
calcDeltaWH: calculate the deltaW for the hidden layer based on gradient descent algorithm
*/
void Network::calcDeltaWH() {
	Matrix sPrimeO; // A matrix that will hold sigma'(output)

	Matrix HT = transpose(hiddenResults);
	sPrimeO.init(numOutputs, 1);
	for (int i = 0; i < sPrimeO.rows; i++) {
		for (int j = 0; j < sPrimeO.columns; j++) {
			sPrimeO.matrix[i][j] = outputResults.matrix[i][j] + (1 - outputResults.matrix[i][j]);
		}
	}
	
	sPrimeO.eWise(outputError, '*');
	sPrimeO.scalarOp(alpha, '*');
	deltaWH = mMultiply(sPrimeO, HT, 0);
}





/*
calcDeltaWI: calculate the deltaW for the input layer based on gradient descent algorithm
*/
void Network::calcDeltaWI(Matrix inputs) {
	Matrix sPrimeH; // A matrix that will hold sigma'(output)
	Matrix IT = transpose(inputs);
	sPrimeH.init(numHidden, 1);
	for (int i = 0; i < sPrimeH.rows; i++) {
		for (int j = 0; j < sPrimeH.columns; j++) {
			sPrimeH.matrix[i][j] = hiddenResults.matrix[i][j] + (1 - hiddenResults.matrix[i][j]);
		}
	}

	sPrimeH.eWise(hiddenError, '*');
	sPrimeH.scalarOp(alpha, '*');
	deltaWI = mMultiply(sPrimeH, IT, 0);
	
}

/*
adjustWeights: adjust the weights for all layers based on calculated delta values
*/
void Network::adjustWeights() {
	weightsHO.eWise(deltaWH, '+');
	weightsIH.eWise(deltaWI, '+');
}


/*
train: Given a matrix of inputs, continually run the feedForward algorithm and adjust weights
*/
void Network::train(Matrix inputData, Matrix targetData, int dataLen) {
	double errorTotal = 0.0;
	for (int i = 0; i < dataLen; i++) {
		Matrix inputs = toColumn(inputData.matrix[i], numInputs);
		Matrix targets = toColumn(targetData.matrix[i], numOutputs);
		feedForward(inputs); // make guess
		calcError(targets); // calculate the output error
		errorTotal += (outputError.matrix[0][0] * outputError.matrix[0][0]);
		calcHError(); // calculate the hidden layer error
		calcDeltaWH(); // calculate the delta for hidden->output
		calcDeltaWI(inputs); // calculate the delta for input->hidden
		adjustWeights(); // adjust weights accordingly
	}
	
	mse = errorTotal / dataLen;

	printf("MEAN-SQUARED-ERROR: %f \n", mse);
	

}

// ************************* END NETWORK CLASS METHOD IMPLEMENTATIONS ****************************



int main()
{
	// Testing methods for Network class
	Network nn1;
	nn1.init(2, 2, 1, 0.01, 0.1, 0.25);
	cout << "*********** NEURAL NETWORK: NN1 ************* \n";
	printf("# of Inputs: %d, # of hidden layers: %d, # of outputs: %d \n",
		nn1.numInputs, nn1.numHidden, nn1.numOutputs);
	printf("Operating with Output Bias: %f, Hidden Bias: %f, Alpha-Val: %f \n",
		nn1.biasO, nn1.biasH, nn1.alpha);

	

	Matrix data1 = createData(1000);
	//data1.show();

	Matrix targetArr1 = createTargets(data1, 1000);
	//targetArr1.show();

	
	nn1.train(data1, targetArr1, 1000);

	return 0;
}


