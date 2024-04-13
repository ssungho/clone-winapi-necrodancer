#include "stdafx.h"
#include "BmpMgr.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStringCmp(pImgKey));

	if (iter == m_mapBit.end())
	{
		auto pMyBmp = new CMyBmp;
		pMyBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({pImgKey, pMyBmp});
	}
}

HDC CBmpMgr::Find_Image(const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStringCmp(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

void CBmpMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), DeleteMap());
	m_mapBit.clear();
}

void CBmpMgr::Load_PlayerBmp()
{
	// 플레이어 머리
	Insert_Bmp(L"../Image/entities/player1_heads.bmp", L"Player_Head");
	Insert_Bmp(L"../Image/entities/player1_heads_Flip.bmp", L"Player_Head_Flip");

	// 플레이어 몸
	Insert_Bmp(L"../Image/entities/player1_armor_body.bmp", L"Player_Body");
	Insert_Bmp(L"../Image/entities/player1_armor_body_Flip.bmp", L"Player_Body_Flip");

	Insert_Bmp(L"../Image/entities/trainingsarcophagus.bmp", L"Player_Death");
}

void CBmpMgr::Load_MonsterBmp()
{
	// 상점 주인
	Insert_Bmp(L"../Image/entities/shopkeeper.bmp", L"Shopkeeper");

	// 그린 드래곤
	Insert_Bmp(L"../Image/entities/dragon_green_Left.bmp", L"Dragon_Green_Left");
	Insert_Bmp(L"../Image/entities/dragon_green_Right.bmp", L"Dragon_Green_Right");

	// 레드 드래곤
	Insert_Bmp(L"../Image/entities/dragon_red_Left.bmp", L"Dragon_Red_Left");
	Insert_Bmp(L"../Image/entities/dragon_red_Right.bmp", L"Dragon_Red_Right");

	// 데스 메탈
	Insert_Bmp(L"../Image/entities/death_metal.bmp", L"Death_Metal_Left");
	Insert_Bmp(L"../Image/entities/death_metal_Flip.bmp", L"Death_Metal_Right");

	// 박쥐
	Insert_Bmp(L"../Image/entities/bat_Flip.bmp", L"Bat_Left");
	Insert_Bmp(L"../Image/entities/bat.bmp", L"Bat_Right");

	// 초록 슬라임
	Insert_Bmp(L"../Image/entities/slime_green.bmp", L"Slime_Green_Left");
	Insert_Bmp(L"../Image/entities/slime_green_Flip.bmp", L"Slime_Green_Right");

	// 파랑 슬라임
	Insert_Bmp(L"../Image/entities/slime_blue.bmp", L"Slime_Blue_Left");
	Insert_Bmp(L"../Image/entities/slime_blue_Flip.bmp", L"Slime_Blue_Right");

	// 주황 슬라임
	Insert_Bmp(L"../Image/entities/slime_orange_Left.bmp", L"Slime_Orange_Left");
	Insert_Bmp(L"../Image/entities/slime_orange_Right.bmp", L"Slime_Orange_Right");

	// 일반 스켈레톤
	Insert_Bmp(L"../Image/entities/skeleton.bmp", L"Skeleton_White_Left");
	Insert_Bmp(L"../Image/entities/skeleton_Flip.bmp", L"Skeleton_White_Right");

	// 노란 스켈레톤
	Insert_Bmp(L"../Image/entities/skeleton_yellow.bmp", L"Skeleton_Yellow_Left");
	Insert_Bmp(L"../Image/entities/skeleton_yellow_Flip.bmp", L"Skeleton_Yellow_Right");

	// 미노타우르스
	Insert_Bmp(L"../Image/entities/minotaur.bmp", L"Minotaur_Left");
	Insert_Bmp(L"../Image/entities/minotaur_Flip.bmp", L"Minotaur_Right");

	// 좀비
	Insert_Bmp(L"../Image/entities/zombie_Left.bmp", L"Zombie_Left");
	Insert_Bmp(L"../Image/entities/zombie_Right.bmp", L"Zombie_Right");
	Insert_Bmp(L"../Image/entities/zombie_Up.bmp", L"Zombie_Up");
	Insert_Bmp(L"../Image/entities/zombie_Down.bmp", L"Zombie_Down");
}

