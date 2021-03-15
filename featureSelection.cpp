#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <limits.h>
#include <algorithm>
using namespace std;

vector<vector<double>> readData() { //prompts user to enter file to read from and returns the data as a 3d array
	string filename;
	string str;
	vector<vector<double>> data;
	
	cout << endl << "Welcome to Justin Luo's feature selection algorithm "<< endl;
	cout << "Type in the name of file to test: ";
	
	while(1) { //loop until a file can be opened
		cin >> filename;
		ifstream inputfile(filename);
		if(!inputfile.is_open()) {
			cout << "Could not open file " << filename << ". Enter a differnt file: ";
		}else {
			while(getline(inputfile, str)){ //read line by line
				stringstream ss(str);
				double input;
				vector<double> line;
				while(ss >> input) { //read in each string of every line as a double 
					line.push_back(input);
				}
				data.push_back(line); //push line vector onto data vector
			}
			inputfile.close();
			return data;
		}
	}
}

void printdata(vector<vector<double>> data) { //prints data(for testing)
	for(int i = 0; i < data.size(); ++i){
		for (int j = 0; j< data.at(i).size(); ++j){
			cout << setprecision(8) << data.at(i).at(j) << " ";
		}
		cout << endl;
	}
}

double crossValidate(vector<vector<double>> data, vector <int> curr_features, int feature_to_add, int addFeat) {
	double accuracy = 0;
	int num_correctly_classified = 0;
	for (int i = 0; i < data.size(); ++i) {
		
		int label = data.at(i).at(0);
		
		vector <double> object_to_classify = data.at(i);

		double nearest_neighbor_distance = INT_MAX;
		
		int nearest_neighbor_location = INT_MAX;
		
		int nearest_neighbor_label;
		
		for(int k = 0; k < data.size(); ++k) {
			if(k != i) { //don't compare object with itself
				double add_feat_dist = pow(object_to_classify.at(feature_to_add) - data.at(k).at(feature_to_add), 2);
				double min_distance = 0;
				for (int j = 0; j < curr_features.size(); ++j) {
					min_distance += pow((object_to_classify.at(curr_features.at(j)) - data.at(k).at(curr_features.at(j))), 2);
				}
				if(addFeat) {
					min_distance += add_feat_dist;
				}
				min_distance = sqrt(min_distance);
				if(min_distance < nearest_neighbor_distance) {
					nearest_neighbor_distance = min_distance;
					nearest_neighbor_location = k;
					nearest_neighbor_label = data.at(nearest_neighbor_location).at(0);
				}
			}
		}
		if (label == nearest_neighbor_label) {
			num_correctly_classified++;
		}
	}
	accuracy = (double)num_correctly_classified / (double)data.size();
	return accuracy;
}			

void forwardSelection(vector<vector<double>> data) { //start with no features and add one by one
	cout << "Beginning search using Forward Selection!" << endl << endl;
	vector <int> curr_features; //stores the indices of added features
	vector <int> final_features;
	double final_accuracy = 0;
	double defaultrate;
	double class1cnt = 0;
	double class2cnt = 0;
	for (int i = 0; i < data.size(); ++i) {
		if(data.at(i).at(0) == 1) {
			class1cnt++;
		}else if(data.at(i).at(0) == 2) {
			class2cnt++;
		}
	}

	if (class1cnt > class2cnt){
		defaultrate = class1cnt/data.size();
	}else{
		defaultrate = class2cnt/data.size();
	}
	cout << "class 1 count: " <<  class1cnt <<endl;
	cout << "class 2 count: " <<  class2cnt <<endl;
	cout << "default rate accuracy = " << max(class1cnt,class2cnt) << "/" <<  data.size() << " = " << setprecision(3) << defaultrate * 100 << "%" << endl << endl;

	for(int i = 1; i < data.at(0).size(); ++i) {
		cout << "On level " << i << " of the search tree..." << endl;
		int feature_to_add;
		double best_accuracy = 0;
		
		for (int  k = 1; k < data.at(0).size(); ++k) {
			
			if(find(curr_features.begin(), curr_features.end(), k) == curr_features.end()) { //feature at index k is not in curr_features so we can add it
				
				cout << "  Using feature(s) {";
				for(int j = 0; j < curr_features.size(); ++j) { //will be empty first itereation, so just print k
					cout << curr_features.at(j) << ",";
				}
				cout << k << "}, ";
				double accuracy = crossValidate(data, curr_features, k, 1);
				cout << "accuracy is " << setprecision(3) << accuracy * 100 << "%" << endl;
				if (accuracy > best_accuracy) {
					best_accuracy = accuracy;
					feature_to_add = k;
				}
			}
		}
		curr_features.push_back(feature_to_add);
		if(i != data.at(0).size() - 1){ //only print this stuff if not on last iteration
			if(best_accuracy >= final_accuracy) { //final_accuracy = max(best_accuracy)
				final_accuracy = best_accuracy;
				final_features = curr_features;
			}else {
				cout << endl << "(Warning, accuracy has decreased! Continuing search in case of local maxima)";
			}
			cout << endl << "Feature set {";
			for(int j = 0; j < curr_features.size(); ++j) {
				if(j == curr_features.size() - 1) {
					cout << curr_features.at(j);
				}else {
					cout << curr_features.at(j) << ",";
				}
			}
			cout << "} was best, accuracy is " << setprecision(3) << best_accuracy * 100 << "%" << endl << endl;
		}
	}
	cout << endl << "Finished search! The best feature subset is {";
	for(int i = 0; i < final_features.size(); ++i){
		if(i == final_features.size() - 1) {
			cout << final_features.at(i);
		}else{
			cout << final_features.at(i) << ",";
		}
	} 
	cout << "}, which has an accuracy of " << setprecision(3) << final_accuracy * 100 << "%" << endl << endl;                                                          
}

