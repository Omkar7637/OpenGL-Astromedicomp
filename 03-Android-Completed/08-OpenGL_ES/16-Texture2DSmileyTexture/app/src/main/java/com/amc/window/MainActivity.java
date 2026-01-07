package com.amc.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import android.graphics.Color;

// packages for Full screen
import androidx.core.view.WindowCompat;

import androidx.core.view.WindowInsetsControllerCompat;

import androidx.core.view.WindowInsetsCompat;

// For Landscape

import android.content.pm.ActivityInfo;


public class MainActivity extends AppCompatActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		// Fullscreen
		// Get and hide action bar
		getSupportActionBar().hide();

		// Do Full Screen
		WindowCompat.setDecorFitsSystemWindows(getWindow(), false);

		// Get window insets controller
		WindowInsetsControllerCompat windowInsetsControllerCompat = WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());

		// Tell above objects to hide system bars means status, navigation, captions bar and IME 
		windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars() | WindowInsetsCompat.Type.ime());

		
		// Do Landscape
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		// Setting bachground color to black
		getWindow().getDecorView().setBackgroundColor(Color.BLACK);

		GLESView glesView = new GLESView(this); 
		
		setContentView(glesView);
	}
	
	@Override
	protected void onPause()
	{
		super.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
	}
}