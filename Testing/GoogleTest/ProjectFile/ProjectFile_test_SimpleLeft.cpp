/** 
 * @file  SimpleLeft.cpp
 *
 * @brief Implementation for SimpleLeft testcase.
 */

#include <gtest/gtest.h>
#include "UnicodeString.h"
#include "ProjectFile.h"

/** @brief Projectfile to load. */
static const TCHAR FileName[] = _T("..\\TestData\\SimpleLeft.WinMerge");
/** @brief Left path we should get from file. */
static const TCHAR LeftPath[] = _T("C:\\Temp\\Left");

namespace
{

class SimpleLeftTest : public testing::Test
{
protected:
	SimpleLeftTest()
	{
		// You can do set-up work for each test	here.
	}

	virtual ~SimpleLeftTest()
	{
		// You can do clean-up work	that doesn't throw exceptions here.
	}

	// If	the	constructor	and	destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp()
	{
		// Code	here will be called	immediately	after the constructor (right
		// before each test).

		m_pProjectFile = new ProjectFile;

		bool success = m_pProjectFile->Read(FileName);
	}

	virtual void TearDown()
	{
		// Code	here will be called	immediately	after each test	(right
		// before the destructor).
		delete m_pProjectFile;
	}

	// Objects declared here can be used by all tests in the test case for Foo.
	ProjectFile *m_pProjectFile;
};

/**
 * @brief Load the projectfile.
 */
TEST_F(SimpleLeftTest, Load)
{
	// setUp already created the project file for us, but this
	// test is for testing creation and loading..
	if (m_pProjectFile)
		delete m_pProjectFile;
	m_pProjectFile = new ProjectFile;
	ASSERT_TRUE(m_pProjectFile != NULL);

	bool success = m_pProjectFile->Read(FileName);

	// Must succeed
	ASSERT_TRUE(success == true);
}

/**
 * @brief Read left path different ways.
 */
TEST_F(SimpleLeftTest, GetLeftPath)
{
	// Has left path (only)
	bool bIsLeft = m_pProjectFile->HasLeft();
	ASSERT_TRUE(bIsLeft == true);
	
	// Get left path without read-only info
	String left = m_pProjectFile->GetLeft();
	ASSERT_TRUE(left.compare(LeftPath) == 0);

	// Get left path with read-only info
	bool bReadOnly;
	left = m_pProjectFile->GetLeft(&bReadOnly);
	ASSERT_TRUE(left.compare(LeftPath) == 0);
	ASSERT_TRUE(bReadOnly == false);
}

/**
 * @brief Make sure right path is not get.
 */
TEST_F(SimpleLeftTest, GetRightPath)
{
	// We don't have right path
	bool bIsRight = m_pProjectFile->HasRight();
	ASSERT_TRUE(bIsRight == false);

	// Get right path without read-only info
	String right = m_pProjectFile->GetRight();
	ASSERT_TRUE(right.empty());

	// Get right path with read-only info
	bool bReadOnly;
	right = m_pProjectFile->GetRight(&bReadOnly);
	ASSERT_TRUE(right.empty());
	ASSERT_TRUE(bReadOnly == false);
}

/**
 * @brief Make sure subfolder inclusion is not get.
 */
TEST_F(SimpleLeftTest, GetSubfolders)
{
	// We don't have a subfolders
	bool bHasSubfolders = m_pProjectFile->HasSubfolders();
	ASSERT_TRUE(bHasSubfolders == false);

	// Returns -1 if not set
	int subfolders = m_pProjectFile->GetSubfolders();
	ASSERT_TRUE(subfolders == -1);
}

/**
 * @brief Make sure filter is not get.
 */
TEST_F(SimpleLeftTest, GetFilter)
{
	// We don't have a filter
	bool bHasFilter = m_pProjectFile->HasFilter();
	ASSERT_TRUE(bHasFilter == false);

	String filter = m_pProjectFile->GetFilter();
	ASSERT_TRUE(filter.empty());
}

}