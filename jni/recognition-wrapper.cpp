#include <iostream>
#include <fstream>
#include <jni.h>
#include "MathRecognizer.h"
#include "wrapper.h"

extern "C" {
/* TODO: We are now ignoring verbose mode */
JNIEXPORT jboolean JNICALL Java_com_mathbrush_tools_Recognizer_initRecognizer(JNIEnv* env, jobject obj, jstring trainingPath, jstring profilePath, jstring profileName, jstring verboseFile)
{
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

	env->ReleaseStringUTFChars(trainingPath, nTrainingPath);
	env->ReleaseStringUTFChars(profilePath, nProfilePath);
	env->ReleaseStringUTFChars(verboseFile, nVerboseFile);
	
	if (scg::InitializeRecognizer()) {
		env->ReleaseStringUTFChars(profileName, nProfileName);
		LOG("FALSE");
		return false;
	}

	if (strcmp(nProfileName, ""))
	{
		scg::SetUserProfile(nProfileName);
		env->ReleaseStringUTFChars(profileName, nProfileName);
	}

	LOG("TRUE");
	return true;
}
}
