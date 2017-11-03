/*
 Alessio Mazzone
 CS1566 Computer Graphics
 University of Pittsburgh
 
Main file for Project 2
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
#define num_spheres 5
#define groundVertices 6
#define sphereVertices 16206
///////// num_vertices is 16206 for a 5 degree increment
#define num_vertices (num_spheres*sphereVertices) + groundVertices
////////////////////////////////////////////////////////

Vec4 vertices[num_vertices];
Vec4 colors[num_vertices];

///////////// Lookat and frustum variables/////////////////////////////////////////
float eyex = 0, eyey = 0, eyez = 1;
float atx = 0.0, aty = 0.0, atz = 0.0;
float left = -0.5, right = 0.5, bottom = -0.5, top = 0.5, near = -0.5, far = -100.0;
///////////////////////////////////////////////////////////////////////////////////

float DegreesToRadians = M_PI / 180.0;;

int enableIdle = 0;

// degrees for rotation of each sphere
float degrees[5] = {1,1.2,1.4,1.6,1.8};

// vertices[] and colors[] index
int v_index = 0;

// sphere_color index
int c_index = 0;

GLuint projection_matrix_location;
GLuint model_view_matrix_location;
GLuint ctm_location;


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

// Transformation matricies for each sphere
Mat4 transformation_matricies[num_spheres] =
{
    {{1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}},
    
    {{1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}},
    
    {{1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}},
    
    {{1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}},
    
    {{1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}},
};

// Rotation matrices for each sphere
Mat4 rotation_matrices[num_spheres] =
{
    {{1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}},
    
    {{1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}},
    
    {{1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}},
    
    {{1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}},
    
    {{1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}},
};


// Color for each of the 5 spheres
Vec4 sphere_colors[5] =
{
    {1,1,0,1},          // Yellow
    {1,0,0,1},          // Red
    {0,1,0,1},          // Green
    {0,0,1,1},          // Blue
    {1,.65,0,1}         // Orange
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
            
            vecArrayAdd(&vertices, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 1);
            vecArrayAdd(&colors, v_index, sphere_colors[c_index].x, sphere_colors[c_index].y, sphere_colors[c_index].z, 1);
            v_index++;
            
            vecArrayAdd(&vertices, v_index, sin(thetar)*cos(phir20), cos(thetar)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(&colors, v_index, sphere_colors[c_index].x, sphere_colors[c_index].y, sphere_colors[c_index].z, 1);
            v_index++;
  
            vecArrayAdd(&vertices, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(&colors, v_index, sphere_colors[c_index].x, sphere_colors[c_index].y, sphere_colors[c_index].z, 1);
            v_index++;
            
            vecArrayAdd(&vertices, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(&colors, v_index, sphere_colors[c_index].x, sphere_colors[c_index].y, sphere_colors[c_index].z, 1);
            v_index++;
            
            vecArrayAdd(&vertices, v_index, sin(thetar20)*cos(phir), cos(thetar20)*cos(phir), sin(phir), 1);
            vecArrayAdd(&colors, v_index, sphere_colors[c_index].x, sphere_colors[c_index].y, sphere_colors[c_index].z, 1);
            v_index++;
             
            vecArrayAdd(&vertices, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 1);
            vecArrayAdd(&colors, v_index, sphere_colors[c_index].x, sphere_colors[c_index].y, sphere_colors[c_index].z, 1);
            v_index++;
        }
    }
    
    // Increment sphere_color each time a new sphere is made
    c_index++;
}


void initGround()
{
    
    
}



void init(void)
{
    // Initialize model_view matrix
    Mat4 temp1 = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
    model_view_matrix = temp1;
    
    Mat4 temp2;
    
    // Initialize frustum
    temp1 = frustum(left, right, bottom, top, near, far);
    projection_matrix = temp1;
    
    // Initialize spheres
    // x,y,z coordinates sphere centers
    float x = 0, y = 0 , z = 0;
    Mat4 scaling_matrix;
    Mat4 translation_matrix;
    Mat4 transformed;

    for(int i = 0; i < num_spheres; i++)
    {
        initSphere(5.0);
        
        // Generate scaling matrix
        scaling_matrix = *scaleMatrix(0.1, &scaling_matrix);
        
        // Generate Translation matrix
        translation_matrix = *translate(x, y, z, &translation_matrix);
        
        // Apply scaling, then translation, then finally model view
        temp1 = *matMultiplication(&translation_matrix, &scaling_matrix, &temp1);
        temp2 = *matMultiplication(&model_view_matrix, &temp1, &temp2);
        transformation_matricies[i] = temp2;
        
        // Move center of next sphere
        x += 0.2;
    }
    
    
    
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
    
    // For Reference:
    //glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
    //glDrawArrays(GL_TRIANGLES, START_INDEX, HOW_MANY_VERTICES_TO_DRAW);
    
    
    // Draw 5 spheres
    for(int i = 0; i < num_spheres; i++)
    {
        glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &transformation_matricies[i]);
        glDrawArrays(GL_TRIANGLES, sphereVertices * i, sphereVertices);
    }

    
    
    
    
    glutSwapBuffers();
}





void keyboard(unsigned char key, int mousex, int mousey)
{
    // Quit program
    if(key == 'q')
    {
        exit(0);
    }
    else if(key == 'x')
    {
        eyex -= 0.05;
    }
    else if(key == 'X')
    {
        eyex += 0.05;
    }
    else if(key == 'y')
    {
        eyey -= 0.05;
    }
    else if(key == 'Y')
    {
        eyey += 0.05;
    }
    else if(key == 'z')
    {
        eyez -= 0.05;
    }
    else if(key == 'Z')
    {
        eyez += 0.05;
    }
    else if (key == ' ')
    {
        enableIdle = 1;
    }
    else if (key == 't')
    {
        top -=.1;
    }
    else if (key == 'T')
    {
        top +=.1;
    }
    else if (key == 'b')
    {
        bottom -= .1;
    }
    else if (key == 'B')
    {
        bottom += .1;
    }
    else if (key == 'l')
    {
        left -= .1;
    }
    else if (key == 'L')
    {
        left += .1;
    }
    else if (key == 'r')
    {
        right -= .1;
    }
    else if (key == 'R')
    {
        right += .1;
    }
    else if (key == 'n')
    {
        near -= .1;
    }
    else if (key == 'N')
    {
        near += .1;
    }
   
    printf("Top: %f, Bottom: %f\n", top, bottom);
    printf("Left: %f, Right: %f\n", left, right);
    printf("Near: %f, Far: %f\n", near, far);
    
    // Initialize frustum
    Mat4 temp1 = frustum(left, right, bottom, top, near, far);
    projection_matrix = temp1;
    
    glutPostRedisplay();
    
}




void idle(void)
{
    int i = 0;
    Mat4 temp;
    
    if(enableIdle)
    {
        for(i = 0; i < num_spheres; i++)
        {
            // Generate rotation matrix for each sphere
            temp = *matRotateAboutY(degrees[i], &temp);
            rotation_matrices[i] = temp;
            
            // Apply rotation matrix
            temp = *matMultiplication(&rotation_matrices[i], &transformation_matricies[i], &temp);
            transformation_matricies[i] = temp;
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
