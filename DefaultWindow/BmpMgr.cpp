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
	// �÷��̾� �Ӹ�
	Insert_Bmp(L"../Image/entities/player1_heads.bmp", L"Player_Head");
	Insert_Bmp(L"../Image/entities/player1_heads_Flip.bmp", L"Player_Head_Flip");

	// �÷��̾� ��
	Insert_Bmp(L"../Image/entities/player1_armor_body.bmp", L"Player_Body");
	Insert_Bmp(L"../Image/entities/player1_armor_body_Flip.bmp", L"Player_Body_Flip");

	Insert_Bmp(L"../Image/entities/trainingsarcophagus.bmp", L"Player_Death");
}

void CBmpMgr::Load_MonsterBmp()
{
	// ���� ����
	Insert_Bmp(L"../Image/entities/shopkeeper.bmp", L"Shopkeeper");

	// �׸� �巡��
	Insert_Bmp(L"../Image/entities/dragon_green_Left.bmp", L"Dragon_Green_Left");
	Insert_Bmp(L"../Image/entities/dragon_green_Right.bmp", L"Dragon_Green_Right");

	// ���� �巡��
	Insert_Bmp(L"../Image/entities/dragon_red_Left.bmp", L"Dragon_Red_Left");
	Insert_Bmp(L"../Image/entities/dragon_red_Right.bmp", L"Dragon_Red_Right");

	// ���� ��Ż
	Insert_Bmp(L"../Image/entities/death_metal.bmp", L"Death_Metal_Left");
	Insert_Bmp(L"../Image/entities/death_metal_Flip.bmp", L"Death_Metal_Right");

	// ����
	Insert_Bmp(L"../Image/entities/bat_Flip.bmp", L"Bat_Left");
	Insert_Bmp(L"../Image/entities/bat.bmp", L"Bat_Right");

	// �ʷ� ������
	Insert_Bmp(L"../Image/entities/slime_green.bmp", L"Slime_Green_Left");
	Insert_Bmp(L"../Image/entities/slime_green_Flip.bmp", L"Slime_Green_Right");

	// �Ķ� ������
	Insert_Bmp(L"../Image/entities/slime_blue.bmp", L"Slime_Blue_Left");
	Insert_Bmp(L"../Image/entities/slime_blue_Flip.bmp", L"Slime_Blue_Right");

	// ��Ȳ ������
	Insert_Bmp(L"../Image/entities/slime_orange_Left.bmp", L"Slime_Orange_Left");
	Insert_Bmp(L"../Image/entities/slime_orange_Right.bmp", L"Slime_Orange_Right");

	// �Ϲ� ���̷���
	Insert_Bmp(L"../Image/entities/skeleton.bmp", L"Skeleton_White_Left");
	Insert_Bmp(L"../Image/entities/skeleton_Flip.bmp", L"Skeleton_White_Right");

	// ��� ���̷���
	Insert_Bmp(L"../Image/entities/skeleton_yellow.bmp", L"Skeleton_Yellow_Left");
	Insert_Bmp(L"../Image/entities/skeleton_yellow_Flip.bmp", L"Skeleton_Yellow_Right");

	// �̳�Ÿ�츣��
	Insert_Bmp(L"../Image/entities/minotaur.bmp", L"Minotaur_Left");
	Insert_Bmp(L"../Image/entities/minotaur_Flip.bmp", L"Minotaur_Right");

	// ����
	Insert_Bmp(L"../Image/entities/zombie_Left.bmp", L"Zombie_Left");
	Insert_Bmp(L"../Image/entities/zombie_Right.bmp", L"Zombie_Right");
	Insert_Bmp(L"../Image/entities/zombie_Up.bmp", L"Zombie_Up");
	Insert_Bmp(L"../Image/entities/zombie_Down.bmp", L"Zombie_Down");
}

