#include "GeometryRenderPass.h"


GeometryRenderPass::GeometryRenderPass(Geometry *g)
{
    m_Type = RenderPassType::GEOMETRY;
    m_Geo = g;
}

void GeometryRenderPass::Render(RenderContext &rc) 
{

    /**
     *     OnRenderMVP(window);
            OnRenderMisc(window);
            OnRenderMaterial();
     * 
    */
    OnRenderMVP(rc);
    OnRenderMaterial(rc);
    m_Geo->PreRender();
    //Global::atomsphere->Bind();
    m_Geo->RenderDrawCall();
}

void GeometryRenderPass::OnRenderMVP(RenderContext &rc) const
{
    Shader * shader = m_Geo->GetMaterial()->GetShader();
    shader->Bind();
    shader->setUniform4m("model", m_Geo->GetModelMat());
    shader->setUniform3f("viewPos", rc.camPos.x, rc.camPos.y, rc.camPos.z);

    if (m_Geo->GetGeoData()->HasAnimation() && m_Geo->IsPlaying())
    {
        m_Geo->PlayAnimation();
        shader->setUniform1i("u_IsPlaying", true);
        m_Geo->GetAnimator()->UpdateAnimation(rc.deltaTime);
        std::vector<glm::mat4> transforms = m_Geo->GetAnimator()->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            shader->setUniform4m("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        //shader->setUniform4m();
    } else {
        shader->setUniform1i("u_IsPlaying", false);
    }
}

void GeometryRenderPass::OnRenderMaterial(RenderContext &rc) const
{
    Material * mat = m_Geo-> GetMaterial();
    
    mat->FlushToShader();
    mat->FlushToGlobalSettings();
}