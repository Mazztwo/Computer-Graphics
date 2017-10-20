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

// CUBE ///////////////////////////////////////////////
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


Vec4 vertices[36*4];
Vec4 colors[36*4];
GLuint ctm_location;

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
    {0.0,0.0,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {0.0,0.0,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {0.0,0.0,0.0,1.0}},
    
    {{1.0,0.0,0.0,0.0},
    {0.0,1.0,0.0,0.0},
    {0.0,0.0,1.0,0.0},
    {0.0,0.0,0.0,1.0}}
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

 
// Declare number of verticies
int num_vertices = 36*4;

void initCubeVertices()
{
    int i, j, v_index = 0;
    
    // Transformation matrix here
    // Cube 1: COM located @ (-0.5,0.5,0.0) and spins about X, fixed pt of rotation is COM
    // Cube 2: COM located @ (0.5,0.5,0.0) and spins about Y, fixed pt of rotation is COM
    // Cube 3: COM located @ (-0.5,-0.5,0.0) and spins about Z, fixed pt of rotation is COM
    // Cube 4: COM located @ (0.5,-0.5,0.0) and spins about X, fixed pt of rotation is COM.
    //         Make Cube 4 spin faster than Cube 1.
    // Use IDLE to make cubes spin continuously
    
    for( i = 0; i < 4; i++)
    {
        for( j = 0; j < 36; j++)
        {
            //vertices[v_index] = M*cube+vertices[j]
            //colors[v_index] = ...;
            //v_index++;
            
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
    
    // Cube 1
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_tr_matrix[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Cube 2
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_tr_matrix[1]);
    glDrawArrays(GL_TRIANGLES, 36, 36);
    // Cube 3
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_tr_matrix[2]);
    glDrawArrays(GL_TRIANGLES, 72, 36);
    // Cube 4
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
    /*
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
        Mat4 rotation = *matRotateAboutX(5.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
       
    }
    else if(key == 'X')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutX(-5.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;

    }
    else if(key == 'y')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutY(5.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
    
    }
    else if(key == 'Y')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutY(-5.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
       
    }
    else if(key == 'z')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutZ(5.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
      
    }
    else if(key == 'Z')
    {
        printf("KEY: %c\n",key);
        Mat4 rotation = *matRotateAboutZ(-5.0, &rotation);
        Mat4 tempMatrix2 = *matMultiplication(&rotation, &tr_matrix, &tempMatrix2);
        tr_matrix = tempMatrix2;
       
    }
    
    */
    glutPostRedisplay();
    
}


void idle(void)
{
    // Update Cube 1
    //Mat4 tempMatrix = *matMultiplication(&R, &tr_matrix,&tempMatrix);
    //tr_matrix = tempMatrix;
    
    // Update Cube 2
    
    // Update Cube 3
    
    // Update Cube 4


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
