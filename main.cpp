#define GL_SILENCE_DEPRECATION

#include<iostream>
using namespace std;
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <GL/freeglut.h>

//-----------------------------------------------------------------------------------------------------------------------------------

// Global variable pool... must be accessible to all functions

bool animationRunning = true;

// Window size
int width = 1000;
int height = 1000;

// Window dimensions
const int WINDOW_WIDTH = 3000;
const int WINDOW_HEIGHT = 3000;

// Cannon attributes
const float CANNON_BASE_RADIUS = 30.0f;
const float CANNON_BARREL_LENGTH = 100.0f;
const float CANNON_BARREL_WIDTH = 20.0f;

// Projectile attributes
const float PROJECTILE_RADIUS = 10.0f;
const float GRAVITY = 9.8f;

// Cannon position and rotation
float cannonX = CANNON_BASE_RADIUS;
float cannonY = CANNON_BASE_RADIUS;
float cannonAngle = 45.0f;

// Projectile position and velocity
float projectileX = 0.0f;
float projectileY = 0.0f;
float projectileVx = 0.0f;
float projectileVy = 0.0f;
float projectileSpeed = 5.0f; // Speed of the projectile
bool isProjectileLaunched = false;
bool isTrajectoryComplete = true;

// Mouse state
bool isLeftMouseButtonPressed = false;
int previousMouseX = 0;
int previousMouseY = 0;

// Rotation limits
const float MIN_ANGLE = 0.0f;
const float MAX_ANGLE = 90.0f;

// Trail history
std::vector<std::pair<float, float> > trailPoints;

// Time of flight and velocity variables
float timeOfFlight = 0.0f;
float velocity = 0.0f;

float menuX = (width - 400) / 2.0;
float menuY = (height - 200) / 2.0;
int id;

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

