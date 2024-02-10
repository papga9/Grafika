#version 400 core
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

struct Torch {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 direction;
	bool on;
	float cutOff;
};

uniform Material ground;
uniform Light light;
uniform Torch torch;
uniform vec3 viewPos;
uniform float attenuation_const;
uniform float attenuation_lin;
uniform float attenuation_quad;
uniform vec3 fogColor;

void main() {
	float distance = length(viewPos - fragPos);
	float attenuation = 1.0f;// / (attenuation_const + attenuation_lin / 5.0f * distance + attenuation_quad / 5.0f * distance * distance);
	vec3 tmp = vec3(0.0f, 0.0f, 0.0f);
	tmp += light.ambient * vec3(texture(ground.diffuse, texCoord)) * 1.1f * attenuation;
	tmp += light.diffuse * max(dot(normalize(normal), normalize(-light.position)), 0.0f) * vec3(texture(ground.diffuse, texCoord)) * 1.1f * attenuation;
	tmp += light.specular * pow(max(dot(normalize(viewPos - fragPos), reflect(normalize(-light.position), normal)), 0.0f), 2) * vec3(texture(ground.specular, texCoord)) * 1.1f * attenuation;

	if (torch.on) {
		if (dot(normalize(torch.position - fragPos * vec3(1.0f, 1.0f, 1.0f)), normalize(-torch.direction)) > torch.cutOff) {
			distance = length(torch.position - fragPos);
			attenuation = exp((dot(normalize(torch.position - fragPos), normalize(-torch.direction)) - 1.0f) * 80.0f) / (attenuation_const + attenuation_lin * distance + attenuation_quad * distance * distance) ;
			tmp += torch.ambient * vec3(texture(ground.diffuse, texCoord)) * attenuation;
			tmp += torch.diffuse * max(dot(normalize(normal), normalize(-torch.direction)), 0.0f) * vec3(texture(ground.diffuse, texCoord)) * attenuation;
			tmp += torch.specular * pow(max(dot(normalize(viewPos - fragPos), reflect(normalize(vec3(-torch.direction.x, torch.direction.y, -torch.direction.z)), normal)), 0.0f), ground.shininess) * vec3(texture(ground.specular, texCoord)) * attenuation;
		}
	}

	fragColor = mix(vec4(fogColor, 1.0f), vec4(tmp, 1.0f), max(min((200.0f - length(viewPos - fragPos)) / (200.0f - 50.0f), 1.0f), 0.0f));
}
