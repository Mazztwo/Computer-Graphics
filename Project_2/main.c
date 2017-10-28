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

#define windowSize 375

#define sizeOfGround 6
#define sizeOfWall 30



Vec4 vertices[10000];
Vec4 colors[10000];


///// NUM ROWS & COLS//////////////////

numRows = 8, numColumns = 8;

///////////////////////////////////////

cell *cells;
cell (*cells2D)[8];


int num_vertices;


float eyex = -1.5, eyey = 1.5, eyez = -.90, startDegrees = 0.0, currDegrees = 0.0, distanceFromOrigin = 0.0;
float atx = 0.0, aty = 0.0, atz = 0.0, near = -0.05, far = -100.0;
float left = -0.05, right = 0.05, bottom = -0.05, top = 0.05, scalingFactor = 0.0;
float wallSize = 0, alpha = 0.0;
char forward = 'e';
int currRow = 0, currCol = 0;

// Declare point & vector pointing from initial mouse click to origin
Vec4 originVector = {0.0,0.0,0.0,0.0};

// Declare vector of motion starting from origin point
Vec4 motionVector = {0.0,0.0,0.0,0.0};

// Declare axis of rotation
Vec4 rotationAxis = {0.0,0.0,0.0,0.0};

GLuint ctm_location;

GLuint projection_matrix_location;
GLuint model_view_matrix_location;


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


