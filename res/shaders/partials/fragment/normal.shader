if (material.normal.hasTexture == 1) {
    norm = CalcChannel(material.normal);
    norm = normalize(norm * 2.0 - 1.0);
    {{__TBN_FRAG_NORMAL__}}

    if (u_CombineNormal == 1)
    {
       // norm = normalize(norm + Normal);
    }
} else {
    norm = normalize(Normal);
}