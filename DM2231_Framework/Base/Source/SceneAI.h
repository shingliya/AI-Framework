#ifndef SCENE_AI_H
#define SCENE_AI_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "SpriteAnimation.h"
#include "GameAsset\WorldOBJ.h"
#include "GameAsset\Waitress.h"
#include "GameAsset\Customer.h"
#include "GameAsset\Chef.h"
#include <vector>

#define DEBUGGING true;

class SceneAI : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_LIGHTBALL,
		GEO_TEXT,
		GEO_QUAD,
		GEO_FIRE_SPRITE,
		GEO_DISHES,
		NUM_GEOMETRY,
	};

	struct s_OBJPOS
	{
		Vector3 pos;
		bool taken;
		int id;

		s_OBJPOS()
		{
			pos.SetZero();
			taken = false;
			id = -1;
		}

		void CleanUp()
		{
			pos.SetZero();
			taken = false;
			id = -1;
		}
	};

	struct ORDER
	{
		//Took Order From Customer
		bool tookOrder;

		//Place Order With Chef
		bool placeOrder;

		//Take order from chef
		bool takenFood;

		bool finishCooking;

		bool finishEating;

		int id;

		Vector3 tablePos;
		Vector3 counterPos;

		ORDER()
		{
			tablePos.SetZero();
			counterPos.SetZero();
			id = -1;
			tookOrder = placeOrder = takenFood = false;
			finishCooking = false;
			finishEating = false;
		}

		void CleanUp()
		{
			tablePos.SetZero();
			tookOrder = placeOrder = takenFood = finishCooking = finishEating = false;
		}
	};
public:
	SceneAI();
	~SceneAI();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, Vector3 scale, Vector3 pos);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderSkybox();
	void RenderSkyPlane();
	void RenderTerrain();
	void RenderWaitress(CGameObject* go);
	void RenderCharacter(CGameObject* go);
	void RenderText();
	void RenderSA_etc();

	void SpawnCustomer();
	void UnspawnCustomer();
	Customer* fetchCustomer(bool getActive = true);
	Customer* fetchQueueingCustomer();
	Customer* fetchOrderingCustomer();
	Customer* fetchCustomerbyid(int id);
	Waitress* fetchWaitress();
	Chef* fetchChef();
	s_OBJPOS* fetchQueue(bool tekan = true);
	s_OBJPOS* fetchQueueByPos(Vector3 pos);
	s_OBJPOS* fetchTable(bool taken = true);
	s_OBJPOS* fetchTableById(int id);
	ORDER* fetchOrderById(int id);
	ORDER* fetchOrder_FinishEating();
	bool moveToLocation(CGameObject* obj, Vector3 destination, bool moveByX = true);
	int getActiveCustomer();
	int getActiveTable();
	int getActiveQueue();
	bool isQueueEmpty();

	//1 to check against tookOrder <-- Variables in struct
	//2 to check against placeOrder
	//3 to check against takenFood
	//4 to check against finishCooking
	//5 to check against finishEating
	bool isOrderEmpty(int num);

	void WaitressUpdate(const double dt);
	void WaitressState_Usher(Waitress* waitress);
	void WaitressState_OrderFood(Waitress* waitress);
	void WaitressState_PlaceOrder(Waitress* waitress);
	void WaitressState_TakeOrder(Waitress* waitress);
	void WaitressState_DeliveryOrder(Waitress* waitress);
	void WaitressState_CleanTable(Waitress* waitress);

	void CustomerUpdate(const double dt);
	void CustomerState_Leaving(Customer* customer);

	void ChefUpdate(const double dt);
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	float fps;
	double elapsedTime;

	static int tableOffset;
	//Accomdate 6 queue
	s_OBJPOS* queue[6];
	//Accomdate 6 table
	s_OBJPOS* table[6];
	//Accomdate 6 Order
	ORDER* order[6];

	//Game Asset
	std::vector<CGameObject*> m_cGOList;
};

#endif