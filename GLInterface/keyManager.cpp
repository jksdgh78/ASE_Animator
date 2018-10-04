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
			_keyDown.set(key, true); // <- ���������� get �̾���! ���Ⱑ set �̾�� ��. ���ϱ�?
			return true;
		}
	}
	else _keyDown.set(key, false);	// ���⵵

	return false;
}

bool keyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true); // ���⵵
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false); // ���⵵
			return true;
		}
	}
	return false;
}

bool keyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;	// ������ ������ 0x8000 ���� �˻��ϸ� �׻� Ʈ���¡?
	return false;
}

bool keyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;	// ����Ʈ�� �ôٸ� ���̻��� ������ �����Ѵ�
	return false;
}
