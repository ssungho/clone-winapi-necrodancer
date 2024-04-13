#pragma once
#include <cassert>

extern HWND g_hWnd;

#pragma region ��ũ��


#define			WINCX		800
#define			WINCY		600

#define			PURE		= 0

#define			OBJ_NOEVENT	0
#define			OBJ_DEAD	1

// ��Ʈ����
#define			BEAT_POSY			518
#define			HEART_POSY			500
#define			HEART_EXTRA_SIZE	20

#define			PI			3.141592f
#define			MAGENTA		RGB(255, 0, 144)

#define			VK_MAX		0xff

// ������ (�κ��丮, �ʵ�) ũ��
#define			ITEMCX		48
#define			ITEMCY		48

// �κ��丮 ������ ���� ũ��
#define			SLOTCX		60
#define			SLOTCY		66

// ���� ����
#define			SLOT_INTER  4

// �κ��丮 ���� ���� ��ġ
#define			SLOTPOSX	50
#define			SLOTPOSY	50

// ��� ǥ�� ��ġ
#define			P_LIFE_X	700
#define			P_LIFE_Y	50

// ��ȭ ǥ�� ��ġ
#define			COINPOSX	700
#define			COINPOSY	48

// �÷��̾� ���, ����, ���̾� UI ũ��
#define			GUICX		48
#define			GUICY		44

// ������Ż �̵� ���� ����
#define			DM_RANGE_X	11
#define			DM_RANGE_Y	9

// ������Ż �� �߾�
#define			DM_CENTER_X 744
#define			DM_CENTER_Y 504

// ������Ż �̵�, �ڷ���Ʈ�� �˻� ����
#define			DM_SEARCH_START_X 504.f
#define			DM_SEARCH_START_Y 312.f

// Ÿ�� ũ��
#define			TILECX		48
#define			TILECY		48

// �� ũ��
#define			WALLCX		48
#define			WALLCY		96

// Ÿ�� ����
#define			TILEX		30
#define			TILEY		30

#define			FLOAT_RANGE	0.001f

#pragma endregion

#pragma region enum ���̵�

// ������Ʈ ����
enum OBJID
{
	OBJ_MOUSE, // ���콺
	OBJ_TRAP, // ����
	OBJ_ITEM, // ������
	OBJ_PLAYER, // �÷��̾�
	OBJ_MONSTER, // ����
	OBJ_EFFECT, // ȿ��
	OBJ_WALL, // ��
	OBJ_BEAT, // ��Ʈ�� ���� ����(�浹üũ..)
	OBJ_UI, // UI
	OBJ_END
};

// ������ �׷�
enum RENDERID
{
	RENDER_BACKGROUND, // ���
	RENDER_GAME_OBJECT, // ���� ������Ʈ
	RENDER_STRUCTURE, // ��������
	RENDER_EFFECT, // ����Ʈ
	RENDER_UI, // UI
	RENDER_BEAT_HEART, // ��Ʈ�� ���� ..
	RENDER_MOUSE,
	RENDER_END
};

// �� ����
enum SCENEID
{
	SC_MENU,
	SC_EDIT, // ������(�޴����� ���ܵ�)
	SC_STAGE,
	SC_STAGE_01,
	SC_STAGE_02,
	SC_BOSS_STAGE,
	SC_LOADING,
	SC_END
};

// ���� ����
enum DIRECTION
{
	UP,
	LEFT,
	DOWN,
	RIGHT,

	LU,
	RU,
	LD,
	RD,

	DIR_END
};

// ����Ʈ ����
enum EFFECTID
{
	EFFECT_DAGGER,
	EFFECT_SWORD,
	EFFECT_SWORD_T,
	EFFECT_FIREBALL,
	EFFECT_TOOL, // ������
	EFFECT_MONSTER_ATTACK,
	EFFECT_END,
};

// ���� ���
enum COIN_X
{
	COIN_X_1,
	COIN_X_2,
	COIN_END
};

// ���� ����
enum CHANNELID
{
	SOUND_EFFECT,

	SOUND_PLAYER_DIG,
	SOUND_PLAYER_ATTACK1,
	SOUND_PLAYER_ATTACK2,
	SOUND_PLAYER_ATTACK3,
	SOUND_PLAYER_SPELL,
	SOUND_PLAYER_HIT,
	SOUND_PLAYER_DEAD,

