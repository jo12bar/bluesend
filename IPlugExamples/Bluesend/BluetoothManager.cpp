#include "BluetoothManager.h"

#include <memory>
#include <vector>
#include "DeviceINQ.h"

BluetoothManager::BluetoothManager()
	: mInq(DeviceINQ::Create())
	, mInitScanDone(false)
	, mConnectedToDevice(false)
{
}


BluetoothManager::~BluetoothManager()
{
	if (mConnectedToDevice) DisconnectDevice();
}

std::vector<device> BluetoothManager::Scan()
{
	mDevices = mInq->Inquire();
	if (!mInitScanDone) mInitScanDone = true;
	return mDevices;
}

void BluetoothManager::ConnectDevice(int deviceArrayIdx)
{
	if (!mInitScanDone || mConnectedToDevice) return;

	mConnectedDevice = mDevices[deviceArrayIdx];
	// int channelID = mInq->SdpSearch(mConnectedDevice.address);
	int channelID = 1;
	mSerialPort = std::unique_ptr<BTSerialPortBinding>(BTSerialPortBinding::Create(mConnectedDevice.address, channelID));
	mSerialPort->Connect();
	mConnectedToDevice = true;
}

void BluetoothManager::DisconnectDevice()
{
	if (!mInitScanDone || !mConnectedToDevice) return;

	mSerialPort->Close();
	mConnectedToDevice = false;
}

void BluetoothManager::Write(const char *buffer, int length)
{
	if (!mInitScanDone || !mConnectedToDevice) return;

	mSerialPort->Write(buffer, length);
}

std::vector<device> BluetoothManager::GetDevices()
{
	return mDevices;
}

device BluetoothManager::GetConnectedDevice()
{
	return mConnectedDevice;
}

bool BluetoothManager::IsConnectedToDevice()
{
	return mConnectedToDevice;
}

bool BluetoothManager::IsInitScanDone()
{
	return mInitScanDone;
}
