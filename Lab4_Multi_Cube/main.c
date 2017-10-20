/*
 Alessio Mazzone
 CS1566 Computer Graphics
 University of Pittsburgh
 
Main file for Project 1
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

Vec4 vertices[36*4];
Vec4 colors[36*4];
GLuint ctm_location;
int num_vertices = 36*4;
float cubes_123_degrees = 5;
float cube_4_degrees = 5;

Vec4 cube_vertices[36] =
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


Mat4 cube_tr_matrix[4] =
{
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {-0.5,0.5,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {0.5,0.5,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {-0.5,-0.5,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {0.5,-0.5,0.0,1.0}}
};

Mat4 cube_R_matrix[4] =
{
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {-0.5,0.5,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {0.5,0.5,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {-0.5,-0.5,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {0.5,-0.5,0.0,1.0}}
};


// Color each face of object
Vec4 cube_colors[36] =
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


void initCubeVertices()
{
    int i, j, v_index = 0;
    
    for( i = 0; i < 4; i++)
    {
        for( j = 0; j < 36; j++)
        {
            vertices[v_index] = cube_vertices[j];
            colors[v_index] = cube_colors[j];
            v_index++;
        }
    
    }
}

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
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    
    // Draw Cube 1
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_tr_matrix[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Draw Cube 2
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_tr_matrix[1]);
    glDrawArrays(GL_TRIANGLES, 36, 36);
    // Draw Cube 3
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_tr_matrix[2]);
    glDrawArrays(GL_TRIANGLES, 72, 36);
    // Draw Cube 4
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_tr_matrix[3]);
    glDrawArrays(GL_TRIANGLES, 108, 36);
    
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    // Quit program
    if(key == 'q')
    {
        exit(0);
    }

    glutPostRedisplay();
    
}

// Have 4 cubes rotate continually by updating their degree
// of rotation every time idle() is called.
void idle(void)
{
    // Cube 1: COM located @ (-0.5,0.5,0.0) and spins about X, fixed pt of rotation is COM
    // Cube 2: COM located @ (0.5,0.5,0.0) and spins about Y, fixed pt of rotation is COM
    // Cube 3: COM located @ (-0.5,-0.5,0.0) and spins about Z, fixed pt of rotation is COM
    // Cube 4: COM located @ (0.5,-0.5,0.0) and spins about X, fixed pt of rotation is COM.
    //         Make Cube 4 spin faster than Cube 1.
    
    // Update Cube 1
    Mat4 rotation = *matRotateAboutX(cubes_123_degrees, &rotation);
    cube_tr_matrix[0] = rotation;
    Mat4 tempMatrix = *matMultiplication(&cube_R_matrix[0], &cube_tr_matrix[0],&tempMatrix);
    cube_tr_matrix[0] = tempMatrix;
    
    // Update Cube 2
    rotation = *matRotateAboutY(cubes_123_degrees, &rotation);
    cube_tr_matrix[1] = rotation;
    tempMatrix = *matMultiplication(&cube_R_matrix[1], &cube_tr_matrix[1],&tempMatrix);
    cube_tr_matrix[1] = tempMatrix;
    
    // Update Cube 3
    rotation = *matRotateAboutZ(cubes_123_degrees, &rotation);
    cube_tr_matrix[2] = rotation;
    tempMatrix = *matMultiplication(&cube_R_matrix[2], &cube_tr_matrix[2],&tempMatrix);
    cube_tr_matrix[2] = tempMatrix;
    
    // Update Cube 4
    rotation = *matRotateAboutX(cube_4_degrees, &rotation);
    cube_tr_matrix[3] = rotation;
    tempMatrix = *matMultiplication(&cube_R_matrix[3], &cube_tr_matrix[3],&tempMatrix);
    cube_tr_matrix[3] = tempMatrix;

    // Increment the degree of rotation for cubes 1,2,and 3 by 1 degree
    // and change the degree of cube 4 by 4 degrees to make it spin faster
    cubes_123_degrees += 1;
    cube_4_degrees += 4;
    
    glutPostRedisplay();
}




int main(int argc, char **argv)
{
    initCubeVertices();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(2*windowSize, 2*windowSize);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 1");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
