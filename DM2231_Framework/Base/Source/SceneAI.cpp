#include "SceneAI.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

int SceneAI::tableOffset = 50;

SceneAI::SceneAI()
{
}

SceneAI::~SceneAI()
{
}

void SceneAI::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//comg.vertexshader", "Shader//MultiTexture.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");

	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 100, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 100);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Diner//Floor.tga");
	meshList[GEO_DISHES] = MeshBuilder::GenerateSphere("Dishes", Color(0.5, 0.5, 0.5), 18, 36);

	meshList[GEO_FIRE_SPRITE] = MeshBuilder::GenerateSpriteAnimation("fire", 2, 4);
	meshList[GEO_FIRE_SPRITE]->textureArray[0] = LoadTGA("Image//fire.tga");
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation *> (meshList[GEO_FIRE_SPRITE]);
	sa->m_anim->Set(0, 7, 0, 1.f);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(75.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	camera.Init(Vector3(50, 50, 50), Vector3(0, 0, 0), Vector3(0, 1, 0));

	bLightEnabled = true;
	elapsedTime = 0;

	//Initialize Waitress
	Waitress* waitress = new Waitress();
	waitress->setMesh(MeshBuilder::GenerateSphere("Waitress", Color(1, 0, 0), 18, 36));
	waitress->setPos(Vector3(400, 170, 0));
	waitress->setScale(Vector3(15, 15, 0));
	waitress->initPos = Vector3(400, 170, 0);
	m_cGOList.push_back(waitress);

	//Initialize Chef
	Chef* chef = new Chef();
	chef->setMesh(MeshBuilder::GenerateSphere("Chef", Color(0, 0, 1), 18, 36));
	chef->setPos(Vector3(300, 550, 2));
	chef->setScale(Vector3(15, 15, 0));
	m_cGOList.push_back(chef);

	for (unsigned a = 0; a < 6; ++a)
	{
		//Initialize Customer
		Customer* customer = new Customer();
		customer->setScale(Vector3(15, 15, 0));
		customer->setMesh(MeshBuilder::GenerateSphere("Customer", Color(0, 1, 0), 18, 36));
		customer->setActive(false);
		customer->id = a + 1;
		m_cGOList.push_back(customer);

		//Initialize Queue
		static int diff = 40;
		queue[a] = new s_OBJPOS();
		queue[a]->pos.x = 25;
		queue[a]->pos.y = 210 + (a * diff);
	}

	//Initialize Table
	table[0] = new s_OBJPOS();
	table[0]->pos = Vector3(200, 275, 0);
	table[0]->id = 1;
	table[1] = new s_OBJPOS();
	table[1]->pos = Vector3(200, 75, 0);
	table[1]->id = 2;
	table[2] = new s_OBJPOS();
	table[2]->pos = Vector3(400, 275, 0);
	table[2]->id = 3;
	table[3] = new s_OBJPOS();
	table[3]->pos = Vector3(400, 75, 0);
	table[3]->id = 4;
	table[4] = new s_OBJPOS();
	table[4]->pos = Vector3(600, 275, 0);
	table[4]->id = 5;
	table[5] = new s_OBJPOS();
	table[5]->pos = Vector3(600, 75, 0);
	table[5]->id = 6;

	//Initialize Order
	order[0] = new ORDER();
	order[0]->id = 1;
	order[0]->counterPos = Vector3(280, 505, 2);
	order[1] = new ORDER();
	order[1]->id = 2;
	order[1]->counterPos = Vector3(310, 505, 2);
	order[2] = new ORDER();
	order[2]->id = 3;
	order[2]->counterPos = Vector3(340, 505, 2);
	order[3] = new ORDER();
	order[3]->id = 4;
	order[3]->counterPos = Vector3(370, 505, 2);
	order[4] = new ORDER();
	order[4]->id = 5;
	order[4]->counterPos = Vector3(400, 505, 2);
	order[5] = new ORDER();
	order[5]->id = 6;
	order[5]->counterPos = Vector3(430, 505, 2);

	static float size = 10;
	//===============Dishes pos=============//
	/*CWorldOBJ* newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateSphere("Dishes", Color(0.5, 1, 0.5), 18, 36));
	newObj->setPos(Vector3(280, 505, 1));
	newObj->setScale(Vector3(10, 10, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateSphere("Dishes", Color(0.5, 1, 0.5), 18, 36));
	newObj->setPos(Vector3(310, 505, 1));
	newObj->setScale(Vector3(10, 10, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateSphere("Dishes", Color(0.5, 1, 0.5), 18, 36));
	newObj->setPos(Vector3(340, 505, 1));
	newObj->setScale(Vector3(10, 10, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateSphere("Dishes", Color(0.5, 1, 0.5), 18, 36));
	newObj->setPos(Vector3(370, 505, 1));
	newObj->setScale(Vector3(10, 10, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateSphere("Dishes", Color(0.5, 1, 0.5), 18, 36));
	newObj->setPos(Vector3(400, 505, 1));
	newObj->setScale(Vector3(10, 10, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateSphere("Dishes", Color(0.5, 1, 0.5), 18, 36));
	newObj->setPos(Vector3(430, 505, 1));
	newObj->setScale(Vector3(10, 10, 0));
	m_cGOList.push_back(newObj);*/

	//===============TABLE=============//
	CWorldOBJ* newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Table", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Table.tga");
	newObj->setPos(Vector3(200, 275, 0));
	newObj->setScale(Vector3(60, 60, 10));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Table", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Table.tga");
	newObj->setPos(Vector3(400, 275, 0));
	newObj->setScale(Vector3(60, 60, 10));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Table", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Table.tga");
	newObj->setPos(Vector3(600, 275, 0));
	newObj->setScale(Vector3(60, 60, 10));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Table", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Table.tga");
	newObj->setPos(Vector3(400, 75, 0));
	newObj->setScale(Vector3(60, 60, 10));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Table", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Table.tga");
	newObj->setPos(Vector3(200, 75, 0));
	newObj->setScale(Vector3(60, 60, 10));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Table", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Table.tga");
	newObj->setPos(Vector3(600, 75, 0));
	newObj->setScale(Vector3(60, 60, 10));
	m_cGOList.push_back(newObj);

	//===============BARRIER=============//
	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Barrier.tga");
	newObj->setPos(Vector3(65, 400, 0));
	newObj->setScale(Vector3(30, 400, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(200, 150, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(200, 450, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(600, 150, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(600, 450, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Kitchen.tga");
	newObj->setPos(Vector3(400, 535, 1));
	newObj->setScale(Vector3(325, 325, size));
	m_cGOList.push_back(newObj);
}

void SceneAI::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	//Spawn Customer
	if (getActiveCustomer() < 6)
	{
		static double respawnTime = 2.0;
		static double timeToRespawn = 0.0;
		static double probabillity = 75;

		if (timeToRespawn < respawnTime)
		{
			timeToRespawn += dt;
		}
		else
		{
			int numGenerated = Math::RandFloatMinMax(1, 100);
			if (numGenerated >= probabillity) //25% to spawn customer
			{
				SpawnCustomer();
			}

			timeToRespawn = 0;
		}
	}

	//Unspawn Customer
	static float time2 = 0.f;
	static float timeLimit2 = 0.5f;

	if (time2 < timeLimit2)
	{
		time2 += dt;
	}
	else
	{
		if (Application::IsKeyPressed('E'))
		{
			for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
			{
				Customer* customer = dynamic_cast<Customer*>(*it);
				s_OBJPOS* queuePos = fetchQueue();
				if (customer != NULL && queuePos != NULL)
				{
					if (queuePos->pos == customer->pos && customer->isQueing())
					{
						queuePos->taken = false;
						customer->setActive(false);
						break;
					}
				}
			}
			time2 = 0.f;
		}
	}

	fps = (float)(1.f / dt);
	elapsedTime += dt;

	CustomerUpdate(dt);
	WaitressUpdate(dt);
	ChefUpdate(dt);

	/*for (unsigned a = 0; a < 6; ++a)
	{
		std::cout << order[a]->finishEating;
	}
	std::cout << std::endl;*/

	/*for (unsigned a = 0; a < 6; ++a)
	{
		std::cout << table[a]->id << ",";
	}
	std::cout << std::endl;*/
}

static const float SKYBOXSIZE = 1000.f;

void SceneAI::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.6f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneAI::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.4f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneAI::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for(int i = 0; i < 2; ++i)
	{
		if(mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED+i], 0);
		}
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
	}
	mesh->Render();

	//for(int i = 0; i < 2; ++i)
	//{
	//	if(mesh->textureArray[i] > 0)
			glBindTexture(GL_TEXTURE_2D, 0);
	//}
}

void SceneAI::RenderMeshIn2D(Mesh *mesh, bool enableLight, Vector3 scale, Vector3 pos)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(pos.x, pos.y, pos.z);
	modelStack.Scale(scale.x, scale.y, scale.z);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	for (unsigned a = 0; a < 2; ++a)
	{
		if (mesh->textureArray[a] > 0)
		{
			if (mesh->textureArray[a] > 0)
			{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + a], 1);
			}
			else
			{
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + a], 0);
			}
			glActiveTexture(GL_TEXTURE0 + a);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[a]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + a], a);
		}
	}
	mesh->Render();

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneAI::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	
	RenderMesh(meshList[GEO_AXES], false);

	for (unsigned a = 0; a < 6; ++a)
	{
		//Render Dishes at counter
		if (order[a]->finishCooking)
		{
			RenderMeshIn2D(meshList[GEO_DISHES], false, Vector3(10, 10, 0), order[a]->counterPos);
		}
		else
		{
			if (!order[a]->tablePos.IsZero())
			{
				RenderMeshIn2D(meshList[GEO_DISHES], false, Vector3(10, 10, 0), order[a]->tablePos);
			}
		}
	}

	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		//Waitress
		Waitress* waitress = dynamic_cast<Waitress*>(*it);
		if (waitress != NULL)
		{
			modelStack.PushMatrix();
			RenderMeshIn2D(waitress->getMesh(), true, waitress->getScale(), waitress->getPos());
			modelStack.PopMatrix();
		}

		//Customer
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			if (customer->getActive())
			{
				RenderMeshIn2D(customer->getMesh(), true, customer->getScale(), customer->getPos());
			}
		}
		
		//World Object
		CWorldOBJ* worldObj = dynamic_cast<CWorldOBJ*>(*it);
		if (worldObj != NULL)
		{
			RenderMeshIn2D(worldObj->getMesh(), true, worldObj->getScale(), worldObj->getPos());
		}

		//Chef
		Chef* chef = dynamic_cast<Chef*>(*it);
		if (chef != NULL)
		{
			if (chef->getActive())
			{
				RenderMeshIn2D(chef->getMesh(), true, chef->getScale(), chef->getPos());
			}
		}

	}
	
	//Render Sprite animation or other game assets
	RenderSA_etc();

	//Render Text
	RenderText();
}

