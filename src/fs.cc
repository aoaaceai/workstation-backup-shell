#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <cstdio>
#include "fs.h"
#include "customException.h"
#include "util.h"

using namespace std;

// TODO: code reuse
namespace fs {
	using DirectoryIterator = std::filesystem::recursive_directory_iterator;

	bool createExclusive(const string &path) {
		// TODO: update this when C++23 is more popular
		FILE *fp = fopen(path.c_str(), "wx");
		if (fp) {
			fclose(fp);
			return true;
		}
		else
			return false;
	}

	void removeFile(const std::string &path) {
		filesystem::remove(path);
	}

	void chdir(string &path) {
		filesystem::current_path(path);
	}

	string relativePath(string &path) {
		return filesystem::relative(path).c_str();
	}

	static bool userOwns(DirectoryEntry entry) {
		struct stat info;
		if (stat(entry.path().c_str(), &info) < 0)
			return false;
		return info.st_uid == getuid();
	}

	vector<DirectoryEntry> getFileList(std::string &location) {
		vector<DirectoryEntry> list;
		for (const DirectoryEntry& dirEntry : DirectoryIterator(location)) {
			// Only include regular files.
			if (!dirEntry.is_regular_file())
				continue;

			if (userOwns(dirEntry))
				list.push_back(dirEntry);
			else
				cout << "skipping file owned by others: " << dirEntry << endl;
		}
		return list;
	}

	static string writeFileList(string &baseName, vector<DirectoryEntry> &targets) {
		string filename = util::fileListName(baseName);
		ofstream filesFrom;

		filesFrom.open(filename, ofstream::out | ofstream::trunc);
		if (filesFrom.fail())
			throw customException::openError;

		for (DirectoryEntry &target : targets)
			filesFrom << target.path().c_str() << endl;

		return filename;
	}

	string zstdCompress(string &outputNameBase, string &rootDirectory, vector<DirectoryEntry> &targets) {
		// tar --ignore-failed-read -caf xxx.tar.zst {*targets}

		// declaring them as variables because we'll c_str() later
		string filesFrom = writeFileList(outputNameBase, targets);
		string outputName = util::outputArchiveName(outputNameBase, ".tar.zst");

		// safe to const_cast because we are not touching these pointers
		vector<char *> argv {
			const_cast<char *>("tar"),
			const_cast<char *>("--ignore-failed-read"),
			const_cast<char *>("-caf"),
			const_cast<char *>((outputName).c_str()),
			const_cast<char *>("-T"),
			const_cast<char *>(filesFrom.c_str()),
			nullptr
		};

		int exitCode = util::fork_exec(argv);
		if (exitCode != 0)
			cout << "Warning: tar exited with non-zero status " << exitCode << endl;

		return outputName;
	}

	string zipCompress(string &outputNameBase, string &rootDirectory, vector<DirectoryEntry> &targets) {
		// zip xxx.zip {*targets}
		
		// same as zstdCompress
		string filesFrom = writeFileList(outputNameBase, targets);
		string outputName = util::outputArchiveName(outputNameBase, ".zip");
		string filesFromOption = "-i@" + filesFrom;

		vector<char *> argv {
			const_cast<char *>("zip"),
			const_cast<char *>("-r"),
			const_cast<char *>((outputName).c_str()),
			const_cast<char *>("."),
			const_cast<char *>(filesFromOption.c_str()),
			nullptr
		};

		int exitCode = util::fork_exec(argv);
		if (exitCode != 0)
			cout << "Warning: zip exited with non-zero status " << exitCode << endl;
		
		return outputName;
	}
}