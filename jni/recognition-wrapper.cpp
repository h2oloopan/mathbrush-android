#include <jni.h>
#include <iostream>
#include <fstream>
#include "MathRecognizer.h"
#include "wrapper.h"

extern "C" {

static scg::MathRecognizer* recognizer = NULL;
static scg::ExpressionTree* exprTree = NULL;

//If the recognizer doesn't exist, recreate it
void checkRecognizer() {
	if (!recognizer) {
		recognizer = scg::CreateMathRecognizer();
	}
}

JNIEXPORT jboolean JNICALL Java_com_mathbrush_tools_Recognizer_init(JNIEnv* env, jobject obj, jstring trainingPath, jstring profilePath, jstring profileName, jstring verboseFile) {
	//Convert jstrings to char*s
	const char* nTrainingPath = env->GetStringUTFChars(trainingPath, NULL);
	const char* nProfilePath = env->GetStringUTFChars(profilePath, NULL);
	const char* nProfileName = env->GetStringUTFChars(profileName, NULL);
	const char* nVerboseFile = env->GetStringUTFChars(verboseFile, NULL);


	if (strlen(nVerboseFile) > 0) {
		//verbose mode
		scg::VerboseOutputToFile(nVerboseFile);
		scg::SetVerbosity(1);
	}
	else {
		scg::SetVerbosity(0);
	}
	
	
	scg::SetProfilePath(nTrainingPath);
	scg::SetUserProfilePath(nProfilePath);
	scg::SetTabletResolution(132);

	//Release allocated native strings
	env->ReleaseStringUTFChars(trainingPath, nTrainingPath);
	env->ReleaseStringUTFChars(profilePath, nProfilePath);
	env->ReleaseStringUTFChars(verboseFile, nVerboseFile);
	
	if (scg::InitializeRecognizer()) {
		env->ReleaseStringUTFChars(profileName, nProfileName);
		return false;
	}

	if (strcmp(nProfileName, ""))
	{
		scg::SetUserProfile(nProfileName);
		
	}

	env->ReleaseStringUTFChars(profileName, nProfileName);

	checkRecognizer();
	return true;
}

JNIEXPORT void JNICALL Java_com_mathbrush_tools_Recognizer_addStroke(JNIEnv* env, jobject obj, jlongArray xs, jlongArray ys, jint count) {

	checkRecognizer();

	jsize jxLength = env->GetArrayLength(xs);
	jsize jyLength = env->GetArrayLength(ys);

	if (jxLength != count || jyLength != count) {
		LOG("Stroke points length does not match!");
		return;
	}

	//convert to native arrays
	jlong* jxs = env->GetLongArrayElements(xs, NULL);
	jlong* jys = env->GetLongArrayElements(ys, NULL);
	long* nxs = new long[count];
	long* nys = new long[count];

	for (int i = 0; i < count; i++) {
		nxs[i] = jxs[i];
		nys[i] = jys[i];
	}

	scg::RawStroke* strokes = new scg::RawStroke[1];
	strokes[0].set_points(nxs, nys, count);
	recognizer->AddStrokes(strokes, 1);
	delete[] strokes;

	//Release allocated native arrays
	env->ReleaseLongArrayElements(xs, jxs, 0);
	env->ReleaseLongArrayElements(ys, jys, 0);
}

JNIEXPORT jboolean JNICALL Java_com_mathbrush_tools_Recognizer_recognize(JNIEnv* env, jobject obj) {
	try {
		exprTree = (scg::ExpressionTree*) recognizer->GetTopExpression();
		if (exprTree == NULL) return false;
	} catch (char* str) {
		LOG("%s", str);
		exprTree = NULL;
		return false;
	}
	return true;
}

JNIEXPORT jstring JNICALL Java_com_mathbrush_tools_Recognizer_getMathML(JNIEnv* env, jobject obj) {
	std::string result = "";
	if (exprTree != NULL) {
		if (exprTree->HasLongForm()) {
			scg::ExpressionIterator* iterator = exprTree->CreateLongFormIterator();
			scg::ExpressionTree* expr = (scg::ExpressionTree*) iterator->next();
			iterator->release();
			if (expr != NULL) {
				result = expr->long_str();
				expr->release();
			}
		}
		else {
			result = exprTree->long_str();
		}
	}
	
	//convert string to jstring
	return env->NewStringUTF(result.c_str());
}

JNIEXPORT void JNICALL Java_com_mathbrush_tools_Recognizer_clear(JNIEnv* env, jobject obj) {
	//remove static stuff
	if (exprTree != NULL) {
		exprTree->release();
		exprTree = NULL;
	}

	if (recognizer != NULL) {
		recognizer->release();
		recognizer = NULL;
	}
}

JNIEXPORT void JNICALL Java_com_mathbrush_tools_Recognizer_reset(JNIEnv* env, jobject obj) {
	Java_com_mathbrush_tools_Recognizer_clear(env, obj);
	checkRecognizer();
}

JNIEXPORT void JNICALL Java_com_mathbrush_tools_Recognizer_shutDown(JNIEnv* env, jobject obj) {
	Java_com_mathbrush_tools_Recognizer_clear(env, obj);
	scg::ShutdownRecognizer();
}


JNIEXPORT void JNICALL Java_com_mathbrush_tools_Recognizer_changeProfile(JNIEnv* env, jobject obj, jstring profileName) {
	const char* nProfileName = env->GetStringUTFChars(profileName, NULL);
	scg::ShutdownRecognizer();
	scg::InitializeRecognizer();
	if (strcmp(nProfileName, "")) {
		scg::SetUserProfile(nProfileName);
	}
	env->ReleaseStringUTFChars(profileName, nProfileName);
}





}
