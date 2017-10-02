/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"
#include <stdio.h>
#include "funct.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// CUBE ///////////////////////////////////////////////
// Create 3D object verticies
Vec4 vertices[36] =
{
    // Front Face 1
    {0.2, 0.2, 0.2, 1.0},              // top right
    {-0.2, 0.2, 0.2, 1.0},             // top left
    {-0.2, -0.2, 0.2, 1.0},            // bottom left
    
    {-0.2, -0.2, 0.2, 1.0},              // top right
    {0.2, -0.2, 0.2, 1.0},            // bottom left
    {0.2, 0.2, 0.2, 1.0},             // bottom right
    
    // Right Face 2
    {0.2, 0.2, -0.2, 1.0},             // top right
    {0.2, 0.2, 0.2, 1.0},              // top left
    {0.2, -0.2, 0.2, 1.0},             // bottom left
    
    {0.2, -0.2, 0.2, 1.0},             // top right
    {0.2, -0.2, -0.2, 1.0},             // bottom left
    {0.2, 0.2, -0.2, 1.0},            // bottom right
    
    // Back Face 3
    {0.2, 0.2, -0.2, 1.0},           // Bottom left
    {-0.2, 0.2, -0.2, 1.0},            // Top left
    {-0.2, -0.2, -0.2, 1.0},             // Top Right
    
    {-0.2, -0.2, -0.2, 1.0},              // Top Right
    {0.2, -0.2, -0.2, 1.0},            // Bottom Right
    {0.2, 0.2, -0.2, 1.0},             // Bottom Left
    
    // Left Face 4
    {-0.2, 0.2, -0.2, 1.0},              // top right
    {-0.2, 0.2, 0.2, 1.0},             // top left
    {-0.2, -0.2, 0.2, 1.0},            // bottom left
    
    {-0.2, -0.2, 0.2, 1.0},              // top right
    {-0.2, -0.2, -0.2, 1.0},            // bottom left
    {-0.2, 0.2, -0.2, 1.0},              // bottom right
    
    // Top Face 5
    {0.2, 0.2, -0.2, 1.0},              // top right
    {-0.2, 0.2, -0.2, 1.0},             // top left
    {-0.2, 0.2, 0.2, 1.0},            // bottom left
    
    {-0.2, 0.2, 0.2, 1.0},              // top right
    {0.2, 0.2, 0.2, 1.0},            // bottom left
    {0.2, 0.2, -0.2, 1.0},              // bottom right
    
    // Botto Face 6
    {0.2, -0.2, -0.2, 1.0},              // bottom left
    {-0.2, -0.2, -0.2, 1.0},             // top left
    {-0.2, -0.2, 0.2, 1.0},            // top right
    
    {-0.2, -0.2, 0.2, 1.0},              // top right
    {0.2, -0.2, 0.2, 1.0},            // bottom right
    {0.2, -0.2, -0.2, 1.0}              // bottom left
};

// Color each face of object
Vec4 colors[36] =
{
    {1.0, 0.0, 0.0, 1.0},    // red
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    
    {0.0, 0.0, 1.0, 1.0},    // Blue
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    
    {1.0, 0.0, 0.0, 1.0},    // red
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    
    {0.0, 0.0, 1.0, 1.0},    // Blue
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    
    {0.0, 1.0, 0.0, 1.0},    // Green
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    
    {0.0, 1.0, 0.0, 1.0},    // Green
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0}
};
 
