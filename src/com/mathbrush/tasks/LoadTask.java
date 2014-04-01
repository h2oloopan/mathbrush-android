package com.mathbrush.tasks;

import java.io.*;

import android.os.AsyncTask;
import android.content.res.AssetManager;

import com.mathbrush.R;
import com.mathbrush.MathBrush;
import com.mathbrush.tools.*;

public class LoadTask extends AsyncTask<Void, Void, Recognizer> {
	private MathBrush mb;

	public LoadTask(MathBrush mb) {
		this.mb = mb;
	}

	@Override
	protected Recognizer doInBackground(Void... params) {
		//copy assets
		File folder = mb.getDir(mb.getString(R.string.data_folder), 0);
		AssetManager manager = mb.getAssets();
		String[] files = null;
		try {
			files = manager.list(mb.getString(R.string.data_files));
		} catch (IOException e) {
			Debugger.log("Failed to get assets list");
		}
		for (String file : files) {
			InputStream in = null;
			OutputStream out = null;
			try {
				in = manager.open(mb.getString(R.string.data_files) + "/" + file);
				File outFile = new File(folder, file);
				out = new FileOutputStream(outFile);
				copyFile(in, out);
				in.close();
				out.flush();
				out.close();
			} catch (IOException e) {
				Debugger.log("Failed to copy asset file: " + file);
			}
		}

		//initialize recognizer
		String trainingPath = folder.getAbsolutePath();
		String profilePath = folder.getAbsolutePath();
		String profileName = "";
		String verboseFile = folder.getAbsolutePath() + "/" + mb.getString(R.string.verbose_file);
		Recognizer recognizer = new Recognizer();
		recognizer.init(trainingPath, profilePath, profileName, verboseFile);
		return recognizer;
	}

	@Override
	protected void onPostExecute(Recognizer recognizer) {
		mb.ready(recognizer);
	}


	//Helper functions to copy assets files to internal storage
	private void copyFile(InputStream in, OutputStream out) throws IOException {
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1) {
			out.write(buffer, 0, read);
		}
	}
}