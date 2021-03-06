#include "Loading.h"
#include "GL\glew.h"
#include "Application.h"
#include "SceneBase.h"
#include <string>
#include <sstream>
#include <fstream>

SceneLoading::SceneLoading()
{
}

SceneLoading::~SceneLoading()
{
}

void SceneLoading::Init()
{
	SceneBase::Init();
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	Math::InitRNG();

	clickpos = 0;
	c_bounceTime = 0;
	swit = 0;

	x1 = m_worldWidth * 0.5;
	x2 = m_worldWidth;

	loadtimer = 0;

}

void SceneLoading::Update(double dt)
{
	SceneBase::Update(dt);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	loadtimer += dt;

	if (loadtimer >= 2)
	{
		if (Application::newGame)
		{
			Application::newGame = false;
			Application::SetScene(2);
		}
		else
		Application::SetScene(1);
	}


	//choose
	if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && c_bounceTime >= 10)
	{
		clickpos--;
		c_bounceTime = 0;
	}
	if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && c_bounceTime >= 10)
	{
		clickpos++;
		c_bounceTime = 0;
	}

	//Lock
	if (clickpos <= 0)
		clickpos = 0;
	if (clickpos >= 5)
		clickpos = 5;

	//Selection
	if (Application::IsKeyPressed(VK_RETURN) && c_bounceTime >= 10)
	{
		if (clickpos == 0)		//Play
		{
			Application::continueGame = false;
			Application::levelName = "level1.csv";
			Application::SetScene(2);
		}

		if (clickpos == 1)
		{
			Application::continueGame = false;
			Application::levelName = "level3.csv";
			Application::SetScene(2);
		}
		if (clickpos == 2)
			Application::SetScene(8);
		if (clickpos == 3)
		{
			std::ifstream file;
			file.open("save.csv");
			file >> Application::SceneID;
			file.close();
			Application::continueGame = true;
			Application::SetScene(Application::SceneID);
		}
		if (clickpos == 4)
			Application::SetScene(0);
		//Application::SetScene(0);
	}

	x1 -= dt;
	x2 -= dt;

	if (x1 < -m_worldWidth*0.5f)
	{
		x1 = m_worldWidth;
	}

	if (x2 < -m_worldWidth*0.5f)
	{
		x2 = m_worldWidth;
	}

	c_bounceTime++;
}

void SceneLoading::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(x1, m_worldHeight*0.4, -3);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(MeshBuilder::GetInstance()->GetMesh("bgd_0"), false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(x2 + 66, m_worldHeight*0.4, -3);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(MeshBuilder::GetInstance()->GetMesh("bgd_1"), false);
	modelStack.PopMatrix();

	//On screen text
	//Exercise 5a: Render m_lives, m_score

	std::ostringstream ss;

	//Selection
	ss.str("");
	ss << "Now Loading";
		RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), ss.str(), Color(0.5, 0.7, 0.5), 3, 25, 34);

}

void SceneLoading::Exit()
{
	SceneBase::Exit();
}
