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

	model_name = "D:/5th sem/graph/project/OpenGLGame/Assets/Models/rock/rock.fbx";

	float vec[] = {
		0.0f, .0f, 0.0f,  1.0f, 0.2f, 0.0f,  // bottom right
		
	};
	OVertexAttribute attr[] = { 3,3 };
	OVertexBufferDesc desc = {
		(void*)vec,
		nullptr,
		6 * 4,
		6 * 1 * 4,
		0,
		attr,
		2
	};
	m_polygonVAO = m_graphicsEngine->createVertexArrayObject(desc);	

	m_shader_prog = m_graphicsEngine->initializeShader(
		{
			"Assets/Shaders/ProjectShader/VS.vert",
			"Assets/Shaders/ProjectShader/PS.frag",
			nullptr,
		});
	m_shader_prog->use();

	

	// Load in a model
	//m_model = m_graphicsEngine->createModel("D:/5th sem/graph/project/OpenGLGame/Assets/models/guitar/backpack.obj");
	

	m_camera = m_graphicsEngine->createCamera();

	ORect rc = m_display->getInnerSize();

	//m_proj_cam.setIdentity();
	//m_proj_cam.setPerspectiveFovLH(OMath::radians(m_camera->Zoom), (float)rc.width / (float)rc.height, 0.1f, 100.0f);

	//m_shader_prog->use();
	//m_shader_prog->setMat4("projection", m_proj_cam);



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

	m_polygonVAO->BindVAO();
	m_graphicsEngine->putpixel();
	

	



	m_display->present(false);
}

void project::onQuit()
{
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

void project::onKeyUp(int key)
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
	//m_scale_cube = 0.5f;
}

void project::onLeftMouseUp(const Point& mouse_pos)
{
	//m_scale_cube = 1.0f;
}

void project::onRightMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 2.0f;
}

void project::onRightMouseUp(const Point& mouse_pos)
{
	//m_scale_cube = 1.0f;
}
