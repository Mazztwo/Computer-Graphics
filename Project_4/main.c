/*
 Alessio Mazzone
 CS1566 Computer Graphics
 University of Pittsburgh
 
 Main file for Project 3
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
#define num_spheres 5
#define num_shadows num_spheres
#define groundVertices 6
#define sphereVertices 16206
///////// num_vertices is 16206 for a 5 degree increment
#define num_vertices (num_spheres*sphereVertices) + groundVertices
////////////////////////////////////////////////////////

Vec4 vertices[num_vertices];
Vec4 normals[num_vertices];

///////////// Lookat and frustum variables/////////////////////////////////////////
float eyex = 0.0, eyey = 0.0, eyez = 0.0;
float atx = 0.0, aty = 0.0, atz = 0.0;
float left = -0.5, right = 0.5, bottom = -0.5, top = 0.5, near = -.5, far = -100.0;
///////////////////////////////////////////////////////////////////////////////////

float DegreesToRadians = M_PI / 180.0;;


float degrees_x, degrees_y, radius;
Mat4 model_view_rotation =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
};

Vec4 originVector = {0.0,0.0,0.0,0.0};


int enableIdle = 0;

// vertices[] and normals[] index
int v_index = 0;

GLuint projection_matrix_location, model_view_matrix_location, ctm_location;
GLuint AmbientProduct_location, DiffuseProduct_location, SpecularProduct_location, LightPosition_location;
GLuint shininess_location, attenuation_constant_location, attenuation_linear_location, attenuation_quadratic_location, isShadow_location;

Vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
float shininess;
float attenuation_constant = 0.2, attenuation_linear = 1.0, attenuation_quadratic = 1.0;

float phi = 50, theta = 90, radius = 2.0;

// Lighting model attributes
Vec4 light_ambient = {0.2, 0.2, 0.2, 1.0};
Vec4 light_diffuse = {1.0, 1.0, 1.0, 1.0};
Vec4 light_specular = {1.0, 1.0, 1.0, 1.0};
Vec4 LightPosition = {0, 0.8 , 0, 1.0};

// Light ball sphere, not actual light
Vec4 Light_Color = {1.0, 1.0, 1.0, 1.0};


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

// Transformation matricies for each sphere
Mat4 transformation_matricies[num_spheres] =
{
    {{1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}},
    
    {{1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}},
    
    {{1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}},
    
    {{1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}},
    
    {{1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}},
};

// ctm for ground
Mat4 ground_transformation =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
};

// ground vertices
Vec4 ground_vertices[groundVertices] =
{
    {-1.0, -0.2, -0.6, 1.0},
    {-1.0, -0.2,  0.6, 1.0},
    { 1.0, -0.2, -0.6, 1.0},
    
    { 1.0, -0.2, -0.6, 1.0},
    {-1.0, -0.2,  0.6, 1.0},
    { 1.0, -0.2,  0.6, 1.0}
    
    
};

// Rotation matrices for each color sphere
Mat4 rotation_matrices[num_spheres] =
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
        {0,0,0,1}},
    
    {{1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}},
};


// Color for each of the 5 spheres
Vec4 sphere_colors[num_spheres] =
{
    {1,1,0,1},          // Yellow
    {1,0,0,1},          // Red
    {0,1,0,1},          // Green
    {0,0,1,1},          // Blue
    {1,.5,0,1},         // Orange
};


// materials
// Ambient, Diffuse, Specular

// Ground
material ground_material = {{0.773911, 0.773911, 0.773911, 1.0}, {0.773911, 0.773911, 0.773911, 1.0}, {0.773911, 0.773911, 0.773911, 1.0}, 89.6};

// Fancy materials!
material sphere_materials =
{
    // Brass
    {0.329412, 0.223529, 0.027451, 1.0}, {0.780392, 0.568627, 0.113725, 1.0}, {0.992157, 0.941176, 0.807843, 1.0}, 27.8974
};











void initSphere(float divisionDegrees)
{
    
    for (float phi = -90.0; phi <= 90.0; phi += divisionDegrees)
    {
        float phir = phi * DegreesToRadians;
        float phir20 = (phi + divisionDegrees)*DegreesToRadians;
        
        for(float theta = -180.0; theta <= 180.0; theta += divisionDegrees)
        {
            float thetar = theta*DegreesToRadians;
            float thetar20 = (theta + divisionDegrees)*DegreesToRadians;
        
            vecArrayAdd(vertices, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 1);
            vecArrayAdd(normals, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 0);;
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar)*cos(phir20), cos(thetar)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(normals, v_index, sin(thetar)*cos(phir20), cos(thetar)*cos(phir20), sin(phir20), 0);
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(normals, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 0);
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 1);
            vecArrayAdd(normals, v_index, sin(thetar20)*cos(phir20), cos(thetar20)*cos(phir20), sin(phir20), 0);
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar20)*cos(phir), cos(thetar20)*cos(phir), sin(phir), 1);
            vecArrayAdd(normals, v_index, sin(thetar20)*cos(phir), cos(thetar20)*cos(phir), sin(phir), 0);
            v_index++;
            
            vecArrayAdd(vertices, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 1);
            vecArrayAdd(normals, v_index, sin(thetar)*cos(phir), cos(thetar)*cos(phir), sin(phir), 0);
            v_index++;
        }
    }
}




void initGround()
{
    for(int i = 0; i < groundVertices; i++)
    {
        
        vecArrayAdd(vertices, v_index, ground_vertices[i].x, ground_vertices[i].y, ground_vertices[i].z, 1);
        vecArrayAdd(normals, v_index, ground_vertices[i].x, ground_vertices[i].y, ground_vertices[i].z, 0);
        
        v_index++;
    }
    
}





void init(void)
{
    
    eyex = radius*sin(phi*(M_PI/180.0))*cos(theta*(M_PI/180.0));
    eyey = radius*cos(phi*(M_PI/180.0));
    eyez = radius*sin(phi*(M_PI/180.0))*sin(theta*(M_PI/180.0));
                            
    
    // Initialize model_view matrix
    Mat4 temp1 = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
    model_view_matrix = temp1;
    
    // Initialize frustum
    temp1 = frustum(left, right, bottom, top, near, far);
    projection_matrix = temp1;
    
    
    // Initialize spheress
    // x,y,z coordinates sphere centers
    float x = -.4, y = 0.2 , z = 0;
    Mat4 scaling_matrix;
    Mat4 translation_matrix;
    int i;

    for(i = 0; i < 5; i++)
    {
        initSphere(5.0);
        
        // Generate scaling matrix
        scaling_matrix = *scaleMatrix(0.1, &scaling_matrix);
        
        // Generate Translation matrix1
        translation_matrix = *translate(x, y, z, &translation_matrix);
        
        // Apply scaling, then translation
        temp1 = *matMultiplication(&translation_matrix, &scaling_matrix, &temp1);
        transformation_matricies[i] = temp1;
        
        // Move center of next sphere
        x += 0.2;
    }
    

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    
    // Generate buffer to store normals and vertices
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
    
    
    // Send in position
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    
    // Send in normal
    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(normals));
    
    
    // Load in matricies to the vertex shader
    projection_matrix_location = glGetUniformLocation(program, "projection_matrix");
    model_view_matrix_location = glGetUniformLocation(program, "model_view_matrix");
    ctm_location = glGetUniformLocation(program, "ctm");
    
    
    // Load the rest
    AmbientProduct_location = glGetUniformLocation(program, "AmbientProduct");
    DiffuseProduct_location = glGetUniformLocation(program, "DiffuseProduct");
    SpecularProduct_location = glGetUniformLocation(program, "SpecularProduct");
    LightPosition_location = glGetUniformLocation(program, "LightPosition");
    
    shininess_location = glGetUniformLocation(program, "shininess");
    isShadow_location = glGetUniformLocation(program, "isShadow");
    attenuation_constant_location = glGetUniformLocation(program, "attenuation_constant");
    attenuation_linear_location = glGetUniformLocation(program, "attenuation_linear");
    attenuation_quadratic_location = glGetUniformLocation(program, "attenuation_quadratic");
    

    glEnable(GL_DEPTH_TEST);

    
    glClearColor(0.4, 0.4, 0.4, 1.0);
    glDepthRange(1,0);
}






void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    
    // Light Position
    glUniform4fv(LightPosition_location, 1, (GLfloat *) &LightPosition);
    
    // Send attenuation info
    glUniform1f(attenuation_constant_location, attenuation_constant);
    glUniform1f(attenuation_linear_location, attenuation_linear);
    glUniform1f(attenuation_quadratic_location, attenuation_quadratic);
    
    // Load frustum
    glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);
    
    // Load Model view
    glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);

    // Load Ground information
    /////////////////////////////////////////////////////////////////
    // Ambient product (array of vectors)
    Vec4 temp = *product(ground_material.reflect_ambient, light_ambient, &temp);
    AmbientProduct = temp;
    glUniform4fv(AmbientProduct_location, 1, (GLfloat *) &AmbientProduct);
    
    // Diffuse product (array of vectors)
    temp = *product(ground_material.reflect_diffuse, light_diffuse, &temp);
    DiffuseProduct = temp;
    glUniform4fv(DiffuseProduct_location, 1, (GLfloat *) &DiffuseProduct);
    
    // Specular product (array of vectors)
    temp = *product(ground_material.reflect_specular, light_specular, &temp);
    SpecularProduct = temp;
    glUniform4fv(SpecularProduct_location, 1, (GLfloat *) &SpecularProduct);
    
    // Shininess (array of floats, just sent 1 here)
    glUniform1f(shininess_location, ground_material.shininess);
    
    // Draw ground after sending in all light info.
    // Else will use whatever is in memory
    glUniform1i(isShadow_location, 0);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ground_transformation);
    glDrawArrays(GL_TRIANGLES, 0, groundVertices);
    /////////////////////////////////////////////////////////////////
    
    
    // Load Sphere information
    /////////////////////////////////////////////////////////////////
    for(int i = 0; i < num_spheres; i++)
    {
        glUniform1i(isShadow_location, 0);
        // Ambient product (array of vectors)
        temp = *product(sphere_materials.reflect_ambient, light_ambient, &temp);
        AmbientProduct = temp;
        glUniform4fv(AmbientProduct_location, 1, (GLfloat *) &AmbientProduct);
            
        // Diffuse product (array of vectors)
        temp = *product(sphere_materials.reflect_diffuse, light_diffuse, &temp);
        DiffuseProduct = temp;
        glUniform4fv(DiffuseProduct_location, 1, (GLfloat *) &DiffuseProduct);
            
        // Specular product (array of vectors)
        temp = *product(sphere_materials.reflect_specular, light_specular, &temp);
        SpecularProduct = temp;
        glUniform4fv(SpecularProduct_location, 1, (GLfloat *) &SpecularProduct);
    
        // Shininess (array of floats, just sent 1 here)
        glUniform1f(shininess_location, sphere_materials.shininess);
            
        // Draw sphere after sending in all light info.
        // Else will use whatever is in memory
        glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &transformation_matricies[i]);
        glUniform1i(isShadow_location, 0);
        glDrawArrays(GL_TRIANGLES, groundVertices + (sphereVertices * i), sphereVertices);
        
            
        // Send in shadow info
        glUniform1i(isShadow_location, 1);
        glDrawArrays(GL_TRIANGLES, groundVertices + (sphereVertices * i), sphereVertices);
    }

    
    
    
    
    glutSwapBuffers();
}







void keyboard(unsigned char key, int mousex, int mousey)
{
    // Quit program
    if(key == 'q')
    {
        exit(0);
    }
    // Controls for eye
    else if(key == 'w')
    {
        phi -= 5.0;
    }
    else if(key == 's')
    {
        phi += 5.0;
    }
    else if(key == 'a')
    {
        theta += 5.0;
    }
    else if(key == 'd')
    {
        theta -= 5.0;
    }
    else if(key == 'r')
    {
        radius -= 0.1;
    }
    else if(key == 'f')
    {
        radius += 0.1;
    }
    // Controls for light position
    else if(key == 'x')
    {
        LightPosition.x -= 0.05;
    }
    else if(key == 'X')
    {
        LightPosition.x += 0.05;
    }
    else if(key == 'y')
    {
        LightPosition.y -= 0.05;
    }
    else if(key == 'Y')
    {
        LightPosition.y += 0.05;
    }
    else if(key == 'z')
    {
        LightPosition.z -= 0.05;
    }
    else if(key == 'Z')
    {
        LightPosition.z += 0.05;
    }
    // Starts rotation
    else if (key == ' ')
    {

    }
    
    // Recalculate model_view matrix
    eyex = radius*sin(phi*(M_PI/180.0))*cos(theta*(M_PI/180.0));
    eyey = radius*cos(phi*(M_PI/180.0));
    eyez = radius*sin(phi*(M_PI/180.0))*sin(theta*(M_PI/180.0));
    
    Mat4 temp1 = look_at(eyex, eyey, eyez, atx, aty, atz, 0.0, 1.0, 0.0);
    model_view_matrix = temp1;
    


    glutPostRedisplay();
    
}


// Listner for mouse button events
void mouse(int button, int state, int x, int y)
{
    
}



// Listner for mouse motion
void motion(int x, int y)
{
    
}

void idle(void)
{
 
    glutPostRedisplay();
}



int main(int argc, char **argv)
{
    initGround();
    
    // OpenGL initializaiton code
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize*2, windowSize*2);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 4 - Newton's Cradle");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
