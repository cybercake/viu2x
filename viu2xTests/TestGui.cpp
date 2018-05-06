#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>

#include <viu2xCore/common.h>
#include <viu2xCore/gui.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace v2x;

namespace viu2xTests
{
	TEST_CLASS(TestGui)
	{
	public:

		TEST_METHOD(TextDisplays)
		{
			Displays::Shared displays(new Displays());

			Assert::IsTrue(displays->count() > 0);

			size_t primaryFound = 0;
			for (size_t i = 0; i < displays->count(); ++i) {
				Assert::IsTrue(displays->getDisplay(i)->getScreenAreaInPx().getArea() > 0);
				Assert::IsTrue(displays->getDisplay(i)->getWorkAreaInPx().getArea() > 0);
				Assert::IsFalse(displays->getDisplay(i)->getPhysicalSizeInMm().isZero());
				Assert::IsTrue(displays->getDisplay(i)->getResolutionInDpi().x() > 0);
				Assert::IsTrue(displays->getDisplay(i)->getResolutionInDpi().y() > 0);
				if (displays->getDisplay(i)->getIsPrimary())
					++primaryFound;
			}

			Assert::AreEqual(1u, primaryFound);

			Assert::IsTrue(displays->getPrimaryDisplay() != nullptr);
		}

		TEST_METHOD(TestEnumStrings) {

			FontStyle t = FontStyle::Bold;

			Assert::IsTrue(EnumString<FontStyle>::toString(FontStyle::Bold) == L"Bold");
			Assert::IsTrue(EnumString<FontStyle>::toString(FontStyle::Italic) == L"Italic");
			Assert::IsTrue(EnumString<FontStyle>::toString(FontStyle::Stroke) == L"Stroke");
			Assert::IsTrue(EnumString<FontStyle>::toString(FontStyle::Underline) == L"Underline");
		}

	};
}