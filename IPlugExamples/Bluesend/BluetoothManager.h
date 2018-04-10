#pragma once

#include <memory>
#include <vector>
#include "DeviceINQ.h"
#include "BTSerialPortBinding.h"

class BluetoothManager
{
public:
	BluetoothManager();
	~BluetoothManager();

	// Scan for new devices, and return a vector of found devices.
	std::vector<device> Scan();

	// Opens a serial connection to the previously-scanned device in the devices
	// vector corresponding to the passed-in index.
	void ConnectDevice(int deviceArrayIdx);

	// Disconnect the connected device, by closing the serial connection &
	// doing other cleanup tasks.
	void DisconnectDevice();

	// Write a buffer of chars to the connected device, passing in the length.
	void Write(const char *buffer, int length);

	std::vector<device> GetDevices();
	device GetConnectedDevice();
	bool IsConnectedToDevice();
	bool IsInitScanDone();

private:
	std::unique_ptr<DeviceINQ> mInq;
	std::unique_ptr<BTSerialPortBinding> mSerialPort;
	std::vector<device> mDevices;
	device mConnectedDevice;
	bool mConnectedToDevice;
	bool mInitScanDone;
};