void SceneAI::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}

	while(m_cGOList.size() > 0)
	{
		CGameObject* go = m_cGOList.back();
		delete go;
		m_cGOList.pop_back();
	}

	for (unsigned a = 0; a < 6; ++a)
	{
		delete queue[a];
		delete table[a];
		delete order[a];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void SceneAI::RenderText()
{
	//Elapsed Time
	RenderTextOnScreen(meshList[GEO_TEXT], "Elapsed Time: " + std::to_string(elapsedTime), Color(0.5, 0.5, 0.5), 50, 30, 0);

	//Table Number
	for (unsigned a = 0; a < 6; ++a)
	{
		if (table[a] != NULL)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(table[a]->id), Color(1, 1, 0), 50, table[a]->pos.x + 20, table[a]->pos.y - 20);
		}
	}
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		//Waitress State
		Waitress* waitress = dynamic_cast<Waitress*>(*it);
		if (waitress != NULL)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], waitress->renderState(), Color(1, 1, 0), 25, waitress->pos.x, waitress->pos.y - 8);
		}

		//Customer State
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			if (customer->getActive())
				RenderTextOnScreen(meshList[GEO_TEXT], customer->renderState(), Color(1, 1, 0), 25, customer->pos.x, customer->pos.y - 8);
		}

		//Chef State
		Chef* chef = dynamic_cast<Chef*>(*it);
		if (chef != NULL)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], chef->renderState(), Color(1, 1, 0), 25, chef->pos.x, chef->pos.y - 8);
		}
	}
}

