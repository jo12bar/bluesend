#include <sstream>
#include <vector>
#include "BluetoothDeviceSelectionMenu.h"
#include "BluetoothManager.h"
#include "IControl.h"
#include "theming.h"

static const int borderThickness = 1;

BluetoothDeviceSelectionMenu::BluetoothDeviceSelectionMenu(
	IPlugBase *pPlug,
	int pParamIdx,
	IRECT pRect,
	BluetoothManager* pBlueManager,
	const char* pLabel
)
	: IControl(pPlug, pRect, pParamIdx)
	, mBlueManager(pBlueManager)
{
	mLabel.Set(pLabel);
}


BluetoothDeviceSelectionMenu::~BluetoothDeviceSelectionMenu()
{}

bool BluetoothDeviceSelectionMenu::Draw(IGraphics * pGraphics)
{
	IRECT interiorRect = mRECT.GetPadded(-borderThickness);

	IColor colorButtonBg = COLOR_LIGHT_BLUE;
	float darkenFactor = 0.15f;
#pragma warning(push)
#pragma warning(disable: 4244)
	colorButtonBg.R *= darkenFactor;
	colorButtonBg.G *= darkenFactor;
	colorButtonBg.B *= darkenFactor;
#pragma warning(pop)

	pGraphics->FillIRect(&COLOR_LIGHT_BLUE, &mRECT);
	pGraphics->FillIRect(&colorButtonBg, &interiorRect);

	IText labelProps(16, &LABEL_COLOR);
	IRECT labelRect(
		interiorRect.L,
		(int)(interiorRect.MH()) - (16 / 2),
		interiorRect.R,
		(int)(interiorRect.MH()) + (16 / 2)
	);
	ITextControl* labelControl = new ITextControl(mPlug, labelRect, &labelProps, mLabel.Get());
	// Prevent label from interfering with mouse clicks.
	labelControl->SetTargetArea(IRECT());
	pGraphics->AttachControl(labelControl);

	return true;
}

void BluetoothDeviceSelectionMenu::OnMouseDown(int x, int y, IMouseMod * pMod)
{
	// If left mouse button clicked, display the popup menu.
	if (pMod->L)
	{
		doPopupMenu(mBlueManager->Scan());
	}

	// Seems to need the below - still need to figure out *why*.
	Redraw();

	// Trigger plug to recieve an updated param.
	SetDirty();
}

void BluetoothDeviceSelectionMenu::doPopupMenu(std::vector<device> devices)
{
	IPopupMenu menu;

	IGraphics* gui = mPlug->GetGUI();

	menu.AddItem(new IPopupMenuItem("Detected devices:", IPopupMenuItem::kTitle));

	for (const auto& d : devices) {
		std::stringstream buff;
		buff << d.name << " (" << d.address << ")";
		menu.AddItem(buff.str().c_str());
	}

	if (gui->CreateIPopupMenu(&menu, &mRECT))
	{
		// Subtract one to account for the menu's title.
		int itemChosen = menu.GetChosenItemIdx() - 1;
		mValue = ScaleChosenDeviceIndex(itemChosen);
		mPlug->SetParameterFromGUI(mParamIdx, mValue);
	}
}

double BluetoothDeviceSelectionMenu::ScaleChosenDeviceIndex(int i)
{
	return (double)(i) / (double)(MAX_AMOUNT_DEVICES);
}
