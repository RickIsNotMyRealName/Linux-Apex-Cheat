#pragma once
#include <cstdint>
#include <cstddef>
#include <map>
#include <string>
#include "vector.h"

//i should probably change this filename to be sdk.hpp or something like that

namespace ApexOffsets
{
	// easy paste offsets from https://apex.dumps.host/offsets
	inline std::uintptr_t cengine = 0x123f690;
	inline std::uintptr_t cinput = 0x1d0a930;
	inline std::uintptr_t cl_entitylist = 0x1971e08;
	inline std::uintptr_t client_render_targets = 0x1fbb2c0;
	inline std::uintptr_t client_state = 0x130c3f0;
	inline std::uintptr_t cnetwork_u64_table = 0x750f360;
	inline std::uintptr_t cprediction = 0x1fbaad0;
	inline std::uintptr_t global_vars = 0x130c100;
	inline std::uintptr_t glow_color = 0x1d0;
	inline std::uintptr_t glow_enable = 0xf30;
	inline std::uintptr_t glow_type = 0x2c0;
	inline std::uintptr_t ibase_client_dll = 0x19588e0;
	inline std::uintptr_t iclient_entity_list = 0x1fbaac8;
	inline std::uintptr_t inetwork_string_table = 0x7511ba8;
	inline std::uintptr_t iswapchain = 0xb932a08;
	inline std::uintptr_t iv_debug_overlay = 0x75acd88;
	inline std::uintptr_t iv_engine_client = 0x0;
	inline std::uintptr_t iv_engine_trace = 0x7511b98;
	inline std::uintptr_t iv_model_info_client = 0x7511b90;
	inline std::uintptr_t iv_model_render = 0x1238490;
	inline std::uintptr_t iv_render_view = 0x7511b80;
	inline std::uintptr_t level_name = 0x130c5a0;
	inline std::uintptr_t local_entity_handle = 0x1233038;
	inline std::uintptr_t local_player = 0x1d21668;
	inline std::uintptr_t name_list = 0xba17f40;
	inline std::uintptr_t sign_on_state = 0x130c488;
	inline std::uintptr_t vgui_system = 0x1f56bd0;
	inline std::uintptr_t view_matrix = 0x1b3bd0;
	inline std::uintptr_t view_render = 0x7511b88;

	inline std::uintptr_t OFFSET_THIRDPERSON = 0x01932920 + 0x6c; // thirdperson_override + 0x6c
	inline std::uintptr_t OFFSET_LASTOUTGOINGCOMMAND = client_state + 0x18CDC;

	inline std::uintptr_t OFFSET_TEAM = 0x0448;			// m_iTeamNum
	inline std::uintptr_t OFFSET_HEALTH = 0x0438;		// m_iHealth
	inline std::uintptr_t OFFSET_SHIELD = 0x0170;		// m_shieldHealth
	inline std::uintptr_t OFFSET_NAME = 0x589;			// m_iName
	inline std::uintptr_t OFFSET_SIGN_NAME = 0x0580;	// m_iSignifierName
	inline std::uintptr_t OFFSET_ABS_VELOCITY = 0x140;	// m_vecAbsVelocity --
	inline std::uintptr_t OFFSET_VISIBLE_TIME = 0x1af4; // CPlayer!lastVisibleTime //0x16e0
	inline std::uintptr_t OFFSET_ZOOMING = 0x1c31;		// m_bZooming

	inline std::uintptr_t OFFSET_LIFE_STATE = 0x798;	   // m_lifeState, >0 = dead
	inline std::uintptr_t OFFSET_BLEED_OUT_STATE = 0x2718; // m_bleedoutState, >0 = knocked//#define OFFSET_BLEED_OUT_STATE        0x26e8

