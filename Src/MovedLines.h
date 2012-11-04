/** 
 * @file  MovedLines.h
 *
 * @brief Declaration of MovedLines class
 */

#ifndef _MOVEDLINES_H_
#define _MOVEDLINES_H_

#include <map>

/**
 * @brief Container class for moved lines/blocks.
 * This class contains list of moved blocs/lines we detect
 * when comparing files.
 */
class MovedLines
{
public:
	/** @brief Sides for mapping functions. */
	enum ML_SIDE
	{
		SIDE_LEFT,
		SIDE_RIGHT,
	};

	void Clear();
	void Add(ML_SIDE side1, unsigned line1, unsigned line2);
	int LineInBlock(unsigned line, ML_SIDE side);

protected:
	int FirstSideInMovedBlock(unsigned secondSideLine);
	int SecondSideInMovedBlock(unsigned firstSideLine);

private:
	typedef std::map<int, int> MovedLinesMap;
	MovedLinesMap m_moved0; /**< Moved lines map for first side */
	MovedLinesMap m_moved1; /**< Moved lines map for second side */
};

#endif // _MOVEDLINES_H_
