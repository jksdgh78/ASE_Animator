#pragma once

#include <bitset>

using namespace std;

#define g_pKeyManager keyManager::GetInstance()

#define KEYMAX 256

class keyManager 
{
private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;

	SINGLETONE(keyManager);

public:
	HRESULT init(void);
	void release(void);

	bool isOnceKeyDown(int key);
	bool isOnceKeyUp(int key);
	bool isStayKeyDown(int key);
	bool isToggleKey(int key);

	bitset<KEYMAX> getKeyUp() {return _keyUp;}
	bitset<KEYMAX> getKeyDown() {return _keyDown;}

	void setKeyDown(int key, bool state) {_keyDown.set(key, state);}
	void setKeyUp(int key, bool state) {_keyUp.set(key, state);}
};
