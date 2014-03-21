#include <iostream>
#include "Functs.h"

int main (int argc, char * const argv[]) 
{
	
	
	scg::MathRecognizer * mRec = scg::CreateMathRecognizer();
	int e = mRec->UpdateParseTable();
	if (FAILURE(e)) {
		std::cerr << "ts: parse error: " << scg::error_message(e) << std::endl;
	}	
	
	SCGRenderer::loadDictionaries();
	scg::ExpressionIterator * exprIt = mRec->CreateDefaultIterator();
	if (exprIt != NULL)
	{
		scg::ExpressionTree * exprTree = (scg::ExpressionTree *)exprIt->next();
		if (exprTree != NULL)
		{
			SCGRenderer::CDevice * device;
			SCGRenderer::CSymbol * dispTree = getDisplayTree(exprTree, device);
		}
	}

	std::cout << "DONE 1 \n";

	delete mRec;


	std::string mathML3 = "<!DOCTYPE math PUBLIC '-//W3C//DTD MathML 2.0//EN' '../data/xhtml-math11-f.dtd'><math xmlns='http://www.w3.org/1998/Math/MathML'><mfrac><mi>x</mi><mn>6</mn></mfrac></math>";
	
	SCGRenderer::CDevice * device;
	SCGRenderer::CSymbol * tree = getDisplayTree(mathML3,device);
	
	std::cout << "DONE 2 \n";
    return 0;
}