void CBmpMgr::Load_EnvBmp()
{
	// 더블 버퍼링용
	Insert_Bmp(L"../Image/Back.bmp", L"Back");

	// 바닥 타일
	Insert_Bmp(L"../Image/Tile/boss_floor_A.bmp", L"Boss_Floor");
	Insert_Bmp(L"../Image/Tile/floor_dirt1.bmp", L"Dirt1_Floor");
	Insert_Bmp(L"../Image/Tile/floor_dirt2.bmp", L"Dirt2_Floor");
	Insert_Bmp(L"../Image/Tile/black_tile_01.bmp", L"Black_Tile");

	Insert_Bmp(L"../Image/Tile/TEMP_shop_floor.bmp", L"Shop_Tile");
	Insert_Bmp(L"../Image/Tile/stairs.bmp", L"Stair_Tile");
	Insert_Bmp(L"../Image/Tile/stairs_locked.bmp", L"Stair_Locked_Tile");
	Insert_Bmp(L"../Image/Tile/stairs_locked_miniboss_01.bmp", L"Stair_Boss_Tile");


	// 벽 타일 (오브젝트로 사용)
	Insert_Bmp(L"../Image/Wall/wall_dirt_crypt.bmp", L"Dirt_Crypt");
	Insert_Bmp(L"../Image/Wall/wall_dirt_crypt_diamond4.bmp", L"Dirt_Crypt_Diamond");
	Insert_Bmp(L"../Image/Wall/boss_wall.bmp", L"Boss_Wall");
	Insert_Bmp(L"../Image/Wall/end_of_world.bmp", L"Level_Wall");
	Insert_Bmp(L"../Image/Wall/door_front.bmp", L"Door_Front");
	Insert_Bmp(L"../Image/Wall/door_side.bmp", L"Door_Side");
	Insert_Bmp(L"../Image/Wall/wall_shop_crypt.bmp", L"Shop_Wall");
	Insert_Bmp(L"../Image/Wall/wall_shop_crypt_cracked.bmp", L"Cracked_Shop_Wall");
	Insert_Bmp(L"../Image/Wall/wall_stone_crypt.bmp", L"Stone_Wall");
	Insert_Bmp(L"../Image/Wall/zone1_wall_stone_cracked.bmp", L"Cracked_Stone_Wall");

	// 함정
	Insert_Bmp(L"../Image/Trap/bouncetrap_Up.bmp", L"Bounce_Trap_Up");
	Insert_Bmp(L"../Image/Trap/bouncetrap_Left.bmp", L"Bounce_Trap_Left");
	Insert_Bmp(L"../Image/Trap/bouncetrap_Down.bmp", L"Bounce_Trap_Down");
	Insert_Bmp(L"../Image/Trap/bouncetrap_Right.bmp", L"Bounce_Trap_Right");

	// 그림자들
	Insert_Bmp(L"../Image/entities/TEMP_shadow_standard.bmp", L"Standard_Shadow_Left");
	Insert_Bmp(L"../Image/entities/TEMP_shadow_standard_Flip.bmp", L"Standard_Shadow_Right");
}

