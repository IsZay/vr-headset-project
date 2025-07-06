#include <stdio.h>
#include <iostream>
#include <string.h>

// Necesary libraries for OpenGL
// sudo apt install libglfw3-dev libglew-dev freeglut3-dev
#include <GL/glew.h> // To be honest, I don't even think this is installed. I went to usr/include/GL and there is no glew.h file
#include <GL/freeglut.h> // sudo apt install freeglut3-dev

void display(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color to black (1 because completely opaque)
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer, don't know why

    // Look at the video, because this is really intersting
    // You make an origin in a 3d space
    // YOu have a 2d screen though
    // So when these 3d objects are rendered, they are projected onto the 2d screen
    // And they can get distorted especially at the ends of the screen
    // So you need to set the projection matrix, which is a matrix that transforms 3d coordinates into 2d screen coordinates
    glLoadIdentity(); // Load the identity matrix to reset transformations
    // People say that you only need to do this once, but I the chad said don't

    glBegin(GL_QUADS); // ONly in GL 2.0 // We want a quadrilateral (a square)
    glVertex3f(-0.5f, -0.5f, 0.0f); // Bottom left corner works from -1 to 1
    glVertex3f(-0.5f,  0.5f, 0.0f);// 1 is the top of the screen, -1 is the bottom of the screen
    glVertex3f( 0.5f,  0.5f, 0.0f);// 1 is the left of the screen, -1 is the right of the screen
    glVertex3f( 0.5f, -0.5f, 0.0f); // The order is important!

    glEnd(); // Only in GL 2.0
    
    
    
    glutSwapBuffers(); // Swap the buffers to display the rendered image

    // glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
}

int main(int argc, char** argv)
{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // | GLUT_DEPTH);

    //Set window stuff
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Square");

    //Point Glut to the display you want to use
    glutDisplayFunc(display);

    glutMainLoop(); // Infinite loop to keep the window open

    return 0;
}
/*
g++: The GNU C++ compiler.
gltset.cpp: The source file to compile.
-o gltest: Output the compiled program as an executable named gltest.
-Wall: Enable all the common warning messages to help catch potential issues in your code.
-I /usr/include: Add include to the list of directories to search for header files (often not needed, as it's a default include path).
-lGL: Link with the OpenGL library.
-lGLU: Link with the OpenGL Utility Library.
-lGLEW: Link with the OpenGL Extension Wrangler Library.
-lglut: Link with the OpenGL Utility Toolkit library.
In summary, this command compiles and links your OpenGL program with the necessary libraries for graphics rendering.

*/