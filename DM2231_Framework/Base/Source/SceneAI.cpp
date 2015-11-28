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
	
	//glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//MultiTexture.fragmentshader" );
	
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

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("ground", Color(1,1,1), 100);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Diner//Floor.tga");

	meshList[GEO_FIRE_SPRITE] = MeshBuilder::GenerateSpriteAnimation("fire", 2, 4); 
	meshList[GEO_FIRE_SPRITE]->textureArray[0] = LoadTGA("Image//fire.tga"); 
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation *> (meshList[GEO_FIRE_SPRITE]);
	sa->m_anim->Set(0, 7, 0, 1.f);
 
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(75.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	camera.Init(Vector3(50, 50, 50), Vector3(0, 0, 0), Vector3(0, 1, 0));

	bLightEnabled = true;

	Waitress* waitress = new Waitress();
	waitress->setMesh("Image//Character//Waitress.tga");
	waitress->setPos(Vector3(0, 0, 60));
	m_cGOList.push_back(waitress);

	Customer* customer = new Customer();
	customer->setPos(Vector3(30, 0, 60));
	m_cGOList.push_back(customer);

	Chef* chef = new Chef();
	chef->setPos(Vector3(60, 0, 60));
	m_cGOList.push_back(chef);

	static float size = 100;
	CWorldOBJ* newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	newObj->setPos(Vector3(0, 0, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	newObj->setPos(Vector3(size, 0, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	newObj->setPos(Vector3(size, 0, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	newObj->setPos(Vector3(0, 0, size * 2));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	newObj->setPos(Vector3(size, 0, size * 2));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	newObj->setPos(Vector3(-size, 0, size * 2));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	newObj->setPos(Vector3(-size, 0, size));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateQuad("Diner Floor", Color(1, 1, 1), size));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Floor.tga");
	newObj->setRotateAngle(-90);
	newObj->setRotation(Vector3(1, 0, 0));
	newObj->setPos(Vector3(-size, 0, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateOBJ("Diner Table", "OBJ//Diner//DinerTable.obj"));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//DinerTable.tga");
	newObj->setPos(Vector3(size, 0, 0));
	newObj->setScale(Vector3(size / 10, size / 10, size / 10));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateOBJ("Diner Table", "OBJ//Diner//DinerTable.obj"));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//DinerTable.tga");
	newObj->setPos(Vector3(size, 0, size));
	newObj->setScale(Vector3(size / 10, size / 10, size / 10));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateOBJ("Diner Table", "OBJ//Diner//DinerTable.obj"));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//DinerTable.tga");
	newObj->setPos(Vector3(0, 0, size));
	newObj->setScale(Vector3(size / 10, size / 10, size / 10));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateOBJ("Diner Table", "OBJ//Diner//DinerTable.obj"));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//DinerTable.tga");
	newObj->setPos(Vector3(0, 0, 0));
	newObj->setScale(Vector3(size / 10, size / 10, size / 10));
	m_cGOList.push_back(newObj);

	for (unsigned a = 0; a < 5; ++a)
	{
		newObj = new CWorldOBJ();
		newObj->setMesh(MeshBuilder::GenerateOBJ("Barrier", "OBJ//Diner//Barrier.obj"));
		newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//Barrier.tga");
		newObj->setPos(Vector3(-size - size / 3 + a * 35, 0, size * 2));
		newObj->setScale(Vector3(size / 10, size / 10, size / 10));
		m_cGOList.push_back(newObj);
	}

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateOBJ("Barrier", "OBJ//Diner//Door.obj"));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//EnterDoor.tga");
	newObj->setPos(Vector3(-size, 0.2, size * 2 + size / 2));
	newObj->setScale(Vector3(size / 10 + 5, size / 10, size / 10 + 5));
	newObj->setRotateAngle(180);
	newObj->setRotation(Vector3(0, 1, 0));
	m_cGOList.push_back(newObj);

	newObj = new CWorldOBJ();
	newObj->setMesh(MeshBuilder::GenerateOBJ("Barrier", "OBJ//Diner//Door.obj"));
	newObj->getMesh()->textureArray[0] = LoadTGA("Image//Diner//ExitDoor.tga");
	newObj->setPos(Vector3(size , 0.2, size * 2 + size / 2));
	newObj->setScale(Vector3(size / 10 + 5, size / 10, size / 10 + 5));
	newObj->setRotateAngle(180);
	newObj->setRotation(Vector3(0, 1, 0));
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
	camera.Update(dt);

	SpriteAnimation *sa = dynamic_cast<SpriteAnimation *> (meshList[GEO_FIRE_SPRITE]);
	
	if(!sa->m_anim->ended)
		sa->Update(dt);

	fps = (float)(1.f / dt);
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
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
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
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
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

void SceneAI::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]);
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
			1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
			0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneAI::RenderWaitress(CGameObject* go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->getPos().x, go->getPos().y, go->getPos().z);
	modelStack.Rotate(go->getRotateAngle(), 0, 1, 0);
	modelStack.Scale(10, 10, 10);

	//Head
	modelStack.PushMatrix();
	modelStack.Translate(0, 2.4, 0);
	RenderMesh(go->head, false);
	modelStack.PopMatrix();

	//Torso
	modelStack.PushMatrix();
	modelStack.Translate(0, 1.81, 0);
	RenderMesh(go->torso, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.4, 2.2, 0);
	RenderMesh(go->leftArm, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.4, 2.2, 0);
	RenderMesh(go->rightArm, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.2, 0);
	RenderMesh(go->leftLeg, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.2, 0);
	RenderMesh(go->rightLeg, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneAI::RenderCharacter(CGameObject* go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->getPos().x, go->getPos().y, go->getPos().z);
	modelStack.Scale(10, 10, 10);

	modelStack.PushMatrix();
	modelStack.Translate(0, 2.41, 0);
	RenderMesh(go->head, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.81, 0);
	RenderMesh(go->torso, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.61, 1.81, 0);
	RenderMesh(go->leftArm, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.61, 1.81, 0);
	RenderMesh(go->rightArm, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.21, 0.6, 0);
	RenderMesh(go->leftLeg, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.21, 0.6, 0);
	RenderMesh(go->rightLeg, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
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

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();*/

	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		//World Object
		CWorldOBJ* worldObj = dynamic_cast<CWorldOBJ*>(*it);
		if (worldObj != NULL)
		{
			modelStack.PushMatrix();
			modelStack.Translate(worldObj->getPos().x, worldObj->getPos().y, worldObj->getPos().z);
			if(!worldObj->getRotation().IsZero())
				modelStack.Rotate(worldObj->getRotateAngle(), worldObj->getRotation().x, worldObj->getRotation().y, worldObj->getRotation().z);
			if(!worldObj->getScale().IsZero())
				modelStack.Scale(worldObj->getScale().x, worldObj->getScale().y, worldObj->getScale().z);
			RenderMesh(worldObj->getMesh(), false);
			modelStack.PopMatrix();
		}

		//Waitress
		Waitress * waitress = dynamic_cast<Waitress*>(*it);
		if (waitress != NULL)
		{
			RenderWaitress(waitress);
		}

		//Customer
		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			RenderCharacter(customer);
		}

		//Chef
		Chef* chef = dynamic_cast<Chef*>(*it);
		if (chef != NULL)
		{
			RenderCharacter(chef);
		}
	}

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	
	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Light(" << lights[0].position.x << ", " << lights[0].position.y << ", " << lights[0].position.z << ")";
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 0, 3);


	for (std::vector<CGameObject*>::iterator it = m_cGOList.begin(); it != m_cGOList.end(); ++it)
	{
		//Waitress
		Waitress* waitress = dynamic_cast<Waitress*>(*it);
		if (waitress != NULL)
		{
			Vector3 diff = camera.position - waitress->getPos();

			modelStack.PushMatrix();
			modelStack.Translate(waitress->getPos().x , 50, waitress->getPos().z);
			modelStack.Rotate(Math::RadianToDegree(atan2(diff.x, diff.z)), 0, 1, 0);
			modelStack.Scale(10, 10, 10);
			RenderText(meshList[GEO_TEXT], "Waitress", Color(0.5, 0.5, 0.5));
			modelStack.PopMatrix();
		}

		Customer* customer = dynamic_cast<Customer*>(*it);
		if (customer != NULL)
		{
			Vector3 diff = camera.position - customer->getPos();

			modelStack.PushMatrix();
			modelStack.Translate(customer->getPos().x, 50, customer->getPos().z);
			modelStack.Rotate(Math::RadianToDegree(atan2(diff.x, diff.z)), 0, 1, 0);
			modelStack.Scale(10, 10, 10);
			RenderText(meshList[GEO_TEXT], "Customer", Color(0.5, 0.5, 0.5));
			modelStack.PopMatrix();
		}

		Chef* chef = dynamic_cast<Chef*>(*it);
		if (chef != NULL)
		{
			Vector3 diff = camera.position - chef->getPos();

			modelStack.PushMatrix();
			modelStack.Translate(chef->getPos().x, 50, chef->getPos().z);
			modelStack.Rotate(Math::RadianToDegree(atan2(diff.x, diff.z)), 0, 1, 0);
			modelStack.Scale(10, 10, 10);
			RenderText(meshList[GEO_TEXT], "Chef", Color(0.5, 0.5, 0.5));
			modelStack.PopMatrix();
		}
	}
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
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
