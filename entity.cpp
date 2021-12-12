#include "entity.h"
#include "memory.h"
#include "offsets.h"
#include "settings.h"
#include "struct.h"

//https://github.com/CasualX/apexbot/blob/master/src/state.cpp#L104


//https://github.com/CasualX/apexbot/blob/master/src/state.cpp#L104
void get_class_name(uint64_t entity_ptr, char* out_str)
{
	uint64_t client_networkable_vtable = Settings::apex.Read<uint64_t>(entity_ptr + 8 * 3);

	uint64_t get_client_class = Settings::apex.Read<uint64_t>(client_networkable_vtable + 8 * 3);

	uint32_t disp = Settings::apex.Read<uint32_t>(get_client_class + 3);
	const uint64_t client_class_ptr = get_client_class + disp + 7;

	ClientClass client_class= Settings::apex.Read<ClientClass>(client_class_ptr);

	Settings::apex.ReadArray<char>(client_class.pNetworkName, out_str, 32);
}

std::string getClassName(uint64_t entity_ptr )
{
	char class_name[32];
	uint64_t client_networkable_vtable = Settings::apex.Read<uint64_t>(entity_ptr + 8 * 3);

	uint64_t get_client_class = Settings::apex.Read<uint64_t>(client_networkable_vtable + 8 * 3);

	uint32_t disp = Settings::apex.Read<uint32_t>(get_client_class + 3);
	const uint64_t client_class_ptr = get_client_class + disp + 7;

	ClientClass client_class= Settings::apex.Read<ClientClass>(client_class_ptr);

	Settings::apex.ReadArray<char>(client_class.pNetworkName, class_name, 32);
	return std::string(class_name);
}



int Entity::getTeamId()
{
	return *(int*)(buffer + ApexOffsets::OFFSET_TEAM);
}

int Entity::getHealth()
{
	return *(int*)(buffer + ApexOffsets::OFFSET_HEALTH);
}

int Entity::getShield()
{
	return *(int*)(buffer + ApexOffsets::OFFSET_SHIELD);
}

Vector Entity::getAbsVelocity()
{
	return *(Vector*)(buffer + ApexOffsets::OFFSET_ABS_VELOCITY);
}

Vector Entity::getPosition()
{
	return *(Vector*)(buffer + ApexOffsets::OFFSET_ORIGIN);
}

bool Entity::isPlayer()
{
	return *(uint64_t*)(buffer + ApexOffsets::OFFSET_NAME) == 125780153691248;
}

bool Entity::isDummy()
{
	char class_name[33] = {};
	get_class_name(ptr, class_name);

	return strncmp(class_name, "CAI_BaseNPC", 11) == 0;
}

/* std::string Entity::getClassName()
{
	char class_name[33] = {};
	get_class_name(ptr, class_name);

	return class_name;
} */
void getClassName(uint64_t ptr, char* className)
{
	get_class_name(ptr, className);
}

std::string getClientClassName( uint64_t entityPtr) 
{
	uint64_t client_networkable_vtable;
	uint64_t get_client_entity;
	uint32_t offset;
	uint64_t network_name_ptr;
	char buffer[32];
	// Read the ClientClass's network name for to given entity
	bool success = Settings::apex.Read(entityPtr + 3 * 8, client_networkable_vtable)
		&& Settings::apex.Read(client_networkable_vtable + 3 * 8, get_client_entity)
		&& Settings::apex.Read(get_client_entity + 3, offset)
		&& Settings::apex.Read(get_client_entity + offset + 7 + 16, network_name_ptr)
		&& Settings::apex.ReadArray(network_name_ptr, buffer, 32);
	std::string result;
	if (success) {
		// Return up to 32 chars from the network name
		size_t len;
		for (len = 0; len < 32; ++len)
			if (buffer[len] == '\0')
				break;
		result.assign(buffer, len);
	}
	return result;
}

bool Entity::isKnocked()
{
	return *(int*)(buffer + ApexOffsets::OFFSET_BLEED_OUT_STATE) > 0;
}

bool Entity::isAlive()
{
	return *(int*)(buffer + ApexOffsets::OFFSET_LIFE_STATE) == 0;
}

float Entity::getLastVisTime()
{
  return *(float*)(buffer + ApexOffsets::OFFSET_VISIBLE_TIME);
}

Vector Entity::getBonePos(int id)
{
	Vector position = getPosition();
	uintptr_t boneArray = *(uintptr_t*)(buffer + ApexOffsets::OFFSET_BONES);
	Vector bone = Vector();
	uint32_t boneloc = (id * 0x30);
	Bone bo = Settings::apex.Read<Bone>(boneArray + boneloc);
	bone.x = bo.x + position.x;
	bone.y = bo.y + position.y;
	bone.z = bo.z + position.z;
	return bone;
}

