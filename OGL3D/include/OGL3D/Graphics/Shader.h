#pragma once
#include <OGL3D/OPrerequisities.h>
#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OVec4.h>

class Shader
{
public:
    Shader(const ShaderDesc& desc);
    ~Shader();
    void use();
    void notuse();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const OVec3& vec) const;
    void setVec3(const std::string& name, f32 x, f32 y, f32 z) const;
    void setVec4(const std::string& name, const OVec4& vec);
    void setMat4(const std::string& name, const OMat4& mat) const;
    
    ui32 getID();
    //void setVec2(const std::string& name, const vec2& value) const;
    //void setMat2(const std::string& name, const mat2& mat) const;
    //void setMat3(const std::string& name, const mat3& mat) const;

private:
    void checkCompileErrors(ui32 shader, std::string type);
private:
    unsigned int ID;
//public:
};