// Function to draw the cannon
void drawCannon()
{
    glColor3f(0.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(cannonX, cannonY, 0.0f);

    // Draw cannon base (a circle)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 360; i += 10)
    {
        float angleRad = i * M_PI / 180.0f;
        float x = CANNON_BASE_RADIUS * cos(angleRad);
        float y = CANNON_BASE_RADIUS * sin(angleRad);
        glVertex2f(x, y);
    }
    glEnd();

    // Draw cannon barrel (a rectangle)
    glPushMatrix();
    glTranslatef(CANNON_BASE_RADIUS, 0.0f, 0.0f);
    glRotatef(cannonAngle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, -CANNON_BARREL_WIDTH / 2.0f);
    glVertex2f(CANNON_BARREL_LENGTH, -CANNON_BARREL_WIDTH / 2.0f);
    glVertex2f(CANNON_BARREL_LENGTH, CANNON_BARREL_WIDTH / 2.0f);
    glVertex2f(0.0f, CANNON_BARREL_WIDTH / 2.0f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

// Function to draw the projectile
void drawProjectile()
{
    if (isProjectileLaunched)
    {
        glColor3f(1.0f, 0.0f, 0.0f);

        glPushMatrix();
        glTranslatef(projectileX, projectileY, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= 360; i += 10)
        {
            float angleRad = i * M_PI / 180.0f;
            float x = PROJECTILE_RADIUS * cos(angleRad);
            float y = PROJECTILE_RADIUS * sin(angleRad);
            glVertex2f(x, y);
        }
        glEnd();
        glPopMatrix();
    }
}

// Function to draw the trail
void drawTrail()
{
    glColor3f(0.0f, 0.0f, 1.0f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xAAAA);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
    for (const auto &point : trailPoints)
    {
        glVertex2f(point.first, point.second);
    }
    glEnd();

    glDisable(GL_LINE_STIPPLE);
}
// Function to update the projectile position
void updateProjectile()
{
    if (isProjectileLaunched)
    {
        projectileX += projectileVx;
        projectileY += projectileVy;

        projectileVy -= GRAVITY * 0.01f; // Adjust the time step as needed

        // Store the current position in the trail
        trailPoints.emplace_back(projectileX, projectileY);

        // Check if the projectile is out of bounds or hits the ground
        if (projectileX > WINDOW_WIDTH || projectileY < 0.0f)
        {
            isProjectileLaunched = false;
            trailPoints.clear();
            std::cout<<"PROJECTILE LAUNCHED:"<<std::endl<<std::endl;
            // Calculate and print the projectile information
            float angleOfProjection = cannonAngle;
            float horizontalDistance = projectileX - cannonX;
            float maxHeight = projectileY - cannonY;
            timeOfFlight = (2.0f * projectileVy) / GRAVITY;
            velocity = sqrt(pow(projectileVx, 2) + pow(projectileVy, 2));

            std::cout << "Angle of Projection: " << angleOfProjection << " degrees" << std::endl;
            std::cout << "Horizontal Distance Traveled: " << horizontalDistance << " units" << std::endl;
            std::cout << "Maximum Height Reached: " << -maxHeight << " units" << std::endl;
            std::cout << "Time of Flight: " << -timeOfFlight << " seconds" << std::endl;
            std::cout << "Velocity: " << velocity << " units/s" << std::endl;

            isTrajectoryComplete = true;
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------

// this space contains display pages of the five pages required for the project, their only purpose
// is to be called and displayed along with necessary parameters.

//-----------------------------------------------------------------------------------------------------------------------------------

// call back functions
//this is for intro_page mouse function:

void mousefunc(int button, int state, int x, int y){
    if(DISPLAY_CALL_BACK_FLAG == 6){
            if (button == GLUT_LEFT_BUTTON)
            {
                if (state == GLUT_DOWN)
                {
                    isLeftMouseButtonPressed = true;
                    previousMouseX = x;
                    previousMouseY = y;
                }
                else if (state == GLUT_UP)
                {
                    isLeftMouseButtonPressed = false;
                }
            }
        
    }
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
            if (x >= menuX && x <= menuX + 400 && y >= menuY + 120 + 200 && y <= menuY + 170 + 200)
            {
                exit(0);
            }
            if (x >= menuX && x <= menuX + 400 && y >= menuY + 50 + 200 && y <= menuY + 100 + 200)
            {
                DISPLAY_CALL_BACK_FLAG = 0;
            }
            break;
            case 1:
            if (x >= 300 && x <= 700 && y >= 225 && y <= 275)
            {
                DISPLAY_CALL_BACK_FLAG = 3;
                glutPostRedisplay();
            }

            if (x >= menuX && x <= (menuX + 400) && y >= menuY && y <= menuY + 50)
            {
                DISPLAY_CALL_BACK_FLAG = 0;
                glutPostRedisplay();
            }

            if (x >= menuX && x <= menuX + 400 && y >= menuY - 100 && y <= menuY - 50)
            {
                DISPLAY_CALL_BACK_FLAG = 4;
                glutPostRedisplay();
            }
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
void motion(int x, int y)
{
    cout<<"AIMING ....."<<endl;
    if (isLeftMouseButtonPressed)
    {
        int deltaX = x - previousMouseX;
        int deltaY = y - previousMouseY;
        cannonAngle += deltaX * 0.5f; // Adjust the rotation speed as needed

        // Limit the cannon rotation within the specified range
        if (cannonAngle < MIN_ANGLE)
        {
            cannonAngle = MIN_ANGLE;
        }
        else if (cannonAngle > MAX_ANGLE)
        {
            cannonAngle = MAX_ANGLE;
        }

        previousMouseX = x;
        previousMouseY = y;

        glutPostRedisplay();
    }
}
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

void myInit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 1000);
    glMatrixMode(GL_MODELVIEW);
}

// change this

// Function to update the scene
void update(int value)
{
    if (animationRunning)
    {
        // Your animation   update code here
        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
        updateProjectile();
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Function to handle keyboard events
void keyboard(unsigned char key, int x, int y)
{
    if (key == ' ')
    {
        if (!isProjectileLaunched && isTrajectoryComplete)
        {
            float angleRad = cannonAngle * M_PI / 180.0f;
            projectileX = cannonX + (CANNON_BARREL_LENGTH * cos(angleRad));
            projectileY = cannonY + (CANNON_BARREL_LENGTH * sin(angleRad));
            projectileVx = projectileSpeed * cos(angleRad);
            projectileVy = projectileSpeed * sin(angleRad);
            isProjectileLaunched = true;
            isTrajectoryComplete = false;
        }
    }
    if(DISPLAY_CALL_BACK_FLAG == 3){
        if(key == 's'){
            DISPLAY_CALL_BACK_FLAG = 6;
            glutPostRedisplay();
        }
    }
    if(DISPLAY_CALL_BACK_FLAG == 6){
        if(key =='m'){
            DISPLAY_CALL_BACK_FLAG = 0;
            glutPostRedisplay();
        }
        if(key == 'e') exit(0);
        if (key == 'p' || key == 'P')
        {
            animationRunning = !animationRunning; // Toggle animation flag
        }
    }
}
void specialKeyboard(int key, int x, int y)
{
    if (!isProjectileLaunched)
    {
        switch (key)
        {
        case GLUT_KEY_UP: // Increase speed on Up arrow key
            if (projectileSpeed < 14.0f)
                projectileSpeed += 1.0f;
            break;
        case GLUT_KEY_DOWN: // Decrease speed on Down arrow key
            if (projectileSpeed > 0.0f)
            {
                projectileSpeed -= 1.0f;
            }
            break;
        default:
            break;
        }

        float angleRad = cannonAngle * M_PI / 180.0f;
        projectileVx = projectileSpeed * cos(angleRad);
        projectileVy = projectileSpeed * sin(angleRad);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------
// display call backs
// simluation canon ball
// Function to display the scene
void subwindowDisplay()
{
        glColor3f(0,1,0);
       drawText(200, 650, "up and down arrow to increase power of projectile and vice versa");
       drawText(200, 600, "press space bar to launch");
       drawText(200,550, "use mouse to aim cannon by dragging");
       drawText(200, 500, "press 'p' to pause, 'p' to continue again");
       drawText(200, 450, "press 'm' to return to main menu");
       drawText(200, 400, "press 'e' to exit");
       glColor3f(1,0,0);
       drawText(200, 350, "press 's' key to start simulation");
      glutSwapBuffers();
}

void simulation_display()
{
    

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, 0.0f); // Set the origin to the bottom left of the window

    drawCannon();
    drawProjectile();
    drawTrail();

    glutSwapBuffers();
}

// under construction page
void under_construct_display()
{

    // Display "Under Construction" message
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    glRasterPos2f(width / 2 - 80, height / 2);
    const char *message = "Under Construction";
    for (int i = 0; message[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
    }

    
    glutSwapBuffers(); // Swap the front and back buffers to display the rendered scene
}

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
    drawText(optionX - 15+100+50, optionY + 30, "Simulate canon ");

    optionY -= 100;
    drawText(optionX - 120, optionY + 30, "Option 2");
    drawText(optionX -10+100+10, optionY + 30, "other functionalities");

    optionY -= 100;
    drawText(optionX - 120, optionY + 30, "Option 3");
    drawText(optionX +70+20, optionY + 30, "back to main menu ");

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
    case 3:
    subwindowDisplay();
    break;
    case 6: // Display option screen
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        simulation_display();
        break;
    case 4:
        under_construct_display();
        std::this_thread::sleep_for(std::chrono::seconds(3));
        DISPLAY_CALL_BACK_FLAG = 1;
        glutPostRedisplay();
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
    glutKeyboardFunc(keyboard); // Register keyboard callback function
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mousefunc);
    glutPassiveMotionFunc(onMouseMove);
    glutMotionFunc(motion);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}