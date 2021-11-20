#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <exception>
#include <memory>
#include <string>

#include "exception"

namespace libs {
	namespace exception {
/**
 * Inherites from std::exception an cretes an internal custom exception handling interface 
 */
class custom_exception: public std::exception {
	private:
		const std::unique_ptr<std::string> m_msg;
	public:
		/**
		 * constructor with argument
		 * \param msg represents a `const char* msg` which is the error message
		 */
		custom_exception(const char* msg): m_msg(std::move(std::make_unique<std::string>(msg))) {
		};
		/**
		 * A virtual function retrives the actual error message
		 * @returns `const char* msg`
		 * \param msg represents a `const char* msg` which is the error message
		 */
		virtual const char* what() const throw() {
			return m_msg.get()->c_str();
		}
};
}
}

#endif //__EXCEPTION_HPP__
