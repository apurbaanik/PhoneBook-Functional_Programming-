/*
Title: 5.2 The PhoneBook project redux with a functional programming approach.
Author: Anik Barua
Version: 1.0
Date: 05-27-2021

Description: In this C++ program, I wrote my own algorithms and structured them in a way that reflects 
functional programming style. It obtains the same results as HW 3.2 and I  created a universal myTransform 
template function that can accept a range of elements, do some operation on them based on a certain predicate, 
and output the result in another range. 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include<numeric>
using namespace std;

//Custom myTransform that accepts an extra type parameter which will be the predicate function.
template <class InputIterator, class OutputIterator, class UnaryOperator, class UnaryPredicate>
OutputIterator myTransform (InputIterator first1, InputIterator last1, OutputIterator result, UnaryOperator op, UnaryPredicate pred){
  while (first1 != last1) {
    if (pred(*first1)) {
        *result = op(*first1);
        ++result;
    }
    ++first1;
  }
  return result;
}

//Predicate is always true for this myTransform template
template <class InputIterator, class OutputIterator, class UnaryOperator>
OutputIterator myTransform (InputIterator first1, InputIterator last1, OutputIterator result, UnaryOperator op){
  while (first1 != last1) {
    *result = op(*first1); 
    ++result; ++first1;
  }
  return result;
}

//a myFold template function that uses recursion to avoid mutable state - a key feature of functional programming.
template<typename X, typename Y>
Y myFold(X iterBegin, X iterEnd, Y value){
    if (iterBegin != iterEnd){
        value += *iterBegin;
        return myFold(++iterBegin, iterEnd, value);
    }
    return value;
}


struct Line {
    string line;
    operator string() const { // defined so Line can be converted to string when inserting to set
        return line;
    }
};

istream& operator>>(istream& lhs, Line& rhs) {
    getline(lhs, rhs.line); 
    return lhs;
}

//Prints the elments in a vector
void vecPrint(vector<int> vec){
     for (auto &x : vec){
        cout << x << "\n";
    }
}

//Boolean function that returns true if the string equals to the number 212-536-6331
bool IsNumber(pair<int, string> x){
    return x.second == "212-536-6331";
}

//Boolean function that returns true if the substring equals to the area code of 212
bool IsCode(pair<int, string> x){
    return (x.second).substr(0, 3) == "212";
}

int main(){
    try {
    map<int, string> Map; //Map where the ID and numbers will be stored
    ifstream inFile{"phonebook.txt"}; //inFile object is created to open the phonebook.txt file

    if (!inFile.good()){
        throw string("Failure opening the phoneno.txt");
    }

    auto lambda1 = [] (string line) {
        int id = stoi(line.substr(0, 5)); 
        string number = line.substr(7, 12); 
        return make_pair(id, number);
    };

    //Calling myTransform with istream_iterator to read files directly into a map
    myTransform(istream_iterator<Line>{inFile}, istream_iterator<Line>{}, inserter(Map, Map.begin()), lambda1);

    vector<int> vecNumbers;
    auto lambda2 = [](pair<int, string> p) {
        if((p.second).substr(0, 3) == "347"){
            return 1;
        } else {
            return 0;
        }
    };

    //Call myTransform to generate a vector of ints which is 1 if a record has 347 number, and 0 otherwise. 
    myTransform(Map.begin(), Map.end(), inserter(vecNumbers, vecNumbers.begin()), lambda2);

    int areacodeCount = myFold(vecNumbers.begin(),vecNumbers.end(),0); //Using myFold to calculate the count of all 347 numbers.
    cout << "Count of phone # with area code 347: " << areacodeCount << endl;

    vector<int> vecOfID;
    auto lambda3 = [](pair<int, string> p) {
        return p.first;
    };

    //Call myTransform(with predicate) to produce a vector of ints holding those IDs with a specific phone# (212-536-6331)
    myTransform(Map.begin(), Map.end(), inserter(vecOfID, vecOfID.begin()), lambda3, IsNumber); 

    cout << "The ID(s) with the given phone# (212-536-6331): " << endl; 
    vecPrint(vecOfID); //Prints the ID(s) with the given phone number.
    
    vector<string> vecOf212;
    auto lambda4 = [](pair<int, string> p) {
        return p.second;
    };

    //Call myTransform(with predicate) to produce a vector of strings holding all 212 phone numbers.
    myTransform(Map.begin(), Map.end(), inserter(vecOf212, vecOf212.begin()), lambda4, IsCode); 
    cout << "Size of the vector with all (212) numbers: " << vecOf212.size() << endl; //prints vecOf212 size

    }catch (string message) {
        cerr << message << endl; //Prints out the error message
        exit(1);
    }
}