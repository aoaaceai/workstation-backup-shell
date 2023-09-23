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
	std::string outputArchiveName(std::string &base, const std::string &&suffix);
	int fork_exec(std::vector<char *> &argv);
}
#endif