void backwardElimination(vector<vector<double>> data) { //start with all features and remove one by one
	double defaultrate;
	double class1cnt = 0;
	double class2cnt = 0;
	for (int i = 0; i < data.size(); ++i) {
		if(data.at(i).at(0) == 1) {
			class1cnt++;
		}else if(data.at(i).at(0) == 2) {
			class2cnt++;
		}
	}

	if (class1cnt > class2cnt){
		defaultrate = class1cnt/data.size();
	}else{
		defaultrate = class2cnt/data.size();
	}
	cout << "class 1 count: " <<  class1cnt <<endl;
	cout << "class 2 count: " <<  class2cnt <<endl;
	cout << "default rate accuracy = " << max(class1cnt,class2cnt) << "/" <<  data.size() << " = " << setprecision(3) << defaultrate * 100 << "%" << endl << endl;
	cout << "Beginning search using Backwards Elimination!" << endl << endl;
	vector <int> curr_features;
	for(int i = 1; i < data.at(0).size(); ++i) { //push all features onto curr_features
		curr_features.push_back(i);
	}
	vector <int> final_features;
	double final_accuracy = 0;
	cout << "On level 1 of the search tree..." << endl; //hardcoding first iteration of loop
	cout << " Using feature(s) {";
	for(int i = 0; i < curr_features.size(); ++i) {
		if(i == curr_features.size() - 1) {
			cout << curr_features.at(i);
		}else {
			cout << curr_features.at(i) << ",";
		}
	}
	cout << "}, ";
	double accuracy = crossValidate(data, curr_features, curr_features.size(), 0);
	cout << "accuracy is " << setprecision(3) << accuracy * 100 << "%" << endl << endl;
	
	for(int i = 2; i < data.at(0).size(); ++i) {
		cout << "On level " << i << " of the search tree..." << endl;
		int feature_to_remove;
		double best_accuracy = 0;

		for(int k = 0; k < curr_features.size(); ++k) {
			vector<int>remove_one = curr_features; //testing removing one index at a time to find highest accuracy
			remove_one.erase(remove_one.begin() + k); //erase kth index of curr_features 
			cout << "  Using feature(s) {";
			for(int j = 0; j < remove_one.size(); ++j) { 		
				if(j == remove_one.size() - 1) {
					cout << remove_one.at(j);
				}else {
					cout << remove_one.at(j) << ",";
				}
			}
			cout << "}, ";
			double accuracy = crossValidate(data, remove_one, k, 0);
			cout << "accuracy is " << setprecision(3) << accuracy * 100 << "%" << endl;
			if (accuracy > best_accuracy) {
				best_accuracy = accuracy;
				feature_to_remove = k;
			}
		}
		curr_features.erase(curr_features.begin() + feature_to_remove); //erase kth index of curr_features where removing k yielded the highest accuracy.
		if(best_accuracy >= final_accuracy) { 
			final_accuracy = best_accuracy;
			final_features = curr_features;
		}else {
			cout << endl << "(Warning, accuracy has decreased! Continuing search in case of local maxima)";
		}
		cout << endl << "Feature set {";
		for(int j = 0; j < curr_features.size(); ++j) {
			if(j == curr_features.size() - 1) {
				cout << curr_features.at(j);
			}else {
				cout << curr_features.at(j) << ",";
			}
		}
		cout << "} was best, accuracy is " << setprecision(3) << best_accuracy * 100 << "%" << endl << endl;
	}
	cout << "Finished search! The best feature subset is {";
	for(int i = 0; i < final_features.size(); ++i){
		if(i == final_features.size() - 1) {
			cout << final_features.at(i);
		}else{
			cout << final_features.at(i) << ",";
		}
	} 
	cout << "}, which has an accuracy of " << setprecision(3) << final_accuracy * 100 << "%" << endl << endl;                                                          
}

int main () {
	int input, numFeatures, numInstances;
	vector<vector<double>> data = readData();
	cout << endl << "Type the number of the algorithm you want to run." << endl << endl;
	cout << "1) Forward Selection" << endl << "2) Backward Elimination" << endl << endl;
	cin >> input;
	while(input!=1 && input!=2) {
		cout << "Invaid input, try again" << endl << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');
		cin >> input;
	}
	numFeatures = data.at(0).size() - 1; //assuming each instance has same amount of features
	numInstances = data.size();
	cout << endl << "The dataset has " << numFeatures << " features (not including the class attribute), with " << numInstances << " instances." << endl << endl;
	cout << "Running nearest neighbor with all " << numFeatures << " features, using \"leaving-one-out\" evaluation, I get an accuracy of ";
	vector<int> features;
	for(int i = 1; i < data.at(0).size() - 1; ++i) {
		features.push_back(i);
	}
	cout << setprecision(3) << crossValidate(data, features, features.size() + 1, 1) * 100 << "%" << endl << endl;
	if (input == 1) {
		forwardSelection(data);
	} else if(input == 2) {
		backwardElimination(data);
	}
	return 0;
}