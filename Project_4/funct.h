/*
 Alessio Mazzone
 CS1566 Computer Graphics
 University of Pittsburgh
 
 Header file Linear Algebra & helper functions.
*/


#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <math.h>


typedef struct
{
    float x;
    float y; 
    float z;
    float w;
}Vec4;

typedef struct
{
    Vec4 col1;
    Vec4 col2;
    Vec4 col3;
    Vec4 col4;
}Mat4;

typedef struct
{
    float x;
    float y;
    float z;
} Vec3;

typedef struct
{
    Vec4 reflect_ambient;
    Vec4 reflect_diffuse;
    Vec4 reflect_specular;
    GLfloat shininess;
} material;



void printVec4(Vec4 *vectorToPrint);
Vec4* crossProduct(Vec4 *vec1, Vec4 *vec2, Vec4 *productVector);
float dotProduct(Vec4 *vec1, Vec4 *vec2);
Vec4* scalarMultVector(float scalar, Vec4 *vec, Vec4 *productVector);
Vec4* vec4addition(Vec4* vec1, Vec4* vec2, Vec4 *sumVec);
Vec4* vec4subtraction(Vec4* vec1, Vec4* vec2, Vec4 *subVec);
float angleBetweenVectors(Vec4 *vec1, Vec4 *vec2);
Vec4* vec4create(float x, float y, float z, float w, Vec4 *result);
float vecMagnitude(Vec4 *vec);
Vec4* product(Vec4 u, Vec4 v, Vec4 *result);

void printMat4(Mat4 *mat);
Mat4* scalarMultMatrix(float scalar, Mat4 *mat, Mat4 *productMatrix);
Mat4* mat4addition(Mat4 *mat1, Mat4 *mat2, Mat4 *sumMatrix);
Mat4* mat4subtraction(Mat4 *mat1, Mat4 *mat2, Mat4 *subMatrix);
Mat4* matMultiplication(Mat4 *mat1, Mat4 *mat2, Mat4 *productMatrix);
Mat4* inverseMatrix(Mat4 *mat1, Mat4 *inverse);
Mat4* transposeMatrix(Mat4 *mat1, Mat4 *transpose);
Mat4* matRotateAboutX(float theta, Mat4 *productMatrix);
Mat4* matRotateAboutY(float theta, Mat4 *productMatrix);
Mat4* matRotateAboutZ(float theta, Mat4 *productMatrix);

Mat4* scaleMatrix(float scalingFactor, Mat4 *productMatrix);
Mat4* translate(float x, float y, float z, Mat4 *productMatrix);

Mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez,
             GLfloat atx, GLfloat aty, GLfloat atz,
             GLfloat upx, GLfloat upy, GLfloat upz);
Mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
             GLfloat near, GLfloat far);

void vecArrayAdd(Vec4 *array, int index, float xVal, float yVal, float zVal, float wVal);
float maxNum(float num1, float num2);



