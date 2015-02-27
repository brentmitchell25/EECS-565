//============================================================================
// Brent Mitchell
// Miniproject 2
// 2/26/2015
// C++11
//============================================================================

#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <chrono>
#include <map>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <thread>
#define NUM_THREADS 16

using namespace std;
using namespace std::chrono; // Needed for recording execution time

const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
unordered_map<char, int> characterNumbers;
unordered_map<string, int> dict;

int firstWordLength, keyLength;
string ciphertext;


void mapCharacters() {
	for (int i = 0; i <= 26; i++) {
		characterNumbers[alphabet[i]] = i;
	}
}

void mapInput(const string input, int* keys) {

	for (unsigned int i = 0; i < input.length(); i++) {
		keys[i] = characterNumbers.find(input[i])->second;
	}
}

void decrypt(const string ciphertext, const string key,
		const int firstWordLength, int* keys) {
	int characterNumber;
	string result = "";
	mapInput(key, keys);

	for (unsigned int i = 0; i < ciphertext.length(); i++) {
		characterNumber = (characterNumbers.find(ciphertext[i])->second
				- keys[i % keyLength] + 26) % 26;

		result += alphabet[characterNumber];

		// No need to continue if the first word is not in dictionary
		if (i == firstWordLength - 1) {
			unordered_map<string, int>::const_iterator it = dict.find(result);
			if (it == dict.end()) {
				return;
			}
		}

	}
	cout << "RESULT = ";
	cout << result << endl;
	cout << "KEY = " << key << endl;
}

void findPlaintext(int threadId) {
	unsigned long limit = pow(26,keyLength);
	int *keys = new int [keyLength];
	unsigned long start = limit * threadId / NUM_THREADS;
	unsigned long finish = limit * (threadId  + 1) / NUM_THREADS;
	for (unsigned long i = start; i < finish; i++) {
		string key;

		// Creates and appends letters to create key
		for (int j = keyLength - 1; j >= 0; j--) {
			int k = (int) (i / pow(26, j)) % 26;
			key += alphabet[k];
		}

		decrypt(ciphertext, key, firstWordLength, keys);
		key = "";
	}
	delete [] keys;

}

int main(int argc, char *argv[]) {
	// Map alphabet to numbers starting with a = 0, b = 1, ...
	mapCharacters();

	ifstream myfile;

	// If no command line argument, open hard coded file
	if (argc > 1)
		myfile.open(argv[1]);
	else
		myfile.open("dict.txt");

	string word;
	while (!myfile.eof()) {
		getline(myfile, word);
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		dict[word] = word.length();
	}
	myfile.close();

	cout << "Ciphertext: ";
	getline(cin, ciphertext);
	cout << "Key Length: ";
	cin >> keyLength;
	cout << "First Word Length: ";
	cin >> firstWordLength;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	// Removes spaces and make everything lowercase
	ciphertext.erase(std::remove(ciphertext.begin(), ciphertext.end(), ' '),
			ciphertext.end());
	transform(ciphertext.begin(), ciphertext.end(), ciphertext.begin(),
			::tolower);

	thread threads[NUM_THREADS];

	   for(int i=0; i < NUM_THREADS; i++ ){
	      cout << "main() : creating thread, " << i << endl;
	      threads[i] = thread(findPlaintext,i);
	   }

	    for(int i=0; i < NUM_THREADS; i++ ){
	      threads[i].join();

	   }

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	cout << "Done!\nTime = " << duration / (double) 1000000 << endl;


	return 0;
}

