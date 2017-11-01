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

int numRows = 8, numColumns = 8;

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
int situation = 0;

float eyexFinal = 0, eyezFinal = 0;
float atxFinal = 0, atzFinal = 0;

Vec4 p1 = {-1.5, 1.5, -0.9, 1.0};
Vec4 p2 = {-1.1, 0.1, -0.9, 1.0};

Vec4 p3 = {0, 0, 0, 1};
Vec4 p4 = {0, 0.1, -0.9, 1};

Vec4 v1 = {0, 0, 0, 0};
Vec4 v2 = {0, 0, 0, 0};

Vec4 currEye = {0, 0, 0, 0};
Vec4 currAt = {0, 0, 0, 0};


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
    
    v1 = *vec4subtraction(&p2, &p1, &v1);
    
    v2 = *vec4subtraction(&p4, &p3, &v2);
    
    
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
        
        enableIdle = 3;
    }
    
    
    
    Mat4 tempMatrix = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
    model_view_matrix = tempMatrix;
    
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
             -turn right 90 degrees
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
        
            // convert to radians
            currDegrees *= (M_PI/180.0);
        
            eyez = distanceFromOrigin * cos(currDegrees);
            eyex = distanceFromOrigin * sin(-currDegrees);
        
            //Update look_at function with new x and z positions
            Mat4 tempMatrix = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
            model_view_matrix = tempMatrix;
            
            // convert back to degrees
            currDegrees *= (180.0/M_PI);
        }
        else
        {
            enableIdle = 2;
            //enableIdle = 0;
            currDegrees = startDegrees;
        }
        
    }
    // Look down to maze entrance &
    // fly down to maze entrance
    else if(enableIdle == 2)
    {
        if(alpha <= 1.0 )
        {
            Vec4 scaledV1 = *scalarMultVector(alpha, &v1, &scaledV1);
            currEye = *vec4addition(&p1, &scaledV1, &currEye);
            
            Vec4 scaledV2 = *scalarMultVector(alpha, &v2, &scaledV2);
            currAt = *vec4addition(&p3, &scaledV2, &currAt);
            
            Mat4 tempMat = look_at(currEye.x, currEye.y, currEye.z, currAt.x, currAt.y, currAt.z, 0, 1, 0);
            model_view_matrix = tempMat;
            
 
            alpha += 0.01;
        }
        else
        {
            alpha = 0.0;
            eyex = currEye.x;
            eyey = currEye.y;
            eyez = currEye.z;
            atx = currAt.x;
            aty = currAt.y;
            atz = currAt.z;
            
           // set up for walk into maze
            eyexFinal = eyex + .2;
            p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
            p2 = *vec4create(eyexFinal, eyey, eyez, 1.0, &p2);
            v1 = *vec4subtraction(&p2, &p1, &v1);
            
            atx += 1.2*atx;
            
            //enableIdle = 0 ;
            enableIdle = 3;
        }
    }
    // walk into maze
    else if(enableIdle == 3)
    {
        if(alpha <= 1.0)
        {
            Vec4 scaledV = *scalarMultVector(alpha, &v1, &scaledV);
            currEye = *vec4addition(&p1, &scaledV, &currEye);
            
            Mat4 tempMatrix = look_at(currEye.x, currEye.y, currEye.z, atx, aty, atz, 0.0, 1.0, 0.0);
            model_view_matrix = tempMatrix;
            
            alpha += 0.02;
        }
        else
        {
            alpha = 0.0;
            eyex = currEye.x;
            eyey = currEye.y;
            eyez = currEye.z;
  
            enableIdle = 4;
        }
    }
    // solve maze
    else if(enableIdle == 4)
    {
        /*
         -need which direction facing (N,S,E or W)q
         -need row/col you are in to know if
         there is a N,S,E,W wall in that cell
         -step = .005
         
         -based on that
             -if wall on left
                 -if no wall forward
                     -move forward one space
                 -if wall in front
                     -turn right 90 degrees
             -if no wall on left
                 -turn left 90 degrees
                 -move forward one space
         
         */
        
        // At end of maze! Turn around and end!
        if(currRow == 7 && currCol > 7)
        {
            situation = 4;
        }
        else
        {
           situation = getSituation(forward, &cells2D[currRow][currCol]);
        }
        
        // 1 = turn right 90 degrees
        // 2 = move forward one cell
        // 3 = turn left 90 degrees and move forward one space
        // 4 = out of maze, turn around and face exit
        
        if(situation == 1)
        {
            // Set up variables and send into
            // another idle() loop.
            if(forward == 'n')
            {
                // must turn to face east
                forward = 'e';
                
                atzFinal = eyez;
                atxFinal += 5*fabs(eyex);
                
                p1 = *vec4create(atx, aty, atz, 1.0, &p1);
                p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 5;
                
            }
            else if(forward == 's')
            {
                // must turn to face west
                forward = 'w';
                
                atzFinal = eyez;
                atxFinal -= 5*fabs(eyex);
                
                p1 = *vec4create(atx, aty, atz, 1.0, &p1);
                p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 5;
            }
            else if(forward == 'e')
            {
                // must turn to face south
                forward = 's';
                
                atxFinal = eyex;
                atzFinal += 5*fabs(eyez);
                
                p1 = *vec4create(atx, aty, atz, 1.0, &p1);
                p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 5;
            }
            else if(forward == 'w')
            {
                // must turn to face north
                forward = 'n';
                
                atxFinal = eyex;
                atzFinal -= 5*fabs(eyez);
                
                p1 = *vec4create(atx, aty, atz, 1.0, &p1);
                p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 5;
            }
                
        }
        // move forward one cell
        else if(situation == 2)
        {
            if(forward == 'n')
            {
                //Update current row & column
                currRow -= 1;
                
                atz -= 5*fabs(eyez);
                eyexFinal = eyex;
                eyezFinal = eyez - 0.25;
                
                p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
                p2 = *vec4create(eyexFinal, eyey, eyezFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 6;
            }
            else if(forward == 's')
            {
                //Update current row & column
                currRow += 1;
                
                atz += 5*fabs(eyez);
                eyexFinal = eyex;
                eyezFinal = eyez + 0.25;
                
                p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
                p2 = *vec4create(eyexFinal, eyey, eyezFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 6;
            }
            else if(forward == 'e')
            {
                //Update current row & column
                currCol += 1;
                
                atx += 5*fabs(eyex);
                eyezFinal = eyez;
                eyexFinal = eyex + 0.25;
                
                p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
                p2 = *vec4create(eyexFinal, eyey, eyezFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 6;
            }
            else if(forward == 'w')
            {
                //Update current row & column
                currCol -= 1;
                
                atx -= 5*fabs(eyex);
                eyezFinal = eyez;
                eyexFinal = eyex - 0.25;
                
                p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
                p2 = *vec4create(eyexFinal, eyey, eyezFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 6;
            }
        }
        // 3 = turn left 90 degrees and move forward one space
        else if(situation == 3)
        {
            if(forward == 'n')
            {
                // must turn to face west
                forward = 'w';
                
                atzFinal = eyez;
                atxFinal -= 5*fabs(eyex);
                
                p1 = *vec4create(atx, aty, atz, 1.0, &p1);
                p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 5;
                
            }
            else if(forward == 's')
            {
                // must turn to face east
                forward = 'e';
                
                atzFinal = eyez;
                atxFinal += 5*fabs(eyex);
                
                p1 = *vec4create(atx, aty, atz, 1.0, &p1);
                p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 5;
            }
            else if(forward == 'e')
            {
                // must turn to face north
                forward = 'n';
                
                atxFinal = eyex;
                atzFinal -= 5*fabs(eyez);
                
                p1 = *vec4create(atx, aty, atz, 1.0, &p1);
                p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 5;
            }
            else if(forward == 'w')
            {
                // must turn to face south
                forward = 's';
                
                atxFinal = eyex;
                atzFinal += 5*fabs(eyez);
                
                p1 = *vec4create(atx, aty, atz, 1.0, &p1);
                p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
                v1 = *vec4subtraction(&p2, &p1, &v1);
                
                // Enter animation loop
                enableIdle = 5;
            }
        }
        else if(situation == 4)
        {
            // turn around and face maze entrance
            // must turn to face south
            forward = 'w';
            
            atxFinal -= 5* fabs(eyex);
            atzFinal = eyez;
            eyex = 1.2;
            
            p1 = *vec4create(atx, aty, atz, 1.0, &p1);
            p2 = *vec4create(atxFinal, aty, atzFinal, 1.0, &p2);
            v1 = *vec4subtraction(&p2, &p1, &v1);
            
            // Enter animation loop
            enableIdle = 5;
            
            
            
        }
    }
    // Animates 'at' turng
    else if(enableIdle == 5)
    {
        if(alpha <= 1.0)
        {
            Vec4 scaledV = *scalarMultVector(alpha, &v1, &scaledV);
            currAt = *vec4addition(&p1, &scaledV, &currAt);
            
            Mat4 tempMatrix = look_at(eyex, eyey, eyez, currAt.x, currAt.y, currAt.z, 0.0, 1.0, 0.0);
            model_view_matrix = tempMatrix;
            
            alpha += 0.025;
        }
        else
        {
            alpha = 0.0;
            atx = currAt.x;
            aty = currAt.y;
            atz = currAt.z;
            
            
            if(situation == 3)
            {
                enableIdle = 7;
            }
            else if(situation == 4)
            {
                // maze solved, idle
                enableIdle = 0;
            }
            else
            {
                enableIdle = 4;
            }
        }
    }
    // Animates moving forward one cell
    else if(enableIdle == 6)
    {
        if(alpha <= 1.0)
        {
            Vec4 scaledV = *scalarMultVector(alpha, &v1, &scaledV);
            currEye = *vec4addition(&p1, &scaledV, &currEye);
            
            Mat4 tempMatrix = look_at(currEye.x, currEye.y, currEye.z, atx, aty, atz, 0.0, 1.0, 0.0);
            model_view_matrix = tempMatrix;
            
            alpha += 0.025;
        }
        else
        {
            alpha = 0.0;
            eyex = currEye.x;
            eyey = currEye.y;
            eyez = currEye.z;
            
            enableIdle = 4;
        }
    }
    else if(enableIdle == 7)
    {
        // set up moving forward after a left turn was made
        if(forward == 'n')
        {
            //Update current row & column
            currRow -= 1;
            
            atz -= 5*fabs(eyez);
            eyexFinal = eyex;
            eyezFinal = eyez - 0.25;
            
            p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
            p2 = *vec4create(eyexFinal, eyey, eyezFinal, 1.0, &p2);
            v1 = *vec4subtraction(&p2, &p1, &v1);
            
            // Enter animation loop
            enableIdle = 6;
        }
        else if(forward == 's')
        {
            //Update current row & column
            currRow += 1;
            
            atz += 5*fabs(eyez);
            eyexFinal = eyex;
            eyezFinal = eyez + 0.25;
            
            p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
            p2 = *vec4create(eyexFinal, eyey, eyezFinal, 1.0, &p2);
            v1 = *vec4subtraction(&p2, &p1, &v1);
            
            // Enter animation loop
            enableIdle = 6;
        }
        else if(forward == 'e')
        {
            //Update current row & column
            currCol += 1;
            
            atx += 5*fabs(eyex);
            eyezFinal = eyez;
            eyexFinal = eyex + 0.25;
            
            p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
            p2 = *vec4create(eyexFinal, eyey, eyezFinal, 1.0, &p2);
            v1 = *vec4subtraction(&p2, &p1, &v1);
            
            // Enter animation loop
            enableIdle = 6;
        }
        else if(forward == 'w')
        {
            //Update current row & column
            currCol -= 1;
            
            atx -= 5*fabs(eyex);
            eyezFinal = eyez;
            eyexFinal = eyex - 0.25;
            
            p1 = *vec4create(eyex, eyey, eyez, 1.0, &p1);
            p2 = *vec4create(eyexFinal, eyey, eyezFinal, 1.0, &p2);
            v1 = *vec4subtraction(&p2, &p1, &v1);
            
            // Enter animation loop
            enableIdle = 6;
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
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
