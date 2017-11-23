#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
 #include <common/shader.hpp>
 
#define M_PI       3.14159265358979323846
 
float verticesAxis[] = {-20.0f, 0.0f, 0.0f, 1.0f,
            20.0f, 0.0f, 0.0f, 1.0f,
 
            0.0f, -20.0f, 0.0f, 1.0f,
            0.0f,  20.0f, 0.0f, 1.0f,
 
            0.0f, 0.0f, -20.0f, 1.0f,
            0.0f, 0.0f,  20.0f, 1.0f};
 
float colorAxis[] = {   0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f};

float vertices1[] = {   
        -1.0f,-4.0f,-1.0f, 1.0f,
        -1.0f,-4.0f, 1.0f, 1.0f,
        -1.0f, 4.0f, 1.0f, 1.0f,

         1.0f, 4.0f,-1.0f, 1.0f,
        -1.0f,-4.0f,-1.0f, 1.0f,
        -1.0f, 4.0f,-1.0f, 1.0f,


         1.0f,-4.0f, 1.0f, 1.0f,
        -1.0f,-4.0f,-1.0f, 1.0f,
         1.0f,-4.0f,-1.0f, 1.0f,
         1.0f, 4.0f,-1.0f, 1.0f,
         1.0f,-4.0f,-1.0f, 1.0f,
        -1.0f,-4.0f,-1.0f, 1.0f,


        -1.0f,-4.0f,-1.0f, 1.0f,
        -1.0f, 4.0f, 1.0f, 1.0f,
        -1.0f, 4.0f,-1.0f, 1.0f,
         1.0f,-4.0f, 1.0f, 1.0f,
        -1.0f,-4.0f, 1.0f, 1.0f,
        -1.0f,-4.0f,-1.0f, 1.0f,


        -1.0f, 4.0f, 1.0f, 1.0f,
        -1.0f,-4.0f, 1.0f, 1.0f,
         1.0f,-4.0f, 1.0f, 1.0f,
         1.0f, 4.0f, 1.0f, 1.0f,
         1.0f,-4.0f,-1.0f, 1.0f,
         1.0f, 4.0f,-1.0f, 1.0f,


         1.0f,-4.0f,-1.0f, 1.0f,
         1.0f, 4.0f, 1.0f, 1.0f,
         1.0f,-4.0f, 1.0f, 1.0f,
         1.0f, 4.0f, 1.0f, 1.0f,
         1.0f, 4.0f,-1.0f, 1.0f,
        -1.0f, 4.0f,-1.0f, 1.0f,


         1.0f, 4.0f, 1.0f, 1.0f,
        -1.0f, 4.0f,-1.0f, 1.0f,
        -1.0f, 4.0f, 1.0f, 1.0f,
         1.0f, 4.0f, 1.0f, 1.0f,
        -1.0f, 4.0f, 1.0f, 1.0f,
         1.0f,-4.0f, 1.0f, 1.0f,








         /*******************/
         1.0f,-1.0f,-1.0f, 1.0f,
         1.0f,-1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 1.0f,
         3.0f, 1.0f,-1.0f, 1.0f,
        1.0f,-1.0f,-1.0f, 1.0f,
        1.0f, 1.0f,-1.0f, 1.0f,

         3.0f,-1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f, 1.0f,
         3.0f,-1.0f,-1.0f, 1.0f,
         3.0f, 1.0f,-1.0f, 1.0f,
         3.0f,-1.0f,-1.0f, 1.0f,
        1.0f,-1.0f,-1.0f, 1.0f,

        1.0f,-1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f, 1.0f,
         3.0f,-1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f, 1.0f,
         3.0f,-1.0f, 1.0f, 1.0f,
         3.0f, 1.0f, 1.0f, 1.0f,
         3.0f,-1.0f,-1.0f, 1.0f,
         3.0f, 1.0f,-1.0f, 1.0f,

         3.0f,-1.0f,-1.0f, 1.0f,
         3.0f, 1.0f, 1.0f, 1.0f,
         3.0f,-1.0f, 1.0f, 1.0f,
         3.0f, 1.0f, 1.0f, 1.0f,
         3.0f, 1.0f,-1.0f, 1.0f,
        1.0f, 1.0f,-1.0f, 1.0f,

         3.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
         3.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
         3.0f,-1.0f, 1.0f, 1.0f,



         /******************////
         -1.0f,4.0f,-1.0f, 1.0f,
        -1.0f,4.0f, 1.0f, 1.0f,
        -1.0f, 6.0f, 1.0f, 1.0f,
         5.0f, 6.0f,-1.0f, 1.0f,
        -1.0f,4.0f,-1.0f, 1.0f,
        -1.0f, 6.0f,-1.0f, 1.0f,

         5.0f,4.0f, 1.0f, 1.0f,
        -1.0f,4.0f,-1.0f, 1.0f,
         5.0f,4.0f,-1.0f, 1.0f,
         5.0f, 6.0f,-1.0f, 1.0f,
         5.0f,4.0f,-1.0f, 1.0f,
        -1.0f,4.0f,-1.0f, 1.0f,

        -1.0f,4.0f,-1.0f, 1.0f,
        -1.0f, 6.0f, 1.0f, 1.0f,
        -1.0f, 6.0f,-1.0f, 1.0f,
         5.0f,4.0f, 1.0f, 1.0f,
        -1.0f,4.0f, 1.0f, 1.0f,
        -1.0f,4.0f,-1.0f, 1.0f,

        -1.0f, 6.0f, 1.0f, 1.0f,
        -1.0f,4.0f, 1.0f, 1.0f,
         5.0f,4.0f, 1.0f, 1.0f,
         5.0f, 6.0f, 1.0f, 1.0f,
         5.0f,4.0f,-1.0f, 1.0f,
         5.0f, 6.0f,-1.0f, 1.0f,

         5.0f,4.0f,-1.0f, 1.0f,
         5.0f, 6.0f, 1.0f, 1.0f,
         5.0f,4.0f, 1.0f, 1.0f,
         5.0f, 6.0f, 1.0f, 1.0f,
         5.0f, 6.0f,-1.0f, 1.0f,
        -1.0f, 6.0f,-1.0f, 1.0f,

         5.0f, 6.0f, 1.0f, 1.0f,
        -1.0f, 6.0f,-1.0f, 1.0f,
        -1.0f, 6.0f, 1.0f, 1.0f,
         5.0f, 6.0f, 1.0f, 1.0f,
        -1.0f, 6.0f, 1.0f, 1.0f,
         5.0f,4.0f, 1.0f, 1.0f,

        };
 
