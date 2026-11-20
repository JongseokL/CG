#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform vec4 objectColor;
uniform vec3 lightPos;
uniform bool useLighting;
uniform bool lightEnabled;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    if (!useLighting)
    {
        FragColor = objectColor;
        return;
    }

    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 normalVector = normalize(Normal);

    vec3 reflectDir = reflect(-lightDir, normalVector);

    float specularLight = max(dot(viewDir, reflectDir), 0.0);

    float shininess = 32.0;
    specularLight = pow(specularLight, shininess);

    float diffuseLight = max(dot(normalVector, lightDir), 0.0);


    float ambientLight = 0.3;
    vec3 lightColor2 = vec3(1.0, 1.0, 1.0);

    vec3 ambient = ambientLight * lightColor2;
    vec3 diffuse = diffuseLight * lightColor;
    vec3 specular = specularLight * lightColor;
    
    if (!lightEnabled)
    {
        diffuse = vec3(0.0);
        specular = vec3(0.0);
    }
    

    vec3 result = (ambient + diffuse + specular) * objectColor.rgb;
    FragColor = vec4(result, objectColor.a);
}