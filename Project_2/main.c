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


//Vec4 *vertices;
//Vec4 *colors;


int num_vertices;

float eyex = -1.0, eyey = 1.0, eyez = -1.0;


// Declare point & vector pointing from initial mouse click to origin
Vec4 originVector = {0.0,0.0,0.0,0.0};

// Declare vector of motion starting from origin point
Vec4 motionVector = {0.0,0.0,0.0,0.0};

// Declare axis of rotation
Vec4 rotationAxis = {0.0,0.0,0.0,0.0};

GLuint ctm_location;

GLuint projection_matrix_location;
GLuint model_view_matrix_location;

// Changes worldview to be at angle
Mat4 projection_matrix =
{
    {0.5, 0.0, 0.0, 0.0},
    {0.0, 0.5, 0.0, 0.0},
    {0.0, 0.0, 0.5, 0.0},
    {0.0, 0.0, 0.0, 1.0}
    
};

// Changes view of world in relationship to camera
Mat4 model_view_matrix =
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
    
    numRows = 8, numColumns = 8;
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
    
    // Set number of vertices and initialize vertex
    // and color arrays.
    num_vertices = sizeOfGround + (num_walls*sizeOfWall);
    
    //vertices = (Vec4 *)malloc(sizeof(Vec4) * num_vertices);
    //colors = (Vec4 *)malloc(sizeof(Vec4) * num_vertices);
    
    // Clear malloc'ed memory
    for (i = 0; i < num_vertices; i++)
    {
        vecArrayAdd(vertices, i, 0.0, 0.0, 0.0, 0.0);
        vecArrayAdd(colors, i, 0.0, 0.0, 0.0, 0.0);
    }
    
     // Cast the one-dimensional array of cells into two-dimensional
    cell (*cells2D)[numColumns] = (cell (*)[numColumns]) cells;
    
    int row, column, v_index = 0;
    float wallSize = 0;
    
    
    // Set ground and ground color
    vecArrayAdd(vertices, v_index, 1.0, -1.0 , -1.0, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    vecArrayAdd(vertices, v_index, -1.0, -1.0, -1.0, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    vecArrayAdd(vertices, v_index, 1.0, -1.0, 1.0, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    // Second Triangle
    vecArrayAdd(vertices, v_index, 1.0, -1.0, 1.0, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    vecArrayAdd(vertices, v_index, -1.0, -1.0, -1.0, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    vecArrayAdd(vertices, v_index, -1.0, -1.0, 1.0, 1.0);
    vecArrayAdd(colors, v_index, 0.0, 0.6, 0.0, 1.0);
    v_index++;
    
    // Top left corner is {-1,-1,-1}
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
    Mat4 tempMatrix = look_at(-1.0, 1.0, -1.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0);
    model_view_matrix = tempMatrix;
    
    //tempMatrix = frustum(1.0, -1.0, 1.0, -1.0, 1.0, -2.0);
    //projection_matrix = tempMatrix;
    
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
    // Zoom In
    else if(key == 'o')
    {
        Mat4 tempMatrix = *scaleMatrix(&projection_matrix, 1.02, &tempMatrix);
        projection_matrix = tempMatrix;
  
    }
    // Zoom Out
    else if(key == 'l')
    {
        Mat4 tempMatrix = *scaleMatrix(&projection_matrix, 1.0/1.02, &tempMatrix);
        projection_matrix = tempMatrix;
       
    }
    else if(key == ' ')
    {
        enableIdle = 1;
        
    }

    glutPostRedisplay();
    
}


void idle(void)
{
    
    if(enableIdle)
    {
        Mat4 tempMatrix = look_at(eyex, eyey, eyez, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0);
        model_view_matrix = tempMatrix;
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
                Mat4 tempMatrix5 = *matMultiplication(&R,&projection_matrix,&tempMatrix5);
                projection_matrix = tempMatrix5;
            
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
