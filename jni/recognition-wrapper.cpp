#include <iostream>
#include <fstream>
#include <jni.h>
#include "MathRecognizer.h"

extern "C" {
/* TODO: We are now ignoring verbose mode */
JNIEXPORT jboolean JNICALL Java_com_mathbrush_wrappers_RecognitionWrapper_initRecognizer(JNIEnv* env, jobject obj, jstring trainingPath, jstring profilePath, jstring profileName)
{
	std::cerr << "Test";
	return true;
	/*
	const char* nTrainingPath = env->GetStringUTFChars(trainingPath, 0);
	const char* nProfilePath = env->GetStringUTFChars(profilePath, 0);
	const char* nProfileName = env->GetStringUTFChars(profileName, 0);

	scg::SetProfilePath(nTrainingPath);
	scg::SetUserProfilePath(nProfilePath);

	if (scg::InitializeRecognizer())
		return false;

	if (strcmp(nProfileName, ""))
	{
		//scg::SetUserProfile(nProfileName);
		scg::SetUserProfile("a");
	}

	return true;
	*/
}
}
