/*
 *  BuildTreeFromMathML.cpp
 *  staticCoacoaRenderingLib
 *
 *  Created by Mirette Marzouk on 10-09-09.
 *  Copyright 2010 U Waterloo. All rights reserved.
 *
 */
#include "Functs.h"
#include "Lines.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

//using namespace std; // to allow extern for a pointer ??

namespace SCGRenderer
{	
	extern errorCodes recognitionError;
	extern std::string parseErrorStr;	
	extern int SYMBOLHI;

	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	CSymbol * constructSymbolsTree(xmlNodePtr _root)
	{
		// element nodes are those that have either text inside them or other elements
		// the document has to have no white spaces otherwise they are counted as nodes
		char nodeTag[200];
		
		CSymbol * expr;
		int noOfChildren;

		strcpy(nodeTag, (const char *)_root->name);
		
		// remove the prefix from the node name if any
		if (_root->ns != NULL && _root->ns->prefix != NULL)
		{
			strcpy(nodeTag, nodeTag + strlen((const char *) _root->ns->prefix) + 1);
		}
		
		if (_root->type== XML_ELEMENT_NODE)	// element_node
		{
			if ((!strcmp(nodeTag,"math") && (int) xmlChildElementCount(_root) > 1))
				strcpy(nodeTag,"mrow");			// by default add an mrow element if it does not exist	
			
			if (!strcmp(nodeTag,"math"))
				expr = constructSymbolsTree(_root->children);
			else if (!(strcmp(nodeTag,"mrow") && strcmp(nodeTag,"mfenced") && strcmp(nodeTag,"mmultiscripts")))
			{
				//lst = _root->getChildNodes();
				noOfChildren = xmlChildElementCount(_root);
				if (!strcmp(nodeTag,"mrow"))
					expr = new CSymbolRow(noOfChildren);
				else if (!strcmp(nodeTag,"mmultiscripts"))
					expr = new CSymbolMultiScript(noOfChildren);
				else	// it is an mfenced the number of children should include the separators
				{
					// the brackets and the separators should be obtained from the mfenced attributes					
					xmlAttrPtr nodeProperties = _root->properties;
					char o [2] = "(";
					char c [2] = ")";
					for (xmlAttrPtr curProp = nodeProperties; curProp; curProp = curProp->next)
					{
						if (!strcmp((const char *) curProp->name, "open"))
							strcpy(o, (const char *) curProp->children->content);
						if (!strcmp((const char *) curProp->name, "close"))
							strcpy(c, (const char *) curProp->children->content);
					}					
					wchar_t s[2]; // open symbol
					wchar_t e[2]; // close symbol
					size_t convertedChars = 0;
					convertedChars = mbstowcs(s,o, strlen(o)+1);
					if (wcslen(s) == 0 || convertedChars == -1)
						wcscpy(s,L"(");
					convertedChars = mbstowcs(e,c, strlen(c)+1);
					if (wcslen(e) == 0 || convertedChars == -1)
						wcscpy(e,L")");
					std::wstring separators = L",";
					// we need to count the separators as children to the mfenced expression
					// that is why the noOfChildren is *2 - 1
					expr = new CSymbolFence(noOfChildren*2 - 1,s,e,separators);
				}
				xmlNodePtr child = _root->children;
				for (int i = 0; i<noOfChildren; i++)
				{
					int j = (!strcmp(nodeTag,"mfenced"))?i*2:i;
					
					expr->addChild(j,constructSymbolsTree(child));
					child = child->next;
				}
			}
			else if (!(strcmp(nodeTag,"mo") && strcmp(nodeTag,"mi") && strcmp(nodeTag,"mn")))
			{
				tokenType t = !strcmp(nodeTag,"mo")?OPERATOR:!strcmp(nodeTag,"mi")?IDENTIFIER:NUMBER;
				
				//the failure in the next line means the entity name is not defined in the dtd
				//int mm = _root->getFirstChild()->getNodeType();
				
				if (_root->children->type == XML_ENTITY_REF_NODE) // entity_reference_node
				{			
					wchar_t y[200];
					wchar_t temp[200];
					wcscpy(y,L"&");
					size_t convertedChars = 0;
					convertedChars = mbstowcs(temp, (const char *) _root->children->name, strlen((const char *)_root->children->name)+1);
					wcscat(y,temp);
					wcscat(y,L";");
					if (t != OPERATOR)
						t = EXTERNAL_IDENTIFIER;
					expr = new CSymbolBasic(y,t,wcslen(y));
				}
				else
				{					
					wchar_t *y;
					y = new wchar_t[strlen((const char *) _root->children->content) + 1];
					size_t convertedChars = 0;
					convertedChars = mbstowcs(y, (char *) _root->children->content, strlen((char *) _root->children->content) + 1);
					expr = new CSymbolBasic(y,t,wcslen(y));
					delete[] y;
				}
			}
			else if (!(strcmp(nodeTag,"none") && strcmp(nodeTag,"mprescripts")))
			{
				tokenType t = !strcmp(nodeTag,"none")?NONE:PRESCRIPT;
				wchar_t txt[1];
				txt[0] = '\0';
				expr = new CSymbolBasic(txt,t,1);
			}
			else if (!strcmp(nodeTag,"mfrac"))
				expr = new CSymbolFrac(constructSymbolsTree(_root->children),constructSymbolsTree(_root->children->next));
			else if (!strcmp(nodeTag,"msup"))
				expr = new CSymbolSup(constructSymbolsTree(_root->children),constructSymbolsTree(_root->children->next));
			else if (!strcmp(nodeTag,"msub"))
				expr = new CSymbolSub(constructSymbolsTree(_root->children),constructSymbolsTree(_root->children->next));
			else if (!strcmp(nodeTag,"msubsup"))
				expr = new CSymbolSubSup(constructSymbolsTree(_root->children),constructSymbolsTree(_root->children->next),constructSymbolsTree(_root->children->next->next));
			else if (!strcmp(nodeTag,"msqrt"))
			{
				noOfChildren = xmlChildElementCount(_root);
				
				if (noOfChildren == 1)
					expr = new CSymbolSqrt(constructSymbolsTree(_root->children));
				else
				{
					CSymbolRow *row = new CSymbolRow(noOfChildren);
					xmlNodePtr child = _root->children;
					for (int i = 0; i<noOfChildren; i++)
					{
						row->addChild(i,constructSymbolsTree(child));
						child = child->next;
					}
					expr = new CSymbolSqrt(row);
				}
			}
			else if (!strcmp(nodeTag,"munder"))
				expr = new CSymbolUnder(constructSymbolsTree(_root->children),constructSymbolsTree(_root->children->next));
			else if (!strcmp(nodeTag,"mover"))
				expr = new CSymbolOver(constructSymbolsTree(_root->children),constructSymbolsTree(_root->children->next));
			else if (!strcmp(nodeTag,"munderover"))
				expr = new CSymbolUnderOver(constructSymbolsTree(_root->children),constructSymbolsTree(_root->children->next),constructSymbolsTree(_root->children->next->next));
			else if (!strcmp(nodeTag,"mroot"))
			{
				CSymbolFence *f = new CSymbolFence(3,L"(",L")",L"");
				f->addChild(0,new CSymbolBasic(L"1",NUMBER,1));
				f->addChild(1,new CSymbolBasic(L"/",OPERATOR,1));
				f->addChild(2,constructSymbolsTree(_root->children->next));
				expr = new CSymbolSup(constructSymbolsTree(_root->children),f);
			}
			else if (!strcmp(nodeTag,"semantics"))
			{
				// the reason of including the semantic tag is that mathematica sometimes
				// genrates semantic tags in the presentation MathML it generates.
				// the first child of the semantic tag will be considered and the rest will be
				// ignored since it is always an annotation for the first child
				expr = constructSymbolsTree(_root->children);
			}
			else if (!strcmp(nodeTag,"mtext")) // generated sometimes from mathematica and includes annotation so it just be ignored for now
			{
				wchar_t txt[1];
				txt[0] = '\0';
				expr = new CSymbolBasic(txt,NONE,1);
			}
			else if (!(strcmp(nodeTag,"mtable") && strcmp(nodeTag,"mtr") && strcmp(nodeTag,"mtd")))
			{
				noOfChildren = xmlChildElementCount(_root);
				if (!strcmp(nodeTag,"mtable"))
				{
					if (noOfChildren == 0)
						expr = new CSymbolBasic(L"??",IDENTIFIER,2);
					else
						expr = new CSymbolTable(noOfChildren);
				}
				else if (!strcmp(nodeTag,"mtr"))
				{ // sometimes when the long form of a matrix is requested is comes back with 0 number of columns
					if (noOfChildren == 0)
					{
						expr = new CSymbolTR(1);
						CSymbol * expr2  = new CSymbolRow(1);
						expr2->addChild(0,new CSymbolBasic(L"??",IDENTIFIER,2));
						expr->addChild(0,expr2);
					}
					else
						expr = new CSymbolTR(noOfChildren);
				}
				else
					expr = new CSymbolRow(noOfChildren);
				
				xmlNodePtr child = _root->children;
				for (int i = 0; i<noOfChildren; i++)
				{
					expr->addChild(i,constructSymbolsTree(child));
					child = child->next;
				}
			}
			else
			{
				std::string s = nodeTag;
				parseErrorStr = "Tag not defined  " + s;
				expr = new CSymbolBasic(L"??",IDENTIFIER,2);
			}
		}
		return expr;
	}
	//------------------------------------------------------------------------------------------------------------
	// the parser assumes the MathML comes with the DTD referenced in its correct location in the application
	xmlDocPtr getDocument(std::string _mathML)
	{
		xmlParserCtxtPtr ctxt; /* the parser context */
		xmlDocPtr doc; /* the resulting document tree */
		
		/* create a parser context */
		ctxt = xmlNewParserCtxt();
		if (ctxt == NULL) {
			parseErrorStr  = "Failed to allocate parser context\n";
			return NULL;
		}
		
		/* parse the file, activating the DTD validation option */
		// doc = xmlCtxtReadFile(ctxt, _mathML.c_str(), NULL, XML_PARSE_DTDVALID);
		try 
		{
			doc = xmlCtxtReadMemory(ctxt, _mathML.c_str(), _mathML.length(), NULL, NULL, XML_PARSE_DTDVALID);
		}
		catch (...) {
			std::cout << "Problem in parsing MathML with DTD\n";
			return NULL;
		}
		
		/* check if parsing suceeded */
		if (doc == NULL) 
		{
			parseErrorStr = "Failed to parse\n";
		} 
		else 
		{
			/* check if validation suceeded */
			if (ctxt->valid == 0)
			{
				parseErrorStr = "Failed to validate\n";
				return NULL;
			}
		}

		return doc;
	}
	//------------------------------------------------------------------------------------------------------------
	CSymbol * parseMathML(std::string _mathML)
	{
		xmlDocPtr doc = NULL;
		xmlNodePtr rootElement = NULL;
		
		/*
		 * this initialize the library and check potential ABI mismatches
		 * between the version it was compiled for and the actual shared
		 * library used.
		 */
		LIBXML_TEST_VERSION
		
		/*parse the file and get the DOM */
		doc = getDocument(_mathML);
		
		if (doc == NULL) 
		{
			return NULL;
		}
		
		/*Get the root element node */
		rootElement = xmlDocGetRootElement(doc);
		
		// construct the internal tree presentation CSymbol
		CSymbol * expr = NULL;
		if (rootElement)
			expr = constructSymbolsTree(rootElement);
		else
			parseErrorStr = "The input must be a valid MathML data";
		
		/*free the document */
		xmlFreeDoc(doc);
		
		/*
		 *Free the global variables that may
		 *have been allocated by the parser.
		 */
		xmlCleanupParser();		

		return expr;
	}
	
	//------------------------------------------------------------------------------------------------------------
	// Generate a CSymbol object (tree) for the MathML.
	// _device is used to create the correct boxes sizes for the symbols
	// _width if != 0 is used to break the lines
	// _noOfTerms if != 0 is used to display the polynomial in short form with those number of terms	
	CSymbol * getDisplayTree(std::string _mathML,CDevice * _device, int _width, int _noOfTerms)
	{
		// parse the expression tree and construct a display tree
		CSymbol * dispExpr = parseMathML(_mathML);
		
		if (dispExpr == NULL)
			return NULL;
		else if (recognitionError == EXPRTYPE_ERR)
		{
			delete dispExpr;
			recognitionError = ERRORFREE;
			return NULL;
		}
		
		// if noOfTerms != 0 this means the expression needs to be in short form and only those number of terms are going to be displayed
		if (_noOfTerms != 0)
			dispExpr->reform(_noOfTerms);
		
		// calculate the depth
		dispExpr->calcDepth(1.0);
		
		// create the boxes
		dispExpr->createBox(0,dispExpr->up *SYMBOLHI,_device);
		
		if (_width != 0)
			breakLine(dispExpr, _width);
		
		return dispExpr;
	}
	//--------------------------------------------------------------------------------------------------
}
