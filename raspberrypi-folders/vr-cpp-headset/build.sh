g++ main.cpp uart.cpp -o vr_test -Wall -std=c++17 -I. -lEGL -lGLESv2
if [ $? == 0 ]
then
    echo "Build successful"
    # ./vr_test
else
    echo "Build failed"
    exit 1
fi