//
// HashStatistic.cpp
//
// $Id: //poco/1.4/Foundation/src/HashStatistic.cpp#1 $
//
// Library: Foundation
// Package: Hashing
// Module:  HashStatistic
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/HashStatistic.h"
#include <sstream>

namespace Poco {


HashStatistic::HashStatistic(
	UInt32 tableSize, 
	UInt32 numEntries, 
	UInt32 numZeroEntries, 
	UInt32 maxEntry, 
	std::vector<UInt32> details):
	_sizeOfTable(tableSize),
	_numberOfEntries(numEntries),
	_numZeroEntries(numZeroEntries),
	_maxEntriesPerHash(maxEntry),
	_detailedEntriesPerHash(details)
{
}


HashStatistic::~HashStatistic()
{
}


std::string HashStatistic::toString() const
{
	std::ostringstream str;
	str << "HashTable of size " << _sizeOfTable << " containing " << _numberOfEntries << " entries:\n";
	str << "  NumberOfZeroEntries: " << _numZeroEntries << "\n";
	str << "  MaxEntry: " << _maxEntriesPerHash << "\n";
	str << "  AvgEntry: " << avgEntriesPerHash() << ", excl Zero slots: " << avgEntriesPerHashExclZeroEntries() << "\n";
	str << "  DetailedStatistics: \n";
	for (size_t i = 0; i < _detailedEntriesPerHash.size(); ++i)
	{
		// 10 entries per line
		if (i % 10 == 0)
		{
			str << "\n  " << i << ":";
		}
		str << " " << _detailedEntriesPerHash[i];
	}
	str << "\n";
	str.flush();
	return str.str();
}


} // namespace Poco
