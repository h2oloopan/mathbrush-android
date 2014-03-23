#include <iostream>
#include <fstream>
#include <jni.h>
#include "MathRecognizer.h"
#include "wrapper.h"

extern "C" {
/* TODO: We are now ignoring verbose mode */
JNIEXPORT jboolean JNICALL Java_com_mathbrush_tools_Recognizer_initRecognizer(JNIEnv* env, jobject obj, jstring trainingPath, jstring profilePath, jstring profileName)
{
	
	const char* nTrainingPath = env->GetStringUTFChars(trainingPath, NULL);

	const char* nProfilePath = env->GetStringUTFChars(profilePath, NULL);
	const char* nProfileName = env->GetStringUTFChars(profileName, NULL);


	LOG("TPATH:");
	LOG("%s", nTrainingPath);
	LOG("PPATH:");
	LOG("%s", nProfilePath);
	LOG("PNAME:");
	LOG("%s", nProfileName);

	//TODO: Here I am fixing Verbosity to 0
	scg::SetVerbosity(0);
	scg::SetProfilePath(nTrainingPath);
	scg::SetUserProfilePath(nProfilePath);
	scg::SetTabletResolution(132);

	env->ReleaseStringUTFChars(trainingPath, nTrainingPath);
	env->ReleaseStringUTFChars(profilePath, nProfilePath);
	
	if (scg::InitializeRecognizer())
		LOG("FALSE");
		env->ReleaseStringUTFChars(profileName, nProfileName);
		return false;

	if (strcmp(nProfileName, ""))
	{
		scg::SetUserProfile(nProfileName);
		env->ReleaseStringUTFChars(profileName, nProfileName);
	}

	LOG("TRUE");
	return true;
}
}
