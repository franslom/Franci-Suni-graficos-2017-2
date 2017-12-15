/*--------------
Fragment Shader: filename "fTexture.glsl"
---------------*/
// #version 150   // YJC: Comment/un-comment this line to resolve compilation errors
                  //      due to different settings of the default GLSL version
#version 330 core

out vec4 fColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Color;
} fs_in;

void main() 
{ 
    fColor=vec4(fs_in.Color,1.0);

} 

