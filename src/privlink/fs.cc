#include <filesystem>
#include "../common/util.h"
#include <unistd.h>
#include "fs.h"

using namespace std;

namespace fs {
	void linkReadonly(const filesystem::path &targetPath, const filesystem::path &readonlyPath) {
		try {
			filesystem::remove(readonlyPath);
		}
		catch (exception e) {}
		filesystem::copy(targetPath, readonlyPath);
		chown(readonlyPath.c_str(), getuid(), getgid());
		filesystem::remove(targetPath);
	}
}