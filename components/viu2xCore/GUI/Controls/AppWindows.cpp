/* Copyright (C) Hao Qin. All rights reserved. */

#include "../../common.h"

#ifdef V2X_WINDOWS

#include "App.h"
#include "WindowHostWinGdi.h"

namespace v2x {

	RenderingEngineType App::m_defaultRenderingEngine = RenderingEngineType::Default;

	/////////
	// App //
	/////////

	App::App(RenderingEngineType renderingEngine) {

		m_defaultRenderingEngine = renderingEngine;

		// Initialize all window hosting systems
		WindowHostWinGdi::initialize();
		// ...
	}

	App::~App() {

		// Deinitialize all window hosting systems
		WindowHostWinGdi::deinitialize();
		// ...
	}

	WindowHost::Shared App::createWindowHost(RenderingEngineType renderingEngine) {

		WindowHost::Shared result;

		RenderingEngineType engine =
			renderingEngine == RenderingEngineType::Default ? m_defaultRenderingEngine : renderingEngine;

		switch (engine) {

		case RenderingEngineType::Default:
		case RenderingEngineType::GDI:
			result = WindowHostWinGdi::createNew();
			break;

		default:
			throw Exception(L"App::createWindowHost(): The rendering engine type (%s) is not supported!",
				EnumString<RenderingEngineType>::toString(m_defaultRenderingEngine).c_str());
		}

		return result;
	}

	std::vector<String> App::getParameters() {

		std::vector<String> result;

		String cmd(GetCommandLine());
		cmd = cmd + L"\""; // A trick to process the last part of the command line

		bool isQuoted = false;
		int anchorPos = -1;

		for (size_t i = 0; i < cmd.length(); ++i) {

			switch (cmd[i]) {
			case ' ':
			case '\t':
				if (!isQuoted && anchorPos >= 0)
				{
					String param = cmd.substr(anchorPos, i - anchorPos);
					result.push_back(param);
					anchorPos = -1;
				}
				break;

			case '"':

				isQuoted = !isQuoted;
				if (anchorPos >= 0) {
					if (isQuoted) {
						String param = cmd.substr(anchorPos, i - anchorPos);
						result.push_back(param);
						anchorPos = -1;
					}
					else {
						String param = cmd.substr(anchorPos, i - anchorPos);
						result.push_back(param);
						anchorPos = -1;
					}
				}
				break;

			default:
				if (anchorPos < 0)
					anchorPos = i;
				continue;
			}
		}

		return result;
	}

	void App::waitUntilTermination() {

		WindowHostWinGdi::doMessageLoop();
	}
}

#endif