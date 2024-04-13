#pragma once
#include "Monster.h"

class CDeathMetal : public CMonster
{
private:
	enum PHASE
	{
		P_FIRST,
		P_SECOND,
		P_THIRD,
		P_FOURTH,
		P_END
	};

public:
	CDeathMetal();
	~CDeathMetal() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

protected:
	void Motion_Change() override;
	void Do_Act() override;
	void Move();

protected:
	void Summon();
	void Chase();
	void Escape();
	void Teleport();
	bool On_Range();

	void Heal(int _iValue)
	{
		m_iHp += _iValue;
		if (m_iHp > m_iMaxHp)
			m_iHp = m_iMaxHp;
	}

	void Chase_Or_Escape();
	void Fire();
	void Run_Offside();

	void Update_Phase();
	void Play_Sound();

private:
	void Move_Frame();

private:
	// int		m_iAlphaPosY;	// 렌더링 Y값 조정
	bool m_bOnShield; // 방패를 들고있을 경우
	int m_iStartFrame; // 애니메이션 시작 위치

	PHASE m_eCurPhase;
	PHASE m_ePrePhase;

	UINT m_iSoundCnt;
	UINT m_iSoundMaxCnt;
	UINT m_iSummonCnt;
	UINT m_iSummonMaxCnt;
	bool m_bCanSummon;
	bool m_bFirstDetect;
};
