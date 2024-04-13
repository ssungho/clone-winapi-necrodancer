#pragma once
#include "Obj.h"

class CBeat : public CObj
{
public:
	CBeat();
	~CBeat() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public: // Getter & Setter
	void Set_OnHeart() { m_bOnHeart = true; }
	void Set_Off() { m_bDisplay = false; }

	bool Get_OnHeart() const { return m_bOnHeart; }

private:
	bool m_bOnHeart; // ���� �ȿ� ���Դ���
	bool m_bDisplay; // ȭ�鿡 �����ٰ��� (�ڷ� Ƣ���;;)
	bool m_bSuccess; // ���� �ߴ���
};
