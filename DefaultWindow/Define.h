#pragma once
#include <cassert>

extern HWND g_hWnd;

#pragma region 매크로


#define			WINCX		800
#define			WINCY		600

#define			PURE		= 0

#define			OBJ_NOEVENT	0
#define			OBJ_DEAD	1

// 비트관련
#define			BEAT_POSY			518
#define			HEART_POSY			500
#define			HEART_EXTRA_SIZE	20

#define			PI			3.141592f
#define			MAGENTA		RGB(255, 0, 144)

#define			VK_MAX		0xff

// 아이템 (인벤토리, 필드) 크기
#define			ITEMCX		48
#define			ITEMCY		48

// 인벤토리 아이템 슬롯 크기
#define			SLOTCX		60
#define			SLOTCY		66

// 슬롯 간격
#define			SLOT_INTER  4

// 인벤토리 슬롯 최초 위치
#define			SLOTPOSX	50
#define			SLOTPOSY	50

// 목숨 표시 위치
#define			P_LIFE_X	700
#define			P_LIFE_Y	50

// 재화 표시 위치
#define			COINPOSX	700
#define			COINPOSY	48

// 플레이어 목숨, 코인, 다이아 UI 크기
#define			GUICX		48
#define			GUICY		44

// 데스메탈 이동 제한 범위
#define			DM_RANGE_X	11
#define			DM_RANGE_Y	9

// 데스메탈 맵 중앙
#define			DM_CENTER_X 744
#define			DM_CENTER_Y 504

// 데스메탈 이동, 텔레포트등 검색 범위
#define			DM_SEARCH_START_X 504.f
#define			DM_SEARCH_START_Y 312.f

// 타일 크기
#define			TILECX		48
#define			TILECY		48

// 벽 크기
#define			WALLCX		48
#define			WALLCY		96

// 타일 개수
#define			TILEX		30
#define			TILEY		30

#define			FLOAT_RANGE	0.001f

#pragma endregion

#pragma region enum 아이디

// 오브젝트 종류
enum OBJID
{
	OBJ_MOUSE, // 마우스
	OBJ_TRAP, // 함정
	OBJ_ITEM, // 아이템
	OBJ_PLAYER, // 플레이어
	OBJ_MONSTER, // 몬스터
	OBJ_EFFECT, // 효과
	OBJ_WALL, // 벽
	OBJ_BEAT, // 비트는 따로 관리(충돌체크..)
	OBJ_UI, // UI
	OBJ_END
};

// 렌더링 그룹
enum RENDERID
{
	RENDER_BACKGROUND, // 배경
	RENDER_GAME_OBJECT, // 게임 오브젝트
	RENDER_STRUCTURE, // 구조물들
	RENDER_EFFECT, // 이펙트
	RENDER_UI, // UI
	RENDER_BEAT_HEART, // 비트는 따로 ..
	RENDER_MOUSE,
	RENDER_END
};

// 씬 종류
enum SCENEID
{
	SC_MENU,
	SC_EDIT, // 에디터(메뉴에서 숨겨둠)
	SC_STAGE,
	SC_STAGE_01,
	SC_STAGE_02,
	SC_BOSS_STAGE,
	SC_LOADING,
	SC_END
};

// 방향 종류
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

// 이펙트 종류
enum EFFECTID
{
	EFFECT_DAGGER,
	EFFECT_SWORD,
	EFFECT_SWORD_T,
	EFFECT_FIREBALL,
	EFFECT_TOOL, // 도구들
	EFFECT_MONSTER_ATTACK,
	EFFECT_END,
};

// 코인 배수
enum COIN_X
{
	COIN_X_1,
	COIN_X_2,
	COIN_END
};

// 사운드 종류
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

// 아이템 종류
enum ITEMID
{
	ITEM_SHOVEL,
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_CAP,
	ITEM_BOMB,
	ITEM_KEY,
	ITEM_SPELL,

	ITEM_COIN, // 돈
	ITEM_DIAMOND, // 다이아
	ITEM_SPECIAL, // UI
	ITEM_MAX
};

// 벽 종류
enum WALLID
{
	NORMAL, // 일반 벽
	STONE, // 돌 벽
	DOOR_V, // 수직 문
	DOOR_H, // 수평 문
	LEVEL, // 레벨 경계
	SHOP, // 상점 문
	BOSS, // 보스 방 벽
};

// 함정 종류
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
	int iFrameStart; // 몇 번 인덱스부터 시작할 것인가
	int iFrameEnd;
	int iMotion;
	DWORD dwSpeed;
	DWORD dwTime;
};

// 메모리 반납 함수
template <typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

// 베열 메모리 반납 함수
template <typename T>
void Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}

// 오브젝트 삭제 함수객체
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
