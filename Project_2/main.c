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
#include "genMaze.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define sizeOfGround 6
#define sizeOfWall 6
#define num_vertices sizeOfGround+(79*sizeOfWall)


// Start with ground vertices
Vec4 vertices[num_vertices] =
{
    {1.5, -1.0, -1.5, 1.0},
    {-1.5, -1.0, -1.5, 1.0},
    {1.5, -1.0, 1.5, 1.0},
    
    {1.5, -1.0, 1.5, 1.0},
    {-1.5, -1.0, -1.5, 1.0},
    {-1.5, -1.0, 1.5, 1.0},
};


// Start with ground color
Vec4 colors[num_vertices] =
{
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
};


// Color each face of object
/*
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

int numRows, numColumns;


// Returns maximum of two floats. If numbers are equal,
// function just returns num2.
float maxNum(float num1, float num2)
{
    if(num1>num2) return num1;
    else return num2;
}

//FIGURE OUT DYNAMIC NUM_VERTICES


// Generate vertices for maze
void gen3Dmaze(cell *cells)
{
    cell (*cells2D)[numColumns] = (cell (*)[numColumns]) cells;
    int row, column, v_index;
    float wallSize = 0;
    
    // V index is 6 because ground has already been added
    v_index = 6;
    
    
    // Top left corner is {-1,-1,-1}
    float startX = -1.0;
    float startY = -1.0;
    float startZ = -1.0;
    
    float currX = -1.0;
    float currY = -1.0;
    float currZ = -1.0;
    
    // Figure out size of wall based on how many rows/cols there are
    // to accomodate for different size mazes. Length of ground is 2.0
    if(numRows != 0 || numColumns !=0)
    {
        wallSize = 2.0 / maxNum(numRows,numColumns);
    }
    else
    {
        printf("ERROR: Row or Column number is 0.\n");
        exit(0);
    }
    
    for(row = 0; row < numRows; row++)
    {
        for(column = 0; column < numColumns; column++)
        {
            // Add every North wall if wall present
            if(cells2D[row][column].north)
            {
                // Make all north walls blue
                //v1
                vertices[v_index].x = currX;
                vertices[v_index].y = currY;
                vertices[v_index].z = currZ;
                vertices[v_index].w = 1.0;
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                //v2
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                //v3
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                //v4
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                //v5
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                //v6
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                vertices[v_index].x
                vertices[v_index].y
                vertices[v_index].z
                vertices[v_index].w
                v_index++;
                
                //c1, c2, c3, c4, c5, c6
                
            }
            
            // Add every West wall if wall present
            if(cells2D[row][column].west)
            {
                // Make all west walls red
                //v1, v2, v3, v4, v5, v6
                
                //c1, c2, c3, c4, c5, c6
            }
            
        }
        
        // Add ending East wall every time
    
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
    glClearColor(0.0, 0.0, 1.0, 1.0);
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
        glutPostRedisplay();
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
            // Get initial click point to use for
            // calculation of axis of rotation
            originVector.x = x-256;
            originVector.y = 256-y;
            originVector.z = sqrt((256*256)-((x-256)*(x-256)));
            originVector.w = 0.0;

            
        }
        else if(state == GLUT_UP)
        {
            // allows spin animation to start
            enableIdle = 1;
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
    motionVector.w = 0.0;
   
    
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

            
            if(d != 0)
            {
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
                
                // The factor in which theta is multiplied by
                // determines the speed at which the object
                // rotates about the rotation axis.
                // Convert degrees to radians by multiplying by 180/pi
                Mat4 tempMatrix2 = *matRotateAboutZ(theta*(180/M_PI), &tempMatrix2);
                
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
                originVector.w = motionVector.w;
            }
        }
        else
        {
            R.col1.x = 1.0; R.col2.x = 0.0; R.col3.x = 0.0; R.col4.x = 0.0;
            R.col1.y = 0.0; R.col2.y = 1.0; R.col3.y = 0.0; R.col4.y = 0.0;
            R.col1.z = 0.0; R.col2.z = 0.0; R.col3.z = 1.0; R.col4.z = 0.0;
            R.col1.w = 0.0; R.col2.w = 0.0; R.col3.w = 0.0; R.col4.w = 1.0;
        }
        
    }
    
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    
   // GENERATE MAZE /////////////////////////////////////////////////////////////////////////////////
   
    int row, column;
    srand(time(0));
    
    /*
        fflush(stdout);
        printf("Enter a number of rows: ");
        fflush(stdout);
        scanf("%i", &numRows);
        fflush(stdout);
        printf("Enter a number of columns: ");
        fflush(stdout);
        scanf("%i", &numColumns);
        fflush(stdout);
     */
    
    numRows = 8;
    numColumns = 8;
    cell *cells = (cell *) malloc(sizeof(cell) * numRows * numColumns);
    
    // Clear malloc'ed memory
    int i;
    for (i = 0; i < numRows * numColumns; i++)
    {
        cells[i].north = 0;
        cells[i].south = 0;
        cells[i].east = 0;
        cells[i].west = 0;
    }
    
    gen_maze(numRows, numColumns, cells);
    print_maze(numRows, numColumns, cells);
    
    int num_walls = get_num_walls(numRows, numColumns, cells);
    
    printf("The number of walls is %i.\n", num_walls);
    
    // Cast the one-dimensional array of cells into two-dimensional
    // array of cells
    
    cell (*cells2D)[numColumns] = (cell (*)[numColumns]) cells;
    
    // Show the north variables of each cell
    printf("The north component of each cell are as follows:\n");
    
    for(row = 0; row < numRows; row++)
    {
        for(column = 0; column < numColumns; column++)
        {
            printf("%i ", cells2D[row][column].north);
        }
        printf("\n");
    }
    /////////////////////////////////////////////////////////////////////////////////////////
    
    gen3Dmaze(cells2D);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 2");
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
