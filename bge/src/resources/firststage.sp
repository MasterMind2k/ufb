[vertex]
in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

out vec3 position, normal;
out vec2 ex_TexCoord;

void main(void)
{
  position = vec3(ModelViewMatrix * vec4(in_Vertex, 1.0));
  normal = mat3(transpose(inverse(ModelViewMatrix))) * in_Normal;

  gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Vertex, 1.0);
  ex_TexCoord = in_TexCoord;
}
[fragment]
in vec3 position, normal;
in vec2 ex_TexCoord;
uniform sampler2D Texture;

struct MaterialStruct {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 emission;
  float shininess;
};
uniform MaterialStruct Material;
uniform float HasLighting;


void main(void)
{
  gl_FragData[0] = vec4(position.xyz, Material.shininess);
  gl_FragData[1] = vec4(normalize(normal.xyz), HasLighting);
  gl_FragData[2] = texture2D(Texture, ex_TexCoord.st);
  gl_FragData[3] = Material.ambient;
  gl_FragData[4] = Material.diffuse;
  gl_FragData[5] = Material.specular;
  gl_FragData[6] = Material.emission;
}

