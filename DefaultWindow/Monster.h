#pragma once
#include "Obj.h"

class CMonster : public CObj
{
public:
	enum STATE
	{
		IDLE,
		MOVE,
		ATTACK,
		SUMMON,
		HIT,
		CHARGE,
		STUN,
		ST_DEAD
	};

public:
	CMonster();
	~CMonster() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	virtual void Do_Act();

public:
	void Render_Heart(HDC hDC);
	void Render_Heart_Boss(HDC hDC, int _iAlphaX = 0, int _iAlphaY = 0);
	void Update_Heart();
	void Set_Heart(int _iHeart);
	void Throw(TCHAR* _pKey, int _iCX, int iCY);

protected:
	virtual void Motion_Change();
	void Drop_Item();
	void Leave();

protected:
	STATE m_ePreState;
	STATE m_eCurState;

	int m_iAlphaPosY;
	UINT m_iNeedToDoAct;

	vector<CObj*> m_vecHeart;
};
