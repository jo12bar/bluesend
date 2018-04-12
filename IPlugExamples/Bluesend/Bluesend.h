#ifndef __BLUESEND__
#define __BLUESEND__

#include "BluetoothManager.h"
#include "IPlug_include_in_plug_hdr.h"

// The factor to throttle the sending of data by, as a fraction of the sampling rate.
// E.g: Setting this to `60.` will cause data to only be sent at 1/60th the sample rate.
// So, if the sample rate was 44100, then the data would only be sent every 735 frames.
#define DATA_THROTTLE_FACTOR 240.

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
	int mDataThrottleCounter;
	int mDataThrottleCounterMax;
};

#endif