void SceneAI::RenderSA_etc()
{
	Chef* chef = fetchChef();
	if (chef != NULL)
	{
		for (int i = 0; i < MAX_COSTOMER_COUNT; ++i)
		{
			if (chef->state == Chef::s_Cook)
			{
				modelStack.PushMatrix();
				RenderMeshIn2D(meshList[GEO_FIRE_SPRITE], true, Vector3(50, 50, 0), Vector3(485, 590, 2));
				modelStack.PopMatrix();
			}
		}
	}
}

Customer* SceneAI::fetchCustomer(bool getActive)
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			if (customer->getActive() == getActive)
			{
				return customer;
			}
		}
	}

	return NULL;
}

Customer* SceneAI::fetchQueueingCustomer()
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			if (customer->getActive() && customer->isQueing())
			{
				return customer;
			}
		}
	}

	return NULL;
}

Customer* SceneAI::fetchOrderingCustomer()
{

	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			if (customer->getActive() && customer->isReadyToOrder())
			{
				return customer;
			}
		}
	}

	return NULL;
}

Customer* SceneAI::fetchCustomerbyid(int id)
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			if (customer->getActive() && customer->id == id)
			{
				return customer;
			}
		}
	}

	return NULL;
}

Waitress* SceneAI::fetchWaitress()
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Waitress* waitress = dynamic_cast<Waitress*>(*it);
		if (waitress != NULL)
		{
			return waitress;
		}
	}

	return NULL;
}