// Declare number of verticies
int num_vertices = 36;
////////////////////////////////////////////////////////////////
/*
// Create 3D object verticies
Vec4 vertices[1140];


void initVertices()
{
    float DegreesToRadians = M_PI / 180.0;
    int k = 0;
    for (float phi = -90.0; phi <= 90.0; phi += 20.0)
    {
        float phir = phi * DegreesToRadians;
        float phir20 = (phi + 20.0)*DegreesToRadians;
        
        for(float theta = -180.0; theta <= 180.0; theta += 20.0)
        {
            float thetar = theta*DegreesToRadians;
            float thetar20 = (theta+20.0)*DegreesToRadians;
            
            vertices[k].x = sin(thetar)*cos(phir);
            vertices[k].y = cos(thetar)*cos(phir);
            vertices[k].z = sin(phir);
            vertices[k].w = 1.0;
            k++;
            
            vertices[k].x = sin(thetar)*cos(phir20);
            vertices[k].y = cos(thetar)*cos(phir20);
            vertices[k].z = sin(phir20);
            vertices[k].w = 1.0;
            k++;
            
            vertices[k].x = sin(thetar20)*cos(phir20);
            vertices[k].y = cos(thetar20)*cos(phir20);
            vertices[k].z = sin(phir20);
            vertices[k].w = 1.0;
            k++;
            
            
            vertices[k].x = sin(thetar20)*cos(phir20);
            vertices[k].y = cos(thetar20)*cos(phir20);
            vertices[k].z = sin(phir20);
            vertices[k].w = 1.0;
            k++;
            
            vertices[k].x = sin(thetar20)*cos(phir);
            vertices[k].y = cos(thetar20)*cos(phir);
            vertices[k].z = sin(phir);
            vertices[k].w = 1.0;
            k++;
            
            vertices[k].x = sin(thetar)*cos(phir);
            vertices[k].y = cos(thetar)*cos(phir);
            vertices[k].z = sin(phir);
            vertices[k].w = 1.0;
            k++;
        }
    }
}


// Color each face of object
Vec4 colors[1140];

void initColors()
{
    srand ( time(NULL) );
    
    for(int i = 0; i < 1140; i++)
    {
        colors[i].x = rand() / (float)RAND_MAX;
        colors[i].y = rand() / (float)RAND_MAX;
        colors[i].z = rand() / (float)RAND_MAX;
        colors[i].w = 1.0;
    }
}

// Declare number of verticies
int num_vertices = 1140;
*/

// Declare point & vector pointing from initial mouse click to origin
Vec4 originVector = {0.0,0.0,0.0,0.0};

// Declare vector of motion starting from origin point
Vec4 motionVector = {0.0,0.0,0.0,0.0};

// Declare axis of rotation
Vec4 rotationAxis = {0.0,0.0,0.0,0.0};

GLuint ctm_location;

Mat4 tr_matrix =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
    
};

Mat4 R =
{
    {1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {0.0,0.0,0.0,1.0}
};

int enableIdle = 0;
int leftDown = 1;

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));
    
    ctm_location = glGetUniformLocation(program, "ctm");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &tr_matrix);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    
    glutSwapBuffers();
}


void keyboard(unsigned char key, int mousex, int mousey)
{
    // Quit program
    if(key == 'q')
    {
        exit(0);
    }
    // Zoom In
    else if(key == 'o')
    {
        Mat4 tempMatrix = *scaleMatrix(&tr_matrix, 1.02, &tempMatrix);
        tr_matrix = tempMatrix;
  
    }
    // Zoom Out
    else if(key == 'l')
    {
        Mat4 tempMatrix = *scaleMatrix(&tr_matrix, 1.0/1.02, &tempMatrix);
        tr_matrix = tempMatrix;
       
    }
    else if(key == 'x')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutX(15.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
       
    }
    else if(key == 'X')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutX(-15.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;

    }
    else if(key == 'y')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutY(15.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
    
    }
    else if(key == 'Y')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutY(-15.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
       
    }
    else if(key == 'z')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutZ(15.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
      
    }
    else if(key == 'Z')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutZ(-15.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
       
    }
    else if(key == ' ')
    {
        R.col1.x = 1.0; R.col2.x = 0.0; R.col3.x = 0.0; R.col4.x = 0.0;
        R.col1.y = 0.0; R.col2.y = 1.0; R.col3.y = 0.0; R.col4.y = 0.0;
        R.col1.z = 0.0; R.col2.z = 0.0; R.col3.z = 1.0; R.col4.z = 0.0;
        R.col1.w = 0.0; R.col2.w = 0.0; R.col3.w = 0.0; R.col4.w = 1.0;
        
    }

    glutPostRedisplay();
    
}

void idle(void)
{
    if(enableIdle)
    {
        Mat4 tempMatrix = *matMultiplication(&R, &tr_matrix,&tempMatrix);
        tr_matrix = tempMatrix;
    }

    glutPostRedisplay();
}

