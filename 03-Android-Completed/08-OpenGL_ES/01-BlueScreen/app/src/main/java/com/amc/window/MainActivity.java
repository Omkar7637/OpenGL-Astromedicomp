package com.amc.window;

// Importing Android OS and graphics packages
import android.os.Bundle;                 // Bundle is used to pass data between activities and save state
import android.graphics.Color;            // Color class to define colors for UI elements
import androidx.appcompat.app.AppCompatActivity; // Base class for activities using the Support Library action bar

// Orientation Related Package
import android.content.pm.ActivityInfo;   // Provides constants for screen orientation modes

// Packages for Full Screen
import androidx.core.view.WindowCompat;              // Provides backward-compatible methods for window operations
import androidx.core.view.WindowInsetsControllerCompat; // Allows controlling visibility of system bars (status/navigation)
import androidx.core.view.WindowInsetsCompat;        // Represents different types of system bars (status, navigation, IME, etc.)

// MainActivity inherits from AppCompatActivity
public class MainActivity extends AppCompatActivity
{
    // onCreate() is called when the Activity is first created
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState); // Call the superclass method to perform default activity setup

        // ===================== FORCE LANDSCAPE ORIENTATION =====================
        // Locks the activity in landscape mode
        // ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE is a constant that tells Android to fix orientation
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        // ===================== FULL SCREEN SETUP =====================
        // Hides the Action Bar (the top title bar in AppCompatActivity)
        getSupportActionBar().hide();

        // Makes the app draw edge-to-edge on the screen, ignoring system window insets
        // false = we want content to extend behind system bars (status/navigation)
        WindowCompat.setDecorFitsSystemWindows(getWindow(), false);

        // Create a controller to manage system bar visibility
        WindowInsetsControllerCompat windowInsetsControllerCompat =
                WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());
        
        // Hide all system bars (status bar, navigation bar) and IME (keyboard)
        // Type.systemBars() = status + navigation bars
        // Type.ime() = on-screen keyboard
        windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars() | WindowInsetsCompat.Type.ime());

        // ===================== WINDOW BACKGROUND =====================
        // Set the window background to black
        getWindow().getDecorView().setBackgroundColor(Color.BLACK);

        // ===================== GLSURFACEVIEW SETUP =====================
        // Create an instance of custom GLSurfaceView class
        GLESView glesView = new GLESView(this);
        // Set the content view of the activity to the GLSurfaceView
        setContentView(glesView);
    }
}