Chef* SceneAI::fetchChef()
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Chef* chef = dynamic_cast<Chef*>(*it);
		if (chef != NULL)
		{
			return chef;
		}
	}

	return NULL;
}

void SceneAI::SpawnCustomer()
{
	s_OBJPOS* queue = fetchQueue(false);
	Customer* customer = fetchCustomer(false);
	if (customer != NULL)
	{
		queue->taken = true;
		customer->setActive(true);
		customer->setPos(queue->pos);
		customer->setToQueueing();
	}
}

void SceneAI::UnspawnCustomer()
{
	s_OBJPOS* queuepos = fetchQueue();
	Customer* customer = fetchCustomer();
	if (queuepos != NULL && customer != NULL)
	{
		if (queuepos->pos == customer->getPos())
		{
			queuepos->taken = false;
			customer->setActive(false);
		}
	}
}

SceneAI::s_OBJPOS* SceneAI::fetchQueue(bool taken)
{
	for (unsigned a = 0; a < 6; ++a)
	{
		if (queue[a]->taken == taken)
		{
			return queue[a];
		}
	}

	return NULL;
}

SceneAI::s_OBJPOS* SceneAI::fetchQueueByPos(Vector3 pos)
{
	for (unsigned a = 0; a < 6; ++a)
	{
		if (queue[a]->taken)
		{
			//if((queue[a]->pos - pos).Length() < 10)
			if(queue[a]->pos == pos)
			{
				return queue[a];
			}
		}
	}

	return NULL;
}

SceneAI::s_OBJPOS* SceneAI::fetchTable(bool taken)
{
	for (unsigned a = 0; a < 6; ++a)
	{
		if (table[a]->taken == taken)
		{
			return table[a];
		}
	}
	return NULL;
}

SceneAI::s_OBJPOS* SceneAI::fetchTableById(int id)
{
	for (unsigned a = 0; a < 6; ++a)
	{
		/*if (table[a]->taken && table[a]->id == id)
		{
			return table[a];
		}*/
		if (table[a]->id == id)
		{
			return table[a];
		}
	}

	return NULL;
}

SceneAI::ORDER* SceneAI::fetchOrderById(int id)
{
	for (unsigned a = 0; a < 6; ++a)
	{
		if (order[a]->id == id)
		{
			return order[a];
		}
	}
	return NULL;
}