// Generate 3D maze
void gen3Dmaze()
{
    srand(time(0));
    
    
    cells = (cell *) malloc(sizeof(cell) * numRows * numColumns);
    
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
    
    // Set number of vertices and initialize vertex
    // and color arrays.
    num_vertices = sizeOfGround + (num_walls*sizeOfWall);
    
    // Clear malloc'ed memory
    for (i = 0; i < num_vertices; i++)
    {
        vecArrayAdd(vertices, i, 0.0, 0.0, 0.0, 0.0);
        vecArrayAdd(colors, i, 0.0, 0.0, 0.0, 0.0);
    }
    
     // Cast the one-dimensional array of cells into two-dimensional
    cells2D =(cell (*)[numColumns]) cells;
    
    int row, column, v_index = 0;
   
    
    // Set ground and ground color
    vecArrayAdd(vertices, v_index, 1.3, 0.0 , -1.3, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    vecArrayAdd(vertices, v_index, -1.3, 0.0, -1.3, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    vecArrayAdd(vertices, v_index, 1.3, 0.0, 1.3, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    // Second Triangle
    vecArrayAdd(vertices, v_index, 1.3, 0.0, 1.3, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    vecArrayAdd(vertices, v_index, -1.3, 0.0, -1.3, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    vecArrayAdd(vertices, v_index, -1.3, 0.0, 1.3, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    
    // Top left corner is {-1,-1,-1}
    float currX = -1.0;
    float currY = 0.0;
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
                // First face. Vertex, followed by color (yellow)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                // Second face. Vertex, followed by color (yellow)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
           
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                // Third face. Vertex, followed by color (pink)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
               
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                
                // Fourth face. Vertex, followed by color (red)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
         
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                
                // Fifth face. Vertex, followed by color (red)
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
             
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ+(.1*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                
            }
            
            // Add every West wall if wall present
            if(cells2D[row][column].west)
            {
                // First Face. Vertex, followed by color (red)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
               
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                
                // Second Face. Vertex, followed by color (red)
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                
                // Third Face. Vertex, followed by color (pink)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                
                // Fourth Face. Vertex, followed by color (yellow)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                // Fifth Face. Vertex, followed by color (yellow)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY+wallSize, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+(.1*wallSize), currY, currZ, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
            }
            
            // If we are on the last row, must add south wall to each column
            if(row == numRows-1)
            {
                // First face. Vertex, followed by color (yellow)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
   
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                // Second face. Vertex, followed by color (yellow)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
                v_index++;
                
                // Third face. Vertex, followed by color (pink)
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
                v_index++;
                
                // Fourth face. Vertex, followed by color (red)
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                
                vecArrayAdd(vertices, v_index, currX+wallSize, currY+wallSize, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ+(.9*wallSize), 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                vecArrayAdd(vertices, v_index, currX+wallSize, currY, currZ+wallSize, 1.0);
                vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
                v_index++;
                
            }
            
            // Move to next column by moving along X direction
            currX += wallSize;
        }
        
        // Keep adding an east wall at the end of the row unless
        // you are on the last row. Then don't add a east wall to
        // make hole for exit.
        if(row != numRows -1)
        {
            // Add ending East wall every time
            // First face. Vertex, followed by color (red)
            vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
         
            vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX, currY, currZ, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            
            // Second face. Vertex, followed by color (red)
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY+wallSize, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY+wallSize, currZ, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY+wallSize, currZ, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY, currZ, 1.0);
            vecArrayAdd(colors, v_index, 0.8, 0, 0, 1.0);
            v_index++;
            
            // Third face. Vertex, followed by color (pink)
            vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY+wallSize, currZ, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY+wallSize, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
            v_index++;
            
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY+wallSize, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 0.4, 1.0, 1.0);
            v_index++;
            
            // Fourth face. Vertex, followed by color (yellow)
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY+wallSize, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
            v_index++;
            
            vecArrayAdd(vertices, v_index, currX, currY+wallSize, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX-(.1*wallSize), currY, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
            v_index++;
            vecArrayAdd(vertices, v_index, currX, currY, currZ+wallSize, 1.0);
            vecArrayAdd(colors, v_index, 1.0, 1.0, 0.4, 1.0);
            v_index++;
            
            // Reset X to be all the way on the left, move Z down one row
            currX = -1.0;
            currZ += wallSize;
        }
        
    }
}


void init(void)
{
    // Initialize model_view matrix
    Mat4 tempMatrix = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
    Mat4 tempMatrix2 = *matMultiplication(&model_view_matrix, &tempMatrix, &tempMatrix2);
    model_view_matrix = tempMatrix2;
    
    // Initialize starting angle
    distanceFromOrigin = sqrt( (eyex*eyex) + (eyez*eyez) );
    startDegrees = acos(eyez / distanceFromOrigin);
    //convert to degrees
    startDegrees *= (180.0/M_PI);
    currDegrees = startDegrees;
    
    tempMatrix = frustum(left, right, bottom, top, near, far);
    projection_matrix = tempMatrix;
    
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
    
    //ctm_location = glGetUniformLocation(program, "ctm");
    projection_matrix_location = glGetUniformLocation(program, "projection_matrix");
    model_view_matrix_location = glGetUniformLocation(program, "model_view_matrix");

    
    
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    
    glClearColor(0.0, 0.5, 1.0, 1.0);
    glDepthRange(1,0);
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &tr_matrix);
    glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);
    glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
    
    //glEnable(GL_CULL_FACE);
    
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
/////////// SCALING DOESN'T WORK FOR NOW////////
    // Zoom In
    else if(key == 'u')
    {
        scalingFactor = 1.02;
    }
    // Zoom Out
    else if(key == 'j')
    {
        scalingFactor = 1.0/1.02;
       
    }
////////////////////////////////////////////////
    else if(key == ' ')
    {
        // Initiate fly around maze
        enableIdle = 1;
    }
    else if(key == 'x')
    {
        eyex -= 0.1;
        
        printf("eyex: %f, eyey: %f, eyez: %f\n", eyex,eyey,eyez);
        printf("atx: %f, aty: %f, atz: %f\n", atx,aty,atz);
    }
    else if(key == 'X')
    {
        eyex += 0.1;

        printf("eyex: %f, eyey: %f, eyez: %f\n", eyex,eyey,eyez);
        printf("atx: %f, aty: %f, atz: %f\n", atx,aty,atz);
    }
    else if(key == 'y')
    {
        eyey -= 0.1;
        
        printf("eyex: %f, eyey: %f, eyez: %f\n", eyex,eyey,eyez);
        printf("atx: %f, aty: %f, atz: %f\n", atx,aty,atz);
    }
    else if(key == 'Y')
    {
        eyey += 0.1;
        
        printf("eyex: %f, eyey: %f, eyez: %f\n", eyex,eyey,eyez);
        printf("atx: %f, aty: %f, atz: %f\n", atx,aty,atz);
    }
    else if(key == 'z')
    {
        eyez -= 0.1;

        printf("eyex: %f, eyey: %f, eyez: %f\n", eyex,eyey,eyez);
        printf("atx: %f, aty: %f, atz: %f\n", atx,aty,atz);
    }
    else if(key == 'Z')
    {
        eyez += 0.1;
        
        printf("eyex: %f, eyey: %f, eyez: %f\n", eyex,eyey,eyez);
        printf("atx: %f, aty: %f, atz: %f\n", atx,aty,atz);
    }
    else if(key == 'g')
    {
        // Initiate fly down to maze entrance
        
        enableIdle = 2;
    }
    else if(key == 'n')
    {
        near -= 0.1;
        
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'N')
    {
        near += 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);

        
    }
    else if(key == 'f')
    {
        far -= 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'F')
    {
        far += 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 't')
    {
        top -= 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'T')
    {
        top += 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'b')
    {
        bottom -= 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'B')
    {
        bottom += 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'l')
    {
        left -= 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'L')
    {
        left += 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'r')
    {
        right -= 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 'R')
    {
        right += 0.1;
        printf("left: %f, right: %f, top: %f, bottom: %f, near: %f, far: %f\n",left,right,top,bottom,near,far);
        
    }
    else if(key == 's')
    {
        // Walk into maze
        
        enableIdle = 4;
    }
    
    
    
    Mat4 tempMatrix = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
    Mat4 tempMatrix2 = *matMultiplication(&model_view_matrix, &tempMatrix, &tempMatrix2);
    model_view_matrix = tempMatrix2;
    
    tempMatrix = frustum(left, right, bottom, top, near, far);
    projection_matrix = tempMatrix;

    glutPostRedisplay();
    
}


int getSituation(char forward, cell *currCell)
{
    /*
     -need which direction facing (N,S,E or W)
     -need row/col you are in to know if
     there is a N,S,E,W wall in that cell
     -step = .005
     
     -based on that
     -if wall on left
         -if no wall forward
             -move forward one space
         -if wall in front
             -turn left 180 degrees
     -if no wall on left
         -turn left 90 degrees
         -move forward one space
     
     */
    if(forward == 'n')
    {
        // check for left wall
        if(currCell->west)
        {
            // check for wall in front
            if( currCell->north)
            {
                return 1;
            }
            
            return 2;
        }
        
        return 3;
    }
    else if(forward == 's')
    {
        // check for left wall
        if(currCell->east)
        {
            // check for wall in front
            if( currCell->south)
            {
                return 1;
            }
            
            return 2;
        }
        
        return 3;
    }
    else if(forward == 'e')
    {
        // check for left wall
        if(currCell->north)
        {
            // check for wall in front
            if( currCell->east)
            {
                return 1;
            }
            
            return 2;
        }
    
        return 3;
    }
    else //forward == 'w'
    {
        // check for left wall
        if(currCell->south)
        {
            // check for wall in front
            if( currCell->west)
            {
                return 1;
            }
            
            return 2;
        }
        
        return 3;
    }
        
        
        
    return 1;
}



void idle(void)
{
    // Fly a circle around the maze
    if(enableIdle == 1)
    {
        
        if (currDegrees < startDegrees + 360)
        {
            // Increment currDegrees by 5 and
            // recalculate new eyex and eyez.
            currDegrees += 1.0;
            printf("degrees: %f\n", currDegrees);
        
            // convert to radians
            currDegrees *= (M_PI/180.0);
        
            eyez = distanceFromOrigin * cos(currDegrees);
            eyex = distanceFromOrigin * sin(-currDegrees);
        
            printf("eyex: %f, eyez: %f\n",eyex, eyez);
        
            //Update look_at function with new x and z positions
            Mat4 tempMatrix = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
            model_view_matrix = tempMatrix;
            
            // convert back to degrees
            currDegrees *= (180.0/M_PI);
        }
        else
        {
            enableIdle = 0;
            currDegrees = startDegrees;
        }
        
    }
    // Look down to maze entrance &
    // fly down to maze entrance
    else if(enableIdle == 2)
    {
        float eyexFinal = -1.2;
        float eyeyFinal = 0.1;
        float eyezFinal = -.85;
        
        float atzFinal = -0.9;
        
        Vec4 p1 = {eyex, eyey, eyez, 1.0};
        Vec4 p2 = {eyexFinal, eyeyFinal, eyezFinal, 1.0};
        
        Vec4 p3 = {atx, aty, atz, 1.0};
        Vec4 p4 = {atx, aty, atzFinal, 1.0};
        
        Vec4 vTemp = *vec4subtraction(&p2, &p1, &vTemp);
        Vec4 v1 = vTemp;
        
        vTemp = *vec4subtraction(&p4, &p3, &vTemp);
        Vec4 v2 = vTemp;
        
        if(alpha <= 1.0)
        {
            Vec4 alphaV = *scalarMultVector(alpha, &v1, &alphaV);
            v1 = alphaV;
            
            alphaV = *scalarMultVector(alpha, &v2, &alphaV);
            v2 = alphaV;
            
            vTemp = *vec4addition(&p1, &v1, &vTemp);
            p2 = vTemp;
            
            vTemp = *vec4addition(&p3, &v2, &vTemp);
            p4 = vTemp;
            
            eyex = p2.x;
            eyey = p2.y;
            eyez = p2.z;
            
            atx = p4.x;
            aty = p4.y;
            atz = p4.z;
            
            Mat4 tempMatrix = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
            model_view_matrix = tempMatrix;
            
            alpha += 0.005;
        }
        else
        {
            alpha = 0.0;
            enableIdle = 0;
        }
         
    }
    // walk into maze
    else if(enableIdle == 4)
    {
        float eyexFinal = eyex + .005;
        
        Vec4 p1 = {eyex, eyey, eyez, 1.0};
        Vec4 p2 = {eyexFinal, eyey, eyez, 1.0};
        
        Vec4 vTemp = *vec4subtraction(&p2, &p1, &vTemp);
        Vec4 v = vTemp;
        
        if(alpha <= 1.0)
        {
            Vec4 alphaV = *scalarMultVector(alpha, &v, &alphaV);
            v = alphaV;
            
            vTemp = *vec4addition(&p1, &v, &vTemp);
            p2 = vTemp;
            
            eyex = p2.x;
            eyey = p2.y;
            eyez = p2.z;
            
            Mat4 tempMatrix = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
            model_view_matrix = tempMatrix;
            
            alpha += 0.2;
        }
        else
        {
            alpha = 0.0;
            printf("atx: %f, aty: %f, atz: %f\n",atx, aty, atz);
            printf("eyex: %f, eyey: %f, eyez: %f\n",eyex, eyey, eyez);
            enableIdle = 5;
        }
    }
    // solve maze
    else if(enableIdle == 5)
    {
        /*
         -need which direction facing (N,S,E or W)
         -need row/col you are in to know if
         there is a N,S,E,W wall in that cell
         -step = .005
         
         -based on that
             -if wall on left
                 -if no wall forward
                     -move forward one space
                 -if wall in front
                    -turn 180 degrees
             -if no wall on left
                 -turn left 90 degrees
                 -move forward one space
         
         */
        int situation = getSituation(forward, &cells2D[currRow][currCol]);
        
        // 1 = turn 180 degrees
        // 2 = move forward one cell
        // 3 = turn left 90 degrees and move forward one space
        
        if(situation == 1)
        {
            if(forward == 'e' || forward == 'w')
            {
                // just flip sign of atx to turn around
                float atxFinal = atx * -1;
                
                Vec4 p1 = {atx, aty, atz, 1.0};
                Vec4 p2 = {atxFinal, aty, atz, 1.0};
                
                Vec4 vTemp = *vec4subtraction(&p2, &p1, &vTemp);
                Vec4 v = vTemp;
                
                if(alpha <= 1.0)
                {
                    Vec4 alphaV = *scalarMultVector(alpha, &v, &alphaV);
                    v = alphaV;
                    
                    vTemp = *vec4addition(&p1, &v, &vTemp);
                    p2 = vTemp;
                    
                    atx = p2.x;
                    aty = p2.y;
                    atz = p2.z;
                    
                    Mat4 tempMatrix = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
                    model_view_matrix = tempMatrix;
                    
                    alpha += 0.01;
                }
                else
                {
                    alpha = 0.0;
                    
                    if(forward == 'e')
                    {
                        forward = 'w';
                    }
                    else
                    {
                        forward = 'e';
                    }
                    
                    printf("atx: %f, aty: %f, atz: %f\n",atx, aty, atz);
                    printf("eyex: %f, eyey: %f, eyez: %f\n",eyex, eyey, eyez);
                    enableIdle = 0;
                }
                
                
                
            }
            else //situation == 'n' || situation == 's'
            {
                
            }
        }
        else if(situation == 2)
        {
            
        }
        else if(situation == 3)
        {
            
        }
        
        
        
        
        
    
    }
    
    glutPostRedisplay();
}




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
            originVector.x = x-windowSize;
            originVector.y = windowSize-y;
            originVector.z = sqrt((windowSize*windowSize)-((x-windowSize)*(x-windowSize)));
            originVector.w = 0.0;

            
        }
    }

    glutPostRedisplay();
}



void motion(int x, int y)
{
    // Capture moving x,y
    motionVector.x = x-windowSize;
    motionVector.y = windowSize-y;
    motionVector.z = sqrt((windowSize*windowSize)-((x-windowSize)*(x-windowSize)));
    motionVector.w = 0.0;
   
    
    // If user drag is outside window, do nothing
    if(motionVector.x > windowSize ||
       motionVector.y > windowSize ||
       motionVector.z > windowSize ||
       motionVector.x < -windowSize ||
       motionVector.y < -windowSize ||
       motionVector.z < -windowSize
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
                Mat4 tempMatrix5 = *matMultiplication(&R,&model_view_matrix,&tempMatrix5);
                model_view_matrix = tempMatrix5;
            
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
    gen3Dmaze();
    
    // OpenGL initializaiton code
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize*2, windowSize*2);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 2 - Maze");
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
