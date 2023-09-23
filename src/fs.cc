#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "fs.h"
#include "customException.h"
#include "util.h"

using namespace std;

namespace fs {
	using DirectoryIterator = std::filesystem::recursive_directory_iterator;

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
			if (
				(
					dirEntry.is_directory() ||
					dirEntry.is_symlink() ||
					dirEntry.is_regular_file()
				) &&
				userOwns(dirEntry)
			)
				list.push_back(dirEntry);
			else
				cout << "skipping: " << dirEntry << endl;
		}
		return list;
	}

	static string writeFileList(string &baseName, vector<DirectoryEntry> &targets) {
		string filename = "/tmp2/backup_list/" + baseName + ".list";
		ofstream filesFrom;

		filesFrom.open(filename, ofstream::out | ofstream::trunc);
		if (filesFrom.fail())
			throw customException::openError;

		for (DirectoryEntry &target : targets)
			filesFrom << target.path().c_str() << endl;

		return filename;
	}

	void compress(string &outputNameBase, string &rootDirectory, vector<DirectoryEntry> &targets) {
		// tar --ignore-failed-read -caf xxx.tar.zst {*targets}

		// declaring them as variables because we'll c_str() later
		string filesFrom = writeFileList(outputNameBase, targets);
		string outputName = "/tmp2/backup_output/" + outputNameBase + ".tar.zst";

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
	}
}