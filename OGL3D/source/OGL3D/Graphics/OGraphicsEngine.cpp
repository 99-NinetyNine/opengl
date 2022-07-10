#include <OGL3D/Graphics/OGraphicsEngine.h>
#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <OGL3D/Graphics/OShaderProgram.h>
#include <OGL3D/Graphics/Shader.h>
#include <OGL3D/Graphics/OUniformBuffer.h>
#include <OGL3D/Graphics/OTexture.h>
#include <OGL3D/Graphics/Camera.h>
#include <OGL3D/Graphics/Mesh.h>
#include <OGL3D/Graphics/ModelLoader.h>

#include <OGL3D/Graphics/ModelAnimation.h>
#include <OGL3D/Graphics/Animation.h>
#include <OGL3D/Graphics/Animators.h>

#include <OGL3D/Math/OVec3.h>
#include <glad/glad_wgl.h>
#include <glad/glad.h>
#include <assert.h>
#include <stdexcept>
OGraphicsEngine::OGraphicsEngine()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"OpenGLDummy";
	wc.lpfnWndProc = DefWindowProc;
    wc.style = CS_OWNDC;

	auto classId = RegisterClassEx(&wc);
	assert(classId);

		
	auto dummyWindow = CreateWindowEx(NULL,MAKEINTATOM(classId), L"", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
	assert(dummyWindow);

	auto dummyDC = GetDC(dummyWindow);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixelFormat = ChoosePixelFormat(dummyDC, &pfd);
    SetPixelFormat(dummyDC, pixelFormat, &pfd);



    HGLRC dummyContext = wglCreateContext(dummyDC);
    assert(dummyContext);

    bool res = wglMakeCurrent(dummyDC, dummyContext);
    assert(res);


    if (!gladLoadWGL(dummyDC))
        throw std::runtime_error("OGraphicsEngine - gladLoadWGL failed");

    if (!gladLoadGL())
        throw std::runtime_error("OGraphicsEngine - gladLoadGL failed");


    wglMakeCurrent(dummyDC, 0);
    wglDeleteContext(dummyContext);
    ReleaseDC(dummyWindow, dummyDC);
    DestroyWindow(dummyWindow);

}

OGraphicsEngine::~OGraphicsEngine()
{
}
OVertexArrayObjectPtr OGraphicsEngine::createVertexArrayObject(const OVertexBufferDesc& data)
{
    return std::make_shared<OVertexArrayObject>(data);
}


OUniformBufferPtr OGraphicsEngine::createUniformBuffer(const OUniformBufferDesc& desc)
{
    return std::make_shared<OUniformBuffer>(desc);
}

OShaderProgramPtr OGraphicsEngine::createShaderProgram(const OShaderProgramDesc& desc)
{
    return std::make_shared<OShaderProgram>(desc);
}
ShaderPtr OGraphicsEngine::initializeShader(const ShaderDesc& desc)
{
    return std::make_shared<Shader>(desc);
}
OCameraPtr OGraphicsEngine::createCamera()
{
    return std::make_shared<Camera>();
}
OModelPtr OGraphicsEngine::createModel(const char* filename)
{
    return std::make_shared<ModelLoader>(filename);
}
OModelAnimationPtr OGraphicsEngine::createAnimationModel(const char* filename)
{
    return std::make_shared<ModelAnimation>(filename,false);
}

OAnimationPtr OGraphicsEngine::createAnimation(const char* filename,OModelAnimationPtr model)
{
    return std::make_shared<Animation>(filename,model);
}

OAnimatorPtr OGraphicsEngine::createAnimator(OAnimationPtr anim)
{
    return std::make_shared<Animators>(anim);
}


void OGraphicsEngine::drawModel(OModelPtr m,ShaderPtr shader_ptr, OCameraPtr camera_ptr)
{
    // Go over all meshes and draw each one
    
}

void OGraphicsEngine::putpixel()
{
    glPointSize(10);
    glDrawArrays(GL_POINTS, 0, 1);

}


OTexturePtr OGraphicsEngine::createTexture(const OTextureDesc& desc)
{
    return std::make_shared<OTexture>(desc);
}

void OGraphicsEngine::clear(const OVec4 & color)
    {
        glClearColor(color.m_x, color.m_y, color.m_z, color.m_w);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
}

void OGraphicsEngine::enable_depth_test()
{
    glEnable(GL_DEPTH_TEST);
}

void OGraphicsEngine::enable_tlation()
{
    glPatchParameteri(GL_PATCH_VERTICES, 4);
}

void OGraphicsEngine::enable_wireframe()

{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void OGraphicsEngine::depthMask(bool mask)
{
    if(mask)
        glDepthFunc(GL_LEQUAL);
    else
        glDepthFunc(GL_LESS);
}

void OGraphicsEngine::setViewport(const ORect& size)
{
    glViewport(size.left, size.top, size.width, size.height);
}

void OGraphicsEngine::setVertexArrayObject(const OVertexArrayObjectPtr& vao)
{
    glBindVertexArray(vao->getId());
}

void OGraphicsEngine::setLightVertexArrayObject(const OVertexArrayObjectPtr& vao)
{
    glBindVertexArray(vao->getLightId());
}

void OGraphicsEngine::setUniformBuffer(const OUniformBufferPtr& buffer, ui32 slot)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer->getId());
}

void OGraphicsEngine::activeTexture()
{
    glActiveTexture(GL_TEXTURE0);
}

void OGraphicsEngine::bindTextureUnits(const OTexturePtr& texture)
{
    if(texture->getCount()==0)
    {
        glActiveTexture(GL_TEXTURE0);
    }
    else
    {
        glActiveTexture(GL_TEXTURE1);
    }
    glBindTexture(GL_TEXTURE_2D, texture->getId());
}

void OGraphicsEngine::setShaderProgram(const OShaderProgramPtr& program)
{
    glUseProgram(program->getId());
}

void OGraphicsEngine::drawTriangles(const OTriangleType& triangleType, ui32 vertexCount, ui32 offset)
{
    auto glTriType = GL_TRIANGLES;

    switch (triangleType)
    {
    case TriangleList: { glTriType = GL_TRIANGLES; break; }
    case TriangleStrip: { glTriType = GL_TRIANGLE_STRIP; break; }
    }
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawArrays(glTriType, offset, vertexCount);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}



void OGraphicsEngine::drawTessellation(ui32 VAO, ui32 size)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_PATCHES, 0, size);
}
