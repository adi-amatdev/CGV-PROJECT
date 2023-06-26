#define GL_SILENCE_DEPRECATION

#include "myHeader.h"
#include<iostream>
using namespace std;
#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <string>
//-----------------------------------------------------------------------------------------------------------------------------------

// Global variable pool... must be accessible to all functions

// Window size
int width = 1000;
int height = 1000;


// arrow global variables
const float g = 9.8;    // acceleration due to gravity
float u = 100.0;        // initial velocity
float theta = 60.0;     // launch angle in degrees (angle of projection)
float t = 0.0;          // time
float x = 0.0, y = 0.0; // position of the projectile
float vx, vy, vz;       // velocity of the projectile along horizontal and vertical.
float dt = 0.01;        // time step the intervals we are calculating the position of the projectile.
float at = 0.09;
float archerHeight = 1.5;   // height of the archer (starting position we haven't drawn the archer actually.)
float animationSpeed = 1.0; // initial animation speed

float menuX = (width - 400) / 2.0;
float menuY = (height - 200) / 2.0;

//-----------------------------------------------------------------------------------------------------------------------------------

//flag section , contains flags for respective deciding of callbacks
//integer flags 
int DISPLAY_CALL_BACK_FLAG =0;

//bool flags
bool isHoveringOption1 = false;
bool isHoveringOption2 = false;
bool isHoveringOption3 = false;

bool aboutpageisHovering1 = false;
bool aboutpageisHovering2 = false;
//-----------------------------------------------------------------------------------------------------------------------------------
// support functions for the program

// called by call back function arrow_display()
void drawArrow(float x, float y, float angle)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(30.0, 20.0, 20.0);

    glBegin(GL_LINES);
    glVertex2f(0.0, 0.0);
    glVertex2f(2.0, 0.0);
    glVertex2f(2.0, 0.0);
    glVertex2f(1.5, 0.5);
    glVertex2f(2.0, 0.0);
    glVertex2f(1.5, -0.5);
    glEnd();

    // Draw the solid triangle head
    glBegin(GL_TRIANGLES);
    glVertex2f(2.0, 0.0);
    glVertex2f(1.5, 0.5);
    glVertex2f(1.5, -0.5);
    glEnd();

    glPopMatrix();
}
// called in drawMenu() and can be used anywhere to draw text
void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

// welcome message function
void drawWelcomeMessage()
{
    glColor3f(0.0, 1.0, 0.0); // Set text color to old green
    float welcomeX = (width - 200) / 2.0;
    float welcomeY = height - 50;
    drawText(welcomeX - 50, welcomeY - 300, "Welcome to Projectile Motion");
}


//invoked in keyboard func call back
void increaseSpeed()
{
    animationSpeed += 0.1;
}

