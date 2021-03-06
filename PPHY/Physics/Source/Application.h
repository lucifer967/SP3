
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <string>
class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();
	static int SceneID;
	static void SetScene(int SceneNo);
	static bool continueGame;
	static bool newGame;
	static std::string levelName;
private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
};

#endif