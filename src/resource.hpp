#ifndef __RESOURCE_HPP__
#define __RESOURCE_HPP__

#include <memory>
#include <string>
#include <type_traits>
 
/// file: resource.hpp

namespace libs {
	namespace resources{

/**
 * @brief Defines the resource.
 * \tparam T the type of raw data stored in resource
 */
template <typename T>
class resource {
	private:
		size_t m_uuid{};
		std::unique_ptr<T> m_resource;
	public: 
		/**
		 * The default constructor
		 */		 
		resource() {
			if constexpr(constexpr bool is_dev_constr = std::is_default_constructible<T>::value) {
				static_assert(is_dev_constr, "T must be default construcitble");
			}
			m_resource = std::move(std::make_unique<T>());
		}
		/**
		 * The copy constructor deleted
		 */		 
		resource(const resource<T>&) = delete;
		/**
		 * The assignement operator deleted
		 */		 
		resource& operator=(const resource<T>&) = delete;
		/**
		 * The move constructor
		 * \param res the resource object that should be moved.
		 */		 
		resource(resource<T>&& res): m_resource(std::move(res.m_resource)) {}
		/**
		 * The move assignement operator
		 * \param res the resource object that should be moved.
		 */		 
		resource& operator=(resource<T>&& res) { 
			m_resource = std::move(res.m_resource);
		}
		/**
		 * The constructor with argument
		 * \param res a reference to the resource object
		 */		 
		resource(T& res): 
	                m_resource(std::move(std::make_unique<T>(std::move(res)))) {}
		/**
		 * The constructor with argument
		 * \param res a rvalue-reference to the resource object
		 */		 
		resource(T&& res): 
	                m_resource(std::move(std::make_unique<T>(res))) {}
		std::unique_ptr<T>& get_resource() {
			return m_resource;
		}
		/**
		 * Retrives the uuid of the specified resource
		 * @returns `size_t`
		 */		 
		size_t get_uuid() {
			return m_uuid;
		}
		/**
		 * Sets the uuid of the specified resource
		 * @returns `void`
		 */		 
		void set_uuid(size_t rand_id) {
			m_uuid = rand_id;
		}
};
}
}

#endif // __RESOURCE_HPP__
