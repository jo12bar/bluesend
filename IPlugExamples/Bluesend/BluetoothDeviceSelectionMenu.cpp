#include <sstream>
#include <vector>
#include "BluetoothDeviceSelectionMenu.h"
#include "BluetoothManager.h"
#include "IControl.h"
#include "theming.h"

static const int borderThickness = 1;

BluetoothDeviceSelectionMenu::BluetoothDeviceSelectionMenu(IPlugBase *pPlug, IRECT pRect, BluetoothManager* pBlueManager, const char* pLabel)
	: IPanelControl(pPlug, pRect, &COLOR_BLACK)
	, mPlug(pPlug)
	, mBlueManager(pBlueManager)
{
	mLabel.Set(pLabel);
}


BluetoothDeviceSelectionMenu::~BluetoothDeviceSelectionMenu()
{}

bool BluetoothDeviceSelectionMenu::Draw(IGraphics * pGraphics)
{
	IRECT interiorRect = mRECT.GetPadded(-borderThickness);

	pGraphics->FillIRect(&COLOR_LIGHT_BLUE, &mRECT);
	pGraphics->FillIRect(&COLOR_BLACK, &interiorRect);

	IText labelProps(16, &LABEL_COLOR);
	IRECT labelRect(
		interiorRect.L,
		interiorRect.MH() - (16 / 2),
		interiorRect.R,
		interiorRect.MH() + (16 / 2)
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
		int itemChosen = menu.GetChosenItemIdx();

		printf("Chose menu item with index %i\n", itemChosen);

		switch (itemChosen)
		{
		case 0:
			printf("Foo\n");
			break;
		case 1:
			printf("Bar\n");
			break;
		case 2:
			printf("Baz\n");
			break;
		case 3:
			printf("Spam\n");
			break;
		case 4:
			printf("Eggs\n");
			break;
		case 5:
			printf("Spam and eggs\n");
			break;
		default:
			break;
		}
	}
}
