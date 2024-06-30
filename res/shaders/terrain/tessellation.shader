

#shader tessellation control
#version 410 core
layout (vertices=4) out;
in vec2 tex[];
in mat3 normalMatrix[];
out vec2 texe[];
uniform int u_TessLevelOuter[4];
uniform int u_TessLevelInner[2];

const int MIN_TESS_LEVEL = 16;
const int MAX_TESS_LEVEL = 128;
const float MIN_DISTANCE = 1;
const float MAX_DISTANCE = 50;

uniform mat4 model;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    texe[gl_InvocationID] = tex[gl_InvocationID];
    if (gl_InvocationID == 0)
    {
        vec4 eyeSpacePos00 = view * model * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = view * model * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = view * model * gl_in[2].gl_Position;
        vec4 eyeSpacePos11 = view * model * gl_in[3].gl_Position;

        float distance00 = clamp( (abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance01 = clamp( (abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance10 = clamp( (abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance11 = clamp( (abs(eyeSpacePos11.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );

        float tessLevel0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
        float tessLevel1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
        float tessLevel2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
        float tessLevel3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}

#shader tessellation evaluation
#version 410 core
layout (quads, equal_spacing, cw) in;
in vec2 texe[];

uniform mat4 model;
uniform sampler2D u_HeightMap;
uniform vec4 u_MapRange;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;
out vec3 FragViewPos;

{{__OUT_TBN_TESS__}}
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};


float fitRange(float value, float srcFrom, float srcTo, float destFrom, float destTo)
{
    float v = clamp(value, srcFrom, srcTo);
    return destFrom + (v - srcFrom) / (srcTo - srcFrom) * (destTo - destFrom);
}

void getTangent(vec4 p1, vec4 p2, vec4 p3, vec4 p4, vec2 uv1, vec2 uv2, vec2 uv3, vec2 uv4, out vec3 tangent, out vec3 bitangent)
{

    vec3 edge1 = vec3(p2 - p1);
    vec3 edge2 = vec3(p3 - p1);

    vec2 deltaUV1 = uv2 - uv1;
    vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    vec3 _tangent, _bitangent;
    _tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    _tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    _tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent = normalize(_tangent);

    _bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    _bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    _bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent = normalize(_bitangent);

}
/**
void TerrainCPU::GetTangent(int x, int y, glm::vec3 &tangent, glm::vec3 &bitangent)
{
    glm::vec3 v1 = GetGridPoints(x-1, y);
    glm::vec2 uv1 = {(float)(x - 1) / m_Meta.res,  (float)y / m_Meta.res};

    glm::vec3 v2 = GetGridPoints(x-1, y-1);
    glm::vec2 uv2 = {(float)(x - 1) / m_Meta.res, (float) (y - 1)/m_Meta.res };

    glm::vec3 v3 = GetGridPoints(x, y-1);
    glm::vec2 uv3 = {(float)(x) / m_Meta.res, (float) (y - 1) / m_Meta.res };

    glm::vec3 v4 = GetGridPoints(x, y);
    glm::vec2 uv4 = {(float)(x) / m_Meta.res, (float) (y) / m_Meta.res };

    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v3 - v1;

    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent = glm::normalize(tangent);

    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent = glm::normalize(bitangent);  
}



*/
void main()
{
    
    float heightFactor = 20;
    float u = gl_TessCoord.x; 
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    float delta = 0.001 ;
    float u1 = max(u + delta, 0);
    float v1 = max(v + delta, 0);

    vec4 p00 = (gl_in[0].gl_Position);
    vec4 p01 = (gl_in[1].gl_Position);
    vec4 p10 = (gl_in[2].gl_Position);
    vec4 p11 = (gl_in[3].gl_Position);


    vec2 t00 = texe[0];
    vec2 t01 = texe[1];
    vec2 t10 = texe[2];
    vec2 t11 = texe[3];

    //vec2 t0 = (t01 - t00) * u + t00;
    //vec2 t1 = (t11 - t10) * u + t10;

    vec2 texCoord =     mix(mix(t00, t01, u),  mix(t10, t11, u), v); //(t1 - t0) * v + t0;
    vec2 texCoordu1v =  mix(mix(t00, t01, u1), mix(t10, t11, u1), v); 
    vec2 texCoordu1v1 = mix(mix(t00, t01, u1), mix(t10, t11, u1), v1); 
    vec2 texCoorduv1 =  mix(mix(t00, t01, u),  mix(t10, t11, u), v1); 

    vec4 p =     mix(mix(p00, p01, u),  mix(p10, p11, u), v);
    vec4 pu1v =  mix(mix(p00, p01, u1), mix(p10, p11, u1), v);
    vec4 pu1v1 = mix(mix(p00, p01, u1), mix(p10, p11, u1), v1);
    vec4 puv1 =  mix(mix(p00, p01, u),  mix(p10, p11, u), v1);


    
    float height     = fitRange(texture(u_HeightMap, texCoord).y, u_MapRange.x, u_MapRange.y, u_MapRange.z, u_MapRange.w) * heightFactor;
    float heightu1v  = fitRange(texture(u_HeightMap, texCoordu1v).y, u_MapRange.x, u_MapRange.y, u_MapRange.z, u_MapRange.w) * heightFactor;
    float heightu1v1 = fitRange(texture(u_HeightMap, texCoordu1v1).y, u_MapRange.x, u_MapRange.y, u_MapRange.z, u_MapRange.w) * heightFactor;
    float heightuv1  = fitRange(texture(u_HeightMap, texCoorduv1).y, u_MapRange.x, u_MapRange.y, u_MapRange.z, u_MapRange.w) * heightFactor;


    //vec4 p = mix(mix(p00, p01, u), mix(p10, p11, u), v);

    p.y  = height;
    pu1v.y = heightu1v;
    pu1v1.y = heightu1v1;
    puv1.y = heightuv1;


    vec3 normal = normalize(cross(vec3(p-pu1v), vec3(puv1-p)));
    gl_Position = projection * view * model * p;


    Normal = normal;
    FragPos = vec3(model * p);
    TexCoords = texCoord;
    FragViewPos = vec3(view * model * p);
    {{__TBN_TESS__}}
    //vec3 tangent, bitangent;
    //getTangent(p, puv1, pu1v1, pu1v, texCoord, texCoorduv1, texCoordu1v1, texCoordu1v, tangent, bitangent);
    //TBN = mat3(tangent, bitangent, Normal); 
}
