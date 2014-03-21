#ifndef SYMBOL_H_
#define SYMBOL_H_
 
#define NOMINMAX
#include "Lines.h"
#include "SymbolBox.h"
#include "MathRecognizer.h"
#include "CDevice.h"

namespace SCGRenderer
{
	int round (double value);

	////======================================================================
	class CSymbol
	{
	public:
		SCGRenderer::tokenType type;
		int lspace;
		int rspace;
		bool stretch;
		bool largeOp;
		int opForm;	// the form of the operator (in case of operator) postfix, prefix, infix
		int crtLineNo;   // the line no where the symbol is displayed (start at 0)
		int targetLineNo;  // the line no where the symbol will be moved to
		bool broken;  // true if this symbol is broken because of line breaking (start false)
		float up;   // ratio of symbol height above baseline
		float down;	// ratio of symbol height below baseline
		bool separator;	// when breaking a line the separator has to stay with the end of last line
		bool selected; // true if the symbol is selected in interface. This symbol should be displayed in different colors
		CSymbol * parentSymbol;
	private:
		int height; // this is set only if the expression is broken. See getHeight method
		int width; // this is set only if the expression is broken. See getWidth method
		int lineAdjustment; // when the expression is broken the boxes sometimes have a negative value in the top. this value needs to be used to modify the moveto function
        
	protected:
		CSymbolBox * mybox;
		scg::ExpressionTree * exprTree;

