[vertex]
use /shaders/vertex/lighting;
in vec3 in_Vertex;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void calculateLighting();

void main()
{
  calculateLighting();
  gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Vertex, 1.0);
}

[fragment]
use /shaders/fragment/lighting;

vec4 calculateLighting();

void main()
{
  gl_FragColor = calculateLighting();
}