	inline std::uintptr_t OFFSET_ORIGIN = 0x14c;			 // FOR BASEENT ----
	inline std::uintptr_t OFFSET_BONES = 0xF38;				 // m_bConstrainBetweenEndpoints
	inline std::uintptr_t OFFSET_AIMPUNCH = 0x2480;			 // m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle --
	inline std::uintptr_t OFFSET_CAMERAPOS = 0x1f20;		 // CPlayer!camera_origin
	inline std::uintptr_t OFFSET_VIEWANGLES = 0x257c - 0x14; // m_ammoPoolCapacity - 0x14
	inline std::uintptr_t OFFSET_BREATH_ANGLES = OFFSET_VIEWANGLES - 0x10;
	inline std::uintptr_t OFFSET_OBSERVER_MODE = 0x33dc;	// m_iObserverMode
	inline std::uintptr_t OFFSET_OBSERVING_TARGET = 0x33e8; // m_hObserverTarget
	inline std::uintptr_t OFFSET_ITEM_ID = 0x1698;			// m_customScriptInt --

	inline std::uintptr_t OFFSET_WEAPON = 0x1a8c;			 // m_latestPrimaryWeapons
	inline std::uintptr_t OFFSET_BULLET_SPEED = 0x1ef0;		 // CWeaponX!m_flProjectileSpeed
	inline std::uintptr_t OFFSET_BULLET_SCALE = 0x1ef8;		 // CWeaponX!m_flProjectileScale
	inline std::uintptr_t OFFSET_ZOOM_FOV = 0x1700 + 0xb8;	 // m_playerData + m_curZoomFOV
	inline std::uintptr_t OFFSET_WEAPON_STATE = 0x16d4;		 // m_weapState
	inline std::uintptr_t OFFSET_WEAPON_NAME_INDEX = 0x1864; // m_weaponNameIndex

	inline std::uintptr_t OFFSET_GLOW_T1 = 0x262;			 // 16256 = enabled, 0 = disabled
	inline std::uintptr_t OFFSET_GLOW_T2 = 0x2dc;			 // 1193322764 = enabled, 0 = disabled
	inline std::uintptr_t OFFSET_GLOW_ENABLE = 0x3c8;		 // 7 = enabled, 2 = disabled
	inline std::uintptr_t OFFSET_GLOW_THROUGH_WALLS = 0x3d0; // 2 = enabled, 5 = disabled

	inline std::uintptr_t OFFSET_GLOW_DISTANCE = 0x3B4;
	inline std::uintptr_t OFFSET_NETCHANNEL = 0x0120a6c0;
	// inline std::uintptr_t OFFSET_NETCHANNEL = 0x11f26b0;
	inline std::uintptr_t OFFSET_SEND_PACKET = 0x12CDC00;

	inline std::uintptr_t GLOW_COLOR_R = 0x1D0; // OK Script_CopyHighlightState mov tcx n�7  / m_highlightParams + 24 (0x18)
	inline std::uintptr_t GLOW_COLOR_G = 0x1D4; // OK Script_CopyHighlightState mov tcx n�7  / m_highlightParams + 24 (0x18)
	inline std::uintptr_t GLOW_COLOR_B = 0x1D8; // OK Script_CopyHighlightState mov tcx n�7  / m_highlightParams + 24 (0x18)
												//#define NUM_ENT_ENTRIES 0x10000
}

enum WeaponId
{
	Havoc = 84,
	R301 = 0,
	Mozambique = 92,
	Peacekeeper = 94,
	Rampage = 13,
	Hands = 17,
	P2020 = 96,
	RE45 = 80,
	R99 = 95,
	G7scout = 87,
	Sentinel = 1,
	ChargeRifle = 81,
	Longbow = 83,
	repeater = 101,
	Wingman = 99,
	Prowler = 93,
	Hemlock = 88,
	Flatline = 86,
	Car = 102,
	Lstar = 90,
	Devotion = 82,
	Volt = 100,
	Bocek = 2,
	Kraber = 89,
	TripleTake = 98,
	Alternator = 79,
	Spitfire = 97,
	Eva8 = 85,
	Mastiff = 91

};

