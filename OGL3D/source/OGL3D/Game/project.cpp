#include <Windows.h>
#include <OGL3D/Game/project.h>
#include <OGL3D/Window/OWindow.h>
#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <OGL3D/Graphics/OShaderProgram.h>
#include <OGL3D/Graphics/OUniformBuffer.h>
#include <OGL3D/Graphics/OTexture.h>
#include <OGL3D/Graphics/Shader.h>
#include <OGL3D/Math/MathUtils.h>
#include <OGL3D/InputSystem/InputSystem.h>
#include <OGL3D/Graphics/Camera.h>
#include <OGL3D/Graphics/ModelLoader.h>
#include <OGL3D/Graphics/ModelAnimation.h>
#include <OGL3D/Graphics/Animation.h>
#include <OGL3D/Graphics/Animators.h>


struct UniformData
{
	f32 scale;
	OVec3 lightPos;
};
struct CameraDesc
{
	OVec3 position;
	OVec3 up;
};

project::project()
{
	m_graphicsEngine = std::make_unique<OGraphicsEngine>();
	m_display = std::make_unique<OWindow>();
	m_display->makeCurrentContext();
	m_graphicsEngine->setViewport(m_display->getInnerSize());
	m_graphicsEngine->enable_depth_test();
	InputSystem::create();

}

project::~project()
{
	//delete m_display;
}

void project::onCreate()
{

	InputSystem::get()->addListener(this);

	m_play_state = true;
	InputSystem::get()->showCursor(false);
	//animated models
	model_falcon = "D:/5th sem/graph/project/OpenGLGame/Assets/Models/rock/source1/rock.fbx";
	model_iron = "D:/5th sem/graph/project/OpenGLGame/Assets/Models/iron/source/iron_man.fbx";
	model_old_house = "D:/5th sem/graph/project/OpenGLGame/Assets/Models/home/abandoned-house-low-poly/source/old/H.obj";
	model_airplane = "D:/5th sem/graph/project/OpenGLGame/Assets/Models/rock/proj/source/project.obj";;

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	OVertexAttribute attr[] = { 3 };
	m_polygonVAO = m_graphicsEngine->createVertexArrayObject({
		(void*)vertices,
		nullptr,
		12,
		3 * 36 * 4,
		0,
		attr,
		1,
		});


	// positions of the point lights
	pointLightPositions.push_back(OVec3(0.0f, 2.0f, 2.5f));
	pointLightPositions.push_back(OVec3(50.0f, -05.0f, 4.0f));
	pointLightPositions.push_back(OVec3(-50.0f, -40.0f, 6.0f));
	pointLightPositions.push_back(OVec3(0.0f, -80.0f, 7.0f));


	birdPositions.push_back(OVec3(3.5f, 5.2f, 3.0f));
	birdPositions.push_back(OVec3(2.5f, 7.3f, 2.0f));
	birdPositions.push_back(OVec3(-3.0f, 9.0f, 4.0f));
	birdPositions.push_back(OVec3(-4.4f, 2.0f, 5.0f));

	birdPositions.push_back(OVec3(4.5f, 7.2f, 2.0f));
	birdPositions.push_back(OVec3(5.5f, 5.3f, 5.0f));
	birdPositions.push_back(OVec3(-5.5f, 9.0f, 4.0f));
	birdPositions.push_back(OVec3(-3.5f, 10.0f, 0.0f));


	m_shader_prog = m_graphicsEngine->initializeShader(
		{
			"Assets/Shaders/ProjectAnimation/AnimationVS.vert",
			"Assets/Shaders/ProjectAnimation/AnimationPS.frag",
			nullptr,
		});

	m_model_shader = m_graphicsEngine->initializeShader(
		{
			"Assets/Shaders/ModelShader/BasicShader.vert",
			"Assets/Shaders/ModelShader/BasicShader.frag",
			nullptr,
		});
	m_shader_prog_light = m_graphicsEngine->initializeShader(
		{
			"Assets/Shaders/ProjectShader/VS.vert",
			"Assets/Shaders/ProjectShader/PS.frag",
			nullptr,
		});


	m_rocket_anim_model = m_graphicsEngine->createAnimationModel(model_falcon);
	m_rocket_animation = m_graphicsEngine->createAnimation(model_falcon, m_rocket_anim_model);
	m_rocket_animator = m_graphicsEngine->createAnimator(m_rocket_animation);



	m_tony_anim_model = m_graphicsEngine->createAnimationModel(model_iron);
	m_tony_animation = m_graphicsEngine->createAnimation(model_iron, m_tony_anim_model);
	m_tony_animator = m_graphicsEngine->createAnimator(m_tony_animation);
	m_old_house = m_graphicsEngine->createModel(model_old_house);
	m_model_airplane = m_graphicsEngine->createModel(model_airplane);


	ORect rc = m_display->getInnerSize();
	m_camera = m_graphicsEngine->createCamera();

	m_proj_cam.setIdentity();
	m_proj_cam.setPerspectiveFovLH(OMath::radians(m_camera->Zoom), (float)rc.width / (float)rc.height, 0.1f, 100.0f);

	setEssentials(m_shader_prog);
	setEssentials(m_model_shader);
	setFewEssentials(m_shader_prog_light);

}

