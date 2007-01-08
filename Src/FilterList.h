/**
 * @file  FilterList.h
 *
 * @brief Declaration file for FilterList.
 */

#ifndef _FILTERLIST_H_
#define _FILTERLIST_H_

#include <vector>
#include "pcre.h"

using namespace std;

/**
 * @brief Container for one filtering rule / compiled expression.
 * This structure holds compiled regular expression and a original expression
 * as a string. We need the original expression string in case we want to
 * know which regular expression did match.
 */
struct filter_item
{
	char * filterAsString; /** Original regular expression string */
 	pcre *pRegExp; /**< Compiled regular expression */
	pcre_extra *pRegExpExtra; /**< Additional information got from regex study */
};

/**
 * @brief Regular expression list.
 * This class holds a list of regular expressions for matching strings.
 * The class also provides simple function for matching and remembers the
 * last matched expression.
 */
class FilterList
{
public:
	FilterList();
	~FilterList();
	
	void AddRegExp(const char *regularExpression);
	void RemoveAllFilters();
	bool HasRegExps();
	bool Match(const char *string);
	const char * GetLastMatchExpression();

private:
	vector <filter_item> m_list;
	char * m_lastMatchExpression;

};


#endif // _FILTERLIST_H_
