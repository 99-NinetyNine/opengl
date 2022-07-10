#include <Windows.h>

#include <OGL3D/Game/Tesseletation.h>

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

Tesseletation::Tesseletation()	
{
	m_graphicsEngine = std::make_unique<OGraphicsEngine>();
	m_display = std::make_unique<OWindow>();
	m_display->makeCurrentContext();
	m_graphicsEngine->setViewport(m_display->getInnerSize());
	m_graphicsEngine->enable_depth_test();
	m_graphicsEngine->enable_tlation();
	m_graphicsEngine->enable_wireframe();
	InputSystem::create();
}

Tesseletation::~Tesseletation()
{
	//delete m_display;
}

void Tesseletation::onCreate()
{

	InputSystem::get()->addListener(this);

	m_play_state = true;
	InputSystem::get()->showCursor(false);


	m_shader_prog = m_graphicsEngine->initializeShader(
		{
			"Assets/Shaders/Tessellation/VS.vert",
			"Assets/Shaders/Tessellation/PS.frag",
			nullptr,
			"Assets/Shaders/Tessellation/TessellationControlShader.tcs",
			"Assets/Shaders/Tessellation/TessellationEvaluationShader.tes",
		});

	m_texture_1 = m_graphicsEngine->createTexture(
		{
			"Assets/Images/height.png",
			0,
			nullptr,
			
		});
	m_shader_prog->setInt("heightMap", 0);

	std::vector<float> vertices;
	int width, height;

	TextureSizeDesc desc = m_texture_1->getTextureSizeDesc();
	width = desc.width;
	height = desc.height;

	unsigned rez = 20;
	m_rez = rez;
	for (unsigned i = 0; i <= rez - 1; i++)
	{
		for (unsigned j = 0; j <= rez - 1; j++)
		{
			vertices.push_back(-width / 2.0f + width * i / (float)rez); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * j / (float)rez); // v.z
			vertices.push_back(i / (float)rez); // u
			vertices.push_back(j / (float)rez); // v

			vertices.push_back(-width / 2.0f + width * (i + 1) / (float)rez); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * j / (float)rez); // v.z
			vertices.push_back((i + 1) / (float)rez); // u
			vertices.push_back(j / (float)rez); // v

			vertices.push_back(-width / 2.0f + width * i / (float)rez); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * (j + 1) / (float)rez); // v.z
			vertices.push_back(i / (float)rez); // u
			vertices.push_back((j + 1) / (float)rez); // v

			vertices.push_back(-width / 2.0f + width * (i + 1) / (float)rez); // v.x
			vertices.push_back(0.0f); // v.y
			vertices.push_back(-height / 2.0f + height * (j + 1) / (float)rez); // v.z
			vertices.push_back((i + 1) / (float)rez); // u
			vertices.push_back((j + 1) / (float)rez); // v
		}

	}
	

	OVertexAttribute attr[] = { 3,2 };
	m_tessellationVAO = m_graphicsEngine->createVertexArrayObject(
		{
			(void*)vertices.data(),
			nullptr,
			4*5,
			static_cast < int>(sizeof(f32) * (vertices.size())),
			0,
			attr,
			2,
			true,
			m_num_patches,

		});
	


	m_camera = m_graphicsEngine->createCamera();
	m_camera->changePitchYaw(-128.1f, -42.4f);

	ORect rc = m_display->getInnerSize();

	m_proj_cam.setIdentity();
	m_proj_cam.setPerspectiveFovLH(OMath::radians(m_camera->Zoom), (float)rc.width / (float)rc.height, 0.1f, 100000.0f);

	m_shader_prog->use();
	m_shader_prog->setMat4("projection", m_proj_cam);

	
}

void Tesseletation::onUpdate()
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

	m_shader_prog->use();
	m_view_cam = m_camera->GetViewMatrix();
	m_shader_prog->setMat4("view", m_view_cam);



	// render the loaded model
	m_world_cam.setIdentity();
	m_shader_prog->setMat4("model", m_world_cam);
	
	m_graphicsEngine->drawTessellation(m_tessellationVAO->getId(), m_num_patches *m_rez*m_rez); 

	m_display->present(false);
}

void Tesseletation::onQuit()
{
}

void Tesseletation::run()
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

void Tesseletation::quit()
{
	m_isRunning = false;
}

void Tesseletation::onKeyDown(int key)
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

void Tesseletation::onKeyUp(int key)
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

void Tesseletation::onMouseMove(const Point& mouse_pos)
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

void Tesseletation::onLeftMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 0.5f;
}

void Tesseletation::onLeftMouseUp(const Point& mouse_pos)
{
	//m_scale_cube = 1.0f;
}

void Tesseletation::onRightMouseDown(const Point& mouse_pos)
{
	//m_scale_cube = 2.0f;
}

void Tesseletation::onRightMouseUp(const Point& mouse_pos)
{
	//m_scale_cube = 1.0f;
}