QAngle Entity::GetSwayAngles()
{
	return *(QAngle*)(buffer + ApexOffsets::OFFSET_BREATH_ANGLES);
}

QAngle Entity::GetViewAngles()
{
	return *(QAngle*)(buffer + ApexOffsets::OFFSET_VIEWANGLES);
}

Vector Entity::GetViewAnglesV()
{
	return *(Vector*)(buffer + ApexOffsets::OFFSET_VIEWANGLES);
}

bool Entity::isZooming()
{
	return *(int*)(buffer + ApexOffsets::OFFSET_ZOOMING) == 1;
}


void Entity::SetViewAngles(SVector angles)
{
	Settings::apex.Write<SVector>(ptr + ApexOffsets::OFFSET_VIEWANGLES, angles);
}

void Entity::SetViewAngles(QAngle& angles)
{
	SetViewAngles(SVector(angles));
}

Vector Entity::GetCamPos()
{
	return *(Vector*)(buffer + ApexOffsets::OFFSET_CAMERAPOS);
}

QAngle Entity::GetRecoil()
{
	return *(QAngle*)(buffer + ApexOffsets::OFFSET_AIMPUNCH);
}

void Entity::get_name(uint64_t g_Base, uint64_t index, char* name)
{
	index *= 0x10;
    uint64_t name_ptr = Settings::apex.Read<uint64_t>(g_Base + ApexOffsets::name_list + index);
	Settings::apex.ReadArray<char>(name_ptr, name, 32);
}


Vector Item::getPosition()
{
	return *(Vector*)(buffer + ApexOffsets::OFFSET_ORIGIN);
}

int Item::getItemId()
{
	return Settings::apex.Read<int>(ptr+ ApexOffsets::OFFSET_ITEM_ID);
}

int Entity::getItemId()
{
	return *(int*)(buffer + ApexOffsets::OFFSET_ITEM_ID);
}


bool Item::isItem()
{
	char class_name[33] = {};
	get_class_name(ptr, class_name);

	return strncmp(class_name, "CPropSurvival", 13) == 0;
}

bool Entity::isItem()
{
	char class_name[33] = {};
	get_class_name(ptr, class_name);
	//printf("%s\n", class_name);

	return strncmp(class_name, "CPropSurvival", 13) == 0;
}

bool Item::isGlowing()
{
    return false;
    //return Settings::apex.Read<int>(ptr +ApexOffsets::glow_enable) == 1;
}

void Item::enableGlow(float color[3])
{
    Settings::apex.Write<GlowMode>(ptr + ApexOffsets::glow_type,Settings::curConfig.glowMode);
    //Settings::apex.Write<GlowMode>(ptr + ApexOffsets::glow_type, { Settings::glowMode.GeneralGlowMode,Settings::glowMode.BorderGlowMode,Settings::glowMode.BorderSize,Settings::glowMode.TransparentLevel});
	Settings::apex.Write<int>(ptr + ApexOffsets::OFFSET_GLOW_ENABLE, 1);
	Settings::apex.Write<int>(ptr + ApexOffsets::OFFSET_GLOW_THROUGH_WALLS, 2);
 	Settings::apex.Write<float>(ptr + ApexOffsets::glow_color, color[0]);
	Settings::apex.Write<float>(ptr + ApexOffsets::glow_color+sizeof(float), color[1]);
	Settings::apex.Write<float>(ptr + ApexOffsets::glow_color+sizeof(float)+ sizeof(float), color[2]);

}
//this is pointless lol
uint64_t Entity::getPtr()
{
	return ptr;
}


Entity getEntity(uintptr_t ptr)
{
	if(ptr == 0)
	{
		Entity ent = Entity(0);
		ent.ptr = 0;
	}
	else
	{
    	Entity entity = Entity(0);
    	entity.ptr = ptr;
    	entity.updateBuffer();

    	return entity;
	}
}

void Entity::updateBuffer()
{
	Settings::apex.ReadArray<uint8_t>(ptr,buffer,sizeof(buffer));
}

Item getItem(uintptr_t ptr)
{
    Item item = Item();
    item.ptr = ptr;
    Settings::apex.ReadArray<uint8_t>(ptr,item.buffer,sizeof(item.buffer));
    return item;
}

bool Entity::isGlowing()
{
    return false;
    //return Settings::apex.Read<int>(ptr +ApexOffsets::glow_enable) == 1;
}

