[vertex]
in vec3 in_Vertex;
in vec2 in_TexCoord;

out vec2 ex_TexCoord;

void main(void)
{
  gl_Position = vec4(in_Vertex, 1.0);

  ex_TexCoord = in_TexCoord;
}
[fragment]
use /shaders/fragment/bloom;
in vec2 ex_TexCoord;

uniform sampler2D Output;

out vec4 out_Color;

float gauss(in int x);
float step();

void main(void)
{
  out_Color = texture2D(Output, ex_TexCoord);
}