void CBmpMgr::Load_EffectBmp()
{
	// 플레이어 단검 공격 모션
	Insert_Bmp(L"../Image/Effect/swipe_dagger.bmp", L"Attack_Dagger_Right");
	Insert_Bmp(L"../Image/Effect/swipe_dagger_Down.bmp", L"Attack_Dagger_Down");
	Insert_Bmp(L"../Image/Effect/swipe_dagger_Left.bmp", L"Attack_Dagger_Left");
	Insert_Bmp(L"../Image/Effect/swipe_dagger_Up.bmp", L"Attack_Dagger_Up");

	Insert_Bmp(L"../Image/Effect/swipe_broadsword_Right.bmp", L"Attack_Sword_Right");
	Insert_Bmp(L"../Image/Effect/swipe_broadsword_Left.bmp", L"Attack_Sword_Left");
	Insert_Bmp(L"../Image/Effect/swipe_broadsword_Down.bmp", L"Attack_Sword_Down");
	Insert_Bmp(L"../Image/Effect/swipe_broadsword_Up.bmp", L"Attack_Sword_Up");

	Insert_Bmp(L"../Image/Effect/swipe_broadsword_Right_T.bmp", L"Attack_Sword_T_Right");
	Insert_Bmp(L"../Image/Effect/swipe_broadsword_Left_T.bmp", L"Attack_Sword_T_Left");
	Insert_Bmp(L"../Image/Effect/swipe_broadsword_Down_T.bmp", L"Attack_Sword_T_Down");
	Insert_Bmp(L"../Image/Effect/swipe_broadsword_Up_T.bmp", L"Attack_Sword_T_Up");


	// 파이어볼
	Insert_Bmp(L"../Image/Effect/fire3.bmp", L"Fireball");
	Insert_Bmp(L"../Image/Effect/fire3_vertical.bmp", L"Fireball_V");

	// 몬스터 공격
	Insert_Bmp(L"../Image/Effect/swipe_enemy_Down.bmp", L"Attack_Enemy_Down");
	Insert_Bmp(L"../Image/Effect/swipe_enemy_Up.bmp", L"Attack_Enemy_Up");
	Insert_Bmp(L"../Image/Effect/swipe_enemy_Left.bmp", L"Attack_Enemy_Left");
	Insert_Bmp(L"../Image/Effect/swipe_enemy_Right.bmp", L"Attack_Enemy_Right");

	// 파티클
	Insert_Bmp(L"../Image/particles/TEMP_particle_dirt.bmp", L"Particle_Dirt");
	Insert_Bmp(L"../Image/particles/TEMP_particle_white.bmp", L"Particle_White");
	Insert_Bmp(L"../Image/particles/TEMP_particle_green.bmp", L"Particle_Green");
	Insert_Bmp(L"../Image/particles/TEMP_particle_purple.bmp", L"Particle_Purple");
	Insert_Bmp(L"../Image/particles/TEMP_particle_blue.bmp", L"Particle_Blue");
	Insert_Bmp(L"../Image/particles/TEMP_particle_yellow.bmp", L"Particle_Yellow");
	Insert_Bmp(L"../Image/particles/TEMP_particle_blood.bmp", L"Particle_Blood");
	Insert_Bmp(L"../Image/particles/skeleton_head_yellow.bmp", L"Particle_Yellow_Head");
	Insert_Bmp(L"../Image/particles/TEMP_particle_orange.bmp", L"Particle_Orange");
	Insert_Bmp(L"../Image/particles/death_metal_shield.bmp", L"Particle_Shield");
}

void CBmpMgr::Load_MenuBmp()
{
	Insert_Bmp(L"../Image/Menu/mainmenu.bmp", L"Menu");
	Insert_Bmp(L"../Image/Menu/continue.bmp", L"Continue");
	Insert_Bmp(L"../Image/alpha_black.bmp", L"Alpha_Black");

	// 지금 안씀
	Insert_Bmp(L"../Image/Button/Start.bmp", L"Start");
	Insert_Bmp(L"../Image/Button/Edit.bmp", L"Edit");
	Insert_Bmp(L"../Image/Button/Exit.bmp", L"Exit");
}

