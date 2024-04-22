#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include <random>
#include <ctime>
#include <string>
#include <array>
#include <sstream>
#include <functional>
#include <ctime>
#include "movie.h"
#include <cmath>
using namespace std;


void swap(int& a, int& b);
void printVector(vector<Movie>& vec);
int partition(vector<Movie>& arr, int low, int high, function<bool(const Movie&, const Movie&)> comp);
void quickSort(std::vector<Movie>& arr, int low, int high, std::function<bool(const Movie&, const Movie&)> comp);


void shellSort(std::vector<Movie>& arr, std::function<bool(const Movie&, const Movie&)> comp);


// YOU CAN UNCOMMENT THE FOLLOWING CODE AND UNCOMMENT THE FIRST LINE IN MAIN TO GENERATE NEW RANDOM DATA
/*
//GENERATE A NUMBER OF MOVIES WITH RANDOM ATTRIBUTES BETWEEN 1-3000
void generate_movies_and_write_to_csv(int num_movies, const std::string& filename) {
    std::ofstream file(filename);
    std::mt19937 rng(time(0)); // random number generator seeded with time
    std::uniform_int_distribution<int> dist(1, 3000); // distribution from 1 to 20
    std::ostringstream name_stream;

    // Write CSV header
    file << "Name,Drama,Comedy,Action,Fiction\n";

    for (int i = 0; i < num_movies; i++) {
        name_stream.str(""); 
        name_stream << "Movie-" << i + 1; //Generate unique name

 
        Movie movie = {
            name_stream.str(),
            dist(rng), // Drama ranking
            dist(rng), // Comedy
            dist(rng), // Action
            dist(rng)  // Fiction ranking
        };

      
        file << name_stream.str() << ","
            << movie.drama << ","
            << movie.comedy << ","
            << movie.action << ","
            << movie.fiction << "\n";
            //<< movie.fantasy << "\n";
    }

    file.close(); 
}
*/

//comparison for movie names
//Only works due to the way movies are named in this specific project
struct MovieCompare {
    bool operator()(const string& a, const string& b) const {
        string prefix_a = a.substr(0, a.find("-"));
        string prefix_b = b.substr(0, b.find("-"));
        if (prefix_a == prefix_b) {
            int num_a = stoi(a.substr(a.find("-") + 1));
            int num_b = stoi(b.substr(b.find("-") + 1));
            return num_a < num_b;
        }
        return prefix_a < prefix_b;
    }
};

void loadData(string fileName, map<string, Movie, MovieCompare>& data);

