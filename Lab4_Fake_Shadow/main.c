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

// Declare number of verticies (cube+sphere) * 2
int num_vertices = 2 * 1176;

// Vertex index
int v_index = 0;

// Declare eye and at
float eyex, eyey, eyez;
float atx, aty, atz;


///////////////// LIGHT //////////////////
float lightx = 0, lighty = 3, lightz = -1;
//////////////////////////////////////////


GLuint ctm_location;
Mat4 model_view;

// Initialize tr matricies
// One for each of the 4 objects
// (sphere, cube, and each shadow)
Mat4 transformation_matricies[4] =
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
    {0,0,0,1}}
};

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
    
    // Bottom Face 6
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
    {0.7, 0.0, 0.1, 1.0},    // red
    {0.7, 0.0, 0.1, 1.0},
    {0.7, 0.0, 0.1, 1.0},
    {0.7, 0.0, 0.1, 1.0},
    {0.7, 0.0, 0.1, 1.0},
    {0.7, 0.0, 0.1, 1.0},
    
    {0.1, 0.0, 0.7, 1.0},    // Blue
    {0.1, 0.0, 0.7, 1.0},
    {0.1, 0.0, 0.7, 1.0},
    {0.1, 0.0, 0.7, 1.0},
    {0.1, 0.0, 0.7, 1.0},
    {0.1, 0.0, 0.7, 1.0},
    
    {0.7, 0.1, 0.0, 1.0},    // red
    {0.7, 0.1, 0.0, 1.0},
    {0.7, 0.1, 0.0, 1.0},
    {0.7, 0.1, 0.0, 1.0},
    {0.7, 0.1, 0.0, 1.0},
    {0.7, 0.1, 0.0, 1.0},
    
    {0.0, 0.1, 0.7, 1.0},    // Blue
    {0.0, 0.1, 0.7, 1.0},
    {0.0, 0.1, 0.7, 1.0},
    {0.0, 0.1, 0.7, 1.0},
    {0.0, 0.1, 0.7, 1.0},
    {0.0, 0.1, 0.7, 1.0},
    
    {0.1, 0.7, 0.0, 1.0},    // Green
    {0.1, 0.7, 0.0, 1.0},
    {0.1, 0.7, 0.0, 1.0},
    {0.1, 0.7, 0.0, 1.0},
    {0.1, 0.7, 0.0, 1.0},
    {0.1, 0.7, 0.0, 1.0},

    {0.0, 0.7, 0.1, 1.0},    // Green
    {0.0, 0.7, 0.1, 1.0},
    {0.0, 0.7, 0.1, 1.0},
    {0.0, 0.7, 0.1, 1.0},
    {0.0, 0.7, 0.1, 1.0},
    {0.0, 0.7, 0.1, 1.0},
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




void initShadows()
{
    for(int i = 0; i < 1176; i++)
    {
        // Create x and z components
        // of shadow
        float shadowx = (lightx)-((lighty)*(lightx - vertices[v_index-1176].x)/(lighty - vertices[v_index-1176].y));
        float shadowz = (lightz)-((lighty)*(lightz - vertices[v_index-1176].z)/(lighty - vertices[v_index-1176].y));
        
        
        Vec4 temp = *vec4create(shadowx, 0, shadowz, 1, &temp);
        vertices[v_index] = temp;
        
        temp = *vec4create(0, 0, 0, 1, &temp);
        colors[v_index] = temp;
        
        v_index++;
    }
}



void init(void)
{
    
    
    eyex = 0, eyey = 0, eyez = 1;
    
    atx = 0, aty = 0, atz = 0;
    
    model_view = look_at(eyex, eyey, eyez, atx, aty, atz, 0, 1, 0);
    
    /*
    // Translate objects and shadows
    // Objects
    Mat4 temp1 = *translate(&transformation_matricies[0], -0.5, 0.25, 0, &temp1);
    Mat4 temp2 = *translate(&transformation_matricies[1],  0.5, 0.25, 0, &temp2);
     
    // Shadows
    Mat4 temp3 = *translate(&transformation_matricies[2], -0.5, 0,  0, &temp3);
    Mat4 temp4 = *translate(&transformation_matricies[3],  0.5, 0 , 0, &temp4);
    
    // Scale objects and shadows
    // Objects
    Mat4 temp5 = *scaleMatrix(&temp1, .25, &temp5);
    Mat4 temp6 = *scaleMatrix(&temp2, .25, &temp6);
    
    // Shadows
    Mat4 temp7 = *scaleMatrix(&temp3, .25, &temp7);
    Mat4 temp8 = *scaleMatrix(&temp4, .25, &temp8);
    */
    
    // Scale objects and shadows about origin
    // Objects
    Mat4 temp1 = *scaleMatrix(&transformation_matricies[0], .25, &temp1);
    Mat4 temp2 = *scaleMatrix(&transformation_matricies[1], .25, &temp2);
    
    // Shadows
    Mat4 temp3 = *scaleMatrix(&transformation_matricies[2], .25, &temp3);
    Mat4 temp4 = *scaleMatrix(&transformation_matricies[3], .25, &temp4);
    
    // Translate objects and shadows to correct points
    // Objects
    Mat4 temp5 = *translate(&temp1, -0.5, 0.0, 0, &temp5);
    Mat4 temp6 = *translate(&temp2,  0.5, 0.0, 0, &temp6);
    
    // Shadows
    Mat4 temp7 = *translate(&temp3,  0.5, 0.25, 0, &temp7);
    Mat4 temp8 = *translate(&temp4, 0.5, 0.25, 0, &temp8);
    
    
    // Apply model view
    // Objects
    temp1 = *matMultiplication(&temp5, &model_view, &temp1);
    temp2 = *matMultiplication(&temp6, &model_view, &temp2);

    // SHadows
    temp3 = *matMultiplication(&temp7, &model_view, &temp3);
    temp4 = *matMultiplication(&temp8, &model_view, &temp4);
    
    transformation_matricies[0] = temp1;
    transformation_matricies[1] = temp2;
    transformation_matricies[2] = temp3;
    transformation_matricies[3] = temp4;
    
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
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    
    
    // Draw Sphere
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &transformation_matricies[0]);
    glDrawArrays(GL_TRIANGLES, 0, 1140);
    
    // Draw Cube
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &transformation_matricies[1]);
    glDrawArrays(GL_TRIANGLES, 1140, 36);
    
    // Draw Sphere Shadow
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &transformation_matricies[2]);
    glDrawArrays(GL_TRIANGLES, 1176, 1140);
    
    // Draw Cube shadow
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &transformation_matricies[3]);
    glDrawArrays(GL_TRIANGLES, 2316, 36);
    
    
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
    
    /*
    printf("Please enter an eye point: ");
    scanf("%f,%f,%f*", &eyex, &eyey, &eyez);
    
    printf("Please enter an at point: ");
    scanf("%f,%f,%f*", &atx, &aty, &atz);
    
    printf("Please enter a light point: ");
    scanf("%f,%f,%f*", &lightx, &lighty, &lightz);
     */
    
    initShadows();
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Lab3 Sphere");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
