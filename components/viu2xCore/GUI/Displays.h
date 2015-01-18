/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include <vector>

#include "../common.h"

#ifdef V2X_WINDOWS
#include <windows.h>
#endif

namespace v2x {

	/// Display object is info holder for connected monitors.
	class Display : public Object {
	public:
		DEFINE_POINTERS(Display);

		typedef int DisplayId;
		Display(const DisplayId & displayId, //
			const String & displayName, //
			const Size2DR & physicalSizeInMm, //
			const Vector2DR & resolutionInDpi, //
			const Rect32I & screenAreaInPx, //
			const Rect32I & workAreaInPx, //
			const bool & isPrimary);
		virtual ~Display();

		/// Identifier of the display
		const DisplayId & getDisplayId() const;

		/// Name of the display
		const String & getDisplayName() const;

		/// Physical size of the screen, given in mm.
		const Size2DR & getPhysicalSizeInMm() const;
		
		/// Specified resolution, not the actual one.
		const Vector2DR & getResolutionInDpi() const;

		/// Specified resolution, not the actual one.
		Vector2DR getActualResolutionInDpi() const;

		/// The region where the display is mapped to.
		const Rect32I & getScreenAreaInPx() const;

		/// The region where the working area of the display is mapped to.
		const Rect32I & getWorkAreaInPx() const;

		/// A flag indicating if the display is the primary one.
		const bool & getIsPrimary() const;

	protected:
		/// Identifier of the display
		DisplayId m_displayId;

		/// Name of the display
		String m_displayName;

		/// Physical size of the screen, given in mm.
		Size2DR m_physicalSizeInMm;

		/// Specified resolution, not the actual one.
		Vector2DR m_resolutionInDpi;

		/// The region where the display is mapped to.
		Rect32I m_screenAreaInPx;

		/// The region where the working area of the display is mapped to.
		Rect32I m_workAreaInPx;

		/// A flag indicating if the display is the primary one.
		bool m_isPrimary;
	};

	/// This class retrieves all monitor information from the system and expose them to user.
	class Displays : public Object
	{
	public:
		DEFINE_POINTERS(Displays);
		Displays();
		virtual ~Displays();

		void update();

		size_t count() const;
		Display::SharedConst getDisplay (size_t index) const;
		Display::SharedConst getPrimaryDisplay() const;

	private:
		typedef std::vector<Display::SharedConst> DisplayList;
		DisplayList m_displays;

#ifdef V2X_WINDOWS
		static BOOL __stdcall monitorEnumProcCallback(
			_In_ HMONITOR hMonitor,
			_In_ HDC hdcMonitor,
			_In_ LPRECT lprcMonitor,
			_In_ LPARAM dwData);
#endif
	};

}