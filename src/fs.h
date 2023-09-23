#include <string>
#include <vector>
#include <filesystem>

namespace fs {
	using DirectoryEntry = std::filesystem::directory_entry;
	bool createExclusive(const std::string &path);
	void removeFile(const std::string &path);
	std::vector<DirectoryEntry> getFileList(std::string &location);
	void chdir(std::string &path);
	std::string zstdCompress(std::string &outputNameBase, std::string &rootDirectory, std::vector<DirectoryEntry> &targets);
	std::string zipCompress(std::string &outputNameBase, std::string &rootDirectory, std::vector<DirectoryEntry> &targets);
	std::string relativePath(std::string &path);
}