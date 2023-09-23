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
	int fork_exec(std::vector<char *> &argv);
}
#endif