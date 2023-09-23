#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "util.h"
#include "customException.h"

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

	string homedirOutputName() {
		string username = getpwuid(getuid())->pw_name;
		return username + "_homedir";
	}

	string tmp2OutputName(string &location) {
		// see man hostname(7), the size limit is 253
		char hostnameBuf[256];
		string username = getpwuid(getuid())->pw_name;
		gethostname(hostnameBuf, sizeof(hostnameBuf));
		return username + '_' + hostnameBuf + '_' + location;
	}

	// returns the exit code of the child process
	int fork_exec(vector<char *> &argv) {
		int status;
		pid_t pid = fork();

		if (pid < 0)
			throw customException::forkError;
		if (pid == 0) {
			// vectors are (by standard) implemented with contiguous arrays
			// so it's safe to pass the raw pointer
			// see https://en.cppreference.com/w/cpp/container/vector
			int res = execvp(argv[0], &argv[0]);
			throw customException::execError;
		}
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	}
}