#pragma once

#include <vector>
#include "BluetoothManager.h"
#include "IControl.h"
#include "wdlstring.h"

// I find it very unlikely that there'll be more than 1024 nearby bluetooth
// devices. That actually is kinda worrying. Of course, I'd rather avoid defining
// this if I could, but IParam::InitInt unfortunently requires a max value.
#define MAX_AMOUNT_DEVICES 1024

class BluetoothDeviceSelectionMenu :
	public IControl
{
public:
	BluetoothDeviceSelectionMenu(
		IPlugBase *pPlug,
		int pParamIdx,
		IRECT pRect,
		BluetoothManager* pBlueManager,
		const char* pLabel
	);
	~BluetoothDeviceSelectionMenu();

	bool Draw(IGraphics* pGraphics);

	void OnMouseDown(int x, int y, IMouseMod* pMod);

	void doPopupMenu(std::vector<device> devices);

private:
	// IPlug requires parameter values to be between 0.0 and 1.0 (something to do
	// with VST's requirements), but we want to pass up the index of the chosen
	// device via a parameter. This function will scale it.
	// (Scaling function credit: https://stackoverflow.com/a/5295202/5013691)
	double ScaleChosenDeviceIndex(int i);

	WDL_String mLabel;
	BluetoothManager* mBlueManager;
};

