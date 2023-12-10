#version 330 core

struct Material {
    vec3 diffuse;
    vec3 specular;    
    float shininess;
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_diffuse4;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_specular4;
	sampler2D texture_normal1;
	sampler2D texture_height1;

	bool useSpecularMaps;

}; 

struct MaterialResults {
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

	float cutOff;
	int initialized;
};

out vec4 outColor;

in vec2 texCoord;
in vec3 fposition;
in vec3 fnormal;

uniform sampler2D tex;
uniform sampler2D texture_diffuse1;

#define MAX_DIR_LIGHTS 6
#define MAX_POINT_LIGHTS 6
#define MAX_SPOT_LIGHTS 6

uniform Light light;
uniform Light dirLight[MAX_DIR_LIGHTS];
uniform Light pointLight[MAX_POINT_LIGHTS];
uniform Light spotLight[MAX_SPOT_LIGHTS];

uniform int id;

uniform Material mat;
uniform vec3 viewPos;

MaterialResults finalMat;
vec3 calculateLight();
vec3 calculatePointLight(int index);
vec3 calculateDirLight(int index);
vec3 calculateSpotLight(int index);
vec3 unifyDiffuse();
vec3 unifySpecular();

uniform bool isModel;

void main()
{	
vec4 texColor = vec4(0.0f,0.0f,0.0f,0.0f);

if(isModel)
{
	texColor = texture(texture_diffuse1, texCoord);
	finalMat.diffuse = unifyDiffuse();
	finalMat.ambient = unifyDiffuse();
	if (mat.useSpecularMaps)
		finalMat.specular = unifySpecular();
	else
		finalMat.specular = mat.specular;
	finalMat.shininess = 32.0f;
}
else
{
	texColor = texture(tex, texCoord);
	finalMat.diffuse = mat.diffuse;
	finalMat.specular = mat.specular;
	finalMat.ambient = mat.diffuse;
	finalMat.shininess = mat.shininess;
}

vec3 lightResultTest = calculateLight();

vec3 lightResultPoint = vec3(0.0f,0.0f,0.0f);
for (int i = 0; i < MAX_POINT_LIGHTS; i++)
{
	lightResultPoint += calculatePointLight(i);
}

vec3 lightResultDir = vec3(0.0f,0.0f,0.0f);
for (int i = 0; i < MAX_DIR_LIGHTS; i++)
{
	lightResultDir += calculateDirLight(i);
}

vec3 lightResultSpot = vec3(0.0f,0.0f,0.0f);
for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
{
	lightResultSpot += calculateSpotLight(i);
}
vec3 lightResultTotal = /*lightResultTest +*/ lightResultPoint + lightResultDir + lightResultSpot;

vec3 baseAmbient = finalMat.ambient * vec3(0.04f,0.04f,0.04f);

outColor = vec4(lightResultTotal, 1.0f);
if (isModel) outColor = vec4(lightResultTotal, 1.0f);

outColor+= vec4(baseAmbient,1.0f); 
}

vec3 calculateLight()
{
	if (light.initialized != 1) return vec3(0.0f,0.0f,0.0f);

	vec3 ambientLight = vec3(0.0f);
	vec3 specular= vec3(0.0f);
	vec3 diffuseFinal= vec3(0.0f);
		
	vec3 posToLightDirVec = normalize(light.position - fposition);

	//Ambient Light
	ambientLight = light.ambient * finalMat.ambient;
	
	//Diffuse Light
	float diffuse = clamp(dot(posToLightDirVec,fnormal),0,1); // producto punto entre la distancia con la luz y la normal
	diffuseFinal = light.diffuse * (diffuse* finalMat.diffuse);
	
	// Specular Light
	vec3 viewDir = normalize(viewPos-fposition);
	vec3 reflectDir = reflect(-posToLightDirVec,fnormal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0f), finalMat.shininess);	
	specular = light.specular * (spec * finalMat.specular);
	
	vec3 lightResult = ambientLight + diffuseFinal + specular;
	
	return lightResult;
}

