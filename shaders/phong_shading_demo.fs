#version 330 core

in vec3 interpPos;
in vec3 interpNor;
in vec2 interpTex;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

uniform vec3 ambientLight;

uniform vec3 cameraPos;

uniform vec3 dirLightDir;
uniform vec3 dirLightRadiance;

uniform vec3 pointLightPos;
uniform vec3 pointLightIntensity;

uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform vec3 spotLightIntensity;	
uniform float totalWidth;
uniform float falloffStart;

uniform bool IsMapKd;
uniform sampler2D texmapKd;

out vec4 FragColor;


vec3 diffuse(vec3 radiance , vec3 normal , vec3 lightdir){
    if(IsMapKd){
        return texture2D(texmapKd , vec2(interpTex.x , interpTex.y)).rgb* radiance * max(0.0f , dot(normal , lightdir));
    }
    return Kd * radiance * max(0.0f , dot(normal , lightdir));
}

vec3 specular(vec3 intensity , vec3 cameradir , vec3 reflectdir){
    float value = dot(cameradir , reflectdir);
    float power = pow(max(value , 0.0) , Ns);
    return Ks * intensity * power;
}

vec3 PointLight(vec3 pos , vec3 nor , vec3 cameradir){
    vec3 PointLightDir = normalize(pointLightPos - pos);
    vec3 ReflectDir = reflect(-PointLightDir , nor);
    ReflectDir = normalize(ReflectDir);

    float Distance = length(pointLightPos - pos);
    float Attenuation = 1.0f / (Distance * Distance);
    vec3 Radiance = pointLightIntensity * Attenuation;

    vec3 D = diffuse(Radiance , nor , PointLightDir);
    vec3 S = specular(Radiance , cameradir , ReflectDir);
    
    return D + S;
}

vec3 DirLight(vec3 nor , vec3 cameradir){
    vec3 LightDir = normalize(-dirLightDir);
    vec3 ReflectDir = reflect(-LightDir , nor);
    ReflectDir = normalize(ReflectDir);
    vec3 D = diffuse(dirLightRadiance , nor , LightDir);
    vec3 S = specular(dirLightRadiance , cameradir , ReflectDir);
    return D + S;
}

vec3 SpotLight(vec3 pos , vec3 nor , vec3 cameradir){
    vec3 LightDir = normalize(spotLightPos - pos);
    vec3 ReflectDir = reflect(-LightDir , nor);
    ReflectDir = normalize(ReflectDir);

    float Cosinetheta = dot(LightDir , normalize(-spotLightDir));
    float Epsilon = cos(radians(falloffStart)) - cos(radians(totalWidth));
    float Distance = length(spotLightPos - pos);
    float Attenuation = 1.0 / (Distance * Distance);
    
    vec3 Intensity = spotLightIntensity * clamp((Cosinetheta - cos(radians(totalWidth))) / Epsilon , 0.0 , 1.0) * Attenuation;
    vec3 D = diffuse(Intensity , nor , LightDir);
    vec3 S = specular(Intensity , cameradir , ReflectDir);
    return D + S;
}

void main()
{
    vec3 interpNor = normalize(interpNor);
    vec3 CameraDir = normalize(cameraPos - interpPos);
    vec3 ambient = Ka * ambientLight;
    
    //pointlight + directionallight + spotlight   
    vec3 interpColor = ambient;
    interpColor += PointLight(interpPos , interpNor , CameraDir); 
    interpColor += DirLight(interpNor , CameraDir);
    interpColor += SpotLight(interpPos , interpNor , CameraDir);    
    
    FragColor = vec4(interpColor , 1.0);
}