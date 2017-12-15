/************************************************************
 * Handout: rotate-cube-new.cpp (A Sample Code for Shader-Based OpenGL ---
                                 for OpenGL version 3.1 and later)
 * Originally from Ed Angel's textbook "Interactive Computer Graphics" 6th Ed
              sample code "example3.cpp" of Chapter 4.
 * Moodified by Yi-Jen Chiang to include the use of a general rotation function
   Rotate(angle, x, y, z), where the vector (x, y, z) can have length != 1.0,
   and also to include the use of the function NormalMatrix(mv) to return the
   normal matrix (mat3) of a given model-view matrix mv (mat4).

   (The functions Rotate() and NormalMatrix() are added to the file "mat-yjc-new.h"
   by Yi-Jen Chiang, where a new and correct transpose function "transpose1()" and
   other related functions such as inverse(m) for the inverse of 3x3 matrix m are
   also added; see the file "mat-yjc-new.h".)

 * Extensively modified by Yi-Jen Chiang for the program structure and user
   interactions. See the function keyboard() for the keyboard actions.
   Also extensively re-structured by Yi-Jen Chiang to create and use the new
   function drawObj() so that it is easier to draw multiple objects. Now a floor
   and a rotating cube are drawn.

** Perspective view of a color cube using LookAt() and Perspective()

** Colors are assigned to each vertex and then the rasterizer interpolates
   those colors across the triangles.
**************************************************************/
#include "Angel-yjc.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

typedef Angel::vec3  color3;
typedef Angel::vec3  point3;

GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);

GLuint program;       /* shader program object id */
GLuint program2; 
GLuint program3; 
GLuint cube_buffer;   /* vertex buffer object id for cube */
GLuint floor_buffer;  /* vertex buffer object id for floor */
GLuint axis_buffer;

// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 3.0;

GLfloat anglex = 0.0; // rotation angle in degrees
GLfloat anglez = 0.0; // rotation angle in degrees
GLfloat delta_x = 3.0; 
GLfloat delta_z = 5.0; 
int tmp_roll=1;
int rollingFlag=1;
vec4 init_eye(7.0, 3.0, -10.0, 1.0); // initial viewer position
//vec4 init_eye(2.0, 3.0, 0.0, 1.0); // initial viewer position


vec4 eye = init_eye;               // current viewer position
vec3 camera={7.0, 1.0, -10.0};

int animationFlag = 1; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'

int cubeFlag = 1;   // 1: solid cube; 0: wireframe cube. Toggled by key 'c' or 'C'
int floorFlag = 1;  // 1: solid floor; 0: wireframe floor. Toggled by key 'f' or 'F'

const int cube_NumVertices = 128*3; //(6 faces)*(2 triangles/face)*(3 vertices/triangle)
#if 0
point3 cube_points[cube_NumVertices]; // positions for all vertices
color3 cube_colors[cube_NumVertices]; // colors for all vertices
#endif
#if 1
point3 cube_points[900]; 
color3 cube_colors[900];
#endif

const int floor_NumVertices = 6; //(1 face)*(2 triangles/face)*(3 vertices/triangle)
point3 floor_points[floor_NumVertices]; // positions for all vertices
color3 floor_colors[floor_NumVertices]; // colors for all vertices


vec3 lightPos={0.0, 8.0, 0.0};
int useSmooth=0;

int useFOG=1;



//Texture declarations
#define ImageWidth  64
#define ImageHeight 64
GLubyte Image[ImageHeight][ImageWidth][4];

#define stripeImageWidth 32
GLubyte stripeImage[4*stripeImageWidth];

static GLuint texName;
int texture_app_flag = 0;  // 0: no texture application: obj color
                           // 1: texutre color
                           // 2: (obj color) * (texture color)

vec2 quad_texCoord[6] = {
  vec2(0.0, 0.0),  // for a
  vec2(0.0, 1.25),  // for b
  vec2(1.5, 1.25),  // for c

  vec2(1.5, 1.25),  // for c
  vec2(1.5, 0.0),  // for d
  vec2(0.0, 0.0),  // for a 
};

