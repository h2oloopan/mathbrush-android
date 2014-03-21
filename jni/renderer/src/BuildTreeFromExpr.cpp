#include "Functs.h"
#include "Lines.h"

using namespace std; // to allow extern for a pointer ??
//#include <iostream>
//#include "expr-node.h"

namespace SCGRenderer
{
	//extern errorCodes recognitionError;
	//extern std::string parseErrorStr;
	errorCodes recognitionError;
	std::string parseErrorStr;	
	extern int SYMBOLHI;
	//extern CFont* renderingFont;

	//=============================================================================
	// PARSING THE EXPRESSION TREE FROM THE RECOGNIZER
	CSymbol * parseExprTree(scg::ExpressionTree *_exprTree, bool _setExprTree = true)
	{
		// element nodes are those that have either text inside them or other elements
		// the document has to have no white spaces otherwise they are counted as nodes
		CSymbol * dispTree;
		
        scg::SemanticId exprType = _exprTree->type();
        
		switch (exprType)
		{
            // Binary operators
			case scg::ADD_EXPR:
			case scg::MULT_EXPR:
			case scg::REL_EXPR:
			{
				CSymbol * lh = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::EXPR_LHS),_setExprTree);
				if (lh == NULL)
					return NULL;
				CSymbol * rh = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::EXPR_RHS),_setExprTree);
				if (rh == NULL)
					return NULL;
				
				dispTree = new CSymbolRow(3, _setExprTree?_exprTree:NULL ); // we will add the binary operator between terms
				
				int noOfRowChildren = 0;
				dispTree->addChild(noOfRowChildren++,lh);
				CSymbol *operatorSymbol;
				switch (exprType)
				{
					case scg::ADD_EXPR:
                    {
                        const wchar_t * addop = (( scg::ExpressionTree * )_exprTree->child(scg::BIN_OP))->wstr();
                        operatorSymbol = new CSymbolBasic(addop,OPERATOR,(int) wcslen(addop));
                        break;
                    }
                    case scg::MULT_EXPR:
                    {
                        const wchar_t * multop = (( scg::ExpressionTree * )_exprTree->child(scg::BIN_OP))->wstr();
                        if (!wcscmp(multop,L""))
                            operatorSymbol = new CSymbolBasic(L"&invisibletimes;",OPERATOR,wcslen(L"&invisibletimes;"));
                        else
                            operatorSymbol = new CSymbolBasic(L"*",OPERATOR,wcslen(L"*"));
						break;
					}
                    case scg::REL_EXPR:
					{
                        std::string op = _exprTree->child(scg::BIN_OP)->str();
						wchar_t opwstr[20];
						if (op == "lt")
							wcscpy(opwstr,L"&lt;");
						else if (op == "leq")
							wcscpy(opwstr,L"&lt;=");
						else if (op == "gt")
							wcscpy(opwstr,L">");
						else if (op == "geq")
							wcscpy(opwstr,L">=");
						else if (op == "eq")
							wcscpy(opwstr,L"=");
						else if (op == "neq")
							wcscpy(opwstr,L"!=");
						
						operatorSymbol = new CSymbolBasic(opwstr,OPERATOR,wcslen(opwstr));
						break;
                    }
				}
				dispTree->addChild(noOfRowChildren++,operatorSymbol);
				dispTree->addChild(noOfRowChildren++,rh);
				break;
			}
                
            case scg::LIST_EXPR:
            {
                int noOfListChildren = _exprTree->nchildren() ; 
                
                if (noOfListChildren > 1)
                    noOfListChildren = noOfListChildren * 2 - 1; // commas
                
                dispTree = new CSymbolRow(noOfListChildren, _setExprTree?_exprTree:NULL);
                for (int i = 0; i * 2 < noOfListChildren; i++)
                {
                    CSymbol * child = parseExprTree((scg::ExpressionTree *)_exprTree->child(i),_setExprTree);
                    dispTree->addChild(i*2,child);
                    if (i * 2 < noOfListChildren - 1)
                    {
                        CSymbolBasic * separator = new CSymbolBasic(L",",OPERATOR,wcslen(L","));
                        dispTree->addChild(i*2 + 1, separator);
                    }
                }
                break;
            }
				
            // Unary operators
			case scg::PLUSMINUS_EXPR:
            case scg::NEG_EXPR:
            case scg::FACTORIAL_EXPR:
			{
				CSymbol * e = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::UNARY_TERM),_setExprTree);
				if (e == NULL)
					return NULL;
				dispTree = new CSymbolRow(2,_setExprTree?_exprTree:NULL);
                CSymbol * operatorSymbol;
                switch (exprType) 
                {
                    case scg::NEG_EXPR:
                        operatorSymbol = new CSymbolBasic(L"-",OPERATOR,1);
                        dispTree->addChild(0,operatorSymbol);
                        dispTree->addChild(1,e);                        
                        break;
                        
                    case scg::PLUSMINUS_EXPR:
                        operatorSymbol = new CSymbolBasic(L"&plusminus;",OPERATOR,wcslen(L"&plusminus;"));
                        dispTree->addChild(0,operatorSymbol);
                        dispTree->addChild(1,e);
                        break;
                    
                    case scg::FACTORIAL_EXPR:
                        operatorSymbol = new CSymbolBasic(L"!",OPERATOR, wcslen(L"!"));
                        dispTree->addChild(0,e);
                        dispTree->addChild(1,operatorSymbol); // factorial is a postfix operator
                        break;
                        
                    default:
                        break;
                }
				break;
			}
				
            // Function expression
			case scg::FN_EXPR:
			{
				CSymbol *e = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::FN_ARG),_setExprTree);
				if (e == NULL)
					return NULL;
				dispTree = new CSymbolRow(3,_setExprTree?_exprTree:NULL);
				dispTree->addChild(0,parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::FN_NAME),_setExprTree));
				CSymbol * operatorSymbol = new CSymbolBasic(L"&applyfunction;",OPERATOR,wcslen(L"&applyfunction;"));
				dispTree->addChild(1,operatorSymbol);
				dispTree->addChild(2,e);
				break;
			}
				
            // prime expression
			case scg::PRIME_EXPR:
			{
				CSymbol *e = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::PRIME_NAME),_setExprTree);
				if (e == NULL)
					return NULL;
				
				int primeCount = ((scg::ExpressionTree *) _exprTree->child(scg::PRIME_COUNT))->nchildren();
				CSymbolBasic * myPrime;
				if (primeCount == 1)
					myPrime = new CSymbolBasic(L"&prime;",OPERATOR,wcslen(L"&prime;"));
				else if (primeCount == 2)
					myPrime = new CSymbolBasic(L"&Prime;",OPERATOR,wcslen(L"&Prime;"));
				else
					myPrime = new CSymbolBasic(L"&tprime;",OPERATOR,wcslen(L"&tprime;"));
				
				dispTree = new CSymbolSup(e,myPrime,_setExprTree?_exprTree:NULL);
				break;
			}
				
				// Fraction expression
			case scg::FRAC_EXPR:
			{
				CSymbol * eTop = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::FRAC_NUMER) ,_setExprTree);
				if (eTop == NULL)
					return NULL;
				CSymbol * eBott = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::FRAC_DENOM) ,_setExprTree);
				if (eBott == NULL)
					return NULL;
				
				dispTree = new CSymbolFrac(eTop,eBott,_setExprTree?_exprTree:NULL);
				break;
			}
				
				// SUP expression
			case scg::SUP_EXPR:
			{
				CSymbol * b = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::SUP_BASE) ,_setExprTree);
				if (b == NULL)
					return NULL;
				CSymbol * p = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::SUP_POW) ,_setExprTree);
				if (p == NULL)
					return NULL;
				dispTree = new CSymbolSup(b,p,_setExprTree?_exprTree:NULL);
				break;
			}
				
			case scg::SUBSCR_EXPR:
			{
				CSymbol * b = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::SUBSCR_BASE) ,_setExprTree);
				if (b == NULL)
					return NULL;
				CSymbol * s = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::SUBSCR_SUB) ,_setExprTree);
				if (s == NULL)
					return NULL;
				
				dispTree = new CSymbolSub(b,s,_setExprTree?_exprTree:NULL);
				break;
			}
				
            // SQRT expression
			case scg::ROOT_EXPR:
			{
				CSymbol * c = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::ROOT_CONTENTS),_setExprTree);
				if (c == NULL)
					return NULL;
				dispTree = new CSymbolSqrt(c,_setExprTree?_exprTree:NULL);
				break;
			}
				
            // Fenced expression
			case scg::PAREN_EXPR:
			{
				CSymbol * parenthesizedExpr = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::PAREN_CONTENTS), _setExprTree);
				if (parenthesizedExpr == NULL)
					return NULL;

                
                wchar_t * openFence = (wchar_t *) ((scg::ExpressionTree *) _exprTree->child(scg::PAREN_OPEN))->wstr();
                wchar_t * closeFence = (wchar_t *) ((scg::ExpressionTree *) _exprTree->child(scg::PAREN_CLOSE))->wstr();;
                dispTree = new CSymbolFence(1,openFence, closeFence, L"",_setExprTree?_exprTree:NULL);
				dispTree->addChild(0,parenthesizedExpr );
				
				break;
			}
				
			case scg::SUM_EXPR:
				// There are two children. One is the underover for the summation and the second is the summand
			{
				CSymbol * operand = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::RANGE_OPERAND),_setExprTree);
				if (operand == NULL)
					return NULL;
				dispTree = new CSymbolRow(2,_setExprTree?_exprTree:NULL);
				CSymbol * summation = new CSymbolBasic(L"&sum;",OPERATOR,(int) wcslen(L"&sum;"),_setExprTree?_exprTree:NULL);
                
				if (_exprTree->child(scg::RANGE_LOLIM)->type() == scg::BLANK_EXPR && _exprTree->child(scg::RANGE_UPLIM)->type() == scg::BLANK_EXPR)
					dispTree->addChild(0,summation); 
				else
				{
                    CSymbol * over;
                    CSymbol * under;
                    const wchar_t emptyStr[2] = L" ";
                    
                    if (_exprTree->child(scg::RANGE_LOLIM)->type() == scg::BLANK_EXPR) // there is only upper
                    {
                        over = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::RANGE_UPLIM),_setExprTree);
                        under = new CSymbolBasic(emptyStr,IDENTIFIER,(int) wcslen(emptyStr),NULL);
                    }
                    else if (_exprTree->child(scg::RANGE_UPLIM)->type() == scg::BLANK_EXPR) // there is only under
                    { 
                        over = new CSymbolBasic(emptyStr,IDENTIFIER,(int) wcslen(emptyStr),NULL);
                        under = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::RANGE_LOLIM),_setExprTree);
                    }
                    else // there is under and over
                    {
                        over = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::RANGE_UPLIM),_setExprTree);
                        under = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::RANGE_LOLIM),_setExprTree);
                    }
					dispTree->addChild(0, new CSymbolUnderOver(summation,under,over,_setExprTree?_exprTree:NULL));
				}
				dispTree->addChild(1,operand);
				break;
			}
				
			case scg::INTEGRAL_EXPR:
			{
				CSymbol * operand = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::RANGE_OPERAND),_setExprTree);
				if (operand == NULL)
					return NULL;
				
                // if the integration is progressing so the variable is not defined yet
                // the integration will have only the operand child 
                int noOfChildren = 2;  // assuming that only the operand is defined
                CSymbol * integralVar;
                
				if ((_exprTree->child(scg::INTEGRAL_VAR))->type() != scg::BLANK_EXPR)
                {
                    noOfChildren = 4;
                    integralVar = parseExprTree((scg::ExpressionTree *) _exprTree->child(scg::INTEGRAL_VAR),_setExprTree);
                    if (integralVar == NULL)
                        return NULL;
                }
                
				// if there is no lower or upper limits, the first child in the row is the operator integral
				// if there are limits the first child in the row is the underover expression with the integral as the operator
				dispTree = new CSymbolRow(noOfChildren,_setExprTree?_exprTree:NULL);
				CSymbol * integral = new CSymbolBasic(L"&integral;",OPERATOR,(int) wcslen(L"&integral;"),_setExprTree?_exprTree:NULL);
                
				if (_exprTree->child(scg::RANGE_LOLIM)->type() == scg::BLANK_EXPR && _exprTree->child(scg::RANGE_UPLIM)->type() == scg::BLANK_EXPR)
					dispTree->addChild(0,integral); 
				else 
                {
                    CSymbol * over;
                    CSymbol * under;
                    const wchar_t emptyStr[2] = L" ";

                    if (_exprTree->child(scg::RANGE_LOLIM)->type() == scg::BLANK_EXPR) // there is only upper
                    {
                        over = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::RANGE_UPLIM),_setExprTree);
                        under = new CSymbolBasic(emptyStr,IDENTIFIER,(int) wcslen(emptyStr),NULL);
                    }
                    else if (_exprTree->child(scg::RANGE_UPLIM)->type() == scg::BLANK_EXPR) // there is only under
                    { 
                        over = new CSymbolBasic(emptyStr,IDENTIFIER,(int) wcslen(emptyStr),NULL);
                        under = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::RANGE_LOLIM),_setExprTree);
                    }
                    else // there is under and over
                    {
                        over = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::RANGE_UPLIM),_setExprTree);
                        under = parseExprTree((scg::ExpressionTree *)_exprTree->child(scg::RANGE_LOLIM),_setExprTree);
                    }
					dispTree->addChild(0, new CSymbolUnderOver(integral,under,over,_setExprTree?_exprTree:NULL));
				}
				dispTree->addChild(1,operand);
                if (noOfChildren > 2)
                {
                    dispTree->addChild(2,new CSymbolBasic(L"&invisibletimes;",OPERATOR,(int) wcslen(L"&invisibletimes;"),_setExprTree?_exprTree:NULL));
				
                    CSymbol * temp = new CSymbolRow(2,_setExprTree?_exprTree:NULL);
                    temp->addChild(0,new CSymbolBasic(L"&differentiald;",OPERATOR,(int) wcslen(L"&differentiald;"),_setExprTree?_exprTree:NULL));
                    temp->addChild(1,integralVar);
				
                    dispTree->addChild(3,temp);
                }
				break;
			}
				
            case scg::NAME_EXPR:
			case scg::TERMINAL_EXPR:
			{
				const wchar_t * name = _exprTree->wstr();
				dispTree = new CSymbolBasic(name,IDENTIFIER,(int) wcslen(name),_setExprTree?_exprTree:NULL);
				break;
			}
                
            case scg::DOTS_EXPR:
            {
                const wchar_t * name = _exprTree->child(0)->wstr();
				dispTree = new CSymbolBasic(name,EXTERNAL_IDENTIFIER,(int) wcslen(name),_setExprTree?_exprTree:NULL);
                break;
            }
                                         
			case scg::VAR_EXPR:
			{
				//std::cout << "size is " << sizeof(scg::raw_expression_node) << " and " << sizeof(std::string) << std::endl;
				//scg::raw_expression_node *node = (scg::raw_expression_node *)_exprTree;
				//scg::raw_expression_node *child = node->mchild(0).ptr();
				const wchar_t * name = _exprTree->child(0)->wstr();
				dispTree = new CSymbolBasic(name,IDENTIFIER,(int) wcslen(name),_setExprTree?_exprTree:NULL);
				break;
			}
				
			case scg::NUM_EXPR:
			{
				const wchar_t * value = _exprTree->child(0)->wstr();
				dispTree = new CSymbolBasic(value,NUMBER,(int) wcslen(value),_setExprTree?_exprTree:NULL);
				break;
			}
				
			case scg::MATRIX_EXPR:
			{
				try
				{
					std::string numS = _exprTree->child(scg::MATRIX_NUMROWS)->str();
					int numRows = atoi(numS.c_str());
					numS = _exprTree->child(scg::MATRIX_NUMCOLS)->str();
					int numCols = atoi(numS.c_str());
					if (numRows < 1 || numCols < 1)
					{
						recognitionError = MATRIXCONSTRUCTION_ERR;
						return NULL;
					}
					dispTree = new CSymbolTable(numRows, _exprTree);
					for (int i = 0; i < numRows; i++)
					{
						scg::ExpressionTree * rowExpr =  (scg::ExpressionTree *) _exprTree->child(scg::MATRIX_ROWS)->child(i);
						if (rowExpr->type() != scg::MATRIXROW_EXPR)
						{
							recognitionError = MATRIXCONSTRUCTION_ERR;
							return NULL;
						}
						CSymbolTR * row = new CSymbolTR(numCols,rowExpr);
						for (int j = 0; j < numCols; j++)
						{
							CSymbol * child = parseExprTree((scg::ExpressionTree *) rowExpr->child(j),_setExprTree);
							if (child == NULL)
								return NULL;
							row->addChild(j,child);
						}
						dispTree->addChild(i,row);
					}
				}
				catch (...)
				{
					recognitionError = MATRIXCONSTRUCTION_ERR;
					return NULL;
				}
				break;
			}
                
            case scg::MULTI_EXPR:
            {
                try
                {
                    int numRows = _exprTree->child(scg::MULTI_ROWS)->nchildren();
                    
                    dispTree = new CSymbolTable(numRows, _exprTree);
                    for (int i = 0; i < numRows; i++)
                    {
                        scg::ExpressionTree * rowExpr =  (scg::ExpressionTree *) _exprTree->child(scg::MULTI_ROWS)->child(i);
                        if (rowExpr->type() != scg::MATRIXROW_EXPR)
                        {
                            recognitionError = MATRIXCONSTRUCTION_ERR;
                            return NULL;
                        }
                        CSymbolTR * row = new CSymbolTR(1,rowExpr);
                        CSymbol * child = parseExprTree((scg::ExpressionTree *) rowExpr->child(0),_setExprTree);
                        if (child == NULL)
                            return NULL;
                        row->addChild(0,child);
                    
                        dispTree->addChild(i,row);
                    }
                }
				catch (...)
                {
                    recognitionError = MATRIXCONSTRUCTION_ERR;
                    return NULL;
                }
				break;
            }