vec3 calculateDirLight(int index)
{
	if (dirLight[index].initialized != 1) return vec3(0.0f,0.0f,0.0f);
	
	vec3 ambientLight = vec3(0.0f);
	vec3 specular= vec3(0.0f);
	vec3 diffuseFinal= vec3(0.0f);

	vec3 posToLightDirVec = normalize(-dirLight[index].direction);
		
	//Ambient Light
	ambientLight = dirLight[index].ambient * finalMat.ambient;
	
	//Diffuse Light
	float diffuse = clamp(dot(posToLightDirVec,fnormal),0,1); // producto punto entre la distancia con la luz y la normal
	diffuseFinal = dirLight[index].diffuse * (diffuse* finalMat.diffuse);
	
	// Specular Light
	vec3 viewDir = normalize(viewPos-fposition);
	vec3 reflectDir = reflect(-posToLightDirVec,fnormal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0), finalMat.shininess);	
	specular = dirLight[index].specular * (spec * finalMat.specular);
	
	vec3 lightResult = ambientLight + diffuseFinal + specular;

	return lightResult;
}

vec3 calculatePointLight(int index)
{
	if (pointLight[index].initialized != 1) return vec3(0.0f,0.0f,0.0f);
	
	vec3 ambientLight = vec3(0.0f);
	vec3 specular= vec3(0.0f);
	vec3 diffuseFinal= vec3(0.0f);
	
	vec3 posToLightDirVec = normalize(pointLight[index].position - fposition);

	//Ambient Light
	ambientLight = pointLight[index].ambient * finalMat.ambient;
	
	//Diffuse Light
	float diffuse = clamp(dot(posToLightDirVec,fnormal),0,1); // producto punto entre la distancia con la luz y la normal
	diffuseFinal = pointLight[index].diffuse * (diffuse* finalMat.diffuse);
	
	//Specular Light	
	vec3 viewDir = normalize(viewPos-fposition);
	vec3 reflectDir = reflect(-posToLightDirVec,fnormal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0), finalMat.shininess);	
	specular = pointLight[index].specular * (spec * finalMat.specular);
	
	//Atenuation
	float distance    = length(pointLight[index].position - fposition);
	float attenuation = 1.0 / (pointLight[index].constant + pointLight[index].linear * distance + pointLight[index].quadratic * (distance * distance));  

	ambientLight *= attenuation;
	diffuseFinal *= attenuation;
	specular *= attenuation;

	vec3 lightResult = ambientLight + diffuseFinal + specular;

	return lightResult;

}

vec3 calculateSpotLight(int index)
{
	if (spotLight[index].initialized != 1) return vec3(0.0f,0.0f,0.0f);
	
	vec3 ambientLight = vec3(0.0f);
	vec3 specular= vec3(0.0f);
	vec3 diffuseFinal= vec3(0.0f);
		
	//Ambient Light
	
	vec3 posToLightDirVec = normalize(spotLight[index].position - fposition);
	
	float theta = dot(posToLightDirVec, normalize(-spotLight[index].direction));
	
	if(theta > spotLight[index].cutOff) 
	{       
		ambientLight = spotLight[index].ambient * finalMat.ambient;
		//Diffuse Light
		float diffuse = max(dot(fnormal, posToLightDirVec),0.0f); // producto punto entre la distancia con la luz y la normal
		diffuseFinal = spotLight[index].diffuse * (diffuse* finalMat.diffuse);
		
		// Specular Light	
		vec3 viewDir = normalize(viewPos - fposition);
		vec3 reflectDir = reflect(-posToLightDirVec,fnormal);
		float spec = pow(max(dot(viewDir,reflectDir),0.0f), finalMat.shininess);	
		specular = spotLight[index].specular * (spec * finalMat.specular);

		float distance    = length(spotLight[index].position - fposition);
		float attenuation = 1.0 / (spotLight[index].constant + spotLight[index].linear * distance + spotLight[index].quadratic * (distance * distance));  
		
		diffuseFinal *= attenuation;
		specular *= attenuation;

	}	
	vec3 lightResult = ambientLight + diffuseFinal + specular;
	return lightResult;
		
}

vec3 unifyDiffuse()
{
	vec3 finalTex;
	finalTex = texture(mat.texture_diffuse1, texCoord).rgb;
	finalTex += texture(mat.texture_diffuse2, texCoord).rgb;
	finalTex += texture(mat.texture_diffuse3, texCoord).rgb;
	finalTex += texture(mat.texture_diffuse4, texCoord).rgb;
	return finalTex;
}

vec3 unifySpecular()
{
	vec3 finalTex;
	finalTex =  texture(mat.texture_specular1, texCoord).rgb;
	finalTex += texture(mat.texture_specular2, texCoord).rgb;
	finalTex += texture(mat.texture_specular3, texCoord).rgb;
	finalTex += texture(mat.texture_specular4, texCoord).rgb;
	return finalTex;
}