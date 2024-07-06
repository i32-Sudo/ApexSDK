#pragma once
#include "../Driver/API/Driver.h"
#include "../utils/pimraryXor.h"
#include "offsets.hpp"
#include "../utils/vector.h"
#include "structs.h"
#include <stdio.h>

enum class HitboxType {
	Head = 0,
	Neck = 1,
	UpperChest = 2,
	LowerChest = 3,
	Stomach = 4,
	Hip = 5,
	LeftShoulder = 6,
	LeftElbow = 7,
	LeftHand = 8,
	RightShoulder = 9,
	RightElbow = 10,
	RightHand = 11,
	LeftThighs = 12,
	LeftKnees = 13,
	LeftLeg = 18,
	RightThighs = 16,
	RightKnees = 17,
	RightLeg = 14
};

class LocalPlayer {
private:

public:
	Vector2 punchAnglesPrev;
	uint64_t LocalPlayerPtr = NULL;

	LocalPlayer() {
		this->LocalPlayerPtr = Driver->RPM<uint64_t>(Driver->BaseAddress + OFF_LOCAL_PLAYER);
		this->punchAnglesPrev = Driver->RPM<Vector2>(LocalPlayerPtr + OFF_PUNCH_ANGLES);
	}

	int GetTeamNum()const {
		return Driver->RPM<int>(LocalPlayerPtr + OFF_TEAM_NUMBER);
	}

	int GetSquadNum()const {
		return Driver->RPM<int>(LocalPlayerPtr + OFF_SQUAD_ID);
	}

	int GetHealth()const {
		return Driver->RPM<int>(LocalPlayerPtr + OFF_CURRENT_HEALTH);
	}

	int GetShields()const {
		return Driver->RPM<int>(LocalPlayerPtr + OFF_CURRENT_SHIELDS);
	}

	int GetInZoom()const {
		return Driver->RPM<short>(LocalPlayerPtr + OFF_ZOOMING) > 0;
	}

	std::string GetPlayerName()const {
		char NameC[128];
		Driver->ReadProcessArray((PVOID)(LocalPlayerPtr + OFF_NAME), NameC, 128);
		return std::string(NameC);
	}

	Vector3 GetCameraPos()const {
		return Driver->RPM<Vector3>(LocalPlayerPtr + OFF_CAMERAORIGIN);
	}

	Vector3 GetLocalOrigin()const {
		return Driver->RPM<Vector3>(LocalPlayerPtr + OFF_LOCAL_ORIGIN);
	}

	Vector3 GetViewAngles()const {
		return Driver->RPM<Vector3>(LocalPlayerPtr + OFF_VIEW_ANGLES);
	}

	Vector2 GetPunchAngles()const {
		return Driver->RPM<Vector2>(LocalPlayerPtr + OFF_PUNCH_ANGLES);
	}

	void setYaw(Vector2 angle) const
	{
		unsigned long ptrLong = LocalPlayerPtr + OFF_VIEW_ANGLES;
		Driver->WPM<Vector2>(ptrLong, angle);
	}

	void setPunch(Vector2 angle) const
	{
		unsigned long ptrLong = LocalPlayerPtr + OFF_PUNCH_ANGLES;
		Driver->WPM<Vector2>(ptrLong, angle);
	}

	unsigned long GetHighlightSettingsPtr() const {
		return Driver->RPM<unsigned long>(Driver->BaseAddress + OFF_GLOW_HIGHLIGHTS);
	}

	float GetProjectileSpeed() const {
		unsigned long weaponHandle = Driver->RPM<unsigned long>(LocalPlayerPtr + OFF_WEAPON_HANDLE);
		unsigned long weaponHandleMasked = weaponHandle & 0xffff;
		unsigned long weaponEntity = Driver->RPM<unsigned long>(Driver->BaseAddress + OFF_ENTITY_LIST + (weaponHandleMasked << 5));
		return Driver->RPM<float>(weaponEntity + OFF_PROJECTILESPEED);
	}

