#pragma once

#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	RECT Get_Rect() { return m_tRect; }
	INFO Get_Info() { return m_tInfo; }

	bool Get_Dead() const { return m_bDead; }
	int Get_Hp() const { return m_iHp; }
	int Get_MaxHp() const { return m_iMaxHp; }
	int Get_Attack() const { return m_iAttack; }
	int Get_Defense() const { return m_iDefense; }
	int Get_Coin() const { return m_iCoin; }
	int Get_Diamond() const { return m_iDiamond; }
	bool On_Attacked() const { return m_bAttacked; }
	bool Is_Flip() const { return m_bIsFlip; }

	OBJID Get_ObjID() const { return m_eObjID; }
	RENDERID Get_GroupID() const { return m_eRenderID; }
	DIRECTION Get_Dir() const { return m_eDir; }
	SCENEID Get_DeadPoint() const { return m_eDeadPoint; }

	TCHAR* Get_CopyKey()
	{
		auto copy = new TCHAR[lstrlen(m_pFrameKey) + 1];
		lstrcpy(copy, m_pFrameKey);
		return copy;
	}

	const TCHAR* Get_FrameKey() { return m_pFrameKey; }

	void Set_Scale(float _fX, float _fY)
	{
		m_tInfo.fCX = _fX;
		m_tInfo.fCY = _fY;
	}

	void Set_Dir(DIRECTION eDir)
	{
		m_eDir = eDir;
	}

	void Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}

	void Set_Pos(const CObj* _pTile)
	{
		m_tInfo.fX = _pTile->m_tInfo.fX;
		m_tInfo.fY = _pTile->m_tInfo.fY;
	}

	void Set_PosY(float _fY) { m_tInfo.fY += _fY; }
	void Set_PosX(float _fX) { m_tInfo.fX += _fX; }

	void Set_Hp(int _iHp) { m_iHp = _iHp; }
	void Set_MaxHp(int _iHp) { m_iMaxHp = _iHp; }
	void Set_Dead() { m_bDead = true; }
	void Set_Defense(int _iDefense) { m_iDefense = _iDefense; }
	void Set_Attack(int _iAttack) { m_iAttack = _iAttack; }
	void Set_Coin(int _iCoin) { m_iCoin += _iCoin; } // += 로 작성
	void Set_Diamond(int _iDiamond) { m_iDiamond += _iDiamond; } // += 로 작성
	void Set_IsAttacked() { m_bAttacked = true; }
	void Set_Bpm(int _iBpm) { m_iBpm = _iBpm; }
	void Set_Flip(bool _bFlip) { m_bIsFlip = _bFlip; }
	void Set_ObjID(OBJID _eID) { m_eObjID = _eID; }

	void Add_Beat_Count() { ++m_iBeatCount; }
	void Reset_Beat_Count() { m_iBeatCount = 0; }

	void Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	void Set_Frame(FRAME _tFrame) { m_tFrame = _tFrame; }
	void Set_FrameKey(TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }

	void On_Damaged(int _iDamage)
	{
		m_iHp -= _iDamage;
		if (m_iHp <= 0)
		{
			m_bDead = true;
		}
	}


	// 마주보는가?
	bool On_Face(const CObj* _pTarget) const
	{
		switch (m_eDir)
		{
		case LEFT:
			if (_pTarget->m_eDir == RIGHT)
				return true;
			else return false;
		case RIGHT:
			if (_pTarget->m_eDir == LEFT)
				return true;
			else return false;
		case UP:
			if (_pTarget->m_eDir == DOWN)
				return true;
			else return false;
		case DOWN:
			if (_pTarget->m_eDir == UP)
				return true;
			else return false;
		}

		return false;
	}

	void Draw_Particle();
	void Draw_Particle(TCHAR* pKey);

public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

protected:
	void Update_Rect();
	void Move_Frame();

protected:
	INFO m_tInfo;
	RECT m_tRect;
	FRAME m_tFrame;
	DIRECTION m_eDir;
	RENDERID m_eRenderID;

	float m_fPreYPos; // 점프 전 위치
	bool m_bMove; // 움직이고 있는지
	float m_fMoveTime; // 움직인 시간
	float m_fSpeed;
	bool m_bDead;
	int m_iMaxHp;
	int m_iHp;
	int m_iAttack;
	int m_iDefense;
	int m_iCoin;
	int m_iDiamond;
	bool m_bDetected;
	bool m_bAttacked;
	int m_iBpm;

	//				비트 카운트
	UINT m_iBeatCount;
	bool m_bIsFlip;

	OBJID m_eObjID;
	CObj* m_pTarget;
	const TCHAR* m_pFrameKey = L"";
	TCHAR* m_pParticleKey = L"";

	//				반시계
	int iDx[4] = {0, -1, 0, 1};
	int iDy[4] = {-1, 0, 1, 0};
	SCENEID m_eDeadPoint;
};
