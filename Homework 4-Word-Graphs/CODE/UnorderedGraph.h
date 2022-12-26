#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <queue>
#include <stack>
using namespace std;
#ifndef _UNORDERED_GRAPH
#define _UNORDERED_GRAPH
// struct that represents vertexes of the graph to be used for path creation
struct Node
{
	string name;  //Words 
	string path;  //Stores the name of the node that is on the shortest path from another node to this

	Node():name(""),path(""){}

	Node(const string & Name): name(Name),path("") {}

	bool operator==(const Node& rhs) {
		return (this->name == rhs.name &&  this->path == rhs.path);
	}
};
//Struct for storing operations inside the adjacency list
struct Operation {
	string Word; //word
	string action; //how to get it from any other word
	Operation() : Word(""), action(""){}

	Operation(const string & word, const string & action): Word(word), action(action){}

	bool operator==(const Operation& rhs) const
	{
		return (this->Word == rhs.Word && this->action == rhs.action);
	}
};

class UnorderedGraph
{
public:
	UnorderedGraph(const string & filename);  //constructor
	~UnorderedGraph();
	bool wordsExist(const string& word1,const string & word2); //to check if both inputs words are in the graph
	void shortPath(const string& word1, const string& word2);	// creating and displaying shortest path from word1 to word2

private:
	unordered_map <string, Node > nodes;
	unordered_map<string, unordered_set< Operation > > graph;  //Adjacency list that represents the graph. 
															// each key stores the words that can be reached with 1 operation and the operation
	char getChar(int a);
	string insertChar(const string & word, int index, char ch);
	string deleteChar(const string & word, int index);
	string substitute(const string & word, int index, char ch);
	void BuildGraph(const string & filename);   //GraphBuilder to construct the graph
	void unweightedDistances(const string& word); //finds the shortest path from word to all other words in the graph using BFS
};
#endif // !_UNORDERED_GRAPH
