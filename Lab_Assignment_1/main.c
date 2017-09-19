/*
LabAssignment1 Main
This file will contain the main() for 
Lab assignment 1.
*/

#include "funct.h"
#include <stdio.h>

int main()
{   
    Vec4 vector1 = {1.0,2.0,3.0,0.0};
    Vec4 vector2 = {3.0,2.0,3.0,0.0};
    Vec4 xProduct = {0,0,0,0}; // vec1 X vec2 = xProduct

    // Test printVec4 method
    printVec4(&vector1);
    printVec4(&vector2);
    //////////////////////////

    // Test crossProduct method
    xProduct = *crossProduct(&vector1, &vector2, &xProduct);
    printVec4(&xProduct);
    //////////////////////////



    
    return 0;
}

