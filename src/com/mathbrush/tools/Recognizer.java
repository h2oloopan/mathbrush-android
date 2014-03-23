package com.mathbrush.tools;


import com.mathbrush.tools.Debugger;

public class Recognizer {
	
	public Recognizer() {
		System.loadLibrary("wrappers");	
	}
	
	public boolean init(String trainingPath, String profilePath, String profileName) {
		return initRecognizer(trainingPath, profilePath, profileName);
	}

	private native boolean initRecognizer(String trainingPath, String profilePath, String profileName);
}
