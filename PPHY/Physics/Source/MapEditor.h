#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "csv.h"
#include <fstream>
#include<ostream>
#include <math.h>
	
using std::vector;

class MapEditor : public SceneBase
{

public:
	MapEditor();
	~MapEditor();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	string fileName;

protected:

	//Physics
	std::vector<GameObject *> m_goList;
	GameObject *m_ghost;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	float saveTime;

	int m_objectCount;
	bool saveSuccesfull;



	vector<string>ObjData;

	void LoadObjects(vector<string> data);
	void SaveFile(vector<GameObject*> List);

};

#endif