void image_set_up(void)
{
 int i, j, c; 
 
 /* --- Generate checkerboard image to the image array ---*/
  for (i = 0; i < ImageHeight; i++)
    for (j = 0; j < ImageWidth; j++)
      {
       c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0));

       if (c == 1) /* white */
	{
         c = 255;  
	 Image[i][j][0] = (GLubyte) c;
         Image[i][j][1] = (GLubyte) c;
         Image[i][j][2] = (GLubyte) c;
	}
       else  /* green */
	{
         Image[i][j][0] = (GLubyte) 0;
         Image[i][j][1] = (GLubyte) 150;
         Image[i][j][2] = (GLubyte) 0;
	}

       Image[i][j][3] = (GLubyte) 255;
      }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

/*--- Generate 1D stripe image to array stripeImage[] ---*/
  for (j = 0; j < stripeImageWidth; j++) {
     /* When j <= 4, the color is (255, 0, 0),   i.e., red stripe/line.
        When j > 4,  the color is (255, 255, 0), i.e., yellow remaining texture
      */
    stripeImage[4*j] = (GLubyte)    255;
    stripeImage[4*j+1] = (GLubyte) ((j>4) ? 255 : 0);
    stripeImage[4*j+2] = (GLubyte) 0; 
    stripeImage[4*j+3] = (GLubyte) 255;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
/*----------- End 1D stripe image ----------------*/

/*--- texture mapping set-up is to be done in 
      init() (set up texture objects),
      display() (activate the texture object to be used, etc.)
      and in shaders.
 ---*/

} /* end function */





// Vertices of a unit cube centered at origin, sides aligned with axes
point3 vertices[8] = {
    point3( -0.5, -0.5,  0.5),
    point3( -0.5,  0.5,  0.5),
    point3(  0.5,  0.5,  0.5),
    point3(  0.5, -0.5,  0.5),
    point3( -0.5, -0.5, -0.5),
    point3( -0.5,  0.5, -0.5),
    point3(  0.5,  0.5, -0.5),
    point3(  0.5, -0.5, -0.5)
};
// RGBA colors
color3 vertex_colors[8] = {
    color3( 0.0, 0.0, 0.0),  // black
    color3( 1.0, 0.0, 0.0),  // red
    color3( 1.0, 1.0, 0.0),  // yellow
    color3( 0.0, 1.0, 0.0),  // green
    color3( 0.0, 0.0, 1.0),  // blue
    color3( 1.0, 0.0, 1.0),  // magenta
    color3( 1.0, 1.0, 1.0),  // white
    color3( 0.0, 1.0, 1.0)   // cyan
};
//----------------------------------------------------------------------------
int Index = 0; // YJC: This must be a global variable since quad() is called
               //      multiple times and Index should then go up to 36 for
               //      the 36 vertices and colors

// quad(): generate two triangles for each face and assign colors to the vertices
void quad( int a, int b, int c, int d )
{
    cube_colors[Index] = vertex_colors[a]; cube_points[Index] = vertices[a]; Index++;

    cube_colors[Index] = vertex_colors[b]; cube_points[Index] = vertices[b]; Index++;
    cube_colors[Index] = vertex_colors[c]; cube_points[Index] = vertices[c]; Index++;

    cube_colors[Index] = vertex_colors[c]; cube_points[Index] = vertices[c]; Index++;
    cube_colors[Index] = vertex_colors[d]; cube_points[Index] = vertices[d]; Index++;
    cube_colors[Index] = vertex_colors[a]; cube_points[Index] = vertices[a]; Index++;
}
//----------------------------------------------------------------------------
// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}
//-------------------------------
point3 verticesAxis[6] = {
            point3(0.0, 0.0, 0.0),
            point3(10.0, 0.0, 0.0),
 
            point3(0.0, 0.0, 0.0),
            point3(0.0,  10.0, 0.0),
 
            point3(0.0, 0.0, 0.0),
            point3(0.0, 0.0,  10.0)};
 
