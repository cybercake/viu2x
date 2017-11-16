/* Copyright (C) Hao Qin. All rights reserved. */

#include "../../common.h"

#ifdef V2X_WINDOWS

#include "App.h"
#include "WindowHostWinGdi.h"
#include "../Displays.h"

namespace v2x {

	RenderingEngineType App::m_renderingEngine = RenderingEngineType::Default;

	/////////
	// App //
	/////////

	App::App(RenderingEngineType renderingEngine) {

		switch (renderingEngine) {

		case RenderingEngineType::Default:
		case RenderingEngineType::GDI:
			WindowHostWinGdi::registerWindowClass();
			m_renderingEngine = renderingEngine;
			break;

		default:
			throw Exception(L"App::App(): The rendering engine type (%s) is not supported!", 
				EnumString<RenderingEngineType>::toString(renderingEngine).c_str());
		}
	}

	App::~App() {

		switch (m_renderingEngine) {

		case RenderingEngineType::Default:
		case RenderingEngineType::GDI:
			WindowHostWinGdi::unregisterWindowClass();
			break;

		default:
			throw Exception(L"App::~App(): The rendering engine type (%s) is not supported!",
				EnumString<RenderingEngineType>::toString(m_renderingEngine).c_str());
		}
	}

	Size2D64F App::getDefaultWindowSize() {

		Displays displays;
		Size2D32I workarea = displays.getPrimaryDisplay()->getWorkAreaInPx().size;
		Size2D64F result(workarea.width / 3, workarea.height / 3);
		return result;
	}

	WindowHost::Shared App::createWindowHost() {

		WindowHost::Shared result;

		switch (m_renderingEngine) {

		case RenderingEngineType::Default:
		case RenderingEngineType::GDI:
			result = WindowHostWinGdi::createNew();
			break;

		default:
			throw Exception(L"App::createWindowHost(): The rendering engine type (%s) is not supported!",
				EnumString<RenderingEngineType>::toString(m_renderingEngine).c_str());
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