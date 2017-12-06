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
#define groundVertices 12
#define poleVertices 18
#define stringVertices num_spheres * 6
#define sphereVertices 16206
///////// num_vertices is 16206 for a 5 degree increment
#define num_vertices (num_spheres*sphereVertices) + groundVertices + poleVertices + stringVertices
////////////////////////////////////////////////////////

Vec4 vertices[num_vertices];
Vec4 normals[num_vertices];

///////////// Lookat and frustum variables/////////////////////////////////////////
float eyex = 0.0, eyey = 0.0, eyez = 0.0;
float atx = 0.0, aty = 0.0, atz = 0.0;
float left = -0.5, right = 0.5, bottom = -0.5, top = 0.5, near = -.5, far = -100.0;
///////////////////////////////////////////////////////////////////////////////////

float DegreesToRadians = M_PI / 180.0;
float RadiansToDegrees = 180.0 / M_PI;

float degrees_x, degrees_y, radius;

int enableIdle = 0;


// vertices[] and normals[] index
int v_index = 0;

GLuint projection_matrix_location, model_view_matrix_location, ctm_location;
GLuint AmbientProduct_location, DiffuseProduct_location, SpecularProduct_location, LightPosition_location;

GLuint shininess_location, attenuation_constant_location, attenuation_linear_location, attenuation_quadratic_location, isShadow_location;

Vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
float shininess;
float attenuation_constant = 0.1, attenuation_linear = 0.1, attenuation_quadratic = 0.1;

// Camera control variables
float phi = 70, theta = 90, radius = 3.0;

// Lighting model attributes
Vec4 light_ambient = {0.2, 0.2, 0.2, 1.0};
Vec4 light_diffuse = {1.0, 1.0, 1.0, 1.0};
Vec4 light_specular = {1.0, 1.0, 1.0, 1.0};
Vec4 LightPosition = {0, 2 , .5, 1.0};

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
Mat4 sphere_transformation_matrices[num_spheres] =
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
    {-2.4, -1.5, -.7, 1.0},
    {-2.4, -1.5,  .7, 1.0},
    { 2.4, -1.5, -.7, 1.0},
    
    { 2.4, -1.5, -.7, 1.0},
    {-2.4, -1.5,  .7, 1.0},
    { 2.4, -1.5,  .7, 1.0},
    
    {-2.4, -1.5, .7, 1.0},
    {-2.4, -1.9, .7, 1.0},
    {2.4, -1.5, .7, 1.0},
    
    { 2.4, -1.5, .7, 1.0},
    {-2.4, -1.9, .7, 1.0},
    {2.4, -1.9, .7, 1.0},
};




// materials
// Ambient, Diffuse, Specular

// Ground
material ground_material = {{1,0,0,1}, {1,0,0,1}, {1,0,0,1}, 100};


material sphere_materials[num_spheres] =
{
    // Spheres
    {{0.19225, 0.19225, 0.19225, 1.0}, {0.50754, 0.50754, 0.50754, 1.0}, {0.508273, 0.508273, 0.508273, 1.0}, 100},
    {{0.19225, 0.19225, 0.19225, 1.0}, {0.50754, 0.50754, 0.50754, 1.0}, {0.508273, 0.508273, 0.508273, 1.0}, 100},
    {{0.19225, 0.19225, 0.19225, 1.0}, {0.50754, 0.50754, 0.50754, 1.0}, {0.508273, 0.508273, 0.508273, 1.0}, 100},
    {{0.19225, 0.19225, 0.19225, 1.0}, {0.50754, 0.50754, 0.50754, 1.0}, {0.508273, 0.508273, 0.508273, 1.0}, 100},
    {{0.19225, 0.19225, 0.19225, 1.0}, {0.50754, 0.50754, 0.50754, 1.0}, {0.508273, 0.508273, 0.508273, 1.0}, 100}
    
};





