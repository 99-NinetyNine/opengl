#include <Windows.h>
#include <OGL3D/Game/OGame.h>
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

OGame::OGame()
{
	m_graphicsEngine = std::make_unique<OGraphicsEngine>();
	m_display = std::make_unique<OWindow>();
	m_display->makeCurrentContext();
	m_graphicsEngine->setViewport(m_display->getInnerSize());
	m_graphicsEngine->enable_depth_test();
	InputSystem::create();
}

OGame::~OGame()
{
	//delete m_display;
}

void OGame::onCreate()
{

	InputSystem::get()->addListener(this);

	m_play_state = true;
	InputSystem::get()->showCursor(false);

	
	//m_uniform = m_graphicsEngine->createUniformBuffer({
		//sizeof(UniformData)
		//});

	/*m_shader = m_graphicsEngine->createShaderProgram(
		{
			L"Assets/Shaders/BasicShader.vert",
			L"Assets/Shaders/BasicShader.frag"
		});*/
	//m_shader->setUniformBufferSlot("UniformData", 0);
	//m_graphicsEngine->setShaderProgram(m_shader);


	m_shader_prog = m_graphicsEngine->initializeShader(
		{
			"Assets/Shaders/BasicShader.vert",
			"Assets/Shaders/SimpleShader.frag",
			nullptr,
		});
	
	

	// Load in a model
	m_model = m_graphicsEngine->createModel("D:/5th sem/graph/project/OpenGLGame/Assets/models/guitar/backpack.obj");
	
	m_camera=m_graphicsEngine->createCamera();
	
	ORect rc = m_display->getInnerSize();

	m_proj_cam.setIdentity();
	m_proj_cam.setPerspectiveFovLH(OMath::radians(m_camera->Zoom), (float)rc.width / (float)rc.height, 0.1f, 100.0f);

	m_shader_prog->use();
	m_shader_prog->setMat4("projection", m_proj_cam);

	//m_shader_prog_light->use();
	//m_shader_prog_light->setMat4("projection", m_proj_cam);


	// camera
	

}

