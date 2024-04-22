#pragma once
#pragma once
#include <iostream>
using namespace std;

struct Movie {
	string name;
	int drama;
	int comedy;
	int action;
	int fiction;
	int similarityScore;

	//public:
	Movie();
	Movie(string _name, int _drama, int _comedy, int _action, int _fiction);

	void printData();

};