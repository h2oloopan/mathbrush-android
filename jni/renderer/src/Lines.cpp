#include "Lines.h"

namespace SCGRenderer
{
	// creates an empty line List
	CLineList::CLineList()
	{
		first = NULL;
		last = NULL;
	}
	//---------------------------------------------
	// creates a list and add one line to it the up and down of that line are as provided in the paramaters
	CLineList::CLineList(int _up, int _down)
	{
		first = NULL;
		last = NULL;

		Line * l = new Line(0,_up,_down,NULL);
		addLine(l);
	}
	//---------------------------------------------
	bool CLineList::isEmpty()
	{
		if (last == NULL)
			return true;
		else
			return false;
	
	}
	//---------------------------------------------
	CLineList::~CLineList()
	{
		if (isEmpty())
			return;
		Line * crt;
		while (first != last)
		{
			crt = first->next;
			delete first;
			first = crt;
		}
		delete first;
	}
	//--------------------------------------------------
	void CLineList::addLine(Line *_l)
	{
		if (isEmpty())
			first = last = _l;
		else
		{
			last->next = _l;
			last = last->next;
		}
	}
	//--------------------------------------------------
	void CLineList::updateLastLineMaxUp_Down(float _up, float _down)
	{
		if (_up > last->maxUp)
			last->maxUp = _up;
	
		if (_down > last->maxDown)
			last->maxDown = _down;
	}
	//--------------------------------------------------
	float CLineList::getUpLineTotal(int _lineNo)
	{
		// gets the total distance above a certain line no
		// we assume that we don't shift line 0
	
		Line * crtLine = first;
		float up = 0;
	
		while (crtLine->lineNo < _lineNo)
		{
			up += crtLine->maxUp + crtLine->maxDown;
			crtLine = crtLine->next;
			if (crtLine == NULL)
				break;
		}
		if (crtLine != NULL)
		{
			up += crtLine->maxUp;  // the crt line consider the up part only
		}
		return up;
	}
	//----------------------------------------------------
	Line * CLineList::getLine(int _lineNo)
	{
		if (isEmpty())
			return NULL;
		else
		{
			Line *l = first;
			while (l->lineNo != _lineNo)
				l = l->next;
			return l;
		}
	
	}
	//-------------------------------------------------------
	void CLineList::adjustLinesUpDown(int _s, int _e)
	{
		// we need to make the up, down of the first and last lines of the
		// fraction the same as the max of the lines in between
		if (isEmpty())
			return;
	
		Line *l = getLine(_s);
		
		float maxUp = l->maxUp;
		float maxDown = l->maxDown;
		while (l->lineNo != _e)
		{
			l = l->next;
			if (l->maxUp > maxUp)
				maxUp = l->maxUp;
			if (l->maxDown > maxDown)
				maxDown = l->maxDown;
		}
	
		l = getLine(_s);
		l->maxUp = maxUp;
		l->maxDown = maxDown;
	
		l = getLine(_e);
		l->maxUp = maxUp;
		l->maxDown = maxDown;
	}
	//-------------------------------------------------------
	float CLineList::getLineHeight(int _lineNo)
	{
		if (isEmpty())
			return 0;
	
		Line *l = getLine(_lineNo);
		return l->maxDown + l->maxUp; 
	}
	//--------------------------------------------------------
	float CLineList::getLineTotal(int _lineNo)
	{
		// gets the total distance above and including _lineNo
	
		Line * crtLine = first;
		float total = 0;
	
		while (crtLine->lineNo <= _lineNo)
		{
			total += crtLine->maxUp + crtLine->maxDown;
			crtLine = crtLine->next;
			if (crtLine == NULL)
				break;
		}
		return total;
	}
	//--------------------------------------------------------
	// Add line space to the top of each line in the list of lines stating from the second
	void CLineList::addLineSpace(float _lineSpace)
	{
		Line *l = first->next;
		while (l != NULL)
		{
			l->maxUp += _lineSpace;
			l = l->next;
		}
	}

}
