[vertex]
in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

out vec3 position, normal;
out vec2 ex_TexCoord;

void main(void)
{
  position = vec3(ModelViewMatrix * vec4(in_Vertex, 1.0));
  normal = NormalMatrix * in_Normal;

  gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Vertex, 1.0);
  ex_TexCoord = in_TexCoord;
}
[fragment]
in vec3 position, normal;
in vec2 ex_TexCoord;
uniform sampler2D tex;

struct MaterialStruct {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 emission;
  float shininess;
};
uniform MaterialStruct Material;


void main(void)
{
  gl_FragData[0] = vec4(position.xyz, Material.shininess);
  gl_FragData[1] = vec4(normalize(normal.xyz), 0.0);
  gl_FragData[2] = texture2D(tex, ex_TexCoord.st);
  gl_FragData[3] = Material.ambient;
  gl_FragData[4] = Material.diffuse;
  gl_FragData[5] = Material.specular;
  gl_FragData[6] = Material.emission;
}

