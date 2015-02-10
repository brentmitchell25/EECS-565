//============================================================================
// Name        : CasearCipher.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <iterator>
#include <algorithm>
using namespace std;

const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
map<char, int> characterNumbers;
int *keys;
int keysLength;

void mapCharacters() {
	for (int i = 0; i < 26; i++) {
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

char numberCharacter(int n) {
	return alphabet[n];
}

string encryptDecrypt(const string plaintext, const string ciphertext, const bool encrypt) {
	int key;
	string result = "";
	for (unsigned int i = 0; i < plaintext.length(); i++) {
		if(encrypt) {
		key = (characterNumbers.find(plaintext[i])->second
				+ keys[i % keysLength]) % 26;
		} else {
			key = (characterNumbers.find(plaintext[i])->second
				- keys[i % keysLength] + 26) % 26;

		}
		result += numberCharacter(key);
	}
	return result;
}

int main(int argc, char *argv[]) {
	// Map alphabet to numbers starting with a = 0, b = 1, ...
	mapCharacters();

	string ciphertext;
	string plaintext;
	string ed;
	bool encrypt;
	cout << "Plaintext: ";
	getline(cin, plaintext);
	cout << "Ciphertext: ";
	getline(cin, ciphertext);
	cout << "(E)ncrypt or (D)ecrypt?: ";
	getline(cin, ed);
	transform(ed.begin(),ed.end(),ed.begin(),::tolower);
	if(ed == "e" || ed == "encrypt")
		encrypt = true;
	// Removes spaces and make everything lowercase
	plaintext.erase(std::remove(plaintext.begin(), plaintext.end(), ' '),
			plaintext.end());
	transform(plaintext.begin(),plaintext.end(),plaintext.begin(),::tolower);

	ciphertext.erase(std::remove(ciphertext.begin(), ciphertext.end(), ' '),
			ciphertext.end());
	transform(ciphertext.begin(),ciphertext.end(),ciphertext.begin(),::tolower);

	// Map the ciphertext to the correct numbers
	mapInput(ciphertext);


	cout << encryptDecrypt(plaintext,ciphertext,encrypt);

	delete[] keys;
	return 0;
}
