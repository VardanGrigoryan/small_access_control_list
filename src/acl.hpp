#ifndef __ACL_HPP__
#define __ACL_HPP__

#include <string>
#include <unordered_map>
#include <vector>

#include "access_level.hpp"
#include "exception.hpp"
#include "resource.hpp"
#include "subject.hpp"
 
/// file: acl.hpp

namespace libs {
	namespace acl {
template <typename U> struct hasher;
/**
 * @brief Defines the main class which is responsible for managing subjects and resources.
 * \tparam S the type of data stored in subject
 * \tparam R the type of data stored in resource
 */
template <typename S, typename R>
class acl {
	struct hasher_sub {
		std::size_t operator() (subjects::subject<S>& obj) const {
			return std::hash<S>()(obj.get_id());
		}
	};
	struct hasher_res {
		std::size_t operator() (size_t key) const {
			return std::hash<size_t>()(key);
		}
	};
	std::unordered_map<subjects::subject<S>&&, std::unordered_map<size_t, 
		                                            std::pair<std::unique_ptr<resources::resource<R>>, enums::access_level>, 
							    hasher_res>, 
					 hasher_sub> m_map;
	size_t m_uuid{1};
	public: 
		/**
		 * The defaulted constructor
		 */
		acl() = default;
		/**
		 * Adds the resource to the specified subject
		 * \param sub the subject
		 * \param res the resource
		 * \param access the access level for the resource, by default it is set to be `forbiden`
		 * @returns `size_t` the uuid of the added resource
		 */
		size_t add(subjects::subject<S>& sub, std::unique_ptr<resources::resource<R>> res, 
				const std::string& access = std::string("forbiden")) {
			res.get()->set_uuid(m_uuid);
			std::pair<size_t, std::pair<std::unique_ptr<resources::resource<R>>, 
				enums::access_level>> pair = std::make_pair(m_uuid, 
						std::make_pair(std::move(res), 
							enums::access_level(access)));
			m_map[std::move(sub)].insert(std::move(pair));
			size_t uuid = m_uuid;
			++m_uuid;
			return uuid;
		}
		/**
		 * Allows an access to the specified resource within the specified subject
		 * \param sub the subject
		 * \param uuid the uuid og the specified resource
		 * @returns `void`
		 */
		void allow_access(subjects::subject<S>& sub, const size_t uuid) {
			// Changes th access level in O(1) - averrage.
			auto it = m_map.find(sub);
			if(it != m_map.end()) {
				auto itt = it->second.find(uuid);
				if(itt != it->second.end()) {
					itt->second.second = enums::access_level("allowed");
				}
			}
		}
		/**
		 * Forbids the access to the specified resource within the specified subject
		 * \param sub the subject
		 * \param uuid the uuid og the specified resource
		 * @returns `void`
		 */
		void forbid_access(subjects::subject<S>& sub, const size_t uuid) {
			// Changes th access level in O(1) - averrage.
			auto it = m_map.find(sub);
			if(it != m_map.end()) {
				auto itt = it->second.find(uuid);
				if(itt != it->second.end()) {
					itt->second.second = enums::access_level("forbiden");
				}
			}
		}
		/**
		 * Checks whether or not the resource is allowed within the specified subject
		 * \param sub the subject
		 * \param res the resource
		 * @returns `bool` returns true if allowed, false vice versa.
		 */
		bool is_allowed(subjects::subject<S>& sub, std::unique_ptr<resources::resource<R>> res) {
			// Chacks the access level in O(1) - averrage.
			auto it = m_map.find(sub);
			size_t uuid = res.get()->get_uuid();
			if(it == m_map.end()) {
				return false;
			}
			auto itt = it->second.find(uuid);
			if(itt == it->second.end()) {
				return false;
			}
			return itt->second.second.get_access_level() == "allowed" ? true : false;
		}
		/**
		 * Removes the subject
		 * \param sub the subject
		 * @returns `void`
		 */
		void remove(subjects::subject<S>& sub) {
			// removes the subject in O(1) - averrage.
			auto it = m_map.find(sub);
			if(it != m_map.end()) {
				m_map.erase(it);
			}
		}
		/**
		 * Removes the specified resource from the subject
		 * \param sub the subject
		 * \param uuid of the resource
		 * @returns `void`
		 */
		void remove(subjects::subject<S>& sub, const size_t uuid) {
			// removes the resource from the subject in O(1) - averrage.
			auto it = m_map.find(sub);
			if(it != m_map.end()) {
				auto itt = it->second.find(uuid);
				if(itt != it->second.end()) {
					it->second.erase(itt);
				}
			}
		}
		/**
		 * Checks whether the specified subject exists.
		 * \param sub the subject
		 * @returns `bool`
		 */
		bool has_subject(subjects::subject<S>& sub) {
			// Checks whether the subject exists in O(1) - averrage.
			return m_map.find(sub) != m_map.end();
		}
		/**
		 * Checks whether the specified resource exists within the specified subject.
		 * \param sub the subject
		 * \param uuid the uuid of the resource
		 * @returns `bool`
		 */
		bool has_resource(subjects::subject<S>& sub, const size_t uuid) {
			// Checks whether the resource exists within the specified subject in O(1) - averrage.
			auto it = m_map.find(sub);
			if(it != m_map.end()) {
				auto itt = it->second.find(uuid);
				if(itt != it->second.end()) {
					return true;
				}
			}
			return false;
		}
		/**
		 * Tryies to pop the resource from the specified subject.
		 * \param sub the subject
		 * \param res the resource
		 * @returns `std::unique_ptr<resources::resource<R>>` returns the specified resourse if it exists, otherwise nullptr
		 * \tparam R is the type of the raw resource
		 */
		std::unique_ptr<resources::resource<R>> try_pop(subjects::subject<S>& sub, std::unique_ptr<resources::resource<R>> res) {
			// Tryies to pop out the specified resource form the specified subject in O(1) - averrage.
			auto it = m_map.find(sub);
			size_t uuid = res.get()->get_uuid();
			if(it != m_map.end()) {
				auto itt = it->second.find(uuid);
				if(itt != it->second.end()) {
					std::unique_ptr<resources::resource<R>> uptr = std::move(itt->second.first);
					subjects::subject<S> key = itt->first;
					m_map.erase(key);
					return std::move(uptr);
				}
			}
			return nullptr;
		}
		/**
		 * Tryies to pop the resource from the specified subject.
		 * \param sub the subject
		 * \param uuid the uuid of the specified resource
		 * @returns `std::unique_ptr<resources::resource<R>>` returns the specified resourse if it exists, otherwise nullptr
		 * \tparam R is the type of the raw resource
		 */
		std::unique_ptr<resources::resource<R>> try_pop(subjects::subject<S>& sub, size_t uuid) {
			// Tryies to pop out the specified resource by its uuid form the specified subject in O(1) - averrage.
			auto it = m_map.find(sub);
			if(it != m_map.end()) {
				auto itt = it->second.find(uuid);
				if(itt != it->second.end()) {
					std::unique_ptr<resources::resource<R>> uptr = std::move(itt->second.first);
					const size_t key = itt->first;
					return std::move(uptr);
				}
			}
			return nullptr;
		}
		/**
		 * Gets the size of access list.
		 * @returns `const size_t`
		 */
		const size_t size() const {
			return m_map.size();
		}
};
}
}

#endif // __ACL_HPP__
