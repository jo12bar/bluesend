#include <sstream>
#include <vector>
#include "Bluesend.h"
#include "BluetoothDeviceSelectionMenu.h"
#include "BluetoothException.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include "theming.h"

const int kNumPrograms = 1;

enum EParams
{
	kDevicesArrayIndex,
	kNumParams
};

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,

	kTitleX = 10,
	kTitleY = 10,
	kTitleFontSize = 28,

	kLabelX = kTitleX,
	kLabelY = kTitleY + kTitleFontSize + 10,
	kLabelFontSize = 16,

	kConnectedDeviceX = kTitleX,
	kConnectedDeviceY = kLabelY + kLabelFontSize + 10,
	kConnectedDeviceFontSize = kLabelFontSize,

	kDeviceMenuX = kTitleX,
	kDeviceMenuY = kConnectedDeviceY + kConnectedDeviceFontSize + 10,
	kDeviceMenuWidth = kWidth - 20,
	kDeviceMenuHeight = kLabelFontSize + 22
};

Bluesend::Bluesend(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
	, mDataThrottleCounter(0)
{
	TRACE;

	GetParam(kDevicesArrayIndex)->InitInt("Devices Array Index", -1, 0, MAX_AMOUNT_DEVICES);

	mBlueManager = new BluetoothManager();

	IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
	pGraphics->AttachPanelBackground(&COLOR_BLACK);

	AttachTitleText(pGraphics, "Bluesend");

	AttachConnectedToText(pGraphics, "Connected to device:");

	// Bluetooth device selector popup menu
	pGraphics->AttachControl(new BluetoothDeviceSelectionMenu(
		this,
		kDevicesArrayIndex,
		IRECT(
			kDeviceMenuX,
			kDeviceMenuY,
			kDeviceMenuX + kDeviceMenuWidth,
			kDeviceMenuY + kDeviceMenuHeight
		),
		mBlueManager,
		"Connect to device"
	));
	AttachGraphics(pGraphics);

	//MakePreset("preset 1", ... );
	MakeDefaultPreset((char *) "-", kNumPrograms);

	mDataThrottleCounterMax = (int)(GetSampleRate() / DATA_THROTTLE_FACTOR);
}

Bluesend::~Bluesend() {}

void Bluesend::AttachTitleText(IGraphics* pGraphics, const char* text)
{
	IRECT rect(kTitleX, kTitleY, kWidth - 10, kHeight - 10);
	IText textProps(kTitleFontSize, &TITLE_COLOR, 0, IText::kStyleBold);
	pGraphics->AttachControl(new ITextControl(this, rect, &textProps, text));
}

void Bluesend::AttachConnectedToText(IGraphics* pGraphics, const char* label)
{
	// Label text
	IRECT labelRect(kLabelX, kLabelY, kWidth - 10, kHeight - 10);
	IText labelProps(kLabelFontSize, &LABEL_COLOR, 0, IText::kStyleItalic, IText::kAlignNear);
	pGraphics->AttachControl(new ITextControl(this, labelRect, &labelProps, label));

	// Connected device text
	IRECT connectedDeviceRect(kConnectedDeviceX, kConnectedDeviceY, kWidth - 10, kHeight - 10);
	IText connectedProps(
			kConnectedDeviceFontSize,
			&ACCENT_TEXT_COLOR,
			0,
			IText::kStyleNormal,
			IText::kAlignNear
	);
	mConnectedDeviceText = new ITextControl(
			this,
			connectedDeviceRect,
			&connectedProps,
			"(None. Connect below!)"
	);
	pGraphics->AttachControl(mConnectedDeviceText);
}

void Bluesend::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	// Mutex is already locked for us.
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];
	std::vector<double> btOutput;
	bool connected = mBlueManager->IsConnectedToDevice();
	bool ready = mDataThrottleCounter == mDataThrottleCounterMax - 1;

	for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
	{
		// Just pass audio signals through.
		*out1 = *in1;
		*out2 = *in2;

		if (connected && ready) btOutput.push_back(*in1);
	}

	if (connected && ready)
	{
		try
		{
			std::vector<char> buff;

			// Transform double vector into char vector for sending.
			for (double datum : btOutput) {
				char arr[sizeof(datum)];
				memcpy(arr, &datum, sizeof(datum));
				buff.insert(buff.end(), arr, arr + sizeof(datum));
			}

			mBlueManager->Write(buff.data(), buff.size());
		}
		catch (const BluetoothException& e)
		{
			DBGMSG(e.what());
		}
	}

	if (ready)
	{
		mDataThrottleCounter = 0;
	}
	else
	{
		mDataThrottleCounter++;
	}
}

void Bluesend::ConnectToDevice()
{
	const std::vector<device> devices = mBlueManager->GetDevices();

	int i = GetParam(kDevicesArrayIndex)->Int();

	if (devices.size() > 0)
	{
		// Update connected device text
		std::stringstream buff;
		buff << devices[i].name << " (" << devices[i].address << ")";
		mConnectedDeviceText->SetTextFromPlug((char*)(buff.str().c_str()));

		// Connect to device with BluetoothManager
		try
		{
			if (mBlueManager->IsConnectedToDevice()) mBlueManager->DisconnectDevice();
			mBlueManager->ConnectDevice(i);

			// Send the sample rate (double has to be converted to char array).
			double sampleRate = GetSampleRate();
			char arr[sizeof(sampleRate)];
			memcpy(arr, &sampleRate, sizeof(sampleRate));
			mBlueManager->Write(arr, sizeof(sampleRate));
		}
		catch (const BluetoothException& e)
		{
			DBGMSG(e.what());
		}
	}
}

void Bluesend::Reset()
{
	TRACE;
	IMutexLock lock(this);
}

void Bluesend::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch (paramIdx)
	{
	case kDevicesArrayIndex:
		ConnectToDevice();
		break;
	default:
		break;
	}
}
