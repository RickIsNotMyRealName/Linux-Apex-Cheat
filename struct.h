#pragma once
#include "vector.h"
#include "bits/stdint-uintn.h"

typedef unsigned long CRC32_t;


//got some of this from misterYs and from UC
class CUserCmd
{
public:
    uint32_t m_commandNumber; //0x0000
    uint32_t m_tickCount; //0x0004
    float m_curtime; //0x0008
    QAngle m_viewAngles; //0x000C
    QAngle m_aimDirection; //0x0018
    float m_forwardmove; //0x0024
    float m_sidemove; //0x0028
    float m_upmove; //0x002C
    char pad_030[8]; //0x0030
    uint32_t m_buttons; //0x0038
    int32_t m_impulse; //0x0034
    char pad_0038[236]; //0x0038
    Vector m_eyePos; //0x0012C
    char pad_019C[224]; //0x00138
}; //Size: 0x218

class CInput
{
	public:
	uint64_t m_cmd;
	CRC32_t m_crc;
};


typedef struct Bone
{
	uint8_t pad1[0xCC];
	float x;
	uint8_t pad2[0xC];
	float y;
	uint8_t pad3[0xC];
	float z;
}Bone;



typedef struct Matrix
{
	float matrix[16];
};


static Vector RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck, bool fullMap, Vector2D radarPixelOffset)
{
	float r_1, r_2;
	float x_1, y_1;
	if(fullMap ==false)
	{
		r_1 = -(EntityPos.y - LocalPlayerPos.y);
		r_2 = EntityPos.x - LocalPlayerPos.x;
		float Yaw = angle - 90.0f;
		float yawToRadian = Yaw * (float)(M_PI / 180.0F);
		x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
		y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;
		*viewCheck = y_1 < 0;
		x_1 *= zoom;
		y_1 *= zoom;
		int sizX = sizeX / 2;
		int sizY = sizeY / 2;
		x_1 += sizX;
	
		y_1 += sizY;
		if (x_1 < 5)
			x_1 = 5;
		if (x_1 > sizeX - 5)
			x_1 = sizeX - 5;
		if (y_1 < 5)
			y_1 = 5;
		if (y_1 > sizeY - 5)
			y_1 = sizeY - 5;
		x_1 += posX;
		y_1 += posY;
		return Vector(x_1, y_1, 0);
	}
	else if(fullMap == true)
	{
		LocalPlayerPos = {0.f,0.f,0.f};
		r_1 = -(EntityPos.y - LocalPlayerPos.y);
		r_2 = EntityPos.x - LocalPlayerPos.x;
		float Yaw = 0.f;
		float yawToRadian = Yaw * (float)(M_PI / 180.0F);
		x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
		y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;
		*viewCheck = y_1 < 0;
		x_1 *= zoom;
		y_1 *= zoom;
		int sizX = sizeX / 2;
		int sizY = sizeY / 2;
		x_1 += sizX;
	
		y_1 += sizY;

		x_1 = x_1 + radarPixelOffset.x;
		y_1 = y_1 + radarPixelOffset.y;

		if (x_1 < 5)
			x_1 = 5;
		if (x_1 > sizeX - 5)
			x_1 = sizeX - 5;
		if (y_1 < 5)
			y_1 = 5;
		if (y_1 > sizeY - 5)
			y_1 = sizeY - 5;
		x_1 += posX;
		y_1 += posY;
		return Vector(x_1, y_1, 0);
	}
	else
	{
		return Vector(0, 0, 0);	
	}
}
