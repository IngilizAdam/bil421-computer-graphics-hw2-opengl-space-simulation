#version 150

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
out vec4 color;

uniform mat4 Model;
uniform mat4 ViewProjection;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition[4];
uniform int NumActiveLights;
uniform float Shininess;

void main() {
	// calculate position in view space
	vec4 world_position = Model * vPosition;

	// calculate color based on lightning and material properties
	vec4 diffuse[4];
	vec4 specular[4];

	vec3 L[4];
	vec3 E[4];
	vec3 H[4];
	vec3 N[4];
	float Kd[4];
	float Ks[4];

	vec4 finalLight = vec4(0.0, 0.0, 0.0, 1.0);

	for (int i = 0; i < NumActiveLights; i++) {
		L[i] = normalize(LightPosition[i].xyz - world_position.xyz);
		E[i] = normalize(-world_position.xyz);
		H[i] = normalize(L[i] + E[i]);
		N[i] = normalize(Model * vec4(vNormal, 0.0)).xyz;
		Kd[i] = max(dot(L[i], N[i]), 0.0);
		diffuse[i] = Kd[i] * DiffuseProduct;
		Ks[i] = pow(max(dot(N[i], H[i]), 0.0), Shininess);
		specular[i] = Ks[i] * SpecularProduct;
		if (dot(L[i], N[i]) < 0.0) {
			specular[i] = vec4(0.0, 0.0, 0.0, 1.0);
		}

		finalLight += AmbientProduct + diffuse[i] + specular[i];
	}
	finalLight.w = 1.0;

	// apply camera projection
	gl_Position = ViewProjection * world_position;

	color = vColor * (finalLight);
	color.a = 1.0;
}
