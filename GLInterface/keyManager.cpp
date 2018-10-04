#include "stdafx.h"
#include "keyManager.h"

keyManager::keyManager(void)
{
}

keyManager::~keyManager(void)
{
}

HRESULT keyManager::init(void)
{
	for (int i = 0; i < KEYMAX; i++)
	{
		this->getKeyUp().set(i, false);
		this->getKeyDown().set(i, false);
	}

	return S_OK;
}

void keyManager::release(void)
{
}

bool keyManager::isOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!_keyDown[key])
		{
			_keyDown.set(key, true); // <- 예전꺼에는 get 이었어! 여기가 set 이어야 함. 왜일까?
			return true;
		}
	}
	else _keyDown.set(key, false);	// 여기도

	return false;
}

bool keyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true); // 여기도
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false); // 여기도
			return true;
		}
	}
	return false;
}

bool keyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;	// 눌려져 있으면 0x8000 으로 검사하면 항상 트루겠징?
	return false;
}

bool keyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;	// 사이트를 봤다면 더이상의 설명은 생략한다
	return false;
}