void CBmpMgr::Load_GUIBmp()
{
	// 비트 관련 UI
	Insert_Bmp(L"../Image/gui/TEMP_beat_heart.bmp", L"Beat_Heart");
	Insert_Bmp(L"../Image/gui/TEMP_beat_marker.bmp", L"Beat_Marker");

	// 텍스트
	Insert_Bmp(L"../Image/Text/missed.bmp", L"Text_Miss");

	// 슬롯
	Insert_Bmp(L"../Image/gui/hud_slot_1.bmp", L"Item_Slot1");
	Insert_Bmp(L"../Image/gui/hud_slot_2.bmp", L"Item_Slot2");
	Insert_Bmp(L"../Image/gui/hud_slot_3.bmp", L"Item_Slot3");
	Insert_Bmp(L"../Image/gui/hud_slot_4.bmp", L"Item_Slot4");
	Insert_Bmp(L"../Image/gui/hud_slot_8.bmp", L"Item_Slot5");
	Insert_Bmp(L"../Image/gui/spell.bmp", L"Spell_Slot");
	Insert_Bmp(L"../Image/gui/hud_slot_boots.bmp", L"Item_Slot_Boots1");

	// 신발이 그려진 슬롯
	Insert_Bmp(L"../Image/gui/hud_slot_boots2.bmp", L"Item_Slot_Boots2");
	Insert_Bmp(L"../Image/gui/hud_slot_boots3.bmp", L"Item_Slot_Boots3");

	// 플레이어 체력 UI
	Insert_Bmp(L"../Image/gui/heart.bmp", L"Player_Heart");
	Insert_Bmp(L"../Image/gui/heart_empty.bmp", L"Player_Heart_Empty");
	Insert_Bmp(L"../Image/gui/heart_half.bmp", L"Player_Heart_Half");

	// 재화 관련 UI
	Insert_Bmp(L"../Image/gui/hud_coins.bmp", L"UI_Coin");
	Insert_Bmp(L"../Image/gui/diamond.bmp", L"UI_Diamond");

	// 몬스터 체력 UI
	Insert_Bmp(L"../Image/gui/TEMP_heart_small.bmp", L"UI_Small_Heart");
	Insert_Bmp(L"../Image/gui/TEMP_heart_empty_small.bmp", L"UI_Empty_Small_Heart");


	// 보스 컷씬
	Insert_Bmp(L"../Image/gui/bg_topblade.bmp", L"Cut_Top");
	Insert_Bmp(L"../Image/gui/bg_gradient_02.bmp", L"Cut_Middle");
	Insert_Bmp(L"../Image/gui/bg_bottomblade_name.bmp", L"Cut_Bottom");
	Insert_Bmp(L"../Image/gui/bosscreen_deathmetal.bmp", L"Cut_Death_Metal");

	// 사망 UI
	Insert_Bmp(L"../Image/gui/dead_popup.bmp", L"Dead_Popup");
}

void CBmpMgr::Load_ItemBmp()
{
	// 삽
	Insert_Bmp(L"../Image/Item/shovel_basic.bmp", L"Shovel_Basic");
	// 곡갱이
	Insert_Bmp(L"../Image/Item/pickaxe.bmp", L"Pickaxe");
	// 단검
	Insert_Bmp(L"../Image/Item/weapon_dagger.bmp", L"Dagger");
	// 브로드 소드
	Insert_Bmp(L"../Image/Item/weapon_broadsword.bmp", L"Broadsword");
	Insert_Bmp(L"../Image/Item/weapon_broadsword_titanium.bmp", L"Broadsword_T");

	// 마법주문
	Insert_Bmp(L"../Image/Item/spell_fireball.bmp", L"Fire_Spell");


	// 옷 방어력 + 0.5
	Insert_Bmp(L"../Image/Item/armor_spiked_leather.bmp", L"Spiked_Leather");
	// 옷 방어력 + 1
	Insert_Bmp(L"../Image/Item/armor_chainmail.bmp", L"Chain_Mail");

	// 모자
	Insert_Bmp(L"../Image/Item/head_helmet.bmp", L"Head_Helmet");
	Insert_Bmp(L"../Image/Item/head_miners_cap.bmp", L"Head_Miners_Cap");

	// 키
	Insert_Bmp(L"../Image/Item/misc_key.bmp", L"Item_Key");


	// 코인
	Insert_Bmp(L"../Image/Item/resource_coin3.bmp", L"Item_Coin3");
	Insert_Bmp(L"../Image/Item/resource_coin5.bmp", L"Item_Coin5");
	Insert_Bmp(L"../Image/Item/resource_coin10.bmp", L"Item_Coin10");

	// 다이아몬드
	Insert_Bmp(L"../Image/Item/resource_diamond.bmp", L"Item_Diamond1");
	Insert_Bmp(L"../Image/Item/resource_diamond3.bmp", L"Item_Diamond3");
}

void CBmpMgr::Load_AllBmp()
{
	Load_MenuBmp();
	Load_EnvBmp();
	Load_PlayerBmp();
	Load_MonsterBmp();
	Load_EffectBmp();
	Load_GUIBmp();
	Load_ItemBmp();
}
