#include <iostream>
#include <string>
#include "GameField.h"
#include "Console.h"
#include "windows.h"

using namespace std;

#define DEFAULT_ITERATIONS 1000
#define DEFAULT_OUTPUT_FILENAME string("a.txt")
#define DEFAULT_INPUT_FILENAME string("default.txt")


enum modeCodes {
	DEFAULT_MODE = 1,
	ONLINE_MODE = 2,
	OFFLINE_MODE = 3
};

void input(int argc, char* argv[], int& mode, int& iterations, string& input_filename, string& output_filename);


int main(int argc, char* argv[])
{
	GameField map;
	Console console_input(argc, argv);
	console_input.gameFieldInitialization(map);
	map.printMap();
	map.dump(DEFAULT_OUTPUT_FILENAME);
	return 0;
}

void input(int argc, char* argv[], int& mode, int& iterations, string& input_filename, string& output_filename) {
	ErrorOutput errout;
	if (argc < 2) {	//only programm
		mode = DEFAULT_MODE;
		input_filename = DEFAULT_INPUT_FILENAME;
		return;
	}
	else if (argc < 3) {	//only programm and input_filename
		input_filename = argv[1];	
		mode = ONLINE_MODE;
		return;
	}
	else {
		input_filename = argv[1];
		mode = OFFLINE_MODE;
	}
	int is_iterations = 0, is_output_filename = 0;
	for (int i = 2; i < argc; i++) {        
		if (strlen(argv[i]) > 1) {
			if (argv[i][0] == '-' && argv[i][1] == 'i') {	//-i
				if (is_iterations == 0) is_iterations = 1;
				else{
					errout.writeToConsole(MULTIPLE_ITERATIONS);
				}
				if (i + 1 < argc) {
					iterations = stoi(argv[i + 1]);
					i++;
				}
				else {
					errout.writeToConsole(INCORRECT_ITERATIONS);
				}
			}
			else if (argv[i][0] == '-' && argv[i][1] == 'o'){	//-o
				if (is_output_filename == 0) is_output_filename = 1;
				else {
					errout.writeToConsole(MULTIPLE_ITERATIONS);
				} 
				if (i + 1 < argc) {
					output_filename = argv[i + 1];
					i++;
				}
				else {
					errout.writeToConsole(INCORRECT_INPUT_FILENAME);
				}
			}
			else if (strlen(argv[i]) >= 13 && string(argv[i]).substr(0, 13) == string("--iterations=")) {		//--iterations=
				if (is_iterations == 0) is_iterations = 1;
				else {
					errout.writeToConsole(MULTIPLE_ITERATIONS);
				}
				if (strlen(argv[i]) == 13) {
					errout.writeToConsole(INCORRECT_ITERATIONS_LONG);
				}
				else {
					iterations = stoi(string(argv[i]).substr(13));
				}
			}
			else if (strlen(argv[i]) >= 9 && string(argv[i]).substr(0, 9) == string("--output=")) {			//--output=
				if (is_output_filename == 0) is_output_filename = 1;
				else {
					errout.writeToConsole(MULTIPLE_ITERATIONS);
				}
				if (strlen(argv[i]) == 9) {
					errout.writeToConsole(INCORRECT_INPUT_FILENAME_LONG);
				}
				else {
					output_filename = string(argv[i]).substr(9);
				}
			} 
			else {
				errout.writeToConsole(INCORRECT_PARAMETER);
			}
		}
	}
	if (is_iterations == 0) {
		iterations = DEFAULT_ITERATIONS;
		errout.writeToConsole(NONINITIALIZED_ITERATIONS);
	}
	if (is_output_filename == 0) {
		output_filename = DEFAULT_OUTPUT_FILENAME;
		errout.writeToConsole(NONINITIALIZED_INPUT_FILENAME);
	}
}
