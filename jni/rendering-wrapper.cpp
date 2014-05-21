#include <jni.h>
#include "Functs.h"

extern "C" {

	static SCGRenderer::CDevice* device;
	static SCGRenderer::CSymbol* displayTree;
	static SCGRenderer::CSymbol* crtSymbol;
	static std::vector<SCGRenderer::CSymbol*> variables;
	static scg::ExpressionTree* exprTree;
	static std::string mathML;

}