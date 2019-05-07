#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec4 FragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition; 
uniform vec3 viewPosition; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientFactor;
uniform float deffuseFactor;
uniform float specularFactor;
uniform int reflectDegree;

void main()
{	
	vec3 fragPosition, fNormal;
    fragPosition = vec3(model * vec4(position, 1.0));
    fNormal = mat3(transpose(inverse(model))) * normal;  
    
    gl_Position = projection * view * vec4(fragPosition, 1.0);

    vec3 ambient = ambientFactor * lightColor;

    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = deffuseFactor * diff * lightColor;
    
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), reflectDegree);
    vec3 specular = specularFactor * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 