SceneAI::ORDER* SceneAI::fetchOrder_FinishEating()
{
	for (unsigned a = 0; a < 6; ++a)
	{
		if (order[a]->finishEating)
		{
			return order[a];
		}
	}

	return NULL;
}

bool SceneAI::moveToLocation(CGameObject* obj1, Vector3 destination, bool moveByX)
{
	static double speed = 1;
	if (obj1->pos != destination)
	{
		Vector3 diff = destination - obj1->pos;
		if (moveByX)
		{
			if (obj1->pos.x != destination.x)
			{
				if (diff.x > 0)
				{
					obj1->pos.x += speed;
				}
				else if (diff.x < 0)
				{
					obj1->pos.x -= speed;
				}
			}
			else
			{
				if (diff.y > 0)
				{
					obj1->pos.y += speed;
				}
				else if (diff.y < 0)
				{
					obj1->pos.y -= speed;
				}
			}
		}
		else
		{
			if (obj1->pos.y != destination.y)
			{
				if (diff.y > 0)
				{
					obj1->pos.y += speed;
				}
				else if (diff.y < 0)
				{
					obj1->pos.y -= speed;
				}
			}
			else
			{
				if (diff.x > 0)
				{
					obj1->pos.x += speed;
				}
				else if (diff.x < 0)
				{
					obj1->pos.x -= speed;
				}
			}
		}
		return false;
	}

	return true;
}

int SceneAI::getActiveCustomer()
{
	int a = 0;
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			if (customer->getActive())
			{
				a++;
			}
		}
	}
	return a;
}

int SceneAI::getActiveTable()
{
	int a = 0;
	for (unsigned i = 0; i < 6; ++i)
	{
		if (table[i]->taken)
		{
			a++;
		}
	}

	return a;
}

int SceneAI::getActiveQueue()
{
	int a = 0;
	for (unsigned i = 0; i < 6; ++i)
	{
		if (queue[i]->taken)
		{
			a++;
		}
	}

	return a;
}

bool SceneAI::isQueueEmpty()
{
	for (unsigned a = 0; a < 6; ++a)
	{
		if (queue[a]->taken)
		{
			return false;
		}
	}
	return true;
}

bool SceneAI::isOrderEmpty(int num)
{
	for (unsigned a = 0; a < 6; ++a)
	{
		if (num == 1)
		{
			if (order[a]->tookOrder)
			{
				return false;
			}
		}
		else if (num == 2)
		{
			if (order[a]->placeOrder)
			{
				return false;
			}
		}
		else if (num == 3)
		{
			if (order[a]->takenFood)
			{
				return false;
			}
		}
		else if (num == 4)
		{
			if (order[a]->finishCooking)
			{
				return false;
			}
		}
		else
		{
			if (order[a]->finishEating)
			{
				return false;
			}
		}
	}

	return true;
}

void SceneAI::WaitressUpdate(const double dt)
{
	Waitress* waitress = fetchWaitress();
	if (waitress != NULL)
	{
		if (waitress->isIdle())
		{
			switch (waitress->previousState)
			{
			case Waitress::s_Idle:
				if(getActiveQueue() < 6 && getActiveTable() < 6 && !isQueueEmpty())
				{
					waitress->setToUsher();
				}
				else
				{
					waitress->previousState = Waitress::s_Usher;
				}
				break;

			case Waitress::s_Usher:
				if (fetchOrderingCustomer() != NULL)
				{
					waitress->setToTakeOrder();
				}
				else
				{
					waitress->previousState = Waitress::s_TakeOrder;
				}
				break;

			case Waitress::s_TakeOrder:
				if (isOrderEmpty(1) == false)
				{
					waitress->setToPlaceOrder();
				}
				else
				{
					waitress->previousState = Waitress::s_PlaceOrder;
				}
				break;

			case Waitress::s_PlaceOrder:
				if (!isOrderEmpty(4))
				{
					waitress->setToTakeFood();
				}
				else
				{
					waitress->previousState = Waitress::s_DeliverFood;
				}
				break;

			case Waitress::s_DeliverFood:
				if (!isOrderEmpty(5))
				{
					waitress->setToCleanTable();
				}
				else
				{
					waitress->previousState = Waitress::s_CleanTable;
				}
				break;

			case Waitress::s_CleanTable:
				waitress->previousState = Waitress::s_Idle;
				break;

			default:
				break;
			}
		}
		else
		{
			if (waitress->isUsher())
			{
				WaitressState_Usher(waitress);
			}
			else if (waitress->isTakingOrder())
			{
				WaitressState_OrderFood(waitress);
			}
			else if (waitress->isPlacingOrder())
			{
				WaitressState_PlaceOrder(waitress);
			}
			else if (waitress->isTakingFood())
			{
				WaitressState_TakeOrder(waitress);
			}
			else if (waitress->isDeliveringFood())
			{
				WaitressState_DeliveryOrder(waitress);
			}
			else if (waitress->isCleaningTable())
			{
				WaitressState_CleanTable(waitress);
			}
		}
	}
}

