#include <cstdlib>	//srand()
#include <iostream>	//cout
#include <ctime>	//time()
#include <cstring>	//strcmp()
#include <string>
#include "test.h"	//testing functions
#include "RSA.h"	//GenerateKeyPair()
#include "PrimeGenerator.h"	//Generate()
#include "keyPair.h"
#include "key.h"

using std::cout;
using std::endl;
using std::cin;

int main(int argc, char *argv[])
{
	//randomize the random number generator
	std::srand(time(NULL));
	
	if (argc > 1)	//arguments present
	{
		void parseParams(int argc, char *argv[]);
		parseParams(argc, argv);
		return EXIT_SUCCESS;
	}
	else	//no arguments
	{
		void printUsage();
		printUsage();
		return EXIT_SUCCESS;
	}
}

void printUsage()
{
	cout <<  
	"Usage: rsa [OPTION] [ARGUMENT]" << endl << 
	"rsa is a command-line frontend to rsalib. rsalib provides RSA encryption,"
	" decryption and digital signature." << endl << 
	endl << 
	"Available options: " << endl <<
	endl << 
	"    genkey LENGTH [N]" << endl << 
	"Generate and print a RSA key. The generated key is LENGTH " 
	"digits long and is generated in N iterations (default N = 3 is fine). "
	"LENGTH and N must be positive decimal integers." << endl << 
	endl <<
	"    genprime LENGTH [N]" << endl << 
	"Generate and print a prime number. The generated prime is LENGTH " 
	"digits long and is generated in N iterations (default N = 3 is fine). "
	"LENGTH and N must be positive decimal integers." << endl << 
	endl << 
	"    test" << endl << 
	"Run preconfigured tests (development version only)." << endl << 
	endl << 
	"Exit status is 0 if OK, otherwise not 0." << endl;
}

void exitError(const char *message = "\0")
{
	cout << message << endl;
	printUsage();
	std::exit(EXIT_FAILURE);
}

void missingArgumentError(const char *argName)
{
	cout << "Missing argument: '" << argName << "'." << endl;
	exitError();
}

void test()
{
	try
	{
		TestBigIntFunctions();
		TestBigIntOperators();
		FactorialBenchmark();
		RandomBigIntTest(1);
		RandomBigIntDivisionTest(1);
		TestPrimeGenerator(1, 10);
		TestKeyGeneration(1, 8);
		TestEncryptionDecryption(1, 8);
		TestFileEncryptionDecryption(1, 8);
	}
	catch (const char errorMessage[])
	{
		exitError(errorMessage);
	}
	catch (...)
	{
		exitError("Unknown error.");
	}
}

void genkey(unsigned long int digits, unsigned long int iterations = 0)
{
	try
	{
		if (iterations != 0)
			cout << RSA::GenerateKeyPair(digits, iterations) << endl;
		else
			cout << RSA::GenerateKeyPair(digits) << endl;
	}
	catch (const char errorMessage[])
	{
		exitError(errorMessage);
	}
	catch (...)
	{
		exitError("Unknown error.");
	}
}

void genprime(unsigned long int digits, unsigned long int iterations = 0)
{
	try
	{
		if (iterations != 0)
			cout << PrimeGenerator::Generate(digits, iterations) << endl;
		else
			cout << PrimeGenerator::Generate(digits) << endl;
	}
	catch (const char errorMessage[])
	{
		exitError(errorMessage);
	}
	catch (...)
	{
		exitError("Unknown error.");
	}
}
void encryptMessages(unsigned long int digits, std::string message)
{
    try
	{
		KeyPair newKeyPair(RSA::GenerateKeyPair(digits));
        cout << "\n" << newKeyPair; 
        cout << "\n" << "done!" ;
        cout << "\n" << "Encrypting the message: " << "\"" << message << "\"" << " ... \n";
        std::string cypherText = RSA::Encrypt(message, newKeyPair.GetPublicKey());
        cout << "done!\nEncrypted the message: ";
        cout << cypherText;
    }
	catch (const char errorMessage[])
	{
		exitError(errorMessage);
	}
	catch (...)
	{
		exitError("Unknown error.");
	}
}
void decryptMessages(std::string cypherText,std::string modulus,std::string exponent)
{
    try
	{
        BigInt n(modulus);
        BigInt d(exponent);
		Key privateKey(n,d);
        cout << "\n" << "Decrypting the message: " << "\"" << cypherText << "\"" << " ... \n";
        std::string newMessage = RSA::Decrypt(cypherText + " ", privateKey);
        cout << "Decrypted the message: ";
        cout << newMessage;
    }
	catch (const char errorMessage[])
	{
		exitError(errorMessage);
	}
	catch (...)
	{
		exitError("Unknown error.");
	}
}
void parseParams(int argc, char *argv[])
{
	if (strcmp(argv[1], "genkey") == 0)	//generate and print a RSA key
	{
		long int digits = 0;
		if (argc > 2)
		{
			digits = std::atol(argv[2]);
			if (digits == 0)
				exitError("'LENGTH' must be a positive integer.");
		}
		else
			missingArgumentError("LENGTH");
		if (argc > 3)
		{
			long int iterations = std::atol(argv[3]);
			if (iterations <= 0)
				exitError("'N' must be a positive integer.");
			
			genkey(digits, iterations);
		}
		else
			genkey(digits);
	}
	else if (strcmp(argv[1], "genprime") == 0)
	{
		long int digits = 0;
		if (argc > 2)
		{
			digits = std::atol(argv[2]);
			if (digits == 0)
				exitError("'LENGTH' must be a positive integer.");
		}
		else
			missingArgumentError("LENGTH");
		if (argc > 3)
		{
			long int iterations = std::atol(argv[3]);
			if (iterations <= 0)
				exitError("'N' must be a positive integer.");

			genprime(digits, iterations);
		}
		else
			genprime(digits);
	}
	else if (strcmp(argv[1], "test") == 0)	//run all the tests
		test();
	else if (strcmp(argv[1], "encrypt") == 0) 
    {
        long int digits = 0;
        if (argc > 3)
        {
            digits = std::atol(argv[2]);
            if (digits == 0)
				exitError("'LENGTH' must be a positive integer."); 
            std::string message = argv[3];
            encryptMessages(digits,message);
        }
        else if (argc > 2) {
            digits = 50;
            std::string message = argv[2];
            encryptMessages(digits,message);
        } 
        else
			missingArgumentError("MESSAGE");
    } 
    else if (strcmp(argv[1], "decrypt") == 0) 
    {
        if (argc > 4)
        {
            std::string cypherText = argv[2]; 
            std::string n = argv[3];
            std::string d = argv[4];
            decryptMessages(cypherText,n,d);
        }
        else
			missingArgumentError("ERROR");
    }
    else
		exitError("Unrecognized option.");
}
