#include <Windows.h>

#include <OGL3D/Game/CubeBox.h>

#include <OGL3D/Window/OWindow.h>
#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <OGL3D/Graphics/OShaderProgram.h>
#include <OGL3D/Graphics/OUniformBuffer.h>
#include <OGL3D/Graphics/OTexture.h>
#include <OGL3D/Graphics/Shader.h>
#include <OGL3D/Math/MathUtils.h>
#include <OGL3D/InputSystem/InputSystem.h>
#include <OGL3D/Graphics/Camera.h>
//#include <OGL3D/Graphics/ModelLoader.h>
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

CubeBox::CubeBox()
{
	m_graphicsEngine = std::make_unique<OGraphicsEngine>();
	m_display = std::make_unique<OWindow>();
	m_display->makeCurrentContext();
	m_graphicsEngine->setViewport(m_display->getInnerSize());
	m_graphicsEngine->enable_depth_test();
	m_graphicsEngine->enable_tlation();
	//m_graphicsEngine->enable_wireframe();
	InputSystem::create();
}

CubeBox::~CubeBox()
{
	//delete m_display;
}

void CubeBox::onCreate()
{

	InputSystem::get()->addListener(this);

	m_play_state = true;
	InputSystem::get()->showCursor(false);


	m_cubebox_shader = m_graphicsEngine->initializeShader(
		{
			"Assets/Shaders/Skybox/cVS.vert",
			"Assets/Shaders/Skybox/cPS.frag",
		});


	m_skybox_shader = m_graphicsEngine->initializeShader(
		{
			"Assets/Shaders/Skybox/sVS.vert",
			"Assets/Shaders/Skybox/sPS.frag",
		});


	m_texture_1 = m_graphicsEngine->createTexture(
		{
			"Assets/Images/me.jpg",
		});

	m_texture_2 = m_graphicsEngine->createTexture(
		{
			nullptr,
			0,
			nullptr,
			true,
		});

	
	float skyboxVertices[] = {
		// positions          
		-5.0f,  5.0f, -5.0f,
		-5.0f, -5.0f, -5.0f,
		 5.0f, -5.0f, -5.0f,
		 5.0f, -5.0f, -5.0f,
		 5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,

		-5.0f, -5.0f,  5.0f,
		-5.0f, -5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f, -5.0f,
		-5.0f,  5.0f,  5.0f,
		-5.0f, -5.0f,  5.0f,

		 5.0f, -5.0f, -5.0f,
		 5.0f, -5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f, -5.0f,
		 5.0f, -5.0f, -5.0f,

		-5.0f, -5.0f,  5.0f,
		-5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f, -5.0f,  5.0f,
		-5.0f, -5.0f,  5.0f,

		-5.0f,  5.0f, -5.0f,
		 5.0f,  5.0f, -5.0f,
		 5.0f,  5.0f,  5.0f,
		 5.0f,  5.0f,  5.0f,
		-5.0f,  5.0f,  5.0f,
		-5.0f,  5.0f, -5.0f,

		-5.0f, -5.0f, -5.0f,
		-5.0f, -5.0f,  5.0f,
		 5.0f, -5.0f, -5.0f,
		 5.0f, -5.0f, -5.0f,
		-5.0f, -5.0f,  5.0f,
		 5.0f, -5.0f,  5.0f
	};


	OVertexAttribute attr[] = { 3 };
	m_skyboxVAO = m_graphicsEngine->createVertexArrayObject({
			(void*)skyboxVertices,
			nullptr,
			12,
			432,
			0,
			attr,
			1
		});

	m_camera = m_graphicsEngine->createCamera();

	//m_camera->changeYawPitch(-128.1f, -42.4f);

	ORect rc = m_display->getInnerSize();

	m_proj_cam.setIdentity();
	m_proj_cam.setPerspectiveFovLH(OMath::radians(m_camera->Zoom), (float)rc.width / (float)rc.height, 0.1f, 100.0f);

	
	m_skybox_shader->use();
	m_skybox_shader->setInt("skybox", 0);
	m_skybox_shader->setMat4("projection", m_proj_cam);



}

void CubeBox::onUpdate()
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
	
	m_world_cam.setIdentity();
	

	m_graphicsEngine->depthMask(true);
	m_skybox_shader->use();

	
	m_view_cam = m_camera->GetViewMatrix();
	m_view_cam.removeTranslation();

	m_skybox_shader->setMat4("view", m_view_cam);

	// skybox cube
	m_skyboxVAO->BindVAO();
	m_graphicsEngine->activeTexture();
	m_texture_2->bindTexture();
	m_graphicsEngine->drawTriangles(TriangleList, 0, 0);
	m_skyboxVAO->UnbindVAO();
	m_graphicsEngine->depthMask(false);
	

	m_display->present(false);
}

void CubeBox::onQuit()
{
}

void CubeBox::run()
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

void CubeBox::quit()
{
	m_isRunning = false;
}

void CubeBox::onKeyDown(int key)
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

void CubeBox::onKeyUp(int key)
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

void CubeBox::onMouseMove(const Point& mouse_pos)
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

void CubeBox::onLeftMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 0.5f;
}

void CubeBox::onLeftMouseUp(const Point& mouse_pos)
{
	//m_scale_cube = 1.0f;
}

void CubeBox::onRightMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 2.0f;
}

void CubeBox::onRightMouseUp(const Point& mouse_pos)
{
	//m_scale_cube = 1.0f;
}
