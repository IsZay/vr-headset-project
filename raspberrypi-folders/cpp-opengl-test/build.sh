g++ gltest.cpp -o gltest -Wall -I /usr/include -lGL -lGLU -lGLEW -lglut
if [ $? == 0 ]
then 
    echo "Compilation successful"
    ./gltest
else
    echo "Compilation failed"
fi