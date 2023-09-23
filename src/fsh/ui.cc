#include <iostream>
#include <memory>
#include "ui.h"
#include "../common/errors.h"
#include "../common/util.h"
using namespace std;

// TODO: code reuse
namespace ui {
	void banner() {
		cout << "Workstation backup service" << endl;
	}

	bool warnParallelExecution() {
		int choice;

		while (true) {
			cout
				<< "Warning: You might have other sessions on this machine." << endl
				<< "This may affect the integrity of the compressed files." << endl
				<< "Do you want to continue?" << endl
				<< "1. Yes" << endl
				<< "2. No" << endl
				<< "> ";

			choice = -1;
			cin >> choice;

			if (cin.eof())
				throw errors::eofError;

			if (choice == 1 || choice == 2)
				break;

			cout << "Please input a number." << endl;
		}

		return choice == 1;
	}

	BackupType askBackupType() {
		int choice;

		while (true) {
			cout
				<< "Choose the directory type to backup:" << endl
				<< "1. Home directory" << endl
				<< "2. /tmp2" << endl
				<< "> ";

			choice = -1;
			cin >> choice;

			if (cin.eof())
				throw errors::eofError;

			if (choice == 1 || choice == 2)
				break;

			cout << "Please input a number." << endl;
		}

		return (BackupType) choice;
	}

	CompressMethod askCompressionMethod() {
		int choice;

		while (true) {
			cout
				<< "Choose the compression method:" << endl
				<< "1. Zstd" << endl
				<< "2. ZIP" << endl
				<< "> ";

			choice = -1;
			cin >> choice;

			if (cin.eof())
				throw errors::eofError;

			if (choice == 1 || choice == 2)
				break;

			cout << "Please input a number." << endl;
		}
		return (CompressMethod) choice;
	}

	static string readNonemptyLine() {
		string input;
		while (getline(cin, input)) {
			if (input.size())
				return input;
		}
		throw errors::eofError;
	}

	static void listTmp2() {
		system("ls -alh /tmp2");
	}

	string askTmp2Directory() {
		string dirname;
		while (true) {
			cout
				<< "Input the directory name under /tmp2." << endl
				<< "e.g. /tmp2/b12902000." << endl
				<< "type \"ls\" to list the directories" << endl
				<< "> ";
			string input = readNonemptyLine();

			if (input == "ls")
				listTmp2();
			else {
				dirname = util::resolvePath(input);
				if (!dirname.compare(0, 6, "/tmp2/"))
					break;
				cout << "Enter a valid path under /tmp2" << endl;
			}
		}
		return dirname;
	}

	void footer(string &outputName) {
		cout
			<< "Compression done. SFTP to port 2301 to collect your files." << endl
			<< "filename: " << outputName << endl;
	}
}