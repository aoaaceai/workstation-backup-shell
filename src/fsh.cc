#include <iostream>
#include <string>
#include <vector>
#include "ui.h"
#include "customException.h"
#include "util.h"
#include "fs.h"
using namespace std;

// TODO: fix double inclusion
namespace fsh {
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
		catch (std::exception &e) {
			cout << "Exception caught: " << e.what() << endl;
		}
	}
}