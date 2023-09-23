#ifndef _customException_h
#define _customException_h

#include <exception>
namespace customException {
	class EofError : public std::exception {
	public:
		virtual const char *what() const noexcept {
			return "EOF Error";
		}
	};

	class OpenError : public std::exception {
		virtual const char *what() const noexcept {
			return "Failed opening file";
		}
	};

	class ForkError : public std::exception {
		virtual const char *what() const noexcept {
			return "Failed to create process";
		}
	};

	class WaitError : public std::exception {
		virtual const char *what() const noexcept {
			return "Failed to call wait syscall";
		}
	};

	class ExecError : public std::exception {
		virtual const char *what() const noexcept {
			return "Failed to execute other programs";
		}
	};

	extern EofError eofError;
	extern OpenError openError;
	extern ForkError forkError;
	extern WaitError waitError;
	extern ExecError execError;
}

#endif