[vertex]
use /shaders/vertex/lighting;
in vec3 in_Vertex;
in vec2 in_TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

out vec2 ex_TexCoord;

void calculateLighting();

void main()
{
  calculateLighting();
  gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Vertex, 1.0);
  ex_TexCoord = in_TexCoord;
}
[fragment]
use /shaders/fragment/lighting;
uniform sampler2D tex;
in vec2 ex_TexCoord;
out vec4 out_Color;

vec4 calculateLighting();

void main()
{
  out_Color = calculateLighting() * texture2D(tex, ex_TexCoord.st);
}