float colors1[] = { 
        0.583f,  0.771f,  0.014f, 1.0f,
        0.609f,  0.115f,  0.436f, 1.0f,
        0.327f,  0.483f,  0.844f, 1.0f,
        0.822f,  0.569f,  0.201f, 1.0f,
        0.435f,  0.602f,  0.223f, 1.0f,
        0.310f,  0.747f,  0.185f, 1.0f,
        0.597f,  0.770f,  0.761f, 1.0f,
        0.559f,  0.436f,  0.730f, 1.0f,
        0.359f,  0.583f,  0.152f, 1.0f,
        0.483f,  0.596f,  0.789f, 1.0f,
        0.559f,  0.861f,  0.639f, 1.0f,
        0.195f,  0.548f,  0.859f, 1.0f,
        0.014f,  0.184f,  0.576f, 1.0f,
        0.771f,  0.328f,  0.970f, 1.0f,
        0.406f,  0.615f,  0.116f, 1.0f,
        0.676f,  0.977f,  0.133f, 1.0f,
        0.971f,  0.572f,  0.833f, 1.0f,
        0.140f,  0.616f,  0.489f, 1.0f,
        0.997f,  0.513f,  0.064f, 1.0f,
        0.945f,  0.719f,  0.592f, 1.0f,
        0.543f,  0.021f,  0.978f, 1.0f,
        0.279f,  0.317f,  0.505f, 1.0f,
        0.167f,  0.620f,  0.077f, 1.0f,
        0.347f,  0.857f,  0.137f, 1.0f,
        0.055f,  0.953f,  0.042f, 1.0f,
        0.714f,  0.505f,  0.345f, 1.0f,
        0.783f,  0.290f,  0.734f, 1.0f,
        0.722f,  0.645f,  0.174f, 1.0f,
        0.302f,  0.455f,  0.848f, 1.0f,
        0.225f,  0.587f,  0.040f, 1.0f,
        0.517f,  0.713f,  0.338f, 1.0f,
        0.053f,  0.959f,  0.120f, 1.0f,
        0.393f,  0.621f,  0.362f, 1.0f,
        0.673f,  0.211f,  0.457f, 1.0f,
        0.820f,  0.883f,  0.371f, 1.0f,
        0.982f,  0.099f,  0.879f, 1.0f,
        0.583f,  0.771f,  0.014f, 1.0f,
        0.609f,  0.115f,  0.436f, 1.0f,
        0.327f,  0.483f,  0.844f, 1.0f,
        0.822f,  0.569f,  0.201f, 1.0f,
        0.435f,  0.602f,  0.223f, 1.0f,
        0.310f,  0.747f,  0.185f, 1.0f,
        0.597f,  0.770f,  0.761f, 1.0f,
        0.559f,  0.436f,  0.730f, 1.0f,
        0.359f,  0.583f,  0.152f, 1.0f,
        0.483f,  0.596f,  0.789f, 1.0f,
        0.559f,  0.861f,  0.639f, 1.0f,
        0.195f,  0.548f,  0.859f, 1.0f,
        0.014f,  0.184f,  0.576f, 1.0f,
        0.771f,  0.328f,  0.970f, 1.0f,
        0.406f,  0.615f,  0.116f, 1.0f,
        0.676f,  0.977f,  0.133f, 1.0f,
        0.971f,  0.572f,  0.833f, 1.0f,
        0.140f,  0.616f,  0.489f, 1.0f,
        0.997f,  0.513f,  0.064f, 1.0f,
        0.945f,  0.719f,  0.592f, 1.0f,
        0.543f,  0.021f,  0.978f, 1.0f,
        0.279f,  0.317f,  0.505f, 1.0f,
        0.167f,  0.620f,  0.077f, 1.0f,
        0.347f,  0.857f,  0.137f, 1.0f,
        0.055f,  0.953f,  0.042f, 1.0f,
        0.714f,  0.505f,  0.345f, 1.0f,
        0.783f,  0.290f,  0.734f, 1.0f,
        0.722f,  0.645f,  0.174f, 1.0f,
        0.302f,  0.455f,  0.848f, 1.0f,
        0.225f,  0.587f,  0.040f, 1.0f,
        0.517f,  0.713f,  0.338f, 1.0f,
        0.053f,  0.959f,  0.120f, 1.0f,
        0.393f,  0.621f,  0.362f, 1.0f,
        0.673f,  0.211f,  0.457f, 1.0f,
        0.820f,  0.883f,  0.371f, 1.0f,
        0.982f,  0.099f,  0.879f, 1.0f,
        0.583f,  0.771f,  0.014f, 1.0f,
        0.609f,  0.115f,  0.436f, 1.0f,
        0.327f,  0.483f,  0.844f, 1.0f,
        0.822f,  0.569f,  0.201f, 1.0f,
        0.435f,  0.602f,  0.223f, 1.0f,
        0.310f,  0.747f,  0.185f, 1.0f,
        0.597f,  0.770f,  0.761f, 1.0f,
        0.559f,  0.436f,  0.730f, 1.0f,
        0.359f,  0.583f,  0.152f, 1.0f,
        0.483f,  0.596f,  0.789f, 1.0f,
        0.559f,  0.861f,  0.639f, 1.0f,
        0.195f,  0.548f,  0.859f, 1.0f,
        0.014f,  0.184f,  0.576f, 1.0f,
        0.771f,  0.328f,  0.970f, 1.0f,
        0.406f,  0.615f,  0.116f, 1.0f,
        0.676f,  0.977f,  0.133f, 1.0f,
        0.971f,  0.572f,  0.833f, 1.0f,
        0.140f,  0.616f,  0.489f, 1.0f,
        0.997f,  0.513f,  0.064f, 1.0f,
        0.945f,  0.719f,  0.592f, 1.0f,
        0.543f,  0.021f,  0.978f, 1.0f,
        0.279f,  0.317f,  0.505f, 1.0f,
        0.167f,  0.620f,  0.077f, 1.0f,
        0.347f,  0.857f,  0.137f, 1.0f,
        0.055f,  0.953f,  0.042f, 1.0f,
        0.714f,  0.505f,  0.345f, 1.0f,
        0.783f,  0.290f,  0.734f, 1.0f,
        0.722f,  0.645f,  0.174f, 1.0f,
        0.302f,  0.455f,  0.848f, 1.0f,
        0.225f,  0.587f,  0.040f, 1.0f,
        0.517f,  0.713f,  0.338f, 1.0f,
        0.053f,  0.959f,  0.120f, 1.0f,
        0.393f,  0.621f,  0.362f, 1.0f,
        0.673f,  0.211f,  0.457f, 1.0f,
        0.820f,  0.883f,  0.371f, 1.0f,
        0.982f,  0.099f,  0.879f, 1.0f
        };
 
