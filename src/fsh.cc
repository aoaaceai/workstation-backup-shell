#include <iostream>
#include <string>
#include <vector>
#include "ui.h"
#include "customException.h"
#include "util.h"
#include "fs.h"
using namespace std;

namespace fsh {
	// TODO: ban parallel login

	static void compress(string &location, string &outputName) {
		string rootDirectory = location + "/../";
		fs::chdir(rootDirectory);
		string relativePath = fs::relativePath(location);
		vector<fs::DirectoryEntry> entries = fs::getFileList(relativePath);
		fs::compress(outputName, rootDirectory, entries);
	}
	static void compressHomedir() {
		string location = util::getHomedir();
		string outputName = util::homedirOutputName();
		compress(location, outputName);
	}

	static void compressTmp2() {
		string location = ui::askTmp2Directory();
		string outputName = util::tmp2OutputName(location);
		compress(location, outputName);
	}

	static void run() {
		util::setUmask();
		ui::banner();
		ui::BackupChoice choice = ui::askBackupChoice();
		switch (choice) {
		case ui::HOMEDIR:
			compressHomedir();
			break;
		case ui::TMP2:
			compressTmp2();
			break;
		}
	}

	void start() {
		try {
			run();
		}
		catch (std::exception &e) {
			cout << "Exception caught: " << e.what() << endl;
		}
	}
}