point3 colorAxis[6] = { 
            color3( 1.0, 0.0, 0.0),
            color3( 1.0, 0.0, 0.0),

            color3( 1.0, 0.0, 1.0),
            color3( 1.0, 0.0, 1.0),

            color3( 0.0, 0.0, 1.0),
            color3( 0.0, 0.0, 1.0)
            };





point3 vertices_floor[4] = {
    point3( 5.0,  0.0, -4.0 ),
    point3( -5.0, 0.0, -4.0 ),
    point3( 5.0,  0.0, 8.0 ),
    point3( -5.0, 0.0, 8.0 )
};


void floor1()
{
    floor_colors[0] = color3( 0.0, 1.0, 0.0); floor_points[0] = vertices_floor[2];
    floor_colors[1] = color3( 0.0, 1.0, 0.0); floor_points[1] = vertices_floor[0];
    floor_colors[2] = color3( 0.0, 1.0, 0.0); floor_points[2] = vertices_floor[1];

    floor_colors[3] = color3( 0.0, 1.0, 0.0); floor_points[3] = vertices_floor[2];
    floor_colors[4] = color3( 0.0, 1.0, 0.0); floor_points[4] = vertices_floor[3];
    floor_colors[5] = color3( 0.0, 1.0, 0.0); floor_points[5] = vertices_floor[0];
}
point3 verticesa[8] = {
    point3( -5.0, 0.0,  8.0),//*
    point3( -0.5, 0.0,  0.5),
    point3(  0.5, 0.0,  0.5),
    point3(  5.0, 0.0,  8.0),//*+
    point3( -5.0, 0.0, -4.0),//*+
    point3( -0.5, 0.0, -0.5),
    point3(  0.5, 0.0, -0.5),
    point3(  5.0, 0.0, -4.0)//+
};

point3 floor_normals[6];

void floor2()
{
    floor_colors[0] = color3( 0.0, 1.0, 0.0); floor_points[0] = verticesa[3];
    floor_colors[1] = color3( 0.0, 1.0, 0.0); floor_points[1] = verticesa[0];
    floor_colors[2] = color3( 0.0, 1.0, 0.0); floor_points[2] = verticesa[4];

    floor_colors[3] = color3( 0.0, 1.0, 0.0); floor_points[3] = verticesa[4];
    floor_colors[4] = color3( 0.0, 1.0, 0.0); floor_points[4] = verticesa[7];
    floor_colors[5] = color3( 0.0, 1.0, 0.0); floor_points[5] = verticesa[3];
    floor_normals[0]=point3(0.0 , 1.0 , 0.0);
    floor_normals[1]=point3(0.0 , 1.0 , 0.0);
    floor_normals[2]=point3(0.0 , 1.0 , 0.0);
    floor_normals[3]=point3(0.0 , 1.0 , 0.0);
    floor_normals[4]=point3(0.0 , 1.0 , 0.0);
    floor_normals[5]=point3(0.0 , 1.0 , 0.0);
}

void floor()
{
    floor_colors[0] = vertex_colors[3]; floor_points[0] = vertices[3];
    floor_colors[1] = vertex_colors[0]; floor_points[1] = vertices[0];
    floor_colors[2] = vertex_colors[4]; floor_points[2] = vertices[4];

    floor_colors[3] = vertex_colors[4]; floor_points[3] = vertices[4];
    floor_colors[4] = vertex_colors[7]; floor_points[4] = vertices[7];
    floor_colors[5] = vertex_colors[3]; floor_points[5] = vertices[3];
}
point3 normales[900];

point3 compute_normals(point3 v1, point3 v2, point3 v3)
{

    point3 v23=v3-v2;
    point3 v21=v1-v2;
    //cout<<v23.x<<v23.y<<v23.z<<endl;
    //cout<<v21.x<<v21.y<<v21.z<<endl;
    return (point3)cross(v23,v21);

}



