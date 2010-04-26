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

uniform sampler2D Emission;

/* 19 precalculated values */
float gauss(in int x);
float step();

void main(void)
{
  vec4 blurSample = vec4(0.0);
  float sampleStep = step();

  for(int i = -10; i < 11; i++) {
    blurSample += texture2D(Emission, ex_TexCoord + vec2(i * sampleStep, 0)) * gauss(i);
  }

  gl_FragData[0] = blurSample * 1.5;
}
