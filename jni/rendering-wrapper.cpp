#include <jni.h>
#include "Functs.h"

extern "C" {

	static SCGRenderer::CDevice* device;
	static SCGRenderer::CSymbol* displayTree;
	static SCGRenderer::CSymbol* crtSymbol;
	static std::vector<SCGRenderer::CSymbol*> variables;
	static scg::ExpressionTree* exprTree;
	static std::string mathML;
	static bool inkChanged;

	JNIEXPORT void JNICALL Java_com_mathbrush_tools_Renderer_init(JNIEnv* env, jobject obj, jobject aDevice, jint fontSize)
	{
		exprTree = NULL;
		inkChanged = false;
		displayTree = NULL;
		crtSymbol = NULL;
		mathML = "";
		device = new SCGRenderer::CDevice(env, aDevice);
		device->createFont("times", (int)fontSize);
		SCGRenderer::loadDictionaries();
	}

}