	public:
		CSymbol(void);
		virtual ~CSymbol(void);
		virtual void addChild(int,CSymbol *){};
		virtual void calcDepth(float);
		virtual CSymbolBox* createBox(int,int,CDevice *);
		virtual void display(CDevice* ,bool);
		virtual SCGRECT getRectangle();
		virtual CSymbol * getCrtSymbol(int _x, int _y);
		virtual CSymbol * getCrtSymbol(SCGRECT _rect);
		virtual bool selectSymbols(SCGRECT _rect); // selects the symbols inside the rectangle provided or deselect them if already selected. returns true if something is selected
		virtual void clearSelection(){selected = false;}; // clears the selection from the expression
		virtual void getSelectedSymbols(std::wstring &_ws){};  // add to the string the symbol and it's box
		virtual void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device){}; // add to the list of symbols the symbol and its box
		virtual std::string getSelectedMathML(){return "";}; // generate MathML for the selection
		void setTextColor(CDevice* _device);
		void setPenColor(CDevice* _device);
		void setBackColor(CDevice* _device, bool _highlight);
		scg::ExpressionTree * getExprTree(){return exprTree;}; 
		virtual void moveTo(int _x, int _y);
		virtual void shift(int,int);
		void stretchBox(int,int,int,int);
		void stretchBoxH(int,int);
		virtual void stretchBoxV(int,int);
		int getTop();
		int getBottom();
		int getLeft();
		int getRight();
		int getHeight();
		int getWidth();
		void setHeightWidth (int _h, int _w);
		bool contains(int _x, int _y);
		virtual expressionType getExprType(){return NULLTYPE;};
		std::string getExprTypeName();
		virtual void generateVariables(std::vector<std::wstring> & _vars){};
        virtual void generateVariables(std::vector<CSymbol *> & _vars){};
		virtual bool isApplyFunction(){return false;}; // overriden by only CSymbol basic to return true if the operator is apply function
		virtual void reform(int _shortTerms){}; // currently only CSymbolRow overrides it to add dots in a series
		virtual float breakLine(int,int,int,int &, int &, CLineList *); // return the shift in the x direction
		virtual float adjustLines(CLineList *, CLineList *);
		virtual void setTargetLineNo(int);
		void setBox(int,int,int,int);
		virtual bool isTxt(wchar_t *){return false;};
        virtual wchar_t * getTxt(){return NULL;};
		void adjustBoxes(float _newUp, float _newDown);
		virtual void generateFunctionsNames(std::vector<std::wstring> &_fnNames){}; // in the classes that might have apply function mainly mrow and mfenced if a child is apply function then the name of the function will be constructed and added to the list
		virtual bool hasEllipses(){return false;};
		std::string getLongMathML(){if (exprTree != NULL) return "<math xmlns='http://www.w3.org/1998/Math/MathML'><mi>jesus</mi></math>"; else return "<math xmlns='http://www.w3.org/1998/Math/MathML'><mi>abou_seifein</mi></math>";};
	};
	//--------------------------------------------------------------------------
	class CSymbolBasic:public CSymbol
	{
	public:
		/* In case of Operator txt contains the character to be displayed. In case of
		external identifiers contains the txt to be displayed with the specified font.
		In all other cases contain the txt to be displayed with the default font */
		wchar_t *txt;
		/* originalTxt contains the unicode value of the character in case of external
		identifiers. It contains the text in the mathML of an operator in the case
		of operators and NULL otherwise*/
		char *originalTxt;
		
	private:
		int noOfChildren;
		CSymbolBasic **children; // needed only if the basic symbol is broken for example a very wide number or identifier name

	public:
		CSymbolBasic(const wchar_t *,SCGRenderer::tokenType,int, scg::ExpressionTree * = NULL);
		~CSymbolBasic(void);
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice *);
		void display(CDevice * ,bool);
		CSymbol * getCrtSymbol(int _x, int _y);
		CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		expressionType getExprType();
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		bool isApplyFunction(); // used to avoid generating functions as identifiers
		float breakLine(int,int,int,int &, int &, CLineList *); // return the shift in the x direction
		float adjustLines(CLineList *, CLineList *);
		bool isTxt(wchar_t *);
        wchar_t * getTxt(){return txt;};
		bool hasEllipses();
	};
	//--------------------------------------------------------------------------
	class CSymbolFrac:public CSymbol 
	{
	private:
		CSymbol *numerator;
		CSymbol *denominator;
		CSymbol *fracBar; // added to support independence of the fraction bar processing

		// the following are used when broken to draw fences and / sign
		CSymbol *numeratorOpenFence;
		CSymbol *numeratorCloseFence;
		CSymbol *denominatorOpenFence;
		CSymbol *denominatorCloseFence;
		CSymbol *divSign;
		CSymbol *one;
	
	public:
		CSymbolFrac(CSymbol*, CSymbol*,scg::ExpressionTree * = NULL);
		~CSymbolFrac();
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice * _device);
		void display(CDevice *, bool);
		virtual CSymbol * getCrtSymbol(int _x, int _y);
		virtual CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		expressionType getExprType(){return FRACTION;};
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &, int &, CLineList *); 
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		void generateFunctionsNames(std::vector<std::wstring> &_fnNames); 
		bool hasEllipses();
	};
	//--------------------------------------------------------------------------
	class CSymbolRow:public CSymbol 
	{
	protected:
		int noOfChildren;
		CSymbol **children;

	public:
		CSymbolRow(void);
		CSymbolRow(int,scg::ExpressionTree * = NULL);
		~CSymbolRow();

		void addChild(int,CSymbol *);
		virtual void calcDepth(float);
		virtual CSymbolBox* createBox(int,int,CDevice *);
		virtual void display(CDevice *, bool); 
		virtual CSymbol * getCrtSymbol(int _x, int _y);
		virtual CSymbol * getCrtSymbol(SCGRECT _rect);
		virtual bool selectSymbols(SCGRECT _rect);
		virtual void clearSelection();
		virtual void getSelectedSymbols(std::wstring &_ws);
		virtual void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		virtual std::string getSelectedMathML();
		virtual void shift(int,int);
		virtual expressionType getExprType();
		virtual void generateVariables(std::vector<std::wstring> & _vars);
        virtual void generateVariables(std::vector<CSymbol *> & _vars);
		void reform(int _shortTerms); 
		float breakLine(int,int,int,int &, int &, CLineList *);
		virtual float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		virtual void generateFunctionsNames(std::vector<std::wstring> &_fnNames);
		virtual bool hasEllipses();
	};
	//--------------------------------------------------------------------------
	class CSymbolFence:public CSymbolRow 
	{
	private:
		CSymbol* openFence;
		CSymbol* closeFence;

		void assignSeparators(std::wstring);
	
	public:
		CSymbolFence(void);
		CSymbolFence(int,wchar_t*,wchar_t*,std::wstring,scg::ExpressionTree * _exprTree=NULL);
		~CSymbolFence();
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice *);
		void display(CDevice *, bool);
		CSymbol * getCrtSymbol(int _x, int _y);
		CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		expressionType getExprType();
		float breakLine(int,int,int,int &, int &, CLineList *);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
	};
	//--------------------------------------------------------------------------
	class CSymbolSqrt:public CSymbol 
	{
	private:
		CSymbol* elem;
		CSymbol* sqrt; // used when the sqrt is broken
	public:
		CSymbolSqrt(void);
		~CSymbolSqrt();
		CSymbolSqrt(CSymbol *,scg::ExpressionTree * _exprTree=NULL);
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice *);
		void display(CDevice *, bool);
		CSymbol * getCrtSymbol(int _x, int _y);
		CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		expressionType getExprType() {return POWER;};
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &,int &, CLineList*);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		bool hasEllipses();
	};
	//--------------------------------------------------------------------------
	class CSymbolUnderOver:public CSymbol 
	{
	private:
		CSymbol *base;
		CSymbol *under;
		CSymbol *over;
		// for now we will assume the attribute accentunder is false as default
		// so under is reduced in size and not so close to the base
	public:
		CSymbolUnderOver(CSymbol* _base, CSymbol* _under, CSymbol* _over, scg::ExpressionTree * _exprTree = NULL);
		~CSymbolUnderOver();
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice *);
		void display(CDevice *, bool);
		CSymbol * getCrtSymbol(int _x, int _y);
		CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		void stretchBoxV(int,int);
		expressionType getExprType(){return base->getExprType();}
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &, int &, CLineList *);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		bool hasEllipses();
	};
	//--------------------------------------------------------------------------
	class CSymbolSup:public CSymbol 
	{
	private:
		CSymbol *base;
		CSymbol *exp;
		CSymbol *hat;  // in case the Sup is broken
	public:
		CSymbolSup(CSymbol*, CSymbol*, scg::ExpressionTree * = NULL);
		~CSymbolSup();
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice * _device);
		void display(CDevice *, bool);
		CSymbol * getCrtSymbol(int _x, int _y);
		CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		void stretchBoxV(int,int);
		expressionType getExprType(){return POWER;};
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &, int &, CLineList *);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		bool hasEllipses();
	};
	//-------------------------------------------------------------------------------------
	class CSymbolSub:public CSymbol 
	{
	private:
		CSymbol *base;
		CSymbol *sub;
	public:
		CSymbolSub(CSymbol*, CSymbol*, scg::ExpressionTree * = NULL);
		~CSymbolSub();
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice * _device);
		void display(CDevice *, bool);
		CSymbol * getCrtSymbol(int _x, int _y);
		CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		void stretchBoxV(int,int);
		expressionType getExprType(){return base->getExprType();}
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &, int &, CLineList *);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		bool hasEllipses();
	};
	//======================================================================================
	// THE FOLLOWING ARE ONLY SUPPORTED FOR MATHML INPUT NOT FROM RECOGNIZER
	//--------------------------------------------------------------------------
	class CSymbolSubSup:public CSymbol 
	{
	private:
		CSymbol *base;
		CSymbol *sub;
		CSymbol *exp;
	public:
		CSymbolSubSup(CSymbol *, CSymbol *, CSymbol *, scg::ExpressionTree * = NULL);
		~CSymbolSubSup();
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice * _device);
		void display(CDevice *, bool);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		void stretchBoxV(int,int);
		expressionType getExprType() {return POWER;};
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &, int &, CLineList *);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		bool hasEllipses();
	};
	//--------------------------------------------------------------------------
	class CSymbolUnder:public CSymbol 
	{
	private:
		CSymbol *base;
		CSymbol *under;
		// for now we will assume the attribute accentunder is false as default
		// so under is reduced in size and not so close to the base
	public:
		CSymbolUnder(CSymbol*, CSymbol*, scg::ExpressionTree * = NULL);
		~CSymbolUnder();
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice * _device);
		void display(CDevice *, bool);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		void stretchBoxV(int,int);
		expressionType getExprType(){return base->getExprType();}
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &, int &, CLineList *);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		bool hasEllipses();
	};
	//--------------------------------------------------------------------------
	class CSymbolOver:public CSymbol 
	{
	private:
		CSymbol *base;
		CSymbol *over;
		// for now we will assume the attribute accentunder is false as default
		// so under is reduced in size and not so close to the base
	public:
		CSymbolOver(CSymbol*, CSymbol*, scg::ExpressionTree * = NULL);
		~CSymbolOver();
		void calcDepth(float);
		CSymbolBox* createBox(int,int,CDevice * _device);
		void display(CDevice *, bool);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int,int);
		void stretchBoxV(int,int);
		expressionType getExprType(){return base->getExprType();}
		void generateVariables(std::vector<std::wstring> & _vars);	
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &, int &, CLineList *);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		bool hasEllipses();
	};
	//-------------------------------------------------------------------------------
	class CSymbolMultiScript:public CSymbol 
	{
	private:
		int noOfChildren;
		CSymbol **children;
		int preScriptIdx; /* if no prescripts this is set to 0 otherwise it has the 
						  index of the prescript element (it has to be an odd number)
						  because the scripts go in pairs with <none/> for skipped ones */
	public:
		CSymbolMultiScript(int,scg::ExpressionTree * _exprTree = NULL);
		~CSymbolMultiScript();
		virtual void calcDepth(float);
		virtual CSymbolBox* createBox(int,int,CDevice * _device);
		virtual void display(CDevice *, bool); 
		bool selectSymbols(SCGRECT _rect);
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		virtual void shift(int,int);
		void stretchBoxV(int,int);
		void addChild(int,CSymbol *);
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int,int,int,int &, int &, CLineList *);
		float adjustLines(CLineList *, CLineList *);
		void setTargetLineNo(int);
		bool hasEllipses();
	};
	//------------------------------------------------------------------------------
	// mtr
	class CSymbolTR:public CSymbol 
	{
	private:
		int noOfElements; 
		CSymbolRow ** elements;
		// broken for the row means that the row is displayed [e1,e2,...en] even if it all fits in one line
		CSymbol **commas; // the commas between the elements in the rows displayed when the table is broken (and hence the rwo is broken even if the whole row fits in one line)( no of commas = noOfElements-1)
		CSymbol *openFence; // the [ before the row when it's broken
		CSymbol *closeFence;	// the ] after the row when it's broken
	
	public:
		CSymbolTR(int _noElem, scg::ExpressionTree * = NULL);
		~CSymbolTR();
		void addChild(int,CSymbol*);
		int getNoOfColumns(){return noOfElements;};
		void calcDepth(float);
		CSymbolBox* createBox(int _left, int _center, CDevice * _device);
		void adjustMaxColWidths(int * _colWidth);
		int alignElementsInCols(int * _maxColWidths);
		void display(CDevice * _device, bool _showBoxes);
		CSymbol * getCrtSymbol(int _x, int _y);
		CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int _x, int _y);
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines);
		void fixRow(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines, int *_maxColWidths);
		float adjustLines(CLineList *_oldLines, CLineList *_lines);
		void setTargetLineNo(int _lineNo);
		bool hasEllipses();
	};

	// mtable 
	class CSymbolTable:public CSymbol 
	{
	private:
		int noOfRows;
		int* noOfColumns;	// contain the number of columns in each row
		CSymbolTR **rows;
		int maxNoOfColumns; // the max no of columns in the table
		int *maxColWidths;	// the max widths of the columns in the table (no of elements in the array = maxNoOfColumns)
		CSymbol **commas;	// array of commas that separate the rows when the table is broken (the number of elements in this array is numberOfRows - 1)
	
	public:
		CSymbolTable(int _noRows, scg::ExpressionTree * = NULL);
		~CSymbolTable();
		void addChild(int,CSymbol*);
		void calcDepth(float);
		CSymbolBox* createBox(int _left, int _center, CDevice * _device);
		void display(CDevice * _device, bool _showBoxes);
		CSymbol * getCrtSymbol(int _x, int _y);
		CSymbol * getCrtSymbol(SCGRECT _rect);
		bool selectSymbols(SCGRECT _rect);
		void clearSelection();
		void getSelectedSymbols(std::wstring &_ws);
		void getSelectedSymbols(std::vector<scg::MathSymbol> &_symbols, CDevice * _device);
		std::string getSelectedMathML();
		void shift(int _x, int _y);
        bool isSquareMatrix();
		expressionType getExprType();
		void generateVariables(std::vector<std::wstring> & _vars);
        void generateVariables(std::vector<CSymbol *> & _vars);
		float breakLine(int _scrLeft,int _scrRight, int _indent, int &_lastLeft, int &_targetLine, CLineList * _lines);
		float adjustLines(CLineList *_oldLines, CLineList *_lines);
		void setTargetLineNo(int _lineNo);
		bool hasEllipses();
	};
}


#endif