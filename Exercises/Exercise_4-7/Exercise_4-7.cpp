// Exercise_4-7.cpp : TA simple module for calculating basic statistics for a given data set
//
// Calculates:
//		Mean
//		Median
//		Mode
//
// Author: Aaron Grisez

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

const int MAX_DATA_SIZE = 2048;			// Largest number of input characters to read
const int MAX_DELIMITER_SIZE = 4;		// Largest number of characters to include in the delimiter

namespace helpers {

	// GET FUNCTIONS
	//////////////////////////////////

	/*Okay, these aren't really *real* getters because there are no classes being defined,
	  but the point is still kind of the same.*/

	int getVectorSize() {
		// Prompts user to input vector size and reads int
		int size = 0;
		std::cout << "Please enter the size of the desired vector." << std::endl;
		std::cin >> size;
		return size;
	}

	double getVectorComponent() {
		// Prompts user to input vector component and reads int
		double component = 0.0;
		std::cout << "Please enter a vector component." << std::endl;
		std::cin >> component;
		return component;
	}

	double getVectorComponentFromString(std::string& token) {
		return std::stod(token);
	}

	std::vector<std::string> getInputStrings() {
		// Prompts user to input vector as delimited string
		char line[MAX_DATA_SIZE * sizeof(char)];
		char delimiter[MAX_DELIMITER_SIZE];

		// User input
		std::cout << "Please enter the delimiter you would like to use." << std::endl;
		std::cin.getline(delimiter, MAX_DELIMITER_SIZE);
		std::cout << "Please enter all vector components separated by your delimiter." << std::endl;
		std::cin.getline(line, MAX_DATA_SIZE * sizeof(char));

		// Separate the input string into tokens delimited by the given delimiter
		std::vector<std::string> tokens;
		char *token = strtok(line, delimiter);	// Set the first token
		while (token != NULL) {
			tokens.push_back(token);			// Add the current token to the list of tokens in string
			token = strtok(NULL, delimiter);	// Set the next token
		}
		return tokens;
	}

	// SET FUNCTIONS
	//////////////////////////////////

	void setVectorByComponent(std::vector<double>& vector) {
		const int size = helpers::getVectorSize();

		for (int i = 0; i < size; i++) {
			vector.push_back(helpers::getVectorComponent());
		}
		// Sort the vector once since multiple computations require sorting
		std::sort(vector.begin(), vector.end());
	}

	void setVectorByString(std::vector<double>& vector) {
		std::vector<std::string> inputStrings = getInputStrings();

		for (std::vector<std::string>::iterator it = inputStrings.begin(); it != inputStrings.end(); ++it) {
			vector.push_back(helpers::getVectorComponentFromString(*it));
		}
		// Sort the vector once since multiple computations require sorting
		std::sort(vector.begin(), vector.end());
	}

	// CALCULATIONS
	//////////////////////////////////

	double mean(std::vector<double>& v) {
		// Calculates the mean of the vector
		std::vector<double>::size_type size = v.size();

		double temp = 0.0;
		
		for (std::vector<double>::iterator it = v.begin(); it != v.end(); ++it) {
			temp += *it;
		}

		return temp / size;
	}

	double median(std::vector<double> v, bool sorted = false) {
		// Calculates the median of the vector
		if (!sorted) {
			std::sort(v.begin(), v.end());
		}
		if (v.size() % 2 == 0) {
			// If the data set has an even number of elements, the median is the average of the two center elements
			int middle = (v.size() - 1) / 2;
			return (v[int(middle)] + v[int(middle) + 1]) / 2;
		}
		else {
			// If the data set has an odd number of elements, the median is the center element
			int middle = v.size() / 2;
			return v[int(middle)];
		}
	}

	std::vector<double> mode(std::vector<double>& v, bool sorted = false) {
		// Calculates the mode of the vector

		std::vector<double> modes = {};
		double headValue = NULL;
		int maxCounts = 0;
		int temp = 0;

		for (std::vector<double>::iterator it = v.begin(); it != v.end(); ++it) {
			// Iterates through values in v counting them along the way
			temp = std::count(v.begin(), v.end(), *it);
			if (temp > maxCounts) {		// Triggered if value is counted more times than the current max
				modes.clear();			// Remove all previously calculated modes, they are now invalid
				headValue = *it;		// Set the current candidate value for mode
				maxCounts = temp;		// Set the current maximum count number
				modes.push_back(*it);	// Add current head value to vector of potential modes
			}
			else if (temp == maxCounts && std::find(modes.begin(), modes.end(), *it) == modes.end()) {
				// This block is only triggered if another element is found the same number of times in the data
				// AND the element wasn't previously included in the vector of potential modes
				modes.push_back(*it);
			}
		}
		return modes;
	}
}

namespace tests {

	void testMean() {

		std::vector<double> test = { 1.5, 2.5, 3.5 };
		double _case = 2.5;
		assert(helpers::mean(test) == _case);

	}

	void testMedian() {

		std::vector<double> test = { 1.5, 3.5, 2.5 };
		double _case = 2.5;
		assert(helpers::mean(test) == _case);

	}

	void testMode() {

		std::vector<double> test1 = { 1.5, 2.5, 2.5, 3.5, 1.5, 1.5, 2.5, 2.5, 1.5, 9, 2.5 };
		std::vector<double> _case1 = { 2.5 };
		assert(helpers::mode(test1)[0] == _case1[0]);

		std::vector<double> test2 = { 1.5, 2.5, 2.5, 3.5, 1.5, 1.5, 2.5, 2.5, 1.5, 9, 2.5 , 1.5};
		std::vector<double> _case2 = { 1.5, 2.5 }; // RIGHT NOW THIS TEST IS VERY FRAGILE; NEEDS TO BE UPDATED TO SUPPORT ANY ORDERING IN MODE VECTOR
		for (unsigned i = 0; i < _case2.size(); i++) {
			assert(helpers::mode(test2)[i] == _case2[i]);
		} 
	}

}

int main()
{
	// Driver function to calculate statistics
	
	// Run tests for good measure
	tests::testMean();
	tests::testMedian();
	tests::testMode();

	// Declare the main vector used for calculation
	std::vector<double> vector;

	// This code can be changed based on the desired behavior. If the components are to be gathered one by one, uncomment the next line
	// Default behaviour is for the user to enter the entire data vector with their chosen delimiter all at once

	//helpers::setVectorByComponent(vector);		// Alternate Mode: Enter components one by one
	helpers::setVectorByString(vector);				// Default mode: Enter vector all at once with chosen delimiter

	// Calculate Lower Moments
	std::cout << "Average is " << helpers::mean(vector) << std::endl;
	std::cout << "Median is " << helpers::median(vector, true) << std::endl;
	std::vector<double> modes = helpers::mode(vector, true);
	std::cout << "Mode is: ";
	for (std::vector<double>::iterator it = modes.begin(); it != modes.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;

	// TODO: Implement higher moments (Variance/Standard Deviation, Skewness, Kurtosis, etc.)
}