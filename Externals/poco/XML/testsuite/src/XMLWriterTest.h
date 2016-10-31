//
// XMLWriterTest.h
//
// $Id: //poco/1.4/XML/testsuite/src/XMLWriterTest.h#1 $
//
// Definition of the XMLWriterTest class.
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


#ifndef XMLWriterTest_INCLUDED
#define XMLWriterTest_INCLUDED


#include "Poco/XML/XML.h"
#include "CppUnit/TestCase.h"


class XMLWriterTest: public CppUnit::TestCase
{
public:
	XMLWriterTest(const std::string& name);
	~XMLWriterTest();

	void testTrivial();
	void testTrivialCanonical();
	void testTrivialDecl();
	void testTrivialDeclPretty();
	void testTrivialFragment();
	void testTrivialFragmentPretty();
	void testDTDPretty();
	void testDTD();
	void testDTDNotation();
	void testDTDEntity();
	void testAttributes();
	void testAttributesPretty();
	void testData();
	void testEmptyData();
	void testDataPretty();
	void testEmptyDataPretty();
	void testComment();
	void testPI();
	void testCharacters();
	void testEmptyCharacters();
	void testCDATA();
	void testRawCharacters();
	void testAttributeCharacters();
	void testDefaultNamespace();
	void testQNamespaces();
	void testQNamespacesNested();
	void testNamespaces();
	void testNamespacesNested();
	void testExplicitNamespaces();
	void testWellformed();
	void testWellformedNested();
	void testWellformedNamespace();
	void testAttributeNamespaces();
	void testEmpty();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // XMLWriterTest_INCLUDED
