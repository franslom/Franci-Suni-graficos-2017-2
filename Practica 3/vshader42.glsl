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

 #version 330 core // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

layout(location = 0) in  vec3 vPosition;
layout(location = 1) in  vec3 vColor;
layout(location = 2) in  vec3 vNormal;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec3 Color;
} vs_out;


out float fogFactor;


varying vec4 color;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;




uniform float useSmooth=0.0;
uniform int useFOG;



const float FogDensity = 0.09;

void main() 
{

    vec4 vPosition4 = vec4( vPosition, 1.0 );
    vec4 vColor4 = vec4( vColor, 1.0 ); 

    gl_Position = projection * view * model * vPosition4;
    vec4 _posRespectToCamera = view * model * vPosition4;

    vs_out.FragPos = vec3(model * vec4(vPosition, 1.0)); 
    vs_out.Color = vColor4.rgb;
    mat3 normalMatrix = transpose(inverse(mat3(model))); 
    if ( useSmooth == 1 )
    {
        vs_out.Normal = normalMatrix * vPosition;
    }
    else
    {
        vs_out.Normal = normalMatrix * vNormal;
    }











    float dist = length( _posRespectToCamera.xyz );
	if(useFOG==1)//no fog
    {
        fogFactor = 1.0;
    }
    else if(useFOG==2)//lineal fog
    {
        fogFactor = (18 - dist)/(18);
    }
    else if(useFOG==3)//exponential fog
    {
        fogFactor = exp( -dist * FogDensity );
    }
    else if(useFOG == 4)//exponential square fog
    {
        fogFactor = exp( -pow( ( dist * FogDensity ), 2 ) );
    }

    fogFactor = clamp( fogFactor, 0.0, 1.0 );




    //color = vColor4;
} 
