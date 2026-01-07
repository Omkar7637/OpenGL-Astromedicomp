package com.amc.window;

import android.graphics.Color;
import android.view.Gravity;
import androidx.appcompat.widget.AppCompatTextView;
import android.content.Context;

public class MyTextView extends AppCompatTextView
{
    public MyTextView(Context context)
    {
        super(context);
        setTextColor(Color.rgb(0, 255, 0));
        setTextSize(60);
        setGravity(Gravity.CENTER);
        setText("Hello World!!!\n Omkar Kashid");
        
    }
}