void decreaseSpeed()
{
    if (animationSpeed > 0.1)
    {
        animationSpeed -= 0.1;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------

// this space contains display pages of the five pages required for the project, their only purpose
// is to be called and displayed along with necessary parameters.

//-----------------------------------------------------------------------------------------------------------------------------------

// call back functions
//this is for intro_page mouse function:

void mousefunc(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        switch (DISPLAY_CALL_BACK_FLAG){
            case 0:
            if (x >= 300 && x <= 700 && y >= 225 && y <= 275)
            {
            DISPLAY_CALL_BACK_FLAG = 1;
            glutPostRedisplay();
            }

            if (x >= menuX && x <= (menuX + 400) && y >= menuY && y <= menuY + 50)
            {
                exit(0);
            }

            if (x >= menuX && x <= menuX + 400 && y >= menuY - 100 && y <= menuY - 50)
            {
                DISPLAY_CALL_BACK_FLAG = 2;
                glutPostRedisplay();
            }
            break;
            case 2:

            break;
        }
    }
}

void onMouseMove(int x, int y)
{
    // Update hover status based on mouse position
    
   if(DISPLAY_CALL_BACK_FLAG ==1 || DISPLAY_CALL_BACK_FLAG ==0){
        if (x >= 300 && x <= 700 && y >= 225 && y <=275)
        {
            isHoveringOption1 = true;
            
        }
        else
        {
            isHoveringOption1 = false;
        }
        //box3
        if (x >= menuX && x <= (menuX + 400) && y >= menuY && y <= menuY + 50)
        {
            isHoveringOption2 = true;
        }
        else
        {
            isHoveringOption2 = false;
        }
        //box2
        if (x >= menuX && x <= menuX + 400 && y >= menuY - 100 && y <= menuY - 50)
        {
            isHoveringOption3 = true;
        }
        else
        {
            isHoveringOption3 = false;
        }
    }
    if(DISPLAY_CALL_BACK_FLAG ==2){
        if(x >= menuX && x<= menuX+400 && y>= menuY+120+200 && y<= menuY+170+200){
            aboutpageisHovering1 = true;
        }
        else {
            aboutpageisHovering1 = false;
        }
        if (x >= menuX && x <= menuX + 400 && y>=menuY+50+200 &&y<= menuY+100+200){
            aboutpageisHovering2 = true ;
        }
        else {
            aboutpageisHovering2 = false;
        }
    }


    
    
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
}

void myInit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 1000);
    glMatrixMode(GL_MODELVIEW);
}
void timer(int value)
{
    t += dt * animationSpeed; // Multiply time step by animation speed
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '+':
        increaseSpeed();
        break;
    case '-':
        decreaseSpeed();
        break;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------
// display call backs 
//DISPLAY CALL BACK PAGE1 
//drawing intro page
void draw_intro_page()
{
    // draw box all around
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 10);
    glVertex2f(990, 10);
    glVertex2f(990, 990);
    glVertex2f(10, 990);
    glEnd();

    glColor3f(1,0,0);
    drawText(500-100,700,"CGV PROJECT ");
    drawText(500-100-10,700-50,"WELCOME PAGE ");
    drawText(500-100-40,700-100,"PROJECTILE MOTION ");

    float menuX = (width - 400) / 2.0;
    float menuY = (height - 200) / 2.0;

    if (isHoveringOption3)
    {
        glColor3f(1.0, 0.0, 0.0); // Set box color to red when hovering
        glRectf(menuX, menuY + 50, menuX + 400, menuY);

        // 
    }
    else if (isHoveringOption2)
    {
        glColor3f(1.0, 0.0, 0.0); // Set box color to red when hovering
        glRectf(menuX, menuY - 50, menuX + 400, menuY - 100);
    }
    else if (isHoveringOption1)
    {
        glColor3f(1.0, 0.0, 0.0); // Set box color to red when hovering
        glRectf(menuX, menuY + 100, menuX + 400, menuY + 150);
    }
    else
    {
        glColor3f(0.0, 1.5, 0.0); // Set box color to old green
    }

    glColor3f(0.0, 1.5, 0.0); // Set box color to old green

    
    // Draw menu boxes
    // glRectf(menuX, menuY + 150, menuX + 400, menuY + 100);
    // glRectf(menuX, menuY + 50, menuX + 400, menuY);
    // glRectf(menuX, menuY - 50, menuX + 400, menuY - 100);
    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY + 150);
    glVertex2f(menuX + 400, menuY + 150);
    glVertex2f(menuX + 400, menuY + 100);
    glVertex2f(menuX, menuY + 100);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY + 50);
    glVertex2f(menuX + 400, menuY + 50);
    glVertex2f(menuX + 400, menuY);
    glVertex2f(menuX, menuY);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY - 50);
    glVertex2f(menuX + 400, menuY - 50);
    glVertex2f(menuX + 400, menuY - 100);
    glVertex2f(menuX, menuY - 100);
    glEnd();

    glColor3f(1.0, 1.0, 1.0); // Set text color to white

    // Draw menu options
    float optionX = menuX + 20;
    float optionY = menuY + 85;
    drawText(optionX - 120, optionY + 30, "Option 1");
    drawText(optionX - 120+100+150, optionY + 30, "MENU");

    optionY -= 100;
    drawText(optionX - 120, optionY + 30, "Option 2");
    drawText(optionX - 120+100+200-50, optionY + 30, "ABOUT");

    optionY -= 100;
    drawText(optionX - 120, optionY + 30, "Option 3");
    drawText(optionX - 120+100+200-40, optionY + 30, "EXIT");

    glutSwapBuffers();
}

