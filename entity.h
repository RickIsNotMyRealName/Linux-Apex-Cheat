#pragma once
#include <cstdint>
#include <string>
#include "vector.h"
#include "mathUtils.h"

class Entity
{

public:
	Entity(uint64_t entity_ptr) : ptr(entity_ptr) {	 this->updateBuffer();}	
	uint64_t getPtr();

    uint64_t ptr;
	uint8_t buffer[0x3FF0];
	float lastVisTime;
	void updateBuffer();

	Vector getPosition();
	bool isDummy();
	bool isPlayer();
	bool isKnocked();
	bool isAlive();
	float getLastVisTime();
	int getTeamId();
	int getHealth();
	int getShield();
	int getItemId();
	bool isGlowing();
	bool isZooming();
	bool isItem();
	Vector getAbsVelocity();
	QAngle GetSwayAngles();
	QAngle GetViewAngles();
	Vector GetCamPos();
	QAngle GetRecoil();
	Vector GetViewAnglesV();
	std::string getClassName();

	void enableGlow(float color[3]);
	void disableGlow();
	void SetViewAngles(SVector angles);
	void SetViewAngles(QAngle& angles);
	Vector getBonePos(int id);
	bool Observing(uint64_t entitylist);
	void get_name(uint64_t g_Base, uint64_t index, char* name);
};

//should either remove this or actually use it lol but my get class name function is broken sometimes idk y
class Item
{

public:
    uint64_t ptr;
	uint8_t buffer[0x16c0];
	Vector getPosition();
	bool isItem();
	bool isGlowing();
	void enableGlow(float color[3]);
	void disableGlow();
	int getItemId();
};

class WeaponXEntity
{
public:
	void update(uint64_t LocalPlayer);
	float get_projectile_speed();
	float get_projectile_gravity();
	float get_zoom_fov();
	int get_m_weapState();
	int get_m_weaponNameIndex();

private:
	float projectile_scale;
	float projectile_speed;
	float zoom_fov;
	int m_weapState;
	int m_weaponNameIndex;
};

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

struct ClientClass {
	uint64_t pCreateFn;
	uint64_t pCreateEventFn;
	uint64_t pNetworkName;
	uint64_t pRecvTable;
	uint64_t pNext;
	uint32_t ClassID;
	uint32_t ClassSize;
};

Entity getEntity(uintptr_t ptr);
Item getItem(uintptr_t ptr);
//checks if ent is in entCache
uint64_t searchForEnt(uint64_t ptrToFind);
//finds an entPtr in entInfos
bool searchForPtr(uint64_t ptrToFind);

void getClassName(uint64_t ptr, char* outStr);
std::string getClientClassName( uint64_t entityPtr);

std::string getMapNameStr(uint64_t gBase);

void getMapNameChar(uint64_t g_Base, char* mapName);

float CalculateFov(Entity& from, Entity& target);
double CalculateBoneFov(Entity& fromEnt, Entity& targetEnt, int targetBone);
bool sendPacket(bool sending, uint64_t gBase);