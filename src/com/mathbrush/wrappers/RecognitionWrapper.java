package com.mathbrush.wrappers;

import java.util.List;


public class RecognitionWrapper {
	public RecognitionWrapper() {
		System.loadLibrary("wrappers");
	}
	
	public native boolean initRecognizer(String trainingPath, String profilePath, String profileName, String verboseFile);
}
