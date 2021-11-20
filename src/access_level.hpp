#ifndef __ACCESS_LEVEL_HPP__
#define __ACCESS_LEVEL_HPP__

#include <string>
#include <unordered_set>

#include "exception.hpp"
 
/// file: access_level.hpp

namespace libs {
	namespace enums {
/**
 * @brief Encapsulates the access level specific information of the resource.
 */
class access_level {
	private:
		std::unordered_set<std::string> valid_access_specifiers{"allowed", "forbiden"};
		mutable std::string m_access;
		bool is_valid_access_specifier(const std::string& access_specifier) {
			return valid_access_specifiers.find(access_specifier) != valid_access_specifiers.end();
		}
	public: 
		/**
		 * The constructor with one argument
		 *
		 * \param access_level a const std::string argument defines the access level
		 */
		access_level(const std::string& access_level) {
			if (is_valid_access_specifier(access_level)) {
				m_access = access_level;
			} else {
				std::string msg = std::string("Error: Invalid access level specifier: ") + access_level;
				throw libs::exception::custom_exception(msg.c_str());
			}

		}
		/**
		 * The copy constructor
		 *
		 * \param rhs an object that should be copyed
		 */
		access_level(const access_level& rhs) {
			m_access = rhs.m_access;
		}
		/**
		 * The assignment operator
		 *
		 * \param rhs an object that should be copy/assigned
		 */
		access_level& operator=(const access_level& rhs) {
			if(&rhs == this) {
				return *this;
			}
			m_access = rhs.m_access;
			return *this;
		}
		/**
		 * Returns the access level of the resource
		 * @returns `std::string`
		 */
		std::string get_access_level() const {
			return m_access;
		}
};
}
}

#endif // __ACCESS_LEVEL_HPP__