// DISPLAY CALL BACK FOR PAGE 2

// drawing the menu page with projectile options
void drawMenu()
{
    // draw box all around
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 10);
    glVertex2f(990, 10);
    glVertex2f(990, 990);
    glVertex2f(10, 990);
    glEnd();

    glColor3f(1, 0, 0);
    drawText(500 , 700 - 100, "MENU");

    float menuX = (width - 400) / 2.0;
    float menuY = (height - 200) / 2.0;
    //third box
    if (isHoveringOption3)
    {
        glColor3f(1.0, 0.0, 0.0); // Set box color to red when hovering
        glRectf(menuX, menuY + 50, menuX + 455, menuY);
    }
    else
    {
        glColor3f(0.0, 1.5, 0.0); // Set box color to old green
    }
    //second box
    if (isHoveringOption2)
    {
        glColor3f(1.0, 0.0, 0.0); // Set box color to red when hovering
        glRectf(menuX, menuY - 50, menuX + 400, menuY - 100);
    }
    else
    {
        glColor3f(0.0, 1.5, 0.0); // Set box color to old green
    }

    if (isHoveringOption1)
    {
        glColor3f(1.0, 0.0, 0.0); // Set box color to red when hovering
        glRectf(menuX, menuY + 100, menuX + 480, menuY + 150);
    }
    

    glColor3f(0.0, 1.5, 0.0); // Set box color to old green

    // Draw menu boxes
    // glRectf(menuX, menuY + 150, menuX + 400, menuY + 100);
    // glRectf(menuX, menuY + 50, menuX + 400, menuY);
    // glRectf(menuX, menuY - 50, menuX + 400, menuY - 100);
    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY + 150);
    glVertex2f(menuX + 400+80, menuY + 150);
    glVertex2f(menuX + 400+80, menuY + 100);
    glVertex2f(menuX, menuY + 100);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY + 50);
    glVertex2f(menuX + 455, menuY + 50);
    glVertex2f(menuX + 455, menuY);
    glVertex2f(menuX, menuY);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY - 50);
    glVertex2f(menuX + 400, menuY - 50);
    glVertex2f(menuX + 400, menuY - 100);
    glVertex2f(menuX, menuY - 100);
    glEnd();

    glColor3f(1.0, 1.0, 1.0); // Set text color to white

    // Draw menu options
    float optionX = menuX + 20;
    float optionY = menuY + 85;
    drawText(optionX - 120, optionY + 30, "Option 1");
    drawText(optionX - 15, optionY + 30, "Input distance , obtain optimal angle , trajectory");

    optionY -= 100;
    drawText(optionX - 120, optionY + 30, "Option 2");
    drawText(optionX -10, optionY + 30, "input time and obtain velocity and other info");

    optionY -= 100;
    drawText(optionX - 120, optionY + 30, "Option 3");
    drawText(optionX +70, optionY + 30, "max height to be thrown ");

    glutSwapBuffers();
}

//display CALLBACK FOR PAGE 3 : ABOUT

