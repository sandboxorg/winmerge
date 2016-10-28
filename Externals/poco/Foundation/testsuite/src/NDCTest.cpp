//
// NDCTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/NDCTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "NDCTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/NestedDiagnosticContext.h"
#include <iostream>


using Poco::NDC;


NDCTest::NDCTest(const std::string& name): CppUnit::TestCase(name)
{
}


NDCTest::~NDCTest()
{
}


void NDCTest::testNDC()
{
	NDC ndc;
	assert (ndc.depth() == 0);
	ndc.push("item1");
	assert (ndc.toString() == "item1");
	assert (ndc.depth() == 1);
	ndc.push("item2");
	assert (ndc.toString() == "item1:item2");
	assert (ndc.depth() == 2);
	ndc.pop();
	assert (ndc.depth() == 1);
	assert (ndc.toString() == "item1");
	ndc.pop();
	assert (ndc.depth() == 0);
}


void NDCTest::testNDCScope()
{
	poco_ndc("item1");
	assert (NDC::current().depth() == 1);
	{
		poco_ndc("item2");
		assert (NDC::current().depth() == 2);
		{
			poco_ndc("item3");
			assert (NDC::current().depth() == 3);
			NDC::current().dump(std::cout);
		}
		assert (NDC::current().depth() == 2);
	}
	assert (NDC::current().depth() == 1);
}


void NDCTest::setUp()
{
}


void NDCTest::tearDown()
{
}


CppUnit::Test* NDCTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NDCTest");

	CppUnit_addTest(pSuite, NDCTest, testNDC);
	CppUnit_addTest(pSuite, NDCTest, testNDCScope);

	return pSuite;
}
