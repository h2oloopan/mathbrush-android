#include "Functs.h"

namespace SCGRenderer
{
	OpDictHashType opDict(ITEM_NOT_FOUND ,37);
	SymbDictHashType symbDict(ITEM_NOT_FOUND ,37);
	int SYMBOLHI;
	//CFont * renderingFont;
	int FontAscent;

	void fillOperatorDict()
	{
		// construct the hash template for the dictionary data

		// fill in the data
		int currentPos;

		currentPos = opDict.insert("(",OperatorDictData(false,true,PREFIX,NOSPACE,THINMATHSPACE,L"(",false));
		currentPos = opDict.insert(")",OperatorDictData(false,true,POSTFIX,THICKMATHSPACE,NOSPACE,L")",false));
		currentPos = opDict.insert("[",OperatorDictData(false,true,PREFIX,THINMATHSPACE,THINMATHSPACE,L"[",false));
		currentPos = opDict.insert("]",OperatorDictData(false,true,POSTFIX,THICKMATHSPACE,THINMATHSPACE,L"]",false));
		currentPos = opDict.insert("{",OperatorDictData(false,true,PREFIX,NOSPACE,THINMATHSPACE,L"{",false));
		currentPos = opDict.insert("}",OperatorDictData(false,true,POSTFIX,THICKMATHSPACE,NOSPACE,L"}",false));
		currentPos = opDict.insert("=",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L"=",false));
		currentPos = opDict.insert("&lt;",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L"<",false));
		currentPos = opDict.insert(">",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L">",false));
		currentPos = opDict.insert("!=",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L"!=",false));
		currentPos = opDict.insert(":=",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L":=",false));
		currentPos = opDict.insert("==",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L"==",false));
		currentPos = opDict.insert("&lt;=",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L"<=",false));
		currentPos = opDict.insert(">=",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L">=",false));
		currentPos = opDict.insert("+",OperatorDictData(false,false,INFIX,MEDIUMMATHSPACE,MEDIUMMATHSPACE,L"+",false));
		currentPos = opDict.insert("-",OperatorDictData(false,false,INFIX,MEDIUMMATHSPACE,MEDIUMMATHSPACE,L"-",false));
		currentPos = opDict.insert("*",OperatorDictData(false,false,INFIX,THINMATHSPACE,THINMATHSPACE,L"*",false));
		currentPos = opDict.insert("&plusminus;",OperatorDictData(false,false,INFIX,THINMATHSPACE,THINMATHSPACE,L"\u00B1",false));        
		currentPos = opDict.insert("&invisibletimes;",OperatorDictData(false,false,INFIX,THINMATHSPACE,THINMATHSPACE ,L"",false));
		currentPos = opDict.insert("/",OperatorDictData(false,true,INFIX,THINMATHSPACE,THINMATHSPACE,L"/",false));
		currentPos = opDict.insert("&applyfunction;",OperatorDictData(false,false,INFIX,NOSPACE,NOSPACE,L"",false));
		currentPos = opDict.insert("&rightarrow;",OperatorDictData(false,false,INFIX,THICKMATHSPACE,THICKMATHSPACE,L"->",false));
		currentPos = opDict.insert("&integral;",OperatorDictData(false,true,PREFIX,NOSPACE,NOSPACE,L"\u222B",true));
		currentPos = opDict.insert("&differentiald;",OperatorDictData(false,false,PREFIX,THICKMATHSPACE,VERYTHINMATHSPACE,L"d",false));
		currentPos = opDict.insert("&sum;",OperatorDictData(false,false,PREFIX,NOSPACE,THINMATHSPACE,L"\u03A3",true));
		currentPos = opDict.insert("&product;",OperatorDictData(false,false,PREFIX,NOSPACE,THINMATHSPACE,L"\u03A0",true));
		currentPos = opDict.insert(";",OperatorDictData(true,false,INFIX,VERYTHINMATHSPACE,THICKMATHSPACE,L";",false));
		currentPos = opDict.insert(",",OperatorDictData(true,false,INFIX,VERYTHINMATHSPACE,THICKMATHSPACE,L",",false));
		currentPos = opDict.insert("!",OperatorDictData(false,false,POSTFIX,VERYTHINMATHSPACE,NOSPACE,L"!",false));
		currentPos = opDict.insert("^",OperatorDictData(false,false,INFIX,VERYTHINMATHSPACE,VERYTHINMATHSPACE,L"^",false));
		currentPos = opDict.insert("|",OperatorDictData(true,true,INFIX,THICKMATHSPACE,THICKMATHSPACE,L"|",true));
		currentPos = opDict.insert("&fracbar;",OperatorDictData(false,false,INFIX,NOSPACE,NOSPACE, L"-",false)); // this is an operator I added for the fraction bar to be a separate symbol. needed for rendering
		currentPos = opDict.insert("&sqrt;",OperatorDictData(false,false,INFIX,NOSPACE,NOSPACE, L"",true)); // need to have the square root as a symbols to be selected and drawn separately.
		currentPos = opDict.insert("&prime;",OperatorDictData(false,false,INFIX,VERYTHINMATHSPACE,VERYTHINMATHSPACE, L"\u2032",false));  // single prime
		currentPos = opDict.insert("&Prime;",OperatorDictData(false,false,INFIX,VERYTHINMATHSPACE,VERYTHINMATHSPACE, L"\u2033",false));  // double prime
        currentPos = opDict.insert("&tprime;",OperatorDictData(false,false,INFIX,VERYTHINMATHSPACE,VERYTHINMATHSPACE, L"\u2034",false));  // triple prime
		currentPos = opDict.insert("&vellip;",OperatorDictData(false, false, INFIX, VERYTHINMATHSPACE, VERYTHINMATHSPACE ,L"\u22EE", false));
		currentPos = opDict.insert("&ctdot;",OperatorDictData(false, false, INFIX, VERYTHINMATHSPACE, VERYTHINMATHSPACE ,L"\u22EF", false));
		currentPos = opDict.insert("&utdot;",OperatorDictData(false, false, INFIX, VERYTHINMATHSPACE, VERYTHINMATHSPACE ,L"\u22F0", false));
		currentPos = opDict.insert("&dtdot;",OperatorDictData(false, false, INFIX, VERYTHINMATHSPACE, VERYTHINMATHSPACE ,L"\u22F1", false));
	}

	void fillSymbolsDict()
	{
		int currentPos;

		currentPos = symbDict.insert("&alpha;",SymbolsDictData(THINMATHSPACE,L"\u03B1"));
		currentPos = symbDict.insert("&beta;",SymbolsDictData(THINMATHSPACE,L"\u03B2"));
		currentPos = symbDict.insert("&gamma;",SymbolsDictData(THINMATHSPACE,L"\u03B3"));
		currentPos = symbDict.insert("&delta;",SymbolsDictData(THINMATHSPACE,L"\u03B4"));
		currentPos = symbDict.insert("&epsilon;",SymbolsDictData(THINMATHSPACE,L"\u03B5"));
		currentPos = symbDict.insert("&epsiv;",SymbolsDictData(THINMATHSPACE,L"\u03B5")); // Maple generates this instead of epsilon ??
		currentPos = symbDict.insert("&zeta;",SymbolsDictData(THINMATHSPACE,L"\u03B6"));
		currentPos = symbDict.insert("&eta;",SymbolsDictData(THINMATHSPACE,L"\u03B7"));
		currentPos = symbDict.insert("&theta;",SymbolsDictData(THINMATHSPACE,L"\u03B8"));
		currentPos = symbDict.insert("&iota;",SymbolsDictData(THINMATHSPACE,L"\u03B9"));
		currentPos = symbDict.insert("&kappa;",SymbolsDictData(THINMATHSPACE,L"\u03BA"));
		currentPos = symbDict.insert("&lambda;",SymbolsDictData(THINMATHSPACE,L"\u03BB"));
		currentPos = symbDict.insert("&mu;",SymbolsDictData(THINMATHSPACE,L"\u03BC"));
		currentPos = symbDict.insert("&nu;",SymbolsDictData(THINMATHSPACE,L"\u03BD"));
		currentPos = symbDict.insert("&xi;",SymbolsDictData(THINMATHSPACE,L"\u03BE"));
		currentPos = symbDict.insert("&omicron;",SymbolsDictData(NOSPACE,L"\u03BF"));
		currentPos = symbDict.insert("&pi;",SymbolsDictData(THINMATHSPACE,L"\u03C0"));
		currentPos = symbDict.insert("&rho;",SymbolsDictData(THINMATHSPACE,L"\u03C1"));
		currentPos = symbDict.insert("&sigma;",SymbolsDictData(THINMATHSPACE,L"\u03C3"));
		currentPos = symbDict.insert("&tau;",SymbolsDictData(THINMATHSPACE,L"\u03C4"));
		currentPos = symbDict.insert("&upsilon;",SymbolsDictData(THINMATHSPACE,L"\u03C5"));
		currentPos = symbDict.insert("&phi;",SymbolsDictData(THINMATHSPACE,L"\u03C6"));
		currentPos = symbDict.insert("&chi;",SymbolsDictData(THINMATHSPACE,L"\u03C7"));
		currentPos = symbDict.insert("&psi;",SymbolsDictData(THINMATHSPACE,L"\u03C8"));
		currentPos = symbDict.insert("&omega;",SymbolsDictData(THINMATHSPACE,L"\u03C9"));
		currentPos = symbDict.insert("&Alpha;",SymbolsDictData(NOSPACE,L"\u0391"));
		currentPos = symbDict.insert("&Beta;",SymbolsDictData(NOSPACE,L"\u0392"));
		currentPos = symbDict.insert("&Gamma;",SymbolsDictData(THINMATHSPACE,L"\u0393"));
		currentPos = symbDict.insert("&Delta;",SymbolsDictData(THINMATHSPACE,L"\u0394"));
		currentPos = symbDict.insert("&Epsilon;",SymbolsDictData(NOSPACE,L"\u0395"));
		currentPos = symbDict.insert("&Zeta;",SymbolsDictData(NOSPACE,L"\u0396"));
		currentPos = symbDict.insert("&Eta;",SymbolsDictData(NOSPACE,L"\u0397"));
		currentPos = symbDict.insert("&Theta;",SymbolsDictData(THINMATHSPACE,L"\u0398"));
		currentPos = symbDict.insert("&Iota;",SymbolsDictData(NOSPACE,L"\u0399"));
		currentPos = symbDict.insert("&Kappa;",SymbolsDictData(NOSPACE,L"\u039A"));
		currentPos = symbDict.insert("&Lambda;",SymbolsDictData(THINMATHSPACE,L"\u039B"));
		currentPos = symbDict.insert("&Mu;",SymbolsDictData(NOSPACE,L"\u039C"));
		currentPos = symbDict.insert("&Nu;",SymbolsDictData(NOSPACE,L"\u039D"));
		currentPos = symbDict.insert("&Xi;",SymbolsDictData(THINMATHSPACE,L"\u039E"));
		currentPos = symbDict.insert("&Omicron;",SymbolsDictData(NOSPACE,L"\u039F"));
		currentPos = symbDict.insert("&Pi;",SymbolsDictData(THINMATHSPACE,L"\u03A0"));
		currentPos = symbDict.insert("&Rho;",SymbolsDictData(NOSPACE,L"\u03A1"));
		currentPos = symbDict.insert("&Sigma;",SymbolsDictData(THINMATHSPACE,L"\u03A3"));
		currentPos = symbDict.insert("&Tau;",SymbolsDictData(NOSPACE,L"\u03A4"));
		currentPos = symbDict.insert("&Upsilon;",SymbolsDictData(NOSPACE,L"\u03A5"));
		currentPos = symbDict.insert("&Phi;",SymbolsDictData(THINMATHSPACE,L"\u03A6"));
		currentPos = symbDict.insert("&Chi;",SymbolsDictData(NOSPACE,L"\u03A7"));
		currentPos = symbDict.insert("&Psi;",SymbolsDictData(THINMATHSPACE,L"\u03A8"));
		currentPos = symbDict.insert("&Omega;",SymbolsDictData(THINMATHSPACE,L"\u03A9"));
		currentPos = symbDict.insert("&infin;",SymbolsDictData(THINMATHSPACE,L"\u221E"));
		currentPos = symbDict.insert("&ExponentialE;",SymbolsDictData(NOSPACE,L"e"));
		currentPos = symbDict.insert("&ImaginaryI;",SymbolsDictData(NOSPACE,L"i"));
		currentPos = symbDict.insert("&empty;",SymbolsDictData(NOSPACE,L" "));
		currentPos = symbDict.insert("&hellip;",SymbolsDictData(NOSPACE,L"\u22EF"));
		currentPos = symbDict.insert("&vellip;",SymbolsDictData(NOSPACE,L"\u22EE"));
		currentPos = symbDict.insert("&ctdot;",SymbolsDictData(NOSPACE,L"\u22EF"));
		currentPos = symbDict.insert("&utdot;",SymbolsDictData(NOSPACE,L"\u22F0"));
		currentPos = symbDict.insert("&dtdot;",SymbolsDictData(NOSPACE,L"\u22F1"));
	}

	void loadDictionaries()
	{
		fillOperatorDict();
		fillSymbolsDict();
	}

	void updateFontHeight(int _fontHeight)
	{
		SYMBOLHI = _fontHeight;
		/* DeleteObject(renderingFont);
		renderingFont = CreateFont(
			SYMBOLHI,                  // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_fontName.c_str());       // lpszFacename
		 */	
	}

	void unloadDictionaries()
	{
		//DeleteObject(renderingFont);
	}
}