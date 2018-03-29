#pragma once

#include <vector>
#include "BluetoothManager.h"
#include "IControl.h"
#include "wdlstring.h"

#define BLUETOOTH_DEVICE_SELECTION_MENU__ITEM_MAX_LEN 1024

class BluetoothDeviceSelectionMenu :
	public IPanelControl
{
public:
	BluetoothDeviceSelectionMenu(IPlugBase *pPlug, IRECT pRect, BluetoothManager* pBlueManager, const char* pLabel);
	~BluetoothDeviceSelectionMenu();

	bool Draw(IGraphics* pGraphics);

	void OnMouseDown(int x, int y, IMouseMod* pMod);

	void doPopupMenu(std::vector<device> devices);

private:
	IPlugBase* mPlug;
	WDL_String mLabel;
	BluetoothManager* mBlueManager;
};

