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
#define sphereVertices 16206
///////// num_vertices is 16206 for a 5 degree increment
#define num_vertices sphereVertices + groundVertices
////////////////////////////////////////////////////////

Vec4 vertices[num_vertices];
Vec4 colors[num_vertices];

///////////// Lookat and frustum variables/////////////////////////////////////////
float eyex = 0.0, eyey = 1.5, eyez = 1.5;
float atx = 0.0, aty = 0.0, atz = 0.0;
float left = -0.5, right = 0.5, bottom = -0.5, top = 0.5, near = -0.5, far = -100.0;
///////////////////////////////////////////////////////////////////////////////////

float DegreesToRadians = M_PI / 180.0;;

Mat4 model_view_rotation =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
};


int enableIdle = 0;

// vertices[] and colors[] index
int v_index = 0;

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

// Transformation matricies for sphere and ground
Mat4 transformation_matricies[2] =
{
    {{1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}},
    
    {{1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}},
};

// ground vertices
Vec4 ground_vertices[groundVertices] =
{
    {-2.0, 0.0, -2.0, 1.0},
    {-2.0, 0.0,  2.0, 1.0},
    { 2.0, 0.0, -2.0, 1.0},
    
    { 2.0, 0.0, -2.0, 1.0},
    {-2.0, 0.0,  2.0, 1.0},
    { 2.0, 0.0,  2.0, 1.0}
};

// Rotation matrix for sphere
Mat4 rotation_matrix[1] =
{
    {{1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}}
};



void initSphere(float divisionDegrees)
{
    
    for (float phi = -90.0; phi <= 90.0; phi += divisionDegrees)
    {
        float phir = phi * DegreesToRadians;
        float phir20 = (phi + divisionDegrees)*DegreesToRadians;
        
        for(float theta = -180.0; theta <= 180.0; theta += divisionDegrees)
        {
            float thetar = theta*DegreesToRadians;
            float thetar20 = (theta + divisionDegrees)*DegreesToRadians;
        
            vecArrayAdd(vertices, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 1);
            vecArrayAdd(colors, v_index, 0, 0, 0, 1);;
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar)*cos(phir20), cos(thetar)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(colors, v_index, 1, 1, 1, 1);
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(colors, v_index, 0, 0, 0, 1);
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(colors, v_index, 1, 1, 1, 1);
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar20)*cos(phir), cos(thetar20)*cos(phir), sin(phir), 1);
            vecArrayAdd(colors, v_index, 0, 0, 0, 1);
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 1);
            vecArrayAdd(colors, v_index, 1, 1, 1, 1);
            v_index++;
        }
    }
}



void initGround()
{
    for(int i = 0; i < groundVertices; i++)
    {
        vecArrayAdd(vertices, v_index, ground_vertices[i].x, ground_vertices[i].y, ground_vertices[i].z, 1);
        vecArrayAdd(colors, v_index, 0, 0, 0.5, 1);
        
        v_index++;
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


    // Initialize spher
    // x,y,z coordinates sphere centers
    Mat4 scaling_matrix;

    initSphere(5.0);
        
    // Generate scaling matrix
    scaling_matrix = *scaleMatrix(0.5, &scaling_matrix);
   
    // Apply scaling, then translation
    temp1 = *matMultiplication(&scaling_matrix, &transformation_matricies[0], &temp1);
    transformation_matricies[0] = temp1;
   

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    
    // Generate buffer to store normals and vertices
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    
    
    // Send in position
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    
    // Send in normal
    GLuint colors = glGetAttribLocation(program, "Colors");
    glEnableVertexAttribArray(colors);
    glVertexAttribPointer(colors, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(colors));
    
    
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
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &transformation_matricies[1]);
    glDrawArrays(GL_TRIANGLES, 0, groundVertices);
    
    // Load Sphere information
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &transformation_matricies[i]);
    glUniform1i(isShadow_location, 0);
    glDrawArrays(GL_TRIANGLES, groundVertices + (sphereVertices * i), sphereVertices);



    
    
    
    
    glutSwapBuffers();
}







void keyboard(unsigned char key, int mousex, int mousey)
{
    // Quit program
    if(key == 'q')
    {
        exit(0);
    }
    // Controls for eye
    else if(key == 'l')
    {
        eyex -= 0.2;
    }
    else if(key == 'L')
    {
        eyex += 0.2;
    }
    else if(key == 'o')
    {
        eyey -= 0.2;
    }
    else if(key == 'O')
    {
        eyey += 0.2;
    }
    else if(key == 'p')
    {
        eyez -= 0.2;
        
    }
    else if(key == 'P')
    {
        eyez += 0.2;
    }
    // Controls for light position
    else if(key == 'x')
    {
        LightPosition.x -= 0.05;
    }
    else if(key == 'X')
    {
        LightPosition.x += 0.05;
    }
    else if(key == 'y')
    {
        LightPosition.y -= 0.05;
    }
    else if(key == 'Y')
    {
        LightPosition.y += 0.05;
    }
    else if(key == 'z')
    {
        LightPosition.z -= 0.05;
    }
    else if(key == 'Z')
    {
        LightPosition.z += 0.05;
    }
    // Starts rotation
    else if (key == ' ')
    {
        if(enableIdle)
        {
            enableIdle = 0;
        }
        else
        {
            enableIdle = 1;
        }
    }

    // Recalculate model_view matrix
    Mat4 temp1 = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
    model_view_matrix = temp1;
    
    
    // Recalculate light ball translation
    Mat4 scaling_matrix = *scaleMatrix(.02, &scaling_matrix);
    Mat4 translation_matrix = *translate(LightPosition.x,LightPosition.y,LightPosition.z, &translation_matrix);
    
    // Apply scaling, then translation
    temp1 = *matMultiplication(&translation_matrix, &scaling_matrix, &temp1);
    transformation_matricies[5] = temp1;
    
    
    
    glutPostRedisplay();
    
}





void idle(void)
{
    int i = 0;
    Mat4 temp;
    
    if(enableIdle)
    {
        // Start at 1 and go to num_spheres -1 to ignore middle sphere and light ball
        for(i = 1; i < num_spheres-1; i++)
        {
            // Generate rotation matrix for each sphere
            temp = *matRotateAboutY(degrees[i], &temp);
            rotation_matrices[i] = temp;
            
            // Apply rotation matrix to transofmation
            temp = *matMultiplication(&rotation_matrices[i], &transformation_matricies[i], &temp);
            transformation_matricies[i] = temp;
            
            glutPostRedisplay();
        }
        
    }
    glutPostRedisplay();
}



int main(int argc, char **argv)
{
    initGround();
    
    // OpenGL initializaiton code
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize*2, windowSize*2);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 3 - Lights");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
