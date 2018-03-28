#include "BluetoothManager.h"

#include <memory>
#include <vector>
#include "DeviceINQ.h"

BluetoothManager::BluetoothManager()
	: mInq(DeviceINQ::Create())
{
}


BluetoothManager::~BluetoothManager()
{
}

std::vector<device> BluetoothManager::Scan()
{
	mDevices = mInq->Inquire();
	return mDevices;
}

std::vector<device> BluetoothManager::GetDevices()
{
	return mDevices;
}
