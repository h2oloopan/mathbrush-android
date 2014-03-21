#include "SymbolBox.h"

namespace SCGRenderer
{
	CSymbolBox::CSymbolBox(void)
	{
		left=top=bottom=right =0;
	}

	CSymbolBox::CSymbolBox(int _left, int _top , int _bottom, int _right)
	{
		left=_left;
		top=_top;
		bottom=_bottom;
		right =_right;
	}
	CSymbolBox::~CSymbolBox(void)
	{
	}

	void CSymbolBox::shift(int x, int y)
	{
		left+=x;
		right+=x;
		top+=y;
		bottom+=y;
	}
	
	SCGRECT CSymbolBox::getRectangle(int _lspace)
	{
		SCGRECT x(left + _lspace,top,right,bottom);
		return x;
	}

	bool CSymbolBox::contains(int _x, int _y)
	{
		if (_x >= left && _x <= right && _y >= top && _y <= bottom)
			return true;
		else
			return false;
	}

	bool CSymbolBox::intersects(SCGRECT _rect)
	{
		// Sometimes the box is a point only if the box is for an invisible operation for example invisible times or apply function.
		// we need to check that kind of bozes first
/*		if (left == right)
		{
			POINT p;
			p.x = right;
			p.y = top + getHeight()/2;
			
			if (PtInRect(&_rect,p))
				return true;
			else
				return false;
		}
*/
		if (rectangles_overlap(getRectangle(), _rect))
		{
			// if intersect make sure it intersects with higher percentage than needed
			float intersectionArea = intersection(getRectangle(),_rect).area();
			float myArea = getHeight() * getWidth();
			float ratio = intersectionArea / myArea;
			if ( ratio > INTERSECTION_RATIO)
				return true;
			else
				return false;
		}
		else
			return false; // no intersection at all
	}

	void CSymbolBox::stretchV(int _top, int _bottom)
	{
		top = _top;
		bottom = _bottom;
	}

	std::wstring CSymbolBox::toWString()
	{
		wchar_t ws[50];

		swprintf(ws,50,L"%d,%d,%d,%d",left,top,right,bottom);

		std::wstring wstr = (const wchar_t * ) ws;

		return wstr;
		
	}
}