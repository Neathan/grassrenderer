#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

layout(location = 2) in vec2 in_offset;
layout(location = 3) in vec2 in_direction;
layout(location = 4) in float in_tilt;
layout(location = 5) in vec4 in_color;
layout(location = 6) in float in_widths;
layout(location = 7) in float in_bends;


layout(location = 0) out vec3 position;
layout(location = 1) out vec2 uv;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec4 color;


layout(location = 4) out float bladePos;


layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;

layout(location = 3) uniform float time;
layout(location = 4) uniform vec2 windDirection;
layout(location = 5) uniform vec3 characterPos;
layout(location = 6) uniform float useCharacterPos;

layout(binding = 1) uniform sampler2D windSampler;


vec3 bezier(float t, vec3 p0, vec3 p1, vec3 p2, vec3 p3) {
	float t1 = (1-t);
	return        t1 * t1 * t1 * p0
		   + 3  * t1 * t1 * t  * p1
		   + 3  * t1 * t  * t  * p2
		   +      t  * t  * t  * p3;
}

vec3 bezierDerivative(float t, vec3 p0, vec3 p1, vec3 p2, vec3 p3) {
	float t1 = (1-t);
	vec3 a = 3*(p1-p0);
	vec3 b = 3*(p2-p1);
	vec3 c = 3*(p3-p2);
	return       t1 * t1 * a
		   + 2 * t1 * t  * b
		   +     t  * t  * c;
}

const float MIDPOINT_FACTOR = 0.7;
const float WIND_SPEED = 10.0f;
const float TOTAL_WORLD_DIMENTION= 250;

const float CHARACTER_SIZE = 5.0f;

vec3 base, tip, bendPoint;
vec3 up, right;

// float delta - Position along the blade (ratio)
// vec3 direction - The foward direction of the grass
// float tilt - Height of the tip of the grass relative to the base (normalized scale)
// float bend - The midpoint offset
// float width - Thw width of the blade
vec3 generatePath(float delta, vec3 direction, float tilt, float bend, float width) {
	base = vec3(0,0,0);
	tip = direction + vec3(0, tilt, 0); // Multi by some length?

	// Sample wind offset
	float windAmount = texture(windSampler, (in_offset + time * WIND_SPEED) / TOTAL_WORLD_DIMENTION).r;
	tip += vec3(windDirection.x, 0, windDirection.y) * windAmount;

	// Push grass from character
	vec3 toCharacter = (characterPos - tip - vec3(in_offset.x, 0, in_offset.y));
	toCharacter.y = 0;
	float dist = length(toCharacter);
	tip -= normalize(toCharacter) * max((CHARACTER_SIZE - dist) * 0.5, 0) * useCharacterPos;


	vec3 midpoint = tip * MIDPOINT_FACTOR;
	
	// Calculate the up vector based on tilt
	right = normalize(cross(direction, vec3(0,1,0)));
	up = normalize(cross(right, tip));

	bendPoint = midpoint + up * bend;

	// Generate bezier point
	vec3 bezierPoint = bezier(delta, base, bendPoint, bendPoint, tip); // Should midpoint controls points differ?

	float side = gl_VertexID % 2 == 0 ? -1 : 1;

	vec3 result = bezierPoint + side * right * width;
	return result;
}

vec3 generateNormal(float delta) {
	vec3 tangent = bezierDerivative(delta, base, bendPoint, bendPoint, tip);

	vec3 normal = cross(right, tangent);

	// Flat move normal (non-spherical)
	normal += right * in_position.x; // Functions as the distance along the width
	normal = normalize(normal);

	// Update TBN
	// TBN = transpose(mat3(tangent, right, normal));

	return normal;
}

float getGrassWidth(float delta) {
	return min(1.5 - 1.5 * delta, 0.85);
}

void main() {
	uv = in_uv;
	color = in_color;

	float t = in_position.y; // Functions as the distance along the curve
	bladePos = t;

	float shapeWidth = getGrassWidth(t);

	vec3 bezierPoint = generatePath(t, vec3(in_direction.x, 0, in_direction.y), in_tilt, in_bends, in_widths * shapeWidth);
	position = bezierPoint;

	position += vec3(in_offset.x, 0, in_offset.y);
	

	normal = generateNormal(t);

	// Old normal generation code

	/*
	vec3 tangent1 = bezierDerivative(t, base, bendPoint, bendPoint, tip);
	vec3 tangent2 = bezierDerivative(t+0.05, base, bendPoint, bendPoint, tip);

	vec3 c = cross(tangent2, tangent1);

	normal = cross(c, tangent1);
	// normal.x += localX;
	normal = normalize(normal);

	normal = mat3(model) * normal;
	*/

	gl_Position = projection * view * vec4(position, 1);
}
