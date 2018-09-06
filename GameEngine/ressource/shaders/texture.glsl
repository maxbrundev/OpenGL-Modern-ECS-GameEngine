#shader vertex
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 v_pos;
out vec3 v_normal;
out vec2 v_texCoord;

void main()
{
	v_pos = vec3(u_model * vec4(position, 1.0));
	v_normal = mat3(transpose(inverse(u_model))) * normal;
	v_texCoord = texCoord;

	gl_Position = u_proj * u_view * vec4(v_pos, 1.0);
}

#shader fragment
#version 450 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texCoord;

uniform sampler2D	u_texture;

uniform vec3		u_viewPos;
uniform Material	u_material;
uniform Light		u_light;

out vec4 color;

void main()
{
	

	// ambient
    vec3 ambient = u_light.ambient * u_material.ambient;

	// diffuse 
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_light.position - v_pos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_texture, v_texCoord)) * u_material.diffuse;

	 // specular
    vec3 viewDir = normalize(u_viewPos - v_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * (spec * u_material.specular);  
        
    vec3 result = ambient + diffuse + specular;

	color = vec4(result, 1.0);
}
