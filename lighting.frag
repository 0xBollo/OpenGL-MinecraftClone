#version 330 core

struct DirLight {
	vec3 direction;   // Direction vector from light to fragment     

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;   // Direction of the spotlight starting from its position
	float innerCutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material {
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	float shininess;
};



in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 fragColor;

uniform DirLight dirLight;
uniform PointLight pointLights[100];
uniform int nrPointLights;
uniform SpotLight spotLight;
uniform bool spotLightOn;
uniform Material material;
uniform vec3 camPos;



vec3 calcDirLightColor(DirLight light, vec3 normal, vec3 camDir, vec3 diffTexelColor, vec3 specTexelColor);
vec3 calcPointLightColor(PointLight light, vec3 normal, vec3 camDir, vec3 diffTexelColor, vec3 specTexelColor);
vec3 calcSpotLightColor(SpotLight light, vec3 normal, vec3 camDir, vec3 diffTexelColor, vec3 specTexelColor);

vec3 calcDiffuseColor(vec3 normal, vec3 lightDir, vec3 lightDiffuse, vec3 diffTexelColor);
vec3 calcSpecularColor(vec3 normal, vec3 lightDir, vec3 camDir, vec3 lightSpecular, vec3 specTexelColor);
float calcAttenuation(vec3 lightPos, float constant, float linear, float quadratic);





void main()
{
	vec3 normal = normalize(Normal);
	vec3 camDir = normalize(camPos - FragPos);   // Direction vector from fragment to camera
	vec3 diffTexelColor = vec3(texture(material.diffuseTexture, TexCoords));
	vec3 specTexelColor = vec3(texture(material.specularTexture, TexCoords));

	// Directional light
	vec3 result = calcDirLightColor(dirLight, normal, camDir, diffTexelColor, specTexelColor);

	// Point light
	for (int i = 0; i < nrPointLights; i++)
		result += calcPointLightColor(pointLights[i], normal, camDir, diffTexelColor, specTexelColor);

	// Spotlight
	if (spotLightOn)
		result += calcSpotLightColor(spotLight, normal, camDir, diffTexelColor, specTexelColor);

	// Final color
	fragColor = vec4(result, 1.0f);
}





vec3 calcDirLightColor(DirLight light, vec3 normal, vec3 camDir, vec3 diffTexelColor, vec3 specTexelColor)
{
	vec3 lightDir = normalize(-light.direction);   // Direction vector from fragment to light

	// Color
	vec3 ambient = light.ambient * diffTexelColor;
	vec3 diffuse = calcDiffuseColor(normal, lightDir, light.diffuse, diffTexelColor);
	vec3 specular = calcSpecularColor(normal, lightDir, camDir, light.specular, specTexelColor);

	return ambient + diffuse + specular;
}


vec3 calcPointLightColor(PointLight light, vec3 normal, vec3 camDir, vec3 diffTexelColor, vec3 specTexelColor)
{
	vec3 lightDir = normalize(light.position - FragPos);   // Direction vector from fragment to light

	// Color
	vec3 ambient = light.ambient * diffTexelColor;
	vec3 diffuse = calcDiffuseColor(normal, lightDir, light.diffuse, diffTexelColor);
	vec3 specular = calcSpecularColor(normal, lightDir, camDir, light.specular, specTexelColor);

	// Attenuation
	float att = calcAttenuation(light.position, light.constant, light.linear, light.quadratic);

	return att * (ambient + diffuse + specular);
}


vec3 calcSpotLightColor(SpotLight light, vec3 normal, vec3 camDir, vec3 diffTexelColor, vec3 specTexelColor)
{
	vec3 lightDir = normalize(light.position - FragPos);   // Direction vector from fragment to light

	// Color
	vec3 ambient = light.ambient * diffTexelColor;
	vec3 diffuse = calcDiffuseColor(normal, lightDir, light.diffuse, diffTexelColor);
	vec3 specular = calcSpecularColor(normal, lightDir, camDir, light.specular, specTexelColor);

	// Attenuation
	float att = calcAttenuation(light.position, light.constant, light.linear, light.quadratic);

	// Intensity
	vec3 spotDir = normalize(-light.direction);   // Reversed lighting direction of the spotlight
	float theta = dot(lightDir, spotDir);
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	return att * intensity * (ambient + diffuse + specular);
}


vec3 calcDiffuseColor(vec3 normal, vec3 lightDir, vec3 lightDiffuse, vec3 diffTexelColor)
{
	float diff = max(dot(normal, lightDir), 0.0f);
	return lightDiffuse * diff * diffTexelColor;
}


vec3 calcSpecularColor(vec3 normal, vec3 lightDir, vec3 camDir, vec3 lightSpecular, vec3 specTexelColor) 
{
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(camDir, reflectDir), 0.0f), material.shininess);
	return lightSpecular * spec * specTexelColor; 
}


float calcAttenuation(vec3 lightPos, float lightConstant, float lightLinear, float lightQuadratic)
{
	float dist = length(lightPos - FragPos);
	return 1.0f / (lightConstant + lightLinear * dist + lightQuadratic * pow(dist, 2));
}