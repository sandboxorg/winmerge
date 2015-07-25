//
// FIFOEventTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/FIFOEventTest.cpp#2 $
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


#include "FIFOEventTest.h"
#include "DummyDelegate.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Delegate.h"
#include "Poco/Expire.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"


using namespace Poco;


#define LARGEINC 100


FIFOEventTest::FIFOEventTest(const std::string& name): CppUnit::TestCase(name)
{
}


FIFOEventTest::~FIFOEventTest()
{
}

void FIFOEventTest::testNoDelegate()
{
	int tmp = 0;
	EventArgs args;

	assert (_count == 0);
	Simple.notify(this, tmp);
	assert (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assert (_count == 0);
	
	ConstSimple += delegate(this, &FIFOEventTest::onConstSimple);
	ConstSimple -= delegate(this, &FIFOEventTest::onConstSimple);
	ConstSimple.notify(this, tmp);
	assert (_count == 0);
	
	//Note: passing &args will not work due to &
	EventArgs* pArgs = &args;
	Complex += delegate(this, &FIFOEventTest::onComplex);
	Complex -= delegate(this, &FIFOEventTest::onComplex);
	Complex.notify(this, pArgs);
	assert (_count == 0);

	Complex2 += delegate(this, &FIFOEventTest::onComplex2);
	Complex2 -= delegate(this, &FIFOEventTest::onComplex2);
	Complex2.notify(this, args);
	assert (_count == 0);

	const EventArgs* pCArgs = &args;
	ConstComplex += delegate(this, &FIFOEventTest::onConstComplex);
	ConstComplex -= delegate(this, &FIFOEventTest::onConstComplex);
	ConstComplex.notify(this, pCArgs);
	assert (_count == 0);

	Const2Complex += delegate(this, &FIFOEventTest::onConst2Complex);
	Const2Complex -= delegate(this, &FIFOEventTest::onConst2Complex);
	Const2Complex.notify(this, pArgs);
	assert (_count == 0);
}

void FIFOEventTest::testSingleDelegate()
{
	int tmp = 0;
	EventArgs args;

	assert (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assert (_count == 1);
	
	ConstSimple += delegate(this, &FIFOEventTest::onConstSimple);
	ConstSimple.notify(this, tmp);
	assert (_count == 2);
	
	EventArgs* pArgs = &args;
	Complex += delegate(this, &FIFOEventTest::onComplex);
	Complex.notify(this, pArgs);
	assert (_count == 3);

	Complex2 += delegate(this, &FIFOEventTest::onComplex2);
	Complex2.notify(this, args);
	assert (_count == 4);

	const EventArgs* pCArgs = &args;
	ConstComplex += delegate(this, &FIFOEventTest::onConstComplex);
	ConstComplex.notify(this, pCArgs);
	assert (_count == 5);

	Const2Complex += delegate(this, &FIFOEventTest::onConst2Complex);
	Const2Complex.notify(this, pArgs);
	assert (_count == 6);
	// check if 2nd notify also works
	Const2Complex.notify(this, pArgs);
	assert (_count == 7);
	
}

void FIFOEventTest::testDuplicateRegister()
{
	int tmp = 0;
	
	assert (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assert (_count == 2);
	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assert (_count == 3);
}

void FIFOEventTest::testDuplicateUnregister()
{
	// duplicate unregister shouldn't give an error,
	int tmp = 0;
	
	assert (_count == 0);

	Simple -= delegate(this, &FIFOEventTest::onSimple); // should work
	Simple.notify(this, tmp);
	assert (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assert (_count == 1);

	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assert (_count == 1);

	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.notify(this, tmp);
	assert (_count == 1);
}


void FIFOEventTest::testDisabling()
{
	int tmp = 0;
	
	assert (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple);
	Simple.disable();
	Simple.notify(this, tmp);
	assert (_count == 0);
	Simple.enable();
	Simple.notify(this, tmp);
	assert (_count == 1);

	// unregister should also work with disabled event
	Simple.disable();
	Simple -= delegate(this, &FIFOEventTest::onSimple);
	Simple.enable();
	Simple.notify(this, tmp);
	assert (_count == 1);
}

void FIFOEventTest::testFIFOOrder()
{
	DummyDelegate o1;
	DummyDelegate o2;

	assert (_count == 0);

	Simple += delegate(&o1, &DummyDelegate::onSimple);
	Simple += delegate(&o2, &DummyDelegate::onSimple2);
	int tmp = 0;
	Simple.notify(this, tmp);
	assert (tmp == 2);

	Simple -= delegate(&o1, &DummyDelegate::onSimple);
	Simple -= delegate(&o2, &DummyDelegate::onSimple2);
	
	// now try with the wrong order
	Simple += delegate(&o2, &DummyDelegate::onSimple2);
	Simple += delegate(&o1, &DummyDelegate::onSimple);

	try
	{
		tmp = 0;
		Simple.notify(this, tmp);
		failmsg ("Notify should not work");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}

void FIFOEventTest::testFIFOOrderExpire()
{
	// expire must not break order!
	DummyDelegate o1;
	DummyDelegate o2;

	assert (_count == 0);

	Simple += delegate(&o1, &DummyDelegate::onSimple, 5000);
	Simple += delegate(&o2, &DummyDelegate::onSimple2, 5000);
	int tmp = 0;
	Simple.notify(this, tmp);
	assert (tmp == 2);

	// both ways of unregistering should work
	Simple -= delegate(&o1, &DummyDelegate::onSimple, 6000);
	Simple -= delegate(&o2, &DummyDelegate::onSimple2);
	Simple.notify(this, tmp);
	assert (tmp == 2);
	
	// now start mixing of expire and non expire
	tmp = 0;
	Simple += delegate(&o1, &DummyDelegate::onSimple);
	Simple += delegate(&o2, &DummyDelegate::onSimple2, 5000);
	Simple.notify(this, tmp);
	assert (tmp == 2);

	Simple -= delegate(&o2, &DummyDelegate::onSimple2);
	// it is not forbidden to unregister a non expiring event with an expire decorator (it is just stupid ;-))
	Simple -= delegate(&o1, &DummyDelegate::onSimple, 6000);
	Simple.notify(this, tmp);
	assert (tmp == 2);

	// now try with the wrong order
	Simple += delegate(&o2, &DummyDelegate::onSimple2, 5000);
	Simple += delegate(&o1, &DummyDelegate::onSimple);
	
	try
	{
		tmp = 0;
		Simple.notify(this, tmp);
		failmsg ("Notify should not work");
	}
	catch (Poco::InvalidArgumentException&)
	{
		
	}

}

void FIFOEventTest::testExpire()
{
	int tmp = 0;
	
	assert (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple, 500);
	Simple.notify(this, tmp);
	assert (_count == 1);
	Poco::Thread::sleep(700);
	Simple.notify(this, tmp);
	assert (_count == 1);
}


void FIFOEventTest::testExpireReRegister()
{
	int tmp = 0;
	
	assert (_count == 0);

	Simple += delegate(this, &FIFOEventTest::onSimple, 500);
	Simple.notify(this, tmp);
	assert (_count == 1);
	Poco::Thread::sleep(200);
	Simple.notify(this, tmp);
	assert (_count == 2);
	// renew registration
	Simple += delegate(this, &FIFOEventTest::onSimple, 600);
	Poco::Thread::sleep(400);
	Simple.notify(this, tmp);
	assert (_count == 3);
	Poco::Thread::sleep(300);
	Simple.notify(this, tmp);
	assert (_count == 3);
}


void FIFOEventTest::testReturnParams()
{
	DummyDelegate o1;
	Simple += delegate(&o1, &DummyDelegate::onSimple);

	int tmp = 0;
	Simple.notify(this, tmp);
	assert (tmp == 1);
}

void FIFOEventTest::testOverwriteDelegate()
{
	DummyDelegate o1;
	Simple += delegate(&o1, &DummyDelegate::onSimple);
	Simple += delegate(&o1, &DummyDelegate::onSimple2);

	int tmp = 0; // onsimple requires 0 as input
	Simple.notify(this, tmp);
	assert (tmp == 2);
}

void FIFOEventTest::testAsyncNotify()
{
	Poco::FIFOEvent<int >* pSimple= new Poco::FIFOEvent<int>();
	(*pSimple) += delegate(this, &FIFOEventTest::onAsync);
	assert (_count == 0);
	int tmp = 0;
	Poco::ActiveResult<int>retArg = pSimple->notifyAsync(this, tmp);
	delete pSimple; // must work even when the event got deleted!
	pSimple = NULL;
	assert (_count == 0);
	retArg.wait();
	assert (retArg.data() == tmp);
	assert (_count == LARGEINC);
}

void FIFOEventTest::onSimple(const void* pSender, int& i)
{
	_count++;
}

void FIFOEventTest::onSimpleOther(const void* pSender, int& i)
{
	_count+=100;
}

void FIFOEventTest::onConstSimple(const void* pSender, const int& i)
{
	_count++;
}

void FIFOEventTest::onComplex(const void* pSender, Poco::EventArgs* & i)
{
	_count++;
}

void FIFOEventTest::onComplex2(const void* pSender, Poco::EventArgs & i)
{
	_count++;
}

void FIFOEventTest::onConstComplex(const void* pSender, const Poco::EventArgs*& i)
{
	_count++;
}

void FIFOEventTest::onConst2Complex(const void* pSender, const Poco::EventArgs * const & i)
{
	_count++;
}

void FIFOEventTest::onAsync(const void* pSender, int& i)
{
	Poco::Thread::sleep(700);
	_count += LARGEINC ;
}

int FIFOEventTest::getCount() const
{
	return _count;
}

void FIFOEventTest::setUp()
{
	_count = 0;
	// must clear events, otherwise repeating test executions will fail
	// because tests are only created once, only setup is called before 
	// each test run
	Simple.clear();
	ConstSimple.clear();
	Complex.clear();
	Complex2.clear();
	ConstComplex.clear();
	Const2Complex.clear();
}


void FIFOEventTest::tearDown()
{
}


CppUnit::Test* FIFOEventTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FIFOEventTest");

	CppUnit_addTest(pSuite, FIFOEventTest, testNoDelegate);
	CppUnit_addTest(pSuite, FIFOEventTest, testSingleDelegate);
	CppUnit_addTest(pSuite, FIFOEventTest, testReturnParams);
	CppUnit_addTest(pSuite, FIFOEventTest, testDuplicateRegister);
	CppUnit_addTest(pSuite, FIFOEventTest, testDuplicateUnregister);
	CppUnit_addTest(pSuite, FIFOEventTest, testDisabling);
	CppUnit_addTest(pSuite, FIFOEventTest, testFIFOOrder);
	CppUnit_addTest(pSuite, FIFOEventTest, testFIFOOrderExpire);
	CppUnit_addTest(pSuite, FIFOEventTest, testExpire);
	CppUnit_addTest(pSuite, FIFOEventTest, testExpireReRegister);
	CppUnit_addTest(pSuite, FIFOEventTest, testOverwriteDelegate);
	CppUnit_addTest(pSuite, FIFOEventTest, testAsyncNotify);
	return pSuite;
}
