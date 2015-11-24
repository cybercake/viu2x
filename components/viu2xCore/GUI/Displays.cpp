/* Copyright (C) Hao Qin. All rights reserved. */

#include "Displays.h"

#include <SetupApi.h>
#include <cfgmgr32.h>

namespace v2x {

	/////////////
	// Display //
	/////////////

	Display::Display(const DisplayId & displayId, //
		const String & displayName, //
		const Size & physicalSizeInMm, //
		const Vector2D & resolutionInDpi, //
		const Rect32I & screenAreaInPx, //
		const Rect32I & workAreaInPx, //
		const bool & isPrimary) : //
		m_displayId(displayId), //
		m_displayName(displayName), //
		m_physicalSizeInMm(physicalSizeInMm), //
		m_resolutionInDpi(resolutionInDpi), //
		m_screenAreaInPx(screenAreaInPx), //
		m_workAreaInPx(workAreaInPx), //
		m_isPrimary(isPrimary) {
	}

	Display::~Display() {
	}

	/// Identifier of the display
	const Display::DisplayId & Display::getDisplayId() const {
		return m_displayId;
	}

	/// Name of the display
	const String & Display::getDisplayName() const {
		return m_displayName;
	}

	/// Physical size of the screen, given in mm.
	const Size & Display::getPhysicalSizeInMm() const {
		return m_physicalSizeInMm;
	}

	/// Specified resolution, not the actual one.
	const Vector2D & Display::getResolutionInDpi() const {
		return m_resolutionInDpi;
	}

	/// Specified resolution, not the actual one.
	Vector2D Display::getActualResolutionInDpi() const {
		return Vector2D(m_screenAreaInPx.getWidth() * 25.4 / m_physicalSizeInMm.width, //
			m_screenAreaInPx.getHeight() * 25.4 / m_physicalSizeInMm.height);
	}

	/// The region where the display is mapped to.
	const Rect32I & Display::getScreenAreaInPx() const {
		return m_screenAreaInPx;
	}

	/// The region where the display is mapped to.
	const Rect32I & Display::getWorkAreaInPx() const {
		return m_workAreaInPx;
	}

	/// A flag indicating if the display is the primary one.
	const bool & Display::getIsPrimary() const {
		return m_isPrimary;
	}

	//////////////
	// Displays //
	//////////////

	Displays::Displays() {
		update();
	}

	Displays::~Displays() {
	}

	void Displays::update() {

		m_displays.clear();

#ifdef V2X_WINDOWS
		EnumDisplayMonitors(NULL, NULL, &monitorEnumProcCallback, (LPARAM)&m_displays);
#else
#error Not implemented!
#endif
	}

	size_t Displays::count() const {
		return m_displays.size();
	}

	Display::ConstShared Displays::getDisplay(size_t index) const {

		if (index >= m_displays.size())
			throw Exception(L"Displays::getDisplay: Index %u out of range!", index);

		return m_displays[index];
	}

	Display::ConstShared Displays::getPrimaryDisplay() const {

		for (DisplayList::const_iterator i = m_displays.begin(); i != m_displays.end(); ++i)
			if ((*i)->getIsPrimary())
				return *i;

		throw Exception(L"Displays::getPrimaryDisplay(): No primary display found!");
	}

#ifdef V2X_WINDOWS

	/// Code from http://ofekshilon.com/2014/06/19/reading-specific-monitor-dimensions/
	/// The code is NOT fully tested by author!!!
	String Get2ndSlashBlock(const String & sIn)
	{
		int firstSlash = sIn.find(L'\\');
		String sOut = sIn.substr(firstSlash + 1, sIn.length() - firstSlash - 1);
		firstSlash = sOut.find(L'\\');
		sOut = sOut.substr(0, firstSlash);
		return sOut;
	}

	/// Code from http://ofekshilon.com/2014/06/19/reading-specific-monitor-dimensions/
	/// The code is NOT fully tested by author!!!
	bool GetMonitorSizeFromEDID(const HKEY hEDIDRegKey, short& WidthMm, short& HeightMm)
	{
		const size_t NAME_SIZE = 128;
		DWORD dwType, AcutalValueNameLength = NAME_SIZE;
		TCHAR valueName[NAME_SIZE];

		BYTE EDIDdata[1024];
		DWORD edidsize = sizeof(EDIDdata);

		for (LONG i = 0, retValue = ERROR_SUCCESS; retValue != ERROR_NO_MORE_ITEMS; ++i)
		{
			retValue = RegEnumValue(hEDIDRegKey, i, &valueName[0],
				&AcutalValueNameLength, NULL, &dwType,
				EDIDdata, // buffer
				&edidsize); // buffer size

			if (retValue != ERROR_SUCCESS || 0 != wcscmp(valueName, L"EDID"))
				continue;

			WidthMm = ((EDIDdata[68] & 0xF0) << 4) + EDIDdata[66];
			HeightMm = ((EDIDdata[68] & 0x0F) << 8) + EDIDdata[67];

			return true; // valid EDID found
		}

		return false; // EDID not found
	}

	/// Code from http://ofekshilon.com/2014/06/19/reading-specific-monitor-dimensions/
	/// The code is NOT fully tested by author!!!
	BOOL DisplayDeviceFromHMonitor(HMONITOR hMonitor, DISPLAY_DEVICE & ddMonOut)
	{
		MONITORINFOEX mi;
		mi.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &mi);

