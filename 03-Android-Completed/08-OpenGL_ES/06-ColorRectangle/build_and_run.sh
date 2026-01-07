#!/bin/bash
# =============================================
# Android Project Build, Install, and Log Script (Portable)
# =============================================

echo
echo "=== Android Project Build, Install, and Log ==="

# -------------------------------
# Detect and configure Windows Java if missing
# -------------------------------
if ! command -v java &> /dev/null; then
    echo "Java not found in PATH. Trying to use Windows Android Studio JBR..."
    WIN_JAVA_PATH="/mnt/c/Program Files/Android/Android Studio/jbr"
    if [ -d "$WIN_JAVA_PATH" ]; then
        export JAVA_HOME="$WIN_JAVA_PATH"
        export PATH="$JAVA_HOME/bin:$PATH"
        alias java="$JAVA_HOME/bin/java.exe"
        echo "JAVA_HOME set to: $JAVA_HOME"
    else
        echo "Windows JBR not found at expected path: $WIN_JAVA_PATH"
        echo "Please check your installation."
        exit 1
    fi
fi

# Verify Java setup
echo
echo "--- Checking Java ---"
java -version || { echo "Java setup failed!"; exit 1; }

# -------------------------------
# Build APK in current directory
# -------------------------------
echo
echo "--- Building APK ---"
./gradlew build

if [ $? -ne 0 ]; then
    echo "Build failed! Exiting..."
    exit 1
fi

# -------------------------------
# Install APK on connected device
# -------------------------------
echo
echo "--- Installing APK ---"
adb -d install -r "app/build/outputs/apk/debug/app-debug.apk"

if [ $? -ne 0 ]; then
    echo "APK installation failed! Exiting..."
    exit 1
else
    echo "APK installed successfully!"
fi

# -------------------------------
# Start filtered logcat
# -------------------------------
echo
echo "--- Starting Logcat (Filtered for 'AMC:') ---"
echo "Press CTRL + C to stop log output."
echo

adb logcat | grep -i "AMC:"
