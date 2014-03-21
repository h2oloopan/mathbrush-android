#ifndef LINES_H_
#define LINES_H_

#define NOMINMAX

#include <stdio.h>

namespace SCGRenderer
{
	struct Line
	{
		int lineNo;
		float maxUp;
		float maxDown;
		Line * next;
	
		Line(int no=0, float up=0, float down=0,Line *n = NULL)
			: lineNo(no), maxUp(up), maxDown(down), next(n) {};
	};
	
	class CLineList
	{
	public:
		Line *first;
		Line *last;
	
		CLineList();
		CLineList(int _up, int _down);
		~CLineList();
		void addLine(Line *);
		float getUpLineTotal(int);
		Line * getLine(int);
		void adjustLinesUpDown(int, int);
		float getLineHeight(int);
		bool isEmpty();
		float getLineTotal(int _lineNo);
		void addLineSpace(float _lineSpace);
		void updateLastLineMaxUp_Down(float _up, float _down);
	};
};
#endif