void CBmpMgr::Load_EnvBmp()
{
	// ���� ���۸���
	Insert_Bmp(L"../Image/Back.bmp", L"Back");

	// �ٴ� Ÿ��
	Insert_Bmp(L"../Image/Tile/boss_floor_A.bmp", L"Boss_Floor");
	Insert_Bmp(L"../Image/Tile/floor_dirt1.bmp", L"Dirt1_Floor");
	Insert_Bmp(L"../Image/Tile/floor_dirt2.bmp", L"Dirt2_Floor");
	Insert_Bmp(L"../Image/Tile/black_tile_01.bmp", L"Black_Tile");

	Insert_Bmp(L"../Image/Tile/TEMP_shop_floor.bmp", L"Shop_Tile");
	Insert_Bmp(L"../Image/Tile/stairs.bmp", L"Stair_Tile");
	Insert_Bmp(L"../Image/Tile/stairs_locked.bmp", L"Stair_Locked_Tile");
	Insert_Bmp(L"../Image/Tile/stairs_locked_miniboss_01.bmp", L"Stair_Boss_Tile");


	// �� Ÿ�� (������Ʈ�� ���)
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

	// ����
	Insert_Bmp(L"../Image/Trap/bouncetrap_Up.bmp", L"Bounce_Trap_Up");
	Insert_Bmp(L"../Image/Trap/bouncetrap_Left.bmp", L"Bounce_Trap_Left");
	Insert_Bmp(L"../Image/Trap/bouncetrap_Down.bmp", L"Bounce_Trap_Down");
	Insert_Bmp(L"../Image/Trap/bouncetrap_Right.bmp", L"Bounce_Trap_Right");

	// �׸��ڵ�
	Insert_Bmp(L"../Image/entities/TEMP_shadow_standard.bmp", L"Standard_Shadow_Left");
	Insert_Bmp(L"../Image/entities/TEMP_shadow_standard_Flip.bmp", L"Standard_Shadow_Right");
}

void CBmpMgr::Load_EffectBmp()
{
	// �÷��̾� �ܰ� ���� ���
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


	// ���̾
	Insert_Bmp(L"../Image/Effect/fire3.bmp", L"Fireball");
	Insert_Bmp(L"../Image/Effect/fire3_vertical.bmp", L"Fireball_V");

	// ���� ����
	Insert_Bmp(L"../Image/Effect/swipe_enemy_Down.bmp", L"Attack_Enemy_Down");
	Insert_Bmp(L"../Image/Effect/swipe_enemy_Up.bmp", L"Attack_Enemy_Up");
	Insert_Bmp(L"../Image/Effect/swipe_enemy_Left.bmp", L"Attack_Enemy_Left");
	Insert_Bmp(L"../Image/Effect/swipe_enemy_Right.bmp", L"Attack_Enemy_Right");

	// ��ƼŬ
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

	// ���� �Ⱦ�
	Insert_Bmp(L"../Image/Button/Start.bmp", L"Start");
	Insert_Bmp(L"../Image/Button/Edit.bmp", L"Edit");
	Insert_Bmp(L"../Image/Button/Exit.bmp", L"Exit");
}

