#version 330 core
out vec4 FragColor;

in vec3 fNormal;  
in vec3 fragPosition;  
  
uniform vec3 lightPosition; 
uniform vec3 viewPosition; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientFactor;
uniform float diffuseFactor;
uniform float specularFactor;
uniform int reflectDegree;

void main()
{
    vec3 ambient = ambientFactor * lightColor;

    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseFactor * diff * lightColor;
    
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), reflectDegree);
    vec3 specular = specularFactor * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 