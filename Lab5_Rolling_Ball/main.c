/*
 Alessio Mazzone
 CS1566 Computer Graphics
 University of Pittsburgh
 
 Main file for Project 3
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

#define windowSize 375
#define groundVertices 6
// numvertices is 16206 for a 5 degree increment for sphere
#define sphereVertices 16206


///////////// Lookat and frustum variables/////////////////////////////////////////
float eyex = 0.0, eyey = 1.5, eyez = 1.5;
float atx = 0.0, aty = 0.0, atz = 0.0;
float left = -0.5, right = 0.5, bottom = -0.5, top = 0.5, near = -0.5, far = -100.0;
///////////////////////////////////////////////////////////////////////////////////

float DegreesToRadians = M_PI / 180.0;;


int enableIdle = 0;

GLuint projection_matrix_location, model_view_matrix_location, ctm_location;

Mat4 model_view_matrix =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
};

Mat4 projection_matrix =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
    
};

Vec4 sphere_vertices[sphereVertices];
Vec4 sphere_colors[sphereVertices];

Mat4 sphere_transformation =
{   {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
};

Mat4 sphere_rotation =
{
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}
};


// Ground vertices
Vec4 ground_vertices[groundVertices] =
{
    {-1.0, 0.0, -1.0, 1.0},
    {-1.0, 0.0,  1.0, 1.0},
    { 1.0, 0.0, -1.0, 1.0},
    
    { 1.0, 0.0, -1.0, 1.0},
    {-1.0, 0.0,  1.0, 1.0},
    { 1.0, 0.0,  1.0, 1.0}
};

// Ground colors
Vec4 ground_colors[groundVertices] =
{
    {0, 0, 0.5, 1.0},
    {0, 0, 0.5, 1.0},
    {0, 0, 0.5, 1.0},
    
    {0, 0, 0.5, 1.0},
    {0, 0, 0.5, 1.0},
    {0, 0, 0.5, 1.0}
};

Mat4 ground_transformation =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
};



void initSphere(float divisionDegrees)
{
    int v_index = 0;
    
    for (float phi = -90.0; phi <= 90.0; phi += divisionDegrees)
    {
        float phir = phi * DegreesToRadians;
        float phir20 = (phi + divisionDegrees)*DegreesToRadians;
        
        for(float theta = -180.0; theta <= 180.0; theta += divisionDegrees)
        {
            float thetar = theta*DegreesToRadians;
            float thetar20 = (theta + divisionDegrees)*DegreesToRadians;
        
            vecArrayAdd(sphere_vertices, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 1);
            vecArrayAdd(sphere_colors, v_index, 0, 0, 0, 1);;
            v_index++;
            
            vecArrayAdd(sphere_vertices, v_index, sin(thetar)*cos(phir20), cos(thetar)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(sphere_colors, v_index, 1, 1, 1, 1);
            v_index++;
            
            vecArrayAdd(sphere_vertices, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(sphere_colors, v_index, 0, 0, 0, 1);
            v_index++;
            
            vecArrayAdd(sphere_vertices, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(sphere_colors, v_index, 1, 1, 1, 1);
            v_index++;
            
            vecArrayAdd(sphere_vertices, v_index, sin(thetar20)*cos(phir), cos(thetar20)*cos(phir), sin(phir), 1);
            vecArrayAdd(sphere_colors, v_index, 0, 0, 0, 1);
            v_index++;
            
            vecArrayAdd(sphere_vertices, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 1);
            vecArrayAdd(sphere_colors, v_index, 1, 1, 1, 1);
            v_index++;
        }
    }
}


void init(void)
{
    // Initialize model_view matrix
    Mat4 temp1 = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
    model_view_matrix = temp1;
    
    // Initialize frustum
    temp1 = frustum(left, right, bottom, top, near, far);
    projection_matrix = temp1;

    // Initialize sphere
    Mat4 scaling_matrix;

    initSphere(5.0);
        
    // Generate scaling matrix
    scaling_matrix = *scaleMatrix(0.5, &scaling_matrix);
   
    // Apply scaling, then translation
    temp1 = *matMultiplication(&scaling_matrix, &sphere_transformation, &temp1);
    sphere_transformation = temp1;
   
    
    // Initialize size of total vertices and colors
    size_t size_of_all_vertices = sizeof(sphere_vertices) + sizeof(ground_vertices);
    size_t size_of_all_colors = sizeof(sphere_colors) + sizeof(ground_colors);
    

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    
    // Generate buffer to store normals and vertices
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size_of_all_vertices + size_of_all_colors, NULL, GL_STATIC_DRAW);
    
    // Load ground & sphere vertices
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_vertices), ground_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_vertices), sizeof(sphere_vertices), sphere_vertices);
    
    // Load ground & sphere colors
    glBufferSubData(GL_ARRAY_BUFFER, size_of_all_vertices, sizeof(ground_colors), ground_colors);
    glBufferSubData(GL_ARRAY_BUFFER, size_of_all_vertices + sizeof(ground_colors), sizeof(sphere_colors), sphere_colors);
    
    // Send in position
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    // Send in normal
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) size_of_all_colors);
    
    // Load in matricies to the vertex shader
    projection_matrix_location = glGetUniformLocation(program, "projection_matrix");
    model_view_matrix_location = glGetUniformLocation(program, "model_view_matrix");
    ctm_location = glGetUniformLocation(program, "ctm");
    

    glEnable(GL_DEPTH_TEST);

    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    
    // Load frustum
    glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);
    
    // Load Model view
    glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);

    // Load Ground information
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ground_transformation);
    glDrawArrays(GL_TRIANGLES, 0, ground_vertices);
    
    // Load Sphere information
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &sphere_transformation);
    glDrawArrays(GL_TRIANGLES, 0, sphere_vertices);
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    // Quit program
    if(key == 'q')
    {
        exit(0);
    }
    // Starts rotation
    else if (key == ' ')
    {
     
    }

    glutPostRedisplay();
    
}

void idle(void)
{
    Mat4 temp;
    
    // Generate rotation matrix for each sphere
    temp = *matRotateAboutY(5, &temp);
    sphere_rotation = temp;
            
    // Apply rotation matrix to transofmation
    temp = *matMultiplication(&sphere_rotation, &sphere_transformation, &temp);
    sphere_transformation = temp;
            
    glutPostRedisplay();
    
}



int main(int argc, char **argv)
{
    
    // OpenGL initializaiton code
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize*2, windowSize*2);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lab5 - Rolling Ball");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
