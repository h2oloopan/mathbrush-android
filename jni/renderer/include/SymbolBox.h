#ifndef SYMBOLBOX_H_
#define SYMBOLBOX_H_

#include "Types.h"

namespace SCGRenderer
{
	class CSymbolBox
	{
	public:
		int left;
		int right;
		int top;
		int bottom;
		
		CSymbolBox(void);
		CSymbolBox(int _left, int _top,int _bottom,int _right);
		void shift(int,int);
		int getHeight(){return bottom - top;};
		int getWidth(){return right- left;};
		SCGRECT getRectangle(int _lspace = 0);
		bool contains(int _x, int _y);
		~CSymbolBox(void);
		void stretchV(int _x, int _y);
		bool intersects(SCGRECT _rect);
		std::wstring toWString();
	};
}
#endif