GLuint p,v,f;
GLuint vertexLoc, colorLoc;
GLuint projMatrixLoc, viewMatrixLoc;
GLuint vao[3];
 

float projMatrix[16];
float viewMatrix[16];
 
void crossProduct( float *a, float *b, float *res) {
 
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}
 

void normalize(float *a) {
 
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
 
    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}
 
void setIdentityMatrix( float *mat, int size) {
 
    // fill matrix with 0s
    for (int i = 0; i < size * size; ++i)
            mat[i] = 0.0f;
 
    // fill diagonal with 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;
}

void multMatrix(float *a, float *b) {
 
    float res[16];
 
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));
 
}

void setTranslationMatrix(float *mat, float x, float y, float z) {
 
    setIdentityMatrix(mat,4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}

 
void buildProjectionMatrix(float fov, float ratio, float nearP, float farP) {
 
    float f = 1.0f / tan (fov * (M_PI / 360.0));
 
    setIdentityMatrix(projMatrix,4);
 
    projMatrix[0] = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;
}
 
 
void setCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ) {
 
    float dir[3], right[3], up[3];
 
    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;
 
    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    normalize(dir);
 
    crossProduct(dir,up,right);
    normalize(right);
 
    crossProduct(right,dir,up);
    normalize(up);
 
    float aux[16];
 
    viewMatrix[0]  = right[0];
    viewMatrix[4]  = right[1];
    viewMatrix[8]  = right[2];
    viewMatrix[12] = 0.0f;
 
    viewMatrix[1]  = up[0];
    viewMatrix[5]  = up[1];
    viewMatrix[9]  = up[2];
    viewMatrix[13] = 0.0f;
 
    viewMatrix[2]  = -dir[0];
    viewMatrix[6]  = -dir[1];
    viewMatrix[10] = -dir[2];
    viewMatrix[14] =  0.0f;
 
    viewMatrix[3]  = 0.0f;
    viewMatrix[7]  = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;
 
    setTranslationMatrix(aux, -posX, -posY, -posZ);
 
    multMatrix(viewMatrix, aux);
}
 
