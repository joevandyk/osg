// Vec3 is implemented entirely in the header. This .cpp file just
// contains utx test code

#ifdef OSG_COMPILE_UNIT_TESTS

#include <osg/Vec3>
#include <osg/UnitTestFramework>

#include <sstream>

namespace osg
{

class Vec3TestFixture
{
public:

	Vec3TestFixture();

	void testAddition(const osgUtx::TestContext& ctx);
	void testSubtraction(const osgUtx::TestContext& ctx);
	void testScalarMultiplication(const osgUtx::TestContext& ctx);
	void testDotProduct(const osgUtx::TestContext& ctx);

private:

	// Some convenience variables for use in the tests
	Vec3 v1_, v2_, v3_;

};

Vec3TestFixture::Vec3TestFixture():
	v1_(1.0f, 1.0f, 1.0f),
	v2_(2.0f, 2.0f, 2.0f),
	v3_(3.0f, 3.0f, 3.0f)
{
}

void Vec3TestFixture::testAddition(const osgUtx::TestContext&)
{
	OSGUTX_TEST_F( v1_ + v2_ == v3_ )
}

void Vec3TestFixture::testSubtraction(const osgUtx::TestContext&)
{
	OSGUTX_TEST_F( v3_ - v1_ == v2_ )
}

void Vec3TestFixture::testScalarMultiplication(const osgUtx::TestContext&)
{
	OSGUTX_TEST_F( v1_ * 3 == v3_ )
}

void Vec3TestFixture::testDotProduct(const osgUtx::TestContext&)
{
	
}

OSGUTX_BEGIN_TESTSUITE(Vec3)
	OSGUTX_ADD_TESTCASE(Vec3TestFixture, testAddition)
	OSGUTX_ADD_TESTCASE(Vec3TestFixture, testSubtraction)
	OSGUTX_ADD_TESTCASE(Vec3TestFixture, testScalarMultiplication)
	OSGUTX_ADD_TESTCASE(Vec3TestFixture, testDotProduct)
OSGUTX_END_TESTSUITE

OSGUTX_AUTOREGISTER_TESTSUITE_AT(Vec3, root.osg)

}

#endif
