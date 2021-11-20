#ifndef __SUBJECT_HPP__
#define __SUBJECT_HPP__

#include <memory>
#include <typeinfo>

#include "exception.hpp"

/// file: subject.hpp

namespace libs {
	namespace subjects {

template <typename T> class subject;
template <typename U>
bool operator==(subject<U>& lhs, subject<U>& rhs);

/**
 * @brief Defines the subject.
 * \tparam T the type of raw data stored in subject
 */
template <typename T>
class subject {
	private:
		std::unique_ptr<T> m_id;
		friend bool operator== <T>(subject<T>&, subject<T>&);
		bool is_equal(subject<T>& rhs) {
			return operator==(rhs);
		}
	public: 
		/**
		 * The defaulted constructor
		 */		 
		subject() = default;
		/**
		 * The copy constructor deleted
		 */		 
		subject(const subject&) = delete;
		/**
		 * The assignement operator deleted
		 */		 
		subject& operator=(const subject&) = delete;
		/**
		 * The move constructor
		 * \param sbj the subject object that should be moved.
		 */		 
		subject(subject&& sbj): m_id(std::move(sbj.m_id)) {}
		/**
		 * The move assignement operator
		 * \param sbj the subject object that should be moved.
		 */		 
		subject& operator=(subject&& sbj) { 
			m_id = std::move(sbj.m_id); 
		}
		/**
		 * The constructor with an argument
		 * \tparam id the unique identificator of the subject
		 */		 
		subject(const T& id): 
			m_id(std::move(std::make_unique<T>(id))) {
		}
		/**
		 * Sets a unique udentificator for the subject
		 * \tparam id the unique identificator of the subject
		 * @returns `void` 
		 */		 
		void set_id(const T& id) {
			m_id = std::move(std::make_unique<T>(id));
		}
		/**
		 * Gets the unique udentificator for the subject
		 * @returns `T` the template type of an id 
		 */		 
		T get_id() const {
			if(m_id) {
				return *m_id.get();
			}
			throw libs::exception::custom_exception("Error: Invalid subject");
		}
		/**
		 * The less than operator required to make the subject instance a key
		 * @returns `bool` 
		 */		 
		bool operator<(const subject<T>& rhs) {
			return *m_id.get() < *(rhs.m_id.get());
		}
		/**
		 * The equal operator required to make the subject instance a key
		 * @returns `bool` 
		 */		 
		bool operator==(const subject<T>& rhs) {
			return *m_id.get() == *(rhs.m_id.get());
		}
};

/**
 * The global equal operator required to make the subject instance a key
 * @returns `bool` 
 */		 
template <typename U>
bool operator==(subject<U>& lhs, subject<U>& rhs) {
	return lhs.is_equal(rhs);
}
}
}

#endif // __SUBJECT_HPP__
