#include <jni.h>
#include "Functs.h"
#include "CDevice.h"
#include "wrapper.h"

extern "C" {

	static SCGRenderer::CDevice* device;
	static SCGRenderer::CSymbol* displayTree;
	static SCGRenderer::CSymbol* crtSymbol;
	static std::vector<SCGRenderer::CSymbol*> variables;
	static scg::ExpressionTree* exprTree;
	static std::string mathML;
	static bool inkChanged;

	JNIEXPORT void JNICALL Java_com_mathbrush_tools_Renderer_init(JNIEnv* env, jobject obj, jobject view, jint fontSize)
	{
		exprTree = NULL;
		inkChanged = false;
		displayTree = NULL;
		crtSymbol = NULL;
		mathML = "";

		//create a global reference
		jobject viewGlobal = reinterpret_cast<jobject>(env->NewGlobalRef(view));
		device = new SCGRenderer::CDevice(env, viewGlobal);
		device->createFont("times", (int)fontSize);
		
		LOG("%d", (long)device);
		//device->drawText("test", 50, 50);
		SCGRenderer::loadDictionaries();
	}

	//JNIEXPORT jboolean JNICALL Java_com_mathbrush_tools_Renderer_display(JNIEnv* env, jobject obj, jstring jMathML, jint x, jint y)
	JNIEXPORT jboolean JNICALL Java_com_mathbrush_tools_Renderer_display(JNIEnv* env, jobject obj, jlong exprTree, jint x, jint y)
	{
		if (displayTree != NULL)
		{
			delete displayTree;
			crtSymbol = NULL;
			variables.clear();
		}

		//const char* cMathML = env->GetStringUTFChars(jMathML, NULL);
		//std::string mathML(cMathML);
		//displayTree = SCGRenderer::getDisplayTree(mathML, device);

		LOG("exprTree%d", exprTree);

		displayTree = SCGRenderer::getDisplayTree((scg::ExpressionTree*)exprTree, device);
		//LOG("%s", mathML.c_str());
		if (displayTree == NULL)
		{
			LOG("NULL");
			return false;
		}
			
		LOG("%d", (long)device);
		
		displayTree->moveTo((int)x, (int)y);
		displayTree->display(device, false);

		//env->ReleaseStringUTFChars(jMathML, cMathML);
	}

}