package com.amc.window;

import android.os.Bundle;
import android.graphics.Color;
import androidx.appcompat.app.AppCompatActivity;

// Pakages For Full Screen
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.core.view.WindowInsetsCompat;


public class MainActivity extends AppCompatActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        // Full Screen
        // Hides the status bar and action bar to provide a full-screen experience
        // Hide Action Bar
        getSupportActionBar().hide();
        // Tell the Android System to make your Window Expand edge to edge of screen
        WindowCompat.setDecorFitsSystemWindows(getWindow(), false);
        // get window insets controller
        WindowInsetsControllerCompat windowInsetsControllerCompat =
                WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());

        // tell this inste controller to hide remaning bars and insets
        windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars() | WindowInsetsCompat.Type.ime());
        



        // Set background color of window to black
        getWindow().getDecorView().setBackgroundColor(Color.BLACK);
        MyTextView mytextview = new MyTextView(this);
        setContentView(mytextview);
    }
}