void project::onUpdate()
{
	InputSystem::get()->update();

	auto currentTime = std::chrono::system_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>();
	if (m_previousTime.time_since_epoch().count())
		elapsedSeconds = currentTime - m_previousTime;
	m_previousTime = currentTime;

	auto deltaTime = (f32)elapsedSeconds.count();
	m_del = deltaTime;


	m_graphicsEngine->clear(OVec4(0.4, 0.2, 0.1, 1));

	if (!m_pause_state)
	{
		m_tony_animator->UpdateAnimation(m_del * m_animation_speed);
	}



	//draw lights
	// we now draw as many light bulbs as we have point lights.
	m_polygonVAO->BindVAO();

	for (unsigned int i = 0; i < 4; i++)
	{
		m_world_cam.setIdentity();
		m_world_cam.setTranslation(pointLightPositions[i]);
		OVec3 scale =OVec3(0.2f);
		m_world_cam.setScale(scale); // Make it a smaller cube
		m_world_cam.setScale(scale); // Make it a smaller cube
		setRuntimeEssentials(m_shader_prog_light);
		m_graphicsEngine->drawTriangles(TriangleStrip, 36, 0);
	}
	m_polygonVAO->UnbindVAO();
	m_shader_prog_light->notuse();


	setTRS(OVec3(0.0f, 0.0f, 0.0f), OVec3(0.f, 0, .0f), OVec3(0.001f));
	setRuntimeEssentials(m_shader_prog);
	m_tony_anim_model->Draw(m_shader_prog);


	for (int i = 0;i < 20;i++)
	{
		int j = -i;
		setTRS(OVec3(-150 + (i % 5) * 50, -19.6, 5 * (i * i)), OVec3(.0, -45.0f, .0f), OVec3(0.5f));
		setRuntimeEssentials(m_model_shader);
		m_old_house->Draw(m_model_shader);
	}

	setTRS(OVec3(-30.0f, -60.0f, -10.0f), OVec3(-90.f, 0, .0f), OVec3(1.0f));
	setRuntimeEssentials(m_shader_prog);
	m_rocket_anim_model->Draw(m_shader_prog);

	for (int i = 0;i < 20;i++)
	{
		int j = -i;
		setTRS(OVec3(90 + (i % 5) * 90, -19.6, 5 * (-50 + (i * i) % 5)), OVec3(.0, 30.0f, .0f), OVec3(0.5f));
		setRuntimeEssentials(m_model_shader);
		m_old_house->Draw(m_model_shader);
	}


	pos_para.m_y += 2.0f;
	if (pos_para.m_y >= 150.0f)
	{
		pos_para.m_y = -20.0f;
	}
	if (pos_para.m_y <= 35.0f && pos_para.m_y >= -9.0)
	{
		setTRS(OVec3(pos_para.m_x, pos_para.m_y, pos_para.m_z), 0, OVec3(0.7f));
		setRuntimeEssentials(m_model_shader);
		m_model_airplane->Draw(m_model_shader);
	}


	m_display->present(false);
}

void project::drawAnimationModel(OAnimatorPtr animator)
{
	auto transforms = animator->GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
		m_shader_prog->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

}

void project::setEssentials(ShaderPtr shader)
{
	shader->use();
	shader->setMat4("projection", m_proj_cam);

	float s = 0.0f;
	shader->setVec3("dirLight.direction", -0.2f, 1.0f, 0.3f);
	shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	shader->setVec3("pointLights[0].position", pointLightPositions[0]);
	shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("pointLights[0].constant", 1.0f);
	shader->setFloat("pointLights[0].linear", s);
	shader->setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	shader->setVec3("pointLights[1].position", pointLightPositions[1]);
	shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("pointLights[1].constant", 1.0f);
	shader->setFloat("pointLights[1].linear", s);
	shader->setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	shader->setVec3("pointLights[2].position", pointLightPositions[2]);
	shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("pointLights[2].constant", 1.0f);
	shader->setFloat("pointLights[2].linear", 0.9f);
	shader->setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	shader->setVec3("pointLights[3].position", pointLightPositions[3]);
	shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("pointLights[3].constant", 1.0f);
	shader->setFloat("pointLights[3].linear", s);
	shader->setFloat("pointLights[3].quadratic", 0.032f);
	// spotLight

	shader->setVec3("spotLight.ambient", 1.0f, 1.0f, 1.0f);
	shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("spotLight.constant", 1.0f);
	shader->setFloat("spotLight.linear", s);
	shader->setFloat("spotLight.quadratic", 0.032f);
	shader->setFloat("spotLight.cutOff", static_cast<float>(cos(OMath::radians(12.5f))));
	shader->setFloat("spotLight.outerCutOff", static_cast<float>(cos(OMath::radians(15.0f))));

	shader->setFloat("material.shininess", 32.0f);
	shader->notuse();


}

