package com.amc.window;

import android.graphics.Color;
import android.view.Gravity;
import androidx.appcompat.widget.AppCompatTextView;
import android.content.Context;

// Events related Packages
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

public class MyTextView extends AppCompatTextView implements OnGestureListener, OnDoubleTapListener {

    private GestureDetector gestureDetector;

    public MyTextView(Context context) {
        super(context);

        // TextView properties
        setTextColor(Color.rgb(0, 255, 0));
        setTextSize(60);
        setGravity(Gravity.CENTER);
        setText("Hello World!!!\n Omkar Kashid");

        // Initialize GestureDetector
        gestureDetector = new GestureDetector(context, this);
        gestureDetector.setOnDoubleTapListener(this);
    }

    // Let GestureDetector handle touch events
    @Override
    public boolean onTouchEvent(MotionEvent e) {
        if (!gestureDetector.onTouchEvent(e)) {
            return super.onTouchEvent(e);
        }
        return true;
    }

    // DoubleTapListener methods
    @Override
    public boolean onDoubleTap(MotionEvent e) {
        setText("DoubleTap");
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {
        setText("SingleTap");
        return true;
    }

    // OnGestureListener methods
    @Override
    public boolean onDown(MotionEvent e) {
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e) {
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        setText("Scroll");
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) {
        setText("LongPress");
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        return true;
    }
}
