[vertex]
in vec3 in_Vertex;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

void main()
{
  gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Vertex, 1.0);
}

[fragment]
out vec4 out_Color;

struct MaterialStruct {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 emission;
  int shininess;
};
uniform MaterialStruct Material;

void main()
{
  out_Color = Material.emission;
}
