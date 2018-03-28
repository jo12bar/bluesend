#pragma once

#include <memory>
#include <vector>
#include "DeviceINQ.h"

class BluetoothManager
{
public:
	BluetoothManager();
	~BluetoothManager();

	// Scan for new devices, and return a vector of found devices.
	std::vector<device> Scan();

	std::vector<device> GetDevices();
private:
	std::unique_ptr<DeviceINQ> mInq;
	std::vector<device> mDevices;
};

