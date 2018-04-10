/***************************
 * File: vshader42.glsl:
 *   A simple vertex shader.
 *
 * - Vertex attributes (positions & colors) for all vertices are sent
 *   to the GPU via a vertex buffer object created in the OpenGL program.
 *
 * - This vertex shader uses the Model-View and Projection matrices passed
 *   on from the OpenGL program as uniform variables of type mat4.
 ***************************/

 #version 120 

attribute  vec3 vPosition;
attribute  vec3 vColor;
attribute  vec3 vNormal;


varying vec3 FragPos;
varying vec3 Normal;
varying vec3 Color;
varying vec4 color;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;


uniform float useSmooth=0;


float ddeDeterminant( mat3 A )
{
    float determinant = +A[0][0] * ( A[1][1] * A[2][2] - A[2][1] * A[1][2] )
    -A[0][1] * ( A[1][0] * A[2][2] - A[1][2] * A[2][0] )
    +A[0][2] * ( A[1][0] * A[2][1] - A[1][1] * A[2][0] ); 

    return determinant;
}


mat3 ddeInverse(const in mat3 Matrix)
{
    float D = ddeDeterminant(Matrix);
    if(D == 0.0)
        return mat3(0.0);
    mat3 MatrixT = transpose(Matrix);
    float D00 = MatrixT[1][1] * MatrixT[2][2] + MatrixT[2][1] * MatrixT[1][2];
    float D10 = MatrixT[0][1] * MatrixT[2][2] + MatrixT[2][1] * MatrixT[0][2];
    float D20 = MatrixT[0][1] * MatrixT[1][2] + MatrixT[1][1] * MatrixT[0][2];
    float D01 = MatrixT[1][0] * MatrixT[2][2] + MatrixT[2][0] * MatrixT[1][2];
    float D11 = MatrixT[0][0] * MatrixT[2][2] + MatrixT[2][0] * MatrixT[0][2];
    float D21 = MatrixT[0][0] * MatrixT[1][2] + MatrixT[1][0] * MatrixT[0][2];
    float D02 = MatrixT[1][0] * MatrixT[2][1] + MatrixT[2][0] * MatrixT[1][1];
    float D12 = MatrixT[0][0] * MatrixT[2][1] + MatrixT[2][0] * MatrixT[0][1];
    float D22 = MatrixT[0][0] * MatrixT[1][1] + MatrixT[1][0] * MatrixT[0][1];
    mat3 MatrixAdjugate;
    MatrixAdjugate[0] = vec3( D00, -D01, D02);
    MatrixAdjugate[1] = vec3(-D10, D11, -D12);
    MatrixAdjugate[2] = vec3( D20, -D21, D22);
    return (1.0 / D) * MatrixAdjugate;
}


void main() 
{

    vec4 vPosition4 = vec4( vPosition, 1.0 );
    vec4 vColor4 = vec4( vColor, 1.0 ); 

    gl_Position = projection * view * model * vPosition4;

    FragPos = vec3(model * vec4(vPosition, 1.0)); 
    Color = vColor4.rgb;
    mat3 normalMatrix = transpose(ddeInverse(mat3(model))); 
    if ( useSmooth == 1 )
    {
        Normal = normalMatrix * vPosition;
    }
    else
    {
        Normal = normalMatrix * vNormal;
    }
} 
