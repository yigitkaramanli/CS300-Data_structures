#include "UnorderedGraph.h"

using namespace std;
/*Apparently this is what you are supposed to do
if you want to use user defined structs with any container that uses std::hash for hashing.
Creating an overload of the hash template to make it hash my struct.
Credit to Rubens on StackOverflow.*/
template <>
struct hash<Operation>
{
	inline size_t operator()(const Operation& x) const {
		size_t value = hash<string>()(x.Word); // hash on the word. Hopefully its correct 
		return value;
	}
};

UnorderedGraph::~UnorderedGraph(){
}

/*Constructor*/
UnorderedGraph::UnorderedGraph(const string & filename) {
	string fileName = filename;
	BuildGraph(fileName);
}

/*Builds the graph by first reading the file into unordered map as keys,
checking all possible strings that can be reached by every key(word) and inserting the strings that are in the words.txt into the adjacency list.
Checking every possible word seems excessive but as the file gets larger, it should be faster than comparing every word with each other*/
void UnorderedGraph::BuildGraph(const string & filename) {
	string fileName = filename ,word,newWord,action;
	ifstream inFile(fileName);
	while (inFile >> word) {   //read the file into the unordered graph
		unordered_set< Operation > operations;
		graph[word] = operations;
	}
	for (auto x : graph) {    //iterate over the keys 
		string keyword = x.first;
		int length = keyword.length();
		//deletecheck
		for (int i = 0; i < length; i++) {   //for each char of the key 
			newWord = deleteChar(keyword, i);   //delete the char 
			if(graph.find(newWord)==graph.end())   // check if the newly created string is in the words.txt
			{ }
			else {									//if it is in words.txt, 
				action = "delete " + string{ keyword[i] } + " at position " + to_string(i + 1);
				Operation fin(newWord, action); //create an Operation object for the newly created string 
				graph[keyword].insert(fin);   //and insert it into keys unordered set
			}
		}
		//insertioncheck
		for (int i = 0; i < length+1; i++) {  //for each char of the key
			for (int j = 97; j < 125; j++) {  // for lowercase and '-','''
				newWord = insertChar(keyword, i, getChar(j)); //insert the char into a spot in the key 
				if(graph.find(newWord)== graph.end())  // check if the newly created string is in the list 
				{ }
				else {																			//if it is
					action = "insert " + string{ getChar(j) } + " after position " + to_string(i);
					Operation fin(newWord, action);												// create an Operation object for it 
					graph[keyword].insert(fin);													// insert it into keys unordered set
				}
			}
		}
		//substitutiion check
		for (int i = 0; i < length; i++) {   //for each char of the keyword
			for (int j = 97; j < 125; j++) {  // for lowercase and '-','''
				newWord = substitute(keyword, i, getChar(j)); //swap keyword[i] with char
				if (newWord == keyword)           //if newly created string is the same as the original one 
					continue;						// jump to the next iteration of the loop
				if( graph.find(newWord) == graph.end()) //check if the newly created string is in the list 
				{ }
				else {																			 // if it is 
					action = "change " + string{ keyword[i] } + " at position " + to_string(i+1) + " to " + getChar(j);
					Operation fin(newWord, action);											// create an Operation object for it
					graph[keyword].insert(fin);												// insert it into keys unordered set
				}
			}
		}
	}// at the end of this function, the graph is created in the form of an adjacency list
}
// function for insertion operation
string UnorderedGraph::insertChar(const string & Word, int index, char ch) {
	string word = Word;
	if (index < word.length()) {
		word.insert(index, string{ ch });
		return word;
	}
	else
		word.push_back(ch);
		return word;
}

// function for deletion operation
string UnorderedGraph::deleteChar(const string &Word, int index) {
	string word = Word;
	return (word.substr(0, index) + word.substr(index + 1));
}

// function for substitution operation
string UnorderedGraph::substitute(const string & Word, int index, char ch) {
	string word = Word;
	word[index] = ch;
	return word;
}
// public function, check if both of the words are in the list (i.e. input is valid)
bool UnorderedGraph::wordsExist(const string& word1, const string& word2) {
	if (graph.find(word1) == graph.end()) {
		return false;
	}
	else if (graph.find(word2) == graph.end()) {
		return false;
	}
	else {
		return true;
	}
}

char UnorderedGraph::getChar(int i) {
	if (i < 123) {
		return char(i);
	}
	else if( i == 124) {
		return '-';
	}
	else {
		return '\'';
	}
}

/*BFS. Finds the shortest path from one word to all other words in the graph
using the unweighted shortest path algorithm shown in lectures with slight modifications.
At the start 'nodes' private object is empty, at the end it contains every reachable node and where to reach it from */
void UnorderedGraph::unweightedDistances(const string & word) {
	queue<string> queue; 
	string wrd = word;
	queue.push(wrd);    // start iterating from the starting word
	Node newNode(word);
	nodes[word] = newNode;
	while (!queue.empty()) { // loop until no reachable 'unknown' is left on the graph
		wrd = queue.front();
		queue.pop();
		for (auto w : graph[wrd]) {  // loop over each word that is adjacent to node 'wrd'
			if (nodes.find(w.Word) == nodes.end()) { //if the node is not in the 'nodes' (aka its unkown)
				Node secondNewNode(w.Word);			// create a node for it 
				secondNewNode.path = wrd;			// set its path 
				nodes[w.Word] = secondNewNode;      // insert it into the 'nodes'
				queue.push(w.Word);					// push it into the queue, so that the vertexes adjacent to it will also be processed
			}
		}
	}
}

/*Constructs and prints the (hopefully) shortest path from word1 to word2*/
void UnorderedGraph::shortPath(const string& word1, const string& word2) {
	string start = word1, current = word2, path;
	stack<Operation> stack; // Stack for printing out the path
	
	unweightedDistances(start);  // BFS and find paths to every reachable vertex
	if (nodes.find(word2) == nodes.end()) {  //If word2 is NOT reachable from word1 
		cout << "No sequence of one letter transformations from " << word1 << " to " << word2 << " exist." << endl;
		return;
	}
	else {					// if word2 is reachable from word1
		while (path != start) {    //until you reach word1 
			path = nodes[current].path;  //start from word2 and follow its path 
			for (auto p : graph[path]) {  //iterate over the adjacent vertexes of the path
				if (p.Word == current) {  // to find the Operation that results in path -> current.(aka how you get current from path)
					stack.push(p);   // push the operation into the stack
					current = path;   //follow the path up
					break;
				}
			}
		}
	}
	cout << start << endl;
	while (!stack.empty()) {  //print out the path 
		cout << stack.top().Word << " (" << stack.top().action << ") "<< endl;
		stack.pop();
	}
	nodes.clear(); // clear 'nodes' so it can be used in consecutive searches.
}