#include <iostream>
#include <exception>
#include <string>
#include "../common/errors.h"
#include "../common/util.h"
#include "privlink.h"
#include "fs.h"
#include <iostream>
using namespace std;

namespace privlink {
	
	static void run(const string &target) {
		string targetPath = util::resolvePath(target);
		string outputDir = util::outputArchiveDir();

		if (targetPath.compare(0, outputDir.size(), outputDir)) {
			cout << "error 1";
			throw errors::argumentError;
		}
		if (!util::userOwns(targetPath.c_str())) {
			cout << "error 2";
			throw errors::argumentError;
		}

		string readonlyPath = util::outputReadonlyName(util::pathToFilename(targetPath));
		fs::linkReadonly(targetPath, readonlyPath);
	}

	void start(int argc, char **argv) {
		try {
			if (argc != 2)
				throw errors::argumentError;
			run(argv[1]);
		}
		catch (exception &e) {
			cout << "Exception caught: " << e.what() << endl;
		}
	}
}