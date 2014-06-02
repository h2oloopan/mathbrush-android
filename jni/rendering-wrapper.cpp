#include <jni.h>
#include "Functs.h"
#include "CDevice.h"

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

	JNIEXPORT jboolean JNICALL Java_com_mathbrush_tools_Renderer_display(JNIEnv* env, jobject obj, jstring jMathML, jint x, jint y)
	{
		if (displayTree != NULL)
		{
			delete displayTree;
			crtSymbol = NULL;
			variables.clear();
		}

		const char* cMathML = env->GetStringUTFChars(jMathML, NULL);
		std::string mathML(cMathML);
		displayTree = SCGRenderer::getDisplayTree(mathML, device);

		if (displayTree == NULL)
			return false;

		displayTree->moveTo((int)x, (int)y);
		displayTree->display(device, false);

		env->ReleaseStringUTFChars(jMathML, cMathML);
	}

}