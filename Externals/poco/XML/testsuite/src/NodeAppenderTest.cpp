//
// NodeAppenderTest.cpp
//
// $Id: //poco/1.4/XML/testsuite/src/NodeAppenderTest.cpp#1 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#include "NodeAppenderTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/DOM/NodeAppender.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DocumentFragment.h"
#include "Poco/DOM/AutoPtr.h"


using Poco::XML::NodeAppender;
using Poco::XML::Element;
using Poco::XML::Document;
using Poco::XML::DocumentFragment;
using Poco::XML::AutoPtr;
using Poco::XML::XMLString;


NodeAppenderTest::NodeAppenderTest(const std::string& name): CppUnit::TestCase(name)
{
}


NodeAppenderTest::~NodeAppenderTest()
{
}


void NodeAppenderTest::testAppendNode()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element>  pRoot = pDoc->createElement("root");
	pDoc->appendChild(pRoot);
	
	NodeAppender appender(pRoot);
	
	AutoPtr<Element> pElem1 = pDoc->createElement("elem");
	AutoPtr<Element> pElem2 = pDoc->createElement("elem");
	AutoPtr<Element> pElem3 = pDoc->createElement("elem");

	appender.appendChild(pElem1);
	appender.appendChild(pElem2);
	appender.appendChild(pElem3);

	assert (pRoot->firstChild() == pElem1);
	assert (pRoot->lastChild() == pElem3);
	
	assert (pElem1->nextSibling() == pElem2);
	assert (pElem2->nextSibling() == pElem3);
	assert (pElem3->nextSibling() == 0);
	
	assert (pElem1->previousSibling() == 0);
	assert (pElem2->previousSibling() == pElem1);	
	assert (pElem3->previousSibling() == pElem2);	
	
	assert (pElem1->parentNode() == pRoot);
	assert (pElem2->parentNode() == pRoot);
	assert (pElem3->parentNode() == pRoot);
}


void NodeAppenderTest::testAppendNodeList()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element>  pRoot = pDoc->createElement("root");
	pDoc->appendChild(pRoot);
	
	NodeAppender appender(pRoot);
	
	AutoPtr<DocumentFragment> pFrag1 = pDoc->createDocumentFragment();
	AutoPtr<DocumentFragment> pFrag2 = pDoc->createDocumentFragment();
	AutoPtr<DocumentFragment> pFrag3 = pDoc->createDocumentFragment();
	
	AutoPtr<Element> pElem1 = pDoc->createElement("elem");
	AutoPtr<Element> pElem2 = pDoc->createElement("elem");
	AutoPtr<Element> pElem3 = pDoc->createElement("elem");
	AutoPtr<Element> pElem4 = pDoc->createElement("elem");

	pFrag2->appendChild(pElem1);
	pFrag2->appendChild(pElem2);
	pFrag2->appendChild(pElem3);
	
	pFrag3->appendChild(pElem4);
	
	appender.appendChild(pFrag1);
	assert (pRoot->firstChild() == 0);
	
	appender.appendChild(pFrag2);
	assert (pRoot->firstChild() == pElem1);
	assert (pRoot->lastChild() == pElem3);

	assert (pElem1->nextSibling() == pElem2);
	assert (pElem2->nextSibling() == pElem3);
	assert (pElem3->nextSibling() == 0);
	
	assert (pElem1->previousSibling() == 0);
	assert (pElem2->previousSibling() == pElem1);	
	assert (pElem3->previousSibling() == pElem2);	
	
	assert (pElem1->parentNode() == pRoot);
	assert (pElem2->parentNode() == pRoot);
	assert (pElem3->parentNode() == pRoot);
	
	appender.appendChild(pFrag3);
	assert (pRoot->lastChild() == pElem4);
	assert (pElem4->parentNode() == pRoot);
	assert (pElem3->nextSibling() == pElem4);
	assert (pElem4->previousSibling() == pElem3);
}


void NodeAppenderTest::setUp()
{
}


void NodeAppenderTest::tearDown()
{
}


CppUnit::Test* NodeAppenderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NodeAppenderTest");

	CppUnit_addTest(pSuite, NodeAppenderTest, testAppendNode);
	CppUnit_addTest(pSuite, NodeAppenderTest, testAppendNodeList);

	return pSuite;
}
