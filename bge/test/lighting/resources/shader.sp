[vertex]
use /shaders/vertex/lighting;

void calculateLighting();

void main()
{ 
  calculateLighting();
  gl_Position = ftransform();
  gl_TexCoord[0] = gl_MultiTexCoord0;
} 
[fragment]
use /shaders/fragment/lighting;

uniform sampler2D tex;

vec4 calculateLighting();

void main()
{
  gl_FragColor = calculateLighting() * texture2D(tex, gl_TexCoord[0].st);
}
