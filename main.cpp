#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "sodium.h"
#include "benis_crypt.h"

using namespace std;
using namespace HideMyBenis;

int main(int argc, char* argv[])
{
	if(sodium_init() == -1)
	{
		cout << "ERROR 00: libsodium could not be initialized!" << endl;
		return 1;
	}
	map<string, vector<string>> arguments;
	for(int i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			int j = i;
			while(i + 1 < argc && argv[i + 1][0] != '-') { arguments[argv[j]].push_back(argv[++i]); }
			arguments[argv[j]];
		}
	}
	if(arguments.count("-h") + arguments.count("-help"))
	{
		cout << "----------------------------------------------------------------------------------------------" << endl;
		cout << "\t\t\tHideMyBenis!" << endl;
		cout << "----------------------------------------------------------------------------------------------" << endl;
		cout << "Encryption:\t-e inputImage inputFile [-f] [-p password] [-o outputImage]" << endl;
		cout << "\tpassword is optional and defaults to no password." << endl;
		cout << "\toutputImage is optional and defaults to output.png" << endl;
		cout << "\t-f enables fast-mode. Has to be decrypted with -f if it was encrypted with -f.\n\t\tFast-mode makes password-hashing a lot faster, which in turn makes brute-force\n\t\tattacks easier." << endl;
		cout << "----------------------------------------------------------------------------------------------" << endl;
		cout << "Decryption:\t-d inputImage [-f] [-p password] [-o outputFile]" << endl;
		cout << "\tpassword is optional and defaults to no password." << endl;
		cout << "\toutputFile is optional and defaults to output" << endl;
		cout << "\t-f decrypts with fast-mode. Has to be done if it was encrypted with -f." << endl;
		cout << "----------------------------------------------------------------------------------------------" << endl;
	}
	else if(arguments.count("-e") && arguments.at("-e").size() == 2)
	{
		string inputImage = arguments["-e"][0];
		string inputFile = arguments["-e"][1];
		string password = "";
		string outputImage = "output.png";
		bool fast = arguments.count("-f") > 0;
		if(arguments.count("-p") && arguments.at("-p").size() == 1) { password = arguments["-p"][0]; }
		else { cout << "No password given or wrong usage of -p. Using the empty string as password." << endl; }
		if(arguments.count("-o") && arguments.at("-o").size() == 1) { outputImage = arguments["-o"][0]; }
		else { cout << "No outputImage given or wrong usage of -o. Using the output.png as outputImage." << endl; }
		try
		{
			Image img(inputImage);
			ifstream input(inputFile, ios::binary);
			if(!input)
			{
				throw std::runtime_error("ERROR in main: Could not open " + inputFile + "!");
			}
			encrypt(img, input, password, fast);
			img.writeToFile(outputImage);
		}
		catch(std::runtime_error& e) { cerr << e.what() << endl; }
	}
	else if(arguments.count("-d") && arguments.at("-d").size() == 1)
	{
		string inputImage = arguments["-d"][0];
		string password = "";
		string outputFile = "output";
		bool fast = arguments.count("-f") > 0;
		if(arguments.count("-p") && arguments.at("-p").size() == 1) { password = arguments["-p"][0]; }
		else { cout << "No password given or wrong usage of -p. Using the empty string as password." << endl; }
		if(arguments.count("-o") && arguments.at("-o").size() == 1) { outputFile = arguments["-o"][0]; }
		else { cout << "No outputImage given or wrong usage of -o. Using the output.png as outputImage." << endl; }
		try
		{
			Image img(inputImage);
			ofstream output(outputFile, ios::binary);
			if (!output)
			{
				throw std::runtime_error("ERROR in main: Could not save " + outputFile + "!");
			}
			decrypt(img, password, output, fast);
		}
		catch(std::runtime_error& e) { cerr << e.what() << endl; }
	}
	else { cout << "Could not interpret command line arguments. Use -h or -help for usage info." << endl; }
	return 0;
}
