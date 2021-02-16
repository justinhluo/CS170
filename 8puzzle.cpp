//Justin Luo - 862013749
#include <iostream>
#include <vector>
#include <queue>
#include <bits/stdc++.h> 
using namespace std;
static const vector <int> goalmatrix = {1,2,3,4,5,6,7,8,0};

struct node {
	vector <int> state;
	int g_n;
	int h_n;
	int f_n;
	node(vector <int> v, int g_n, int h_n){ //constructor
		this->state = v;
		this->g_n = g_n;
		this->h_n = h_n;
		f_n = g_n + h_n;
	}
	void print() { //print the vector as a 3x3 matrix, omitting the 0
		cout << endl;
		for(int i = 0; i < state.size(); i++){
			if(state.at(i) == 0) {
				cout << "  ";
			}else{
				cout << state.at(i) << " ";
			}	
			if ((i+1) % 3 == 0){
				cout << endl;
			}
		}
		cout << endl;
		return;
	}
	int getg_n() {
		return g_n;
	}
	int geth_n() {
		return h_n;
	}
	int getf_n() {
		return f_n;
	}
	vector<int> getstate() {
		return state;
	}
};

int misplaced (vector <int> v) { //return h(n) using misplaced tile 
	int heuristic = 0;
	for(int i = 0; i < v.size(); ++i) {
		if (v.at(i) != goalmatrix.at(i) && v.at(i) != 0){ //if tile is misplaced, excluding the blank
			heuristic++;
		}
	}
	return heuristic;
}

int mhat_row (int n) { //helper function, returns correct row for a given number
	if(n <= 3) {
		return 1;
	}else if (n >= 4 && n <= 6) {
		return 2;
	}else{
		return 3;
	}
}

int mhat_col (int n) { //helper function, returns correct column for a given number
	if((n + 2) % 3 == 0){
		return 1;
	}else if((n + 1) % 3 == 0){
		return 2;
	}else{
		return 3;
	}
}

int manhattan (vector <int> v) { //return h(n) using manhattan distance 
	int heuristic = 0;
	int curr_row = 1;
	int curr_col = 1;
	for (int i = 0; i < v.size(); ++i){
		if(i != 0 && i % 3 == 0) { //set every thrid iteration as new row and reset column, but don't do this for first iteration
			curr_row ++;
			curr_col = 1;
		}
		if(v.at(i) != goalmatrix.at(i) && v.at(i) != 0) { //don't count blank as misplaced
			heuristic += (abs(mhat_row(v.at(i)) - curr_row) + abs(mhat_col(v.at(i)) - curr_col)); //formula for manhattan distance, consulted https://www.geeksforgeeks.org/sum-manhattan-distances-pairs-points/
		}
		curr_col++;
	}
	return heuristic;
}

bool operator<(const node& n1, const node& n2){ //overload to get the min(f_n) on top of priority queue, consulted https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
		return n1.f_n > n2.f_n;
}

vector<node> expandNode(node n, int qfunc){//returns a vector containing all children of a node. get h(n) from helper functions. increment g(n).
	vector<node> children;
	int temp;
	vector<int> temp_state;
	int newg_n = n.getg_n() + 1;
	int newh_n;
	vector<int> curr_state = n.getstate();
	int index = -1;
	for(int i = 0; i < curr_state.size(); ++i) { //find index of 0
		if(curr_state.at(i) == 0) {
			index = i;
			break;
		}
	}
	//each node has a max of 4 children(if the 0 is in the middle) and a min of 2 children(if the 0 is in a corner)
	if((index % 3) != 0){//if 0 is not in the first column (0,3,6) then we can move the 0 left
		temp_state = curr_state;
		temp = curr_state.at(index-1);
		temp_state.at(index-1) = 0;
		temp_state.at(index) = temp;
		if(qfunc == 1) {
			newh_n = 0; //hardcode h(n) to 0 for uniform cost search
		}else if(qfunc == 2) {
			newh_n = misplaced(temp_state);
		}else if(qfunc == 3) {
			newh_n = manhattan(temp_state);
		}
		node n(temp_state, newg_n, newh_n);
		children.push_back(n);
	}
	if(((index + 1) % 3) != 0){//if 0 is not in the last column (2,5,8) then we can move the 0 right
		temp_state = curr_state;
		temp = curr_state.at(index+1);
		temp_state.at(index+1) = 0;
		temp_state.at(index) = temp;
		if(qfunc == 1) {
			newh_n = 0;
		}else if(qfunc == 2) {
			newh_n = misplaced(temp_state);
		}else if(qfunc == 3) {
			newh_n = manhattan(temp_state);
		}
		node n(temp_state, newg_n, newh_n);
		children.push_back(n);
	}
	if(index > 2){//if 0 is not in the first row (0,1,2) then we can move the 0 up
		temp_state = curr_state;
		temp = curr_state.at(index-3);
		temp_state.at(index-3) = 0;
		temp_state.at(index) = temp;
		if(qfunc == 1) {
			newh_n = 0;
		}else if(qfunc == 2) {
			newh_n = misplaced(temp_state);
		}else if(qfunc == 3) {
			newh_n = manhattan(temp_state);
		}
		node n(temp_state, newg_n, newh_n);
		children.push_back(n);
	}
	if(index < 6){//if 0 is not in the last row (6,7,8) then we can move the 0 down
		temp_state = curr_state;
		temp = curr_state.at(index+3);
		temp_state.at(index+3) = 0;
		temp_state.at(index) = temp;
		if(qfunc == 1) {
			newh_n = 0;
		}else if(qfunc == 2) {
			newh_n = misplaced(temp_state);
		}else if(qfunc == 3) {
			newh_n = manhattan(temp_state);
		}
		node n(temp_state, newg_n, newh_n);
		children.push_back(n);
	}
	return children;
}

