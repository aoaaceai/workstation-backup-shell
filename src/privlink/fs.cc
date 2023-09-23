#include <filesystem>
#include "../common/util.h"
#include "fs.h"

using namespace std;

namespace fs {
	void linkReadonly(const filesystem::path &targetPath) {
		filesystem::create_hard_link(
			targetPath,
			util::outputReadonlyName(
				targetPath.filename().c_str()
			)
		);
	}
}