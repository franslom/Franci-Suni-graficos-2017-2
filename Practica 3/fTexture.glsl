/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

 #version 330 core // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version

in  vec4 color;
out vec4 fColor;
in float fogFactor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
    vec2 Texture;
} fs_in;


uniform vec3 lightPos;
uniform vec3 viewPos;

const vec4 fogColor = vec4(0.7, 0.7, 0.7, 0.5);



uniform sampler2D texture_2D;
uniform int Texture_app_flag;


void main() 
{ 
    



    vec3 normal = normalize(fs_in.Normal);

     // Get diffuse color
    vec3 color = fs_in.Color;

    if (Texture_app_flag == 0)
       color = fs_in.Color;
    else if (Texture_app_flag == 1)
       color = texture( texture_2D, fs_in.Texture ).rgb;
    else // Texture_app_flag == 2
       color = fs_in.Color * texture( texture_2D, fs_in.Texture ).rgb;  

    //color = fs_in.Color * texture( texture_2D, fs_in.Texture ).rgb;  







    // Ambient
    vec3 ambient = 0.1 * color;//vec3(1.0,1.0,1.0)+color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);


    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;


    //nueva Tarea
    vec4 fColor_a = vec4(ambient + diffuse + specular, 1.0f);
    fColor = mix(fogColor, fColor_a, fogFactor);
    

} 

