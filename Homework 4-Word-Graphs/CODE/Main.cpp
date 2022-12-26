#include <iostream>
#include<string>
#include "UnorderedGraph.h"
using namespace std;

//COPYRIGHT YIGIT KARAMANLI
//It takes for a file with 345.000 words around 1 minutes and 20 seconds(in release mode) to be processed into a graph. (it takes around 40 mins in debug mode)
//So if the input file is large, please be patient, It should work eventually.
int main() {
	string filename = "words.txt";   //Hardcoded filename.
	cout << "Processing file... This may take some time. Please do not kill me.\n";
	UnorderedGraph graph(filename); // create the graph
	cout << "Processing has finished.\n";
	string word1, word2;
	while (1) {
		cout << "Enter two words to be processed: ";
		cin >> word1 >> word2; // take input 
		if (word1[0] == '*') { 
			cout << "Terminating. GOOD BYE\n";
			break;
		}
		else {
			if (!graph.wordsExist(word1, word2)) { //input check
				cout << "At least one of the words is not in the list. Try again.\n";
			}
			else {
				graph.shortPath(word1, word2);
			}
			cout << "\n";
		}
	}
	return 0;
}