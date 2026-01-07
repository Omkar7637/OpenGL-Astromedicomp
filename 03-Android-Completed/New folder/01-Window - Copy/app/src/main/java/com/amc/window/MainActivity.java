package com.amc.window;

import android.os.Bundle;
import android.graphics.Color;
import android.view.Gravity;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatTextView;

public class MainActivity extends AppCompatActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        getWindow().getDecorView().setBackgroundColor(Color.BLACK);
        AppCompatTextView myTextView = new AppCompatTextView(this);
        myTextView.setTextColor(Color.rgb(0, 255, 0));
        myTextView.setTextSize(60);
        myTextView.setGravity(Gravity.CENTER);
        myTextView.setText("Hello World!!!\n Omkar Kashid");
        
        setContentView(myTextView);
    }
}