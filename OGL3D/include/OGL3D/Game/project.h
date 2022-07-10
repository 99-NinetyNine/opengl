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
class ModelLoader;
class ModelAnimation;
class Animators;
class Animation;

class project : public InputListener
{
public:
	project();
	~project();

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

public:
	void drawAnimationModel(OAnimatorPtr animator);
	void setEssentials(ShaderPtr shader);
	void setFewEssentials(ShaderPtr shader);
	void setRuntimeEssentials(ShaderPtr shader);
	void setTRS(OVec3 tranlate, OVec3 rotate, OVec3 scale);

private:

	OMat4 m_world_cam;
	OMat4 m_view_cam;
	OMat4 m_proj_cam;

	OVec3 m_light_pos;

	std::vector<OVec3> pointLightPositions;
	std::vector<OVec3> birdPositions;
protected:
	bool m_isRunning = true;
	std::unique_ptr<OGraphicsEngine> m_graphicsEngine;
	std::unique_ptr<OWindow> m_display;

	OVertexArrayObjectPtr m_polygonVAO;
	OUniformBufferPtr m_uniform;

	ShaderPtr m_model_shader;
	ShaderPtr m_shader_prog;
	ShaderPtr m_shader_prog_light;
	
	OCameraPtr m_camera;

	OModelPtr m_old_house;
	OModelPtr m_model_airplane;

	OModelAnimationPtr m_tony_anim_model;
	OAnimationPtr m_tony_animation;
	OAnimatorPtr  m_tony_animator;


	OModelAnimationPtr m_bird_anim_model;
	OAnimationPtr m_bird_animation;
	OAnimatorPtr  m_bird_animator;

	OModelAnimationPtr m_rocket_anim_model;
	OAnimationPtr m_rocket_animation;
	OAnimatorPtr  m_rocket_animator;

	const char* model_falcon;
	const char* model_iron;
	const char* model_old_house;
	const char* model_airplane;

	std::chrono::system_clock::time_point m_previousTime;
	f32 m_scale = 0;
	bool m_play_state = false;
	bool m_pause_state = true;

	OVec3 pos_para = OVec3(-33.0, -9.6, 88.0f);
	f32 size_para = 0.01f;
	f32 m_del = 0.0f;
	f32 m_animation_speed = 1.0f;
	f32 lastX;// = SCR_WIDTH / 2.0f;
	f32 lastY;// = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

};

