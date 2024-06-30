#shader vertex
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
	
#shader fragment
#version 410

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 camPos;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(FragPos - lightPos);

    vec3 viewDir = normalize(FragPos - camPos); // point to object
    vec3 reflectDir = -reflect(lightDir, norm); // point to object

    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    float specularStrength = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse ) * objectColor;
    FragColor = vec4(result, 1.0);
}