/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

 #version 120 

varying  vec4 color;

varying vec3 FragPos;
varying vec3 Normal;
varying vec3 Color;


uniform vec3 lightPos;
uniform vec3 viewPos;

void main() 
{ 
    vec3 normal = normalize(Normal);
     // Get diffuse color
    vec3 color = Color;
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
    gl_FragColor = vec4(ambient + diffuse + specular, 1.0f);

} 