void OGame::onUpdate()
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
	/*

	//UniformData data = { currentScale };
	//m_uniform->setData(&data);

	
	//m_shader_prog->use();
	//m_shader_prog->setVec3("viewPos", m_camera->Position);
	//m_shader_prog->setFloat("material.shininess", 32.0f);

	//m_shader_prog->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//m_shader_prog->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//m_shader_prog->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//m_shader_prog->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	//// point light 1
	//m_shader_prog->setVec3("pointLights[0].position", pointLightPositions[0]);
	//m_shader_prog->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	//m_shader_prog->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	//m_shader_prog->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	//m_shader_prog->setFloat("pointLights[0].constant", 1.0f);
	//m_shader_prog->setFloat("pointLights[0].linear", 0.09f);
	//m_shader_prog->setFloat("pointLights[0].quadratic", 0.032f);
	//// point light 2
	//m_shader_prog->setVec3("pointLights[1].position", pointLightPositions[1]);
	//m_shader_prog->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	//m_shader_prog->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	//m_shader_prog->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	//m_shader_prog->setFloat("pointLights[1].constant", 1.0f);
	//m_shader_prog->setFloat("pointLights[1].linear", 0.09f);
	//m_shader_prog->setFloat("pointLights[1].quadratic", 0.032f);
	//// point light 3
	//m_shader_prog->setVec3("pointLights[2].position", pointLightPositions[2]);
	//m_shader_prog->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	//m_shader_prog->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	//m_shader_prog->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	//m_shader_prog->setFloat("pointLights[2].constant", 1.0f);
	//m_shader_prog->setFloat("pointLights[2].linear", 0.09f);
	//m_shader_prog->setFloat("pointLights[2].quadratic", 0.032f);
	//// point light 4
	//m_shader_prog->setVec3("pointLights[3].position", pointLightPositions[3]);
	//m_shader_prog->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	//m_shader_prog->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	//m_shader_prog->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	//m_shader_prog->setFloat("pointLights[3].constant", 1.0f);
	//m_shader_prog->setFloat("pointLights[3].linear", 0.09f);
	//m_shader_prog->setFloat("pointLights[3].quadratic", 0.032f);
	//// spotLight
	//m_shader_prog->setVec3("spotLight.position", m_camera->Position);
	//m_shader_prog->setVec3("spotLight.direction", m_camera->Front);
	//m_shader_prog->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	//m_shader_prog->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	//m_shader_prog->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	//m_shader_prog->setFloat("spotLight.constant", 1.0f);
	//m_shader_prog->setFloat("spotLight.linear", 0.09f);
	//m_shader_prog->setFloat("spotLight.quadratic", 0.032f);
	//m_shader_prog->setFloat("spotLight.cutOff", cos(OMath::radians(12.5f)));
	//m_shader_prog->setFloat("spotLight.outerCutOff", cos(OMath::radians(15.0f)));
	//
	
	m_graphicsEngine->bindTextureUnits(m_texture_1);
	m_graphicsEngine->bindTextureUnits(m_texture_2);
	
	m_view_cam = m_camera->GetViewMatrix();
	m_shader_prog->setMat4("view", m_view_cam);
	
	OMat4 temp;
	//m_graphicsEngine->setUniformBuffer(m_uniform, 0);
	
	m_graphicsEngine->setVertexArrayObject(m_polygonVAO);

	// world space positions of our cubes
	OVec3 cubePositions[] = {
		OVec3(0.0f,  0.0f,  0.0f),
		OVec3(2.0f,  5.0f, -15.0f),
		OVec3(-1.5f, -2.2f, -2.5f),
		OVec3(-3.8f, -2.0f, -12.3f),
		OVec3(2.4f, -0.4f, -3.5f),
		OVec3(-1.7f,  3.0f, -7.5f),
		OVec3(1.3f, -2.0f, -2.5f),
		OVec3(1.5f,  2.0f, -2.5f),
		OVec3(1.5f,  0.2f, -1.5f),
		OVec3(-1.3f,  1.0f, -1.5f)
	};
	
	m_world_cam.setIdentity();
	m_shader_prog->setMat4("model", m_world_cam);
	OMat4 model;
	//m_graphicsEngine->drawTriangles(TriangleStrip, m_polygonVAO->getVertexBufferSize(), 0);
	for (unsigned int i = 0; i < 10; i++)
	{
		model.setIdentity();	
		model.setTranslation(cubePositions[i]);
		float angle = 20.0f * i;
		model.setRotationX(OMath::radians(angle));
		m_shader_prog->setMat4("model", model);
		m_graphicsEngine->drawTriangles(TriangleStrip, m_polygonVAO->getVertexBufferSize(), 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	// also draw the lamp object(s)
	m_shader_prog_light->use();
	
	m_shader_prog_light->setMat4("view", m_view_cam);

	// we now draw as many light bulbs as we have point lights.
	m_graphicsEngine->setLightVertexArrayObject(m_polygonVAO);
	for (unsigned int i = 0; i < 4; i++)
	{
		model.setIdentity();
		model.setTranslation(pointLightPositions[i]);
		model.setScale(OVec3(0.2f, 0.2f,0.2f)); // Make it a smaller cube
		m_shader_prog_light->setMat4("model", model);
		m_graphicsEngine->drawTriangles(TriangleStrip, m_polygonVAO->getVertexBufferSize(), 0);
	}
	*/
	m_shader_prog->use();
	m_view_cam = m_camera->GetViewMatrix();
	m_shader_prog->setMat4("view", m_view_cam);

	
	m_world_cam.setIdentity();
	m_world_cam.setTranslation(OVec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	m_world_cam.setScale(OVec3(1, 1,1.0f));;	// it's a bit too big for our scene, so scale it down

	m_shader_prog->setMat4("model", m_world_cam);
	m_model->Draw(m_shader_prog);
	//m_graphicsEngine->drawModel(m_model,m_shader_prog, m_camera);
	
	m_display->present(false);
}

void OGame::onQuit()
{
}

void OGame::run()
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

void OGame::quit()
{
	m_isRunning = false;
}

void OGame::onKeyDown(int key)
{
	if (!m_play_state) return;

	if (key == 'W')
	{
		m_camera->ProcessKeyboard(FORWARD, m_del);
	}
	else if (key == 'S')
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
}

void OGame::onKeyUp(int key)
{

	if (key == 'G')
	{
		m_play_state = (m_play_state) ? false : true;
		InputSystem::get()->showCursor(!m_play_state);
	}
	//m_camera_speed = 0.0f;
	/*m_forward = 0.0f;
	m_rightward = 0.0f;

	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;
		RECT size_screen = this->getSizeScreen();

		m_swap_chain->setFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
	}*/

}

void OGame::onMouseMove(const Point& mouse_pos)
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

void OGame::onLeftMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 0.5f;
}

void OGame::onLeftMouseUp(const Point& mouse_pos)
{
	//m_scale_cube = 1.0f;
}

void OGame::onRightMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 2.0f;
}

void OGame::onRightMouseUp(const Point& mouse_pos)
{
	//m_scale_cube = 1.0f;
}