// ----------------------------------------------------
 
void changeSize(int w, int h) {
 
    float ratio;
    if(h == 0)
        h = 1;
    glViewport(0, 0, w, h);
 
    ratio = (1.0f * w) / h;
    buildProjectionMatrix(53.13f, ratio, 1.0f, 30.0f);
}
 
void setupBuffers() {
 
    GLuint buffers[2];
 
    glGenVertexArrays(3, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
    glBindVertexArray(vao[2]);
    glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAxis), verticesAxis, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorAxis), colorAxis, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
 
}
 
void setUniforms() {
 
    glUniformMatrix4fv(projMatrixLoc,  1, false, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, false, viewMatrix);
}
 
void renderScene(void) {
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    setCamera(10,2,10,0,2,-5);
 
    glUseProgram(p);
    setUniforms();
 
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 108);//numero de triangulos

    glBindVertexArray(vao[2]);
    glDrawArrays(GL_LINES, 0, 6);
 
    glutSwapBuffers();
}
 
void processNormalKeys(unsigned char key, int x, int y) {
 
    if (key == 27) {
        glDeleteVertexArrays(3,vao);
        glDeleteProgram(p);
        glDeleteShader(v);
        glDeleteShader(f);
        exit(0);
    }
}
 
GLuint setupShaders() {

    p= LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
 
    vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "color"); 
 
    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
 
    return(p);
}
 
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("Practica 1 - Ejercicio 1.2");
 
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processNormalKeys);
 
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,1.0);
 
    p = setupShaders(); 
    setupBuffers(); 
 
    glutMainLoop();
 
    return(0); 
}