#include <OGL3D/Graphics/Shader.h>
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
Shader::Shader(const ShaderDesc& desc)
{
    const char* vertexPath = desc.vsFilePath;
    const char* fragmentPath = desc.fsFilePath;
    const char* geometryPath = desc.gsFilePath;
    const char* tes_ControlPath = desc.tcsFilePath;
    const char* tes_EvaluationPath = desc.tesFilePath;

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::string tes_control_Code;
    std::string tes_eval_Code;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    std::ifstream tes_controlShaderFile;
    std::ifstream tes_evalShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    tes_controlShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    tes_evalShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
        if (tes_ControlPath != nullptr && tes_EvaluationPath!=nullptr)
        {
            tes_controlShaderFile.open(tes_ControlPath);
            std::stringstream tcShaderStream;
            tcShaderStream << tes_controlShaderFile.rdbuf();
            tes_controlShaderFile.close();
            tes_control_Code = tcShaderStream.str();

            tes_evalShaderFile.open(tes_EvaluationPath);
            std::stringstream teShaderStream;
            teShaderStream << tes_evalShaderFile.rdbuf();
            tes_evalShaderFile.close();
            tes_eval_Code = teShaderStream.str();

        }
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    ui32 vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    ui32 geometry;
    if (geometryPath != nullptr)
    {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    ui32 tesse_control,tesse_eval;
    if (tes_ControlPath != nullptr && tes_EvaluationPath != nullptr)
    {
        const char* tcShaderCode = tes_control_Code.c_str();
        tesse_control = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tesse_control, 1, &tcShaderCode, NULL);
        glCompileShader(tesse_control);
        checkCompileErrors(tesse_control, "TESSELLATION_CONTROL");


        const char* teShaderCode = tes_eval_Code.c_str();
        tesse_eval = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tesse_eval, 1, &teShaderCode, NULL);
        glCompileShader(tesse_eval);
        checkCompileErrors(tesse_eval, "TESSELLATION_EVALUATION");



    }
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometryPath != nullptr)
        glAttachShader(ID, geometry);
    if (tes_ControlPath != nullptr && tes_EvaluationPath != nullptr)
    {
        glAttachShader(ID, tesse_control);
        glAttachShader(ID, tesse_eval);
    }

    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr)
        glDeleteShader(geometry);
    if (tes_ControlPath != nullptr && tes_EvaluationPath != nullptr)
    {
        glDeleteShader(tesse_control);
        glDeleteShader(tesse_eval);
    }


}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::notuse()
{
    glUseProgram(0);
}
void Shader::use()
{
    glUseProgram(0);
    glUseProgram(ID);
}
ui32 Shader::getID()
{
    return ID;
}
void Shader:: setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader:: setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader:: setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader:: setVec3(const std::string& name, const OVec3& vec) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()),vec.m_x,vec.m_y,vec.m_z);
}
void Shader::setVec3(const std::string& name, f32 x,f32 y, f32 z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader:: setVec4(const std::string& name, const OVec4& vec)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()),vec.m_x, vec.m_y, vec.m_z, vec.m_w);
}

//void Shader:: setMat2(const std::string& name, const glm::mat2& mat) const
//{
//    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//}

//void Shader:: setMat3(const std::string& name, const glm::mat3& mat) const
//{
//    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//}

void Shader:: setMat4(const std::string& name, const OMat4& mat) const
{
    
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, (const GLfloat*)mat.m_mat);
 }


void Shader:: checkCompileErrors(ui32 shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
