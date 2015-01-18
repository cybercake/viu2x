#include "stdafx.h"
#include "CppUnitTest.h"

#include <viu2xCore/common.h>
#include <viu2xCore/gui.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace v2x;

namespace viu2xTests
{
	TEST_CLASS(TextGui)
	{
	public:

		TEST_METHOD(TextDisplays)
		{
			Displays::Shared displays(new Displays());
			displays->update();

			Assert::IsTrue(displays->count() > 0);

			size_t primaryFound = 0;
			for (size_t i = 0; i < displays->count(); ++i) {
				Assert::IsTrue(displays->getDisplay(i)->getScreenAreaInPx().getWidth() > 0);
				Assert::IsTrue(displays->getDisplay(i)->getScreenAreaInPx().getHeight() > 0);
				if (displays->getDisplay(i)->getIsPrimary())
					++primaryFound;
			}

			Assert::AreEqual(1u, primaryFound);
		}

	};
}