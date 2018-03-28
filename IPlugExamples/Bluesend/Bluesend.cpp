#include "Bluesend.h"
#include "BluetoothDeviceSelectionMenu.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include "theming.h"

const int kNumPrograms = 1;

enum EParams
{
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kTitleX = 10,
  kTitleY = 10,
  kTitleFontSize = 28,

  kConnectedToX = kTitleX,
  kConnectedToY = kTitleY + kTitleFontSize + 10,
  kLabelFontSize = 16,

  kDeviceMenuX = kTitleX,
  kDeviceMenuY = kConnectedToY + kLabelFontSize + 10,
  kDeviceMenuWidth = kWidth - 20,
  kDeviceMenuHeight = kLabelFontSize + 22
};

Bluesend::Bluesend(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
{
  TRACE;

  mBlueManager = new BluetoothManager();

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_BLACK);

  AttachTitleText(pGraphics, "Bluesend");
  
  AttachConnectedToText(pGraphics, "Connected to device:");

  // Bluetooth device selector popup menu
  pGraphics->AttachControl(new BluetoothDeviceSelectionMenu(
	  this,
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
	IRECT rect(kConnectedToX, kConnectedToY, kWidth - 10, kHeight - 10);

	// Label text
	IText labelProps(kLabelFontSize, &LABEL_COLOR, 0, IText::kStyleItalic, IText::kAlignNear);
	pGraphics->AttachControl(new ITextControl(this, rect, &labelProps, label));

	// Connected device text
	// TODO: Make this dynamic!
	IText connectedProps(kLabelFontSize, &ACCENT_TEXT_COLOR, 0, IText::kStyleNormal, IText::kAlignFar);
	pGraphics->AttachControl(new ITextControl(this, rect, &connectedProps, "Placeholder McDeviceface"));
}

void Bluesend::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.

  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];

  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
  {
	// Just pass audio signals through.
    *out1 = *in1;
	*out2 = *in2;
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
    default:
      break;
  }
}