float sphere_offsets[num_spheres] = {-1,-.5,0,.5,1};
float sphere_degrees[num_spheres] = {270,270,270,270,270};
int ball_up[num_spheres] = {0,0,0,0,0};

float GRAVITY = 9.80665, velocity = 0.0;
float oldTime, currTime;

Vec4 resting_sphere_centers[num_spheres] =
{
    {-1,-1,0,1},
    {-.5,-1,0,1},
    {0,-1,0,1},
    {.5,-1,0,1},
    {1,-1,0,1},
};


Vec4 curr_sphere_centers[num_spheres] =
{
    {-1,-1,0,1},
    {-.5,-1,0,1},
    {0,-1,0,1},
    {.5,-1,0,1},
    {1,-1,0,1},
};


// Ground
material pole_material = {{0,0,0,1}, {0,0,0,1}, {0,0,0,1}, 100};


Vec4 pole_vertices[poleVertices] =
{
    {-.1, 0, -.7, 1.0},
    {-.1, -1.5, -.7, 1.0},
    {.1, 0, -.7, 1.0},
    
    {.1, 0, -.7, 1.0},
    {-.1, -1.5, -.7, 1.0},
    {.1, -1.5, -.7, 1.0},
    
    {-.1, 0, -.7, 1},
    {-.1, 0, 0, 1},
    {.1, 0, -.7, 1},
    
    {.1, 0, -.7, 1},
    {-.1, 0, 0, 1},
    {.1, 0, 0, 1}
};

Mat4 pole_transformation =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
};


material string_material = {{0,0,0,1}, {0,0,0,1}, {0,0,0,1}, 100};




Vec4 string_vertices[stringVertices] =
{
    {-1.025, 0, 0, 1},
    {-1.025, -1, 0, 1},
    {-.975, 0, 0, 1},
    
    {-.975, 0, 0, 1},
    {-1.025, -1, 0, 1},
    {-.975, -1, 0, 1},
    
    {-.525, 0, 0, 1},
    {-.525, -1, 0, 1},
    {-.475, 0, 0, 1},
    
    {-.475, 0, 0, 1},
    {-.525, -1, 0, 1},
    {-.475, -1, 0, 1},
    
    {-.025, 0, 0, 1},
    {-.025, -1, 0, 1},
    {.025, 0, 0, 1},
    
    {.025, 0, 0, 1},
    {-.025, -1, 0, 1},
    {.025, -1, 0, 1},
    
    {.475, 0, 0, 1},
    {.525, -1, 0, 1},
    {.475, -1, 0, 1},
    
    {.525, 0, 0, 1},
    {.525, -1, 0, 1},
    {.475, 0, 0, 1},
    
    {.975, 0, 0, 1},
    {1.025, -1, 0, 1},
    {.975, -1, 0, 1},
    
    {1.025, 0, 0, 1},
    {1.025, -1, 0, 1},
    {.975, 0, 0, 1},
};

Mat4 string_transformation =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
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





void initPoles()
{
    for(int i = 0; i < poleVertices; i++)
    {
        
        vecArrayAdd(vertices, v_index, pole_vertices[i].x, pole_vertices[i].y, pole_vertices[i].z, 1);
        vecArrayAdd(normals, v_index, pole_vertices[i].x, pole_vertices[i].y, pole_vertices[i].z, 0);
        
        v_index++;
    }
}