void SceneAI::WaitressState_Usher(Waitress* waitress)
{
	static Customer* customer = NULL;
	static s_OBJPOS* queuepos = NULL;
	static s_OBJPOS* tablepos = NULL;

	if (!waitress->usheringCustomer)
	{
		if (!isQueueEmpty())
		{
			if (customer == NULL && queuepos == NULL && tablepos == NULL)
			{
				if (moveToLocation(waitress, Waitress::queuePos, false))
				{
					Customer* tempCus = fetchQueueingCustomer();
					if (tempCus != NULL)
					{
						s_OBJPOS* tempQueue = fetchQueueByPos(tempCus->pos);
						if (tempQueue != NULL && tempCus->isQueing())
						{
							s_OBJPOS* tempTable = fetchTable(false);
							if (tempTable != NULL)
							{
								customer = tempCus;
								queuepos = tempQueue;
								tablepos = tempTable;
							}
						}
					}
				}
			}
			else
			{
				queuepos->taken = false;
				customer->pos.y = 170;
				customer->setToFollow();
				waitress->usheringCustomer = true;
			}
		}
		else
		{
			if (moveToLocation(waitress, Vector3(waitress->pos.x, waitress->initPos.y, 0), false))
			{
				waitress->previousState = waitress->currentState;
				waitress->setToIdle();
			}
		}
	}
	else
	{
		if (customer->isFollowing())
		{
			Vector3 tableOffsetPos = tablepos->pos;
			if (tablepos->id % 2 == 0)
			{
				tableOffsetPos.y += 50;
			}
			else
			{
				tableOffsetPos.y -= 50;
			}


			if (moveToLocation(waitress, tableOffsetPos))
			{
				customer->pos = tablepos->pos - Vector3(tableOffset, 0, 0);
				customer->setToSitDown();
				customer->id = tablepos->id;
				tablepos->taken = true;
				waitress->usheringCustomer = false;

				customer = NULL;
				tablepos = NULL;
				queuepos = NULL;
			}
		}
		else
		{
			customer = NULL;
			tablepos = NULL;
			queuepos = NULL;
		}
	}
}

void SceneAI::WaitressState_OrderFood(Waitress* waitress)
{
	static bool setUp = false;
	static Customer* customer = NULL;
	static s_OBJPOS* tablePos = NULL;

	if (!setUp)
	{
		if(fetchOrderingCustomer() != NULL)
		{
			if (customer == NULL && tablePos == NULL)
			{
				Customer* tempCus = fetchOrderingCustomer();
				if (tempCus != NULL)
				{
					if (tempCus->isReadyToOrder())
					{
						s_OBJPOS* tempTable = fetchTableById(tempCus->id);
						if (tempTable != NULL)
						{
							customer = tempCus;
							tablePos = tempTable;
						}
					}
				}
			}
			else
			{
				setUp = true;
			}
		}
		else
		{
			if (moveToLocation(waitress, Vector3(waitress->pos.x, waitress->initPos.y, 0), false))
			{
				waitress->previousState = waitress->currentState;
				waitress->setToIdle();
			}
		}
	}
	else
	{
		if (customer->isReadyToOrder())
		{
			Vector3 tableOffsetPos = tablePos->pos;
			if (tablePos->id % 2 == 0)
			{
				tableOffsetPos.y += 50;
			}
			else
			{
				tableOffsetPos.y -= 50;
			}


			if (moveToLocation(waitress, tableOffsetPos))
			{
				customer->setToOrdered();
				order[customer->id - 1]->tookOrder = true;
				order[customer->id - 1]->id = customer->id;

				customer = NULL;
				tablePos = NULL;
				setUp = false;
			}
		}
		else
		{
			customer = NULL;
			tablePos = NULL;
			setUp = false;
		}
	}
}

