//============================================================================
// Brent Mitchell
// Miniproject 1
// 2/11/2015
// C++ 199711
//============================================================================

#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <map>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <thread>
#define NUM_THREADS 1

using namespace std;

const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
map<char, int> characterNumbers;
//unordered_map<int, unordered_map<string, int>> dictionary;
unordered_map<string, int> dict;

int firstWordLength, keyLength;
string ciphertext;
clock_t t1,t2;

void mapCharacters() {
	for (int i = 0; i <= 26; i++) {
		characterNumbers[alphabet[i]] = i;
	}
}

void mapInput(const string input, int* keys) {
//	keys = new int[2];

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
		int ke = keys[i];
		characterNumber = (characterNumbers.find(ciphertext[i])->second
				- keys[i % keyLength] + 26) % 26;

		result += alphabet[characterNumber];


		// No need to continue if the first word is not in dictionary
		if (i == firstWordLength - 1) {
			unordered_map<string, int>::const_iterator it = dict.find(result);
//			cout << key << endl;
			if (it == dict.end()) {
//				cout << endl;
				return;
			}

		}

	}
	cout << "RESULT = ";
	cout << result << endl;
}

void findPlaintext(int threadId) {
	int limit = pow(26,keyLength);
	int *keys = new int [keyLength];
	for (int i = limit/NUM_THREADS*threadId; i < ceil(limit / ( threadId % (NUM_THREADS - 1) + 1)); i++) {
		string key;

		for (int j = keyLength - 1; j >= 0; j--) {
			int k = (int) (i / pow(26, j)) % 26;
			key += alphabet[k];
		}

		decrypt(ciphertext, key, firstWordLength, keys);
		key = "";
		//delete[] keys;
	}

}

int main(int argc, char *argv[]) {
	t1 = clock();
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

	// Removes spaces and make everything lowercase
	ciphertext.erase(std::remove(ciphertext.begin(), ciphertext.end(), ' '),
			ciphertext.end());
	transform(ciphertext.begin(), ciphertext.end(), ciphertext.begin(),
			::tolower);
	// Map the ciphertext to the correct numbers
	//mapInput(ciphertext);

	thread threads[NUM_THREADS];

	   for(int i=0; i < NUM_THREADS; i++ ){
	      cout << "main() : creating thread, " << i << endl;
	      threads[i] = thread(findPlaintext,i);
	   }

	    for(int i=0; i < NUM_THREADS; i++ ){
	      threads[i].join();

	   }



	t2 = clock();
	cout << "Done! " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC;


	return 0;
}

