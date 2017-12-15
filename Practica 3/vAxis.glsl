/*--------------
Vertex Shader: filename "vTexture.glsl"
---------------*/
// #version 150    // YJC: Comment/un-comment this line to resolve compilation errors
                   //      due to different settings of the default GLSL version
#version 330 core

layout(location = 0) in  vec3 vPosition;
layout(location = 1) in  vec3 vColor;

out VS_OUT {
	vec3 FragPos;
	vec3 Color;
} vs_out;


uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;


void main() 
{     
 	vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
 	gl_Position = projection * view*model * vPosition4;
 	vec4 vColor4 = vec4( vColor, 1.0 ); 

	vs_out.FragPos = vec3(model * vec4(vPosition, 1.0)); 
    vs_out.Color = vColor4.rgb;

} 