		DISPLAY_DEVICE dd;
		dd.cb = sizeof(dd);
		DWORD devIdx = 0; // device index

		bool bFoundDevice = false;
		while (EnumDisplayDevices(0, devIdx, &dd, 0))
		{
			devIdx++;
			if (0 != wcscmp(dd.DeviceName, mi.szDevice))
				continue;

			DISPLAY_DEVICE ddMon;
			ZeroMemory(&ddMon, sizeof(ddMon));
			ddMon.cb = sizeof(ddMon);
			DWORD MonIdx = 0;

			while (EnumDisplayDevices(dd.DeviceName, MonIdx, &ddMon, 0))
			{
				MonIdx++;

				ddMonOut = ddMon;
				return TRUE;

				ZeroMemory(&ddMon, sizeof(ddMon));
				ddMon.cb = sizeof(ddMon);
			}

			ZeroMemory(&dd, sizeof(dd));
			dd.cb = sizeof(dd);
		}

		return FALSE;
	}

	/// Code from http://ofekshilon.com/2014/06/19/reading-specific-monitor-dimensions/
	/// The code is NOT fully tested by author!!!
	bool GetSizeForDevID(const String & TargetDevID, short& WidthMm, short& HeightMm)
	{
		const GUID GUID_CLASS_MONITOR = { 0x4d36e96e, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 };

		HDEVINFO devInfo = SetupDiGetClassDevsEx(
			&GUID_CLASS_MONITOR, //class GUID
			NULL, //enumerator
			NULL, //HWND
			DIGCF_PRESENT | DIGCF_PROFILE, // Flags //DIGCF_ALLCLASSES|
			NULL, // device info, create a new one.
			NULL, // machine name, local machine
			NULL);// reserved

		if (NULL == devInfo)
			return false;

		bool bRes = false;

		for (ULONG i = 0; ERROR_NO_MORE_ITEMS != GetLastError(); ++i)
		{
			SP_DEVINFO_DATA devInfoData;
			memset(&devInfoData, 0, sizeof(devInfoData));
			devInfoData.cbSize = sizeof(devInfoData);

			if (SetupDiEnumDeviceInfo(devInfo, i, &devInfoData))
			{
				TCHAR Instance[MAX_DEVICE_ID_LEN];
				SetupDiGetDeviceInstanceId(devInfo, &devInfoData, Instance, MAX_PATH, NULL);

				String sInstance(Instance);
				if (String::npos == sInstance.find(TargetDevID))
					continue;

				HKEY hEDIDRegKey = SetupDiOpenDevRegKey(devInfo, &devInfoData,
					DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

				if (!hEDIDRegKey || (hEDIDRegKey == INVALID_HANDLE_VALUE))
					continue;

				bRes = GetMonitorSizeFromEDID(hEDIDRegKey, WidthMm, HeightMm);

				RegCloseKey(hEDIDRegKey);
			}
		}
		SetupDiDestroyDeviceInfoList(devInfo);
		return bRes;
	}

	/// This function retrieves related information of the found monitor and 
	/// add it to the list given by dwData.
	BOOL __stdcall Displays::monitorEnumProcCallback(
		_In_ HMONITOR hMonitor,
		_In_ HDC hdcMonitor,
		_In_ LPRECT lprcMonitor,
		_In_ LPARAM dwData) {

		DisplayList * list = (DisplayList*)dwData;
		if (list == nullptr)
			throw Exception(L"Displays::monitorEnumProcCallback(): The display list (as dwData) is NULL!");

		MONITORINFOEX info;
		memset(&info, 0, sizeof(info));
		info.cbSize = sizeof(info);
		BOOL r = GetMonitorInfo(hMonitor, &info);
		if (!r)
			throw Exception(L"Displays::monitorEnumProcCallback(): Failed to retrieve monitor info!");

		const int w1 = info.rcMonitor.right - info.rcMonitor.left;
		const int w2 = info.rcWork.right - info.rcWork.left;
		const int h1 = info.rcMonitor.bottom - info.rcMonitor.top;
		const int h2 = info.rcWork.top - info.rcWork.bottom;

		HDC dc = CreateDC(NULL, info.szDevice, NULL, NULL);
		const double hPixelsPerInch = GetDeviceCaps(dc, LOGPIXELSX);
		const double vPixelsPerInch = GetDeviceCaps(dc, LOGPIXELSY);
		DeleteDC(dc);

		DISPLAY_DEVICE ddMon;
		if (DisplayDeviceFromHMonitor(hMonitor, ddMon) == FALSE)
			OsException::throwLatest(L"Displays::monitorEnumProcCallback");

		String DeviceID(ddMon.DeviceID);
		DeviceID = Get2ndSlashBlock(DeviceID);

		short hSize, vSize;
		bool bFoundDevice = GetSizeForDevID(DeviceID, hSize, vSize);

		Display::Shared display(new Display( //
			(Display::DisplayId)hMonitor, // Id
			String(info.szDevice), // Name
			Size(hSize, vSize), // Physical size
			Vector2D(hPixelsPerInch, vPixelsPerInch), // Resolution in dpi
			Rect32I(info.rcMonitor.left, info.rcMonitor.top, w1, h1), // Screen area
			Rect32I(info.rcWork.left, info.rcWork.top, w1, h1), // Work area
			(info.dwFlags & MONITORINFOF_PRIMARY) != 0));

		list->push_back(display);

		return true;
	}
#endif
}