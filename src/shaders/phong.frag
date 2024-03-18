#version 330


in vec2 TexCord;
in vec3 Normal;
in vec3 CurrentPosition;

uniform vec3 Color;

//Camera
uniform vec3 CameraPosition;

//Light Uniforms
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform float LightPower;

uniform float AmbientPower;
uniform vec4 AmbientColor;

uniform float DiffusePower;
uniform vec4 DiffuseColor;

uniform float SpecularPower;
uniform vec4 SpecularColor;

uniform vec3 MeshColor;
uniform float Roughness;

out vec4 FragColor;

vec4 PointLight(float power){
	
	vec3 LightVector = LightPosition - CurrentPosition;
	float distance = length(LightVector);

	float a = 3.1f;
	float b = 1.2f;

	float intensity = 1.0f / (a*distance*distance + b*distance + 1.0f);
	
	vec3 normal = normalize(Normal);
	vec3 LightDirection = normalize(LightPosition - CurrentPosition);
	float diffuse = max(dot(normal, LightDirection), 0.0f);

	diffuse = max(diffuse,0.0);
	
	vec3 ViewDirection = normalize(CameraPosition - CurrentPosition);
	vec3 ReflectionDirection = reflect(-LightDirection, normal);

	vec3 halfway = normalize(ViewDirection + LightDirection);

	float SpecularAmount = pow(max(dot(normal,halfway),0.0f),16);
	float specular = SpecularAmount * SpecularPower;

	return ((vec4(Color,1.0f) * (diffuse*DiffusePower + AmbientPower + intensity) + specular) * vec4(LightColor,1.0)) * power;
}

void main(){
    FragColor = PointLight(LightPower);
}