	float GetProjectileScale() const {
		unsigned long weaponHandle = Driver->RPM<unsigned long>(LocalPlayerPtr + OFF_WEAPON_HANDLE);
		unsigned long weaponHandleMasked = weaponHandle & 0xffff;
		unsigned long weaponEntity = Driver->RPM<unsigned long>(Driver->BaseAddress + OFF_ENTITY_LIST + (weaponHandleMasked << 5));
		return Driver->RPM<float>(weaponEntity + OFF_PROJECTILESCALE);
	}

	int GetAmmoInClip() const {
		unsigned long weaponHandle = Driver->RPM<unsigned long>(LocalPlayerPtr + OFF_WEAPON_HANDLE);
		unsigned long weaponHandleMasked = weaponHandle & 0xffff;
		unsigned long weaponEntity = Driver->RPM<unsigned long>(Driver->BaseAddress + OFF_ENTITY_LIST + (weaponHandleMasked << 5));
		return Driver->RPM<int>(weaponEntity + OFFSET_AMMO);
	}
	
	float GetWorldTime()const {
		return Driver->RPM<float>(LocalPlayerPtr + OFFSET_TIME_BASE);
	}

};

class Player {
private:

public:
	int Index = NULL;
	uint64_t PlayerBase = NULL;
	int PlayerDataTableIndex = NULL;

	float lastTimeVisiblePrev = NULL;
	float lastTimeVisible = NULL;


	LocalPlayer lp;
	Player(int IndexID, LocalPlayer lpA) {
		this->Index = IndexID;
		this->PlayerBase = Driver->RPM<uint64_t>(Driver->BaseAddress + OFF_ENTITY_LIST + ((uint64_t)IndexID + 1 << 5));
		this->PlayerDataTableIndex = Driver->RPM<int>(PlayerBase + OFF_NAMEINDEX);
		this->lp = lpA;

		this->lastTimeVisible = Driver->RPM<float>(PlayerBase + OFF_LAST_VISIBLE_TIME);
		this->lastTimeVisiblePrev = lastTimeVisible;
	}

	int GetTeamNum()const {
		return Driver->RPM<int>(PlayerBase + OFF_TEAM_NUMBER);
	}

	int GetSquadNum()const {
		return Driver->RPM<int>(PlayerBase + OFF_SQUAD_ID);
	}

	int GetHealth()const {
		return Driver->RPM<int>(PlayerBase + OFF_CURRENT_HEALTH);
	}

	int GetShields()const {
		return Driver->RPM<int>(PlayerBase + OFF_CURRENT_SHIELDS);
	}

	int GetInZoom()const {
		return Driver->RPM<short>(PlayerBase + OFF_ZOOMING) > 0;
	}

	bool IsVisible()const {
		float lastTimeVisible = Driver->RPM<float>(PlayerBase + OFF_LAST_VISIBLE_TIME);
		bool visible = (lastTimeVisible + 0.2) > lp.GetWorldTime();
		return visible;
	}

	Vector3 GetLocalOrigin()const {
		return Driver->RPM<Vector3>(PlayerBase + OFF_LOCAL_ORIGIN);
	}

	Vector3 GetAbsoluteVelocity()const {
		return Driver->RPM<Vector3>(PlayerBase + OFF_ABSVELOCITY);
	}

	std::string GetLifeStatus()const {
		/* 1 = Alive, 2 = Knocked, 3 = Dead */
		int LiveState = Driver->RPM<int>(PlayerBase + OFF_LIFE_STATE);
		int BleedState = Driver->RPM<int>(PlayerBase + OFF_BLEEDOUT_STATE);
		if (LiveState == 0)
			return "dead";
		if (BleedState == 0)
			return "knocked";
		return "alive";
	}

	std::string GetPlayerName()const {
		char NameC[128];
		Driver->ReadProcessArray((PVOID)(PlayerBase + OFF_NAME), NameC, 128);
		return std::string(NameC);
	}