void initStrings()
{
    for(int i = 0; i < stringVertices; i++)
    {
        
        vecArrayAdd(vertices, v_index, string_vertices[i].x, string_vertices[i].y, string_vertices[i].z, 1);
        vecArrayAdd(normals, v_index, string_vertices[i].x, string_vertices[i].y, string_vertices[i].z, 0);
        
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
    float x = -1, y = -1 , z = 0;
    Mat4 scaling_matrix;
    Mat4 translation_matrix;
    int i;
    
    
    for(i = 0; i < 5; i++)
    {
        initSphere(5.0);
        
        // Generate scaling matrix
        scaling_matrix = *scaleMatrix(.25, &scaling_matrix);
        
        // Generate Translation matrix1
        translation_matrix = *translate(x, y, z, &translation_matrix);
        
        // Apply scaling, then translation
        temp1 = *matMultiplication(&translation_matrix, &scaling_matrix, &temp1);
        sphere_transformation_matrices[i] = temp1;
    
        // Move center of next sphere
        x += .5;
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
    
    // Load Pole info
    ////////////////////
    temp = *product(ground_material.reflect_ambient, light_ambient, &temp);
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
    glUniform1f(shininess_location, pole_material.shininess);
    
    // Draw poles after sending in all light info.
    // Else will use whatever is in memory
    glUniform1i(isShadow_location, 0);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &pole_transformation);
    glDrawArrays(GL_TRIANGLES, groundVertices, poleVertices);
    ////////////////////
    
    // Load strings
    /////////////////////////////////////////
    for(int i = 0; i < num_spheres; i++)
    {
        temp = *product(string_material.reflect_ambient, light_ambient, &temp);
        AmbientProduct = temp;
        glUniform4fv(AmbientProduct_location, 1, (GLfloat *) &AmbientProduct);
        
        // Diffuse product (array of vectors)
        temp = *product(string_material.reflect_diffuse, light_diffuse, &temp);
        DiffuseProduct = temp;
        glUniform4fv(DiffuseProduct_location, 1, (GLfloat *) &DiffuseProduct);
        
        // Specular product (array of vectors)
        temp = *product(string_material.reflect_specular, light_specular, &temp);
        SpecularProduct = temp;
        glUniform4fv(SpecularProduct_location, 1, (GLfloat *) &SpecularProduct);
        
        // Shininess (array of floats, just sent 1 here)
        glUniform1f(shininess_location, string_material.shininess);
        
        // Draw poles after sending in all light info.
        // Else will use whatever is in memory
        glUniform1i(isShadow_location, 0);
        glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &string_transformation);
        glDrawArrays(GL_TRIANGLES, groundVertices + poleVertices + (stringVertices*i), stringVertices);
    }
    ////////////////////////////////////////
    
    
    // Load Sphere information
    /////////////////////////////////////////////////////////////////
    for(int i = 0; i < num_spheres; i++)
    {
        glUniform1i(isShadow_location, 0);
        // Ambient product (array of vectors)
        temp = *product(sphere_materials[i].reflect_ambient, light_ambient, &temp);
        AmbientProduct = temp;
        glUniform4fv(AmbientProduct_location, 1, (GLfloat *) &AmbientProduct);
            
        // Diffuse product (array of vectors)
        temp = *product(sphere_materials[i].reflect_diffuse, light_diffuse, &temp);
        DiffuseProduct = temp;
        glUniform4fv(DiffuseProduct_location, 1, (GLfloat *) &DiffuseProduct);
            
        // Specular product (array of vectors)
        temp = *product(sphere_materials[i].reflect_specular, light_specular, &temp);
        SpecularProduct = temp;
        glUniform4fv(SpecularProduct_location, 1, (GLfloat *) &SpecularProduct);
    
        // Shininess (array of floats, just sent 1 here)
        glUniform1f(shininess_location, sphere_materials[i].shininess);
            
        // Draw sphere after sending in all light info.
        // Else will use whatever is in memory
        glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &sphere_transformation_matrices[i]);
        glUniform1i(isShadow_location, 0);
        glDrawArrays(GL_TRIANGLES, stringVertices + poleVertices + groundVertices + (sphereVertices * i), sphereVertices);
        
            
        // Send in shadow info
        glUniform1i(isShadow_location, 1);
        glDrawArrays(GL_TRIANGLES, stringVertices + poleVertices + groundVertices + (sphereVertices * i), sphereVertices);
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
    else if(key == 'B')
    {
        sphere_degrees[4] += 5.0;
        ball_up[4] = 1;
        
    }
    else if(key == 'b')
    {
        if( !(sphere_degrees[4] < 275 ))
        {
            sphere_degrees[4] -= 5.0;
            ball_up[4] = 1;
        }
        else
        {
            ball_up[4] = 0;
        }
    }
    else if(key == 'V')
    {
        sphere_degrees[3] += 5.0;
        ball_up[3] = 1;
        
        sphere_degrees[4] += 5.0;
        ball_up[4] = 1;
        
    }
    else if(key == 'v')
    {
        if( !(sphere_degrees[3] < 275 ))
        {
            sphere_degrees[3] -= 5.0;
            ball_up[3] = 1;
        }
        else
        {
            ball_up[3] = 0;
        }
    }
    else if(key == 'C')
    {
        sphere_degrees[2] += 5.0;
        if(sphere_degrees[2] != 270)
        {
            ball_up[2] = 1;
        }
        else
        {
            ball_up[2] = 0;
        }
    }
    else if(key == 'c')
    {
        sphere_degrees[2] -= 5.0;
        if(sphere_degrees[2] != 270)
        {
            ball_up[2] = 1;
        }
        else
        {
            ball_up[2] = 0;
        }
    
    }
    else if(key == 'X')
    {
        if( !(sphere_degrees[1] > 265 ))
        {
            sphere_degrees[1] += 5.0;
            ball_up[1] = 1;
        }
        else
        {
            ball_up[1] = 0;
        }
        
    }
    else if(key == 'x')
    {
        sphere_degrees[1] -= 5.0;
        ball_up[1] = 1;
        sphere_degrees[0] -= 5.0;
        ball_up[0] = 1;
    }
    else if(key == 'Z')
    {
        if( !(sphere_degrees[0] > 265 ))
        {
            sphere_degrees[0] += 5.0;
            ball_up[0] = 1;
        }
        else
        {
            ball_up[0] = 0;
        }
        
    }
    else if(key == 'z')
    {
        sphere_degrees[0] -= 5.0;
        ball_up[0] = 1;
    }
    // Start swinging animation
    else if(key == ' ')
    {
        if(enableIdle)
        {
            enableIdle = 0;
            
            for(int i = 0; i < num_spheres; i++)
            {
                ball_up[i] = 0;
            }
            
            velocity = 0;
        }
        else
        {
            enableIdle = 1;
            //oldTime = glutGet(GLUT_ELAPSED_TIME);
            oldTime = (float) clock() / CLOCKS_PER_SEC;
        }
    }
    

    
    
    // Recalculate new sphere positions
    for(int i = 0; i < num_spheres; i++)
    {
        
        float newX = cosf(DegreesToRadians*sphere_degrees[i]) + sphere_offsets[i];
        float newY = sinf(DegreesToRadians*sphere_degrees[i]);
        
        Mat4 translation = *translate(newX, newY, 0.0, &translation);
        Mat4 scale = *scaleMatrix(.25, &scale);
        Mat4 temp = *matMultiplication(&translation, &scale, &temp);
        sphere_transformation_matrices[i] = temp;
        
        // Update current sphere centers
        vecArrayAdd(curr_sphere_centers, i, newX, newY, 0.0, 1.0);
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
    if(button == GLUT_LEFT_BUTTON)
    {
   
    }
    
    glutPostRedisplay();
}





// Listner for mouse motion
void motion(int x, int y)
{
    // Read Pixel Color under mouse.
    // R: first number is biggest
    // G: second number is biggest
    // B: third number is biggest
    // Y: first two numbers are equal
    // SkyBlue: last two numbers are equal
    // Solid colors: All three numbers are the same
    unsigned char color[3];
    glReadPixels(x, 750 - y, 1,1,GL_RGB, GL_UNSIGNED_BYTE, &color);
    
    //printf("Color: [%d %d %d]\n", color[0], color[1], color[2]);
    
    
    // Translate sphere center to be where mouse is...?
    // Generate translation matrix to be at curr mouse position
    GLint window[4]; //var to hold the viewport info
    GLdouble modelview[16]; //var to hold the modelview info
    GLdouble projection[16]; //var to hold the projection matrix info
    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates
    
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, window ); //get the viewport info
    
    // Calculate window based on world
    winX = (float)x;
    winY = (float)window[3] - (float)y;
    winZ = 0;
    
    //get the world coordinates from the screen coordinates
    gluUnProject( winX, winY, winZ, modelview, projection, window, &worldX, &worldY, &worldZ);
    
    printf("x: %f, y: %f, z: %f\n", worldX, worldY, 0.0);
    
    
    
    // Red ball
    if( (color[0] > color[1]) && (color[0] > color[2]))
    {
        /*
        Vec4 old = *vec4create(curr_sphere_centers[0].x,
                              curr_sphere_centers[0].y,
                              0.0,
                              1.0,
                              &old);
        
        Vec4 new = *vec4create(x, y, 0.0, 1.0, &new);
        
        Vec4 direction = *vec4subtraction(&new, &old, &direction);
        Vec4 normalizedDirection = *scalarMultVector((1.0/vecMagnitude(&direction)), &direction, &normalizedDirection);
        
        direction = *vec4addition(&normalizedDirection, &old, &direction);
    
        Mat4 translation = *translate(old.x + direction.x, old.y + direction.y, 0.0, &translation);
        Mat4 temp = *matMultiplication(&translation, &transformation_matricies[0], &temp);
        transformation_matricies[0] = temp;
      
        curr_sphere_centers[0].x = old.x + direction.x;
        curr_sphere_centers[0].y = old.y + direction.y;
        */
        
        
        
        
    }
    
    
    glutPostRedisplay();
}