void read_sphere()
{
    std::ifstream file("sphere.128");
    GLfloat xt, yt, zt;
    int num_triangles, tmp;
    file>>num_triangles;
    point3 normal_tmp;
    //file>>tmp;
    int Ix=0;
    int Ix_n=0;
    while(!file.eof())
    {
      //file>>tmp;
      if((Ix%3)==0)
      {
        file>>tmp;
        if(Ix!=0)
        {
          normal_tmp=compute_normals(cube_points[Ix-3], cube_points[Ix-2], cube_points[Ix-1]);
          //normal_tmp=(point3)normalize(normal_tmp);
          //cout<<normal_tmp.x<<normal_tmp.y<<normal_tmp.z<<endl;
          for(int yy=0;yy<3;yy++)
          {
              normales[Ix_n]=normal_tmp;
              Ix_n++;
          }
          
                 
        }
      }

      file>>xt>>yt>>zt;
      cube_points[Ix]=point3(xt,yt,zt);
      //cube_points[Ix]=point3(xt,yt,zt);
      cube_colors[Ix]=color3( 1.0, 0.84, 0.0);
      Ix++;

    }
    //cube_NumVertices=num_triangles*3;


}


//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{
    read_sphere();
    //colorcube();

#if 0 //YJC: The following is not needed
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
#endif

 // Create and initialize a vertex buffer object for cube, to be used in display()
    glGenBuffers(1, &cube_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, cube_buffer);

#if 0
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points) + sizeof(cube_colors),
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_points), cube_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_points), sizeof(cube_colors),
                    cube_colors);
#endif
#if 1
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(point3)*cube_NumVertices + sizeof(color3)*cube_NumVertices + sizeof(point3) * cube_NumVertices,
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 
                    sizeof(point3) * cube_NumVertices, cube_points);
    glBufferSubData(GL_ARRAY_BUFFER, 
                    sizeof(point3) * cube_NumVertices, 
                    sizeof(color3) * cube_NumVertices,
                    cube_colors);
     glBufferSubData(GL_ARRAY_BUFFER, 
                    sizeof(point3) * cube_NumVertices+sizeof(color3) * cube_NumVertices, 
                    sizeof(point3) * cube_NumVertices,
                    normales);
#endif

    floor2();     
 // Create and initialize a vertex buffer object for floor, to be used in display()
    glGenBuffers(1, &floor_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, floor_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_points) +sizeof(quad_texCoord)+ sizeof(floor_colors)+sizeof(floor_normals),
     NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(floor_points), floor_points);

    glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points), sizeof(quad_texCoord),
                    quad_texCoord);

    glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points)+sizeof(quad_texCoord), sizeof(floor_colors),
                    floor_colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(floor_points)+sizeof(quad_texCoord)+sizeof(floor_colors), sizeof(floor_normals),
                    floor_normals);


