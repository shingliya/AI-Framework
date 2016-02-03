#include "SceneAI.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

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

	meshList[GEO_CHAT] = MeshBuilder::GenerateQuad("", Color(0, 0, 0));
	meshList[GEO_CHAT]->textureArray[0] = LoadTGA("Image//CHAT.tga");

	meshList[GEO_DEBUGGING] = MeshBuilder::GenerateQuad("", Color(0, 0, 0));
	meshList[GEO_DEBUGGING]->textureArray[0] = LoadTGA("Image//DEBUGGING.tga");

	meshList[GEO_BLUE_BACKGROUND] = MeshBuilder::GenerateQuad("", Color(0, 0, 0));
	meshList[GEO_BLUE_BACKGROUND]->textureArray[0] = LoadTGA("Image//Blue.tga");

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
	availableMaterialCount = 20;
	availableBreadCount = 0;
	bakerReadytoInform = false;
	cashierShopping = false;
	bakerMessageSent = false;
	cashierMessageSent = false;
	breadCount = 0;

	textState = TEXT_STATE::s_DEBUGGING;

	shelf[0] = new BREAD_SHELF();
	shelf[0]->id = 1;
	shelf[0]->pos = Vector3(1280 * 0.5 - 90, 720 * 0.5 - 50, 1);

	shelf[1] = new BREAD_SHELF();
	shelf[1]->id = 2;
	shelf[1]->pos = Vector3(1280 * 0.5 - 90, 720 * 0.5 - 200, 1);

	shelf[2] = new BREAD_SHELF();
	shelf[2]->id = 3;
	shelf[2]->pos = Vector3(1280 - 50, 720 * 0.5 - 50, 1);

	shelf[3] = new BREAD_SHELF();
	shelf[3]->id = 4;
	shelf[3]->pos = Vector3(1280 - 50, 720 * 0.5 - 200, 1);

	//Initialize Waitress
	//Waitress* waitress = new Waitress();
	//waitress->setMesh(MeshBuilder::GenerateSphere("Waitress", Color(1, 0, 0), 18, 36));
	//waitress->setPos(Vector3(400, 170, 0));
	//waitress->setScale(Vector3(15, 15, 0));
	//waitress->initPos = Vector3(400, 170, 0);
	//m_cGOList.push_back(waitress);

	//Initialize Baker
	Baker* baker = new Baker();
	baker->setMesh(MeshBuilder::GenerateSphere("Chef", Color(0, 0.5, 1), 18, 36));
	baker->setPos(Vector3(1155, 669, 2));
	baker->setScale(Vector3(20, 20, 0));
	m_cGOList.push_back(baker);

	//Initialize Cashier
	Cashier* cashier = new Cashier();
	cashier->setMesh(MeshBuilder::GenerateSphere("Cashier", Color(1, 0.5, 0), 18, 36));
	cashier->setPos(Vector3(855, 483, 2));
	cashier->setScale(Vector3(20, 20, 0));
	m_cGOList.push_back(cashier);

	for (int a = 0; a < 4; ++a)
	{
		Customer* customer = new Customer();
		customer->setMesh(MeshBuilder::GenerateSphere("", customer->color[a], 18, 36));
		customer->setPos(Vector3(1280 * 0.5, 720 * 0.5, 2));
		customer->setScale(Vector3(20, 20, 0));
		customer->id = a + 1;
		m_cGOList.push_back(customer);
	}

	static float size = 10;
	//===============TABLE=============//
	CWorldOBJ* newObj = new CWorldOBJ();

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(1100, 150, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(1100, 450, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(700, 150, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(700, 450, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(700, 750, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setPos(Vector3(1100, 750, 0));
	newObj->setScale(Vector3(400, 300, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Black.tga");
	newObj->setPos(Vector3(200, 720 * 0.5, 0));
	newObj->setScale(Vector3(600, 2180 * 0.5, size));
	m_cGOList.push_back(newObj);

	//Vector3(100, 100, 100), Vector3(1280 - 50, 720 - 50, 1
	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Stove.tga");
	newObj->setPos(Vector3(1280 - 50, 720 - 50, 1));
	newObj->setScale(Vector3(100, 100, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Fridge.tga");
	newObj->setPos(Vector3(1280 - 50, 720 - 150, 1));
	newObj->setScale(Vector3(100, 100, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Table.tga");
	newObj->setPos(Vector3(550, 720 - 100, 1));
	newObj->setScale(Vector3(100, 200, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Barrier.tga");
	newObj->setPos(Vector3(600, 720 - 205, 1));
	newObj->setScale(Vector3(200, 10, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Barrier.tga");
	newObj->setPos(Vector3(1280 - 200, 720 - 205, 1));
	newObj->setScale(Vector3(400, 10, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Counter.tga");
	newObj->setPos(Vector3(900, 720 * 0.5 + 100, 1));
	newObj->setScale(Vector3(200, 100, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Shelf.tga");
	newObj->setPos(shelf[0]->pos);
	newObj->setScale(Vector3(100, 100, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Shelf.tga");
	newObj->setPos(shelf[1]->pos);
	newObj->setScale(Vector3(100, 100, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Shelf.tga");
	newObj->setPos(shelf[2]->pos);
	newObj->setScale(Vector3(100, 100, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Shelf.tga");
	newObj->setPos(shelf[3]->pos);
	newObj->setScale(Vector3(100, 100, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Mat.tga");
	newObj->setPos(Vector3(900 , 25, 1));
	newObj->setScale(Vector3(100, 100, 100));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 0, 0)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//White.tga");
	newObj->setPos(Vector3(250, 620, 1));
	newObj->setScale(Vector3(500, 20, 200));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 0, 0)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//White.tga");
	newObj->setPos(Vector3(250, 700, 1));
	newObj->setScale(Vector3(20, 150, 200));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//DEBUGGING.tga");
	newObj->setPos(Vector3(0 + 100, 720 - 50, 1));
	newObj->setScale(Vector3(200, 200, 200));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Kitchen", Color(1, 1, 1)));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//CHAT.tga");
	newObj->setPos(Vector3(400, 720 - 50, 1));
	newObj->setScale(Vector3(200, 200, 200));
	m_cGOList.push_back(newObj);
}

void SceneAI::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	else if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	fps = (float)(1.f / dt);
	elapsedTime += dt;

	Vector3 mousePos;
	mousePos.x = Application::mouse_current_x;
	mousePos.y = 720 - Application::mouse_current_y;

	if (Application::IsMousePressed(0))
	{
		if (mousePos.y >= 640 && mousePos.y <= 720)
		{
			if (mousePos.x >= 0 && mousePos.x <= 250)
			{
				textState = TEXT_STATE::s_DEBUGGING;
			}
			else if (mousePos.x >= 250 && mousePos.x <= 500)
			{
				textState = TEXT_STATE::s_CHAT;
			}
		}
	}

	BakerUpdate(dt);
	CashierUpdate(dt);
	CustomerUpdate(dt);
	std::cout << fetchCashier()->renderState() << ", " << fetchCashier()->renderState2() << std::endl;
}

static const float SKYBOXSIZE = 1000.f;

void SceneAI::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
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
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	ortho.SetToOrtho(0, 1280, 0, 720, -10, 10);
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
	for (unsigned i = 0; i < text.length(); ++i)
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
	if (enableLight && bLightEnabled)
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

	for (int i = 0; i < 2; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
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
	ortho.SetToOrtho(0, 1280, 0, 720, -10, 10);
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

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}

	RenderMesh(meshList[GEO_AXES], false);

	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		//Baker
		Baker* baker = dynamic_cast<Baker*>(*it);
		if (baker)
		{
			RenderMeshIn2D(baker->getMesh(), false, baker->getScale(), baker->getPos());
		}

		//Cashier
		Cashier* cashier = dynamic_cast<Cashier*>(*it);
		if (cashier)
		{
			RenderMeshIn2D(cashier->getMesh(), false, cashier->getScale(), cashier->getPos());
		}

		//Customer
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer)
		{
			if (customer->getActive())
			{
				RenderMeshIn2D(customer->getMesh(), false, customer->getScale(), customer->getPos());
			}
		}

		//World Object
		CWorldOBJ* worldObj = dynamic_cast<CWorldOBJ*>(*it);
		if (worldObj)
		{
			RenderMeshIn2D(worldObj->getMesh(), false, worldObj->getScale(), worldObj->getPos());
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
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	while (m_cGOList.size() > 0)
	{
		CGameObject* go = m_cGOList.back();
		delete go;
		m_cGOList.pop_back();
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void SceneAI::RenderText()
{
	//Elapsed Time
	RenderTextOnScreen(meshList[GEO_TEXT], "Elapsed Tim e: " + std::to_string(elapsedTime), Color(1, 0, 1), 50, 30, 0);


	for (int a = 0; a < 4; ++a)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(shelf[a]->id), Color(1, 1, 0), 60, shelf[a]->pos.x + 20, shelf[a]->pos.y - 30);
	}



	if (textState == TEXT_STATE::s_DEBUGGING)
	{
		for (int a = 0; a < m_cGOList.size(); ++a)
		{
			std::string textToRender = "";

			//Baker
			Baker *baker = dynamic_cast<Baker*>(m_cGOList[a]);
			if (baker)
			{
				textToRender = "Baker: " + baker->renderState();

				RenderTextOnScreen(meshList[GEO_TEXT], textToRender, Color(0, 0.5, 1), 40, 50, 560 - a * 40);
			}

			//Cashier
			Cashier* cashier = dynamic_cast<Cashier*>(m_cGOList[a]);
			if (cashier)
			{
				textToRender = "Cashier: " + cashier->renderState();
				RenderTextOnScreen(meshList[GEO_TEXT], textToRender, Color(1, 0.5, 0), 40, 50, 560 - a * 40);
			}

			//Customer
			Customer* customer = dynamic_cast<Customer*>(m_cGOList[a]);
			if (customer)
			{
				textToRender = "Custom er " + std::to_string(customer->id) + ": " + customer->renderState();
				RenderTextOnScreen(meshList[GEO_TEXT], textToRender, customer->color[customer->id - 1], 40, 50, 560 - a * 40);
			}
		}

		for (int a = 0; a < 4; ++a)
		{
			std::string textToRender = "";
			textToRender = "Shelf " + std::to_string(shelf[a]->id) + ": " + std::to_string(shelf[a]->breadCount);
			RenderTextOnScreen(meshList[GEO_TEXT], textToRender, Color(1, 1, 0), 40, 50, 240 - a * 40);
		}

		RenderTextOnScreen(meshList[GEO_TEXT], "Available Bread: " + std::to_string(availableBreadCount), Color(1, 1, 1), 40, 50, 80);
		RenderTextOnScreen(meshList[GEO_TEXT], "Available M aterial: " + std::to_string(availableMaterialCount), Color(1, 1, 1), 40, 50, 40);
	}
	else
	{
		int startingY = 40;
		//ivan
		for (int i = messageboard.getTotalMassage()-1; i > -1; i--)
		{
			string uptext = "From:" + messageboard.getFrom(i) + " " + "To:" + messageboard.getTo(i) + " " + "Completed:" + messageboard.getComplete(i);
			string downtext = "Message:" + messageboard.getMessage(i);

			RenderTextOnScreen(meshList[GEO_TEXT], uptext, Color(1, 1, 1), 30, 35, startingY + 20);
			RenderTextOnScreen(meshList[GEO_TEXT], downtext, Color(1, 1, 1), 30, 35, startingY);
			
			startingY += 55;
		}
	}

}

void SceneAI::RenderSA_etc()
{
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

Baker* SceneAI::fetchBaker()
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Baker* baker = dynamic_cast<Baker*>(*it);
		if (baker != NULL)
		{
			return baker;
		}
	}

	return NULL;
}

Cashier* SceneAI::fetchCashier()
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Cashier* cashier = dynamic_cast<Cashier*>(*it);
		if (cashier != NULL)
		{
			return cashier;
		}
	}

	return NULL;
}

Customer* SceneAI::fetchActiveCustomer()
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer)
		{
			if (customer->active)
			{
				return customer;
			}
		}
	}

	return NULL;
}

Customer* SceneAI::fetchInactiveCustomer()
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer)
		{
			if (!customer->active)
			{
				return customer;
			}
		}
	}

	return NULL;
}

Customer* SceneAI::fetchPayingCustomer()
{
	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer)
		{
			if (customer->getActive() && customer->currentState == Customer::s_Paying)
				return customer;
		}
	}

	return NULL;
}

void SceneAI::BakerState_Baking(const double dt, Baker* baker)
{
	if (moveToLocation(baker, Baker::stovePos, false))
		baker->update(dt);
}

void SceneAI::BakerState_PlacingFood(Baker* baker)
{
	if (moveToLocation(baker, Baker::tablePos, false))
	{
		availableBreadCount += 40;
		messageboard.setMessage("baker", "cashier", "bread is ready, come take it");
		baker->setToRestock();
	}
}

void SceneAI::BakerState_Restock(Baker* baker)
{
	if (moveToLocation(baker, Baker::fridgePos, false))
	{
		if (availableMaterialCount <= 0)
		{
			baker->setToInformCashier();
		}
		else
		{
			availableMaterialCount -= 20;
			baker->setToBaking();
		}
	}
}

void SceneAI::BakerState_OutOfMaterial(Baker* baker)
{
	Cashier* cashier = fetchCashier();

	if (bakerMessageSent == false)
	{
		messageboard.setMessage("baker", "cashier", "no more material, help buy pls");
		bakerMessageSent = true;
	}

	moveToLocation(baker, Cashier::CounterPos + Vector3(0, 40, 0));
}

void SceneAI::BakerState_Cashier(Baker* baker)
{
	static bool moveToLoc = false;

	if (!moveToLoc)
	{
		if (moveToLocation(baker, Cashier::CounterPos))
		{
			moveToLoc = true;
		}
	}
	else
	{
		static bool setToRestocking = false;
		Customer* customer = fetchPayingCustomer();

		if (messageboard.checkMessageAvalible("ready to purchase") && customer && baker->getPos() == Cashier::CounterPos)
		{
			if (customer->getPos() == Cashier::CounterPos - Vector3(0, 100, 0))
			{
				customer->setToExit();
				messageboard.setComplete("ready to purchase");
			}
		}
		else if (messageboard.checkMessageAvalible("bread is ready, come take it") && availableBreadCount > 0 && !setToRestocking)
		{
			if (moveToLocation(baker, Baker::tablePos - Vector3(0, 100, 0), false))
			{
				breadCount = availableBreadCount;
				availableBreadCount = 0;
				setToRestocking = true;
				messageboard.setComplete("bread is ready, come take it");
			}
		}
		else if (availableBreadCount == 0 && setToRestocking)
		{
			static bool moved1 = false, moved2 = false, moved3 = false;

			if (!moved1)
			{
				if (moveToLocation(baker, Vector3(750, baker->getPos().y, baker->getPos().z)))
				{
					moved1 = true;
				}
			}
			else
			{
				static bool shelf1 = false, shelf2 = false, shelf3 = false, shelf4 = false;
				static BREAD_SHELF* shelfVariable = NULL;
				static Vector3 shelfPos;

				if (breadCount > 0)
				{
					if (shelfVariable == NULL)
					{
						if (!shelf1 && !shelf2 && !shelf3 && !shelf4)
							shelfVariable = shelf[0];
						else if (shelf1 && !shelf2 && !shelf3 && !shelf4)
							shelfVariable = shelf[1];
						else if (shelf1 && shelf2 && !shelf3 && !shelf4)
							shelfVariable = shelf[2];
						else
							shelfVariable = shelf[3];

						if (shelfVariable->id <= 2)
						{
							shelfPos = shelfVariable->pos + Vector3(80, 0, 0);
						}
						else
						{
							shelfPos = shelfVariable->pos - Vector3(80, 0, 0);
						}
						shelfPos.z = 2;
					}
					else
					{
						if (moveToLocation(baker, shelfPos, false))
						{
							shelfVariable->breadCount += breadCount / 4;

							if (!shelf1 && !shelf2 && !shelf3 && !shelf4)
								shelf1 = true;
							else if (shelf1 && !shelf2 && !shelf3 && !shelf4)
								shelf2 = true;
							else if (shelf1 && shelf2 && !shelf3 && !shelf4)
								shelf3 = true;
							else
							{
								breadCount = 0;
								shelf1 = shelf2 = shelf3 = shelf4 = false;
							}

							shelfVariable = NULL;
						}
					}
				}
				else
				{
					if (!moved2)
					{
						if (moveToLocation(baker, Vector3(750, Cashier::CounterPos.y, baker->getPos().z)))
						{
							moved2 = true;
						}
					}
					else
					{
						if (!moved3)
						{
							if (moveToLocation(baker, Vector3(Cashier::CounterPos.x, baker->getPos().y, baker->getPos().z)))
							{
								moved3 = true;
							}
						}
						else
						{
							moved1 = moved2 = moved3 = false;
							setToRestocking = false;
							/*				cashier->previousState = cashier->currentState;
											cashier->setToIdle();*/

						}
					}
				}
				/*
				if (cashier->isIdle())
			{
				if (cashier->previousState == Cashier::s_IDLE)
				{
					if (bakerReadytoInform && baker->getPos() == Cashier::CounterPos + Vector3(0, 40, 0))
					{
						cashier->setToShopping();
						baker->setToCashier();
						bakerReadytoInform = false;
					}
					else
					{
						cashier->previousState = Cashier::s_CHECKFORBREAD;
					}
				}
				else if (cashier->previousState == Cashier::s_CHECKFORBREAD)
				{
					if (availableBreadCount > 0)
					{
						cashier->setToCheckForBread();
					}
					else
					{
						cashier->previousState = Cashier::s_RESTOCK;
					}
				}
				else if (cashier->previousState == Cashier::s_RESTOCK)
				{
					if (fetchPayingCustomer() != NULL && fetchPayingCustomer()->getPos() == Cashier::CounterPos - Vector3(0, 100, 0))
					{
						cashier->setToSettlePurchase();
					}
					else
					{
						cashier->previousState = Cashier::s_SETTLEPURCHASE;
					}
				}
				else if (cashier->previousState == Cashier::s_SETTLEPURCHASE)
				{
					cashier->previousState = Cashier::s_IDLE;
				}
			}
			else
			{
				if (cashier->isShopping())
				{
					CashierState_Shopping(cashier, dt);
				}
				else if (cashier->isCheckingForBread())
				{
					if (moveToLocation(cashier, Baker::tablePos - Vector3(0, 100, 0), false))
					{
						breadCount = availableBreadCount;
						availableBreadCount = 0;
						cashier->setToRestocking();
					}
				}
				else if (cashier->isRestocking())
				{
					static bool moved1 = false, moved2 = false, moved3 = false;

					if (!moved1)
					{
						if (moveToLocation(cashier, Vector3(750, cashier->getPos().y, cashier->getPos().z)))
						{
							moved1 = true;
						}
					}
					else
					{
						static bool shelf1 = false, shelf2 = false, shelf3 = false, shelf4 = false;
						static BREAD_SHELF* shelfVariable = NULL;
						static Vector3 shelfPos;

						if (breadCount > 0)
						{
							if (shelfVariable == NULL)
							{
								if (!shelf1 && !shelf2 && !shelf3 && !shelf4)
									shelfVariable = shelf[0];
								else if (shelf1 && !shelf2 && !shelf3 && !shelf4)
									shelfVariable = shelf[1];
								else if (shelf1 && shelf2 && !shelf3 && !shelf4)
									shelfVariable = shelf[2];
								else
									shelfVariable = shelf[3];

								if (shelfVariable->id <= 2)
								{
									shelfPos = shelfVariable->pos + Vector3(80, 0, 0);
								}
								else
								{
									shelfPos = shelfVariable->pos - Vector3(80, 0, 0);
								}
								shelfPos.z = 2;
							}
							else
							{
								if (moveToLocation(cashier, shelfPos, false))
								{
									shelfVariable->breadCount += breadCount / 4;

									if (!shelf1 && !shelf2 && !shelf3 && !shelf4)
										shelf1 = true;
									else if (shelf1 && !shelf2 && !shelf3 && !shelf4)
										shelf2 = true;
									else if (shelf1 && shelf2 && !shelf3 && !shelf4)
										shelf3 = true;
									else
									{
										breadCount = 0;
										shelf1 = shelf2 = shelf3 = shelf4 = false;
									}

									shelfVariable = NULL;
								}
							}
						}
						else
						{
							if (!moved2)
							{
								if (moveToLocation(cashier, Vector3(750, Cashier::CounterPos.y, cashier->getPos().z)))
								{
									moved2 = true;
								}
							}
							else
							{
								if (!moved3)
								{
									if (moveToLocation(cashier, Vector3(Cashier::CounterPos.x, cashier->getPos().y, cashier->getPos().z)))
									{
										moved3 = true;
									}
								}
								else
								{
									moved1 = moved2 = moved3 = false;
									cashier->previousState = cashier->currentState;
									cashier->setToIdle();
								}
							}
						}
					}
				}
				else if (cashier->isSettlingPurchase())
				{
					Customer* customer = fetchPayingCustomer();
					if (customer != NULL)
					{
						if (customer->getPos() == Cashier::CounterPos - Vector3(0, 100, 0))
						{
							customer->setToExit();
						}
					}

					cashier->previousState = cashier->currentState;
					cashier->setToIdle();
				}
			}*/
			}
		}
		/*else if (messageboard.getMessage("cashier") == "ready to purchase" && availableBreadCount == 0 && !setToRestocking && customer)
		{
			if (customer->getPos() == Cashier::CounterPos - Vector3(0, 100, 0))
			{
				customer->setToExit();
				messageboard.setComplete("ready to purchase");
			}
		}*/
	}
}

void SceneAI::BakerUpdate(const double dt)
{
	Baker* baker = fetchBaker();

	if (baker->isBaking())
	{
		BakerState_Baking(dt, baker);
	}
	else if (baker->isPlacingFood())
	{
		BakerState_PlacingFood(baker);
	}
	else if (baker->isRestocking())
	{
		BakerState_Restock(baker);
	}
	else if (baker->isOutOfMaterial())
	{
		BakerState_OutOfMaterial(baker);
	}
	else if (baker->isCashier())
	{
		BakerState_Cashier(baker);
	}
}

void SceneAI::CashierUpdate(const double dt)
{
	Cashier* cashier = fetchCashier();
	Baker* baker = fetchBaker();

	if (cashier->isIdle())
	{
		if (messageboard.checkMessageAvalible("ready to purchase"))
		{
			if (fetchPayingCustomer() != NULL && fetchPayingCustomer()->getPos() == Cashier::CounterPos - Vector3(0, 100, 0))
				cashier->setToSettlePurchase();
		}
		else if (messageboard.checkMessageAvalible("no more material, help buy pls"))
		{
			if (baker->getPos() == Cashier::CounterPos + Vector3(0, 40, 0))
			{
				cashier->setToShopping();
				baker->setToCashier();
				messageboard.setComplete("no more material, help buy pls");
				bakerMessageSent = false;
			}
		}
		else if (messageboard.checkMessageAvalible("bread is ready, come take it"))
		{
			if(availableBreadCount > 0)
				cashier->setToCheckForBread();
		}
		/*if (cashier->previousState == Cashier::s_IDLE)
		{
			///if (bakerReadytoInform && baker->getPos() == Cashier::CounterPos + Vector3(0, 40, 0))
			if (messageboard.getMessage("cashier") == "no more material, help buy pls" && baker->getPos() == Cashier::CounterPos + Vector3(0, 40, 0))
			{
				cashier->setToShopping();
				baker->setToCashier();
				///bakerReadytoInform = false;
				messageboard.setComplete("no more material, help buy pls");
				bakerMessageSent = false;
			}
			else
			{
				cashier->previousState = Cashier::s_CHECKFORBREAD;
			}
		}
		else if (cashier->previousState == Cashier::s_CHECKFORBREAD)
		{
			if (messageboard.getMessage("cashier") == "bread is ready, come take it" && availableBreadCount > 0)
			{
				cashier->setToCheckForBread();
			}
			else
			{
				cashier->previousState = Cashier::s_RESTOCK;
			}
		}
		else if (cashier->previousState == Cashier::s_RESTOCK)
		{
			if (messageboard.getMessage("cashier") == "ready to purchase" && fetchPayingCustomer() != NULL && fetchPayingCustomer()->getPos() == Cashier::CounterPos - Vector3(0, 100, 0))
			{
				cashier->setToSettlePurchase();
			}
			else
			{
				cashier->previousState = Cashier::s_SETTLEPURCHASE;
			}
		}
		else if (cashier->previousState == Cashier::s_SETTLEPURCHASE)
		{
			cashier->previousState = Cashier::s_IDLE;
		}*/
	}
	else
	{
		if (cashier->isShopping())
		{
			CashierState_Shopping(cashier, dt);
		}
		else if (cashier->isCheckingForBread())
		{
			if (moveToLocation(cashier, Baker::tablePos - Vector3(0, 100, 0), false))
			{
				breadCount = availableBreadCount;
				availableBreadCount = 0;
				messageboard.setComplete("bread is ready, come take it");
				cashier->setToRestocking();
			}
		}
		else if (cashier->isRestocking())
		{
			static bool moved1 = false, moved2 = false, moved3 = false;

			if (!moved1)
			{
				if (moveToLocation(cashier, Vector3(750, cashier->getPos().y, cashier->getPos().z)))
				{
					moved1 = true;
				}
			}
			else
			{
				static bool shelf1 = false, shelf2 = false, shelf3 = false, shelf4 = false;
				static BREAD_SHELF* shelfVariable = NULL;
				static Vector3 shelfPos;

				if (breadCount > 0)
				{
					if (shelfVariable == NULL)
					{
						if (!shelf1 && !shelf2 && !shelf3 && !shelf4)
							shelfVariable = shelf[0];
						else if (shelf1 && !shelf2 && !shelf3 && !shelf4)
							shelfVariable = shelf[1];
						else if (shelf1 && shelf2 && !shelf3 && !shelf4)
							shelfVariable = shelf[2];
						else
							shelfVariable = shelf[3];

						if (shelfVariable->id <= 2)
						{
							shelfPos = shelfVariable->pos + Vector3(80, 0, 0);
						}
						else
						{
							shelfPos = shelfVariable->pos - Vector3(80, 0, 0);
						}
						shelfPos.z = 2;
					}
					else
					{
						if (moveToLocation(cashier, shelfPos, false))
						{
							shelfVariable->breadCount += breadCount / 4;

							if (!shelf1 && !shelf2 && !shelf3 && !shelf4)
								shelf1 = true;
							else if (shelf1 && !shelf2 && !shelf3 && !shelf4)
								shelf2 = true;
							else if (shelf1 && shelf2 && !shelf3 && !shelf4)
								shelf3 = true;
							else
							{
								breadCount = 0;
								shelf1 = shelf2 = shelf3 = shelf4 = false;
							}

							shelfVariable = NULL;
						}
					}
				}
				else
				{
					if (!moved2)
					{
						if (moveToLocation(cashier, Vector3(750, Cashier::CounterPos.y, cashier->getPos().z)))
						{
							moved2 = true;
						}
					}
					else
					{
						if (!moved3)
						{
							if (moveToLocation(cashier, Vector3(Cashier::CounterPos.x, cashier->getPos().y, cashier->getPos().z)))
							{
								moved3 = true;
							}
						}
						else
						{
							moved1 = moved2 = moved3 = false;
							cashier->previousState = cashier->currentState;
							cashier->setToIdle();
						}
					}
				}
			}
		}
		else if (cashier->isSettlingPurchase())
		{
			Customer* customer = fetchPayingCustomer();
			if (customer != NULL)
			{
				if (customer->getPos() == Cashier::CounterPos - Vector3(0, 100, 0))
				{
					customer->setToExit();
					messageboard.setComplete("ready to purchase");
				}
			}

			cashier->previousState = cashier->currentState;
			cashier->setToIdle();
		}
	}
}

void SceneAI::CashierState_Shopping(Cashier* cashier, const double dt)
{
	static bool moved1 = false, moved2 = false, moved3 = false;
	static float currentTime = 0.f, timeLimit = 0.f;

	if (!cashierShopping)
	{
		if (!moved1)
		{
			if (moveToLocation(cashier, Vector3(770, cashier->getPos().y, cashier->getPos().z)))
			{
				moved1 = true;
			}
		}
		else
		{
			if (!moved2)
			{
				if (moveToLocation(cashier, Vector3(cashier->getPos().x, 370, 2)))
				{
					moved2 = true;
				}
			}
			else
			{
				if (!moved3)
				{
					if (moveToLocation(cashier, Cashier::DoorPos))
					{
						moved3 = true;
					}
				}
				else
				{
					cashierShopping = true;
				}
			}
		}
	}
	else
	{
		if (timeLimit == 0.f)
		{
			timeLimit = Math::RandFloatMinMax(60, 120);
		}
		else
		{
			if (currentTime < timeLimit)
			{
				currentTime += dt;
			}
			else
			{
				if (cashierMessageSent == false)
				{
					messageboard.setMessage("cashier", "baker", "im back with the materials");
					cashierMessageSent = true;
				}

				if (moved3)
				{
					if (moveToLocation(cashier, Vector3(cashier->getPos().x, 370, 2)))
					{
						moved3 = false;
					}
				}
				else
				{
					if (moved2)
					{
						if (moveToLocation(cashier, Vector3(770, 483, cashier->getPos().z)))
						{
							moved2 = false;
						}
					}
					else
					{
						if (moved1)
						{
							if (moveToLocation(cashier, Cashier::CounterPos))
							{
								moved1 = false;
							}
						}
						else
						{
							moved1 = false;
							moved2 = false;
							moved3 = false;
							cashierShopping = false;
							currentTime = 0.f;
							timeLimit = 0.f;
							availableMaterialCount = 100;
							fetchBaker()->setToRestock();
							cashier->setToIdle();
							messageboard.setComplete("im back with the materials");
							cashierMessageSent = false;
						}
					}
				}
			}
		}
	}
}

void SceneAI::CustomerUpdate(const double dt)
{
	static BREAD_SHELF* shelfVariable = NULL;
	static float currentTime = 0.f, timeLimit = 10.f;

	for (CGameObject* go : m_cGOList)
	{
		Customer* customer = dynamic_cast<Customer*>(go);
		if (customer != NULL)
		{
			if (customer->getActive())
			{
				if (customer->isAtShop())
				{
					CustomerState_AtShop(customer);
				}
				else if (customer->isBrowsing())
				{
					CustomerState_Browsing(customer);
				}
				else if (customer->isPaying())
				{
					CustomerState_Paying(customer);
				}
				else if (customer->isExiting())
				{
					CustomerState_Exit(customer);
				}
			}
			else
			{
				if (currentTime < timeLimit)
				{
					currentTime += dt;
				}
				else
				{
					float chance = Math::RandFloatMinMax(0, 100);
					if (chance >= 75)
					{
						customer->active = true;
						customer->currentState = Customer::s_AtShop;
						customer->pos = Cashier::DoorPos;
					}
					currentTime = 0.f;
				}
			}
		}
	}
}

void SceneAI::CustomerState_AtShop(Customer* customer)
{
	if (moveToLocation(customer, Cashier::DoorPos + Vector3(0, 60, 0)))
	{
		customer->setToBrowsing();
	}
}

void SceneAI::CustomerState_Browsing(Customer* customer)
{
	int shelfNum = Math::RandIntMinMax(1, 4);
	static BREAD_SHELF* shelfVariable = NULL;
	static Vector3 shelfPos;

	if (shelfVariable == NULL)
	{
		if (shelf[shelfNum - 1]->breadCount > 0)
		{
			shelfVariable = shelf[shelfNum - 1];
		}
		else
		{
			customer->setToExit();
		}
	}
	else
	{
		if (shelfVariable->id >= 3)
		{
			shelfPos = shelfVariable->pos - Vector3(80, 0, 0);
		}
		else
		{
			shelfPos = shelfVariable->pos + Vector3(80, 0, 0);
		}
		shelfPos.z = 2;

		if (moveToLocation(customer, shelfPos, false))
		{
			shelfVariable->breadCount -= 5;
			shelfVariable = NULL;
			messageboard.setMessage("customer", "cashier", "ready to purchase");
			customer->setToPaying();
		}
	}
}

void SceneAI::CustomerState_Paying(Customer* customer)
{
	moveToLocation(customer, Cashier::CounterPos - Vector3(0, 100, 0));
}

void SceneAI::CustomerState_Exit(Customer* customer)
{
	if (moveToLocation(customer, Cashier::DoorPos))
	{
		customer->active = false;
	}
}