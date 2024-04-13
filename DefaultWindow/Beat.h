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
	bool m_bOnHeart; // 범위 안에 들어왔는지
	bool m_bDisplay; // 화면에 보여줄건지 (뒤로 튀어나옴;;)
	bool m_bSuccess; // 성공 했는지
};
