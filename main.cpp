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

//merge sort
//void merge(vector<Movie>& arr, int left, int mid, int right, function<bool(const Movie&, const Movie&)> comp);
//void mergeSort(vector<Movie>& arr, int left, int right, std::function<bool(const Movie&, const Movie&)> comp);
void shellSort(std::vector<Movie>& arr, std::function<bool(const Movie&, const Movie&)> comp);



//GENERATE A NUMBER OF MOVIES WITH RANDOM ATTRIBUTES BETWEEN 1-3000
void generate_movies_and_write_to_csv(int num_movies, const std::string& filename) {
    std::ofstream file(filename);
    std::mt19937 rng(time(0)); // random number generator seeded with time
    std::uniform_int_distribution<int> dist(1, 3000); // distribution from 1 to 20
    std::ostringstream name_stream;

    // Write CSV header
    file << "Name,Drama,Fantasy,Action,Comedy,Fiction\n";

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
    generate_movies_and_write_to_csv(3000, "movies.csv");
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
        //cout << entry.second.similarityScore << endl;
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

   
    vector<Movie> vec;
    for (const auto& iter : movies) {
        vec.push_back(iter.second);
    }
    int n = vec.size();
    
    //quickSort(movies, 0, movies.size() - 1, [](const Movie& a, const Movie& b) {
    //    return a.action < b.action; 
    //    });
    clock_t t;
    t = clock() - t;
    if (input == 1) {
        shellSort(vec, [](const Movie& a, const Movie& b) {
            return a.drama < b.drama; 
            });
        t = clock() - t;
    }
    else if (input == 2) {
        shellSort(vec, [](const Movie& a, const Movie& b) {
            return a.comedy < b.comedy; // Sorting in ascending order
            });
    }
    else if (input == 3) {
        shellSort(vec, [](const Movie& a, const Movie& b) {
            return a.action < b.action; // Sorting in ascending order
            });
    }
    else if (input == 4) {
        shellSort(vec, [](const Movie& a, const Movie& b) {
            return a.fiction < b.fiction; // Sorting in ascending order
            });
    }

    //
    //int n = vec.size();
    //quickSort(vec, 0, n - 1);
    //mergeSort(vec, 0, n - 1);
    printVector(vec);

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



// Function to swap position of elements
void swap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

// Function to print elements of a vector
void printVector(vector<Movie>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i].printData();
    }
}

int partition(vector<Movie>& arr, int low, int high, function<bool(const Movie&, const Movie&)> comp) {
    Movie pivot = arr[high]; // Taking the last element as pivot
    int i = low - 1; // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than the pivot
        if (comp(arr[j], pivot)) {
            i++; // increment index of smaller element
            std::swap(arr[i], arr[j]); // Swap arr[i] and arr[j]
        }
    }
    std::swap(arr[i + 1], arr[high]); // Swap the pivot element with the element at i + 1
    return (i + 1);
}

void quickSort(std::vector<Movie>& arr, int low, int high, std::function<bool(const Movie&, const Movie&)> comp) {
    if (low < high) {
        int pi = partition(arr, low, high, comp); // pi is partitioning index

        quickSort(arr, low, pi - 1, comp); // Recursively sort elements before partition
        quickSort(arr, pi + 1, high, comp); // Recursively sort elements after partition
    }
}


//void merge(vector<Movie>& arr, int left, int mid, int right, function<bool(const Movie&, const Movie&)> comp) {
//    int n1 = mid - left + 1;
//    int n2 = right - mid;
//
//    std::vector<Movie> X(n1), Y(n2);
//
//    for (int i = 0; i < n1; i++)
//        X[i] = arr[left + i];
//    for (int j = 0; j < n2; j++)
//        Y[j] = arr[mid + 1 + j];
//
//    int i = 0, j = 0, k = left;
//    while (i < n1 && j < n2) {
//        if (comp(X[i], Y[j])) {
//            arr[k] = X[i];
//            i++;
//        }
//        else {
//            arr[k] = Y[j];
//            j++;
//        }
//        k++;
//    }
//
//    while (i < n1) {
//        arr[k] = X[i];
//        i++;
//        k++;
//    }
//
//    while (j < n2) {
//        arr[k] = Y[j];
//        j++;
//        k++;
//    }
//}
//
//void mergeSort(vector<Movie>& arr, int left, int right, std::function<bool(const Movie&, const Movie&)> comp) {
//    if (left < right) {
//        int mid = left + (right - left) / 2;
//
//        mergeSort(arr, left, mid, comp);
//        mergeSort(arr, mid + 1, right, comp);
//
//        merge(arr, left, mid, right, comp);
//    }
//}


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