void draw_about_page()
{
    // draw box all around
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 10);
    glVertex2f(990, 10);
    glVertex2f(990, 990);
    glVertex2f(10, 990);
    glEnd();

    glColor3f(1, 0, 0);
    drawText(500-50, 800 - 100, "ABOUT");
    // displaying info about project 
    { int x=300,y=650;
    std::ifstream aboutFile("about.txt");
    if (aboutFile.is_open())
    {
        std::string line;
        while (std::getline(aboutFile, line))
        {
            // Display each line of about info
            glRasterPos2i(x, y);
            for (char c : line)
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
            }
            y -= 20; // Move to the next line
        }
        aboutFile.close();
    }
    }

    float menuX = (width - 400) / 2.0;
    float menuY = (height - 200) / 2.0;
    // third box
    if (aboutpageisHovering1)
    {
        glColor3f(1.0, 0.0, 0.0); // Set box color to red when hovering
        glRectf(menuX, menuY -170-200, menuX + 400, menuY-120-200);
    }
    else
    {
        glColor3f(0.0, 1.5, 0.0); // Set box color to old green
    }
    // second box
    if (aboutpageisHovering2)
    {
        glColor3f(1.0, 0.0, 0.0); // Set box color to red when hovering
        glRectf(menuX, menuY - 50-200, menuX + 400, menuY - 100-200);
    }
    else
    {
        glColor3f(0.0, 1.5, 0.0); // Set box color to old green
    }


    glColor3f(0.0, 1.5, 0.0); // Set box color to old green

    // Draw menu boxes
    // glRectf(menuX, menuY + 150, menuX + 400, menuY + 100);
    // glRectf(menuX, menuY + 50, menuX + 400, menuY);
    // glRectf(menuX, menuY - 50, menuX + 400, menuY - 100);
   {
    menuY = 200;
    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY-170);
    glVertex2f(menuX + 400, menuY - 170);
    glVertex2f(menuX + 400, menuY-120);
    glVertex2f(menuX, menuY-120);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(menuX, menuY - 50);
    glVertex2f(menuX + 400, menuY - 50);
    glVertex2f(menuX + 400, menuY - 100);
    glVertex2f(menuX, menuY - 100);
    glEnd();
   
    glColor3f(1.0, 1.0, 1.0); // Set text color to white

    // Draw menu options
    float optionX = menuX + 20;
    float optionY = menuY + 85;
    drawText(optionX - 120, optionY -160, "Option 1");
    drawText(optionX+90, optionY -160, "BACK TO MENU");

    optionY -= 100;
    drawText(optionX - 120, optionY -140, "Option 2");
    drawText(optionX +150, optionY -140, "EXIT");
   }

    glutSwapBuffers();
}

// CALL BACK FUNCTION FOR ARROW 
void arrow_display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Calculate angle of velocity vector
    float velocityAngle = atan2(vy, vx) * 180.0 / M_PI;
    drawArrow(x, y, velocityAngle);

    // Draw the trajectory points
    glColor3f(1.0, 1.0, 1.0); // Set color for trajectory points
    glPointSize(2.0);         // Set point size for trajectory points
    glBegin(GL_POINTS);
    for (float i = 0.0; i <= t; i += at)
    {
        float xPos = u * cos(theta * M_PI / 180.0) * i;
        float yPos = archerHeight + u * sin(theta * M_PI / 180.0) * i - 0.5 * g * i * i;
        glVertex2f(xPos, yPos);
    }
    glEnd();

    // Update the position and velocity of the projectile
    x = u * cos(theta * M_PI / 180.0) * t;
    y = archerHeight + u * sin(theta * M_PI / 180.0) * t - 0.5 * g * t * t;
    vx = u * cos(theta * M_PI / 180.0);
    vy = u * sin(theta * M_PI / 180.0) - g * t;
    vz = 0.0;

    // Check if the projectile has hit the ground
    if (y < archerHeight)
    {
        t = 0.0;
        x = 0.0;
        y = archerHeight;
    }

    glutSwapBuffers();
}
//-----------------------------------------------------------------------------------------------------------------------------------
//MASTER DISPLAY FUNC : REGISTERED CALL BACK
void display(){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);


    // Check the current screen and display accordingly

    switch (DISPLAY_CALL_BACK_FLAG)
    {
    case 0: // Display the menu
        draw_intro_page();
        break;
    case 1: // Display option 1 screen
        drawMenu();
        break;
    case 2: // Display option 2 screen
        draw_about_page();
        break;
    case 3: // Display option 3 screen
        arrow_display();
        break;
    default:
        break;
    }

    glFlush();
}

//-----------------------------------------------------------------------------------------------------------------------------------
//MAIN FUNCTION FOR PROJECTILE MOTION

int main(int argc , char *argv[]){
    //we call all pages to main and use redisplayfuncs to handle them
    //we write call backs in main , display method with switch cases and remove resize func button for our window

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Projectile Motion");

    myInit();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard); // Register keyboard callback function
    glutMouseFunc(mousefunc);
    glutPassiveMotionFunc(onMouseMove);

    glutMainLoop();
    return 0;
}