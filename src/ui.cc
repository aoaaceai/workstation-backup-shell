#include "ui.h"
#include <iostream>
#include <memory>
#include "customException.h"
using namespace std;

namespace ui {
	void banner() {
		cout << "Workstation backup service" << endl;
	}

	BackupChoice askBackupChoice() {
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
				throw customException::EofError();

			if (choice == 1 || choice == 2)
				break;

			cout << "Please input a number." << endl;
		}

		return (BackupChoice) choice;
	}

	static string resolvePath(string &path) {
		char *raw = realpath(path.c_str(), nullptr);
		string res = raw ?: "";
		free(raw);
		return res;
	}

	static string readNonemptyLine() {
		string input;
		while (getline(cin, input)) {
			if (input.size())
				return input;
		}
		throw customException::EofError();
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
				dirname = resolvePath(input);
				if (!dirname.compare(0, 6, "/tmp2/"))
					break;
				cout << "Enter a valid path under /tmp2" << endl;
			}
		}
		return dirname;
	}
}