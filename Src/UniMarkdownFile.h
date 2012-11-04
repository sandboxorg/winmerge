/**
 *  @file UniMarkdownFile.h
 *
 *  @brief Declaration of UniMarkdownFile class.
 */
// ID line follows -- this is updated by SVN
// $Id$

#include <boost/scoped_ptr.hpp>
#include "Common/UniFile.h"

class CMarkdown;

/**
 * @brief XML file reader class.
 */
class UniMarkdownFile : public UniMemFile
{
public:
	UniMarkdownFile();
	virtual bool ReadString(String & line, String & eol, bool * lossy);
	virtual void Close();

protected:
	virtual bool DoOpen(const String& filename, AccessMode mode);

private:
	void Move();
	String maketstring(const char *lpd, size_t len);

	int m_depth;
	bool m_bMove;
	unsigned char *m_transparent;
	boost::scoped_ptr<CMarkdown> m_pMarkdown;
};
