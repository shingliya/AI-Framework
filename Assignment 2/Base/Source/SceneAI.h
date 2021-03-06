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
#include "GameAsset\Customer.h"
#include "GameAsset\Baker.h"
#include "GameAsset\Cashier.h"
#include "GameAsset\Messageboard.h"
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
		GEO_CHAT,
		GEO_BLUE_BACKGROUND,
		GEO_DEBUGGING,
		NUM_GEOMETRY,
	};

	enum TEXT_STATE
	{
		s_DEBUGGING,
		s_CHAT
	};

	struct BREAD_SHELF
	{
		int id;
		int breadCount;
		Vector3 pos;

		BREAD_SHELF()
		{
			id = 0;
			pos = Vector3(0, 0, 0);
			breadCount = 0;
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

	bool moveToLocation(CGameObject* obj, Vector3 destination, bool moveByX = true);

	/*void WaitressUpdate(const double dt);
	void WaitressState_Usher(Waitress* waitress);
	void WaitressState_OrderFood(Waitress* waitress);
	void WaitressState_PlaceOrder(Waitress* waitress);
	void WaitressState_TakeOrder(Waitress* waitress);
	void WaitressState_DeliveryOrder(Waitress* waitress);
	void WaitressState_CleanTable(Waitress* waitress);*/

	//void CustomerUpdate(const double dt);
	void CustomerState_Leaving(Customer* customer);

	void ChefUpdate(const double dt);

	Baker* fetchBaker();
	Cashier* fetchCashier();
	Customer* fetchActiveCustomer();
	Customer* fetchInactiveCustomer();
	Customer* fetchPayingCustomer();


	void BakerUpdate(const double dt);
	void BakerState_Baking(const double dt, Baker* baker);
	void BakerState_PlacingFood(Baker* baker);
	void BakerState_Restock(Baker* baker);
	void BakerState_OutOfMaterial(Baker* baker);
	void BakerState_Cashier(Baker* baker);

	void CashierUpdate(const double dt);
	void CashierState_Shopping(Cashier* cashier, const double dt);

	void CustomerUpdate(const double dt);
	void CustomerState_AtShop(Customer* customer);
	void CustomerState_Browsing(Customer* customer);
	void CustomerState_Paying(Customer* customer);
	void CustomerState_Exit(Customer* customer);
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
	bool bakerReadytoInform;
	bool cashierShopping;

	bool bakerMessageSent;
	bool cashierMessageSent;

	int availableMaterialCount;
	int breadCount;
	int availableBreadCount;

	float fps;
	double elapsedTime;

	TEXT_STATE textState;
	BREAD_SHELF* shelf[4];

	//Game Asset
	std::vector<CGameObject*> m_cGOList;

	Messageboard messageboard;
};

#endif