//Axis buffer
    glGenBuffers(1, &axis_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, axis_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAxis) + sizeof(colorAxis),
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesAxis), verticesAxis);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticesAxis), sizeof(colorAxis),
                    colorAxis);





    image_set_up();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //Texture
     /*--- Create and Initialize a texture object ---*/
   glGenTextures(1, &texName);      // Generate texture obj name(s)

   glActiveTexture( GL_TEXTURE0 );  // Set the active texture unit to be 0 
   glBindTexture(GL_TEXTURE_2D, texName); // Bind the texture to this texture unit

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageWidth, ImageHeight, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, Image);






 // Load shaders and create a shader program (to be used in display())
    program = InitShader("vshader42.glsl", "fshader42.glsl");
    program2 = InitShader("vTexture.glsl", "fTexture.glsl");
    program3 = InitShader("vAxis.glsl", "fAxis.glsl");
    
    glEnable( GL_DEPTH_TEST );
    glClearColor(0.529, 0.807, 0.92, 0.0 ); 
    glLineWidth(2.0);





}
//----------------------------------------------------------------------------
// drawObj(buffer, num_vertices):
//   draw the object that is associated with the vertex buffer object "buffer"
//   and has "num_vertices" vertices.
//
void drawObj(GLuint buffer, int num_vertices)
{
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    
    GLuint lightPos_id = glGetUniformLocation(program, "lightPos" );
    glUniform3fv(lightPos_id, 1, &lightPos.x);

    GLuint viewPos_id = glGetUniformLocation(program, "viewPos" );
    glUniform3fv(viewPos_id, 1, &camera.x);


    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, //NULL);
        BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation(program, "vColor"); 
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 
        BUFFER_OFFSET(sizeof(point3) * num_vertices) ); 



    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, //NULL);
        BUFFER_OFFSET(sizeof(point3) * num_vertices*2) );



      // the offset is the (total) size of the previous vertex attribute array(s)

    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    //glDrawArrays(GL_LINES, 0, 6);

    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void drawObj_floor(GLuint buffer, int num_vertices)
{
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    
    GLuint lightPos_id = glGetUniformLocation(program2, "lightPos" );
    glUniform3fv(lightPos_id, 1, &lightPos.x);

    GLuint viewPos_id = glGetUniformLocation(program2, "viewPos" );
    glUniform3fv(viewPos_id, 1, &camera.x);


    GLuint vPosition = glGetAttribLocation(program2, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, //NULL);
        BUFFER_OFFSET(0) );

    GLuint vTexture = glGetAttribLocation(program2, "vTexCoord"); 
    glEnableVertexAttribArray(vTexture);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 
        BUFFER_OFFSET(sizeof(vec3) * num_vertices) ); 




    GLuint vColor = glGetAttribLocation(program2, "vColor"); 
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 
        BUFFER_OFFSET(sizeof(point3) * num_vertices + sizeof(vec2) * num_vertices ) ); 


    GLuint vNormal = glGetAttribLocation(program2, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, //NULL);
        BUFFER_OFFSET(sizeof(point3) * num_vertices*2 + sizeof( vec2 ) * num_vertices ) );





      // the offset is the (total) size of the previous vertex attribute array(s)

    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    //glDrawArrays(GL_LINES, 0, 6);

    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}



void draw_axis(GLuint buffer, int num_vertices)
{
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    GLuint vPosition = glGetAttribLocation(program3, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation(program3, "vColor"); 
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(point3) * num_vertices) ); 
      // the offset is the (total) size of the previous vertex attribute array(s)

    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    //glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glDrawArrays(GL_LINES, 0, num_vertices);

    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}



