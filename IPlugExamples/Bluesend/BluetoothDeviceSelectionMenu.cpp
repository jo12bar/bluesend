#include "BluetoothDeviceSelectionMenu.h"



BluetoothDeviceSelectionMenu::BluetoothDeviceSelectionMenu(IPlugBase *pPlug, IRECT pRect)
	: IPanelControl(pPlug, pRect, &COLOR_BLUE)
{}


BluetoothDeviceSelectionMenu::~BluetoothDeviceSelectionMenu()
{}

bool BluetoothDeviceSelectionMenu::Draw(IGraphics * pGraphics)
{
	pGraphics->FillIRect(&COLOR_WHITE, &mRECT);

	// Compute points of the triangle to draw:
	int ax = mRECT.R - 8;
	int ay = mRECT.T + 4;
	int bx = ax + 4;
	int by = ay;
	int cx = ax + 2;
	int cy = ay + 2;

	pGraphics->FillTriangle(&COLOR_GRAY, ax, ay, bx, by, cx, cy, &mBlend);

	return true;
}

void BluetoothDeviceSelectionMenu::OnMouseDown(int x, int y, IMouseMod * pMod)
{
	// If left mouse button clicked, display the popup menu.
	if (pMod->L)
	{
		doPopupMenu();
	}

	// Seems to need the below - still need to figure out *why*.
	Redraw();
	SetDirty();
}

void BluetoothDeviceSelectionMenu::doPopupMenu()
{
	IPopupMenu menu;

	IGraphics* gui = mPlug->GetGUI();

	menu.AddItem("Foo");
	menu.AddItem("Bar");
	menu.AddItem("Baz");
	menu.AddSeparator();
	menu.AddItem("Spam");
	menu.AddItem("Eggs");
	menu.AddItem("Spam and eggs");

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
