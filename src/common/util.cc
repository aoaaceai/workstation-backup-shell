#include <cstdlib>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "util.h"
#include "errors.h"

using namespace std;
namespace util {
	void setUmask() {
		umask(0077);
	}
	string getHomedir() {
		struct passwd *pw = getpwuid(getuid());
		return pw->pw_dir;
	}

	string resolvePath(const string &path) {
		char *raw = realpath(path.c_str(), nullptr);
		string res = raw ?: "";
		free(raw);
		return res;
	}

	static string getUsername() {
		return getpwuid(getuid())->pw_name;
	}

	string runtimeLockName() {
		return "/tmp2/backup_locks/" + getUsername() + ".lock";
	}

	string fileListName(string &baseName) {
		return "/tmp2/backup_list/" + baseName + ".list";
	}

	string outputArchiveDir() {
		return "/tmp2/backup_output/";
	}

	string outputArchiveName(string &base, const string &&suffix) {
		return outputArchiveDir() + base + suffix;
	}

	string outputReadonlyDir() {
		return "/tmp2/backup_rootonly/";
	}

	string outputReadonlyName(const string &filename) {
		return outputReadonlyDir() + filename;
	}

	string homedirOutputName() {
		string username = getpwuid(getuid())->pw_name;
		return getUsername() + "_homedir";
	}

	string tmp2OutputName(string &location) {
		// see man hostname(7), the size limit is 253
		char hostnameBuf[256];
		gethostname(hostnameBuf, sizeof(hostnameBuf));
		string res = getUsername() + '_' + hostnameBuf + location;
		replace(res.begin(), res.end(), '/', '_');
		return res;
	}

	// returns the exit code of the child process
	int fork_exec(vector<char *> &argv) {
		if (argv.size() == 0 || argv.back() != nullptr)
			throw errors::argumentError;

		pid_t pid = fork();

		if (pid < 0)
			throw errors::forkError;
		if (pid == 0) {
			// vectors are (by standard) implemented with contiguous arrays
			// so it's safe to pass the raw pointer
			// see https://en.cppreference.com/w/cpp/container/vector
			int res = execvp(argv[0], &argv[0]);
			throw errors::execError;
		}

		int status;
		waitpid(pid, &status, 0);

		return WEXITSTATUS(status);
	}

	bool userOwns(const char *path) {
		struct stat info;
		if (stat(path, &info) < 0)
			return false;
		return info.st_uid == getuid();
	}

	string pathToFilename(const string &path) {
		return path.substr(path.find_last_of("/") + 1);
	}
}