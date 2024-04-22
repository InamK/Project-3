#include "Movie.h"


Movie::Movie() {
	name = "";
	drama = 0;
	comedy = 0;
	action = 0;
	fiction = 0;
	similarityScore = 0;
}

Movie::Movie(string _name, int _drama, int _comedy, int _action, int _fiction) {
	name = _name;
	drama = _drama;
	comedy = _comedy;
	action = _action;
	fiction = _fiction;
	similarityScore = 0;
}

void Movie::printData() {
	cout << "Name:" << name << "\t\t";
	cout << "Drama: " << drama << "\t";
	cout << "Comedy: " << comedy << "\t";
	cout << "Action " << action << "\t";
	cout << "Fiction " << fiction << endl;

}