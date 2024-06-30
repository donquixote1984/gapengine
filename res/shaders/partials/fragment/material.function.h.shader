vec3 CalcChannel(Channel c)
{
   
    vec3 color = c.color;
    vec4 tex = vec4(1.0, 1.0, 1.0, 1.0);
    if (c.enable == 0)
    {
        return vec3(0);
    }
    if (c.hasTexture == 1)
    {
        tex = texture(c.tex, TexCoords {{__TILE_FACTOR__}});
        if (tex.a < 0.1)
        {
           // discard;
        }
    }
    return color * vec3(tex);
}

vec3 CalcChannel(Channel c, vec2 uv)
{
   
    vec3 color = c.color;
    vec4 tex = vec4(1.0, 1.0, 1.0, 1.0);
    if (c.enable == 0)
    {
        return vec3(0);
    }
    if (c.hasTexture == 1)
    {
        tex = texture(c.tex, uv {{__TILE_FACTOR__}});
        if (tex.a < 0.1)
        {
           // discard;
        }
    }
    return color * vec3(tex);
}