//----------------------------------------------------------------------------
void display( void )
{
  GLuint  model;  // model-view matrix uniform shader variable location
  GLuint  view;  // model-view matrix uniform shader variable location
  GLuint  projection;  // projection matrix uniform shader variable location

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   






    glUseProgram(program); // Use the shader program
    view = glGetUniformLocation(program, "view" );
    model = glGetUniformLocation(program, "model" );
    projection = glGetUniformLocation(program, "projection" );
    mat4  p = Perspective(45.0, 1.0, 1.0, 100.0);
    glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major
    vec4    at(0.0, 0.0, 0.0, 1.0);
    vec4    up(0.0, 1.0, 0.0, 0.0);


    mat4  view_ = LookAt(eye, at, up);
    mat4  model_ = mat4( 1.0f );
    model_ = Scale( 0.5, 0.5, 0.5 ) * model_;
    model_ = RotateX(anglex) * model_;
    model_ = RotateZ(anglez) * model_;
    //model_ = RotateY(angle) * model_;
    //model_ = RotateZ(angle) * model_;
    // model_ = rotate( w * dt, ax, ay, az );
    model_ = Translate(delta_x, 1.0, delta_z) * model_;
    //model_ = Translate(3.0, 1.0, -5.0) * model_;
    //model_ = Translate(-1.0, 1.0, -4.0) * model_;
    // sx += vx * dt , ...
    //model_ = Translate(sx, sy, sz) * model_;







    GLuint  booluseSmooth=glGetUniformLocation(program, "useSmooth" );
     glUniform1f(booluseSmooth, (float)useSmooth); 

     GLuint  booluseFOG=glGetUniformLocation(program, "useFOG" );
     glUniform1i(booluseFOG, (int)useFOG); 



    glUniformMatrix4fv(view, 1, GL_TRUE, view_); 
    glUniformMatrix4fv(model, 1, GL_TRUE, model_); 


    



    if (cubeFlag == 1) // Filled cube
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else              // Wireframe cube
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(cube_buffer, cube_NumVertices);



    glUseProgram(program3); // Use the shader program

    mat4  model_axis={{1,0,0,0},
                      {0,1,0,0},
                      {0,0,1,0},
                      {0,0,0,1}};

    GLuint view3 = glGetUniformLocation(program3, "view" );
    GLuint model3 = glGetUniformLocation(program3, "model" );
    GLuint projection3 = glGetUniformLocation(program3, "projection" );

    glUniformMatrix4fv(view3, 1, GL_TRUE, view_); 
    glUniformMatrix4fv(model3, 1, GL_TRUE,  model_axis);
    glUniformMatrix4fv(projection3, 1, GL_TRUE, p);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw_axis(axis_buffer, 6);  // draw the axis









    glUseProgram(program2); // Use the shader program
    mat4  model_floor={{1,0,0,0},
                      {0,1,0,0},
                      {0,0,1,0},
                      {0,0,0,1}};

    glUniform1i( glGetUniformLocation(program2, "texture_2D"), 0 );
    glUniform1i( glGetUniformLocation(program2, "Texture_app_flag"), 
                texture_app_flag);
    GLuint view2 = glGetUniformLocation(program2, "view" );
    GLuint model2 = glGetUniformLocation(program2, "model" );
    GLuint projection2 = glGetUniformLocation(program2, "projection" );
         glUniformMatrix4fv(view2, 1, GL_TRUE, view_); 
    glUniformMatrix4fv(model2, 1, GL_TRUE,  model_floor);
    glUniformMatrix4fv(projection2, 1, GL_TRUE, p);
    GLuint  booluseFOG1=glGetUniformLocation(program2, "useFOG" );
     glUniform1i(booluseFOG1, (int)useFOG); 
     



    glUniformMatrix4fv(model, 1, GL_TRUE, model_floor); 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawObj_floor(floor_buffer, floor_NumVertices); 



    glutSwapBuffers();
}
//---------------------------------------------------------------------------
void idle (void)
{
  if(rollingFlag==1)
  {
    
    if((int)delta_x==3&&(int)delta_z==5)
    {
        
        tmp_roll=1;
        delta_x=3;
        delta_z=5;
    }
    if((int)delta_x==-1&&(int)delta_z==-4)
    {

        tmp_roll=2;        
    }
    if(delta_x>=3.5&&delta_z>=-2.5)
    {
        tmp_roll=3;        
    }
    switch(tmp_roll) {
      case 1 : 
            anglez += 1.0;    
            anglex=0.0;
            delta_x -=0.01;
            delta_z -=0.01*2.25;
            break;
      case 2 : 
            anglez -= 1.0;    
            anglex=0.0;
            delta_x +=0.01*3;
            delta_z +=0.01;
            break;
      case 3 :
            anglez = 0.0;    
            anglex+= 1.0;    
            delta_x -=(0.01)/3;
            delta_z +=(0.01*15)/3;
    }
    }
    
    glutPostRedisplay();
}
//----------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
      exit( EXIT_SUCCESS );
      break;

        case 'X': eye[0] += 1.0; break;
  case 'x': eye[0] -= 1.0; break;
        case 'Y': eye[1] += 1.0; break;
  case 'y': eye[1] -= 1.0; break;
        case 'Z': eye[2] += 1.0; break;
  case 'b': rollingFlag=1-rollingFlag; break;
        case 'B': rollingFlag=1-rollingFlag; break;
  case 'z': eye[2] -= 1.0; break;

        case 'a': case 'A': // Toggle between animation and non-animation
      animationFlag = 1 -  animationFlag;
            if (animationFlag == 1) glutIdleFunc(idle);
            else                    glutIdleFunc(NULL);
            break;
     
        case 'c': case 'C': // Toggle between filled and wireframe cube
      cubeFlag = 1 -  cubeFlag;   
            break;

        case 'f': case 'F': // Toggle between filled and wireframe floor
      floorFlag = 1 -  floorFlag; 
            break;

  /*case ' ':  // reset to initial viewer/eye position
      eye = init_eye;
      break;*/
            case ' ':  // Toggle among No Texture (obj color), Texture Only, 
               //        and Modulate the two.
        texture_app_flag++;
            if (texture_app_flag > 2)
            texture_app_flag = 0;
            glutPostRedisplay();
            break;
    }
    glutPostRedisplay();
}
//----------------------------------------------------------------------------
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    aspect = (GLfloat) width  / (GLfloat) height;
    glutPostRedisplay();
}
//----------------------------------------------------------------------------


