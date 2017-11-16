/* Copyright (C) Hao Qin. All rights reserved. */

#pragma once

#include "../../common.h"

namespace v2x {

	/// This enum type specifies the engine to be used for GUI rendering.
	/// It is set while instatiating the App object.
	enum class RenderingEngineType {
		
		/// This flag enables the v2x system to determine the rendering engine 
		/// at runtime.
		Default,

#ifdef V2X_WINDOWS
		GDI,
#endif
	};
	/// The strings for MouseButton
	std::vector<const Char *> EnumString<RenderingEngineType>::m_strings = {
		L"Default",
#ifdef V2X_WINDOWS
		L"GDI",
#endif
	};

}