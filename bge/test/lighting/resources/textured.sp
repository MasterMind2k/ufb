[vertex]
use /shaders/vertex/lighting;
in vec3 in_Vertex;
in vec2 in_TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

varying vec2 var_TexCoord;

void calculateLighting();

void main()
{
  calculateLighting();
  gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Vertex, 1.0);
  var_TexCoord = in_TexCoord;
}
[fragment]
use /shaders/fragment/lighting;

uniform sampler2D tex;
varying vec2 var_TexCoord;

vec4 calculateLighting();

void main()
{
  gl_FragColor = calculateLighting() * texture2D(tex, var_TexCoord.st);
}