	Vector3 GetBonePositionByHitBox(HitboxType BoneIndex) const {
		Vector3 origin = Driver->RPM<Vector3>(PlayerBase + OFF_LOCAL_ORIGIN);
		//BoneByHitBox
		uint64_t Model = Driver->RPM<uint64_t>(PlayerBase + OFF_STUDIOHDR);
		if (Model == NULL) return origin;

		//get studio hdr
		uint64_t StudioHdr = Driver->RPM<uint64_t>(Model + 0x8);
		if (StudioHdr == NULL) return origin;

		//get hitbox array
		uint16_t HitboxCache = Driver->RPM<uint16_t>(StudioHdr + 0x34);
		if (HitboxCache == NULL) return origin;
		uint64_t HitBoxsArray = StudioHdr + ((uint16_t)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1)));
		if (HitBoxsArray == NULL) return origin;

		uint16_t IndexCache = Driver->RPM<uint16_t>(HitBoxsArray + 0x4);
		if (IndexCache == NULL) return origin;
		int HitboxIndex = ((uint16_t)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));

		uint16_t Bone = Driver->RPM<uint16_t>(HitBoxsArray + HitboxIndex + (static_cast<int>(BoneIndex) * 0x20));

		if (Bone < 0 || Bone > 255)
			return Vector3();

		//hitpos
		uint64_t BoneArray = Driver->RPM<uint64_t>(PlayerBase + OFF_BONES);
		if (BoneArray == NULL) return origin;
		uintptr_t Add = BoneArray + Bone * sizeof(matrix3x4_t);
		if (Add == NULL) return origin;
		matrix3x4_t Matrix = Driver->RPM<matrix3x4_t>(Add);

		return Vector3(Matrix.m_flMatVal[0][3] + origin.x, Matrix.m_flMatVal[1][3] + origin.y, Matrix.m_flMatVal[2][3] + origin.z);
	}

	int GetBoneFromHitbox(HitboxType HitBox) const {
		long ModelPointer = Driver->RPM<long>(PlayerBase + OFF_STUDIOHDR);
		if (ModelPointer == NULL) {
			return -1;
		}

		unsigned long StudioHDR = Driver->RPM<long>(ModelPointer + 0x8);
		if ((StudioHDR + 0x34) == NULL) {
			return -1;
		}
		auto HitboxCache = Driver->RPM<uint16_t>(StudioHDR + 0x34);
		auto HitboxArray = StudioHDR + ((uint16_t)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1)));
		if ((HitboxArray + 0x4) == NULL) {
			return -1;
		}

		auto IndexCache = Driver->RPM<uint16_t>(HitboxArray + 0x4);
		auto HitboxIndex = ((uint16_t)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));
		auto BonePointer = HitboxIndex + HitboxArray + (static_cast<int>(HitBox) * 0x20);
		if (BonePointer == NULL) {
			return -1;
		}

		return Driver->RPM<uint16_t>(BonePointer);
	}

	Vector3 GetBonePosition(HitboxType HitBox) const {
		Vector3 LocalOrigin = Driver->RPM<Vector3>(PlayerBase + OFF_LOCAL_ORIGIN);
		Vector3 Offset = Vector3(0.0f, 0.0f, 0.0f);

		int Bone = GetBoneFromHitbox(HitBox);
		if (Bone < 0 || Bone > 255)
			return LocalOrigin.Add(Offset);

		long BonePtr = Driver->RPM<long>(PlayerBase + OFF_BONES);
		BonePtr += (Bone * sizeof(Matrix3x4));
		if (BonePtr == NULL) {
			return LocalOrigin.Add(Offset);
		}


		Matrix3x4 BoneMatrix = Driver->RPM<Matrix3x4>(BonePtr);
		Vector3 BonePosition = BoneMatrix.GetPosition();

		if (!BonePosition.IsValid()) {
			return LocalOrigin.Add(Vector3(0, 0, 0));
		}

		BonePosition += LocalOrigin;
		return BonePosition;
	}

	void setGlowThroughWall(int glowThroughWall) const
	{
		unsigned long ptrLong = PlayerBase + OFF_GLOW_THROUGH_WALL;
		if (ptrLong)
			Driver->WPM<int>(ptrLong, glowThroughWall);
	}

	void setGlowEnable(int glowEnable) const
	{
		unsigned long ptrLong = PlayerBase + OFF_GLOW_HIGHLIGHT_ID;
		if (ptrLong)
			Driver->WPM<int>(ptrLong, glowEnable);
	}

};