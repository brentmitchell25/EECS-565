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

using namespace std;

const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
map<char, int> characterNumbers;
//unordered_map<int, unordered_map<string, int>> dictionary;
unordered_map<string, int> dict;
int *keys, keysLength;

clock_t t1,t2;

void mapCharacters() {
	for (int i = 0; i <= 26; i++) {
		characterNumbers[alphabet[i]] = i;
	}
}

void mapInput(const string input) {
	keys = new int[input.length()];
	keysLength = input.length();
	for (unsigned int i = 0; i < input.length(); i++) {
		keys[i] = characterNumbers.find(input[i])->second;
	}
}

void decrypt(const string ciphertext, const string key,
		const int firstWordLength) {
	int characterNumber;
	string result = "";
	mapInput(key);
	for (unsigned int i = 0; i < ciphertext.length(); i++) {
//	for (int i = 0; i < firstWordLength; i++) {
		characterNumber = (characterNumbers.find(ciphertext[i])->second
				- keys[i % keysLength] + 26) % 26;

		result += alphabet[characterNumber];

		/*
		// No need to continue if the first word is not in dictionary
		if (i == firstWordLength - 1) {
			unordered_map<string, int>::const_iterator it = dictionary.find(
					firstWordLength)->second.find(
					result.substr(0, firstWordLength));

			if (it == dictionary.find(firstWordLength)->second.end()) {
				return;
			}

		}
		*/

		// No need to continue if the first word is not in dictionary
		if (i == firstWordLength - 1) {
			unordered_map<string, int>::const_iterator it = dict.find(result);

			if (it == dict.end()) {
				return;
			}

		}

	}

	cout << result << endl;
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
	//	dictionary[word.length()].insert(make_pair(word, word.length()));
		dict[word] = word.length();
	}
	myfile.close();
	/*
	 for(map<int,map<string,int>>::const_iterator it = dictionary.begin();
	 it != dictionary.end(); ++it)
	 {
	 //    cout << it->first << " ";

	 for(map<string,int>::const_iterator it2 = it->second.begin();
	 it2 != it->second.end(); ++it2) {
	 cout << it2->first << " " << it2->second << endl;
	 }
	 }
	 */

	string ciphertext;
	int keyLength;
	int firstWordLength;
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
	mapInput(ciphertext);

	for (int i = 0; i < pow(26, keyLength); i++) {
		string key;
		for (int j = keyLength - 1; j >= 0; j--) {
			int k = (int) (i / pow(26, j)) % 26;
			key += alphabet[k];
		}

		decrypt(ciphertext, key, firstWordLength);
		delete[] keys;
	}

	/*
	 transform(result.begin(),result.end(),result.begin(),::toupper);
	 cout << (encrypt ? "Ciphertext = " : "Plaintext = ");
	 cout << result << endl;
	 */
	t2 = clock();
	cout << "Done! " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC;


	return 0;
}

