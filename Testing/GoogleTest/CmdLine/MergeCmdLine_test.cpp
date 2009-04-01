#include <gtest/gtest.h>
#include <windows.h>
#include <tchar.h>
#include <vector>
#include "Constants.h"  // FFILEOPEN_* flags
#include "UnicodeString.h"
#include "MergeCmdLineInfo.h"

using std::vector;

namespace
{
	// The fixture for testing command line parsing.
	class MergeCmdLineInfoTest : public testing::Test
	{
	protected:
		// You can remove any or all of the following functions if its body
		// is	empty.

		MergeCmdLineInfoTest()
		{
			// You can do set-up work for each test	here.
		}

		virtual ~MergeCmdLineInfoTest()
		{
			// You can do clean-up work	that doesn't throw exceptions here.
		}

		// If	the	constructor	and	destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp()
		{
			// Code	here will be called	immediately	after the constructor (right
			// before each test).
		}

		virtual void TearDown()
		{
			// Code	here will be called	immediately	after each test	(right
			// before the destructor).
		}

		// Objects declared here can be used by all tests in the test case for Foo.
	};

	// NOTE: Windows command line ALWAYS has the executable name as first parameter

	// Empty command line
	TEST_F(MergeCmdLineInfoTest, Empty)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe");
		EXPECT_EQ(0, cmdInfo.m_Files.size());
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

	// Empty command line containing space
	TEST_F(MergeCmdLineInfoTest, EmptySpace)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe ");
		EXPECT_EQ(0, cmdInfo.m_Files.size());
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

	// Empty command line containing tab
	TEST_F(MergeCmdLineInfoTest, EmptyTab)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe\t");
		EXPECT_EQ(0, cmdInfo.m_Files.size());
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

	// Empty command line containing tab
	TEST_F(MergeCmdLineInfoTest, EmptyEOL)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe\n");
		EXPECT_EQ(0, cmdInfo.m_Files.size());
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

	// Command line with left path
	TEST_F(MergeCmdLineInfoTest, LeftPath1)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe C:\\Temp\\");
		EXPECT_EQ(1, cmdInfo.m_Files.size());
		EXPECT_EQ("C:\\Temp", cmdInfo.m_Files[0]);
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_CMDLINE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

	// Command line with left path
	TEST_F(MergeCmdLineInfoTest, LeftPath2)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe C:\\Temp\\");
		EXPECT_EQ(1, cmdInfo.m_Files.size());
		EXPECT_EQ("C:\\Temp\\", cmdInfo.m_Files[0]);
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_CMDLINE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

	// Command line with left path
	TEST_F(MergeCmdLineInfoTest, LeftPath3)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe  C:\\Temp\\");
		EXPECT_EQ(1, cmdInfo.m_Files.size());
		EXPECT_EQ("C:\\Temp\\", cmdInfo.m_Files[0]);
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_CMDLINE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

	// Command line with left path
	// CRASHES! The tab in command line causes a crash!
#if 0
	TEST_F(MergeCmdLineInfoTest, LeftPath4)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe\tC:\\Temp\\");
		EXPECT_EQ(1, cmdInfo.m_Files.size());
		EXPECT_EQ("C:\\Temp\\", cmdInfo.m_Files[0]);
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_CMDLINE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}
#endif

	// Command line with left path
	TEST_F(MergeCmdLineInfoTest, LeftPath5)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe C:\\Temp\\ ");
		EXPECT_EQ(1, cmdInfo.m_Files.size());
		EXPECT_EQ("C:\\Temp\\", cmdInfo.m_Files[0]);
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_CMDLINE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

	// Command line with left path
	TEST_F(MergeCmdLineInfoTest, LeftPath6)
	{
		MergeCmdLineInfo cmdInfo("C:\\WinMerge\\WinMerge.exe C:\\Temp\\\t");
		EXPECT_EQ(1, cmdInfo.m_Files.size());
		EXPECT_EQ("C:\\Temp\\", cmdInfo.m_Files[0]);
		EXPECT_EQ(SW_SHOWNORMAL, cmdInfo.m_nCmdShow);
		EXPECT_FALSE(cmdInfo.m_bClearCaseTool);
		EXPECT_FALSE(cmdInfo.m_bEscShutdown);
		EXPECT_FALSE(cmdInfo.m_bExitIfNoDiff);
		EXPECT_FALSE(cmdInfo.m_bRecurse);
		EXPECT_FALSE(cmdInfo.m_bNonInteractive);
		EXPECT_FALSE(cmdInfo.m_bSingleInstance);
		EXPECT_FALSE(cmdInfo.m_bShowUsage);
		EXPECT_EQ(FFILEOPEN_CMDLINE, cmdInfo.m_dwLeftFlags);
		EXPECT_EQ(FFILEOPEN_NONE, cmdInfo.m_dwRightFlags);
		EXPECT_EQ("", cmdInfo.m_sLeftDesc);
		EXPECT_EQ("", cmdInfo.m_sRightDesc);
		EXPECT_EQ("", cmdInfo.m_sFileFilter);
		EXPECT_EQ("", cmdInfo.m_sPreDiffer);
	}

}  // namespace

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