/*            {
                scg::ExpressionTree * rows = (scg::ExpressionTree *) _exprTree->child(scg::MULTI_ROWS);
                int noOfRows = rows->nchildren();
                dispTree = new CSymbolTable(noOfRows, _exprTree);

                for (int i = 0; i < noOfRows; i++)
                {
                    CSymbolTR * row = new CSymbolTR(1,(scg::ExpressionTree *)rows->child(i));
                    
                    CSymbol * child = parseExprTree((scg::ExpressionTree *) rows->child(i), _setExprTree);
                    if (child == NULL) 
                        return NULL;
                    row->addChild(0,child);
                    dispTree->addChild(i,row);
                }
                break;
            }
*/
            case scg::BLANK_EXPR:
			case scg::PLACEHOLDER_EXPR:
			{
				const wchar_t * name = L" ";
				dispTree = new CSymbolBasic(name,IDENTIFIER,(int) wcslen(name),_setExprTree?_exprTree:NULL);
				break;
			}

			default:
			{
				recognitionError = EXPRTYPE_ERR;
				return NULL;
				break;
			}
		}		
		return dispTree;
	}
	//---------------------------------------------------------------------
	/* return the display tree or NULL if the display tree could not be generated
	the flag _setExprTree = true by default means that the display tree will reference the expression tree from where
	it was generated. This is needed if this is the main expression tree. if this flag is false the reference to the
	source expression tree will not be valid. This is needed for the display trees generated for the alternates of 
	a recognition since the user will not need to access the source expression of those alternates again.
	*/
	CSymbol * getDisplayTree(scg::ExpressionTree *_exprTree, CDevice * _device, bool _setExprTree)
	{
		// parse the expression tree and construct a display tree
		CSymbol * dispExpr = parseExprTree(_exprTree, _setExprTree);

		if (dispExpr == NULL)
			return NULL;
		else if (recognitionError == EXPRTYPE_ERR)
		{
			delete dispExpr;
			recognitionError = ERRORFREE;
			return NULL;
		}
        if (_device != NULL)
        {
            // calculate the depth
            dispExpr->calcDepth(1.0);

            // create the boxes
            dispExpr->createBox(0,dispExpr->up *SYMBOLHI,_device);
		}
        return dispExpr;
	}
	//---------------------------------------------------------------------------------------------------------------------
	// break the expression into multiple lines if needed
	void breakLine(CSymbol * _expr, int _width)
	{
		if (_expr->getWidth() > _width)
		{
			// The original boxes all fit in one line
			CLineList * lines = new CLineList(_expr->up, _expr->down);

			// Start line breaking
			int lastLeft = 0;
			int targetLine = 0;

			CLineList * newLines = new CLineList(0,0);

			int _indent = 20; // is this the first line indent ???
			int _lineSpace = 0;

			_width = _width - 100;
			_expr->breakLine(0,_width,_indent,lastLeft,targetLine,newLines);
			newLines->addLineSpace(_lineSpace);
			_expr->adjustLines(lines,newLines);

		    int height = newLines->getLineTotal(newLines->last->lineNo) * SYMBOLHI;
			_expr->setHeightWidth(height,_width);

			delete newLines;
			delete lines;
		}
	}

	//-----------------------------------------------------------------------------------------------------------------
	void updateTreeFont(CSymbol * _root, CDevice * _device, int _width)
	{
		// create the boxes
		//SelectObject(_device,renderingFont);
		_root->createBox(0,_root->up *SYMBOLHI,_device);

		if (_width != 0)
			breakLine(_root, _width);
	}
}