inline std::map<int, std::string> PropList = {
	{190, {("Arc")}},
	{189, {("Frag")}},
	{188, {("Thermite")}},
	{122, {("Shotgun Ammo")}},
	{84, {("Mozambique")}},
	{79, {("Peacekeeper")}},
	{74, {("Eva")}},
	{2, {("Mastiff")}},

	{223, {("Bolt")}},
	{222, {("Bolt")}},
	{221, {("Bolt")}},

	{196, {("3X")}},
	{195, {("Digi")}},
	{119, {("Dual Shell")}},
	{194, {("1X-2X")}},
	{193, {("Holo")}},
	{192, {("2X")}},
	{191, {("Dot")}},

	{68, {("Spitfire")}},
	{42, {("Alternator")}},
	{37, {("G7")}},
	{1, {("Kraber")}},

	{241, {("Shatter Caps")}},
	{239, {("Deadeye's Tempo")}},

	{109, {("Bocek")}},
	{125, {("Arrows")}},

	{121, {("Energy Ammo")}},
	{53, {("Volt")}},
	{22, {("Triple Take")}},
	{17, {("Devotion")}},
	{12, {("Havoc")}},
	{7, {("L-Star")}},

	{226, {("Stock")}},
	{225, {("Stock")}},
	{224, {("Stock")}},

	{203, {("Barrel")}},
	{202, {("Barrel")}},
	{201, {("Barrel")}},

	{216, {("Extended")}},
	{215, {("Extended")}},
	{214, {("Extended")}},
	{213, {("Extended")}},

	{230, {("Turbocharger")}},

	{183, {("Knockdown")}},
	{182, {("Knockdown")}},
	{181, {("Knockdown")}},
	{180, {("Knockdown")}},

	{187, {("Backpack")}},
	{186, {("Backpack")}},
	{185, {("Backpack")}},
	{184, {("Backpack")}},

	{169, {("Helmet")}},
	{168, {("Helmet")}},
	{167, {("Helmet")}},
	{166, {("Helmet")}},

	{173, {("Body Shield")}},
	{172, {("Body Shield")}},
	{171, {("Body Shield")}},
	{170, {("Body Shield")}},
	{178, {("Evo Shield")}},
	{177, {("Evo Shield")}},
	{176, {("Evo Shield")}},
	{175, {("Evo Shield")}},

	{160, {("Accelerant")}},
	{162, {("Med Kit")}},
	{161, {("Phoenix Kit")}},

	{165, {("Shield Cell")}},
	{164, {("Shield Battery")}},
	{163, {("Syringe")}},

	{123, {("Heavy Rounds")}},
	{131, {("C.A.R")}},
	{126, {("Rampage")}},
	{114, {("30-30")}},
	{89, {("Wingman")}},
	{48, {("Prowler")}},
	{32, {("Hemlok")}},
	{27, {("Flatline")}},

	{212, {("Heavy Mag")}},
	{211, {("Heavy Mag")}},
	{210, {("Heavy Mag")}},
	{209, {("Heavy Mag")}},

	{229, {("Sniper Stock")}},
	{228, {("Sniper Stock")}},
	{227, {("Sniper Stock")}},

	{242, {("Boosted Loader")}},

	{120, {("Light Ammo")}},
	{94, {("P2020")}},
	{99, {("Re-45")}},
	{43, {("R-99")}},
	{69, {("R-301")}},

	{208, {("Light Mag")}},
	{207, {("Light Mag")}},
	{206, {("Light Mag")}},
	{205, {("Light Mag")}},

	{124, {("Sniper Ammo")}},
	{104, {("Sentinel")}},
	{63, {("Charge Rifle")}},
	{58, {("Longbow")}},

	{220, {("Sniper Mag")}},
	{219, {("Sniper Mag")}},
	{218, {("Sniper Mag")}},
	{217, {("Sniper Mag")}},

	{200, {("4X-10X Digi")}},
	{199, {("4X-8X")}},
	{198, {("6X")}},
	{197, {("2X-4X")}},
	{246, {("Treasure Pack")}},
	{250, {("Mobile")}},
	{249, {("Heat")}}

};
/*The Firing Range	mp_rr_canyonlands_staging
Kings Canyon	mp_rr_canyonlands_64k_x_64k
Kings Canyon	mp_rr_canyonlands_mu1
Kings Canyon After Dark	mp_rr_canyonlands_mu1_night
Kings Canyon	mp_rr_canyonlands_mu2
Kings Canyon	mp_rr_canyonlands_mu3
World's Edge	mp_rr_desertlands_64k_x_64k
Winter Express	mp_rr_desertlands_holiday
World's Edge	mp_rr_desertlands_mu1
World's Edge	mp_rr_desertlands_mu2
World's Edge	mp_rr_desertlands_mu3
Olympus	mp_rr_olympus
Olympus	mp_rr_olympus_mu1
Storm Point	mp_rr_tropic_island
Party Crasher	mp_rr_party_crasher
Phase Runner	mp_rr_arena_phase_runner
Overflow	mp_rr_aqueduct
Encore	mp_rr_arena_skygarden
*/

