#ifndef _errors_h
#define _errors_h

#include <exception>
namespace errors {
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

	class UnknownError : public std::exception {
		virtual const char *what() const noexcept {
			return "Unknown error";
		}
	};

	extern EofError eofError;
	extern OpenError openError;
	extern ForkError forkError;
	extern WaitError waitError;
	extern ExecError execError;
	extern UnknownError unknownError;
}

#endif