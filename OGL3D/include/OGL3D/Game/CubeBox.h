#pragma once
#include <memory>
#include <OGL3D/Graphics/OGraphicsEngine.h>
#include <OGL3D/InputSystem/InputListener.h>

#include <OGL3D/Math/OVec2.h>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OVec4.h>

#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/OQuaternion.h>


#include <chrono>
class OGraphicsEngine;
class OWindow;
//class ModelLoader;
class ModelAnimation;
class Animators;
class Animation;

class CubeBox : public InputListener
{
public:
	CubeBox();
	~CubeBox();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onQuit();


	void run();
	void quit();

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
private:


	OMat4 m_world_cam;
	OMat4 m_view_cam;
	OMat4 m_proj_cam;

	OVec3 m_light_pos;

	OVec3 pointLightPositions[4];
protected:
	bool m_isRunning = true;
	std::unique_ptr<OGraphicsEngine> m_graphicsEngine;
	std::unique_ptr<OWindow> m_display;

	OVertexArrayObjectPtr m_polygonVAO;
	OVertexArrayObjectPtr m_cubemapVAO;
	OVertexArrayObjectPtr m_skyboxVAO;

	OVertexArrayObjectPtr m_tessellationVAO;

	OUniformBufferPtr m_uniform;
	//OShaderProgramPtr m_shader;
	
	ShaderPtr m_cubebox_shader;
	ShaderPtr m_skybox_shader;

	ShaderPtr m_shader_prog_light;
	OTexturePtr m_texture_1;
	OTexturePtr m_texture_2;

	OCameraPtr m_camera;

	//OModelPtr m_model;

	OModelAnimationPtr m_anim_model;
	OAnimationPtr m_animation;
	OAnimatorPtr  m_animator;

	const char* model_name;

	std::chrono::system_clock::time_point m_previousTime;
	f32 m_scale = 0;
	bool m_play_state = false;

	f32 m_del;
	f32 lastX;// = SCR_WIDTH / 2.0f;
	f32 lastY;// = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	ui32 m_rez = 0;
	ui32 m_num_patches = 4;
};