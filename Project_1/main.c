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

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


Vec3 vertices[3] =
{{ 0.0,  0.5,  0.0},    // top
    {-0.5, -0.5,  0.0},    // bottom left
    { 0.5, -0.5,  0.0}};    // bottom right

Vec4 colors[3] =
{{1.0, 0.0, 0.0, 1.0},    // red   (for top)
    {0.0, 1.0, 0.0, 1.0},    // green (for bottom left)
    {0.0, 0.0, 1.0, 1.0}};    // blue  (for bottom right)

int num_vertices = 3;

GLuint ctm_location;

Mat4 tr_matrix =
{{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};

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
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
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
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    exit(0);
    
    if(!enableIdle)
    {
        if(key == 'X')
        tr_matrix.col4.x += 0.1;
        else if(key == 'x')
        tr_matrix.col4.x -= 0.1;
        else if(key == 'Y')
        tr_matrix.col4.y += 0.1;
        else if(key == 'y')
        tr_matrix.col4.y -= 0.1;
        else if(key == 'Z')
        tr_matrix.col4.z += 0.1;
        else if(key == 'z')
        tr_matrix.col4.z -= 0.1;
        else if(key == ' ')
        {
            tr_matrix.col4.x = 0.5;
            tr_matrix.col4.y = 0.5;
            tr_matrix.col4.z = 0.0;
            enableIdle = 1;
        }
        
        glutPostRedisplay();
    }
    else
    {
        if(key == ' ')
        {
            tr_matrix.col4.x = 0.0;
            tr_matrix.col4.y = 0.0;
            tr_matrix.col4.z = 0.0;
            enableIdle = 0;
        }
    }
    
}

void idle(void)
{
    if(enableIdle)
    {
        if(leftDown)
        {
            tr_matrix.col4.x -= 0.01;
            tr_matrix.col4.y -= 0.01;
            
            if(tr_matrix.col4.x < -0.5)
            leftDown = 0;
        }
        else
        {
            tr_matrix.col4.x += 0.01;
            tr_matrix.col4.y += 0.01;
            
            if(tr_matrix.col4.x > 0.5)
            leftDown = 1;
        }
        
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Triangle");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
