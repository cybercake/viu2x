/* Copyright (C) Hao Qin. All rights reserved. */

#include "Displays.h"

namespace v2x {

	/////////////
	// Display //
	/////////////

	Display::Display(const DisplayId & displayId, //
		const String & displayName, //
		const Size2DR & physicalSizeInMm, //
		const Vector2DR & resolutionInDpi, //
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
	const Size2DR & Display::getPhysicalSizeInMm() const {
		return m_physicalSizeInMm;
	}

	/// Specified resolution, not the actual one.
	const Vector2DR & Display::getResolutionInDpi() const {
		return m_resolutionInDpi;
	}

	/// Specified resolution, not the actual one.
	Vector2DR Display::getActualResolutionInDpi() const {
		return Vector2DR(m_screenAreaInPx.getWidth() * 25.4 / m_physicalSizeInMm.width, //
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

	Display::SharedConst Displays::getDisplay(size_t index) const {

		if (index >= m_displays.size())
			throw Exception(L"Displays::getDisplay: Index %u out of range!", index);

		return m_displays[index];
	}

#ifdef V2X_WINDOWS
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
		const double hSize = GetDeviceCaps(dc, HORZSIZE);
		const double vSize = GetDeviceCaps(dc, VERTSIZE);
		const double hPixelsPerInch = GetDeviceCaps(dc, LOGPIXELSX);
		const double vPixelsPerInch = GetDeviceCaps(dc, LOGPIXELSY);
		DeleteDC(dc);

		Display::Shared display(new Display( //
			(Display::DisplayId)hMonitor, // Id
			String(info.szDevice), // Name
			Size2DR(hSize, vSize), // Physical size
			Vector2DR(hPixelsPerInch, vPixelsPerInch), // Resolution in dpi
			Rect32I(info.rcMonitor.left, info.rcMonitor.top, w1, h1), // Screen area
			Rect32I(info.rcWork.left, info.rcWork.top, w1, h1), // Work area
			(info.dwFlags & MONITORINFOF_PRIMARY) != 0));

		list->push_back(display);

		return true;
	}
#endif
}