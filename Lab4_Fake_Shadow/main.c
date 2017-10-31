/*
 Alessio Mazzone
 Lab4 Fake Shadows
 
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

const float DegreesToRadians = M_PI / 180.0; // M_PI = 3.14159...


// Create 3D object verticies
Vec4 vertices[2*1176];

// Color each face of object
Vec4 colors[2*1176];

// Vertex index
int v_index = 0;


Vec4 cube_vertices[36] =
{
    // Front Face 1
    {1.0, 1.0, 1.0, 1.0},              // top right
    {-1.0, 1.0, 1.0, 1.0},             // top left
    {-1.0, -1.0, 1.0, 1.0},            // bottom left
    
    {-1.0, -1.0, 1.0, 1.0},              // top right
    {1.0, -1.0, 1.0, 1.0},            // bottom left
    {1.0, 1.0, 1.0, 1.0},             // bottom right
    
    // Right Face 2
    {1.0, 1.0, -1.0, 1.0},             // top right
    {1.0, 1.0, 1.0, 1.0},              // top left
    {1.0, -1.0, 1.0, 1.0},             // bottom left
    
    {1.0, -1.0, 1.0, 1.0},             // top right
    {1.0, -1.0, -1.0, 1.0},             // bottom left
    {1.0, 1.0, -1.0, 1.0},            // bottom right
    
    // Back Face 3
    {1.0, 1.0, -1.0, 1.0},           // Bottom left
    {-1.0, 1.0, -1.0, 1.0},            // Top left
    {-1.0, -1.0, -1.0, 1.0},             // Top Right
    
    {-1.0, -1.0, -1.0, 1.0},              // Top Right
    {1.0, -1.0, -1.0, 1.0},            // Bottom Right
    {1.0, 1.0, -1.0, 1.0},             // Bottom Left
    
    // Left Face 4
    {-1.0, 1.0, -1.0, 1.0},              // top right
    {-1.0, 1.0, 1.0, 1.0},             // top left
    {-1.0, -1.0, 1.0, 1.0},            // bottom left
    
    {-1.0, -1.0, 1.0, 1.0},              // top right
    {-1.0, -1.0, -1.0, 1.0},            // bottom left
    {-1.0, 1.0, -1.0, 1.0},              // bottom right
    
    // Top Face 5
    {1.0, 1.0, -1.0, 1.0},              // top right
    {-1.0, 1.0, -1.0, 1.0},             // top left
    {-1.0, 1.0, 1.0, 1.0},            // bottom left
    
    {-1.0, 1.0, 1.0, 1.0},              // top right
    {1.0, 1.0, 1.0, 1.0},            // bottom left
    {1.0, 1.0, -1.0, 1.0},              // bottom right
    
    // Botto Face 6
    {1.0, -1.0, -1.0, 1.0},              // bottom left
    {-1.0, -1.0, -1.0, 1.0},             // top left
    {-1.0, -1.0, 1.0, 1.0},            // top right
    
    {-1.0, -1.0, 1.0, 1.0},              // top right
    {1.0, -1.0, 1.0, 1.0},            // bottom right
    {1.0, -1.0, -1.0, 1.0}              // bottom left
};

// Color each face of object
Vec4 cube_colors[36] =
{
    {0.8, 0.0, 0.0, 1.0},    // red
    {0.8, 0.0, 0.0, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    
    {0.0, 0.0, 0.8, 1.0},    // Blue
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    
    {0.8, 0.0, 0.0, 1.0},    // red
    {0.8, 0.0, 0.0, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    
    {0.0, 0.0, 0.8, 1.0},    // Blue
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    {0.0, 0.0, 0.8, 1.0},
    
    {0.0, 0.8, 0.0, 1.0},    // Green
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
    
    {0.0, 0.8, 0.0, 1.0},    // Green
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
    {0.0, 0.8, 0.0, 1.0},
};



void initSphere()
{
    srand ( time(NULL) );
 
    for (float phi = -90.0; phi <= 90.0; phi += 20.0)
    {
        float phir = phi * DegreesToRadians;
        float phir20 = (phi + 20.0)*DegreesToRadians;
        
        for(float theta = -180.0; theta <= 180.0; theta += 20.0)
        {
            float thetar = theta*DegreesToRadians;
            float thetar20 = (theta+20.0)*DegreesToRadians;
              
            vertices[v_index].x = sin(thetar)*cos(phir);
            vertices[v_index].y = cos(thetar)*cos(phir);
            vertices[v_index].z = sin(phir);
            vertices[v_index].w = 1.0;
            
            colors[v_index].x = rand() / (float)RAND_MAX;
            colors[v_index].y = rand() / (float)RAND_MAX;
            colors[v_index].z = rand() / (float)RAND_MAX;
            colors[v_index].w = 1.0;
            v_index++;
            
            vertices[v_index].x = sin(thetar)*cos(phir20);
            vertices[v_index].y = cos(thetar)*cos(phir20);
            vertices[v_index].z = sin(phir20);
            vertices[v_index].w = 1.0;
            
            colors[v_index].x = rand() / (float)RAND_MAX;
            colors[v_index].y = rand() / (float)RAND_MAX;
            colors[v_index].z = rand() / (float)RAND_MAX;
            colors[v_index].w = 1.0;
            v_index++;
            
            vertices[v_index].x = sin(thetar20)*cos(phir20);
            vertices[v_index].y = cos(thetar20)*cos(phir20);
            vertices[v_index].z = sin(phir20);
            vertices[v_index].w = 1.0;
            
            colors[v_index].x = rand() / (float)RAND_MAX;
            colors[v_index].y = rand() / (float)RAND_MAX;
            colors[v_index].z = rand() / (float)RAND_MAX;
            colors[v_index].w = 1.0;
            v_index++;
            
            
            vertices[v_index].x = sin(thetar20)*cos(phir20);
            vertices[v_index].y = cos(thetar20)*cos(phir20);
            vertices[v_index].z = sin(phir20);
            vertices[v_index].w = 1.0;
            
            colors[v_index].x = rand() / (float)RAND_MAX;
            colors[v_index].y = rand() / (float)RAND_MAX;
            colors[v_index].z = rand() / (float)RAND_MAX;
            colors[v_index].w = 1.0;
            v_index++;
            
            vertices[v_index].x = sin(thetar20)*cos(phir);
            vertices[v_index].y = cos(thetar20)*cos(phir);
            vertices[v_index].z = sin(phir);
            vertices[v_index].w = 1.0;
            
            colors[v_index].x = rand() / (float)RAND_MAX;
            colors[v_index].y = rand() / (float)RAND_MAX;
            colors[v_index].z = rand() / (float)RAND_MAX;
            colors[v_index].w = 1.0;
            v_index++;
            
            vertices[v_index].x = sin(thetar)*cos(phir);
            vertices[v_index].y = cos(thetar)*cos(phir);
            vertices[v_index].z = sin(phir);
            vertices[v_index].w = 1.0;
            
            colors[v_index].x = rand() / (float)RAND_MAX;
            colors[v_index].y = rand() / (float)RAND_MAX;
            colors[v_index].z = rand() / (float)RAND_MAX;
            colors[v_index].w = 1.0;
            v_index++;
        }
    }
}


void initCube()
{
    int j;
    
    for( j = 0; j < 36; j++)
    {
        Vec4 temp = *vec4create(cube_vertices[j].x,
                               cube_vertices[j].y,
                               cube_vertices[j].z,
                               1.0, 
                               &temp);
        vertices[v_index] = temp;
        
        
        temp = *vec4create(cube_colors[j].x,
                          cube_colors[j].y,
                          cube_colors[j].z,
                          1.0,
                          &temp);
        colors[v_index] = temp;
        
        v_index++;
    }
        
    
}



// Declare number of verticies
int num_vertices = 1176;


GLuint ctm_location;

// First 3 cols are the scaling factor
// Last col is point of center of object
Mat4 tr_matrix[2] =
{
    {{0.25, 0.0, 0.0, 0.0},
    {0.0, 0.25, 0.0, 0.0},
    {0.0, 0.0, 0.25, 0.0},
    {-0.5, 0.25, 0.0, 1.0}},
    
    {{0.25, 0.0, 0.0, 0.0},
    {0.0, 0.25, 0.0, 0.0},
    {0.0, 0.0, 0.25, 0.0},
    {0.5, 0.25, 0.0, 1.0}}
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
    glClearColor(0.3, 0.5, 0.5, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &scaling_matrix);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    //glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    
    // Draw Sphere
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &tr_matrix[0]);
    glDrawArrays(GL_TRIANGLES, 0, 1140);
    
    // Draw Cube
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &tr_matrix[1]);
    glDrawArrays(GL_TRIANGLES, 1140, 36);
    
    
    
    glutSwapBuffers();
}


void idle(void)
{
    glutPostRedisplay();
    
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

int main(int argc, char **argv)
{
    initSphere();
    initCube();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lab3 Sphere");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