void project::setFewEssentials(ShaderPtr shader)
{
	shader->use();
	shader->setMat4("projection", m_proj_cam);
	shader->notuse();
}

void project::setRuntimeEssentials(ShaderPtr shader)
{
	shader->use();
	shader->setVec3("viewPos", m_camera->Front);


	m_view_cam = m_camera->GetViewMatrix();
	shader->setMat4("model", m_world_cam);
	shader->setMat4("view", m_view_cam);


	shader->setVec3("spotLight.position", m_camera->Position);
	shader->setVec3("spotLight.direction", m_camera->Front);
	//shader->notuse();
}

void project::setTRS(OVec3 tranlate, OVec3 rotate, OVec3 scale)
{
	m_world_cam.setIdentity();

	OMat4 temp;
	temp.setTranslation(tranlate);
	m_world_cam *= temp;


	temp.setIdentity();
	temp.setRotationX(OMath::radians(rotate.m_x));
	m_world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(OMath::radians(rotate.m_y));
	m_world_cam *= temp;

	temp.setIdentity();
	temp.setRotationZ(OMath::radians(rotate.m_z));
	m_world_cam *= temp;

	//0.0025 for tony
	temp.setIdentity();
	temp.setScale(scale);
	m_world_cam *= temp;

}





void project::onQuit()
{
	std::cout << pos_para.m_x << ",\t" << pos_para.m_y << ",\t" << pos_para.m_z << "\n";
}

void project::run()
{
	onCreate();
	while (m_isRunning)
	{
		MSG msg = {};
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
			{
				m_isRunning = false;
				continue;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		onUpdate();

	}
	onQuit();
}

void project::quit()
{

	m_isRunning = false;
}

void project::onKeyDown(int key)
{
	if (!m_play_state) return;

	if (key == 'W' || key == VK_UP)
	{
		m_camera->ProcessKeyboard(FORWARD, m_del);
	}
	else if (key == 'S' || key == VK_DOWN)
	{
		m_camera->ProcessKeyboard(BACKWARD, m_del);
	}
	else if (key == 'A')
	{
		m_camera->ProcessKeyboard(LEFT, m_del);
	}

	else if (key == 'D')
	{
		m_camera->ProcessKeyboard(RIGHT, m_del);
	}

	else if (key == 'R')
	{
		m_camera->ProcessKeyboard(RIGHT, m_del);
	}
	else if (key == 'T')
	{
		m_camera->ProcessKeyboard(RIGHT, m_del);
	}

	else if (key == VK_LEFT)
	{
		m_camera->ProcessMouseMovement(-20.0f, 0.0f);
	}
	else if (key == VK_RIGHT)
	{
		m_camera->ProcessMouseMovement(+20.0f, 0.0f);
	}


	else if (key == 'I')
	{//slow
		m_animation_speed = m_animation_speed - 0.1;
		if (m_animation_speed < 0.0f)
		{
			m_animation_speed = 0.1f;
		}
	}
	else if (key == 'O')
	{
		//fast
		m_animation_speed = m_animation_speed + 0.1;
		if (m_animation_speed >= 20.0f)
		{
			m_animation_speed = 20.0f;
		}
	}
}

void project::onKeyUp(int key)
{

	if (key == 'G')
	{
		m_play_state = (m_play_state) ? false : true;
		InputSystem::get()->showCursor(!m_play_state);
	}
	else if (key == VK_SPACE)
	{
		m_pause_state = (m_pause_state) ? false : true;
	}

}

void project::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;

	ORect rc = m_display->getInnerSize();
	int width = rc.width;
	int height = rc.height;

	f32 x_off = (mouse_pos.m_x - (width / 2.0f));
	f32 y_off = (mouse_pos.m_y - (height / 2.0f));

	m_camera->ProcessMouseMovement(x_off, y_off);

	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void project::onLeftMouseDown(const Point& mouse_pos)
{

}

void project::onLeftMouseUp(const Point& mouse_pos)
{

}

void project::onRightMouseDown(const Point& mouse_pos)
{

}

void project::onRightMouseUp(const Point& mouse_pos)
{

}
