thtt#include <string>
#include <iostream>
#include <fstream>
#include "hashtable.h"
#include <vector>
using namespace std;

int main() {
	KeyCodePair Notfound;
	HashTable<KeyCodePair>* table= new HashTable <KeyCodePair>(Notfound, 4096); //Create the dictionary
	char ch;
	string dummy;
    
	KeyCodePair Temp1;
    
	for (int i = 0; i < 256; i++) {   //insert the first 256 characters from the ASCII table
		dummy = unsigned char(i);
		Temp1 = KeyCodePair(dummy, i);
		if (i == 0) {
			table->insert(Temp1);
		}
		else {
			Temp1 = KeyCodePair(dummy, i);
			table->insert(Temp1);
		}
	}
	int codecounter = 256;
	ifstream deneme;
	string filename = "compin";  // input file
	deneme.open(filename.c_str());
	ofstream sonuc;
	sonuc.open("compout");  // output file
	KeyCodePair Temp,wtf;
	string tempstring = "";
	string loopstring;
	while (!deneme.eof()) //eof returns true if next character 
	 //is eof 
	{
		loopstring = tempstring; // assign tempkey into the loop var to check the substrings
		deneme.get(ch);//get command reads next character 
		loopstring.push_back(ch); // append character to the end of the loopstring
		Temp.Key = loopstring;
		Temp.code = 0;
		if (table->inTable(Temp)) { //if the key is already in the dictionary
			tempstring = Temp.Key;  // assign the key into the temp string to get carried into the next iteration of the loop 
		}
		else { // if the key is not in the dictionary
			wtf.Key = tempstring; // compress the tempstring (prev_text) 
			sonuc << (table->find(wtf)).code << " "; // output compression
			Temp.code = codecounter; // assign
			codecounter++;
			table->insert(Temp); // insert prev_text (tempstring) + next char into the dictionary
			tempstring = Temp.Key[Temp.Key.length() - 1]; 
		}
		if (deneme.eof()) {  //if the file ended
			tempstring = tempstring.substr(0,tempstring.length() - 1);
			Temp.Key = tempstring;
			if (table->inTable(Temp)) { // if the last string is in the dictionary thus is not compressed and outputted when the loop ends
				sonuc << table->find(Temp).code << " "; // output the last substr
			}
		}
		
	}
	
	deneme.close();  //close the files
	sonuc.close();
	delete table;  //deallocate  the table 


	return 0;
}