void Entity::enableGlow(float color[3])
{
    Settings::apex.Write<GlowMode>(ptr + ApexOffsets::glow_type,Settings::curConfig.glowMode);
    //Settings::apex.Write<GlowMode>(ptr + ApexOffsets::glow_type, { Settings::glowMode.GeneralGlowMode,Settings::glowMode.BorderGlowMode,Settings::glowMode.BorderSize,Settings::glowMode.TransparentLevel});
	Settings::apex.Write<int>(ptr + ApexOffsets::OFFSET_GLOW_ENABLE, 1);
	Settings::apex.Write<int>(ptr + ApexOffsets::OFFSET_GLOW_THROUGH_WALLS, 2);
 	Settings::apex.Write<float>(ptr + ApexOffsets::glow_color, color[0]);
	Settings::apex.Write<float>(ptr + ApexOffsets::glow_color+sizeof(float), color[1]);
	Settings::apex.Write<float>(ptr + ApexOffsets::glow_color+sizeof(float)+ sizeof(float), color[2]);
}

void getMapNameChar(uint64_t g_Base, char* mapName)
{
	Settings::apex.ReadArray<char>(g_Base + ApexOffsets::level_name, mapName, sizeof(mapName));
}
std::string getMapNameStr(uint64_t gBase)
{
	char buf[64] = {};
	Settings::apex.ReadArray<char>(gBase + ApexOffsets::level_name, buf, sizeof(buf));
	//printf("%s\n", buf);
	return buf;
}

float CalculateFov(Entity& from, Entity& target)
{
	QAngle ViewAngles = from.GetViewAngles();
	Vector LocalCamera = from.GetCamPos();
	Vector EntityPosition = target.getPosition();
	QAngle Angle = Math::CalcAngle(LocalCamera, EntityPosition);
	return Math::GetFov(ViewAngles, Angle);
}

void WeaponXEntity::update(uint64_t LocalPlayer)
{
    uint64_t gBase = Settings::apex.get_proc_baseaddr();
	uint64_t entitylist = gBase + ApexOffsets::cl_entitylist;
	uint64_t wephandle = Settings::apex.Read<uint64_t>(LocalPlayer + ApexOffsets::OFFSET_WEAPON);
	
	wephandle &= 0xffff;

	uint64_t wep_entity = Settings::apex.Read<uint64_t>(entitylist + (wephandle << 5));

	projectile_speed = Settings::apex.Read<float>(wep_entity + ApexOffsets::OFFSET_BULLET_SPEED);
	projectile_scale = Settings::apex.Read<float>(wep_entity + ApexOffsets::OFFSET_BULLET_SCALE);
	zoom_fov = Settings::apex.Read<float>(wep_entity + ApexOffsets::OFFSET_ZOOM_FOV);
	m_weapState = Settings::apex.Read<int>(wep_entity + ApexOffsets::OFFSET_WEAPON_STATE);
	m_weaponNameIndex = Settings::apex.Read<int>(wep_entity + ApexOffsets::OFFSET_WEAPON_NAME_INDEX);
}

float WeaponXEntity::get_projectile_speed()
{
	return projectile_speed;
}

float WeaponXEntity::get_projectile_gravity()
{
	return 750.0f * projectile_scale;
}

float WeaponXEntity::get_zoom_fov()
{
	return zoom_fov;
}


int WeaponXEntity::get_m_weapState()
{
	return m_weapState;
}

int WeaponXEntity::get_m_weaponNameIndex()
{
	return m_weaponNameIndex;
}

double CalculateBoneFov(Entity& fromEnt, Entity& targetEnt, int targetBone)
{
	QAngle ViewAngles = fromEnt.GetViewAngles();
	Vector LocalCamera = fromEnt.GetCamPos();
	Vector EntityPosition = targetEnt.getBonePos(targetBone);
	QAngle Angle = Math::CalcAngle(LocalCamera, EntityPosition);
	return Math::GetFov(ViewAngles, Angle);
}
bool sendPacket(bool sending, uint64_t gBase)
{
	uint64_t netchan_addr = Settings::apex.Read<uintptr_t>(gBase + ApexOffsets::OFFSET_SEND_PACKET); //48 8B 05 ? ? ? ? 48 85 C0 74 07 48 05 ? ? ? ?
    uint64_t netchan = Settings::apex.Read<uintptr_t>(netchan_addr);
	Settings::apex.Write<int>(netchan, sending);
	printf("netchan: %llx\n", netchan);
    if (netchan) {
        Settings::apex.Write<int>(netchan, sending);
		return true;
    }
	return false;
}