#version 330 core
#define NB_POINT_LIGHTS 2

out vec4 FragColor;
uniform float uTime;
uniform vec3 cameraPos;
in vec3 Normal;
in vec3 FragPos;
in vec2 textureCoords;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
	
struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
struct DirLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct PointLight{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NB_POINT_LIGHTS];
uniform SpotLight spotLight;
vec3 calcDirectionLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir);

vec3 calcDirectionLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
	vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, textureCoords));
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, textureCoords));

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, textureCoords));

	return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir)
{
	vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, textureCoords));
	
	vec3 lightDir = normalize(pointLight.position - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = pointLight.diffuse * diff * vec3(texture(material.diffuse, textureCoords));

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot( viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, textureCoords));

	float distance = length(pointLight.position - FragPos);
	float attenuation = 1.0/(pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir)
{
	vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, textureCoords));
	vec3 lightDir = normalize(spotLight.position - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, textureCoords));

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot( viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, textureCoords));

	float distance = length(spotLight.position - FragPos);
	float attenuation = 1.0/(spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) +epsilon * theta, 0.0, 1.0);
	// we’ll leave ambient unaffected so we always have a little light.
	diffuse *= intensity;
	specular *= intensity;
	
	return (ambient + diffuse + specular);
}
	
void main()

{
	vec3 result = vec3(0.0f);
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 norm = normalize(Normal);

	result += calcDirectionLight(dirLight, norm, viewDir);


	for(int i = 0; i < NB_POINT_LIGHTS; i++)
		result += calcPointLight(pointLights[i], norm, viewDir);
	//result += calcSpotLight(spotLight, norm, viewDir);	


	FragColor = vec4(result , 1.0f);
}
