#include "Symbol.h"
#include "wrapper.h"

namespace SCGRenderer
{
	extern int SYMBOLHI;
	extern OpDictHashType opDict;
	extern SymbDictHashType symbDict;
	extern int FontAscent;


	CSymbol::CSymbol(void)
	{
		separator = false;	// the real separators will overrwrite that when created
		mybox = NULL;
		lspace = THINMATHSPACE; 
		rspace = THINMATHSPACE;
		stretch = false;
		largeOp = false;
		type = NONE;
		crtLineNo = 0;
		targetLineNo = 0;
		broken = false;
		separator = false;
		selected = false;
		opForm = -1;	// if operator this will be assigned the operator form
		up = 0.0;
		down = 0.0;
		parentSymbol = NULL;
	}
	//--------------------------------------------
	CSymbol::~CSymbol(void)
	{
		if (mybox != NULL)
			delete mybox;
	}
	//--------------------------------------------
	void CSymbol::calcDepth(float l)
	{
	}
	//--------------------------------------------
	CSymbolBox* CSymbol::createBox(int _left, int _center, CDevice * _device )
	{
		if (mybox != NULL)
		{
			delete mybox;
			mybox = NULL;
		}
		crtLineNo = 0;
		targetLineNo = 0;
		broken = false;

		return NULL;
	}
	//--------------------------------------------    
    SCGRECT CSymbol::getRectangle()
    {
        if (broken)
        {
            // I am not really sure why adding this make it work but for some
            // reason it did !!!
            SCGRECT x(mybox->left, mybox->top - lineAdjustment ,mybox->left + width, mybox->top  - lineAdjustment + height + SYMBOLHI);
            
            return x;
        }
        else
            return mybox->getRectangle();
    }
	//--------------------------------------------
	void CSymbol::display(CDevice * _device, bool _showBoxes)
	{
	}
	//--------------------------------------------
	CSymbol * CSymbol::getCrtSymbol(int _x, int _y)
	{
		return this;
	}
	//--------------------------------------------
	CSymbol * CSymbol::getCrtSymbol(SCGRECT _rect)
	{
		return this;
	}
	//--------------------------------------------
	// The general selection is based on the amount of intersection with the given rectangle. This
	// applies to CSymbolBasic as well.
	bool CSymbol::selectSymbols(SCGRECT _rect)
	{
		if (mybox->intersects(_rect))
		{
			if (selected)
				selected = false;
			else
				selected = true;
			return true;
		}
		return false;
	}
	//-----------------------------------------------
	void CSymbol::setTextColor(CDevice * _device)
	{
		if (selected)
		{
			_device->setTextColor(SELECTEDTEXTCOLOR);
		}	
		else
		{
			_device->setTextColor(REGULARTEXTCOLOR);
		}
	}
	//-----------------------------------------------
	void CSymbol::setPenColor(CDevice * _device)
	{
		//COLORREF penColor;
		if (selected)
		{
			//penColor = RGB(MYSELECTEDTEXTCOLOR_R,MYSELECTEDTEXTCOLOR_G,MYSELECTEDTEXTCOLOR_B);
			_device->setPenColor(SELECTEDPENCOLOR);
		}	
		else
		{
			//penColor = RGB(MYTEXTCOLOR_R,MYTEXTCOLOR_G,MYTEXTCOLOR_B);
			_device->setPenColor(REGULARPENCOLOR);
		}
		//SetDCPenColor (_device, penColor);
	}
	//------------------------------------------------
	void CSymbol::setBackColor(CDevice *_device, bool _highlight)
	{
		if (_highlight)
		{
			_device->setBackColor(HIGHLIGHTBACKCOLOR);
		}	
		else
		{
			_device->setBackColor(REGULARBACKCOLOR);
		}
	}
	//------------------------------------------------
	void CSymbol::moveTo(int _x, int _y)
	{
		if (broken)
			_y += lineAdjustment;

		if (mybox->left != _x || mybox->top != _y)
		{
			int shiftX = _x - mybox->left;
			int shiftY = _y - mybox->top;
			shift(shiftX,shiftY);
		}
	}
	//---------------------------------------------
	void CSymbol::shift(int _x, int _y)
	{
		mybox->shift(_x,_y);
	}
	//---------------------------------------------
	void CSymbol::stretchBox(int _top, int _bottom, int _left, int _right)
	{
		stretchBoxV(_top,_bottom);
		stretchBoxH(_left,_right);
	}
	//---------------------------------------------
	/* we will override the following function in classes that they can include 
	embellished operators so the whole embellished operator can stretch */
	void CSymbol::stretchBoxV(int _top, int _bottom)
	{
		mybox->stretchV(_top,_bottom);
	}
	//---------------------------------------------
	void CSymbol::stretchBoxH(int _left, int _right)
	{
		mybox->left = _left;
		mybox->right = _right;
	}
	//---------------------------------------------
	int CSymbol::getTop()
	{
		return mybox->top;
	}
	//---------------------------------------------
	int CSymbol::getBottom()
	{
		return mybox->bottom;
	}
	//---------------------------------------------
	int CSymbol::getLeft()
	{
		return mybox->left;
	}
	//---------------------------------------------
	int CSymbol::getRight()
	{
		return mybox->right;
	}
	//---------------------------------------------
	int CSymbol::getHeight()
	{
		if (broken)
			return height;
		else
			return mybox->getHeight();
	}
	//---------------------------------------------
	int CSymbol::getWidth()
	{
		if (broken)
			return width;
		else
			return mybox->getWidth();
	}
	//---------------------------------------------
	void CSymbol::setHeightWidth(int _h, int _w)
	{
		height = _h;
		width = _w;
		if (mybox->top < 0)
			lineAdjustment = mybox->top;
		else
			lineAdjustment = 0;
	}
	//----------------------------------------------
	bool CSymbol::contains(int _x , int _y)
	{
		if (mybox->contains(_x,_y))
			return true;
		else
			return false;
	}
	//---------------------------------------------------------
	std::string CSymbol::getExprTypeName()
	{
		expressionType exType = getExprType();
		return expressionTypeStr[(int) exType];
	}
	//---------------------------------------------------------
	float CSymbol::breakLine(int _scrLeft,int _scrRight,int _indent,int &_lastLeft, int & _targetLine, CLineList * _lines)
	{
		return 0;
	}
	//---------------------------------------------------------
	float CSymbol::adjustLines(CLineList * _oldLines, CLineList * _lines)
	{
		return 0;
	}
	//-------------------------------------------------
	void CSymbol::setTargetLineNo(int _lineNo)
	{
		targetLineNo = _lineNo;
	}
	//---------------------------------------------
	void CSymbol::setBox(int _left, int _right, int _top, int _bottom)
	{
		mybox = new CSymbolBox(_left,_top,_bottom,_right);
	}
	////--------------------------------------------------
	void CSymbol::adjustBoxes(float _newUp, float _newDown)
	{
		// change the top and bottom based on the change of up, down
		mybox->top = mybox->top + round((up - _newUp)*SYMBOLHI);
		mybox->bottom = mybox->bottom + round((_newDown - down)*SYMBOLHI);
		up = _newUp;
		down = _newDown;
	}
	//=========================================================================
	//=========================================================================
	CSymbolBasic::CSymbolBasic(const wchar_t * _txt, SCGRenderer::tokenType _type, int size, scg::ExpressionTree * _exprTree)
	{
		size_t originalSize = size + 1; // original size of the string
		size_t convertedChars = 0; // the number of converted characters

		char * ansiText = new char [originalSize];
		//wcstombs_s(&convertedChars,ansiText,originalSize,_txt,originalSize+1);
		convertedChars = wcstombs(ansiText, _txt, originalSize);
				 
		txt = NULL;
		originalTxt = NULL;

		type = _type;
		noOfChildren = 0;

		if (type == SCGRenderer::OPERATOR)
		{
			// get the information from the operator dictionary the txt contains the character that will be displayed
			// the originalTxt contains the original text in the mathML
			OperatorDictData data;
//			_strlwr_s(ansiText,originalSize);  // convert to lower case since all dictionary entries of operators are lower case 
			int pos = opDict.find(ansiText);
			if (pos!= ITEM_NOT_FOUND)
			{
				data = opDict.getData((char *)ansiText);		
				lspace = data.lspace;
				rspace = data.rspace;
				stretch = data.stretch;
				largeOp = data.largeOp;
				separator = data.separator;
				opForm = data.form;

				originalSize = data.visual.length() + 1;
				
				txt = new wchar_t [originalSize];
				wcscpy(txt,data.visual.c_str());
			}
			else
			{
				// if the operator is not found copy the text in the txt field until the display time.
				txt = new wchar_t [size + 1];
				wcscpy(txt,_txt); 
			}
			originalTxt = new char [size+1];
			strcpy(originalTxt,ansiText);			
		}
		else if (type == SCGRenderer::EXTERNAL_IDENTIFIER) // comes from the MathML
		{
			SymbolsDictData data2;
			
			// if external itentifier the txt contains the unicode value and the originaltxt will be used if the symbol is selected and MathML needs to be generated for it
			int pos = symbDict.find(ansiText);
			if (pos != ITEM_NOT_FOUND)
			{
				data2 = symbDict.getData((char *) ansiText);
				lspace = data2.lspace;
				txt = new wchar_t [2];
				wcscpy(txt,(wchar_t *) data2.unicode.c_str());

				originalTxt = new char [size+1];
				strcpy(originalTxt,ansiText);			
			}
			else
			{
				txt = new wchar_t [3];
				wcscpy(txt,L"??"); 
			}
		}
		else
		{
			// the txt contains the original text in the MathML and the originaltxt is NULL
			txt = new wchar_t[size+1];
			wcscpy(txt,_txt);
		}

		delete[] ansiText;
		exprTree = _exprTree;
	}
	//--------------------------------------------
	CSymbolBasic::~CSymbolBasic(void)
	{
		delete [] txt;
		if (originalTxt != NULL)
			delete [] originalTxt;

		if (broken)
		{
			for (int i=0; i < noOfChildren; i++)
				delete children[i];
			delete children;
		}
	}
	//--------------------------------------------
	bool CSymbolBasic::isTxt(wchar_t * _txt)
	{
		return !wcscmp(txt,_txt);
	}
	//--------------------------------------------
	void CSymbolBasic::calcDepth(float l)
	{
		if (type == OPERATOR && largeOp)
		{
			up = down = l;
			//down = l;
		}
		if (type == OPERATOR && equals(originalTxt,"&fracbar;",false))
		{
			up = down = 0.1 * l;
		}
		else if (type == NONE || type == PRESCRIPT)
			up = down = 0;
		else
		{	
			up = 0.5*l;
			down = 0.5*l;
		}
	}
	//--------------------------------------------
	void CSymbolBasic::shift(int _x,int _y)
	{
		if (broken)
		{
			for(int i = 0; i < noOfChildren; i++)
				children[i]->shift(_x,_y);
			mybox->shift(_x,_y);
		}
		else
			mybox->shift(_x,_y);
	}
	//--------------------------------------------
	CSymbolBox* CSymbolBasic::createBox(int _left, int _center, CDevice * _device )
	{
		LOG("CSymbolBasic::createBox %d %d", _left, _center);
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x;
		x= new CSymbolBox();
		x->left=_left; // current position is the start of the bo

		SCGRECT txtSize = _device->getStringBox(txt);
        //std::cout << "for text " << (wchar_t *) txt;
        //std::cout << " I got a box of width " << scg::width(txtSize);
        //std::cout << " and height " << scg::height(txtSize) << "\n";
		x->right = _left + lspace + txtSize.width() + rspace;
		x->top = _center - up * txtSize.height();
		x->bottom = _center + down*txtSize.height();
		mybox = x;
		return x;
	}
	//--------------------------------------------
	void CSymbolBasic::display(CDevice * _device, bool _showBoxes)
	{
		if (broken)
		{
			for (int i=0; i < noOfChildren; i++)
				children[i]->display(_device,_showBoxes);
			return;
		}
	
		if (type == SCGRenderer::NONE  || type == SCGRenderer::PRESCRIPT)
			return;
	
		setBackColor(_device, _showBoxes);
		if (_showBoxes)
			_device->fillRect(mybox->getRectangle(lspace));

		setTextColor(_device);
		setPenColor(_device);

		int width = mybox->getWidth();
		int r = width/2;

		if (originalTxt != NULL)  // an operator or an external identifier
		{
			if (equals(originalTxt,"(",false) && mybox->getHeight() > SYMBOLHI)
			{
				SCGRECT containingRect(mybox->left+r,mybox->top+2,mybox->right+r,mybox->top+width+2);
				_device->drawArc(mybox->right,mybox->top+2,mybox->left + r ,mybox->top+r+3,containingRect);
				containingRect.top = mybox->bottom-width-2;
				containingRect.bottom = mybox->bottom-2;
				_device->drawArc(mybox->right+1,mybox->bottom,mybox->left+r,mybox->bottom-r-2,containingRect);
				_device->drawLine(mybox->left+r,mybox->top+r+2,mybox->left+r,mybox->bottom-r-2);
			}
			else if (equals(originalTxt,")",false) && mybox->getHeight() > SYMBOLHI)
			{
                SCGRECT containingRect(mybox->left,mybox->top+2,mybox->left+2*r,mybox->top + width+2);
				_device->drawArc(mybox->left+r,mybox->top+2,mybox->left+2*r,mybox->top+2*r+2,containingRect);
				containingRect.top = mybox->bottom-width-2;
				containingRect.bottom = mybox->bottom-2;
				_device->drawArc(mybox->left+r,mybox->bottom-2,mybox->left+2*r,mybox->bottom-r-2,containingRect);
				_device->drawLine(mybox->left+2*r,mybox->top+r+2,mybox->left+2*r,mybox->bottom-r-2);

			}
			else if (equals(originalTxt,"[",false) && mybox->getHeight() > SYMBOLHI)
			{
				_device->drawLine(mybox->right,mybox->top,mybox->left + r,mybox->top);
				_device->drawLine(mybox->left + r,mybox->top,mybox->left + r,mybox->bottom);
				_device->drawLine(mybox->left + r,mybox->bottom,mybox->right,mybox->bottom);
			}
			else if (equals(originalTxt,"]",false) && mybox->getHeight() > SYMBOLHI)
			{
				_device->drawLine(mybox->left +lspace,mybox->top,mybox->right - r + lspace,mybox->top);
				_device->drawLine(mybox->right - r + lspace ,mybox->top,mybox->right - r + lspace,mybox->bottom);
				_device->drawLine(mybox->right - r + lspace,mybox->bottom,mybox->left + lspace,mybox->bottom);
			}
			else if (equals(originalTxt,"{",false) && mybox->getHeight() > SYMBOLHI)
			{
				int l = mybox->getHeight() / 2;
				_device->drawLine(mybox->right,mybox->top,mybox->right - r,mybox->top+r);
				_device->drawLine(mybox->right - r,mybox->top+r,mybox->right-r,mybox->top+l-r);
				_device->drawLine(mybox->right-r,mybox->top+l-r,mybox->left,mybox->top+l);
				_device->drawLine(mybox->left,mybox->top+l,mybox->right-r,mybox->bottom-l+r);
				_device->drawLine(mybox->right-r,mybox->bottom-l+r,mybox->right-r,mybox->bottom - r);
				_device->drawLine(mybox->right-r,mybox->bottom - r,mybox->right,mybox->bottom);
			}
			else if (equals(originalTxt,"}",false) && mybox->getHeight() > SYMBOLHI)
			{
				int l = mybox->getHeight() / 2;
				_device->drawLine(mybox->left,mybox->top,mybox->left + r,mybox->top+r);
				_device->drawLine(mybox->left + r,mybox->top+r,mybox->left+r,mybox->top+l-r);
				_device->drawLine(mybox->left+r,mybox->top+l-r,mybox->right,mybox->top+l);
				_device->drawLine(mybox->right,mybox->top+l,mybox->left+r,mybox->bottom-l+r);
				_device->drawLine(mybox->left+r,mybox->bottom-l+r,mybox->left+r,mybox->bottom - r);
				_device->drawLine(mybox->left+r,mybox->bottom - r,mybox->left,mybox->bottom);
			}
			else if (equals(originalTxt,"&integral;",false))
			{
				SCGRECT containingRect (mybox->left+r,mybox->top+2,mybox->right+r,mybox->top+width+2);
				_device->drawArc(mybox->right,mybox->top+2,mybox->left + r ,mybox->top+r+3,containingRect);
				containingRect.left =mybox->left-r;
				containingRect.top = mybox->bottom-width-2;
				containingRect.right = mybox->left+r;
				containingRect.bottom = mybox->bottom-2;
				_device->drawArc(mybox->left,mybox->bottom-2,mybox->left+r,mybox->bottom-r-2,containingRect);
				_device->drawLine(mybox->left+r,mybox->top+r+2,mybox->left+r,mybox->bottom-r-2);
			}
			else if (equals(originalTxt,"/",false) && mybox->getHeight() > SYMBOLHI)
			{
				_device->drawLine(mybox->right,mybox->top,mybox->left,mybox->bottom);
			}
			else if (equals(originalTxt,"&rightarrow;",false)) // need to consider horizontal stretching
			{
				_device->drawLine(mybox->left+lspace,mybox->top + mybox->getHeight()/2,mybox->right - rspace,mybox->top + mybox->getHeight()/2);
				_device->drawLine(mybox->right - rspace,mybox->top + mybox->getHeight()/2,mybox->right - rspace - 0.25 * mybox->getWidth(),mybox->top + mybox->getHeight()/2 - SYMBOLHI/4);
				_device->drawLine(mybox->right - rspace,mybox->top + mybox->getHeight()/2,mybox->right - rspace - 0.25 * mybox->getWidth(),mybox->top + mybox->getHeight()/2 + SYMBOLHI/4);
			}
			else if (equals(originalTxt,"&fracbar;",false))
			{
				_device->drawLine(mybox->left,mybox->top + mybox->getHeight()/2,mybox->right,mybox->top+mybox->getHeight()/2);
			}
			else  // might be an operator but it will be displayed regularly or an external identifier
				_device->drawText(txt, mybox->left + lspace, mybox->top);
		}
		else  // originalTxt = NULL display whatever is in txt
		{
			// SKIP THE ITALIC PART FOR NOW -- TODO
			_device->drawText(txt,mybox->left,mybox->top);				
		}
	}
	//-----------------------------------------------------------
	CSymbol * CSymbolBasic::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y) && exprTree != NULL)
			return this;
		else
			return NULL;
	}
	//-----------------------------------------------------------
	CSymbol * CSymbolBasic::getCrtSymbol(SCGRECT _rect)
	{
		if (mybox->intersects(_rect) && exprTree != NULL)
			return this;
		else
			return NULL;
	}
	//----------------------------------------------------------------------
	bool CSymbolBasic::selectSymbols(SCGRECT _rect)
	{
		if (!broken)
			return CSymbol::selectSymbols(_rect);

		// broken
		bool somethingSelected = false;
		if (children[0]->selectSymbols(_rect))
			somethingSelected = true;

		for (int i =1; i < noOfChildren; i++)
			children[i]->selected = children[0]->selected;
		selected = children[0]->selected;

		return somethingSelected;
	}
	//----------------------------------------------------------------------
	void CSymbolBasic::clearSelection()
	{
		selected = false;
		if (broken)
			for (int i =0; i < noOfChildren; i++)
				children[i]->selected = false;
	}
	//----------------------------------------------------------------------
	void CSymbolBasic::getSelectedSymbols(std::wstring &_ws)
	{
		if (selected)
		{
			_ws += mybox->toWString() + L"," + (const wchar_t *) txt + L"\n";;
		}
	}
	//-----------------------------------------------------------------------------
	void CSymbolBasic::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		if (selected)
		{
			if (originalTxt != NULL)
			{
				if (equals(originalTxt,"&fracbar;",false) || equals(originalTxt,"&rightarrow;",false) || mybox->getHeight() > SYMBOLHI)
				{
					_symbols.push_back(scg::MathSymbol((scg::unicode_char) txt[0], scg::Rect<long>(mybox->left, mybox->top,mybox->right,mybox->bottom)));
					return;
				}
			}

			// coming here means no special operators sizes
			// when we have multiple characters identifiers or numbers we need to devide the big box between them
			int i = 0;  // the index of the character in the multiple charaters text
			int step = 0; // get approximately the width of each character in the txt.
			
			if (wcslen(txt) != 0)
				step = mybox->getWidth()/wcslen(txt); 

			for (scg::unicode_char crtChar = (scg::unicode_char) txt[0];crtChar != '\0'; crtChar = txt[++i])
			{
				SCGRECT rect = _device->getUnicodeExactBox(crtChar);
				_symbols.push_back(scg::MathSymbol(crtChar,scg::Rect<long>(rect.left,rect.top,rect.right,rect.bottom))); 
			}
		}
	}
	//-----------------------------------------------------------------------------
	std::string CSymbolBasic::getSelectedMathML()
	{
		if (!selected)
			return "";

		std::string s;
		if (originalTxt != NULL) // an operator or an external identifier
		{
			s = (const char * ) originalTxt;		
			if (type == OPERATOR)
				s = "<mo>" + s + "</mo>";
			else // an external identifier
				s = "<mi>" + s + "</mi>";
		}
		else // original txt not null we will use what ever is in txt
		{
			size_t size = wcslen(txt) + 1; // original size of the string
			size_t convertedChars = 0; // the number of converted characters

			char * ansiText = new char [size];
			convertedChars = wcstombs(ansiText,txt,size+1);

			s = (const char *) ansiText;

			if (type == OPERATOR)
				s = "<mo>" + s + "</mo>";
			else if (type == IDENTIFIER)
				s = "<mi>" + s + "</mi>";
			else if (type == NUMBER)
				s = "<mn>" + s + "</mn>";
			else
				s = "<munknown>" + s + "</munknown>";

			delete (ansiText);
		}

		return s;
	}
	//-----------------------------------------------------------------------------
	expressionType CSymbolBasic::getExprType()
	{
		if (type == OPERATOR)
		{
			if (!wcscmp(txt,L"=") || !wcscmp(txt,L">=") || !wcscmp(txt,L"!=") || !wcscmp(txt,L">") || !wcscmp(txt,L"<=") || !wcscmp(txt,L"<"))
				return EQUAL;
			else if (!(wcscmp(txt,L"+") && wcscmp(txt,L"-")))
				return PLUS;
            else if (!(wcscmp(txt,L",")))
                return LIST;
			else if (equals(originalTxt,"&invisibletimes;",false))
				return TIMES;
			else if (!wcscmp(txt,L"^")) 
				return POWER;
			else if (!wcscmp(txt,L"{"))
				return CURLYBRACE;
			else if (!wcscmp(txt,L"["))
				return SQUAREBRACKET;
			else if (equals(originalTxt,"&integral;",false) && !equals(originalTxt,"&sum;",false) && !equals(originalTxt,"&product;",false))
				return FUNCTION;
			else if (isApplyFunction())
				return FUNCTION;
			else
				return NULLTYPE;
		}
		else if (type == NUMBER)
			return NUMERIC;
		else
			return NULLTYPE;
	}
	//------------------------------------------------------------------------------
	void CSymbolBasic::generateVariables(std::vector<std::wstring> &_vars)
	{
		if (type == IDENTIFIER || type == EXTERNAL_IDENTIFIER)
		{
			std::wstring val(txt);
			_vars.push_back(val);
		}
	}
	//------------------------------------------------------------------------------
    void CSymbolBasic::generateVariables(std::vector<CSymbol *> & _vars)
    {
        if (type == IDENTIFIER || type == EXTERNAL_IDENTIFIER)
        {
            _vars.push_back(this);
        }
    }
	//------------------------------------------------------------------------------
	bool CSymbolBasic::isApplyFunction()
	{
		if (type == OPERATOR)
		{
			if (equals(originalTxt,"&applyfunction;",false))
				return true;
		}
		return false;
	}
	//---------------------------------------------------------
	float CSymbolBasic::breakLine(int _scrLeft,int _scrRight,int _indent,int &_lastLeft, int & _targetLine, CLineList * _lines)
	{
		int lastLeft = _lastLeft;
		int targetLine = _targetLine;
		int firstXShift = 0;
	
		// recreate the lines everytime we try to break them
		if (broken)
		{
			broken = false;
			for(int i = 0; i < noOfChildren; i++)
				delete children[i];
			delete children;
		}
	
		// check if we will need to breal at all
		if (getWidth() + lastLeft > _scrRight)
		{
			if (getWidth() + _scrLeft + _indent > _scrRight)
			{
				// we will have to break anyway so start breaking where we are
				broken = true;
				// estimate the number of lines we need for the txt of this symbol 
				int charWidth = mybox->getWidth()/ wcslen(txt);
				int noCharactersPerFullLine = (_scrRight - _scrLeft - _indent) / charWidth;
				int noCharactersInCrtLine = (_scrRight - lastLeft) / charWidth;
				int noOfLines = 2 + (wcslen(txt) - noCharactersInCrtLine)/ noCharactersPerFullLine;
				//firstXShift = lastLeft - getLeft();
				noOfChildren = noOfLines;
				children = (CSymbolBasic **) malloc(sizeof(CSymbolBasic *) * noOfChildren);
				int crtPos = 0;
	
				for (int i = 0; i < noOfLines; i++)
				{
					if (lastLeft >= _scrRight)
					{
						targetLine++;
						lastLeft = _scrLeft + _indent;
						Line * l = new Line(targetLine,0,0,NULL);
						_lines->addLine(l);
					}
	
					int noOfCharacters = (_scrRight - lastLeft) / charWidth;
					wchar_t * subStr = new wchar_t[noOfCharacters + 2];
					wcsncpy(subStr,txt+crtPos,noOfCharacters);
					subStr[noOfCharacters] = '\0';
					crtPos += noOfCharacters;
					if (i < noOfChildren-1)
						wcscat(subStr,L"\\");
	                children[i] = new CSymbolBasic(subStr, type, noOfCharacters + 1);
					children[i]->crtLineNo = crtLineNo;
					children[i]->up = up;
					children[i]->down = down;
					children[i]->setTargetLineNo(targetLine);
					children[i]->setBox(lastLeft,lastLeft + wcslen(subStr) * charWidth ,mybox->top,mybox->bottom);
					_lines->updateLastLineMaxUp_Down(children[i]->up,children[i]->down);
					delete[] subStr;
					if (i == noOfChildren - 1) // last line
						lastLeft = children[i]->getRight();
					else
						lastLeft = _scrRight;
				}
			}
			else
			{
				// we can just start in a new line and it will fit
				targetLine++;
				lastLeft = _scrLeft + _indent;
				Line * l = new Line(targetLine,0,0,NULL);
				l->maxDown = down;
				l->maxUp = up;
				_lines->addLine(l);
				firstXShift = lastLeft - getLeft(); 
				setTargetLineNo(targetLine);
				lastLeft = getRight();
				shift(firstXShift,0);
			}
		}
		else
		{
			// we do not need to break
			firstXShift = lastLeft - getLeft();
			lastLeft = getRight();
			shift(firstXShift,0);
			setTargetLineNo(targetLine);
			_lines->updateLastLineMaxUp_Down(up,down);
		}
	
		_lastLeft = lastLeft;
		_targetLine = targetLine;
	
		return firstXShift;
	}
	//---------------------------------------------------------
	float CSymbolBasic::adjustLines(CLineList * _oldLines, CLineList * _lines)
	{
		float YShift=0;
	
		if (!broken)
		{
			float myOldLineUp = _oldLines->getUpLineTotal(crtLineNo);
			float myNewLineUp = _lines->getUpLineTotal(targetLineNo);
			YShift = myNewLineUp - myOldLineUp;
			shift(0,round(YShift*SYMBOLHI));  
			crtLineNo = targetLineNo;
		}
		else
		{
			for (int i=0; i < noOfChildren; i++)
			{
				float y = children[i]->adjustLines(_oldLines,_lines);
				if (i == 0)
					YShift = y;
			}
		}
                
		return YShift;
	}
	//----------------------------------------------
	bool CSymbolBasic::hasEllipses() 
	{
		if (type == SCGRenderer::OPERATOR)
		{
			if (!wcscmp(txt,L"\u22EE"))
				return true;
			else if (!wcscmp(txt,L"\u22EF"))
				return true;
			else if (!wcscmp(txt,L"\u22F0"))
				return true;
			else if (!wcscmp(txt,L"\u22F1"))
				return true;
		}
		return false;
	}
	//=========================================================================
	//=========================================================================
	
	CSymbolFrac::CSymbolFrac(CSymbol *_num, CSymbol *_denom,scg::ExpressionTree * _exprTree)
	{
		denominator = _denom;
		numerator = _num;
		fracBar = new CSymbolBasic(L"&fracbar;",OPERATOR,wcslen(L"&fracbar;"));

		numeratorOpenFence = new CSymbolBasic(L"(",OPERATOR,1);
		numeratorCloseFence = new CSymbolBasic(L")",OPERATOR,1);
		denominatorOpenFence = new CSymbolBasic(L"(",OPERATOR,1);
		denominatorCloseFence = new CSymbolBasic(L")",OPERATOR,1);
		divSign = new CSymbolBasic(L"/",OPERATOR,1);
		one = new CSymbolBasic(L"1",NUMBER,1);
		
		exprTree = _exprTree;
		
		// set the parent symbol of all the children
		numerator->parentSymbol = this;
		denominator->parentSymbol = this;
		fracBar->parentSymbol = this;
		numeratorOpenFence->parentSymbol = this;
		numeratorCloseFence->parentSymbol = this;
		denominatorOpenFence->parentSymbol = this;
		denominatorCloseFence->parentSymbol = this;
		divSign->parentSymbol = this;
		one->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolFrac::~CSymbolFrac(void)
	{
		delete(denominator);
		delete(numerator);
		delete(fracBar);
		delete(numeratorOpenFence);
		delete(numeratorCloseFence);
		delete(denominatorOpenFence);
		delete(denominatorCloseFence);
		delete(divSign);
		delete(one);
	}
	//------------------------------------------------
	void CSymbolFrac::calcDepth(float l)
	{	
		numerator->calcDepth(l);
		denominator->calcDepth(l);
		fracBar->calcDepth(l);
		numeratorOpenFence->calcDepth(l);
		numeratorCloseFence->calcDepth(l);
		denominatorOpenFence->calcDepth(l);
		denominatorCloseFence->calcDepth(l);
		divSign->calcDepth(l);
		one->calcDepth(l);
	
		up = numerator->up+numerator->down+fracBar->up;
		down = denominator->up+denominator->down+fracBar->down;
	}
	//--------------------------------------------
	CSymbolBox* CSymbolFrac::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x,*d,*n;
		x = new CSymbolBox;
	
		x->left = _left;
		n = numerator->createBox(_left + THINMATHSPACE,_center - (numerator->down+fracBar->up)*SYMBOLHI,_device);
		d = denominator->createBox(_left + THINMATHSPACE,_center + (denominator->up+fracBar->down)*SYMBOLHI,_device);
		fracBar->createBox(_left+THINMATHSPACE,_center,_device);

		int w = d->getWidth()>n->getWidth()?d->getWidth():n->getWidth();
		fracBar->stretchBoxH(fracBar->getLeft(),fracBar->getLeft()+w);

		x->right=_left + w + 2* THINMATHSPACE;
		x->top = numerator->getTop();
		x->bottom = denominator->getBottom();
	
		numerator->shift((w-n->getWidth())/2,0);
		denominator->shift((w-d->getWidth())/2,0);
	
		mybox = x;
	
		// create boxes for the parethesis
		numeratorOpenFence->createBox(getLeft(),_center,_device);
		numeratorCloseFence->createBox(getRight(),_center,_device);
		denominatorOpenFence->createBox(getLeft(),_center,_device);
		denominatorCloseFence->createBox(getRight(),_center,_device);
		divSign->createBox(getLeft(),_center,_device);
		one->createBox(getLeft(),_center-one->down * SYMBOLHI,_device);
	
		return x;
	}
	//----------------------------------------------
	void CSymbolFrac::display(CDevice * _device, bool _showBoxes)
	{
	    denominator->display(_device,_showBoxes);
		numerator->display(_device, _showBoxes);

		if (broken)
		{
			if (one->targetLineNo == -1) // there is no 1/denominator
			{
				// need to display parenthesis and / sign
				numeratorOpenFence->display(_device,_showBoxes);
				numeratorCloseFence->display(_device,_showBoxes);
				denominatorOpenFence->display(_device,_showBoxes);
				denominatorCloseFence->display(_device,_showBoxes);
				divSign->display(_device,_showBoxes);
			}
			else
			{
				// need to display 1/ denominator
				one->display(_device,_showBoxes);
				numeratorOpenFence->display(_device,_showBoxes);
				numeratorCloseFence->display(_device,_showBoxes);
				//MoveToEx(_device,denominator->getLeft()+THINMATHSPACE,denominator->getTop(),NULL);
				//_device->drawLineTo(denominator->getRight()-THINMATHSPACE, denominator->getTop());
				fracBar->display(_device,_showBoxes);
			}
		}
		else
		{
			fracBar->display(_device,_showBoxes);
			//setPenColor(_device);
			//MoveToEx(_device,mybox->left+THINMATHSPACE,numerator->getBottom(),NULL);
			//_device->drawLineTo(mybox->right-THINMATHSPACE,numerator->getBottom());
		}
	}
	//------------------------------------------------
	void CSymbolFrac::shift(int _x,int _y)
	{
		numerator->shift(_x,_y);
		denominator->shift(_x,_y);
		fracBar->shift(_x,_y);
		numeratorOpenFence->shift(_x,_y);
		numeratorCloseFence->shift(_x,_y);
		denominatorOpenFence->shift(_x,_y);
		denominatorCloseFence->shift(_x,_y);
		divSign->shift(_x,_y);
		one->shift(_x,_y);
	
		mybox->shift(_x,_y);
	}
	//-------------------------------------------------
	CSymbol * CSymbolFrac::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			CSymbol * crtSymbol = numerator->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;
			
			crtSymbol = denominator->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;

			return this;
		}
		else
			return NULL;
	}
	//---------------------------------------------------
	CSymbol * CSymbolFrac::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			CSymbol * crtSymbol = numerator->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = denominator->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			return NULL;
		}
	}
	//-------------------------------------------------------
	bool CSymbolFrac::selectSymbols(SCGRECT _rect)
	{
		bool somethingSelected = false;

		if (numerator->selectSymbols(_rect))
			somethingSelected = true;

		if (denominator->selectSymbols(_rect))
			somethingSelected = true;

		if (fracBar->selectSymbols(_rect))
			somethingSelected = true;

		if (broken)
		{
			if (numeratorOpenFence->selectSymbols(_rect))
				somethingSelected = true;
			if (numeratorCloseFence->selectSymbols(_rect))
				somethingSelected = true;
			if (denominatorOpenFence->selectSymbols(_rect))
				somethingSelected = true;
			if (denominatorCloseFence->selectSymbols(_rect))
				somethingSelected = true;
			if (divSign->selectSymbols(_rect))
				somethingSelected = true;
			if (one->selectSymbols(_rect))
				somethingSelected = true;

		}
		return somethingSelected;
	}
	//---------------------------------------------------
	void CSymbolFrac::clearSelection()
	{
		numerator->clearSelection();
		denominator->clearSelection();
		fracBar->clearSelection();
		numeratorOpenFence->clearSelection();
		numeratorCloseFence->clearSelection();
		denominatorOpenFence->clearSelection();
		denominatorCloseFence->clearSelection();
		divSign->clearSelection();
		one->clearSelection();
	}
	//---------------------------------------------------
	void CSymbolFrac::getSelectedSymbols(std::wstring &_ws)
	{
		numerator->getSelectedSymbols(_ws);
		denominator->getSelectedSymbols(_ws);
		fracBar->getSelectedSymbols(_ws);

		if (broken)
		{
			numeratorOpenFence->getSelectedSymbols(_ws);
			numeratorCloseFence->getSelectedSymbols(_ws);
			denominatorOpenFence->getSelectedSymbols(_ws);
			denominatorCloseFence->getSelectedSymbols(_ws);
			divSign->getSelectedSymbols(_ws);
			one->getSelectedSymbols(_ws);
		}
	}
	//---------------------------------------------------
	void CSymbolFrac::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		numerator->getSelectedSymbols(_symbols,_device);
		denominator->getSelectedSymbols(_symbols,_device);
		fracBar->getSelectedSymbols(_symbols,_device);

		if (broken)
		{
			numeratorOpenFence->getSelectedSymbols(_symbols,_device);
			numeratorCloseFence->getSelectedSymbols(_symbols,_device);
			denominatorOpenFence->getSelectedSymbols(_symbols,_device);
			denominatorCloseFence->getSelectedSymbols(_symbols,_device);
			divSign->getSelectedSymbols(_symbols,_device);
			one->getSelectedSymbols(_symbols,_device);
		}
	}
	//---------------------------------------------------
	std::string CSymbolFrac::getSelectedMathML()
	{
		std::string numeratorStr = numerator->getSelectedMathML();
		std::string denominatorStr = denominator->getSelectedMathML();
		
		if (numeratorStr == "" && broken)
			numeratorStr = one->getSelectedMathML();

		if (numeratorStr != "" && denominatorStr != "")
		{
			if (fracBar->selected || (broken && divSign->selected))
				return "<mfrac>"+numeratorStr+denominatorStr+"</mfrac>";
			else
				return UNKNOWNSELECTION; // both numerator and denominator have selection but not the fraction bar is not allowed
		}

		if (fracBar->selected || (broken && divSign->selected)) // the fraction bar is selected either alone or with only either the numerator or the denominator
			return UNKNOWNSELECTION;
		else
			return numeratorStr + denominatorStr;
	}
	//---------------------------------------------------
	void CSymbolFrac::generateVariables(std::vector<std::wstring> &_vars) 
	{
		numerator->generateVariables(_vars);
		denominator->generateVariables(_vars);
	}
    //------------------------------------------------------------------------------
    void CSymbolFrac::generateVariables(std::vector<CSymbol *> & _vars)
    {
        numerator->generateVariables(_vars);
        denominator->generateVariables(_vars);
    }
	//---------------------------------------------------------
	float CSymbolFrac::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int targetLine=_targetLine;
		int lastLeft = _lastLeft;
		int firstXShift=0;
		// in the case the fraction gets together and it was broken as num/dom
		// when we enter the function of line break of the numerator and denominator
		// it affects the heights (up and down) of the current line.
		// and the only way to remove this effect in case the fraction gets
		// together is to save the values of the heights before any change
		float beforeFracUp = _lines->last->maxUp;
		float beforeFracDown = _lines->last->maxDown;
	
		// if the fraction is not broken align numerator and denominator left
		if (!broken)
		{
			numerator->shift(mybox->left - numerator->getLeft(),0);
			denominator->shift(mybox->left - denominator->getLeft(),0);
		}
	
		// before doing anything check if the fraction can get together again
		// ignore the spaces around the fraction
		if (getWidth() <= _scrRight - _scrLeft - _indent)
		{
			// check if the fraction will fit in this or next line
			if (lastLeft + getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
				_targetLine = targetLine;
			}
			one->setTargetLineNo(-1);
		}
		// else check if only the denominator can fit in one line
		else if (denominator->getWidth() <= _scrRight - _scrLeft - _indent)
		{
			// check if it will fit in this line or the next
			if (lastLeft + denominator->getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
			}
	
			if (denominator->broken)
				denominator->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
			else
			{
				denominator->shift(lastLeft-denominator->getLeft(),0);
				lastLeft += denominator->getWidth();
			}
			denominator->setTargetLineNo(targetLine);
			// the frac bar now has to be the same line as the denominator and the same width
			fracBar->setTargetLineNo(targetLine);
			fracBar->stretchBoxH(denominator->getLeft(),denominator->getRight());
			one->setTargetLineNo(targetLine);
			one->shift((denominator->getLeft()-one->getLeft())+(denominator->getWidth()-one->getWidth())/2,0);
			
			_lines->updateLastLineMaxUp_Down(one->up + one->down,down);
		}
		else
			one->setTargetLineNo(-1);
	
		//----- START WORKING ON NUMERATOR
		// will need to break numerator
		// we need to know the max up and down that comes from the numerator
		// not any part before it starts that happens to be on the same line
		int startingLine = targetLine;
		float startingLineUp = _lines->last->maxUp;
		float startingLineDown = _lines->last->maxDown;
		_lines->last->maxUp = 0;
		_lines->last->maxDown = 0;
	
		numeratorOpenFence->shift(lastLeft - numeratorOpenFence->getLeft(),0);
		numeratorOpenFence->targetLineNo = targetLine;
		lastLeft+=numeratorOpenFence->getWidth();
	
		if (numerator->getWidth() > _scrRight - _scrLeft - _indent || numerator->broken)
		{
			firstXShift = numerator->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
			numerator->targetLineNo = targetLine;
		}
		else
		{
			if (numerator->getWidth() + lastLeft > _scrRight)
			{
				targetLine++;
				startingLine = targetLine;
				_lines->updateLastLineMaxUp_Down(startingLineUp,startingLineDown);
	
				startingLineUp = 0;
				startingLineDown = 0;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
			}
			firstXShift = lastLeft - numerator->getLeft();
			numerator->shift(lastLeft - numerator->getLeft(),0);
			lastLeft = numerator->getRight();
			numerator->setTargetLineNo(targetLine);
			// adjust the max up and down of the line
			_lines->updateLastLineMaxUp_Down(numerator->up,numerator->down);
		}
		// the numerator split on more than one line. Adjust their heights
		numeratorCloseFence->targetLineNo = targetLine;
		divSign->targetLineNo = targetLine;
	
	//	if (startingLine != targetLine) 
		if (numeratorOpenFence->targetLineNo != targetLine)
			_lines->adjustLinesUpDown(numeratorOpenFence->targetLineNo, targetLine);
	//		_lines->adjustLinesUpDown(startingLine, targetLine);
	
	    // adjust the top and bottom based on the change in the up and down
		numeratorOpenFence->adjustBoxes(_lines->last->maxUp, _lines->last->maxDown);
		numeratorCloseFence->adjustBoxes(_lines->last->maxUp, _lines->last->maxDown);
		divSign->adjustBoxes(_lines->last->maxUp,_lines->last->maxDown);
	
		// if the part before breaking the fraction is bigger than 
		// the part of the fraction broken on the same line
		Line *line = _lines->getLine(startingLine);
		if (startingLineUp > line->maxUp)
			line->maxUp = startingLineUp;
		if (startingLineDown > line->maxDown)
			line->maxDown = startingLineDown;
	
		lastLeft+= THINMATHSPACE;
		numeratorCloseFence->shift(lastLeft - numeratorCloseFence->getLeft(),0);
		divSign->shift(lastLeft + numeratorCloseFence->getWidth() - divSign->getLeft(),0);
	
		lastLeft += numeratorCloseFence->getWidth() + divSign->getWidth();
	
	
		// ------------ START WORK ON denominator
		if (one->targetLineNo == -1)  // will not be in the form 1/dominator
		{
			// the fraction will be together. the denominator will start same as numerator
			if (getWidth() <= _scrRight - _scrLeft - _indent)
			{
				//numerator->shift(-numeratorOpenFence->getWidth(),0);
				lastLeft = numerator->getLeft();
			}
			else 
			{
				if (lastLeft + denominatorOpenFence->getWidth() > _scrRight)
				{
					// move to a new line
					targetLine++;
					lastLeft = _scrLeft + _indent;
					// add the new line to the line list - construct a list of the new lines in the document
					Line * l = new Line(targetLine,0,0,NULL);
					_lines->addLine(l);
				}
	
				denominatorOpenFence->shift(lastLeft-denominatorOpenFence->getLeft(),0);
				denominatorOpenFence->targetLineNo = targetLine;
				lastLeft += denominatorOpenFence->getWidth();
			}
	
			startingLine = targetLine;
			startingLineUp = _lines->last->maxUp;
			startingLineDown = _lines->last->maxDown;
			_lines->last->maxUp = 0;
			_lines->last->maxDown = 0;
	
			if (denominator->getWidth() > _scrRight - _scrLeft - _indent || denominator->broken)
			{
				denominator->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
				denominator->targetLineNo = targetLine;
			}
			else
			{
				if (denominator->getWidth() + lastLeft > _scrRight)
				{
					targetLine++;
					startingLine = targetLine;
					_lines->updateLastLineMaxUp_Down(startingLineUp,startingLineDown);
					startingLineUp = 0;
					startingLineDown = 0;
					lastLeft = _scrLeft + _indent;
					denominatorOpenFence->shift(lastLeft - denominatorOpenFence->getLeft(),0);
					lastLeft += denominatorOpenFence->getWidth();
					// add the new line to the line list - construct a list of the new lines in the document
					Line * l = new Line(targetLine,0,0,NULL);
					_lines->addLine(l);
				}
				denominator->shift(lastLeft - denominator->getLeft(),0);
				denominator->setTargetLineNo(targetLine);
				lastLeft = denominator->getRight();
				// adjust the max up and down of the line
				_lines->updateLastLineMaxUp_Down(denominator->up,denominator->down);
			}
			// the denominator split on more than one line adjust their heights
			denominatorOpenFence->targetLineNo = startingLine;	
			denominatorCloseFence->targetLineNo = targetLine;	
	
			if (startingLine != targetLine) 
				_lines->adjustLinesUpDown(startingLine , targetLine);
	
			denominatorOpenFence->adjustBoxes(_lines->last->maxUp, _lines->last->maxDown);
			denominatorCloseFence->adjustBoxes(_lines->last->maxUp, _lines->last->maxDown);
		
			line = _lines->getLine(startingLine);
			if (startingLineUp > line->maxUp)
				line->maxUp = startingLineUp;
			if (startingLineDown > line->maxDown)
				line->maxDown = startingLineDown;
	
			lastLeft += THINMATHSPACE;
			denominatorCloseFence->shift(lastLeft - denominatorCloseFence->getLeft(),0);
			lastLeft += denominatorCloseFence->getWidth();
	
			// adjust the size of the DIV sign
			if (denominatorOpenFence->up > numeratorCloseFence->up)
			{
				divSign->adjustBoxes(denominatorOpenFence->up, divSign->down);
				// the denominator does not start in the same line as the numerator  
				if ((_lines->getLine(numeratorCloseFence->targetLineNo))->maxUp < (_lines->getLine(denominatorOpenFence->targetLineNo))->maxUp)
					(_lines->getLine(numeratorCloseFence->targetLineNo))->maxUp = (_lines->getLine(denominatorOpenFence->targetLineNo))->maxUp;
			}
			if (denominatorOpenFence->down > numeratorCloseFence->down)
			{
				divSign->adjustBoxes(divSign->up,denominatorOpenFence->down);
				// the denominator does not start in the same line as the numerator  
				if ((_lines->getLine(numeratorCloseFence->targetLineNo))->maxDown < (_lines->getLine(denominatorOpenFence->targetLineNo))->maxDown)
					(_lines->getLine(numeratorCloseFence->targetLineNo))->maxDown = (_lines->getLine(denominatorOpenFence->targetLineNo))->maxDown;
			}
		}
		mybox->shift(firstXShift,0);
		
		if (targetLine == _targetLine) // the fraction is back as a fraction
		{
			setTargetLineNo(targetLine);
			// we need to return it to the fraction form
			// reverse the action we made at breaking
			numerator->shift((mybox->getWidth() - numerator->getWidth())/2 ,-numerator->down * SYMBOLHI);
			denominator->shift((mybox->getWidth() - denominator->getWidth())/2+(mybox->left - denominator->getLeft()),0);
			if (one->crtLineNo == -1) // was a broken as numerator/denominator
				denominator->shift(0,denominator->up*SYMBOLHI);
			mybox->shift(0,-numerator->down*SYMBOLHI); // box follow numerator
			if (beforeFracUp < up)
				_lines->last->maxUp = up;
			else
				_lines->last->maxUp = beforeFracUp;
	
			if (beforeFracDown < down)
				_lines->last->maxDown = down;
			else
				_lines->last->maxDown = beforeFracDown;
	
			if (broken)
				shift (-numeratorOpenFence->getWidth(),0);
			lastLeft = mybox->right;
			broken = false;
			// take care of the fracBar again
			fracBar->stretchBoxH(mybox->left,mybox->right);
		}
		else
		{
			if (!broken)  // was not broken but now is
			{
				numerator->shift(0,numerator->down * SYMBOLHI);
				if (one->targetLineNo == -1) // will be numerator/denominator
					denominator->shift(0,-denominator->up*SYMBOLHI);
				mybox->shift(0,numerator->down*SYMBOLHI); // box follow numerator
			}
			else 
			{
				// was 1/denominator * numerator and now numerator/denominator
				if (one->crtLineNo != -1 && one->targetLineNo == -1) 
					denominator->shift(0,-denominator->up*SYMBOLHI);
	
				// was numerator/denominator and now 1/denominator * numerator 
				if (one->crtLineNo == -1 && one->targetLineNo != -1)
					denominator->shift(0,denominator->up*SYMBOLHI);
			}
			broken = true;
		}
		// shift the current box to the same relative position to the first child
		_lastLeft = lastLeft;
		_targetLine = targetLine;
		return firstXShift;
	}
	//----------------------------------------------------
	float CSymbolFrac::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = numerator->adjustLines(_oldLines,_lines);
		float y = denominator->adjustLines(_oldLines,_lines);
		y = fracBar->adjustLines(_oldLines,_lines);

		mybox->shift(0,YShift * SYMBOLHI);
		crtLineNo = targetLineNo;
	
		numeratorOpenFence->adjustLines(_oldLines,_lines);
		numeratorCloseFence->adjustLines(_oldLines,_lines);
		denominatorOpenFence->adjustLines(_oldLines,_lines);
		denominatorCloseFence->adjustLines(_oldLines,_lines);
		divSign->adjustLines(_oldLines,_lines);
		one->adjustLines(_oldLines,_lines);
	
		return YShift;
	}
	//------------------------------------------
	void CSymbolFrac::setTargetLineNo(int _lineNo)
	{
		numerator->setTargetLineNo(_lineNo);
		denominator->setTargetLineNo(_lineNo);
		fracBar->setTargetLineNo(_lineNo);
		numeratorOpenFence->setTargetLineNo(_lineNo);
		numeratorCloseFence->setTargetLineNo(_lineNo);
		denominatorOpenFence->setTargetLineNo(_lineNo);
		denominatorCloseFence->setTargetLineNo(_lineNo);
		divSign->setTargetLineNo(_lineNo);
		one->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//--------------------------------------------------------------------
	void CSymbolFrac::generateFunctionsNames(std::vector<std::wstring> &_fnNames)
	{
		numerator->generateFunctionsNames(_fnNames);
		denominator->generateFunctionsNames(_fnNames);

	}
	//----------------------------------------------------------------
	bool CSymbolFrac::hasEllipses() 
	{
		if (numerator->hasEllipses())
			return true;
		if (denominator->hasEllipses())
			return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolRow::CSymbolRow(void)
	{
		noOfChildren = 0;
		crtLineNo = 0;
		targetLineNo = 0;
		broken = false;
		selected = false;
		mybox = NULL;
		exprTree = NULL;
	}
	//--------------------------------------------
	CSymbolRow::CSymbolRow(int _noOfChildren, scg::ExpressionTree * _exprTree)
	{
		noOfChildren = _noOfChildren;
		children = (CSymbol **) malloc(sizeof(CSymbol *) * noOfChildren);
		exprTree = _exprTree;
	}
	//--------------------------------------------
	CSymbolRow::~CSymbolRow(void)
	{
		for(int i =0; i< noOfChildren; i++)
			delete children[i];
		free(children);
	}
	//--------------------------------------------
	void CSymbolRow::addChild(int _index, CSymbol* _symb)
	{
		children[_index] = _symb;
		if (noOfChildren == 1 && _symb->type == OPERATOR)
		{
			type = _symb->type;
			stretch = _symb->stretch;
			opForm = _symb->opForm;
			lspace = _symb->lspace;
			rspace = _symb->rspace;
			largeOp = _symb->largeOp;
		}
		
		_symb->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolBox* CSymbolRow::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x, *b1, *b2;
		x = new CSymbolBox;
		int heighestTop = _center;
		int lowestBottom = _center;
	
		x->left = _left;
		x->right = _left;
		
		b1 = x;
		for (int i=0; i < noOfChildren; i++)
		{
			b2 =  children[i]->createBox(b1->right, _center, _device);
			b1 = b2;
			if (b1->top < heighestTop)
				heighestTop = b1->top;
	
			if (b1->bottom > lowestBottom)
				lowestBottom = b1->bottom;
		}
	
		x->top = heighestTop;
		x->right = b2->right;
		x->bottom = lowestBottom;
		mybox = x;
	
		return x;
	}
	//--------------------------------------------
	void CSymbolRow::display(CDevice * _device, bool _showBoxes)
	{
		for (int i=0; i<noOfChildren; i++)
			children[i]->display(_device,_showBoxes);
	}
	//-------------------------------------------
	void CSymbolRow::calcDepth(float l)
	{
		for (int i = 0 ;i < noOfChildren ; i++)
		{
			children[i]->calcDepth(l);
		}
	
		up=children[0]->up;
		down=children[0]->down;
		// this loop is not going to be visited if there is only one child in mrow
		for (int i = 1 ;i < noOfChildren ; i++)
		{
			float x = children[i]->up;
			up = up > x ? up : x;
			x = children[i]->down;
			down = down > x ? down : x;
	
			// sometimes operators need to stretch according to their operands
			// the operator and the operand appear in the same row
			// example : integral. We need to stretch the operator to cover the
			// whole operand (the operand is the box following the operator.
			// the operator needs to be of type PREFIX to stretch with the next operand
			if (children[i-1]->type == OPERATOR && children[i-1]->stretch && children[i-1]->opForm == PREFIX)
			{
				if (children[i]->up > 0.5)
				{
					children[i-1]->up+= children[i]->up - 0.5;
					up = children[i-1]->up > up? children[i-1]->up: up;
				}
				if (children[i]->down > 0.5)
				{
					children[i-1]->down+= children[i]->down - 0.5;
					down = children[i-1]->down > down? children[i-1]->down: down;
				}
			}
			// The following is added for the case when brackets are represented like <mo> instead
			// of mfenced. In this case the bracket might be the last child of mrow but it needs to 
			// stretch as well. (Mathematica output brackets as <mo>)
			// in this case stretch it to follow the child before it rather than after it
			// The operator needs to be a POSTFIX to follow the operand before it
			if (children[i]->type == OPERATOR && children[i]->stretch && children[i]->opForm == POSTFIX)
			{
				if (children[i-1]->up > 0.5)
				{
					children[i]->up += children[i-1]->up - 0.5;
					up = children[i]->up > up? children[i]->up: up;
				}
				if (children[i-1]->down > 0.5)
				{
					children[i]->down += children[i-1]->down - 0.5;
					down = children[i]->down > down? children[i]->down: down;
				}
			}
		}
	}
	//-----------------------------------------------
	CSymbol * CSymbolRow::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			for (int i =0; i < noOfChildren; i++)
			{
				CSymbol * crtSymbol = children[i]->getCrtSymbol(_x,_y);
				if (crtSymbol != NULL)
					return crtSymbol;
			}
			return this;
		}
		else
			return NULL;
	}
	//-------------------------------------------
	CSymbol * CSymbolRow::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			for (int i = 0; i < noOfChildren; i++)
			{
				CSymbol * crtSymbol = children[i]->getCrtSymbol(_rect);
				if (crtSymbol != NULL)
					return crtSymbol;
			}
			return NULL;
		}
	}
	//-------------------------------------------
	bool CSymbolRow::selectSymbols(SCGRECT _rect)
	{
		bool somethingSelected = false;

		for (int i = 0; i < noOfChildren; i++)
		{
			if (children[i]->selectSymbols(_rect))
				somethingSelected = true;
		}
		return somethingSelected;
	}
	//-------------------------------------------
	void CSymbolRow::clearSelection()
	{
		for (int i = 0; i < noOfChildren; i++)
			children[i]->clearSelection();
	}
	//-------------------------------------------
	void CSymbolRow::getSelectedSymbols(std::wstring &_ws)
	{
		for (int i = 0; i < noOfChildren; i++)
			children[i]->getSelectedSymbols(_ws);
	}
	//-------------------------------------------
	void CSymbolRow::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		for (int i = 0; i < noOfChildren; i++)
			children[i]->getSelectedSymbols(_symbols,_device);
	}
	//-------------------------------------------
	std::string CSymbolRow::getSelectedMathML()
	{
		int noOfSelectedChildren = 0; // we will not add the <mrow> tags if only one child is selected

		std::string rowStr = "";
		int lastSelectedChild = 0;
		for (int i = 0; i < noOfChildren; i++)
		{
			std::string s = "";
			s = children[i]->getSelectedMathML();
			if (s != "")
			{
				if (noOfSelectedChildren == 0 && children[i]->getWidth() == 0) // an invisible operator. Do not add as a first child 
					continue;
				noOfSelectedChildren++;
				rowStr += s;
				lastSelectedChild = i;
			}
		}
		
		if (noOfSelectedChildren >= 1)
		{
			if (children[lastSelectedChild]->getWidth() == 0) // do not let the last thing an invisible operator
			{
				// remove it from the string
				int pos = rowStr.rfind("<mo>");
				rowStr = rowStr.substr(0, pos);
				noOfSelectedChildren--;
			}

			if (noOfSelectedChildren > 1)
				rowStr = "<mrow>" + rowStr + "</mrow>";
		}
		return rowStr;
	}
	//-------------------------------------------
	void CSymbolRow::shift(int _x,int _y)
	{
		for (int i=0; i < noOfChildren; i++)
			children[i]->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//--------------------------------------------
	void CSymbolRow::generateVariables(std::vector<std::wstring> &_vars)
	{
		for (int i = 0; i < noOfChildren; i++)
		{
			std::vector<std::wstring> tempVars;
			children[i]->generateVariables(tempVars);
			if (tempVars.size() != 0)
			{
				if (i+1 < noOfChildren)
				{
					if (!children[i+1]->isApplyFunction())
					{
						for (int j = 0; j< tempVars.size(); j++)
							_vars.push_back(tempVars[j]);
					}
				}
				else
				{
					for (int j = 0; j< tempVars.size(); j++)
						_vars.push_back(tempVars[j]);
				}
			}
		}
	}
    //------------------------------------------------------------------------------
    void CSymbolRow::generateVariables(std::vector<CSymbol *> & _vars)
    {
        for (int i = 0; i < noOfChildren; i++)
		{
			std::vector<CSymbol *> tempVars;
			children[i]->generateVariables(tempVars);
			if (tempVars.size() != 0)
			{
				if (i+1 < noOfChildren)
				{
					if (!children[i+1]->isApplyFunction())
					{
						for (int j = 0; j< tempVars.size(); j++)
							_vars.push_back(tempVars[j]);
					}
				}
				else
				{
					for (int j = 0; j< tempVars.size(); j++)
						_vars.push_back(tempVars[j]);
				}
			}
		}
    }
	//---------------------------------------------------------
	float CSymbolRow::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int targetLine=_targetLine;
		int lastLeft = _lastLeft;
		int firstXShift=0;
		int lastBacki = 0;
		
		for (int i=0; i<noOfChildren; i++)
		{
            // child can not fit in the current line
			if (children[i]->getWidth() > _scrRight - _scrLeft - _indent || children[i]->broken)
			{		
				float x = children[i]->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
				if (!i)
					firstXShift = x;
				children[i]->targetLineNo = targetLine;
			}
			else
			{
                // move the child to the next line. Ignore the fact that the + and - operators might end up being at the end of the line for now
                if (children[i]->getWidth() + lastLeft > _scrRight)
                {
					targetLine++;
					lastLeft = _scrLeft + _indent;
					Line * l = new Line(targetLine,0,0,NULL);
					_lines->addLine(l);                    
                }
                /*
				// start a new line for
				// the +/- operator can not be the end of a line
				// or already passed the right bounds
				if (children[i]->getWidth() + lastLeft > _scrRight || (i < noOfChildren-1 && children[i]->type == OPERATOR && (children[i]->isTxt(L"+") || children[i]->isTxt(L"-")) && children[i+1]->getWidth() + children[i]->getWidth() + lastLeft > _scrRight ))
				// the rest of the condition was
				// && children[i+1]->getWidth() < (_scrRight - _scrLeft - _indent)
				{
					targetLine++;
					lastLeft = _scrLeft + _indent;
					Line * l = new Line(targetLine,0,0,NULL);
					_lines->addLine(l);
				}
	
				if (!i)
					firstXShift = lastLeft - children[i]->getLeft();
		
				children[i]->shift(lastLeft - children[i]->getLeft(),0);
				lastLeft = children[i]->getRight();
				// change the line number of all the children of that child
				children[i]->setTargetLineNo(targetLine);
	
				// adjust the max up and down of the line
				_lines->updateLastLineMaxUp_Down(children[i]->up, children[i]->down);
                 */
                // put the child in the current place
				if (!i)
					firstXShift = lastLeft - children[i]->getLeft();

                children[i]->shift(lastLeft - children[i]->getLeft(),0);
				lastLeft = children[i]->getRight();
				children[i]->setTargetLineNo(targetLine);
				// adjust the max up and down of the line
				_lines->updateLastLineMaxUp_Down(children[i]->up, children[i]->down);
			}	
		}	
		_lastLeft = lastLeft;
		if (targetLine == _targetLine)
		{
			setTargetLineNo(targetLine);
			broken = false;
		}
		else
			broken = true;
	
		// shift the current box to the same relative position to the first child
		mybox->shift(firstXShift,0);
	
		_targetLine = targetLine;
		return firstXShift;
	}
	//----------------------------------------------------
	float CSymbolRow::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = 0;
		for (int i=0; i<noOfChildren; i++)
		{
			float y = children[i]->adjustLines(_oldLines,_lines);
			if (!i)
			{
				YShift = y;
				mybox->shift(0,YShift*SYMBOLHI);
			}
		}
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//---------------------------------------------------
	void CSymbolRow::setTargetLineNo(int _lineNo)
	{
		for (int i=0; i <noOfChildren; i++)
			children[i]->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//----------------------------------------------------
	// when we need the row in a short form
	void CSymbolRow::reform (int _shortTerms)
	{
		if (noOfChildren <= _shortTerms)
			return;
	
		if (_shortTerms < 2)
			return;

		int sTerms = _shortTerms - 2;
		int eTerms = 2;

		if (_shortTerms == 2)
		{
			sTerms = 1;
			eTerms = 1;
		}
	
		CSymbol ** tempChildren;
		tempChildren = (CSymbol **) malloc(sizeof(CSymbol *) * (_shortTerms + 1));
		
		int i;
		for (i=0; i < sTerms; i++)
			tempChildren[i] = children[i];
	
		tempChildren[i] = new CSymbolBasic(L".....",IDENTIFIER,5);
	
		for (int j = noOfChildren - eTerms; j < noOfChildren; j++)
			tempChildren[++i] = children[j];
	
		// delete the terms that are not needed
		for(int k = sTerms; k < noOfChildren - eTerms; k++)
			delete children[k];
		free(children);
		
		children = tempChildren;
		noOfChildren = _shortTerms + 1;
	}
	//------------------------------------------------------------------------
	expressionType CSymbolRow::getExprType()
	{
		for (int i=0; i < noOfChildren; i++)
			if (children[i]->type == OPERATOR)
			{
				expressionType t = children[i]->getExprType();
				if (t != NULLTYPE)
					return children[i]->getExprType();
			}
	
		for (int i=0; i < noOfChildren; i++)
			if (children[i]->getExprType() != NULLTYPE)
				return children[i]->getExprType();
	
		return NULLTYPE;
	}
	//-----------------------------------------------------
	void CSymbolRow::generateFunctionsNames(std::vector<std::wstring> &_fnNames) 
	{
		children[0]->generateFunctionsNames(_fnNames);
		// apply function needs to be between two other children !!
		for (int i = 1; i < noOfChildren - 1; i++)
		{
			if (children[i]->isApplyFunction())
			{
				std::vector <std::wstring> varsV;
				children[i-1]->generateVariables(varsV);
				if (varsV.size() == 1)
				{
					children[i+1]->generateVariables(varsV);
					if (varsV.size() == 2)
					{
						_fnNames.push_back(varsV.at(0));
						_fnNames.push_back(varsV.at(1));
					}
				}
			}
			else
				children[i]->generateFunctionsNames(_fnNames);
		}
		children[noOfChildren - 1]->generateFunctionsNames(_fnNames);
	}
	//----------------------------------------------------------
	bool CSymbolRow::hasEllipses()
	{
		for (int i = 0; i < noOfChildren; i++)
			if (children[i]->hasEllipses())
				return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolFence::CSymbolFence(void)
	{
		noOfChildren = 0;
		type = NONTOKEN;
	}
	//-------------------------------------------
	CSymbolFence::CSymbolFence(int _noOfChildren, wchar_t* _openFence, wchar_t* _closeFence, std::wstring _separators, scg::ExpressionTree * _exprTree)
	{
		/* in the case of mfence the number of children provided to the function 
		includes the separators as well. So the function really should not worry about the
		separators */
		noOfChildren = _noOfChildren;
		children = (CSymbol **) malloc(sizeof(CSymbol *) * noOfChildren);
		openFence = new CSymbolBasic(_openFence,OPERATOR,1);
		closeFence = new CSymbolBasic(_closeFence,OPERATOR,1);
		assignSeparators(_separators);
	
		type = NONTOKEN;
		exprTree = _exprTree;
		
		openFence->parentSymbol = this;
		closeFence->parentSymbol = this;
	}
	//--------------------------------------------
	/* the following function assign the separators passed as a string to the children locations
	where they should be in the children of mfenced. If the string is an empty string nothing 
	happens. The separator is only one character long, if the number of characters in the 
	separators string is more than the needed number of separators the rest of them is ignored.
	If the number of characters is less than the needed number of separators the last one is 
	repeated to the end (these are MathML recommendations for rendering mfenced)*/
	void CSymbolFence::assignSeparators(std::wstring _separators)
	{
		if (_separators.length() == 0)
			return;

		wchar_t lastSeparator[2];
		wcscpy (lastSeparator,L",");
		lastSeparator[0] = _separators[0];

		for (int i = 0; i < noOfChildren /2 ; i++)
		{
			if (i < _separators.length())
				lastSeparator[0] = _separators[0];

			addChild(i*2+1, new CSymbolBasic(lastSeparator,OPERATOR,1));
		}
	}
	//--------------------------------------------
	void CSymbolFence::calcDepth(float l)
	{
		// we will not take the separators into account when we calculate the depth
		// because they should always be less than the fences and the children
		openFence->calcDepth(l);
		closeFence->calcDepth(l);
		for (int i = 0 ;i < noOfChildren ; i++)
			children[i]->calcDepth(l);
	
		up=children[0]->up;
		down=children[0]->down;
		for (int i = 1 ;i < noOfChildren ; i++)
		{
			float x = children[i]->up;
			up = up > x ? up : x;
			x = children[i]->down;
			down = down > x ? down : x;
	
			if (children[i-1]->type == OPERATOR && children[i-1]->stretch)
			{
				if (children[i]->up > 0.5)
					children[i-1]->up+= children[i]->up - 0.5;
				if (children[i]->down > 0.5)
					children[i-1]->down+= children[i]->down - 0.5;
			}
		}
	}
	//--------------------------------------------
	CSymbolFence::~CSymbolFence(void)
	{
		delete openFence;
		delete closeFence;
	}
	//--------------------------------------------
	CSymbolBox* CSymbolFence::createBox(int _left, int _center, CDevice * _device)
	{
		LOG("CSymbolFence::createBox %d %d", _left, _center);
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x, *b1, *b2;
		x = new CSymbolBox;
		x->left = _left;
		int heighestTop = _center;
		int lowestBottom = _center;
	
		b1 = openFence->createBox(_left,_center,_device);
	
		for (int i=0; i < noOfChildren; i++)
		{
			b2 =  children[i]->createBox(b1->right, _center,_device);
			b1 = b2;
			
			if (b1->top < heighestTop)
				heighestTop = b1->top;
	
			if (b1->bottom > lowestBottom)
				lowestBottom = b1->bottom;
		}
	
		b2 = closeFence->createBox(b1->right,_center,_device);
		
		x->top = heighestTop;
		x->right = b2->right;
		x->bottom = lowestBottom;
	
		// stretch the brackets
		LOG("CSymbolFence:createBox 2068: %d %d", x->getHeight(), SYMBOLHI);
		int ratio = x->getHeight() / SYMBOLHI; // the ratio of vertical stretch
		openFence->stretchBox(x->top,x->bottom,openFence->getLeft(),openFence->getRight()+ratio);
		closeFence->stretchBox(x->top,x->bottom,closeFence->getLeft(),closeFence->getRight()+ratio);
		for (int i=0;i<noOfChildren; i++)
			children[i]->shift(ratio,0);
		closeFence->shift(ratio,0);
	
		x->right = closeFence->getRight();
		mybox = x;
	
		// change the up and down of the fence because they stretched
		// this needs to be adjusted so they can be used in line breaking
		openFence->up = (_center - openFence->getTop())/(1.0* SYMBOLHI);
		openFence->down = (openFence->getBottom() - _center)/(1.0 * SYMBOLHI);
		closeFence->up = (_center - closeFence->getTop())/(1.0 * SYMBOLHI);
		closeFence->down = (closeFence->getBottom() - _center)/(1.0 *SYMBOLHI);

		return x;
	}
	//-------------------------------------------
	void CSymbolFence::display(CDevice * _device, bool _showBoxes)
	{
		openFence->display(_device,_showBoxes);
		closeFence->display(_device,_showBoxes);
		for (int i=0; i<noOfChildren; i++)
			children[i]->display(_device,_showBoxes);
	}
	//-------------------------------------------
	CSymbol * CSymbolFence::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			CSymbol * crtSymbol = openFence->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = closeFence->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;

			for (int i =0; i < noOfChildren; i++)
			{
				CSymbol * crtSymbol = children[i]->getCrtSymbol(_x,_y);
				if (crtSymbol != NULL)
					return crtSymbol;
			}
			return this;
		}
		else
			return NULL;
	}
	//-------------------------------------------
	CSymbol * CSymbolFence::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			CSymbol * crtSymbol = openFence->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = closeFence->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			for (int i =0; i < noOfChildren; i++)
			{
				CSymbol * crtSymbol = children[i]->getCrtSymbol(_rect);
				if (crtSymbol != NULL)
					return crtSymbol;
			}
			return NULL;
		}
	}
	//-------------------------------------------
	bool CSymbolFence::selectSymbols(SCGRECT _rect)
	{
		bool somethingSelected = false;

		somethingSelected = CSymbolRow::selectSymbols(_rect);
		if (openFence->selectSymbols(_rect))
			somethingSelected = true;

		if (closeFence->selectSymbols(_rect))
			somethingSelected = true;

		return somethingSelected;
	}
	//-------------------------------------------
	void CSymbolFence::clearSelection()
	{
		CSymbolRow::clearSelection();
		openFence->clearSelection();
		closeFence->clearSelection();
	}
	//-------------------------------------------
	void CSymbolFence::getSelectedSymbols(std::wstring & _ws)
	{
		openFence->getSelectedSymbols(_ws);
		CSymbolRow::getSelectedSymbols(_ws);
		closeFence->getSelectedSymbols(_ws);
	}
	//-------------------------------------------
	void CSymbolFence::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		openFence->getSelectedSymbols(_symbols,_device);
		CSymbolRow::getSelectedSymbols(_symbols,_device);
		closeFence->getSelectedSymbols(_symbols,_device);
	}
	//-------------------------------------------
	std::string CSymbolFence::getSelectedMathML()
	{
		std::string selectedRow = CSymbolRow::getSelectedMathML();
		if (selectedRow != "") // if the fences are selected add them as well otherwise ignore
		{
			std::string fenceStr = "";
			if (openFence->selected && closeFence->selected)
			{
				if (openFence->isTxt(L"("))
					fenceStr = "<mfenced open='(' ";
				else if (openFence->isTxt(L"{"))
					fenceStr = "<mfenced open='{' ";
				else if (openFence->isTxt(L"["))
					fenceStr = "<mfenced open='[' ";

				if (closeFence->isTxt(L")"))
					fenceStr += "close=')'>";
				if (closeFence->isTxt(L"}"))
					fenceStr += "close='}'>";
				if (closeFence->isTxt(L"]"))
					fenceStr += "close=']'>";
				
				selectedRow = fenceStr + selectedRow + "</mfenced>";
			}
		}
		return selectedRow;
	}
	//--------------------------------------------
	void CSymbolFence::shift(int _x,int _y)
	{
		openFence->shift(_x,_y);
	    closeFence->shift(_x,_y);
	
		for (int i=0; i < noOfChildren; i++)
			children[i]->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//---------------------------------------------------------
	float CSymbolFence::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int targetLine=_targetLine;
		int lastLeft = _lastLeft;
		int firstXShift=0;
		int lastBacki = 0;
	
		if (lastLeft + openFence->getWidth() > _scrRight)
		{
			// put the open fence in the next line
			targetLine++;
			lastLeft = _scrLeft + _indent;
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
		}
		firstXShift = lastLeft - openFence->getLeft();
		openFence->shift(firstXShift,0);
		openFence->setTargetLineNo(targetLine);
		lastLeft = openFence->getRight();
	
		int startingLine = targetLine;
		float startingLineUp = _lines->last->maxUp;
		float startingLineDown = _lines->last->maxDown;
		_lines->last->maxUp = 0;
		_lines->last->maxDown = 0;
	
		for (int i=0; i<noOfChildren; i++)
		{
			if (children[i]->getWidth() > _scrRight - _scrLeft - _indent || children[i]->broken)
			{		
				children[i]->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
				children[i]->targetLineNo = targetLine;
			}
			else
			{
				// the operator can not be the end of a line
				// move me to the next line only if I'm NOT a separator !!!
				if (!children[i]->separator && (children[i]->getWidth() + lastLeft > _scrRight || (i < noOfChildren-1 && children[i]->type == OPERATOR && children[i+1]->getWidth() + children[i]->getWidth() + lastLeft > _scrRight)))
				{
					// create a new line that start with the operator
					targetLine++;
					lastLeft = _scrLeft + _indent;
					Line * l = new Line(targetLine,0,0,NULL);
					_lines->addLine(l);
				}
	
				children[i]->shift(lastLeft - children[i]->getLeft(),0);
				lastLeft = children[i]->getRight();
				// change the line number of all the children of that child
				children[i]->setTargetLineNo(targetLine);
	
				// adjust the max up and down of the line
				_lines->updateLastLineMaxUp_Down(children[i]->up, children[i]->down);
			}	
		}
	
		if (lastLeft + closeFence->getWidth() > _scrRight)
		{
			// put the open fence in the next line
			targetLine++;
			lastLeft = _scrLeft + _indent;
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
		}
		closeFence->shift(lastLeft-closeFence->getLeft(),0);
		closeFence->setTargetLineNo(targetLine);
		lastLeft += closeFence->getWidth();
	
		// adjust the line heights
		if (startingLine != targetLine) 
			_lines->adjustLinesUpDown(startingLine, targetLine);
	
	    // adjust the top and bottom based on the change in the up and down
		openFence->adjustBoxes(_lines->last->maxUp, _lines->last->maxDown);
		closeFence->adjustBoxes(_lines->last->maxUp, _lines->last->maxDown);
	
		// if the part before breaking the fraction is bigger than 
		// the part of the fraction broken on the same line
		Line *line = _lines->getLine(startingLine);
		if (startingLineUp > line->maxUp)
			line->maxUp = startingLineUp;
		if (startingLineDown > line->maxDown)
			line->maxDown = startingLineDown;
	
		_lastLeft = lastLeft;
		if (targetLine == _targetLine)
		{
			setTargetLineNo(targetLine);
			broken = false;
		}
		else
			broken = true;
	
		// shift the current box to the same relative position to the first child
		mybox->shift(firstXShift,0);
	
		_targetLine = targetLine;
		return firstXShift;
	}
	//----------------------------------------------------
	float CSymbolFence::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = openFence->adjustLines(_oldLines, _lines);
		for (int i=0; i < noOfChildren; i++)
			children[i]->adjustLines(_oldLines,_lines);
		closeFence->adjustLines(_oldLines,_lines);
		
		mybox->shift(0,YShift*SYMBOLHI);
		crtLineNo = targetLineNo;

		return YShift;
	}
	//------------------------------------------
	void CSymbolFence::setTargetLineNo(int _lineNo)
	{
		openFence->setTargetLineNo(_lineNo);
		for (int i = 0; i< noOfChildren; i++)
			children[i]->setTargetLineNo(_lineNo);
		closeFence->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//--------------------------------------------------------------------------
	expressionType CSymbolFence::getExprType()
	{
		if (openFence->getExprType() == CURLYBRACE)
			return SET;
		else if (openFence ->getExprType() == SQUAREBRACKET)
        {
			if (children[0]->getExprType() == MULTIEXPR)
            {
                if (((CSymbolTable *)children[0])->isSquareMatrix())
                    return SQUAREMATRIX;
                else
                    return MATRIX;
            }
		}
        else if (noOfChildren == 1)
			return children[0]->getExprType();
	}
	//=========================================================================
	//=========================================================================
	
	CSymbolSup::CSymbolSup(CSymbol* _base, CSymbol* _exp, scg::ExpressionTree * _exprTree)
	{
		base = _base;
		exp = _exp;
		type = NONTOKEN;
		if (base->type == OPERATOR)
		{
			type = base->type;
			opForm = base->opForm;
			stretch = base->stretch;
			lspace = base->lspace;
			rspace = base->rspace;
			largeOp = base->largeOp;
		}
		hat = new CSymbolBasic(L"^",OPERATOR,1);
		exprTree = _exprTree;
		
		base->parentSymbol = this;
		exp->parentSymbol = this;
		hat->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolSup::~CSymbolSup(void)
	{
		delete(base);
		delete(exp);
		delete(hat);
	}
	//------------------------------------------------
	void CSymbolSup::calcDepth(float l)
	{
	
		//if(l>=0.7)
		//	exp->calcDepth(l-2.0/SYMBOLHI);
		//else
			exp->calcDepth(l);
		
		base->calcDepth(l);
		hat->calcDepth(l);
	
		up = base->up + exp->up+exp->down- 0.5*l;
		down = base->down;
	}
	//---------------------------------------------
	CSymbolBox* CSymbolSup::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x,*b,*e;
		x = new CSymbolBox;
	
		x->left = _left;
		
		b = base->createBox(_left ,_center,_device);
		e = exp->createBox(b->right,_center -(base->up + exp->down-.5) *SYMBOLHI ,_device);
		hat->createBox(base->getRight(),_center - (base->up - 0.5) *SYMBOLHI,_device);
	
		x->right=e->right;
		x->top = exp->getTop();
		x->bottom = base->getBottom();
		mybox = x;
	
		if (type == OPERATOR && stretch)
			stretchBoxV(_center - up*SYMBOLHI , _center + down * SYMBOLHI);
	
		return mybox;
	}
	//----------------------------------------------
	void CSymbolSup::display(CDevice * _device, bool _showBoxes)
	{
	    base->display(_device,_showBoxes);
		//CFont *defFont = (CFont *)_pDC->SelectStockObject(ANSI_VAR_FONT);
		//LOGFONT lf;
		//defFont->GetLogFont(&lf);
		//if(lf.lfHeight>=0.7*SYMBOLHI)
		//	lf.lfHeight-=2;
		//CFont expFont;
		//VERIFY(expFont.CreateFontIndirect(&lf));
		//_pDC->SelectObject(expFont);	
	
		exp->display(_device, _showBoxes);
	    
		//_pDC->SelectObject(defFont);	
		//expFont.DeleteObject();
		if (broken)
			hat->display(_device,_showBoxes);
	
		//if (_showBoxes)
		//	_pDC->FrameRect(mybox->getRectangle(),&CBrush(RGB(0,0,0)));	
	}
	//------------------------------------------------
	CSymbol * CSymbolSup::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			CSymbol * crtSymbol = base->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;
			
			crtSymbol = exp->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;

			return this;
		}
		else
			return NULL;
	}
	//------------------------------------------------
	CSymbol * CSymbolSup::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			CSymbol * crtSymbol = base->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = exp->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			return NULL;
		}
	}
	//------------------------------------------------
	bool CSymbolSup::selectSymbols(SCGRECT _rect)
	{
		bool a = base->selectSymbols(_rect);
		bool b = exp->selectSymbols(_rect);
		bool c = false;

		if (broken)
			if (hat->selectSymbols(_rect))
				c = true;

		return a || b || c;
	}
	//------------------------------------------------
	void CSymbolSup::clearSelection()
	{
		base->clearSelection();
		exp->clearSelection();
		hat->clearSelection();
	}
	//------------------------------------------------
	void CSymbolSup::getSelectedSymbols(std::wstring &_ws)
	{
		base->getSelectedSymbols(_ws);
		exp->getSelectedSymbols(_ws);

		if (broken)
			hat->getSelectedSymbols(_ws);
	}
	//------------------------------------------------
	void CSymbolSup::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		base->getSelectedSymbols(_symbols,_device);
		exp->getSelectedSymbols(_symbols,_device);

		if (broken)
			hat->getSelectedSymbols(_symbols,_device);
	}
	//------------------------------------------------
	std::string CSymbolSup::getSelectedMathML()
	{
		std::string s1 = base->getSelectedMathML();
		std::string s2 = exp->getSelectedMathML();

		if (s1 != "" && s2 != "")
			return "<msup>" + s1 + s2 + "</msup>";
		else
			return s1 + s2;
	}
	//------------------------------------------------
	void CSymbolSup::shift(int _x,int _y)
	{
		base->shift(_x,_y);
		exp->shift(_x,_y);
		hat->shift(_x,_y);
	
		mybox->shift(_x,_y);
	}
	//------------------------------------------------
	void CSymbolSup::stretchBoxV(int _top, int _bottom)
	{
		exp->shift(0,_top-exp->getTop());
		base->stretchBoxV(exp->getBottom() - 0.5 * SYMBOLHI, _bottom);
		mybox->stretchV(_top,_bottom);
	}
	//------------------------------------------------
	void CSymbolSup::generateVariables(std::vector<std::wstring> &_vars)
	{
		exp->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//------------------------------------------------
	void CSymbolSup::generateVariables(std::vector<CSymbol *> &_vars)
	{
		exp->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//---------------------------------------------------------
	float CSymbolSup::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int targetLine=_targetLine;
		int lastLeft = _lastLeft;
		int firstXShift=0;
	
		float beforeSupUp = _lines->last->maxUp;
		float beforeSupDown = _lines->last->maxDown;
	
		// before doing anything check if the Sup can get together again
		if (getWidth() < _scrRight - _scrLeft - _indent)
		{
			// check if the Sup will fit in this or next line
			if (lastLeft + getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
				_targetLine = targetLine;
			}
		}
		
		if (base->getWidth() > _scrRight - _scrLeft - _indent || base->broken) 
		{	
			firstXShift = base->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
			base->targetLineNo = targetLine;
		}
		else
		{
			if (lastLeft + base->getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
			}
			firstXShift = lastLeft- base->getLeft();
			base->shift(firstXShift,0);
			base->setTargetLineNo(targetLine);
			lastLeft = base->getRight();
			_lines->updateLastLineMaxUp_Down(base->up,base->down);
		}
	
		hat->shift(lastLeft-hat->getLeft(),0);
		hat->setTargetLineNo(targetLine);
		lastLeft = hat->getRight();
	
		if (exp->getWidth() > _scrRight - _scrLeft - _indent || exp->broken)
		{
			exp->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
			exp->targetLineNo = targetLine;
		}
		else
		{
			if (lastLeft - hat->getWidth() + exp->getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
			}
			exp->shift(lastLeft - exp->getLeft(),0);
			exp->setTargetLineNo(targetLine);
			lastLeft = exp->getRight();
			_lines->updateLastLineMaxUp_Down(exp->up,exp->down);
		}
		mybox->shift(firstXShift,0);
		
		if (targetLine == _targetLine) // the Sup is back together
		{
			setTargetLineNo(targetLine);
			// we need to return it to the fraction form
			// reverse the action we made at breaking
			exp->shift(base->getRight() - exp->getLeft(),0);
			if (broken)
				exp->shift(0,-round((base->up + exp->down-.5) *SYMBOLHI)); 
	
			if (beforeSupUp < up)
				_lines->last->maxUp = up;
			else
				_lines->last->maxUp = beforeSupUp;
	
			if (beforeSupDown < down)
				_lines->last->maxDown = down;
			else
				_lines->last->maxDown = beforeSupDown;
	
			lastLeft = getRight();
			broken = false;
		}
		else
		{
			if (!broken)  // was not broken but now is
				exp->shift(0,round((base->up + exp->down-.5) *SYMBOLHI)); 
			broken = true;
		}
		// shift the current box to the same relative position to the first child
		_lastLeft = lastLeft;
		_targetLine = targetLine;
		return firstXShift;
	}
	//----------------------------------------------------------
	float CSymbolSup::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = base->adjustLines(_oldLines,_lines);
		exp->adjustLines(_oldLines,_lines);
		hat->adjustLines(_oldLines,_lines);
		mybox->shift(0,YShift * SYMBOLHI);
	
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------
	void CSymbolSup::setTargetLineNo(int _lineNo)
	{
		base->setTargetLineNo(_lineNo);
		exp->setTargetLineNo(_lineNo);
		hat->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//-------------------------------------------
	bool CSymbolSup::hasEllipses() 
	{
		if (base->hasEllipses())
			return true;
		if (exp->hasEllipses())
			return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolSub::CSymbolSub(CSymbol *_base, CSymbol *_sub, scg::ExpressionTree * _exprTree)
	{
		base = _base;
		sub = _sub;
		type = NONTOKEN;
		if (base->type == OPERATOR)
		{
			type = base->type;
			stretch = base->stretch;
			opForm = base->opForm;
			lspace = base->lspace;
			rspace = base->rspace;
			largeOp = base->largeOp;
		}
		exprTree = _exprTree;
		
		base->parentSymbol = this;
		sub->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolSub::~CSymbolSub(void)
	{
		delete(base);
		delete(sub);
	}
	//------------------------------------------------
	void CSymbolSub::calcDepth(float l)
	{
		//if(l>=0.7)
		//	sub->calcDepth(l-2.0/SYMBOLHI);
		//else
			sub->calcDepth(l);
		
		base->calcDepth(l);
	
		up = base->up;
		down = base->down + sub->up + sub->down - 0.5*l;
	}
	//---------------------------------------------
	CSymbolBox* CSymbolSub::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x,*b,*e;
		x = new CSymbolBox;
	
		x->left = _left;
		
		b = base->createBox(_left ,_center,_device);
		e = sub->createBox(b->right,_center +(base->down + sub->up-.5) *SYMBOLHI ,_device);
		
		x->right=e->right;
		x->top = base->getTop();
		x->bottom = sub->getBottom();
		mybox = x;
	
		if (type == OPERATOR && stretch)
			stretchBoxV(_center - up*SYMBOLHI , _center + down * SYMBOLHI);
	
		return mybox;
	}
	//----------------------------------------------
	void CSymbolSub::display(CDevice * _device, bool _showBoxes)
	{
	    base->display(_device,_showBoxes);
		//CFont *defFont = (CFont *)_pDC->SelectStockObject(ANSI_VAR_FONT);
		//LOGFONT lf;
		//defFont->GetLogFont(&lf);
		//if(lf.lfHeight>=0.7*SYMBOLHI)
		//	lf.lfHeight-=2;
		//CFont subFont;
		//VERIFY(subFont.CreateFontIndirect(&lf));
		//_pDC->SelectObject(subFont);	
	
		sub->display(_device, _showBoxes);
	    
		//_pDC->SelectObject(defFont);	
		//subFont.DeleteObject();
	}
	//------------------------------------------------
	CSymbol * CSymbolSub::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			CSymbol * crtSymbol = base->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;
			
			crtSymbol = sub->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;

			return this;
		}
		else
			return NULL;
	}
	//------------------------------------------------
	CSymbol * CSymbolSub::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			CSymbol * crtSymbol = base->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = sub->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			return NULL;
		}
	}
	//------------------------------------------------
	bool CSymbolSub::selectSymbols(SCGRECT _rect)
	{
		bool a = base->selectSymbols(_rect);
		bool b = sub->selectSymbols(_rect);

		return a|| b;
	}
	//------------------------------------------------
	void CSymbolSub::clearSelection()
	{
		base->clearSelection();
		sub->clearSelection();
	}
	//------------------------------------------------
	void CSymbolSub::getSelectedSymbols(std::wstring &_ws)
	{
		base->getSelectedSymbols(_ws);
		sub->getSelectedSymbols(_ws);
	}
	//------------------------------------------------
	void CSymbolSub::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		base->getSelectedSymbols(_symbols,_device);
		sub->getSelectedSymbols(_symbols,_device);
	}
	//------------------------------------------------
	std::string CSymbolSub::getSelectedMathML()
	{
		std::string s1 = base->getSelectedMathML();
		std::string s2 = sub->getSelectedMathML();

		if (s1 != "" && s2 != "")
			return "<msub>" + s1 + s2 + "</msub>";
		else
			return s1 + s2;
	}
	//------------------------------------------------
	void CSymbolSub::shift(int _x,int _y)
	{
		base->shift(_x,_y);
		sub->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//------------------------------------------------
	void CSymbolSub::stretchBoxV(int _top, int _bottom)
	{
		sub->shift(0,_bottom-sub->getBottom());
		base->stretchBoxV(_top, sub->getTop()+ 0.5 * SYMBOLHI);
	
		mybox->stretchV(_top, _bottom);
	}
	//------------------------------------------------
	void CSymbolSub::generateVariables(std::vector<std::wstring> &_vars)
	{
		sub->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//------------------------------------------------
	void CSymbolSub::generateVariables(std::vector<CSymbol *> &_vars)
	{
		sub->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//---------------------------------------------------------
	float CSymbolSub::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int targetLine=_targetLine;
		int lastLeft = _lastLeft;
		int firstXShift=0;
	
		float beforeSubUp = _lines->last->maxUp;
		float beforeSubDown = _lines->last->maxDown;
	
		// before doing anything check if the Sub can get together again
		if (getWidth() < _scrRight - _scrLeft - _indent)
		{
			// check if the Sup will fit in this or next line
			if (lastLeft + getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
				_targetLine = targetLine;
			}
		}
		
		if (base->getWidth() > _scrRight - _scrLeft - _indent || base->broken) 
		{	
			firstXShift = base->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
			base->targetLineNo = targetLine;
		}
		else
		{
			if (lastLeft + base->getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
			}
			firstXShift = lastLeft- base->getLeft();
			base->shift(firstXShift,0);
			base->setTargetLineNo(targetLine);
			lastLeft = base->getRight();
			_lines->updateLastLineMaxUp_Down(base->up,base->down);
		}
	
		if (sub->getWidth() > _scrRight - _scrLeft - _indent || sub->broken)
		{
			int startingLeft = lastLeft;
			int startingLine = targetLine;
	
			sub->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
			sub->targetLineNo = targetLine;
			
			if (sub->getLeft() != startingLeft) // the sub starts at the same line of the base
				startingLine++;
			
			Line *l = _lines->getLine(startingLine);
			while (l != _lines->last)
			{
				l->maxDown += base->down + sub->up - 0.5;
				l = l->next;
			}
			_lines->last->maxDown+= base->down + sub->up - 0.5;
		}
		else
		{
			if (lastLeft + sub->getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
			}
			sub->shift(lastLeft - sub->getLeft(),0);
			sub->setTargetLineNo(targetLine);
			lastLeft = sub->getRight();
			// note that the line of the subscript keeps the up and down of
			// the whole sub expression not just the subscript
			_lines->updateLastLineMaxUp_Down(up,down);
		}
		mybox->shift(firstXShift,0);
		
		if (targetLine == _targetLine) // the Sub is back together
		{
			setTargetLineNo(targetLine);
			if (beforeSubUp < up)
				_lines->last->maxUp = up;
			else
				_lines->last->maxUp = beforeSubUp;
	
			if (beforeSubDown < down)
				_lines->last->maxDown = down;
			else
				_lines->last->maxDown = beforeSubDown;
	
			broken = false;
		}
		else
		{
			broken = true;
		}
		// shift the current box to the same relative position to the first child
		_lastLeft = lastLeft;
		_targetLine = targetLine;
		return firstXShift;
	}
	//----------------------------------------------------------
	float CSymbolSub::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = base->adjustLines(_oldLines,_lines);
		sub->adjustLines(_oldLines,_lines);
		mybox->shift(0,YShift * SYMBOLHI);
	
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------
	void CSymbolSub::setTargetLineNo(int _lineNo)
	{
		base->setTargetLineNo(_lineNo);
		sub->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//-------------------------------------------
	bool CSymbolSub::hasEllipses() 
	{
		if (base->hasEllipses())
			return true;
		if (sub->hasEllipses())
			return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolSubSup::CSymbolSubSup(CSymbol *_base, CSymbol *_sub, CSymbol *_exp,scg::ExpressionTree * _exprTree)
	{
		base = _base;
		sub = _sub;
		exp = _exp;
		type = NONTOKEN;

		if (base->type == OPERATOR)
		{
			type = base->type;
			stretch = base->stretch;
			opForm = base->opForm;
			lspace = base->lspace;
			rspace = base->rspace;
			largeOp = base->largeOp;
		}
		exprTree = _exprTree;
		
		base->parentSymbol = this;
		sub->parentSymbol = this;
		exp->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolSubSup::~CSymbolSubSup(void)
	{
		delete(base);
		delete(sub);
		delete(exp);
	}
	//--------------------------------------------
	CSymbolBox* CSymbolSubSup::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x,*b,*e, *s;
		x = new CSymbolBox;
	
		x->left = _left;
		
		b = base->createBox(_left ,_center,_device);
		e = exp->createBox(b->right,_center -(base->up + exp->down-.5) *SYMBOLHI ,_device);
		s = sub->createBox(b->right,_center +(base->down + sub->up - .5) * SYMBOLHI,_device);
	
		x->right=e->right;
		x->top = exp->getTop();
		x->bottom = sub->getBottom();
		mybox = x;
	
		if (type == OPERATOR && stretch)
			stretchBoxV(_center - up*SYMBOLHI , _center + down * SYMBOLHI);
	
		return x;
	}
	//----------------------------------------------
	void CSymbolSubSup::display(CDevice * _device, bool _showBoxes)
	{
	    base->display(_device,_showBoxes);
		//CFont *defFont = (CFont *)_pDC->SelectStockObject(ANSI_VAR_FONT);
		//LOGFONT lf;
		//defFont->GetLogFont(&lf);
		//if(lf.lfHeight>=0.7*SYMBOLHI)
		//	lf.lfHeight-=2;
		//CFont expFont;
		//VERIFY(expFont.CreateFontIndirect(&lf));
		//_pDC->SelectObject(expFont);	
	
		exp->display(_device, _showBoxes);
	    sub->display(_device, _showBoxes);
	
		//_pDC->SelectObject(defFont);	
		//expFont.DeleteObject();
	}
	//------------------------------------------------
	bool CSymbolSubSup::selectSymbols(SCGRECT _rect)
	{
		bool a = base->selectSymbols(_rect);
		bool b = exp->selectSymbols(_rect);
		bool c = sub->selectSymbols(_rect);

		return a|| b || c;
	}
	//------------------------------------------------
	void CSymbolSubSup::clearSelection()
	{
		base->clearSelection();
		exp->clearSelection();
		sub->clearSelection();
	}
	//------------------------------------------------
	void CSymbolSubSup::getSelectedSymbols(std::wstring &_ws)
	{
		base->getSelectedSymbols(_ws);
		exp->getSelectedSymbols(_ws);
		sub->getSelectedSymbols(_ws);
	}
	//------------------------------------------------
	void CSymbolSubSup::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		base->getSelectedSymbols(_symbols,_device);
		exp->getSelectedSymbols(_symbols,_device);
		sub->getSelectedSymbols(_symbols,_device);
	}
	//------------------------------------------------
	std::string CSymbolSubSup::getSelectedMathML()
	{
		std::string s1 = base->getSelectedMathML();
		std::string s2 = exp->getSelectedMathML();
		std::string s3 = sub->getSelectedMathML();

		if (s1 != "" && s2 != "" && s3 != "")
			return "<msubsup>" + s1 + s2 +s3 + "</msubsup>";
		
		if (s1 != "") // the base is selected
			if (s2 == "" && s3 == "") // none of the limits
				return s1;
			else
				return UNKNOWNSELECTION;  // can npot select the base and one of the limits
		else  // the base is not selected can only select one of the limits
			if (s2 != "" && s3 != "")
				return UNKNOWNSELECTION;
			else
				return s2+s3; // one of them is selected
	}
	//------------------------------------------------
	void CSymbolSubSup::calcDepth(float l)
	{
	
		//if(l>=0.7)
		//{
		//	exp->calcDepth(l-2.0/SYMBOLHI);
		//	sub->calcDepth(l-2.0/SYMBOLHI);
		//}
		//else
		//{
			exp->calcDepth(l);
			sub->calcDepth(l);
		//}
		base->calcDepth(l);
	
		up = base->up + exp->up+exp->down- 0.5*l;
		down = base->down + sub->up + sub->down - 0.5*l;
	}
	//------------------------------------------------
	void CSymbolSubSup::shift(int _x,int _y)
	{
		base->shift(_x,_y);
		exp->shift(_x,_y);
		sub->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//------------------------------------------------
	void CSymbolSubSup::stretchBoxV(int _top, int _bottom)
	{
		exp->shift(0,_top-exp->getTop());
		sub->shift(0,_bottom-sub->getBottom());
		base->stretchBoxV(exp->getBottom() - 0.5 * SYMBOLHI, sub->getTop() + 0.5 * SYMBOLHI);
	
		mybox->top = _top;
		mybox->bottom = _bottom;
	}
	//-------------------------------------------------
	void CSymbolSubSup::generateVariables(std::vector<std::wstring> &_vars)
	{
		exp->generateVariables(_vars);
		sub->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//-------------------------------------------------
	void CSymbolSubSup::generateVariables(std::vector<CSymbol *> &_vars)
	{
		exp->generateVariables(_vars);
		sub->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//---------------------------------------------------------
	float CSymbolSubSup::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int lastLeft = _lastLeft;
		int targetLine = _targetLine;
	
		// for now breaking the under expression is not supported !!!
		if (lastLeft > _scrLeft + _indent)
		{
			targetLine++;
			lastLeft = _scrLeft + _indent;
			// add the new line to the line list - construct a list of the new lines in the document
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
		}
	
		_lines->updateLastLineMaxUp_Down(up,down);
	
		int firstXShift = lastLeft - getLeft(); 
		shift(firstXShift,0);
		setTargetLineNo(targetLine);
		lastLeft = getRight();
		
		_targetLine = targetLine;
		_lastLeft = lastLeft;
	
		return firstXShift;
	}
	//----------------------------------------------------------
	float CSymbolSubSup::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = base->adjustLines(_oldLines,_lines);
		exp->adjustLines(_oldLines,_lines);
		sub->adjustLines(_oldLines,_lines);
	
		mybox->shift(0,YShift * SYMBOLHI);
	
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------
	void CSymbolSubSup::setTargetLineNo(int _lineNo)
	{
		base->setTargetLineNo(_lineNo);
		exp->setTargetLineNo(_lineNo);
		sub->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//-------------------------------------------
	bool CSymbolSubSup::hasEllipses() 
	{
		if (base->hasEllipses())
			return true;
		if (exp->hasEllipses())
			return true;
		if (sub->hasEllipses())
			return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolSqrt::CSymbolSqrt(CSymbol * _elem, scg::ExpressionTree * _exprTree)
	{
		elem = _elem;
		type = NONTOKEN;
		lspace = THICKMATHSPACE;   // added this recently to add somespace before drawing the root
	
		sqrt = new CSymbolBasic(L"sqrt",IDENTIFIER,4);

		exprTree = _exprTree;
		
		elem->parentSymbol = this;
		sqrt->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolSqrt::~CSymbolSqrt(void)
	{
		if (elem != NULL)
			delete(elem);
		delete(sqrt);
	}
	//------------------------------------------------
	void CSymbolSqrt::calcDepth(float l)
	{
	
		elem->calcDepth(l);
		sqrt->calcDepth(l);
	
		up = elem->up + 0.25*l;
		down = elem->down;
	}
	//--------------------------------------------
	CSymbolBox* CSymbolSqrt::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x,*e;
		x = new CSymbolBox;
		
		e = elem->createBox(_left,_center,_device);
		elem->shift(lspace + e->getHeight()/4.0,0);
		
		x->left = _left;
		x->right = elem->getRight();
		x->top = elem->getTop() - 0.25 * SYMBOLHI;
		x->bottom = elem->getBottom();
		mybox = x;
	
		sqrt->createBox(getLeft(),_center,_device);
		return x;
	}
	//----------------------------------------------
	void CSymbolSqrt::display(CDevice * _device, bool _showBoxes)
	{
		setBackColor(_device, _showBoxes);
        if (_showBoxes)
            _device->fillRect(mybox->getRectangle());

		elem->display(_device,_showBoxes);
		if (!broken)
		{
			setPenColor(_device);
			_device->drawLine(mybox->left+lspace,(mybox->top+mybox->bottom)/2,(mybox->left+lspace+elem->getLeft())/2,mybox->bottom); // the \ line
			_device->drawLine((mybox->left+lspace+elem->getLeft())/2,mybox->bottom,elem->getLeft(),elem->getTop() - 2); // the | line
			_device->drawLine(elem->getLeft(),elem->getTop() - 2,mybox->right,elem->getTop() - 2); // the horizontal line
		}
		else
		{
			sqrt->display(_device, _showBoxes);
		}
	}
	//------------------------------------------------
	CSymbol * CSymbolSqrt::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			CSymbol * crtSymbol = elem->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;
			
			return this;
		}
		else
			return NULL;
	}
	//------------------------------------------------
	CSymbol * CSymbolSqrt::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			CSymbol * crtSymbol = elem->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			return NULL;
		}
	}
	//------------------------------------------------
	bool CSymbolSqrt::selectSymbols(SCGRECT _rect)
	{
		bool a = elem->selectSymbols(_rect);
		bool b = false;
		bool c = false;
		
		CSymbolBox * sqrtBox = new CSymbolBox(mybox->left+lspace,elem->getTop() - 2,mybox->bottom, elem->getLeft());
		if (sqrtBox->intersects(_rect))
		{
			if (selected)
				selected = false;
			else
				selected = true;
			c = true;
		}
		delete sqrtBox;

		if (broken)
			if (sqrt->selectSymbols(_rect))
				b = true;

		return a || b || c;
	}
	//------------------------------------------------
	void CSymbolSqrt::clearSelection()
	{
		selected = false;
		elem->clearSelection();
		sqrt->clearSelection();
	}
	//------------------------------------------------
	void CSymbolSqrt::getSelectedSymbols(std::wstring &_ws)
	{
		elem->getSelectedSymbols(_ws);
		if (selected)
			_ws += mybox->toWString() + L",\u221A\n";
		
		if (broken)
			sqrt->getSelectedSymbols(_ws);
	}
	//------------------------------------------------
	void CSymbolSqrt::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		elem->getSelectedSymbols(_symbols,_device);
		if (selected)
			_symbols.push_back(scg::MathSymbol(8730,scg::Rect<long>(mybox->left, mybox->top, mybox->right, mybox->bottom)));
		
		if (broken)
			sqrt->getSelectedSymbols(_symbols,_device);
	}
	//------------------------------------------------
	std::string CSymbolSqrt::getSelectedMathML()
	{
		std::string s = elem->getSelectedMathML();
		std::string s1 = "";
		if (broken)
			s1 = sqrt->getSelectedMathML();

		if (selected || s1 != "")  // the sqrt sign is selected
		{
			if (s != "")
				return "<msqrt>" + s + "</msqrt>";
			else
				return UNKNOWNSELECTION;
		}
		else
			return s;

	}
	//------------------------------------------------
	void CSymbolSqrt::shift(int _x,int _y)
	{
		elem->shift(_x,_y);
		sqrt->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//------------------------------------------------
	void CSymbolSqrt::generateVariables(std::vector<std::wstring> &_vars)
	{
		elem->generateVariables(_vars);
	}
	//------------------------------------------------
	void CSymbolSqrt::generateVariables(std::vector<CSymbol *> &_vars)
	{
		elem->generateVariables(_vars);
	}
	//---------------------------------------------------------
	float CSymbolSqrt::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int targetLine=_targetLine;
		int lastLeft = _lastLeft;
		int firstXShift=0;
	
		if (getWidth() > _scrRight - _scrLeft - _indent)
		{
			// the element will be broken
			if (lastLeft + sqrt->getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
			}
			sqrt->shift(lastLeft-sqrt->getLeft(),0);
			lastLeft += sqrt->getWidth();
			sqrt->setTargetLineNo(targetLine);
		}
		else // the sqrt will be together
		{
			if (lastLeft + getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
				_targetLine = targetLine;
			}
			lastLeft += elem->getHeight()/4.0;
		}
	
		if (elem->getWidth() > _scrRight - _scrLeft - _indent || elem->broken)
		{
			firstXShift = elem->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
			elem->targetLineNo = targetLine;
		}
		else
		{
			if (elem->getWidth() + lastLeft > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
			}
			firstXShift = lastLeft - elem->getLeft();
			elem->shift(firstXShift,0);
			elem->setTargetLineNo(targetLine);
			lastLeft = elem->getRight();
			
			// adjust the max up and down of the line
			_lines->updateLastLineMaxUp_Down(elem->up,elem->down);
		}
		mybox->shift(firstXShift,0);
		
		if (targetLine == _targetLine) // the sqrt is one piece again
		{
			setTargetLineNo(targetLine);
			if (elem->up > _lines->last->maxUp)
				_lines->last->maxUp = elem->up;
			broken = false;
		}
		else
			broken = true;
	
		_lastLeft = lastLeft;
		_targetLine = targetLine;
		return firstXShift;
	}
	//----------------------------------------------------------
	float CSymbolSqrt::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = elem->adjustLines(_oldLines,_lines);
		sqrt->adjustLines(_oldLines,_lines);
	
		mybox->shift(0,YShift * SYMBOLHI);
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------
	void CSymbolSqrt::setTargetLineNo(int _lineNo)
	{
		elem->setTargetLineNo(_lineNo);
		sqrt->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//---------------------------------------------
	bool CSymbolSqrt::hasEllipses() 
	{
		if (elem->hasEllipses())
			return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolUnder::CSymbolUnder(CSymbol* _base, CSymbol* _under, scg::ExpressionTree * _exprTree)
	{
		base = _base;
		under = _under;
		type = NONTOKEN;
		if (base->type == OPERATOR)
		{
			type = base->type;
			stretch = base->stretch;
			opForm = base->opForm;
			lspace = base->lspace;
			rspace = base->rspace;
			largeOp = base->largeOp;
		}
		exprTree = _exprTree;
		
		base->parentSymbol = this;
		under->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolUnder::~CSymbolUnder(void)
	{
		delete(base);
		delete(under);
	}
	//--------------------------------------------
	CSymbolBox* CSymbolUnder::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x,*b,*u;
		x = new CSymbolBox;
	
		x->left = _left;
		
		b = base->createBox(_left ,_center,_device);
		u = under->createBox(_left,_center +(base->down + under->up)*SYMBOLHI,_device);
		
		// center both of the base and the under together
		if (b->right > u->right)  // base is bigger then shift the under
		{
			under->shift((b->getWidth()-u->getWidth())/2,0);
			x->right = b->right;
		}
		else
		{
			base->shift((u->getWidth() - b->getWidth())/2,0);
			x->right = u->right;
		}
		x->top = b->top;
		x->bottom = u->bottom;
		mybox = x;
	
		if (type == OPERATOR && stretch)
			stretchBoxV(_center - up*SYMBOLHI , _center + down * SYMBOLHI);
	
		return x;
	}
	//----------------------------------------------
	void CSymbolUnder::display(CDevice * _device, bool _showBoxes)
	{
	    base->display(_device,_showBoxes);
		//CFont *defFont = (CFont *)_pDC->SelectStockObject(ANSI_VAR_FONT);
		//LOGFONT lf;
		//defFont->GetLogFont(&lf);
		//if(lf.lfHeight>=0.7*SYMBOLHI)
		//	lf.lfHeight-=2;
		//CFont underFont;
		//VERIFY(underFont.CreateFontIndirect(&lf));
		//_pDC->SelectObject(underFont);	
	
		under->display(_device, _showBoxes);
	    
		//_pDC->SelectObject(defFont);	
		//underFont.DeleteObject();
	}
	//------------------------------------------------
	void CSymbolUnder::calcDepth(float l)
	{
		//if(l>=0.7)
		//	under->calcDepth(l-2.0/SYMBOLHI);
		//else
			under->calcDepth(l);
		
		base->calcDepth(l);
	
		up = base->up ;
		down = base->down + under->up + under->down ;
	}
	//------------------------------------------------
	bool CSymbolUnder::selectSymbols(SCGRECT _rect)
	{
		bool a = base->selectSymbols(_rect);
		bool b = under->selectSymbols(_rect);

		return a || b;
	}
	//------------------------------------------------
	void CSymbolUnder::clearSelection()
	{
		base->clearSelection();
		under->clearSelection();
	}
	//------------------------------------------------
	void CSymbolUnder::getSelectedSymbols(std::wstring &_ws)
	{
		base->getSelectedSymbols(_ws);
		under->getSelectedSymbols(_ws);
	}
	//------------------------------------------------
	void CSymbolUnder::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		base->getSelectedSymbols(_symbols,_device);
		under->getSelectedSymbols(_symbols,_device);
	}
	//------------------------------------------------
	std::string CSymbolUnder::getSelectedMathML()
	{
		std::string s1 = base->getSelectedMathML();
		std::string s2 = under->getSelectedMathML();

		if (s1 != "" && s2 != "")
			return "<munder>" + s1 + s2 + "</munder>";
		else
			return s1 + s2;

	}
	//------------------------------------------------
	void CSymbolUnder::shift(int _x,int _y)
	{
		base->shift(_x,_y);
		under->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//------------------------------------------------
	void CSymbolUnder::stretchBoxV(int _top, int _bottom)
	{
		under->shift(0,_bottom - under->getBottom());
		base->stretchBoxV(_top,under->getTop());
	
		mybox->top = _top;
		mybox->bottom = _bottom;
	}
	//------------------------------------------------
	void CSymbolUnder::generateVariables(std::vector<std::wstring> &_vars)
	{
		under->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//------------------------------------------------
	void CSymbolUnder::generateVariables(std::vector<CSymbol *> &_vars)
	{
		under->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//---------------------------------------------------------
	float CSymbolUnder::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int lastLeft = _lastLeft;
		int targetLine = _targetLine;
	
		// for now breaking the under expression is not supported !!!
		if (lastLeft > _scrLeft + _indent)
		{
			targetLine++;
			lastLeft = _scrLeft + _indent;
			// add the new line to the line list - construct a list of the new lines in the document
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
		}
	
		_lines->updateLastLineMaxUp_Down(up,down);
	
		int firstXShift = lastLeft - getLeft(); 
		shift(firstXShift,0);
		setTargetLineNo(targetLine);
		lastLeft = getRight();
		
		_targetLine = targetLine;
		_lastLeft = lastLeft;
	
		return firstXShift;
	}
	//----------------------------------------------------------
	float CSymbolUnder::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = base->adjustLines(_oldLines,_lines);
		under->adjustLines(_oldLines,_lines);
		mybox->shift(0,YShift * SYMBOLHI);
	
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------
	void CSymbolUnder::setTargetLineNo(int _lineNo)
	{
		base->setTargetLineNo(_lineNo);
		under->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//------------------------------------------
	bool CSymbolUnder::hasEllipses()
	{
		if (base->hasEllipses())
			return true;
		if (under->hasEllipses())
			return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolOver::CSymbolOver(CSymbol* _base, CSymbol* _over, scg::ExpressionTree * _exprTree)
	{
		base = _base;
		over = _over;
		type = NONTOKEN;
		if (base->type == OPERATOR)
		{
			type = base->type;
			stretch = base->stretch;
			opForm = base->opForm;
			lspace = base->lspace;
			rspace = base->rspace;
			largeOp = base->largeOp;
		}
		exprTree = _exprTree;
		
		base->parentSymbol = this;
		over->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolOver::~CSymbolOver(void)
	{
		delete(base);
		delete(over);
	}
	//---------------------------------------------
	CSymbolBox* CSymbolOver::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		CSymbolBox *x,*b,*o;
		x = new CSymbolBox;
	
		x->left = _left;
		
		b = base->createBox(_left ,_center,_device);
		o = over->createBox(_left,_center - (base->up + over->down)*SYMBOLHI,_device);
		
		// center both of the base and the over together
		if (b->right > o->right)  // base is bigger then shift the under
		{
			over->shift((b->getWidth()- o->getWidth())/2,0);
			x->right = b->right;
		}
		else
		{
			base->shift((o->getWidth() - b->getWidth())/2,0);
			x->right = o->right;
		}
		x->top = o->top;
		x->bottom = b->bottom;
		mybox = x;
	
		if (type == OPERATOR && stretch)
			stretchBoxV(_center - up*SYMBOLHI , _center + down * SYMBOLHI);
	
		return x;
	}
	//----------------------------------------------
	void CSymbolOver::display(CDevice * _device, bool _showBoxes)
	{
	    base->display(_device,_showBoxes);
		//CFont *defFont = (CFont *)_pDC->SelectStockObject(ANSI_VAR_FONT);
		//LOGFONT lf;
		//defFont->GetLogFont(&lf);
		//if(lf.lfHeight>=0.7*SYMBOLHI)
		//	lf.lfHeight-=2;
		//CFont overFont;
		//VERIFY(overFont.CreateFontIndirect(&lf));
		//_pDC->SelectObject(overFont);	
	
		over->display(_device, _showBoxes);
	    
		//_pDC->SelectObject(defFont);	
		//overFont.DeleteObject();
	}
	//------------------------------------------------
	bool CSymbolOver::selectSymbols(SCGRECT _rect)
	{
		bool a = base->selectSymbols(_rect);
		bool b = over->selectSymbols(_rect);

		return a || b;
	}
	//------------------------------------------------
	void CSymbolOver::clearSelection()
	{
		base->clearSelection();
		over->clearSelection();
	}
	//------------------------------------------------
	void CSymbolOver::getSelectedSymbols(std::wstring &_ws)
	{
		base->getSelectedSymbols(_ws);
		over->getSelectedSymbols(_ws);
	}
	//------------------------------------------------
	void CSymbolOver::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		base->getSelectedSymbols(_symbols,_device);
		over->getSelectedSymbols(_symbols,_device);
	}
	//------------------------------------------------
	std::string CSymbolOver::getSelectedMathML()
	{
		std::string s1 = base->getSelectedMathML();
		std::string s2 = over->getSelectedMathML();

		if (s1 != "" && s2 != "")
			return "<mover>" + s1 + s2 + "</mover>";
		else
			return s1 + s2;
	}
	//------------------------------------------------
	void CSymbolOver::calcDepth(float l)
	{
		//if(l>=0.7)
		//	over->calcDepth(l-2.0/SYMBOLHI);
		//else
			over->calcDepth(l);
		
		base->calcDepth(l);
	
		up = base->up + over->down + over->up ;
		down = base->down;
	}
	//------------------------------------------------
	void CSymbolOver::shift(int _x,int _y)
	{
		base->shift(_x,_y);
		over->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//------------------------------------------------
	void CSymbolOver::stretchBoxV(int _top, int _bottom)
	{
		over->shift(0,_top - over->getTop());
		base->stretchBoxV(over->getBottom(),_bottom);
	
		mybox->top = _top;
		mybox->bottom = _bottom;
	}
	//------------------------------------------------
	void CSymbolOver::generateVariables(std::vector<std::wstring> &_vars)
	{
		over->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//------------------------------------------------
	void CSymbolOver::generateVariables(std::vector<CSymbol *> &_vars)
	{
		over->generateVariables(_vars);
		base->generateVariables(_vars);
	}
	//---------------------------------------------------------
	float CSymbolOver::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int lastLeft = _lastLeft;
		int targetLine = _targetLine;
	
		// for now breaking the under expression is not supported !!!
		if (lastLeft > _scrLeft + _indent)
		{
			targetLine++;
			lastLeft = _scrLeft + _indent;
			// add the new line to the line list - construct a list of the new lines in the document
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
		}
	
		_lines->updateLastLineMaxUp_Down(up,down);
	
		int firstXShift = lastLeft - getLeft(); 
		shift(firstXShift,0);
		setTargetLineNo(targetLine);
		lastLeft = getRight();
		
		_targetLine = targetLine;
		_lastLeft = lastLeft;
	
		return firstXShift;
	}
	//----------------------------------------------------------
	float CSymbolOver::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = base->adjustLines(_oldLines,_lines);
		over->adjustLines(_oldLines,_lines);
		mybox->shift(0,YShift * SYMBOLHI);
	
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------
	void CSymbolOver::setTargetLineNo(int _lineNo)
	{
		base->setTargetLineNo(_lineNo);
		over->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//---------------------------------------------
	bool CSymbolOver::hasEllipses() 
	{
		if (base->hasEllipses())
			return true;
		if (over->hasEllipses())
			return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolUnderOver::CSymbolUnderOver(CSymbol* _base, CSymbol* _under, CSymbol* _over, scg::ExpressionTree * _exprTree)
	{
		base = _base;
		under = _under;
		over = _over;
		type = NONTOKEN;

		if (base->type == OPERATOR)
		{
			type = base->type;
			stretch = base->stretch;
			opForm = base->opForm;
			lspace = base->lspace;
			rspace = base->rspace;
			largeOp = base->largeOp;
		}
		exprTree = _exprTree;
		
		base->parentSymbol = this;
		over->parentSymbol = this;
		under->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolUnderOver::~CSymbolUnderOver(void)
	{
		delete(base);
		delete(over);
		delete(under);
	}
	//---------------------------------------------
	CSymbolBox* CSymbolUnderOver::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		int xx = _center;
		CSymbolBox *x,*b,*o,*u;
		x = new CSymbolBox;
	
		x->left = _left;
		
		b = base->createBox(_left ,_center,_device);
		o = over->createBox(_left,_center - (base->up + over->down)*SYMBOLHI,_device);
		u = under->createBox(_left,_center + (base->down + under->up) * SYMBOLHI, _device);
	
		// center base, under and over together
		int maxLimitsWidth = u->getWidth() > o->getWidth()? u->getWidth():o->getWidth();
		int maxWidth =  maxLimitsWidth > b->getWidth()? maxLimitsWidth : b->getWidth();
	
		over->shift((maxWidth-o->getWidth())/2,0);
		base->shift((maxWidth-b->getWidth())/2,0);
		under->shift((maxWidth-u->getWidth())/2,0);
	
		x->right = _left + maxWidth;
		x->top = o->top;
		x->bottom = u->bottom;
		mybox = x;
	
		if (type == OPERATOR && stretch)
			stretchBoxV(_center - up*SYMBOLHI , _center + down * SYMBOLHI);
	
		return mybox;
	}
	//----------------------------------------------
	void CSymbolUnderOver::display(CDevice * _device, bool _showBoxes)
	{
	    base->display(_device,_showBoxes);
		//LOGFONT lf;
		//if (GetObject(_device,sizeof(HFONT),&lf))
		//{
		//	if(lf.lfHeight>=0.7*SYMBOLHI)
		//		lf.lfHeight-=2;
		//
		//	HFONT overFont = CreateFontIndirect(&lf);

		//	HFONT defFont = (HFONT) SelectObject(_device,overFont);	

			over->display(_device, _showBoxes);
			under ->display(_device, _showBoxes);

		//	overFont = (HFONT) SelectObject(_device,defFont);
		//	DeleteObject(overFont);
		//}
		//else
		//{
		//	over->display(_device, _showBoxes);
		//	under ->display(_device, _showBoxes);
		//}
	}
	//------------------------------------------------
	void CSymbolUnderOver::calcDepth(float l)
	{
		//if(l>=0.7)
		//{
		//	over->calcDepth(l-2.0/SYMBOLHI);
		//	under->calcDepth(1-2.0/SYMBOLHI);
		//}
		//else
		//{
			over->calcDepth(l);
			under->calcDepth(l);
		//}
	
		base->calcDepth(l);
	
		up = base->up + over->down + over->up ;
		down = base->down + under->up + under->down;
	}
	//------------------------------------------------
	void CSymbolUnderOver::shift(int _x,int _y)
	{
		base->shift(_x,_y);
		over->shift(_x,_y);
		under->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//------------------------------------------------
	void CSymbolUnderOver::stretchBoxV(int _top, int _bottom)
	{
		under->shift(0,_bottom - under->getBottom());
		over->shift(0,_top - over->getTop());
		base->stretchBoxV(over->getBottom(),under->getTop());
	
		mybox->stretchV(_top, _bottom);
	}
	//-------------------------------------------
	CSymbol * CSymbolUnderOver::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			CSymbol * crtSymbol = base->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = over->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = under->getCrtSymbol(_x,_y);
			if (crtSymbol != NULL)
				return crtSymbol;
			
			return this;
		}
		else
			return NULL;
	}
	//------------------------------------------------
	CSymbol * CSymbolUnderOver::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			CSymbol * crtSymbol = base->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = over->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			crtSymbol = under->getCrtSymbol(_rect);
			if (crtSymbol != NULL)
				return crtSymbol;

			return NULL;
		}
	}
	//------------------------------------------------
	bool CSymbolUnderOver::selectSymbols(SCGRECT _rect)
	{
		bool a = base->selectSymbols(_rect);
		bool b = over->selectSymbols(_rect);
		bool c = under->selectSymbols(_rect);

		return a || b || c;
	}
	//-----------------------------------------------
	void CSymbolUnderOver::clearSelection()
	{
		base->clearSelection();
		under->clearSelection();
		over->clearSelection();
	}
	//------------------------------------------------
	void CSymbolUnderOver::getSelectedSymbols(std::wstring &_ws)
	{
		base->getSelectedSymbols(_ws);
		over->getSelectedSymbols(_ws);
		under->getSelectedSymbols(_ws);
	}
	//------------------------------------------------
	void CSymbolUnderOver::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		base->getSelectedSymbols(_symbols,_device);
		over->getSelectedSymbols(_symbols,_device);
		under->getSelectedSymbols(_symbols,_device);
	}
	//------------------------------------------------
	std::string CSymbolUnderOver::getSelectedMathML()
	{
		std::string s1 = base->getSelectedMathML();
		std::string s2 = under->getSelectedMathML();
		std::string s3 = over->getSelectedMathML();

		if (s1 != "" && s2 != "" && s3 != "")
			return "<munderover>" + s1 + s2 +s3 + "</munderover>";
		
		if (s1 != "") // the base is selected
		{
			if (s2 == "" && s3 == "") // none of the limits
				return s1;
			else
				return UNKNOWNSELECTION;  // can npot select the base and one of the limits
		}
		else  // the base is not selected can only select one of the limits
		{
			if (s2 != "" && s3 != "")
				return UNKNOWNSELECTION;
			else
				return s2+s3; // one of them is selected
		}
	}
	//------------------------------------------------
	void CSymbolUnderOver::generateVariables(std::vector<std::wstring> &_vars)
	{
		base->generateVariables(_vars);
		under->generateVariables(_vars);
		over->generateVariables(_vars);
	}
	//------------------------------------------------
	void CSymbolUnderOver::generateVariables(std::vector<CSymbol *> &_vars)
	{
		base->generateVariables(_vars);
		under->generateVariables(_vars);
		over->generateVariables(_vars);
	}
	//---------------------------------------------------------
	float CSymbolUnderOver::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int lastLeft = _lastLeft;
		int targetLine = _targetLine;
	
		// for now breaking the under expression is not supported !!!
		if (lastLeft > _scrLeft + _indent)
		{
			targetLine++;
			lastLeft = _scrLeft + _indent;
			// add the new line to the line list - construct a list of the new lines in the document
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
		}
	
		_lines->updateLastLineMaxUp_Down(up,down);
	
		int firstXShift = lastLeft - getLeft(); 
		shift(firstXShift,0);
		setTargetLineNo(targetLine);
		lastLeft = getRight();
		
		_targetLine = targetLine;
		_lastLeft = lastLeft;
	
		return firstXShift;
	}
	//----------------------------------------------------------
	float CSymbolUnderOver::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = base->adjustLines(_oldLines,_lines);
		under->adjustLines(_oldLines,_lines);
		over->adjustLines(_oldLines,_lines);
	
		mybox->shift(0,YShift * SYMBOLHI);
	
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------
	void CSymbolUnderOver::setTargetLineNo(int _lineNo)
	{
		base->setTargetLineNo(_lineNo);
		under->setTargetLineNo(_lineNo);
		over->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//------------------------------------------
	bool CSymbolUnderOver::hasEllipses() 
	{
		if (base->hasEllipses())
			return true;
		if (under->hasEllipses())
			return true;
		if (over->hasEllipses())
			return true;
		return false;
	}
	//=========================================================================
	//=========================================================================
	CSymbolMultiScript::CSymbolMultiScript(int _noOfChildren, scg::ExpressionTree * _exprTree)
	{
		preScriptIdx = _noOfChildren;
		noOfChildren = _noOfChildren;
		children = (CSymbol **) malloc(sizeof(CSymbol *) * noOfChildren);
		exprTree = _exprTree;
	}
	//--------------------------------------------
	CSymbolMultiScript::~CSymbolMultiScript(void)
	{
		for(int i =0; i< noOfChildren; i++)
			delete children[i];
		delete children;
	}
	//-------------------------------------------
	void CSymbolMultiScript::calcDepth(float l)
	{
		children[0]->calcDepth(l);
	
		if(l>=0.7)
			for (int i = 1 ;i < noOfChildren ; i++)
				children[i]->calcDepth(l-2.0/SYMBOLHI);
		else
			for (int i = 1 ;i < noOfChildren ; i++)
				children[i]->calcDepth(l);
	
		// children[0] is the base it will be taken as it is
		// we need to calculate the max depth for all the scripts
		float maxSubHi, maxSuperHi;
		maxSubHi = maxSuperHi = 0;
	
		for (int i=1; i < preScriptIdx; i+=2)
		{
			if (children[i]->up + children[i]->down > maxSubHi)
				maxSubHi = children[i]->up + children[i]->down;
			if (children[i+1]->up + children[i+1]->down > maxSuperHi)
				maxSuperHi = children[i+1]->up + children[i+1]->down;
		}
		for (int i=preScriptIdx+1; i < noOfChildren; i+=2)
		{
			if (children[i]->up + children[i]->down > maxSubHi)
				maxSubHi = children[i]->up + children[i]->down;
			if (children[i+1]->up + children[i+1]->down > maxSuperHi)
				maxSuperHi = children[i+1]->up + children[i+1]->down;
		}
	
		if (maxSuperHi)
			up=children[0]->up + maxSuperHi - 0.5*l;
		else
			up=children[0]->up;
	
		if (maxSubHi)
			down=children[0]->down + maxSubHi - 0.5*l;
		else
			down=children[0]->down;
	}
	//--------------------------------------------
	CSymbolBox* CSymbolMultiScript::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);
		
		// note that the lists of pre and post scripts are given in a left-to-right order
		CSymbolBox *x, *sub, *sup, *b1, *b2, *b;
		x = new CSymbolBox;
	
		x->left = _left;
		x->right = _left;
	
		b1 = b2 = x;
	
		// create boxes for the prescripts first
		for (int i = preScriptIdx + 1; i < noOfChildren; i+=2)
		{
			sub = children[i]->createBox(b1->right,_center +(children[0]->down + children[i]->up - .5) * SYMBOLHI,_device);
			b1 = sub;
	
			sup = children[i+1]->createBox(b2->right,_center - (children[0]->up + children[i+1]->down-.5) *SYMBOLHI ,_device);
			b2 = sup;
	
			if (b1->right > b2->right)
				b2->right = b1->right;
			else
				b1->right = b2->right;
		}
	
		// create the box of the base
		b = children[0]->createBox(b1->right, _center,_device);
	
		// create boxes of the post scripts
		b1 = b2 = b;
		for (int i = 1; i < preScriptIdx ; i+=2)
		{
			sub = children[i]->createBox(b1->right,_center +(children[0]->down + children[i]->up - .5) * SYMBOLHI,_device);
			b1 = sub;
	
			sup = children[i+1]->createBox(b2->right,_center - (children[0]->up + children[i+1]->down-.5) *SYMBOLHI ,_device);
			b2 = sup;
	
			if (b1->right > b2->right)
				b2->right = b1->right;
			else
				b1->right = b2->right;
		}
	
		x->top = _center - up * SYMBOLHI;
		x->right = b2->right;
		x->bottom = _center + down * SYMBOLHI;
		
		mybox = x;
	
		return x;
	}
	//--------------------------------------------
	void CSymbolMultiScript::addChild(int _index, CSymbol* _symb)
	{
		children[_index] = _symb;
		if (_index == 0 && _symb->type == OPERATOR)
		{
			type = _symb->type;
			stretch = _symb->stretch;
			opForm = _symb->opForm;
			lspace = _symb->lspace;
			rspace = _symb->rspace;
			largeOp = _symb->largeOp;
		}
		if (_symb->type == PRESCRIPT)
			preScriptIdx = _index;
		
		_symb->parentSymbol = this;
	}
	//--------------------------------------------
	void CSymbolMultiScript::display(CDevice * _device, bool _showBoxes)
	{
		children[0]->display(_device,_showBoxes);
		

		//CFont *defFont = (CFont *)_pDC->SelectStockObject(ANSI_VAR_FONT);
		//LOGFONT lf;
		//defFont->GetLogFont(&lf);
		//if(lf.lfHeight>=0.7*SYMBOLHI)
		//	lf.lfHeight-=2;
		//CFont subFont;
		//VERIFY(subFont.CreateFontIndirect(&lf));
		//_pDC->SelectObject(subFont);	
	
		for (int i = 1 ;i < noOfChildren ; i++)
			children[i]->display(_device,_showBoxes);
	    
		//_pDC->SelectObject(defFont);	
		//subFont.DeleteObject();
		//if (_showBoxes)
		//	_pDC->FrameRect(mybox->getRectangle(),&CBrush(RGB(0,0,0)));	
	}
	//-------------------------------------------
	bool CSymbolMultiScript::selectSymbols(SCGRECT _rect)
	{
		bool somethingSelected = false;
		for (int i = 0 ;i < noOfChildren ; i++)
			if (children[i]->selectSymbols(_rect))
				somethingSelected = true;

		return somethingSelected;
	}
	//-------------------------------------------
	void CSymbolMultiScript::getSelectedSymbols(std::wstring & _ws)
	{
		for (int i = 0 ;i < noOfChildren ; i++)
			children[i]->getSelectedSymbols(_ws);
	}
	//-------------------------------------------
	void CSymbolMultiScript::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		for (int i = 0 ;i < noOfChildren ; i++)
			children[i]->getSelectedSymbols(_symbols,_device);
	}
	//-------------------------------------------
	void CSymbolMultiScript::shift(int _x,int _y)
	{
		for (int i=0; i < noOfChildren; i++)
			children[i]->shift(_x,_y);
		mybox->shift(_x,_y);
	}
	//-------------------------------------------
	void CSymbolMultiScript::stretchBoxV(int _top,int _bottom)
	{
		// get the lowest point of the super script and the highest point of the subscript
		int lowestPoint = _top;
		int highestPoint = _bottom;
	
		for (int i=0; i < preScriptIdx ; i+=2)
		{
			children[i]->shift(0,_top-children[i]->getTop()); // sub
			lowestPoint = children[i]->getBottom() > lowestPoint? children[i]->getBottom():lowestPoint;    
			children[i+1]->shift(0,_bottom-children[i+1]->getBottom()); //super
			highestPoint = children[i+1]->getTop() < highestPoint? children[i+1]->getTop():highestPoint;
		}
		
		children[0]->stretchBoxV(lowestPoint - 0.5 * SYMBOLHI, highestPoint + 0.5 * SYMBOLHI);
	
		mybox->top = _top;
		mybox->bottom = _bottom;
	}
	//--------------------------------------------------
	void CSymbolMultiScript::generateVariables(std::vector<std::wstring> &_vars) 
	{
		for (int i =0; i < noOfChildren; i++)
			children[i]->generateVariables(_vars);
	}
	//--------------------------------------------------
	void CSymbolMultiScript::generateVariables(std::vector<CSymbol *> &_vars) 
	{
		for (int i =0; i < noOfChildren; i++)
			children[i]->generateVariables(_vars);
	}
	//---------------------------------------------------------
	float CSymbolMultiScript::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int lastLeft = _lastLeft;
		int targetLine = _targetLine;
	
		// for now breaking the under expression is not supported !!!
		if (lastLeft > _scrLeft + _indent)
		{
			targetLine++;
			lastLeft = _scrLeft + _indent;
			// add the new line to the line list - construct a list of the new lines in the document
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
		}
	
		_lines->updateLastLineMaxUp_Down(up,down);
	
		int firstXShift = lastLeft - getLeft(); 
		shift(firstXShift,0);
		setTargetLineNo(targetLine);
		lastLeft = getRight();
		
		_targetLine = targetLine;
		_lastLeft = lastLeft;
	
		return firstXShift;
	
	}
	//----------------------------------------------------------
	float CSymbolMultiScript::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = 0;
		for (int i=0; i<noOfChildren; i++)
		{
			float y = children[i]->adjustLines(_oldLines,_lines);
			if (!i)
			{
				YShift = y;
				mybox->shift(0,YShift*SYMBOLHI);
			}
		}
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------
	void CSymbolMultiScript::setTargetLineNo(int _lineNo)
	{
		for (int i=0; i <noOfChildren; i++)
			children[i]->setTargetLineNo(_lineNo);
	
		targetLineNo = _lineNo;
	}
	//---------------------------------------------
	bool CSymbolMultiScript::hasEllipses()
	{
		for (int i=0; i <noOfChildren; i++)
			if (children[i]->hasEllipses())
				return true;
		return false;
	}
	//==========================================================
	//=========================================================================
	CSymbolTable::CSymbolTable(int _noOfRows,scg::ExpressionTree * _exprTree)
	{
		noOfRows = _noOfRows;
		rows = (CSymbolTR **) malloc(sizeof(CSymbolTR *) * noOfRows);
		noOfColumns = (int *) malloc(sizeof(int) * noOfRows);
		commas = (CSymbol **) malloc(sizeof(CSymbol *) * (noOfRows - 1));
		for (int i = 0; i < noOfRows - 1 ; i++)
		{
			commas[i] = new CSymbolBasic(L",",OPERATOR,1);
			commas[i]->parentSymbol = this;
		}
		maxNoOfColumns = 0;
		maxColWidths = NULL;
		exprTree = _exprTree;
	}
	//--------------------------------------------
	CSymbolTable::~CSymbolTable(void)
	{
		for(int i =0; i< noOfRows; i++)
		{
			delete rows[i];
			if (i < noOfRows - 1)
				delete commas[i];
		}
		free(rows);
		free(commas);
		free(noOfColumns);
	
		if (maxNoOfColumns != 0 && maxColWidths != NULL)
			free(maxColWidths);
	}
	//--------------------------------------------
	void CSymbolTable::addChild(int _idx, CSymbol* _symb)
	{
		rows[_idx] = (CSymbolTR *) _symb;
		noOfColumns[_idx] = rows[_idx]->getNoOfColumns();
		if (noOfColumns[_idx] > maxNoOfColumns)
			maxNoOfColumns = noOfColumns[_idx];
		
		rows[_idx]->parentSymbol = this;
	}
	//--------------------------------------------
	void CSymbolTable::calcDepth(float l)
	{
		float total = 0;
	
		for (int i = 0 ;i < noOfRows ; i++)
		{
			rows[i]->calcDepth(l);
			total += rows[i]->up + rows[i]->down;
			if (i < noOfRows - 1)
				commas[i]->calcDepth(1);
		}
		total += (noOfRows - 1) * ROWSPACING;
	
		// the up and down of a table must include the space between the rows and
		// make sure the baseline is in the middle of the matrix as a whole.
		up = down = total/2;
	}
	//------------------------------------------------------------------------
	CSymbolBox* CSymbolTable::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		// we have to specify the center for each row. because the matrix center
		// is not the center of all its rows.
		// because the matrix might be shifted in the page the top of the matrix 
		// is not 0. To calculate the shift we see how much is the difference
		// between the real center and the provided center
	
		int lastRowBottom = _center - up * SYMBOLHI;
	
		// the element has to be centered in the column and the columns have to be aligned
		maxColWidths = (int *) malloc (sizeof(int) * maxNoOfColumns);
		for (int i = 0; i < maxNoOfColumns; i++)
			maxColWidths[i] = 0;
		
		for (int i = 0; i < noOfRows; i++)
		{
			int crtCenter = lastRowBottom + rows[i]->up * SYMBOLHI;
			CSymbolBox *b = rows[i]->createBox(_left,crtCenter,_device);
			rows[i]->adjustMaxColWidths(maxColWidths);		
			lastRowBottom = b->bottom + ROWSPACING * SYMBOLHI;
		}
	
		// now based on the max width of each column the element needs to be centered
		int maxRight = 0;
		for (int i = 0 ; i < noOfRows; i++)
		{
			int rowRight = rows[i]->alignElementsInCols(maxColWidths);
			if (rowRight > maxRight)
				maxRight = rowRight;
		}
	
		mybox = new CSymbolBox(_left,rows[0]->getTop(),rows[noOfRows-1]->getBottom(),maxRight);
		
		for (int i= 0; i < noOfRows - 1; i++)
			commas[i]->createBox(getLeft(),_center,_device);
	
		return mybox;
	}
	//--------------------------------------------
	void CSymbolTable::display(CDevice * _device, bool _showBoxes)
	{
		for (int i=0; i<noOfRows; i++)
			rows[i]->display(_device,_showBoxes);
	
		if (broken)
		{
			for (int i = 0; i < noOfRows - 1; i++)
				commas[i]->display(_device,_showBoxes);
		}
	
		//if (_showBoxes)
		//	_pDC->FrameRect(mybox->getRectangle(),&CBrush(RGB(0,0,0)));	
	}
	//-----------------------------------------------
	CSymbol * CSymbolTable::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			for (int i =0; i < noOfRows ; i++)
			{
				CSymbol * crtSymbol = rows[i]->getCrtSymbol(_x,_y);
				if (crtSymbol != NULL)
					return crtSymbol;
			}
			return this;
		}
		else
			return NULL;
	}
	//-------------------------------------------
	CSymbol * CSymbolTable::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			for (int i = 0; i < noOfRows ; i++)
			{
				CSymbol * crtSymbol = rows[i]->getCrtSymbol(_rect);
				if (crtSymbol != NULL)
					return crtSymbol;
			}
			return NULL;
		}
	}
	//-------------------------------------------
	bool CSymbolTable::selectSymbols(SCGRECT _rect)
	{
		bool somethingSelected = false;

		for (int i = 0; i < noOfRows; i++)
		{
			if (rows[i]->selectSymbols(_rect))
				somethingSelected = true;
		}
		return somethingSelected;
	}
	//-------------------------------------------
	void CSymbolTable::clearSelection()
	{
		for (int i = 0; i < noOfRows; i++)
			rows[i]->clearSelection();
	}
	//-------------------------------------------
	void CSymbolTable::getSelectedSymbols(std::wstring &_ws)
	{
		for (int i = 0; i < noOfRows; i++)
			rows[i]->getSelectedSymbols(_ws);
	}
	//-------------------------------------------
	void CSymbolTable::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		for (int i = 0; i < noOfRows; i++)
			rows[i]->getSelectedSymbols(_symbols,_device);
	}
	//-------------------------------------------
	std::string CSymbolTable::getSelectedMathML()
	{
		int noOfSelectedRows = 0; // we will not add the <mrow> tags if only one child is selected

		std::string tableStr = "";
		int lastSelectedRow = 0;
		for (int i = 0; i < noOfRows; i++)
		{
			std::string s = "";
			s = rows[i]->getSelectedMathML();
			if (s != "")
			{
				noOfSelectedRows++;
				tableStr += s;
				lastSelectedRow = i;
			}
		}
		
		if (noOfSelectedRows > 1)
		{
			tableStr = "<mfenced><mtable>" + tableStr + "</mtable></mfenced>";
		}
		else if (noOfSelectedRows == 1) // if also one element is selected in that row then the user selected a single element and no need for the table tags at all
		{
			int noOfElements = 0;
			size_t found = tableStr.find("<mtd>");
			while (found != std::string::npos)
			{
				noOfElements++;
				found = tableStr.find("<mtd>",found + 5);
			}

			if (noOfElements == 1)
			{
				tableStr = tableStr.substr(10); // remove the <mtr><mtd>
				found = tableStr.find("</mtd>");
				tableStr = tableStr.substr(0,found);
			}
			else
				tableStr = "<mfenced><mtable>" + tableStr + "</mtable></mfenced>";
		}
		return tableStr;
	}
	//-------------------------------------------
	void CSymbolTable::shift(int _x,int _y)
	{
		for (int i=0; i < noOfRows; i++)
		{
			rows[i]->shift(_x,_y);
			if (i < noOfRows -1)
				commas[i]->shift(_x,_y);
		}
		mybox->shift(_x,_y);
	}
	//--------------------------------------------
	void CSymbolTable::generateVariables(std::vector<std::wstring> &_vars)
	{
		for (int i = 0; i < noOfRows; i++)
		{
			std::vector<std::wstring> tempVars;
			rows[i]->generateVariables(tempVars);
			for (int j = 0; j< tempVars.size(); j++)
				_vars.push_back(tempVars[j]);
		}
	}
	//--------------------------------------------
	void CSymbolTable::generateVariables(std::vector<CSymbol *> &_vars)
	{
		for (int i = 0; i < noOfRows; i++)
		{
			std::vector<CSymbol *> tempVars;
			rows[i]->generateVariables(tempVars);
			for (int j = 0; j< tempVars.size(); j++)
				_vars.push_back(tempVars[j]);
		}
	}
	//--------------------------------------------
	float CSymbolTable::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		int targetLine=_targetLine;
		int lastLeft = _lastLeft;
		int firstXShift=0;
		int crtShift;
	
		int tableStartingLine = _targetLine;
	
		// before doing anything check if the table can get together again
		// if so fix the rows to be not broken
		if (getWidth() < _scrRight - _scrLeft - _indent)
		{
			// check if the Table will fit in this or next line
			if (lastLeft + getWidth() > _scrRight)
			{
				targetLine++;
				lastLeft = _scrLeft + _indent;
				// add the new line to the line list - construct a list of the new lines in the document
				Line * l = new Line(targetLine,0,0,NULL);
				_lines->addLine(l);
				_targetLine = targetLine;
			}
	
			// here the table can get together in targetLine and at lastLeft
			// note that the up and down of the table do not change even if it's broken
			float totalHeights = 0.0;
			for (int i=0; i<noOfRows;i++)
			{
				rows[i]->fixRow(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines,maxColWidths);
				// reverse the shift action to center all the rows in the table
				if (broken)
				{
					rows[i]->shift(0,-1 * (up - (totalHeights + rows[i]->up)) * SYMBOLHI );
					totalHeights +=rows[i]->up + rows[i]->down + ROWSPACING;
	
				}
			}
			_lines->updateLastLineMaxUp_Down(up,down);
			firstXShift = lastLeft - getLeft();
			mybox->shift(firstXShift,0);  // shift the box of the table only because all the elements are aligned on the lastLeft already
			broken = false;
			lastLeft = getRight();
		}
		else // the table is going to be broken
		{
			firstXShift = lastLeft - getLeft();
	
			float totalHeights = 0.0;		// total of ups and downs and spaces before current row
			for (int i =0; i < noOfRows; i++)
			{
				// before breaking the row make the center of the row same as the center of the
				// table so when it's adjusted in the Y direction later it moves the right amount
				if (!broken)
				{
					rows[i]->shift(0,(up - (totalHeights + rows[i]->up)) * SYMBOLHI );
					totalHeights +=rows[i]->up + rows[i]->down + ROWSPACING;
				}
				rows[i]->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
				if (i < noOfRows - 1)
				{
					if (lastLeft + commas[i]->getWidth() > _scrRight)
					{
						// create a new line and put the comma there
						targetLine++;
						lastLeft = _scrLeft + _indent;
						// add the new line to the line list - construct a list of the new lines in the document
						Line * l = new Line(targetLine,0,0,NULL);
						_lines->addLine(l);
						_targetLine = targetLine;
					}
					commas[i]->shift(lastLeft - commas[i]->getLeft(),0);
					commas[i]->setTargetLineNo(targetLine);
					_lines->updateLastLineMaxUp_Down(commas[i]->up,commas[i]->down);
	
					lastLeft = commas[i]->getRight();
				}
			}
			// the heights of the line of the begining of the table and the end of the table
			// need to retain the height of the original table
			Line * l = _lines->getLine(tableStartingLine);
			if (up > l->maxUp)
				l->maxUp = up;
			if (down > l->maxDown)
				l->maxDown = down;
	
			l = _lines->getLine(targetLine);
			if (up > l->maxUp)
				l->maxUp = up;
			if (down > l->maxDown)
				l->maxDown = down;
	
			broken = true;
			// shift the current box to the same relative position to the first child
			mybox->shift(firstXShift,0);
		}
		
		targetLineNo = _targetLine;	// the table line no is the original line
		_lastLeft = lastLeft;
		_targetLine = targetLine;
		return firstXShift;
	}
	//-----------------------------------------------------------
    bool CSymbolTable::isSquareMatrix()
    {
        if (noOfRows == maxNoOfColumns)
            return true;
    }
	//-----------------------------------------------------------    
	expressionType CSymbolTable::getExprType()
	{
        return MULTIEXPR;
	}
	//----------------------------------------------------------
	bool CSymbolTable::hasEllipses()
	{
		for (int i = 0; i < noOfRows; i++)
			if (rows[i]->hasEllipses())
				return true;
		return false;
	}
	//-------------------------------------------------------------
	float CSymbolTable::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = 0;
		for (int i=0; i< noOfRows ; i++)
		{
			float y = rows[i]->adjustLines(_oldLines,_lines);
			if (!i)
			{
				YShift = y;
				mybox->shift(0,YShift*SYMBOLHI);
			}
			if (i < noOfRows - 1)
				commas[i]->adjustLines(_oldLines,_lines);
		}
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//------------------------------------------------------------------------
	void CSymbolTable::setTargetLineNo(int _lineNo)
	{
		for (int i = 0; i < noOfRows; i++)
		{
			rows[i]->setTargetLineNo(_lineNo);
			if (i < noOfRows - 1)
				commas[i]->setTargetLineNo(_lineNo);
		}
	}
	//=========================================================================
	//=========================================================================
	CSymbolTR::CSymbolTR(int _noOfElem ,scg::ExpressionTree * _exprTree)
	{
		noOfElements = _noOfElem;
		elements = (CSymbolRow **) malloc(sizeof(CSymbolRow *) * noOfElements);
		openFence = new CSymbolBasic(L"[",OPERATOR,1);
		closeFence = new CSymbolBasic(L"]",OPERATOR,1);
		commas = (CSymbol **) malloc (sizeof(CSymbol *) * (noOfElements - 1));
		for (int i = 0; i < noOfElements - 1; i++)
		{
			commas[i] = new CSymbolBasic(L",",OPERATOR, 1);
			commas[i]->parentSymbol = this;
		}
		exprTree = _exprTree;
		
		openFence->parentSymbol = this;
		closeFence->parentSymbol = this;
	}
	//--------------------------------------------
	CSymbolTR::~CSymbolTR(void)
	{
		for(int i =0; i< noOfElements; i++)
		{
			delete elements[i];
			if (i < noOfElements -1)
				delete commas[i];
		}
		free(elements);
		free(commas);
		delete openFence;
		delete closeFence;
	}
	//--------------------------------------------
	void CSymbolTR::addChild(int _idx, CSymbol* _symb)
	{
		elements[_idx] = (CSymbolRow *) _symb;
		elements[_idx]->parentSymbol = this;
	}
	//--------------------------------------------
	void CSymbolTR::calcDepth(float l)
	{
		// the up and down of a row is the max of the any of its elements
		up = 0;
		down = 0;
		for (int i = 0 ;i < noOfElements ; i++)
		{
			elements[i]->calcDepth(l);
			up = up >= elements[i]->up ? up : elements[i]->up;  
			down = down >= elements[i]->down ? down : elements[i]->down;  
	
			if (i < noOfElements - 1)
				commas[i]->calcDepth(1);
		}
		openFence->calcDepth(1);
		closeFence->calcDepth(1);
	}
	//------------------------------------------------
	CSymbolBox* CSymbolTR::createBox(int _left, int _center, CDevice * _device)
	{
		CSymbol::createBox(_left,_center,_device);

		// the center for all the elements in the row is the same but the left of each
		// element is different
		int crtLeft = _left;
		for (int i = 0; i < noOfElements; i++)
		{
			CSymbolBox *b = elements[i]->createBox(crtLeft,_center, _device);
			crtLeft = b->right + COLUMNSPACING * SYMBOLHI;
		}
	
		mybox = new CSymbolBox(_left,_center - up*SYMBOLHI,_center + down * SYMBOLHI,elements[noOfElements - 1]->getRight());
	
		for (int i = 0; i < noOfElements - 1; i++)
			commas[i]->createBox(getLeft(),_center,_device);
		openFence->createBox(getLeft(),_center,_device);
		closeFence->createBox(getLeft(),_center,_device);
		return mybox;
	}
	//--------------------------------------------
	void CSymbolTR::display(CDevice * _device, bool _showBoxes)
	{
		for (int i=0; i<noOfElements; i++)
			elements[i]->display(_device,_showBoxes);
	
		if (broken)
		{
			for (int i = 0; i < noOfElements - 1; i++)
				commas[i]->display(_device,_showBoxes);
			openFence->display(_device,_showBoxes);
			closeFence->display(_device,_showBoxes);
		}
	
		//if (_showBoxes)
		//	_pDC->FrameRect(mybox->getRectangle(),&CBrush(RGB(0,0,0)));	
	}
	//-----------------------------------------------
	CSymbol * CSymbolTR::getCrtSymbol(int _x, int _y)
	{
		if (mybox->contains(_x,_y))
		{
			for (int i =0; i < noOfElements; i++)
			{
				CSymbol * crtSymbol = elements[i]->getCrtSymbol(_x,_y);
				if (crtSymbol != NULL)
					return crtSymbol;
			}
			return this;
		}
		else
			return NULL;
	}
	//-------------------------------------------
	CSymbol * CSymbolTR::getCrtSymbol(SCGRECT _rect) 
	{
		if (mybox->intersects(_rect))
			return this;
		else
		{
			for (int i = 0; i < noOfElements; i++)
			{
				CSymbol * crtSymbol = elements[i]->getCrtSymbol(_rect);
				if (crtSymbol != NULL)
					return crtSymbol;
			}
			return NULL;
		}
	}
	//-------------------------------------------
	bool CSymbolTR::selectSymbols(SCGRECT _rect)
	{
		bool somethingSelected = false;

		for (int i = 0; i < noOfElements; i++)
		{
			if (elements[i]->selectSymbols(_rect))
				somethingSelected = true;
		}
		return somethingSelected;
	}
	//-------------------------------------------
	void CSymbolTR::clearSelection()
	{
		for (int i = 0; i < noOfElements; i++)
			elements[i]->clearSelection();
	}
	//-------------------------------------------
	void CSymbolTR::getSelectedSymbols(std::wstring &_ws)
	{
		for (int i = 0; i < noOfElements; i++)
			elements[i]->getSelectedSymbols(_ws);
	}
	//-------------------------------------------
	void CSymbolTR::getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device)
	{
		for (int i = 0; i < noOfElements; i++)
			elements[i]->getSelectedSymbols(_symbols,_device);
	}
	//-------------------------------------------
	std::string CSymbolTR::getSelectedMathML()
	{
		int noOfSelectedElements = 0; // we will not add the <mrow> tags if only one child is selected

		std::string rowStr = "";
		int lastSelectedElement = 0;
		for (int i = 0; i < noOfElements; i++)
		{
			std::string s = "";
			s = elements[i]->getSelectedMathML();
			if (s != "")
			{
				noOfSelectedElements++;
				s = "<mtd>" + s + "</mtd>";
				rowStr += s;
				lastSelectedElement = i;
			}
		}
		
		if (noOfSelectedElements >= 1)
		{
			rowStr = "<mtr>" + rowStr + "</mtr>";
		}
		return rowStr;
	}
	//-------------------------------------------
	void CSymbolTR::shift(int _x,int _y)
	{
		for (int i=0; i < noOfElements; i++)
		{
			elements[i]->shift(_x,_y);
			if (i < noOfElements -1)
				commas[i]->shift(_x,_y);
		}
		openFence->shift(_x,_y);
		closeFence->shift(_x,_y);
	
		mybox->shift(_x,_y);
	}
	//--------------------------------------------
	void CSymbolTR::generateVariables(std::vector<std::wstring> &_vars)
	{
		for (int i = 0; i < noOfElements; i++)
		{
			std::vector<std::wstring> tempVars;
			elements[i]->generateVariables(tempVars);
			for (int j = 0; j< tempVars.size(); j++)
				_vars.push_back(tempVars[j]);
		}
	}
	//--------------------------------------------
	void CSymbolTR::generateVariables(std::vector<CSymbol *> &_vars)
	{
		for (int i = 0; i < noOfElements; i++)
		{
			std::vector<CSymbol *> tempVars;
			elements[i]->generateVariables(tempVars);
			for (int j = 0; j< tempVars.size(); j++)
				_vars.push_back(tempVars[j]);
		}
	}
	//-------------------------------------------
	void CSymbolTR::adjustMaxColWidths(int * _maxColWidths) 
	{
		for (int i =0; i < noOfElements; i++)
			if (elements[i]->getWidth() > _maxColWidths[i])
				_maxColWidths[i] = elements[i]->getWidth();
	}
	//--------------------------------------------
	// curently this function centers the element in the column
	// it returns the position of the right of the row
	int CSymbolTR::alignElementsInCols(int * _maxColWidths)
	{
		int crtColStart = elements[0]->getLeft();
	
		for (int i=0; i < noOfElements; i++)
		{
			int shift = crtColStart - elements[i]->getLeft() + (_maxColWidths[i] - elements[i]->getWidth())/2;
			elements[i]->shift(shift,0);
			crtColStart += _maxColWidths[i] + COLUMNSPACING * SYMBOLHI; 
		}
	
		// adjust the row box to exactly surround the elements inside
		mybox->left = elements[0]->getLeft();
		mybox->right = elements[noOfElements-1]->getRight();
	
		return (mybox->right);
	}
	//----------------------------------------------------------------
	float CSymbolTR::breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines)
	{
		// coming here means the row will be displayed as [e1,e2,....]
		// the row does not have to stay together
		int lastLeft = _lastLeft;
		int targetLine = _targetLine;
	
		if (lastLeft + openFence->getWidth() > _scrRight)
		{
			targetLine++;
			lastLeft = _scrLeft + _indent;
			// add the new line to the line list - construct a list of the new lines in the document
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
			_targetLine = targetLine;
		}
	
		int firstXShift = lastLeft - openFence->getLeft();
		openFence->shift(firstXShift,0);
		openFence->setTargetLineNo(targetLine);
		lastLeft = openFence->getRight();
		_lines->updateLastLineMaxUp_Down(openFence->up,openFence->down);
	
		for (int i = 0; i < noOfElements; i ++)
		{
			if (lastLeft + elements[i]->getWidth() > _scrRight)
				elements[i]->breakLine(_scrLeft,_scrRight,_indent,lastLeft,targetLine,_lines);
			else
			{
				elements[i]->shift(lastLeft - elements[i]->getLeft(),0);
				lastLeft = elements[i]->getRight();
				elements[i]->setTargetLineNo(targetLine);
				_lines->updateLastLineMaxUp_Down(elements[i]->up, elements[i]->down);
			}
	
			if (i<noOfElements-1)
			{
				if (lastLeft + commas[i]->getWidth() > _scrRight)
				{
					targetLine++;
					lastLeft = _scrLeft + _indent;
					// add the new line to the line list - construct a list of the new lines in the document
					Line * l = new Line(targetLine,0,0,NULL);
					_lines->addLine(l);
					_targetLine = targetLine;
				}
				commas[i]->shift(lastLeft - commas[i]->getLeft(),0);
				lastLeft = commas[i]->getRight();
				commas[i]->setTargetLineNo(targetLine);
				_lines->updateLastLineMaxUp_Down(commas[i]->up, commas[i]->down);
			}
		}
	
		if (lastLeft + closeFence->getWidth() > _scrRight)
		{
			targetLine++;
			lastLeft = _scrLeft + _indent;
			// add the new line to the line list - construct a list of the new lines in the document
			Line * l = new Line(targetLine,0,0,NULL);
			_lines->addLine(l);
			_targetLine = targetLine;
		}
	
		closeFence->shift(lastLeft - closeFence->getLeft(),0);
		closeFence->setTargetLineNo(targetLine);
		lastLeft = closeFence->getRight();
		_lines->updateLastLineMaxUp_Down(closeFence->up,closeFence->down);
	
		_lastLeft = lastLeft;
		_targetLine = targetLine;
		broken = true;
	
		return firstXShift;
	}
	//----------------------------------------------------------------
	// this function is called when the table breakline is called and the rows in the Table
	// need to be displayed normally (does not need the [] or the commas
	void CSymbolTR::fixRow(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines, int *_maxColWidths)
	{
		//_lastLeft is the tableLeft
		int lastLeft = _lastLeft;
		int targetLine = _targetLine;
	
		// coming here means the table was broken and now it needs to be fixed
		for (int i =0; i < noOfElements; i++)
			// if the elements were broken they need to be fixed
			elements[i]->breakLine(_scrLeft,_scrRight,_indent, lastLeft, targetLine, _lines);
	
		alignElementsInCols(_maxColWidths);
	
		// shift the row to start relative to the table start
		int firstElemShift = (_maxColWidths[0]-elements[0]->getWidth()) /2;
		shift(_lastLeft + firstElemShift - getLeft(),0);
		setTargetLineNo(_targetLine);
		broken = false;
	}
	//------------------------------------------------------------
	float CSymbolTR::adjustLines(CLineList *_oldLines, CLineList *_lines)
	{
		float YShift = 0;
		for (int i=0; i< noOfElements ; i++)
		{
			float y = elements[i]->adjustLines(_oldLines,_lines);
			if (!i)
			{
				YShift = y;
				mybox->shift(0,YShift*SYMBOLHI);
			}
			if (i < noOfElements - 1)
				commas[i]->adjustLines(_oldLines,_lines);
		}
		openFence->adjustLines(_oldLines,_lines);
		closeFence->adjustLines(_oldLines, _lines);
	
		crtLineNo = targetLineNo;
        
		return YShift;
	}
	//----------------------------------------------------------------
	void CSymbolTR::setTargetLineNo(int _lineNo)
	{
		for (int i = 0; i < noOfElements; i++)
		{
			elements[i]->setTargetLineNo(_lineNo);
			if (i < noOfElements - 1)
				commas[i]->setTargetLineNo(_lineNo);
		}
		openFence->setTargetLineNo(_lineNo);
		closeFence->setTargetLineNo(_lineNo);
	}
	//---------------------------------------------------------
	bool CSymbolTR::hasEllipses() 
	{
		for (int i =0; i < noOfElements; i++)
			if (elements[i]->hasEllipses())
				return true;
		return false;
	}
}

