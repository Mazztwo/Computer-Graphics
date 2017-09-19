/*Lab Assignment 1
 This file will contain all the implementations of the 
 functions required of the Lab Assignment: 1 
 instruction pdf.

 */


#include "funct.h"
#include <stdio.h>



/*
Function:
    This methods will derefrence a vec4 pointer
    and print its x,y,z, and w components.
Inputs: 
    Vec4 pointer VectorTo Print --> Contains a
    pointer to a vec4 struc to print to terminal.
Outputs:
    None
    
*/
void printVec4(Vec4 *ptr_vectorToPrint)
{   
    // Declare vec4 components and 
    // extract from vector
    float xComp, yComp, zComp, wComp;
    
    xComp = ptr_vectorToPrint->x;
    yComp = ptr_vectorToPrint->y;
    zComp = ptr_vectorToPrint->z;
    wComp = ptr_vectorToPrint->w;
    
    // Print each component of vector
    printf("[%f,%f,%f,%f]\n", xComp,yComp,zComp,wComp);

}

