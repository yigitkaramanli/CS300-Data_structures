#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

bool isEmpty(string *arr, const int& index) {  // checking if the index is empty or not 
	return arr[index] == "";
}

int getcode(string* arr, const int & code, const int & size) {  // did not really use this function but who knows
	int index = code % size;
	while (!isEmpty(arr, index)) {
		index++;
	}
	return index;
}
void insert(string *arr,const string& element,const int& index	) { // insert 
	if (index < 4096) {
		arr[index] = element;
	}
}

int main() {
	const int size = 4096;
	string *arr = new string [size]; // create the array dynamically 
	string dummy;
	for (int i=0 ; i < 256; i++) {  // insert the first 256 characters from the ASCII table
		dummy = unsigned char(i);
		insert(arr, dummy, getcode(arr,i, size));
	}
	ifstream input;
	ofstream output;
	string filename = "compout"; // input file 
	input.open(filename.c_str());
	output.open("decompout"); // output file 
	int curr, prev = 0;
	int count = 256;
	string tobeins;
	while (input >> curr) {
		if (isEmpty(arr, curr)) { // if the code is not in the directory 
			insert(arr, arr[prev] + arr[prev][0], count); //insert new key to the directory ( Prev_text + firstchar(prev_text)
			count++;
			output << arr[curr]; // output decompression 
			prev = curr; //current code will be the previous in the next iteration 
		}
		else { // if the code is already in the directory
			if (prev == 0) { // exception for the first code since its previos is NULL
				tobeins = arr[curr][0];
				output << arr[curr];
				prev = curr;
			}
			else { 
				output << arr[curr]; //decompress value 
				tobeins = arr[prev] + arr[curr][0]; 
				prev = curr;
				insert(arr, tobeins, count); //insert prev_text + first_char(current text) into the directory
				count++;
			}
			
		}

	}
	delete[] arr; //delete dictionary
	return 0;
}