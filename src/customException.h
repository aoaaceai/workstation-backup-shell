#ifndef _customException_h
#define _customException_h

#include <exception>
namespace customException {
	class EofError : public std::exception {
	public:
		virtual const char* what() const noexcept {
			return "EOF Error";
		}
	};
}

#endif