void idle(void)
{
    if(enableIdle)
    {
        if(ball_up[3] && ball_up[4])
        {
            if(velocity >= 0)
            {
                currTime = (float) clock() / CLOCKS_PER_SEC;
                float deltaTime = (currTime - oldTime)*30;
                oldTime = currTime;
            
                velocity += deltaTime * GRAVITY;
            
                sphere_degrees[3] -= velocity;
                sphere_degrees[4] -= velocity;
            
                if(sphere_degrees[3] < 270)
                {
                    sphere_degrees[3] = 270;
                    sphere_degrees[4] = 270;
                    sphere_degrees[0] = 270;
                    sphere_degrees[0] = 270;
                    ball_up[3] = 0;
                    ball_up[4] = 0;
                    ball_up[0] = 1;
                    ball_up[1] = 1;
                }
            
                float newX = cosf(DegreesToRadians*sphere_degrees[3]) + sphere_offsets[3];
                float newY = sinf(DegreesToRadians*sphere_degrees[3]);
            
                Mat4 translation = *translate(newX, newY, 0.0, &translation);
                Mat4 scale = *scaleMatrix(.25, &scale);
                Mat4 temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[3] = temp;
            
                vecArrayAdd(curr_sphere_centers, 3, newX, newY, 0.0, 1.0);
            
                newX = cosf(DegreesToRadians*sphere_degrees[4]) + sphere_offsets[4];
                newY = sinf(DegreesToRadians*sphere_degrees[4]);
            
                translation = *translate(newX, newY, 0.0, &translation);
                scale = *scaleMatrix(.25, &scale);
                temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[4] = temp;
            
                // Update current sphere center
                vecArrayAdd(curr_sphere_centers, 4, newX, newY, 0.0, 1.0);
            }
            else
            {
                currTime = (float) clock() / CLOCKS_PER_SEC;
                float deltaTime = (currTime - oldTime) * 30;
                oldTime = currTime;
                
                velocity += deltaTime * GRAVITY;
                
                sphere_degrees[3] -= velocity;
                sphere_degrees[4] -= velocity;
                
                float newX = cosf(DegreesToRadians*sphere_degrees[3]) + sphere_offsets[3];
                float newY = sinf(DegreesToRadians*sphere_degrees[3]);
                
                Mat4 translation = *translate(newX, newY, 0.0, &translation);
                Mat4 scale = *scaleMatrix(.25, &scale);
                Mat4 temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[3] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 3, newX, newY, 0.0, 1.0);
                
                newX = cosf(DegreesToRadians*sphere_degrees[4]) + sphere_offsets[4];
                newY = sinf(DegreesToRadians*sphere_degrees[4]);
                
                translation = *translate(newX, newY, 0.0, &translation);
                scale = *scaleMatrix(.25, &scale);
                temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[4] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 4, newX, newY, 0.0, 1.0);
            }
        }
        else if(ball_up[4])
        {
            if(velocity >= 0)
            {
                //currTime = glutGet(GLUT_ELAPSED_TIME);
                currTime = (float) clock() / CLOCKS_PER_SEC;
                float deltaTime = (currTime - oldTime)*30;///1000.0;
                oldTime = currTime;
            
                
                velocity += deltaTime * GRAVITY;
              
                sphere_degrees[4] -= velocity;
                
                if(sphere_degrees[4] < 270)
                {
                    sphere_degrees[4] = 270;
                    sphere_degrees[0] = 270;
                    ball_up[4] = 0;
                    ball_up[0] = 1;
                }
              
                  float newX = cosf(DegreesToRadians*sphere_degrees[4]) + sphere_offsets[4];
                  float newY = sinf(DegreesToRadians*sphere_degrees[4]);
                
                  Mat4 translation = *translate(newX, newY, 0.0, &translation);
                  Mat4 scale = *scaleMatrix(.25, &scale);
                  Mat4 temp = *matMultiplication(&translation, &scale, &temp);
                  sphere_transformation_matrices[4] = temp;
                
                  // Update current sphere centers
                  vecArrayAdd(curr_sphere_centers, 4, newX, newY, 0.0, 1.0);
            }
            else // velocity less than 0
            {
                currTime = (float) clock() / CLOCKS_PER_SEC;
                float deltaTime = (currTime - oldTime) * 30;
                oldTime = currTime;
                

                velocity += deltaTime * GRAVITY;
                
                sphere_degrees[4] -= velocity;
                
                float newX = cosf(DegreesToRadians*sphere_degrees[4]) + sphere_offsets[4];
                float newY = sinf(DegreesToRadians*sphere_degrees[4]);
                
                Mat4 translation = *translate(newX, newY, 0.0, &translation);
                Mat4 scale = *scaleMatrix(.25, &scale);
                Mat4 temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[4] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 4, newX, newY, 0.0, 1.0);
            }
        }
        else if(ball_up[1] && ball_up[0])
        {
            if(velocity > 0)
            {
                currTime = (float) clock() / CLOCKS_PER_SEC;
                float deltaTime = (currTime - oldTime) * 30;
                oldTime = currTime;
                
                
                velocity -= deltaTime * GRAVITY;
                
                sphere_degrees[1] -= velocity;
                sphere_degrees[0] -= velocity;
                
                float newX = cosf(DegreesToRadians*sphere_degrees[1]) + sphere_offsets[1];
                float newY = sinf(DegreesToRadians*sphere_degrees[1]);
                
                Mat4 translation = *translate(newX, newY, 0.0, &translation);
                Mat4 scale = *scaleMatrix(.25, &scale);
                Mat4 temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[1] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 1, newX, newY, 0.0, 1.0);
                
                newX = cosf(DegreesToRadians*sphere_degrees[0]) + sphere_offsets[0];
                newY = sinf(DegreesToRadians*sphere_degrees[0]);
                
                translation = *translate(newX, newY, 0.0, &translation);
                scale = *scaleMatrix(.25, &scale);
                temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[0] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 0, newX, newY, 0.0, 1.0);
                
            }
            else // velocity is zero or less
            {
                currTime = (float) clock() / CLOCKS_PER_SEC;
                float deltaTime = (currTime - oldTime)*30;
                oldTime = currTime;
                
                velocity -= deltaTime * GRAVITY;
                
                sphere_degrees[1] -= velocity;
                sphere_degrees[0] -= velocity;
                
                if(sphere_degrees[1] > 270)
                {
                    sphere_degrees[1] = 270;
                    sphere_degrees[0] = 270;
                    sphere_degrees[3] = 270;
                    sphere_degrees[4] = 270;
                    ball_up[3] = 1;
                    ball_up[4] = 1;
                    ball_up[0] = 0;
                    ball_up[1] = 0;
                    
                }
                
                
                float newX = cosf(DegreesToRadians*sphere_degrees[1]) + sphere_offsets[1];
                float newY = sinf(DegreesToRadians*sphere_degrees[1]);
                
                Mat4 translation = *translate(newX, newY, 0.0, &translation);
                Mat4 scale = *scaleMatrix(.25, &scale);
                Mat4 temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[1] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 1, newX, newY, 0.0, 1.0);
                
                newX = cosf(DegreesToRadians*sphere_degrees[0]) + sphere_offsets[0];
                newY = sinf(DegreesToRadians*sphere_degrees[0]);
                
                translation = *translate(newX, newY, 0.0, &translation);
                scale = *scaleMatrix(.25, &scale);
                temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[0] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 0, newX, newY, 0.0, 1.0);
                
            }
        }
        else if(ball_up[0])
        {
            if(velocity > 0)
            {
                currTime = (float) clock() / CLOCKS_PER_SEC;
                float deltaTime = (currTime - oldTime) * 30;
                oldTime = currTime;
                
    
                velocity -= deltaTime * GRAVITY;
                
                sphere_degrees[0] -= velocity;
                
                float newX = cosf(DegreesToRadians*sphere_degrees[0]) + sphere_offsets[0];
                float newY = sinf(DegreesToRadians*sphere_degrees[0]);
                
                Mat4 translation = *translate(newX, newY, 0.0, &translation);
                Mat4 scale = *scaleMatrix(.25, &scale);
                Mat4 temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[0] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 0, newX, newY, 0.0, 1.0);
                
            }
            else // velocity is zero or less
            {
                currTime = (float) clock() / CLOCKS_PER_SEC;
                float deltaTime = (currTime - oldTime)*30;
                oldTime = currTime;
                
                velocity -= deltaTime * GRAVITY;
                
                sphere_degrees[0] -= velocity;
                
                if(sphere_degrees[0] > 270)
                {
                    sphere_degrees[0] = 270;
                    sphere_degrees[4] = 270;
                    ball_up[4] = 1;
                    ball_up[0] = 0;
                }
                
                
                float newX = cosf(DegreesToRadians*sphere_degrees[0]) + sphere_offsets[0];
                float newY = sinf(DegreesToRadians*sphere_degrees[0]);
                
                Mat4 translation = *translate(newX, newY, 0.0, &translation);
                Mat4 scale = *scaleMatrix(.25, &scale);
                Mat4 temp = *matMultiplication(&translation, &scale, &temp);
                sphere_transformation_matrices[0] = temp;
                
                // Update current sphere centers
                vecArrayAdd(curr_sphere_centers, 0, newX, newY, 0.0, 1.0);
                
            }
        }
    
    }
    
 
    glutPostRedisplay();
}



int main(int argc, char **argv)
{
    initGround();
    initPoles();
    initStrings();
    
    // OpenGL initializaiton code
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowSize*2, windowSize*2);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 4 - Newton's Cradle");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}

