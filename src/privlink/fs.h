#ifndef _fs_h
#define _fs_h

#include <filesystem>

namespace fs {
	void linkReadonly(const std::filesystem::path &targetPath, const std::filesystem::path &readonlyPath);
}
#endif