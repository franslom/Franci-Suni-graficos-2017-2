/*--------------
Vertex Shader: filename "vTexture.glsl"
---------------*/
 #version 120    

attribute vec3 vPosition;
attribute  vec3 vColor;


varying vec3 FragPos;
varying vec3 Color;


uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;


void main() 
{     
 	vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
 	gl_Position = projection * view*model * vPosition4;
 	vec4 vColor4 = vec4( vColor, 1.0 ); 

	FragPos = vec3(model * vec4(vPosition, 1.0)); 
    Color = vColor4.rgb;

} 
