#ifndef FUNCTS_H_
#define FUNCTS_H_

#define NOMINMAX
#include <iostream>

#include "MathRecognizer.h"
#include "Symbol.h"

namespace SCGRenderer
{
	const std::string DefaultFontName = "Times New Roman";
	const int DefaultFontHeight = 30;

	// implemented in ParseExprTree.cpp
	CSymbol * getDisplayTree(scg::ExpressionTree *, CDevice *, bool _setExprTree = true);
	CSymbol * getDisplayTree(std::string _mathML, CDevice * _device, int _width = 0, int _noOfTerms = 0);
	void updateTreeFont(CSymbol * _root, CDevice * _device, int _width=0);
	void breakLine(CSymbol * _expr, int _width);

	// implemented in dictionaries.cpp
	void loadDictionaries();
	void updateFontHeight(int _fontHeight);
	void unloadDictionaries();
}

#endif