#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "ui.h"
#include "../common/errors.h"
#include "../common/util.h"
#include "fs.h"
using namespace std;

namespace fsh {
	static string compress(ui::CompressMethod method, string &location, string &outputName) {
		string rootDirectory = location + "/../";
		fs::chdir(rootDirectory);
		string relativePath = fs::relativePath(location);
		vector<fs::DirectoryEntry> entries = fs::getFileList(relativePath);
		switch (method) {
		case ui::ZSTD:
			return fs::zstdCompress(outputName, rootDirectory, entries);
		case ui::ZIP:
			return fs::zipCompress(outputName, rootDirectory, entries);
		default:
			throw errors::unknownError;
		}
	}
	static string compressHomedir(ui::CompressMethod method) {
		string location = util::getHomedir();
		string outputName = util::homedirOutputName();
		return compress(method, location, outputName);
	}

	static string compressTmp2(ui::CompressMethod method) {
		string location = ui::askTmp2Directory();
		string outputName = util::tmp2OutputName(location);
		return compress(method, location, outputName);
	}

	static void run() {
		util::setUmask();
		ui::banner();
		ui::BackupType backupType = ui::askBackupType();
		ui::CompressMethod method = ui::askCompressionMethod();
		string outputName;
		switch (backupType) {
		case ui::HOMEDIR:
			outputName = compressHomedir(method);
			break;
		case ui::TMP2:
			outputName = compressTmp2(method);
			break;
		default:
			throw errors::unknownError;
		}
		ui::footer(outputName);
	}

	static bool acquireRuntimeLock() {
		bool res = fs::createExclusive(util::runtimeLockName());
		if (res)
			return true;
		return ui::warnParallelExecution();
	}

	static void releaseRuntimeLock() {
		fs::removeFile(util::runtimeLockName());
	}

	void start() {
		try {
			if (!acquireRuntimeLock())
				return;
			run();
			releaseRuntimeLock();
		}
		catch (exception &e) {
			cout << "Exception caught: " << e.what() << endl;
		}
	}
}