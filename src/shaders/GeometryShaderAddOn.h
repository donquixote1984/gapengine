#pragma once
#include <string>
#include <vector>
#include "../global/Sky.h"
#include "ShaderSnippet.h"
#include "Shader.h"
enum class ShaderAddOn
{
    ATOMSPHERE,
};
class GeometryShaderAddOn
{
protected:
    std::vector<ShaderSnippet> snippets;
public:
    virtual std::string GetShaderSource(SourceType &) = 0;
    void ApplySnippets(Shader * s) {
        for(auto snippet: snippets)
        {
            snippet.ApplyToShader(s);
        }
    }
};

class AtomsphereShaderAddOn: public GeometryShaderAddOn
{

public:
    AtomsphereShaderAddOn()
    {
        snippets.push_back(ShaderSnippet::AtomsphereSnippet());
    }
    static AtomsphereShaderAddOn *get() {
        static AtomsphereShaderAddOn asa;
        return &asa;
    }

    std::string GetShaderSource(SourceType &sourceType)
    {
        sourceType = SourceType::SOURCE;
        auto atomsphere = Global::atomsphere;
        if (atomsphere== nullptr) {
            return "";
        }
        return atomsphere->GetShaderSource();
    }
};