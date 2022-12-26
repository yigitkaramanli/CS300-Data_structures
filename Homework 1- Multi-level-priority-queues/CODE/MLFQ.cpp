// 28105-YigitKaramanli.cpp 

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Process
{
	string name;
	queue<string> q; 
	Process(string name) :
		name(name)
	{}
};

struct PriorityQueue
{
	string name;
	queue<Process> q;
	PriorityQueue(string name):
		name(name)
	{}

};



Process P_Creator(string foldername,int num) {
	ifstream file;
	string filename = "p" + to_string(num) + ".txt";
	file.open(foldername + "\\" + filename.c_str());
	Process proc("PC" + to_string(num));
	string dumb;
	while (getline(file, dumb)) {
		proc.q.push(dumb);
	}
	file.close();
	return proc;
}

bool IsEND(vector<PriorityQueue> Q) {
	for (int i = 0; i < Q.size(); i++) {
		if (!Q[i].q.empty()) {
			return false;
		}
	}
	return true;
}

bool ISprocEnd(Process p) {
	if (p.q.front() == "-") {
		return true;
	}
	return false;
}

void Idkwhat(ostream& os, vector <PriorityQueue>& Q , int num) {
	if (Q[num].q.front().q.front() == "1") {
		Q[num].q.front().q.pop();
		Process p = Q[num].q.front();
		Q[num].q.pop();
		if (ISprocEnd(p)) {
			os << "E, " << p.name << ", QX\n";
		}
		else {
			if (num == Q.size() - 1) {
				Q[num].q.push(p);
				os << "1, " << p.name << ", " << Q[num].name << "\n";
			}
			else {
				Q[num + 1].q.push(p);
				os << "1, " << p.name << ", " << Q[num + 1].name << "\n";
			}
		}
	}
	else {
		Q[num].q.front().q.pop();
		Process p = Q[num].q.front();
		Q[num].q.pop();
		if (ISprocEnd(p)) {
			os << "E, " << p.name << ", QX";
			if (!IsEND(Q)) {
				os << "\n";
			}

		}
		else {
			Q[num].q.push(p);
			os << "0, " << p.name << ", " << Q[num].name << "\n";
		}
	}

}

void reset(vector<PriorityQueue>& Q, ostream & os) {
	int cur = 1;
	for (int i = cur; i < Q.size(); i++) {
		while (!Q[i].q.empty()) {
			Process p = Q[i].q.front();
			Q[i].q.pop();
			Q[0].q.push(p);
			os << "B, " << p.name <<", " << Q[0].name << "\n";
		}
	}
}

int main() {

	string folder_name;
	cout << "Please enter the process folder name: ";
	cin >> folder_name;
	cout << "When all processes are completed, you can find execution sequence in \"" << folder_name + "/output.txt" << "\"\n";

	ofstream output;
	output.open(folder_name + "\\" + "output.txt",ios::app);
	ifstream conf_f;
	string conf = "configuration.txt";
	conf_f.open(folder_name + "\\"+conf.c_str());
	int num_queues, num_proc, S;
	vector<int> dumdum;

	string dumbie;
	while (getline(conf_f, dumbie)) {
		dumdum.push_back(stoi(dumbie));
	}
	conf_f.close();

	num_queues = dumdum[0];
	num_proc = dumdum[1];
	S = dumdum[2];

	vector<PriorityQueue> Mainqueue;
	for (int i = num_queues; i > 0; i--) {
		string q_name = "Q" + to_string(i);
		PriorityQueue Q(q_name);
		Mainqueue.push_back(Q);
	}

	for (int i = 1; i <= num_proc; i++) {
		Mainqueue[0].q.push(P_Creator(folder_name, i));
	}
	
	bool dummy = true;
	while (!IsEND(Mainqueue)) {

		int time_passed = 0;
		int current_Q = 0;
		while (time_passed < S && dummy) {
			if (!Mainqueue[current_Q].q.empty()) {
				Idkwhat(output, Mainqueue, current_Q);
				time_passed++;
			}
			else {
				current_Q++;
				if (IsEND(Mainqueue)) {
					dummy = false;
				}
			}
		}
		reset(Mainqueue, output);

	}
	output.close();
	return 0;

}