#ifndef TYPES_H_
#define TYPES_H_

#define NOMINMAX

#include <string>
#include "hashTable.h"
#include "rect.h"
#include "symbols.h"  // only for the unicode_char definition I will change that later

namespace SCGRenderer
{
	/////////////////////////////////////
	
	enum tokenType {OPERATOR = 0, IDENTIFIER = 1, NUMBER = 2, NONTOKEN = 3, NONE = 4, PRESCRIPT = 5,EXTERNAL_IDENTIFIER = 6};

	enum spaceType {NOSPACE = 0, VERYTHINMATHSPACE = 1, THINMATHSPACE = 2, MEDIUMMATHSPACE = 4, THICKMATHSPACE = 6}; // the type of the spaces around operators

	enum errorCodes {ERRORFREE, CREATERECO_ERR, NULLEXPR_ERR, EXPRTYPE_ERR, DISPLAY_ERR,ADDSTROKES_ERR, REMOVESTROKES_ERR, UPDATEPARSETABLE_ERR, NULLITERATOR_ERR, CREATEINK_ERR, MATRIXCONSTRUCTION_ERR};

	enum formType {PREFIX, INFIX, POSTFIX }; // the form of the operator

	enum expressionType {NULLTYPE, EQUAL, PLUS, TIMES, POWER, SET, CURLYBRACE, LIST, SQUAREBRACKET, FUNCTION, FRACTION, NUMERIC, MATRIX, SQUAREMATRIX,MULTIEXPR}; // this is public so it can be seen in the interface
	
	// this has to be in the same order as the expressionType enumeration elements
	const std::string expressionTypeStr[15] = {"NULLTYPE", "EQUAL", "PLUS", "TIMES", "POWER", "SET", "CURLYBRACE", "LIST", "SQUAREBRACKET", "FUNCTION", "FRACTION", "NUMERIC", "MATRIX", "SQUAREMATRIX","MULTIEXPR"}; // this is public so it can be seen in the interface

	enum TEXT_COLOR {SELECTEDTEXTCOLOR, REGULARTEXTCOLOR};

	enum PEN_COLOR {SELECTEDPENCOLOR, REGULARPENCOLOR};
	
	enum BACK_COLOR {HIGHLIGHTBACKCOLOR, REGULARBACKCOLOR};

	/////////////////////////////////////
	
	const int ITEM_NOT_FOUND  = -1;

	#define HIGHLIGHT 13
	#define ROWSPACING 0.5
	#define COLUMNSPACING 1.0


	const float INTERSECTION_RATIO = 0.3f; // the intersection percentage to consider the rectangle highlighted 

	const int MYBACKHILIGHT_R = 255;
	const int MYBACKHILIGHT_G = 255;
	const int MYBACKHILIGHT_B = 0;

	const int MYBACKCOLOR_R = 255;
	const int MYBACKCOLOR_G = 255;
	const int MYBACKCOLOR_B = 255;

	const int MYTEXTCOLOR_R = 0;
	const int MYTEXTCOLOR_G = 0;
	const int MYTEXTCOLOR_B = 0;

	const int MYSELECTEDTEXTCOLOR_R = 255;
	const int MYSELECTEDTEXTCOLOR_G = 0;
	const int MYSELECTEDTEXTCOLOR_B = 0;

	const std::string UNKNOWNSELECTION = "<mi>unknown</mi>";

	/////////////////////////////////////

	struct OperatorDictData
	{
		bool separator;
		bool stretch;
		formType form;
		spaceType lspace;
		spaceType rspace;
		std::wstring visual;  // the way to render the symbol
		bool largeOp;   // if the operator is of large size

		OperatorDictData(bool sep = false, bool s = false, formType f2 = PREFIX, spaceType ls = NOSPACE, spaceType rs = NOSPACE, std::wstring v = L"mm", bool lop = false)
			: separator(sep),stretch(s),form(f2),lspace(ls), rspace(rs), visual (v), largeOp(lop){}
	};

	struct SymbolsDictData
	{
		int lspace;
		std::wstring unicode;
		SymbolsDictData(int ls=0,std::wstring _unicode=L"")
			: lspace(ls), unicode (_unicode) {}
	};

	//////////////////////////////////////////////////
	
	typedef CHashTable<char*, OperatorDictData> OpDictHashType;

	typedef CHashTable<char*, SymbolsDictData> SymbDictHashType;
	
	typedef scg::Rect<float> SCGRECT;
}

#endif
