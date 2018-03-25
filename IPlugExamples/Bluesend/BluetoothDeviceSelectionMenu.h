#pragma once
#include "IControl.h"
class BluetoothDeviceSelectionMenu :
	public IPanelControl
{
public:
	BluetoothDeviceSelectionMenu(IPlugBase *pPlug, IRECT pRect);
	~BluetoothDeviceSelectionMenu();

	bool Draw(IGraphics* pGraphics);

	void OnMouseDown(int x, int y, IMouseMod* pMod);

	void doPopupMenu();
};