void CBmpMgr::Load_GUIBmp()
{
	// ��Ʈ ���� UI
	Insert_Bmp(L"../Image/gui/TEMP_beat_heart.bmp", L"Beat_Heart");
	Insert_Bmp(L"../Image/gui/TEMP_beat_marker.bmp", L"Beat_Marker");

	// �ؽ�Ʈ
	Insert_Bmp(L"../Image/Text/missed.bmp", L"Text_Miss");

	// ����
	Insert_Bmp(L"../Image/gui/hud_slot_1.bmp", L"Item_Slot1");
	Insert_Bmp(L"../Image/gui/hud_slot_2.bmp", L"Item_Slot2");
	Insert_Bmp(L"../Image/gui/hud_slot_3.bmp", L"Item_Slot3");
	Insert_Bmp(L"../Image/gui/hud_slot_4.bmp", L"Item_Slot4");
	Insert_Bmp(L"../Image/gui/hud_slot_8.bmp", L"Item_Slot5");
	Insert_Bmp(L"../Image/gui/spell.bmp", L"Spell_Slot");
	Insert_Bmp(L"../Image/gui/hud_slot_boots.bmp", L"Item_Slot_Boots1");

	// �Ź��� �׷��� ����
	Insert_Bmp(L"../Image/gui/hud_slot_boots2.bmp", L"Item_Slot_Boots2");
	Insert_Bmp(L"../Image/gui/hud_slot_boots3.bmp", L"Item_Slot_Boots3");

	// �÷��̾� ü�� UI
	Insert_Bmp(L"../Image/gui/heart.bmp", L"Player_Heart");
	Insert_Bmp(L"../Image/gui/heart_empty.bmp", L"Player_Heart_Empty");
	Insert_Bmp(L"../Image/gui/heart_half.bmp", L"Player_Heart_Half");

	// ��ȭ ���� UI
	Insert_Bmp(L"../Image/gui/hud_coins.bmp", L"UI_Coin");
	Insert_Bmp(L"../Image/gui/diamond.bmp", L"UI_Diamond");

	// ���� ü�� UI
	Insert_Bmp(L"../Image/gui/TEMP_heart_small.bmp", L"UI_Small_Heart");
	Insert_Bmp(L"../Image/gui/TEMP_heart_empty_small.bmp", L"UI_Empty_Small_Heart");


	// ���� �ƾ�
	Insert_Bmp(L"../Image/gui/bg_topblade.bmp", L"Cut_Top");
	Insert_Bmp(L"../Image/gui/bg_gradient_02.bmp", L"Cut_Middle");
	Insert_Bmp(L"../Image/gui/bg_bottomblade_name.bmp", L"Cut_Bottom");
	Insert_Bmp(L"../Image/gui/bosscreen_deathmetal.bmp", L"Cut_Death_Metal");

	// ��� UI
	Insert_Bmp(L"../Image/gui/dead_popup.bmp", L"Dead_Popup");
}

void CBmpMgr::Load_ItemBmp()
{
	// ��
	Insert_Bmp(L"../Image/Item/shovel_basic.bmp", L"Shovel_Basic");
	// ���
	Insert_Bmp(L"../Image/Item/pickaxe.bmp", L"Pickaxe");
	// �ܰ�
	Insert_Bmp(L"../Image/Item/weapon_dagger.bmp", L"Dagger");
	// ��ε� �ҵ�
	Insert_Bmp(L"../Image/Item/weapon_broadsword.bmp", L"Broadsword");
	Insert_Bmp(L"../Image/Item/weapon_broadsword_titanium.bmp", L"Broadsword_T");

	// �����ֹ�
	Insert_Bmp(L"../Image/Item/spell_fireball.bmp", L"Fire_Spell");


	// �� ���� + 0.5
	Insert_Bmp(L"../Image/Item/armor_spiked_leather.bmp", L"Spiked_Leather");
	// �� ���� + 1
	Insert_Bmp(L"../Image/Item/armor_chainmail.bmp", L"Chain_Mail");

	// ����
	Insert_Bmp(L"../Image/Item/head_helmet.bmp", L"Head_Helmet");
	Insert_Bmp(L"../Image/Item/head_miners_cap.bmp", L"Head_Miners_Cap");

	// Ű
	Insert_Bmp(L"../Image/Item/misc_key.bmp", L"Item_Key");


	// ����
	Insert_Bmp(L"../Image/Item/resource_coin3.bmp", L"Item_Coin3");
	Insert_Bmp(L"../Image/Item/resource_coin5.bmp", L"Item_Coin5");
	Insert_Bmp(L"../Image/Item/resource_coin10.bmp", L"Item_Coin10");

	// ���̾Ƹ��
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
