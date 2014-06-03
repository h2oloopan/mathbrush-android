package com.mathbrush.tools;


import com.mathbrush.tools.Debugger;

public class Recognizer {
	
	public Recognizer() {
		System.loadLibrary("wrappers");	
	}

	public native boolean init(String trainingPath, String profilePath, String profileName, String verboseFile);
	public native void addStroke(long[] xs, long[] ys, int count);
	public native boolean recognize();
	public native String getMathML();
	public native long getExprTree();
	public native void shutDown();
	public native void clear();
	public native void reset();
	public native void changeProfile(String profileName);
}