//im not sure how i could improve these maps or if they need to be improved. Is there a map or something where i can store more than 2 values?
inline std::map<std::string, std::string> codeToHumanName = {
	{{("mp_lobby")}, {("Lobby")}},
	{{("mp_rr_canyonlands_staging")}, {("The Firing Range")}},
	{{("mp_rr_canyonlands_64k_x_64k")}, {("Kings Canyon (Season 0)")}},
	{{("mp_rr_canyonlands_mu1")}, {("Kings Canyon (Map Update 1)")}},
	{{("mp_rr_canyonlands_mu1_night")}, {("Kings Canyon at Night (Map Update 1)")}},
	{{("mp_rr_canyonlands_mu2")}, {("Kings Canyon (Map Update 2)")}},
	{{("mp_rr_canyonlands_mu3")}, {("Kings Canyon (Map Update 3)")}},
	{{("mp_rr_desertlands_64k_x_64k")}, {("World's Edge (Season 3)")}},
	{{("mp_rr_desertlands_holiday")}, {("Winter Express")}},
	{{("mp_rr_desertlands_mu1")}, {("World's Edge (Map Update 1)")}},
	{{("mp_rr_desertlands_mu2")}, {("World's Edge (Map Update 2)")}},
	{{("mp_rr_desertlands_mu3")}, {("World's Edge (Map Update 3)")}},
	{{("mp_rr_olympus")}, {("Olympus (Season 9")}},
	{{("mp_rr_olympus_mu1")}, {("Olympus (Map Update 1)")}},
	{{("mp_rr_tropic_island")}, {("Storm Point (Season 11)")}},
	{{("mp_rr_party_crasher")}, {("Party Crasher")}},
	{{("mp_rr_arena_phase_runner")}, {("Phase Runner")}},
	{{("mp_rr_aqueduct")}, {("Overflow")}},
	{{("mp_rr_arena_skygarden")}, {("Encore")}},
};
//some of this might not be accurate as some maps arent in rotation.
inline std::map<std::string, float> mapNameToScale = {
	{{("mp_lobby")}, {1.f}},
	{("mp_rr_canyonlands_staging"), {1.4f}},
	{("mp_rr_canyonlands_64k_x_64k"), {0.249098f}},
	{("mp_rr_canyonlands_mu1"), {0.249098f}},
	{("mp_rr_canyonlands_mu1_night"), {0.249098f}},
	{("mp_rr_canyonlands_mu2"), {0.249098f}},
	{("mp_rr_canyonlands_mu3"), {0.249098f}},
	{("mp_rr_desertlands_64k_x_64k"), {1.0f}},
	{("mp_rr_desertlands_holiday"), {1.0f}},
	{("mp_rr_desertlands_mu1"), {1.0f}},
	{("mp_rr_desertlands_mu2"), {1.0f}},
	{("mp_rr_desertlands_mu3"), {1.0f}},
	{("mp_rr_olympus"), {0.225}},
	{("mp_rr_olympus_mu1"), {0.225}},
	{("mp_rr_tropic_island"), {0.198747f}},
	{("mp_rr_party_crasher"), {1.32716f}},
	{("mp_rr_arena_phase_runner"), {1.32716f}},
	{("mp_rr_aqueduct"), {1.32716f}},
	{("mp_rr_arena_skygarden"), {1.32716f}}};