int main() {
    //generate_movies_and_write_to_csv(3000, "movies.csv");
    map<string, Movie, MovieCompare> movies;
    loadData("movies.csv", movies);
    
    int drama = 0;
    int dramaRank;

    int comedy = 0;
    int comedyRank;

    int action = 0;
    int actionRank;

    double fiction = 0;
    int fictionRank;
    int _similarityScore = 0;

    cout << "Please enter the desired drama: ";
    cin >> drama;
    cout << "Please enter desired ranking for drama (1-4): ";
    cin >> dramaRank;

    cout << "Please enter the desired comedy: ";
    cin >> comedy;
    cout << "Please enter desired ranking for comedy (1-4): ";
    cin >> comedyRank;

    cout << "Please enter the desired action: ";
    cin >> action;
    cout << "Please enter desired ranking for action(1-4): ";
    cin >> actionRank;

    cout << "Please enter the desired fiction: ";
    cin >> fiction;
    cout << "Please enter desired ranking for fiction (1-4): ";
    cin >> fictionRank;

    for (auto& iter : movies) {
        _similarityScore = abs(iter.second.drama - drama) / dramaRank
            + abs(iter.second.comedy - comedy) / comedyRank
            + abs(iter.second.action - action) / actionRank
            + abs(iter.second.fiction - fiction) / fictionRank;
        iter.second.similarityScore = _similarityScore;
    }

    int min = 10000;
    string key = "";


    for (const auto& entry : movies) {
        if (entry.second.similarityScore < min) {
            min = entry.second.similarityScore;
            key = entry.first;
        }
    }

    cout << "The most suitable movie for you is: " << endl;
    auto it = movies.find(key);
    Movie similar = it->second;
    similar.printData();

    int input;
    cout << "What would you like to sort the movies by? \n" <<
        "1. Drama \n" <<
        "2. Comedy \n" <<
        "3. Action \n" <<
        "4. Fiction" << endl;
    cin >> input;
    //

   
    //We are using two vectors here with the same exact data in order to get accurate time readings for Shell Sort and Quick Sort
    vector<Movie> vec; //this vector will be used for Shell Sort
    vector<Movie> vec1; // This vector will be used for Quick Sort
    for (const auto& iter : movies) {
        vec.push_back(iter.second);
        vec1.push_back(iter.second);
    }
    int n = vec.size();
    
    clock_t shellTime = clock();
    clock_t quickTime = clock();

    if (input == 1) {
        shellSort(vec, [](const Movie& a, const Movie& b) {
            return a.drama < b.drama; 
            });
        shellTime = clock() - shellTime;
        quickTime = clock();
        quickSort(vec1, 0, movies.size() - 1, [](const Movie& a, const Movie& b) {
                return a.drama < b.drama; 
                });
    }
    else if (input == 2) {
        shellSort(vec, [](const Movie& a, const Movie& b) {
            return a.comedy < b.comedy; 
            });
        shellTime = clock() - shellTime;
        quickTime = clock();
        quickSort(vec1, 0, movies.size() - 1, [](const Movie& a, const Movie& b) {
            return a.comedy < b.comedy;
            });
    }
    else if (input == 3) {
        shellSort(vec, [](const Movie& a, const Movie& b) {
            return a.action < b.action; 
            });
        shellTime = clock() - shellTime;
        quickTime = clock();
        quickSort(vec1, 0, movies.size() - 1, [](const Movie& a, const Movie& b) {
            return a.action < b.action;
            });
    }
    else if (input == 4) {
        shellSort(vec, [](const Movie& a, const Movie& b) {
            return a.fiction < b.fiction; 
            });
        shellTime = clock() - shellTime;
        quickTime = clock();
        quickSort(vec1, 0, movies.size() - 1, [](const Movie& a, const Movie& b) {
            return a.fiction < b.fiction;
            });
    }

    quickTime = clock() - quickTime;
    printVector(vec); //We can change vec to vec1 to display the results of quick sort instead of shell sort
    cout << "It took " << shellTime << " nano seconds to sort the data using Shell Sort" << endl;
    cout << "It took " << quickTime << " nano seconds to sort the data using Quick Sort" << endl;
    return 0;
}

void loadData(string fileName, map<string, Movie, MovieCompare>& data) {
    ifstream file(fileName);
    string line = "";
    string movie;
    int drama;
    int comedy;
    int action;
    int fiction;

    getline(file, line);
    int count = 1;
    while (true) {
        while (count != 5 && getline(file, line, ',')) {
            
            if (count == 1) {
                movie = line;
                
            }
            else if (count == 2) {
                drama = stoi(line);
              
            }
            else if (count == 3) {
                comedy = stoi(line);
      
            }
            else if (count == 4) {
                action = stoi(line);
            }
            count++;
        }
        if (count == 5 && getline(file, line)) {
            fiction = stoi(line);
            count = 1;
            Movie newData = Movie(movie, drama, comedy, action, fiction);
            data.emplace(movie, newData);
            continue;
        }

        break;
    }
    file.close();
}



void swap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

void printVector(vector<Movie>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i].printData();
    }
}

int partition(vector<Movie>& arr, int low, int high, function<bool(const Movie&, const Movie&)> comp) {
    Movie pivot = arr[high]; 
    int i = low - 1; 

    for (int j = low; j <= high - 1; j++) {
        if (comp(arr[j], pivot)) {
            i++; 
            std::swap(arr[i], arr[j]); 
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(std::vector<Movie>& arr, int low, int high, std::function<bool(const Movie&, const Movie&)> comp) {
    if (low < high) {
        int pi = partition(arr, low, high, comp); 

        quickSort(arr, low, pi - 1, comp);
        quickSort(arr, pi + 1, high, comp); 
    }
}



void shellSort(std::vector<Movie>& arr, std::function<bool(const Movie&, const Movie&)> comp) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Movie temp = arr[i];
            int j;
            for (j = i; j >= gap && comp(temp, arr[j - gap]); j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}
