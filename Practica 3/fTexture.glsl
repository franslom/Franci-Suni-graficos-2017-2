/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

 #version 120
 
varying  vec4 color;

varying float fogFactor;



varying vec3 FragPos;
varying vec3 Normal;
varying vec3 Color;
varying vec2 Texture;


uniform vec3 lightPos;
uniform vec3 viewPos;

const vec4 fogColor = vec4(0.7, 0.7, 0.7, 0.5);



uniform sampler2D texture_2D;
uniform int Texture_app_flag;


void main() 
{ 
    



    vec3 normal = normalize(Normal);

     // Get diffuse color
    vec3 color = Color;

    if (Texture_app_flag == 0)
       color = Color;
    else if (Texture_app_flag == 1)
       color = texture2D( texture_2D, Texture ).rgb;
    else // Texture_app_flag == 2
       color = Color * texture2D( texture_2D, Texture ).rgb;  

    //color = Color * texture( texture_2D, Texture ).rgb;  







    // Ambient
    vec3 ambient = 0.1 * color;//vec3(1.0,1.0,1.0)+color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - FragPos);


    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;


    //nueva Tarea
    vec4 fColor_a = vec4(ambient + diffuse + specular, 1.0f);
    gl_FragColor = mix(fogColor, fColor_a, fogFactor);
    

} 