	SOUND_BOSS_CUT_SCENE,
	SOUND_BOSS_DEATH,
	SOUND_BOSS_SUMMON,
	SOUND_BOSS_ATTACK,
	SOUND_BOSS_HIT,

	SOUND_MONSTER_ATTACK,
	SOUND_MONSTER_HIT,
	SOUND_MONSTER_DEAD,

	SOUND_MINO_IMPACT,
	SOUND_MINO_CHARGE,

	SOUND_DRAGON_IMPACT,
	SOUND_DRAGON_WALK,

	SOUND_BGM,

	MAX_CHANNEL
};

// ������ ����
enum ITEMID
{
	ITEM_SHOVEL,
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_CAP,
	ITEM_BOMB,
	ITEM_KEY,
	ITEM_SPELL,

	ITEM_COIN, // ��
	ITEM_DIAMOND, // ���̾�
	ITEM_SPECIAL, // UI
	ITEM_MAX
};

// �� ����
enum WALLID
{
	NORMAL, // �Ϲ� ��
	STONE, // �� ��
	DOOR_V, // ���� ��
	DOOR_H, // ���� ��
	LEVEL, // ���� ���
	SHOP, // ���� ��
	BOSS, // ���� �� ��
};

// ���� ����
enum TRAPID
{
	TRAP_BOMB,
	TRAP_BOUNCE,
	TRAP_END
};

#pragma endregion

using INFO = struct tagInfo
{
	float fX;
	float fY;

	float fCX;
	float fCY;
};

using FRAME = struct tagFrame
{
	int iFrameStart; // �� �� �ε������� ������ ���ΰ�
	int iFrameEnd;
	int iMotion;
	DWORD dwSpeed;
	DWORD dwTime;
};

// �޸� �ݳ� �Լ�
template <typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

// ���� �޸� �ݳ� �Լ�
template <typename T>
void Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}

// ������Ʈ ���� �Լ���ü
struct DeleteObj
{
	template <typename T>
	void operator()(T& Obj)
	{
		if (Obj)
		{
			delete Obj;
			Obj = nullptr;
		}
	}
};

struct DeleteMap
{
	template <typename T>
	void operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};

class CStringCmp
{
public:
	CStringCmp(const TCHAR* pString) : m_pString(pString)
	{
	}

public:
	template <typename T>
	bool operator()(T& MyPair)
	{
		return !lstrcmp(m_pString, MyPair.first);
	}

private:
	const TCHAR* m_pString;
};

using VEC2 = struct Vector
{
	Vector()
	{
	}

	Vector(float x, float y)
		: x(x), y(y)
	{
	}

	Vector(POINT pt)
		: x(static_cast<float>(pt.x)), y(static_cast<float>(pt.y))
	{
	}

	bool Is_Zero()
	{
		if (x == 0.f && y == 0.f)
			return true;

		return false;
	}

	const Vector operator+(const Vector& other) const
	{
		/*Vector ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;*/

		return Vector(x + other.x, y + other.y);
	}

	Vector operator-(const Vector& other)
	{
		Vector ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	Vector operator*(float value)
	{
		Vector ret;
		ret.x = x * value;
		ret.y = y * value;
		return ret;
	}

	Vector operator /(Vector _vOther)
	{
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return Vector(x / _vOther.x, y / _vOther.y);
	}

	Vector operator /(float _f)
	{
		assert(!(0.f == _f));
		return Vector(x / _f, y / _f);
	}

	void operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
	}

	void operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
	}

	void operator*=(float value)
	{
		x *= value;
		y *= value;
	}

	float Length_Square() const
	{
		return x * x + y * y;
	}

	float Length() const
	{
		return sqrtf(Length_Square());
	}

	Vector& Normalize()
	{
		float fLen = Length();

		if (fLen < 0.0001f)
			fLen = 0.0001f;

		x /= fLen;
		y /= fLen;

		return *this;
	}

	float Dot(Vector other) const
	{
		return x * other.x + y * other.y;
	}

	float Cross(Vector other) const
	{
		return x * other.y - y * other.x;
	}

	float x = 0;
	float y = 0;
};

#define DECLARE_SINGLE(classname)		\
private:								\
   classname() {}						\
public:									\
   static classname* Get_Instance()     \
   {									\
      static classname s_instance;		\
      return &s_instance;				\
   }

#define GET_SINGLE(classname)   classname::Get_Instance()