void generalsearch(vector<int> v, int qfunc){
	int max_queued = 1; //initial state always get enqueued
	int num_expanded = 0;
	int initial_heuristic;
	node temp_node(v, 0, 0);
	vector <node> temp_vec;
	if(qfunc == 1) {
		initial_heuristic = 0;
	}else if(qfunc == 2) {
		initial_heuristic = misplaced(v);
	}else if(qfunc == 3) {
		initial_heuristic = manhattan(v);
	}
	node initial_node(v,0,initial_heuristic); //create root node
	priority_queue<node> q;
	q.push(initial_node);
	while(1) {
		if(q.empty()){
			cout << "No solution" << endl;
			return;
		}
		temp_node = q.top();
		q.pop();
		if(temp_node.getstate() == goalmatrix) { //if popped node has goal state, then return, else expand it
			cout << "Found solution:" << endl;
			temp_node.print();
			cout << "Nodes expanded: " << num_expanded << endl;
			cout << "Max nodes enqueued: " << max_queued << endl;
			cout << "Depth of the goal node: " << temp_node.getg_n() << endl;
			return;
		}
		cout << "Best node to expand with g(n) = " << temp_node.getg_n() << " and h(n) = " << temp_node.geth_n() << endl;
		temp_node.print();
		cout << "expanding this node..." << endl; 
		num_expanded++;
		temp_vec = expandNode(temp_node, qfunc);
		for(int i = 0; i < temp_vec.size(); ++i) { //push children of expanded node into queue
			q.push(temp_vec.at(i));
		}
		if(q.size() > max_queued) { //update max_queue size
			max_queued = q.size();
		}
	}
}

int main() {
	int input;
	int input2;
	vector <int> matrix = {1,2,3,4,8,0,7,6,5};  //default puzzle, gets overwritten if user chooses to use custom puzzle
	vector <int> test = {0,1,2,3,4,5,6,7,8};
	vector <int> usermatrix;
	cout << "8 puzzle solver by Justin Luo" << endl;
	cout << "Press 1 to use default puzzle, 2 to use custom puzzle: ";
	cin >> input;
	if (input == 1) {
		cout << "Using default puzzle" << endl;
	} else if (input == 2) {
		int num;
		cout << "Separate each number with space or enter. Use only 1-8 one time each. Use 0 to represnt the blank space" << endl;
		for (int i = 0; i < goalmatrix.size(); ++i){
			cin >> num;
			usermatrix.push_back(num);
		}
		vector <int> temp = usermatrix;
		sort(temp.begin(), temp.end());
		if(temp != test) { // sort the vecotr user inputted and compare with test vector
			cout << "invalid puzzle! try again" << endl;
			exit(0);
		} 
		matrix = usermatrix;
	}else {
		cout << "Invalid input! try again" << endl;
		exit(0);
	}
	cin.ignore(numeric_limits<streamsize>::max(),'\n'); //clear input buffer in case user entered more numbers, consulted https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/
	cout << "Select algorithm: " << endl << "(1 for uniform cost, 2 for A* with misplaced tile heuristic, 3 for A* with manhattan distance heuristic): ";
	cin >> input2;
	if(input2 == 1) {
		generalsearch(matrix, 1);
	}else if(input2 == 2){
		generalsearch(matrix, 2);
	}else if(input2 == 3){
		generalsearch(matrix, 3);
	}else {
		cout << "Invalid input! try again" << endl;
		exit(0);
	}		
	return 0;
}