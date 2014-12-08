package com.cs241honors.mathmagicka;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.zip.GZIPInputStream;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;

import com.googlecode.tesseract.android.TessBaseAPI;

public class MathMagickaActivity extends Activity {
	public static final String PACKAGE_NAME = "com.cs241honors.mathmagicka";
	public static final String DATA_PATH = Environment
			.getExternalStorageDirectory().toString() + "/MathMagicka/";
	public static final String lang = "eng";
	private static final String TAG = "MathMagicka.java";

	protected Button _btn_camera;
	protected Button _btn_compute;
	protected EditText _field;
	protected EditText _output;
	protected String _path;
	protected boolean _taken;

	protected static final String PHOTO_TAKEN = "photo_taken";
	
	// NDK stuff
	// static so gets loaded once on application begin
	static
	{
		System.loadLibrary("localsolver");
	}
	/*
	public native int checkNumber(const char * part);
	public native char * substring(const char * string, int i, int j);
	public native int simpleSolver(const char * equation);
	*/
	public native String simpleSolver(String input);
	// END NDK STUFF

	@Override
	public void onCreate(Bundle savedInstanceState) {

		String[] paths = new String[] { DATA_PATH, DATA_PATH + "tessdata/" };

		for (String path : paths) {
			File dir = new File(path);
			if (!dir.exists()) {
				if (!dir.mkdirs()) {
					Log.v(TAG, "ERROR: Creation of directory " + path + " on sdcard failed");
					return;
				} else {
					Log.v(TAG, "Created directory " + path + " on sdcard");
				}
			}

		}
		
		if (!(new File(DATA_PATH + "tessdata/" + lang + ".traineddata")).exists()) {
			try {
				AssetManager assetManager = getAssets();
				InputStream in = assetManager.open("tessdata/" + lang + ".traineddata");
				OutputStream out = new FileOutputStream(DATA_PATH
						+ "tessdata/" + lang + ".traineddata");

				// Transfer bytes from in to out
				byte[] buf = new byte[1024];
				int len;
				while ((len = in.read(buf)) > 0) {
					out.write(buf, 0, len);
				}
				in.close();
				out.close();
				
				Log.v(TAG, "Copied " + lang + " traineddata");
			} catch (IOException e) {
				Log.e(TAG, "Was unable to copy " + lang + " traineddata " + e.toString());
			}
		}

		super.onCreate(savedInstanceState);

		setContentView(R.layout.main);

		// _image = (ImageView) findViewById(R.id.image);
		_field = (EditText) findViewById(R.id.field);
		_output = (EditText) findViewById(R.id.output);
		_btn_camera = (Button) findViewById(R.id.btn_camera);
		_btn_compute = (Button) findViewById(R.id.btn_compute);
		_btn_camera.setOnClickListener(new CameraClickHandler());
		_btn_compute.setOnClickListener(new ComputeClickHandler());

		_path = DATA_PATH + "/ocr.jpg";
	}

	public class CameraClickHandler implements View.OnClickListener {
		public void onClick(View view) {
			Log.v(TAG, "Starting Camera app");
			startCameraActivity();
		}
	}
	
	public class ComputeClickHandler implements View.OnClickListener {
		public void onClick(View view){
			// hide the keyboard
			InputMethodManager mgr = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
			mgr.hideSoftInputFromWindow(_field.getWindowToken(),0);
			
			Log.v(TAG, "Running local solve code");
			String input = _field.getText().toString().trim();
			//String output = "...";
			try{ 
				//output = simpleSolver(input);
				_field.setText("solve code running..."); // this is to provide the user some progress info
				new ProcessInput().execute(input);       // this will run in the background
			}
			catch (Exception e){
				_field.setText("ERROR -> simpleSolver thread");
				return;
			}
			
		}
		// synchronization stuff
		private class ProcessInput extends AsyncTask<String,Integer,String> {
			protected void onProgressUpdate(Integer progress){
				// we could have a loading bar if we wanted here
				//setProgressPercent(progress[0]);
			}
			protected void onPostExecute(String output){
				_field.setText("Input solved, see below");
				_output.setText("Solution: " + output);
			}
			@Override
			protected String doInBackground(String... inputs) {
				// here we could do API calls, linear algebra libraries, whatever, all
				// of it will be in the background :0
				return simpleSolver(inputs[0]);
			}
		}
	}

	// Simple android photo capture:
	// http://labs.makemachine.net/2010/03/simple-android-photo-capture/

	protected void startCameraActivity() {
		File file = new File(_path);
		Uri outputFileUri = Uri.fromFile(file);

		final Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		intent.putExtra(MediaStore.EXTRA_OUTPUT, outputFileUri);

		startActivityForResult(intent, 0);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {

		Log.i(TAG, "resultCode: " + resultCode);

		if (resultCode == -1) {
			onEquationPhotoTaken();
		} else {
			Log.v(TAG, "User cancelled");
		}
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		outState.putBoolean(MathMagickaActivity.PHOTO_TAKEN, _taken);
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		Log.i(TAG, "onRestoreInstanceState()");
		if (savedInstanceState.getBoolean(MathMagickaActivity.PHOTO_TAKEN)) {
			onEquationPhotoTaken();
		}
	}

	protected void onEquationPhotoTaken() {
		_taken = true;

		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inSampleSize = 4;

		Bitmap bitmap = BitmapFactory.decodeFile(_path, options);

		try {
			// we are required to convert to ARGB_8888 for the tesseract engine
			bitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);

		} catch (Exception e){
			Log.e(TAG, "Could not convert bitmap to ARGB_8888: " + e.toString());
		}

		TessBaseAPI baseApi = new TessBaseAPI();
		baseApi.setDebug(true);
		baseApi.init(DATA_PATH, lang);
		baseApi.setImage(bitmap);
		
		String recognizedText = baseApi.getUTF8Text();
		
		baseApi.end();

		// Strip returned text down to alpha-numerics
		Log.v(TAG, "Raw OCR text: " + recognizedText);

		// only strip if lang is eng
		if ( lang.equalsIgnoreCase("eng") ) {
			recognizedText = recognizedText.replaceAll("[^a-zA-Z0-9]+", " ");
		}
		
		recognizedText = recognizedText.trim();

		if (recognizedText.length() != 0) {
			_field.setText(recognizedText);
			_field.setSelection(_field.getText().toString().length());
		}
	}
}
