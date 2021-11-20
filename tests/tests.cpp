#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE TEST_ACL

#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <memory>

#include "acl.hpp"

struct acl_fixture
{
	public:
	acl_fixture(): obj() { 
	}
	~acl_fixture() {}
	libs::acl::acl<std::string, std::unique_ptr<std::fstream>> obj;
};

// Testing whether the resource gets a valid uuid after adding 
BOOST_FIXTURE_TEST_CASE(TEST_IS_VALID_UUID, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	fsm.get()->open("./tests/text.txt", std::fstream::in);
	libs::subjects::subject<std::string> sub("my_files");
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	size_t uuid = obj.add(sub, std::move(res_fsm));
	
	BOOST_CHECK_EQUAL(1, uuid);
}
// Testing access list size with different subjects 
BOOST_FIXTURE_TEST_CASE(TEST_IS_VALID_SIZE_FOR_DIFFERENT_SUBJECTS, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	std::unique_ptr<std::fstream> fsm_one = std::make_unique<std::fstream>();
	std::unique_ptr<std::fstream> fsm_sec = std::make_unique<std::fstream>();

	fsm.get()->open("./tests/text.txt", std::fstream::in);
	fsm_one.get()->open("./tests/text.txt", std::fstream::in);
	fsm_sec.get()->open("./tests/text.txt", std::fstream::in);
	
	libs::subjects::subject<std::string> sub("my_files");
	libs::subjects::subject<std::string> sub_one("my_files_one");
	libs::subjects::subject<std::string> sub_sec("my_files_sec");
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm_one = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm_one);
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm_sec = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm_sec);
	
	size_t uuid = obj.add(sub, std::move(res_fsm));
	BOOST_CHECK_EQUAL(1, uuid);
	size_t uuid_one = obj.add(sub_one, std::move(res_fsm_one));
	BOOST_CHECK_EQUAL(2, uuid_one);
	size_t uuid_sec = obj.add(sub_sec, std::move(res_fsm_sec));
	BOOST_CHECK_EQUAL(3, uuid_sec);
	
	BOOST_CHECK_EQUAL(3, obj.size());
}
// Testing access list size with several resources within the same subject 
BOOST_FIXTURE_TEST_CASE(TEST_IS_VALID_SIZE_FOR_SAME_SUBJECT, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	std::unique_ptr<std::fstream> fsm_one = std::make_unique<std::fstream>();
	std::unique_ptr<std::fstream> fsm_sec = std::make_unique<std::fstream>();

	fsm.get()->open("./tests/text.txt", std::fstream::in);
	fsm_one.get()->open("./tests/text.txt", std::fstream::in);
	fsm_sec.get()->open("./tests/text.txt", std::fstream::in);
	
	libs::subjects::subject<std::string> sub("my_files");
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm_one = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm_one);
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm_sec = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm_sec);
	
	size_t uuid = obj.add(sub, std::move(res_fsm));
	BOOST_CHECK_EQUAL(1, uuid);
	uuid = obj.add(sub, std::move(res_fsm_one));
	BOOST_CHECK_EQUAL(2, uuid);
	uuid = obj.add(sub, std::move(res_fsm_sec));
	BOOST_CHECK_EQUAL(3, uuid);
	
	BOOST_CHECK_EQUAL(1, obj.size());
}
// Testing whether the resource is in valied state 
BOOST_FIXTURE_TEST_CASE(TEST_IS_RESOURCE_IN_VALID_STATE, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	fsm.get()->open("./tests/text.txt", std::fstream::in);
	libs::subjects::subject<std::string> sub("my_files");
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	size_t uuid = obj.add(sub, std::move(res_fsm));
	
	BOOST_CHECK_EQUAL(1, uuid);
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> uptr = obj.try_pop(sub, uuid);
	BOOST_CHECK(uptr);
	const std::unique_ptr<std::unique_ptr<std::fstream>>& fstm_uptruptr = uptr.get()->get_resource();
	BOOST_CHECK(fstm_uptruptr);
	const std::unique_ptr<std::fstream>& fstm_uptr = *fstm_uptruptr.get();
	BOOST_CHECK(fstm_uptr);
	bool ret = fstm_uptr.get()->is_open();
	BOOST_CHECK_EQUAL(true, ret);
}
// Testing whether the resource is in valied state in case of invalid raw resource 
BOOST_FIXTURE_TEST_CASE(TEST_IS_RESOURCE_IN_VALID_STATE_WHEN_RAW_RESOURCE_IS_INVALID, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	fsm.get()->open("./tests/text_non_exists.txt", std::fstream::in);
	libs::subjects::subject<std::string> sub("my_files");
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	size_t uuid = obj.add(sub, std::move(res_fsm));
	
	BOOST_CHECK_EQUAL(1, uuid);
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> uptr = obj.try_pop(sub, uuid);
	BOOST_CHECK(uptr);
	const std::unique_ptr<std::unique_ptr<std::fstream>>& fstm_uptruptr = uptr.get()->get_resource();
	BOOST_CHECK(fstm_uptruptr);
	const std::unique_ptr<std::fstream>& fstm_uptr = *fstm_uptruptr.get();
	BOOST_CHECK(fstm_uptr);
	bool ret = fstm_uptr.get()->is_open();
	BOOST_CHECK_EQUAL(false, ret);
}
// Testing one valid opened file and the other is invalid in the acl 
BOOST_FIXTURE_TEST_CASE(TEST_ONE_OPENED_AND_SECOND_NON_EXISTING_RAW_RESOURCE_WITHIN_THE_SAME_SUBJECT, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	std::unique_ptr<std::fstream> fsm_one = std::make_unique<std::fstream>();

	fsm.get()->open("./tests/text.txt", std::fstream::in);
	fsm_one.get()->open("./tests/text_non_existing.txt", std::fstream::in);
	
	libs::subjects::subject<std::string> sub("my_files");
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm_one = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm_one);
	
	size_t uuid_one = obj.add(sub, std::move(res_fsm));
	BOOST_CHECK_EQUAL(1, uuid_one);
	size_t uuid_sec = obj.add(sub, std::move(res_fsm_one));
	BOOST_CHECK_EQUAL(2, uuid_sec);
	
	BOOST_CHECK_EQUAL(1, obj.size());
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> uptr_one = obj.try_pop(sub, uuid_one);
	BOOST_CHECK(uptr_one);
	const std::unique_ptr<std::unique_ptr<std::fstream>>& fstm_uptruptr_one = uptr_one.get()->get_resource();
	BOOST_CHECK(fstm_uptruptr_one);
	const std::unique_ptr<std::fstream>& fstm_uptr_one = *fstm_uptruptr_one.get();
	BOOST_CHECK(fstm_uptr_one);
	bool ret = fstm_uptr_one.get()->is_open();
	BOOST_CHECK_EQUAL(true, ret);
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> uptr_sec = obj.try_pop(sub, uuid_sec);
	BOOST_CHECK(uptr_sec);
	const std::unique_ptr<std::unique_ptr<std::fstream>>& fstm_uptruptr_sec = uptr_sec.get()->get_resource();
	BOOST_CHECK(fstm_uptruptr_sec);
	const std::unique_ptr<std::fstream>& fstm_uptr_sec = *fstm_uptruptr_sec.get();
	BOOST_CHECK(fstm_uptr_sec);
	ret = fstm_uptr_sec.get()->is_open();
	BOOST_CHECK_EQUAL(false, ret);
}
// Testing whether the resource is has a default access level specifier `forbiden` 
BOOST_FIXTURE_TEST_CASE(TEST_WHETHER_DEFAUT_ACCESS_LEVEL_IS_FORBIDEN, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	fsm.get()->open("./tests/text.txt", std::fstream::in);
	libs::subjects::subject<std::string> sub("my_files");
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	size_t uuid = obj.add(sub, std::move(res_fsm));
	
	BOOST_CHECK_EQUAL(1, uuid);
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> uptr = obj.try_pop(sub, uuid);
	BOOST_CHECK(uptr);
	const std::unique_ptr<std::unique_ptr<std::fstream>>& fstm_uptruptr = uptr.get()->get_resource();
	BOOST_CHECK(fstm_uptruptr);
	bool ret = obj.is_allowed(sub, std::move(uptr));
	BOOST_CHECK_EQUAL(false, ret);
}
// Testing access level change from `forbiden` to `allowed`
BOOST_FIXTURE_TEST_CASE(TEST_ACCESS_LEVEL_CHANGE, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	fsm.get()->open("./tests/text.txt", std::fstream::in);
	libs::subjects::subject<std::string> sub("my_files");
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	size_t uuid = obj.add(sub, std::move(res_fsm));
	/*
	 * By default the acces level of the newly created resource is `forbiden`.
	 * Changing to `allowed`.
	 */
	obj.allow_access(sub, uuid);
	
	BOOST_CHECK_EQUAL(1, uuid);
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> uptr = obj.try_pop(sub, uuid);
	BOOST_CHECK(uptr);
	const std::unique_ptr<std::unique_ptr<std::fstream>>& fstm_uptruptr = uptr.get()->get_resource();
	BOOST_CHECK(fstm_uptruptr);
	bool ret = obj.is_allowed(sub, std::move(uptr));
	BOOST_CHECK_EQUAL(true, ret);
}
// Testing access level change from `allowed` to `forbiden`
BOOST_FIXTURE_TEST_CASE(TEST_ACCESS_LEVEL_CHANGE_TO_FORBIDEN, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	fsm.get()->open("./tests/text.txt", std::fstream::in);
	libs::subjects::subject<std::string> sub("my_files");
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	size_t uuid = obj.add(sub, std::move(res_fsm));
	/*
	 * By default the acces level of the newly created resource is `forbiden`.
	 * Changing to `allowed`.
	 */
	obj.allow_access(sub, uuid);
	
	BOOST_CHECK_EQUAL(1, uuid);

	/*
	 * Again changing the access level to `forbiden`
	 */

	obj.forbid_access(sub, uuid);
	
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> uptr = obj.try_pop(sub, uuid);
	BOOST_CHECK(uptr);
	const std::unique_ptr<std::unique_ptr<std::fstream>>& fstm_uptruptr = uptr.get()->get_resource();
	BOOST_CHECK(fstm_uptruptr);
	bool ret = obj.is_allowed(sub, std::move(uptr));
	BOOST_CHECK_EQUAL(false, ret);
}
// Testing remove the subject
BOOST_FIXTURE_TEST_CASE(TEST_REMOVE_SUBJECT, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	fsm.get()->open("./tests/text.txt", std::fstream::in);
	libs::subjects::subject<std::string> sub("my_files");
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	size_t uuid = obj.add(sub, std::move(res_fsm));
	BOOST_CHECK_EQUAL(1, uuid);
	bool has_sub = obj.has_subject(sub);
	BOOST_CHECK_EQUAL(true, has_sub);
	obj.remove(sub);
	has_sub = obj.has_subject(sub);
	BOOST_CHECK_EQUAL(false, has_sub);
}
// Testing remove the resource from the subject
BOOST_FIXTURE_TEST_CASE(TEST_REMOVE_RESOURCE_FROM_SUBJECT, acl_fixture)
{
	std::unique_ptr<std::fstream> fsm = std::make_unique<std::fstream>();
	fsm.get()->open("./tests/text.txt", std::fstream::in);
	libs::subjects::subject<std::string> sub("my_files");
	std::unique_ptr<libs::resources::resource<std::unique_ptr<std::fstream>>> res_fsm = std::make_unique<libs::resources::resource<std::unique_ptr<std::fstream>>>(fsm);
	size_t uuid = obj.add(sub, std::move(res_fsm));
	BOOST_CHECK_EQUAL(1, uuid);
	bool has_sub = obj.has_subject(sub);
	bool has_res = obj.has_resource(sub, uuid);
	BOOST_CHECK_EQUAL(true, has_sub);
	BOOST_CHECK_EQUAL(true, has_res);
	obj.remove(sub, uuid);
	has_sub = obj.has_subject(sub);
	has_res = obj.has_resource(sub, uuid);
	BOOST_CHECK_EQUAL(true, has_sub);
	BOOST_CHECK_EQUAL(false, has_res);
}
