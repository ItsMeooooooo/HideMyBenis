#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "sodium.h"
#include "benis_crypt.h"
#include "argparser.h"

using namespace std;
using namespace HideMyBenis;

int main(int argc, char* argv[])
{
	if(sodium_init() == -1)
	{
		cerr << "ERROR: libsodium could not be initialized!" << endl;
		return 1;
	}
	ArgParser parser(argc, argv);
	bool fast = parser.exists("f");
	if(parser.exists("h") || parser.exists("help"))
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
		cout << "\toutputFile is optional and defaults to output.txt" << endl;
		cout << "\t-f decrypts with fast-mode. Has to be done if it was encrypted with -f." << endl;
		cout << "----------------------------------------------------------------------------------------------" << endl;
	}
	else if(parser.exists("e", 2))
	{
		string inputImage = parser["e"][0];
		string inputFile = parser["e"][1];
		string password = "";
		string outputImage = "output.png";
		if (parser.exists("p", 1)) { password = parser["p"][0]; }
		else { cout << "No password given or wrong usage of -p. Using the empty string as password." << endl; }
		if(parser.exists("o", 1)) { outputImage = parser["o"][0]; }
		else { cout << "No outputImage given or wrong usage of -o. Using output.png as outputImage." << endl; }
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
	else if(parser.exists("d", 1))
	{
		string inputImage = parser["d"][0];
		string password = "";
		string outputFile = "output.txt";
		
		if (parser.exists("p", 1)) { password = parser["p"][0]; }
		else { cout << "No password given or wrong usage of -p. Using the empty string as password." << endl; }
		if (parser.exists("o", 1)) { outputFile = parser["o"][0]; }
		else { cout << "No outputFile given or wrong usage of -o. Using output.txt as outputFile." << endl; }
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