//some also might not be accurate as some maps arent in rotation.
inline std::map<std::string, Vector2D> mapNameTooOffsets = {
	{{("mp_lobby")}, {0,0}},
	{("mp_rr_canyonlands_staging"), Vector2D(-2135, -500)},
	{("mp_rr_canyonlands_64k_x_64k"), Vector2D(-42.f, 37.f)},
	{("mp_rr_canyonlands_mu1"), Vector2D(-42.f, 37.f)},
	{("mp_rr_canyonlands_mu1_night"), Vector2D(-42.f, 37.f)},
	{("mp_rr_canyonlands_mu2"), Vector2D(-42.f, 37.f)},
	{("mp_rr_canyonlands_mu3"), Vector2D(-42.f, 37.f)},
	{("mp_rr_desertlands_64k_x_64k"), Vector2D(76, 34)},
	{("mp_rr_desertlands_holiday"), Vector2D(76, 34)},
	{("mp_rr_desertlands_mu1"), Vector2D(76, 34)},
	{("mp_rr_desertlands_mu2"), Vector2D(76, 34)},
	{("mp_rr_desertlands_mu3"), Vector2D(76, 34)},
	{("mp_rr_olympus"), Vector2D(+79.f, 34.f)},
	{("mp_rr_olympus_mu1"), Vector2D(+79.f, 34.f)},
	{("mp_rr_tropic_island"), Vector2D(-7.f, 10.f)},
	{("mp_rr_party_crasher"), Vector2D(0, 0)},
	{("mp_rr_arena_phase_runner"), Vector2D(0, 0)},
	{("mp_rr_aqueduct"), Vector2D(0, 0)},
	{("mp_rr_arena_skygarden"), Vector2D(0, 0)}

};

inline std::map<int, int> CatagoryList = {
	{-1, -1},
	{Hands, 0},
	{479, 0},

	{Havoc, 1},
	{Flatline, 1},
	{Hemlock, 1},
	{R301, 1},

	{Alternator, 2},
	{Prowler, 2},
	{R99, 2},
	{Volt, 2},
	{Car, 2},

	{Devotion, 3},
	{Lstar, 3},
	{Spitfire, 3},
	{Rampage, 3},

	{G7scout, 4},
	{TripleTake, 4},
	{repeater, 4},
	{Bocek, 4},

	{ChargeRifle, 5},
	{Longbow, 5},
	{Kraber, 5},
	{Sentinel, 5},

	{Eva8, 6},
	{Mastiff, 6},
	{Mozambique, 6},
	{Peacekeeper, 6},

	{RE45, 7},
	{P2020, 7},
	{Wingman, 7},
};

// thanks to casualX for this. see apexBot on github for the original some not used in this cheat
namespace sdk
{
	const size_t NUM_ENT_ENTRIES = 0x10000;
	typedef struct CEntInfo
	{ // 100
		uint64_t pEntity;
		int64_t SerialNumber;
		uint64_t pPrev;
		uint64_t pNext;
	} CEntInfo;

	enum class SignonState : int32_t
	{
		None,
		Challenge,
		Connected,
		StateNew,
		Prespawn,
		GettingData,
		Spawn,
		FirstSnap,
		Full,
		ChangeLevel,
	};
	enum class ButtonCode : uint32_t
	{
		None = 0,
		// More codes...
		MouseLeft = 108,
		MouseRight = 109,
		MouseMiddle = 110,
		Mouse4 = 111,
		Mouse5 = 112,
	};

	// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/globalvars_base.h
	struct CGlobalVars
	{
		/*0x00*/ double realtime;
		/*0x08*/ int32_t framecount;
		/*0x0c*/ float absoluteframetime;
		/*0x10*/ float curtime;
		/*0x14*/ float curtime2;
		/*0x18*/ float curtime3;
		/*0x1c*/ float curtime4;
		/*0x20*/ float frametime;
		/*0x24*/ float curtime5;
		/*0x28*/ float curtime6;
		/*0x2c*/ float zero;
		/*0x30*/ float frametime2;
		/*0x34*/ int32_t maxClients;
		/*0x38*/ int32_t unk38;
		/*0x3c*/ int32_t unk3C;
		/*0x40*/ int32_t tickcount;
		/*0x44*/ float interval_per_tick;
		/*0x48*/ float interpolation_amount;
		// There's more stuff after this but I don't know and I don't care
	};

	struct kbutton_t
	{
		int down[2];
		int state;
	};
}