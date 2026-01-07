package com.amc.window;

import android.os.Bundle;
import android.graphics.Color;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        getWindow().getDecorView().setBackgroundColor(Color.BLACK);
        MyTextView mytextview = new MyTextView(this);
        setContentView(mytextview);
    }
}