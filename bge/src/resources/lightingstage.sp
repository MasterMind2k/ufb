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

uniform sampler2D Tex0, Tex1, Tex2, Tex3, Tex4, Tex5, Tex6;

out vec4 out_Color;

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

uniform LightStruct Lights0;
uniform LightStruct Lights1;
uniform LightStruct Lights2;
uniform LightStruct Lights3;
uniform int UsedLights;
uniform vec4 GlobalAmbient;

vec4 positionalLight(in vec3 ecPos, in vec3 n, in vec4 colorMap, in MaterialStruct Material, in LightStruct Light)
{
  vec4 color;
  vec4 ambient, diffuse;
  vec3 aux, lightDir, halfVector;
  float NdotL, NdotHV, dist, att;

  aux = Light.position.xyz - ecPos;
  dist = length(aux);
  lightDir = normalize(aux);

  color = Material.ambient * GlobalAmbient;
  ambient = Material.ambient * Light.ambient;

  diffuse = Material.diffuse * Light.diffuse;
  NdotL = max(dot(n, lightDir), 0.0);

  if (NdotL > 0.0) {
    float spotEffect = 1.0;
    if (Light.spot_cutoff != 180)
      spotEffect = dot(normalize(Light.spot_direction), normalize(-lightDir));

    if (spotEffect > cos(Light.spot_cutoff)) {
      spotEffect = pow(spotEffect, Light.spot_exponent);
      att = spotEffect / (Light.constant +
            Light.linear * dist +
            Light.quadratic * dist * dist);
      color += att * (diffuse * NdotL + ambient);

      halfVector = normalize(lightDir - normalize(ecPos));
      NdotHV = max(dot(n, halfVector), 0.0);
      color += att * Material.specular * Light.specular * pow(NdotHV, Material.shininess);
    }
  }
  
  return color;
}

vec4 directionalLight(in vec3 ecPos, in vec3 n, in vec4 colorMap, in MaterialStruct Material, in LightStruct Light)
{
  vec4 color;
  vec3 lightDir, halfVector, eye;
  float NdotL, NdotHV;
  
  lightDir = normalize(Light.position.xyz);
  eye = vec3(0, 0, 1);

  /* Ambient */
  color = GlobalAmbient * Material.ambient + Material.ambient * Light.ambient;
  
  NdotL = max(dot(n, lightDir), 0.0);

  if (NdotL > 0.0) {
    color += Material.diffuse * Light.diffuse * NdotL;
    halfVector = normalize(eye + lightDir);
    NdotHV = max(dot(n, halfVector), 0.0);
    color += Material.specular *
             Light.specular *
             pow(NdotHV, Material.shininess);
  }
  
  return color;
}

vec4 light1(in vec3 ecPos, in vec3 n, in vec4 colorMap, in MaterialStruct Material)
{
  if (Lights0.position.w == 1.0)
    return positionalLight(ecPos, n, colorMap, Material, Lights0);
  else
    return directionalLight(ecPos, n, colorMap, Material, Lights0);
}

vec4 light2(in vec3 ecPos, in vec3 n, in vec4 colorMap, in MaterialStruct Material)
{
  if (Lights1.position.w == 1.0)
    return positionalLight(ecPos, n, colorMap, Material, Lights1);
  else
    return directionalLight(ecPos, n, colorMap, Material, Lights1);
}

vec4 light3(in vec3 ecPos, in vec3 n, in vec4 colorMap, in MaterialStruct Material)
{
  if (Lights2.position.w == 1.0)
    return positionalLight(ecPos, n, colorMap, Material, Lights2);
  else
    return directionalLight(ecPos, n, colorMap, Material, Lights2);
}

vec4 light4(in vec3 ecPos, in vec3 n, in vec4 colorMap, in MaterialStruct Material)
{
  if (Lights3.position.w == 1.0)
    return positionalLight(ecPos, n, colorMap, Material, Lights3);
  else
    return directionalLight(ecPos, n, colorMap, Material, Lights3);
}

void main(void)
{
  vec4 temp = texture2D(Tex0, ex_TexCoord.st);
  vec3 position = temp.xyz;
  vec3 normal = texture2D(Tex1, ex_TexCoord.st).xyz;
  vec4 colorMap = texture2D(Tex2, ex_TexCoord.st);

  /* Material */
  MaterialStruct Material;
  Material.shininess = temp.w * 100;
  Material.ambient = texture2D(Tex3, ex_TexCoord.st);
  Material.diffuse = texture2D(Tex4, ex_TexCoord.st);
  Material.specular = texture2D(Tex5, ex_TexCoord.st);
  Material.emission = texture2D(Tex6, ex_TexCoord.st);
  vec4 color = Material.emission;

  if (color.r + color.g + color.b == 0.0) {
    if (UsedLights > 0)
      color += light1(position, normal, colorMap, Material);
    if (UsedLights > 1)
      color += light2(position, normal, colorMap, Material);
    if (UsedLights > 2)
      color += light3(position, normal, colorMap, Material);
    if (UsedLights > 3)
      color += light4(position, normal, colorMap, Material);
  }
  if (colorMap.r + colorMap.g + colorMap.g > 0.0)
    color *= colorMap;
  out_Color = color;
}