void SceneAI::WaitressState_PlaceOrder(Waitress* waitress)
{
	static bool passOrder = false;

	if (!passOrder)
	{
		if (moveToLocation(waitress, Vector3(300, 450, 0)))
		{
			for (unsigned a = 0; a < 6; ++a)
			{
				if (order[a]->tookOrder)
				{
					order[a]->placeOrder = order[a]->tookOrder;
					order[a]->tookOrder = false;
				}
			}

			passOrder = true;
		}
	}
	else
	{
		if (moveToLocation(waitress, Vector3(waitress->pos.x, waitress->initPos.y, 0), false))
		{
			waitress->previousState = waitress->currentState;
			waitress->setToIdle();
			passOrder = false;
		}
	}
}

void SceneAI::WaitressState_TakeOrder(Waitress* waitress)
{
	if (moveToLocation(waitress, Vector3(300, 450, 0)))
	{
		for (unsigned a = 0; a < 6; ++a)
		{
			if (order[a]->finishCooking)
			{
				order[a]->finishCooking = false;
				order[a]->takenFood = true;
			}
		}

		waitress->setToDeliveryFood();
	}
}

void SceneAI::WaitressState_DeliveryOrder(Waitress* waitress)
{
	static s_OBJPOS* tablePos = NULL;
	static Customer* customer = NULL;
	static ORDER* tempOrder = NULL;

	if (!isOrderEmpty(3))
	{
		if (tablePos == NULL && customer == NULL && tempOrder == NULL)
		{
			for (unsigned a = 0; a < 6; ++a)
			{
				if (order[a]->takenFood)
				{
					s_OBJPOS* tempTable = fetchTableById(order[a]->id);
					if (tempTable != NULL)
					{
						Customer* tempCus = fetchCustomerbyid(order[a]->id);
						if (tempCus != NULL)
						{
							if (tempCus != NULL)
							{
								tempOrder = order[a];
								customer = tempCus;
								tablePos = tempTable;
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			Vector3 tableOffsetPos = tablePos->pos;
			if (tablePos->id % 2 == 0)
			{
				tableOffsetPos.y += 50;
			}
			else
			{
				tableOffsetPos.y -= 50;
			}

			if (moveToLocation(waitress, tableOffsetPos, false))
			{
				tempOrder->tablePos = customer->pos + Vector3(tableOffset, 0, 0);
				tempOrder->takenFood = true;
				tempOrder->takenFood = false;
				customer->setToEating();

				customer = NULL;
				tempOrder = NULL;
				tablePos = NULL;
			}
		}
	}
	else
	{
		if (moveToLocation(waitress, Vector3(waitress->pos.x, waitress->initPos.y, 0), false))
		{
			waitress->previousState = waitress->currentState;
			waitress->setToIdle();
		}
	}
}

void SceneAI::WaitressState_CleanTable(Waitress* waitress)
{
	static ORDER* order = NULL;
	static s_OBJPOS* tablePos = NULL;

	if (!isOrderEmpty(5))
	{
		if (order == NULL && tablePos == NULL)
		{
			ORDER* tempOrder = fetchOrder_FinishEating();
			if (tempOrder != NULL)
			{
				s_OBJPOS* tempTable = fetchTableById(tempOrder->id);
				if (tempTable != NULL)
				{
					order = tempOrder;
					tablePos = tempTable;
				}
			}
		}
		else
		{
			Vector3 tableOffsetPos = tablePos->pos;
			if (tablePos->id % 2 == 0)
			{
				tableOffsetPos.y += 50;
			}
			else
			{
				tableOffsetPos.y -= 50;
			}

			if (moveToLocation(waitress, tableOffsetPos))
			{
				order->CleanUp();

				order = NULL;
				tablePos = NULL;
			}
		}
	}
	else
	{
		if (moveToLocation(waitress, Vector3(waitress->pos.x, waitress->initPos.y, 0), false))
		{
			waitress->previousState = waitress->currentState;
			waitress->setToIdle();
		}
	}
}

void SceneAI::CustomerUpdate(const double dt)
{
	Waitress* waitress = fetchWaitress();
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			if (customer->getActive())
			{
				customer->update(dt);

				if (customer->isFollowing())
				{
					//Move the customer to their seats
					moveToLocation(customer, waitress->pos);
				}
				else if (customer->isLeaving())
				{
					CustomerState_Leaving(customer);
				}
			}
		}
	}
}

void SceneAI::CustomerState_Leaving(Customer* customer)
{
	if (customer->previouState == Customer::s_Queing)
	{
		s_OBJPOS* queuePos = fetchQueueByPos(customer->pos);
		if (queuePos != NULL)
		{
			queuePos->taken = false;
			customer->setActive(false);
		}
	}
	else
	{
		if (customer->previouState == Customer::s_Eating)
		{
			ORDER* tempOrder = fetchOrderById(customer->id);
			if (tempOrder != NULL)
			{
				tempOrder->finishEating = true;
				customer->previouState = Customer::s_Leave;
			}
		}

		if (customer->id != 0)
		{
			s_OBJPOS* tablePos = fetchTableById(customer->id);
			if (tablePos != NULL)
			{
				Vector3 tableOffsetPos = tablePos->pos;
				if (tablePos->id % 2 == 0)
				{
					tableOffsetPos.y += tableOffset;
				}
				else
				{
					tableOffsetPos.y -= tableOffset;
				}

				if (moveToLocation(customer, Vector3(customer->pos.x, tableOffsetPos.y, 0), false))
				{
					tablePos->taken = false;
					customer->id = 0;
				}
			}
		}
		else
		{
			if (moveToLocation(customer, Vector3(800, customer->pos.y, 0), false))
			{
				customer->setActive(false);
			}
		}
	}
}

void SceneAI::ChefUpdate(const double dt)
{
	Chef* chef = fetchChef();
	if (chef != NULL)
	{
		////chef->update(dt);
		//if (Application::IsKeyPressed('F'))
		//{
		//	/*	bool order[6];
		//	order[0] = 1;
		//	order[1] = 0;
		//	order[2] = 0;
		//	order[3] = 0;
		//	order[4] = 0;
		//	order[5] = 0;
		//	chef->passOrder(order, 6);*/
		//	chef->setToCooking();
		//}

		//SpriteAnimation *sa = dynamic_cast<SpriteAnimation *> (meshList[GEO_FIRE_SPRITE]);
		//if (!sa->m_anim->ended)
		//	sa->Update(dt);

		///*if (chef->isCooking())
		//{
		//	if (moveToLocation(chef, ))
		//}*/

		chef->update(dt);

		if (chef->isIdle())
		{
			if (!isOrderEmpty(2))
				chef->startToCook = true;

			if (chef->startToCook)
			{
				if (moveToLocation(chef, chef->stovePos))
				{
					chef->setToCooking();
					chef->startToCook = false;
				}
			}
		}
		else if (chef->isPlacingFood())
		{
			if (moveToLocation(chef, chef->initPos))
			{
				for (unsigned a = 0; a < 6; ++a)
				{
					/*if (order[a]->placeOrder)
					{
						order[a]->tookOrder = false;
						order[a]->placeOrder = true;
						order[a]->takenFood = false;
						chef->setToIdle();
						break;
					}*/
					if (order[a]->placeOrder)
					{
						order[a]->placeOrder = false;
						order[a]->finishCooking = true;
						chef->setToIdle();
						break;
					}
				}
			}
		}
	}
}