#include <string>
#include <vector>
#include <filesystem>

namespace fs {
	using DirectoryEntry = std::filesystem::directory_entry;
	std::vector<DirectoryEntry> getFileList(std::string &location);
	void chdir(std::string &path);
	void compress(std::string &outputNameBase, std::string &rootDirectory, std::vector<DirectoryEntry> &targets);
	std::string relativePath(std::string &path);
}