void myMouse(int button, int state, int x, int y)
{
    if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        rollingFlag=1-rollingFlag;
    }
}




void menu_mouse(int id)
{
    switch(id)
    {
      case 1:
        eye = init_eye;
        break;
      case 6:
        exit(0);        
        break;
    }



}

void Light_menu_options(int id)
{
  switch(id)
    {
      case 1:
        cout<<"No"<<endl;
        break;
      case 2:
        cout<<"Yes"<<endl;
        break;
    }

}


void Wire_Frame_menu_options(int id)
{
  switch(id)
    {
      case 1:
        cubeFlag = 1 ;
        break;
      case 2:
        cubeFlag = 0 ;
        break;
    }

}


void Shading_menu_options(int id)
{
  switch(id)
    {
      case 1:
        useSmooth=1;
        break;
      case 2:
        useSmooth=0;
        break;
    }

}

void FOG_menu_options(int id)
{
  useFOG=id;
    /*switch(id)
    {
      case 1:
        useFOG=1;
        break;
      case 2:
        useFOG=2;
        break;
      case 3:
        useFOG=3;
        break;
      case 4:
        useFOG=4;
        break;
    }*/
}


void Texture_menu_options(int id)
{
      switch(id)
    {
      case 1:
        texture_app_flag=0;
        break;
      case 2:
        texture_app_flag=2;
        break;
      }

}


int main(int argc, char **argv)
{ int err;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Color Cube");

    int Light_menu=glutCreateMenu(Light_menu_options);
    glutAddMenuEntry("No",1);
    glutAddMenuEntry("Yes",2);  

    int Wire_Frame_menu=glutCreateMenu(Wire_Frame_menu_options);
    glutAddMenuEntry("No",1);
    glutAddMenuEntry("Yes",2); 

    int Shading_menu=glutCreateMenu(Shading_menu_options);
    glutAddMenuEntry("Flat Shading",1);
    glutAddMenuEntry("Smooth Shading",2);



    int FOG_menu=glutCreateMenu(FOG_menu_options);
    glutAddMenuEntry("no fog",1);
    glutAddMenuEntry("linear",2);
    glutAddMenuEntry("exponential",3);
    glutAddMenuEntry("exponential square",4); 


    int Texture_menu = glutCreateMenu(Texture_menu_options);
    glutAddMenuEntry("No",1);
    glutAddMenuEntry("Yes",2); 


    
    glutCreateMenu(menu_mouse);
    glutAddMenuEntry("Default View Point",1);
    //glutAddMenuEntry("Enable Lighting",2);
    glutAddSubMenu("Enable Lighting", Light_menu);
    //glutAddMenuEntry("Shading",3);
    glutAddSubMenu("Shading", Shading_menu);
    glutAddSubMenu("Fog Options", FOG_menu);
    glutAddSubMenu("Texture Mapped Ground", Texture_menu);
    glutAddMenuEntry("Light Source",4);
    //glutAddMenuEntry("Wire Frame",5);
    glutAddSubMenu("Wire Frame", Wire_Frame_menu);
    glutAddMenuEntry("Quit",6);
    glutAttachMenu(GLUT_LEFT_BUTTON);

  /* Call glewInit() and error checking */
  err = glewInit();
  if (GLEW_OK != err)
  { printf("Error: glewInit failed: %s\n", (char*) glewGetErrorString(err)); 
    exit(1);
  }
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(myMouse);








    init();
    glutMainLoop();
    return 0;
}
