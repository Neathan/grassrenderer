#version 460 core

layout(location = 0) out vec4 fragColor;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 color;
layout(location = 4) in float bladePos;

layout(binding = 0) uniform sampler2D grassSampler;

layout(location = 10) uniform float debugLODAmount;
layout(location = 11) uniform vec3 cameraPos;

const vec4 DEBUG_COLOR = vec4(1, 0, 1, 1);

void main() {
	// vec4 color = vec4(0.27,0.42,0.11,1);
	//if(texture(grassSampler, uv).a < 0.5) {
	//	discard;
	//}

	//vec3 tNormal = texture(grassNormalSampler, uv).rgb;
	//tNormal = normalize(tNormal * 2.0 - 1.0);

	vec3 lightDir = normalize(vec3(0, 1, 0));

	vec3 viewDir = normalize(cameraPos - position);
	vec3 reflectDir = reflect(-lightDir, normal);

	float ambient = 0.1;
	float diffuse = max(dot(lightDir, normal), 0);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	float totalLight = min(1, ambient + diffuse + specular);

	float ao = min((bladePos * 2 + 0.1), 1) ;

	fragColor = color * totalLight * ao + DEBUG_COLOR * debugLODAmount;
}
