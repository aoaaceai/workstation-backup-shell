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

	static void compress(ui::CompressMethod method, string &location, string &outputName) {
		string rootDirectory = location + "/../";
		fs::chdir(rootDirectory);
		string relativePath = fs::relativePath(location);
		vector<fs::DirectoryEntry> entries = fs::getFileList(relativePath);
		switch (method) {
		case ui::ZSTD:
			fs::zstdCompress(outputName, rootDirectory, entries);
			break;
		case ui::ZIP:
			fs::zipCompress(outputName, rootDirectory, entries);
			break;
		default:
			throw customException::unknownError;
		}
	}
	static void compressHomedir(ui::CompressMethod method) {
		string location = util::getHomedir();
		string outputName = util::homedirOutputName();
		compress(method, location, outputName);
	}

	static void compressTmp2(ui::CompressMethod method) {
		string location = ui::askTmp2Directory();
		string outputName = util::tmp2OutputName(location);
		compress(method, location, outputName);
	}

	static void run() {
		util::setUmask();
		ui::banner();
		// TODO: change this name
		ui::BackupType backupType = ui::askBackupType();
		ui::CompressMethod method = ui::askCompressionMethod();
		switch (backupType) {
		case ui::HOMEDIR:
			compressHomedir(method);
			break;
		case ui::TMP2:
			compressTmp2(method);
			break;
		default:
			throw customException::unknownError;
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