#ifndef __BLUESEND__
#define __BLUESEND__

#include "BluetoothManager.h"
#include "IPlug_include_in_plug_hdr.h"

class Bluesend : public IPlug
{
public:
	Bluesend(IPlugInstanceInfo instanceInfo);
	~Bluesend();

	void AttachTitleText(IGraphics* pGraphics, const char* text);
	void AttachConnectedToText(IGraphics* pGraphics, const char* label);

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

	void ConnectToDevice();

private:
	ITextControl* mConnectedDeviceText;
	BluetoothManager* mBlueManager;
};

#endif