// Listener for mouse button events
void mouse(int button, int state, int x, int y)
{
    // If button is pressed
    // button = GLUT LEFT BUTTON
    // state = GLUT_UP or GLUT_DOWN
    // The top-left corner of the screen is at (0, 0)
    // x y represent pointer position on screen
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            enableIdle = 0;
            // Can get initial click point to use for
            // calculation of axis of rotation

            originVector.x = x-256;
            originVector.y = 256-y;
            originVector.z = sqrt((256*256)-((x-256)*(x-256)));
            
        }
        else if(state == GLUT_UP)
        {
            if(originVector.x == motionVector.x &&
               originVector.y == motionVector.y &&
               originVector.z == motionVector.z
               ){}
            else
            {
                enableIdle = 1;
            }
        }
            
    }
    
    
    glutPostRedisplay();
}

void motion(int x, int y)
{
    // Capture moving x,y
    motionVector.x = x-256;
    motionVector.y = 256-y;
    motionVector.z = sqrt((256*256)-((x-256)*(x-256)));
    
    // If user drag is outside window, do nothing
    if(motionVector.x > 256 ||
       motionVector.y > 256 ||
       motionVector.z > 256 ||
       motionVector.x < -256 ||
       motionVector.y < -256 ||
       motionVector.z < -256
       ){}
    else
    {
        // Caclculate rotation axis and normalize it
        Vec4 tempVector = *crossProduct(&originVector, &motionVector, &tempVector);
        rotationAxis = tempVector;
        float rotationAxisMagnitude = vecMagnitude(&rotationAxis);
        
        // As long as the magnitude is not 0, which is the case
        // initially at the first moment of dragging
        if(rotationAxisMagnitude != 0)
        {
            // Normalize rotationAxis
            tempVector = *scalarMultVector(1/rotationAxisMagnitude, &rotationAxis, &tempVector);
            rotationAxis = tempVector;
            
            // Generate rotation matrix by calculating
            // Rx, Ry, Rz, being rotation about x,y,z axes.
            float theta = angleBetweenVectors(&originVector, &motionVector);
            float d = sqrt((rotationAxis.y*rotationAxis.y) + (rotationAxis.z*rotationAxis.z));
            
            Mat4 ry =
            {
                {d, 0.0, rotationAxis.x, 0.0},
                {0.0,1.0,0.0,0.0},
                {-rotationAxis.x,0.0,d,0.0},
                {0.0,0.0,0.0,1.0}
            };
        
            Mat4 rx =
            {
                {1.0,0.0,0.0,0.0},
                {0.0,rotationAxis.z/d,rotationAxis.y/d,0.0},
                {0.0,-rotationAxis.y/d,rotationAxis.z/d,0.0},
                {0.0,0.0,0.0,1.0}
            };
            
            
            Mat4 ryNeg =
            {
                {d, 0.0, -rotationAxis.x, 0.0},
                {0.0,1.0,0.0,0.0},
                {rotationAxis.x,0.0,d,0.0},
                {0.0,0.0,0.0,1.0}
            };
            
            Mat4 rxNeg =
            {
                {1.0,0.0,0.0,0.0},
                {0.0,rotationAxis.z/d,-rotationAxis.y/d,0.0},
                {0.0,rotationAxis.y/d,rotationAxis.z/d,0.0},
                {0.0,0.0,0.0,1.0}
            };
            
            
            //Generate R
            Mat4 tempMatrix1 = *matMultiplication(&ry, &rx, &tempMatrix1);
            Mat4 tempMatrix2 = *matRotateAboutZ(theta, &tempMatrix2);
            Mat4 tempMatrix3 = *matMultiplication(&tempMatrix2, &tempMatrix1,&tempMatrix3);
            Mat4 tempMatrix4 = *matMultiplication(&ryNeg, &tempMatrix3, &tempMatrix4);
            R = *matMultiplication(&rxNeg, &tempMatrix4, &R);
            
            
            // Apply R to current transformation matrix
            Mat4 tempMatrix5 = *matMultiplication(&R,&tr_matrix,&tempMatrix5);
            tr_matrix = tempMatrix5;
            
            // Reset initial point to last point in motion
            // in order to allow user to change axis of rotation
            // on demand
            originVector.x = motionVector.x;
            originVector.y = motionVector.y;
            originVector.z = motionVector.z;
         
            
        }
    }
    
    
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    
    //initVertices();
    //initColors();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 1");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
