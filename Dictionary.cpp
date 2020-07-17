
// Author: Samy Barakat
// Assignment: 02
// C++ Dictionary


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>


using namespace std;

map<string, map<string, vector<string>>> wordbatch;			// Will be word , parts of speech, and description


  // Here, we get our data from dataSource

void recData() {
	fstream ioFile;
	vector<string> temp;
	string textf = "Data.CS.SFSU.txt";

	cout << "! Opening data file... ./" << textf << endl;
	ioFile.open(textf, ios::in);

	if (ioFile.fail())
		cout << "Fail" << endl;

	string line;
	int x = 0;

	cout << "! Loading data..." << endl;
	while (getline(ioFile, line)) {
		temp.push_back(line);

		// Increment here

		x++;
	}
	cout << "! Loading completed..." << endl; // Printage for file

	ioFile.close();
	cout << "! Closing data file... ./" << textf << endl;

	// Here, we store our data in map
	// Loop
	// Using auto to deduce in initilization

	for (auto line = temp.begin(); line != temp.end(); ++line) {


		// Here, we cut down string from the textf to keyword as well as others

		vector<string> tok;
		string s = *line;
		size_t x = 0;
		while ((x = s.find("|")) != std::string::npos) {
			tok.push_back(s.substr(0, x));
			s.erase(0, x + 1);
		}
		tok.push_back(s);


		map <string, vector<string>> tempPartosDes;

		for (auto l = tok.begin() + 1; l != tok.end(); ++l) {

			string s2 = *l;

			string pos = s2.substr(0, s2.find(" -=>> "));				// Here, we have part of speech

			s2.erase(0, s2.find(" -=>> ") + 6);

			string des = s2;


			map <string, vector<string>>::iterator it;

			it = tempPartosDes.find(pos);

			if (it != tempPartosDes.end()) {

				// Part of speech(pos) does exist in map(tempPosDes)

				vector<string> tempVec = it->second;
				tempVec.push_back(des);
				tempPartosDes.erase(it);


				tempPartosDes.insert(pair<string, vector<string>>(pos, tempVec));
			}
			else {														// Part of speech(pos) does not exist in map(tempPosDes)
				vector<string> tempVec;
				tempVec.push_back(des);


				tempPartosDes.insert(std::pair<string, vector<string>>(pos, tempVec));
			}
		}

		// Here, added to global map for storage

		wordbatch.insert(pair<string, map<string, vector<string>>>(tok[0], tempPartosDes));		//keyword, map of pos and des

	} // We end data storage in map
}


// Here, we check the input words and print the output

void process(string io) {
	string partOfSpeech[] = { "adjective", "adverb", "conjunction", "interjection", "noun", "preposition", "pronoun", "verb" }; //all parts of speech
	vector<string> tok;
	size_t x = 0;
	transform(io.begin(), io.end(), io.begin(), ::tolower);
	while ((x = io.find(" ")) != std::string::npos) {
		tok.push_back(io.substr(0, x));
		io.erase(0, x + 1);
	}
	tok.push_back(io);

	// Here, only first three words are important

	string first, second, third;
	bool secondAvailable = false;
	bool thirdAvailable = false;
	bool isFound = false;
	bool isPos = false;
	bool isDistinct = false;

	// Switch tok (Checkpoint)

	first = tok[0];
	tok.erase(tok.begin());

	// Here, we check to see if there is a second word

	if (!tok.empty()) {
		second = tok[0];
		tok.erase(tok.begin());
		secondAvailable = true;
	}

	// Here, we check to see if there is a third word

	if (!tok.empty() && secondAvailable) {
		third = tok[0];
		tok.erase(tok.begin());
		thirdAvailable = true;
	}

	// Here, we check if second word is inside the wordbatch

	for (map <string, map<string, vector<string>>>::iterator it = wordbatch.begin(); it != wordbatch.end(); ++it) {
		if (first.compare(it->first) == 0)
			isFound = true;
	}

	if (thirdAvailable && third.compare("distinct") == 0) {
		isDistinct = true;
	}

	if (secondAvailable) {

		// Here, we check if second word is part of speech
		// Loop

		for (int i = 0; i < 8; i++) {
			if (second.compare(partOfSpeech[i]) == 0)
				isPos = true;
		}
		// Here, we check if distinct

		if (!isPos && second.compare("distinct") == 0)
			isDistinct = true;
	}

	// Here, we print the output

	if (!isFound)
		cout << "    <Not found>" << endl;
	else if (secondAvailable && !isPos && !isDistinct)
		cout << "    <2nd argument must be a part of speech or \"distinct\">" << endl;
	else {

		// Loop 

		for (map <string, map<string, vector<string>>>::iterator it = wordbatch.begin(); it != wordbatch.end(); ++it) {

			// Statement

			if (first.compare(it->first) == 0) {
				map<string, vector<string>> tempMap = it->second;

				// Loop

				for (map<string, vector<string>>::iterator it2 = tempMap.begin(); it2 != tempMap.end(); ++it2) {

					// Statement

					if (isPos) {
						// Here, the user input particular part of speech
						if (second.compare(it2->first) == 0) {							// Here, the user input particular part of speech
							vector<string> v = it2->second;
							sort(v.begin(), v.end());
							if (isDistinct)
								v.erase(unique(v.begin(), v.end()), v.end());
							string s = it->first;
							if (isdigit(s[s.length() - 1])) {						    // For situations as CSC340, CSC220...
								transform(s.begin(), s.end(), s.begin(), ::toupper);
							}
							else {
								s[0] = toupper(s[0]);									// This is for uppercase first letter
							}
							for (auto i = v.begin(); i != v.end(); ++i)
								cout << "    " << s << " [" << it2->first << "] : " << *i << endl;
						}
					}
					else {
						vector<string> v = it2->second;
						sort(v.begin(), v.end());
						if (isDistinct)
							v.erase(unique(v.begin(), v.end()), v.end());
						string s = it->first;
						if (isdigit(s[s.length() - 1])) {								// For situations as CSC340, CSC220...
							transform(s.begin(), s.end(), s.begin(), ::toupper);
						}
						else {
							s[0] = toupper(s[0]);									    // This is for uppercase first letter
						}
						for (auto i = v.begin(); i != v.end(); ++i)
							cout << "    " << s << " [" << it2->first << "] : " << *i << endl;
					}

				}
			}
		}
	}
}


int main()
{
	recData();
	string io = "";
	cout << "\n-----DICTIONARY 340 C++ -----\n" << endl;
	while (io.compare("!Q") != 0) {
		cout << "Search: ";
		getline(cin, io);
		if (io.compare("!Q") == 0) {
			cout << "-----THANK YOU-----" << endl;
		}
		else {
			cout << "   |" << endl;
			process(io);
			cout << "   |" << endl;
		}
	}
	return 0;
}