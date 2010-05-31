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
in vec2 ex_TexCoord;

uniform sampler2D Positions, Normals, ColorMap, Ambient, Diffuse, Specular, Emission;

struct LightStruct {
  vec4 position;
  /* Colors */
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  /* Attenuations */
  float constant;
  float linear;
  float quadratic;
  /* Spot properties */
  float spot_cutoff;
  float spot_exponent;
  vec3 spot_direction;
};

struct MaterialStruct {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 emission;
  float shininess;
};

uniform LightStruct Light;
uniform int UsedLights;
uniform vec4 GlobalAmbient;

vec4 positionalLight(in vec3 ecPos, in vec3 n, in MaterialStruct Material, in LightStruct Light)
{
  vec4 color;
  vec4 ambient, diffuse;
  vec3 aux, lightDir, halfVector;
  float NdotL, NdotHV, dist, att;

  aux = Light.position.xyz - ecPos;
  dist = length(aux);
  lightDir = normalize(aux);

  color = vec4(0.0);

  NdotL = max(dot(n, lightDir), 0.0);

  if (NdotL > 0.0) {
    float spotEffect = 1.0;
    if (Light.spot_cutoff != 180)
      spotEffect = dot(normalize(Light.spot_direction), normalize(-lightDir));

    if (spotEffect > cos(Light.spot_cutoff)) {
      diffuse = Material.diffuse * Light.diffuse;
      ambient = Material.ambient * Light.ambient;

      spotEffect = pow(spotEffect, Light.spot_exponent);
      att = spotEffect / (Light.constant +
            Light.linear * dist +
            Light.quadratic * dist * dist);
      color += att * (diffuse * NdotL + ambient);

      halfVector = normalize(lightDir - normalize(ecPos));
      NdotHV = max(dot(n, halfVector), 0.0);
      color += att * Material.specular * Light.specular * pow(NdotHV, Material.shininess);
    }
  } else {
    discard;
  }
  
  return color;
}

vec4 directionalLight(in vec3 ecPos, in vec3 n, in MaterialStruct Material, in LightStruct Light)
{
  vec4 color;
  vec3 lightDir, halfVector, eye;
  float NdotL, NdotHV;
  
  lightDir = normalize(Light.position.xyz);
  eye = vec3(0, 0, 1);

  /* Ambient */
  color = Material.ambient * Light.ambient;
  
  NdotL = max(dot(n, lightDir), 0.0);

  if (NdotL > 0.0) {
    color += Material.diffuse * Light.diffuse * NdotL;
    halfVector = normalize(eye + lightDir);
    NdotHV = max(dot(n, halfVector), 0.0);
    color += Material.specular *
             Light.specular *
             pow(NdotHV, Material.shininess);
  } else {
    discard;
  }
  
  return color;
}

vec4 light1(in vec3 ecPos, in vec3 n, in MaterialStruct Material)
{
  if (Light.position.w == 1.0)
    return positionalLight(ecPos, n, Material, Light);
  else
    return directionalLight(ecPos, n, Material, Light);
}

void main(void)
{
  vec4 temp = texture2D(Positions, ex_TexCoord.st);
  vec3 position = temp.xyz;
  if (length(position) == 0.0)
    discard;
  vec3 normal = texture2D(Normals, ex_TexCoord.st).xyz;
  float lighting = texture2D(Normals, ex_TexCoord.st).w;
  vec4 colorMap = texture2D(ColorMap, ex_TexCoord.st);

  /* Material */
  MaterialStruct Material;
  Material.shininess = temp.w * 100;
  Material.ambient = texture2D(Ambient, ex_TexCoord.st);
  Material.diffuse = texture2D(Diffuse, ex_TexCoord.st);
  Material.specular = texture2D(Specular, ex_TexCoord.st);
  Material.emission = texture2D(Emission, ex_TexCoord.st);
  vec4 color = Material.emission;

  bool lightingOnly = GlobalAmbient.r + GlobalAmbient.g + GlobalAmbient.b == 0.0;

  if (color.r + color.g + color.b == 0.0) {
    if (lighting > 0.0) {
      color = GlobalAmbient * Material.ambient;
      if (UsedLights > 0)
        color += light1(position, normal, Material);
    } else if (!lightingOnly) {
      color = vec4(1.0);
    }
  } else {
    discard;
  }
  if (colorMap.r + colorMap.g + colorMap.b > 0.0)
    color *= colorMap;
  gl_FragData[0] = color;
}

