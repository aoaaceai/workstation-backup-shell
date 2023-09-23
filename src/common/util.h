#ifndef _util_h
#define _util_h

#include <string>
#include <vector>

namespace util {
	void setUmask();
	std::string resolvePath(const std::string &path);
	std::string getHomedir();
	std::string runtimeLockName();
	std::string homedirOutputName();
	std::string tmp2OutputName(std::string &location);
	std::string fileListName(std::string &baseName);
	std::string outputArchiveDir();
	std::string outputArchiveName(std::string &base, const std::string &&suffix);
	std::string outputReadonlyDir();
	std::string outputReadonlyName(const std::string &&filename);
	int fork_exec(std::vector<char *> &argv);
	bool userOwns(const char *path);
}

#endif