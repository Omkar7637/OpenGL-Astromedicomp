package com.amc.window;

import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    EditText etMass, etAccel, etAngle, etWheelRadius, etGearRatio, etWheelCount;
    TextView tvOutput;
    Button btnCalculate;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etMass = findViewById(R.id.etMass);
        etAccel = findViewById(R.id.etAccel);
        etAngle = findViewById(R.id.etAngle);
        etWheelRadius = findViewById(R.id.etWheelRadius);
        etGearRatio = findViewById(R.id.etGearRatio);
        etWheelCount = findViewById(R.id.etWheelCount);
        tvOutput = findViewById(R.id.tvOutput);
        btnCalculate = findViewById(R.id.btnCalculate);

        tvOutput.setMovementMethod(new ScrollingMovementMethod());

        btnCalculate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                calculateAndDisplay();
            }
        });
    }

    // Helper method for safe double parsing with default value
    private double parseDouble(EditText et, double defaultValue) {
        String s = et.getText().toString().trim();
        if (s.isEmpty()) return defaultValue;
        try {
            return Double.parseDouble(s);
        } catch (NumberFormatException e) {
            return defaultValue;
        }
    }

    // Helper method for safe int parsing with default value
    private int parseInt(EditText et, int defaultValue) {
        String s = et.getText().toString().trim();
        if (s.isEmpty()) return defaultValue;
        try {
            return Integer.parseInt(s);
        } catch (NumberFormatException e) {
            return defaultValue;
        }
    }

    private void calculateAndDisplay() {
        // Read inputs safely
        double mass = parseDouble(etMass, 25.0);
        double accel = parseDouble(etAccel, 0.5);
        double angle = parseDouble(etAngle, 5.0);
        double wheelRadius = parseDouble(etWheelRadius, 0.1);
        double gearRatio = parseDouble(etGearRatio, 20.0);
        int wheelCount = parseInt(etWheelCount, 4);

        // Constants
        double g = 9.81;
        double eta = 0.8;
        double mu = 0.02;

        // Forces
        double F_normal = mass * g * Math.cos(Math.toRadians(angle));
        double Fr = mu * F_normal;
        double Fg = mass * g * Math.sin(Math.toRadians(angle));
        double Fa = mass * accel;
        double Ftotal = Fg + Fa + Fr;

        // Torque
        double Twheel = Ftotal * wheelRadius;
        double Twheel_per_wheel = Twheel / wheelCount;
        double Tmotor = Twheel_per_wheel / (gearRatio * eta);

        // RPM
        double wheelRPM = wheelRadius != 0 ? (60 * accel / (2 * Math.PI * wheelRadius)) : 0;
        double motorRPM = wheelRPM * gearRatio;

        // Build output string
        String output = String.format(
                "Input Parameters:\n" +
                        "Mass: %.2f kg\nAcceleration: %.2f m/s²\nIncline Angle: %.2f deg\nWheel Radius: %.2f m\nGear Ratio: %.2f\nNumber of Wheels: %d\n\n" +
                        "Step 1: Gravitational Force along incline\nFg = m*g*sin(theta) = %.2f N\n\n" +
                        "Step 2: Acceleration Force\nFa = m*a = %.2f N\n\n" +
                        "Step 3: Friction Calculations\nFnormal = %.2f N, Friction = %.2f N, Ftotal = %.2f N\n\n" +
                        "Step 4: Wheel Torque\nTwheel (total) = %.2f N.m, per wheel = %.2f N.m\n\n" +
                        "Step 5: Motor Torque\nTmotor = %.2f N.m\n\n" +
                        "Step 6: Wheel RPM = %.2f RPM\nStep 7: Motor RPM = %.2f RPM",
                mass, accel, angle, wheelRadius, gearRatio, wheelCount,
                Fg, Fa, F_normal, Fr, Ftotal,
                Twheel, Twheel_per_wheel, Tmotor,
                wheelRPM, motorRPM
        );

        tvOutput.setText(output);
    }
}
