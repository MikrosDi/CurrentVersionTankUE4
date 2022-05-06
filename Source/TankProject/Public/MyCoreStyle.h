
#pragma once

#include "Slate.h"


class TANKPROJECT_API FMyCoreStyle
{
	
public:
	
	static void Initialize();

	static void Shutdown();

	static const ISlateStyle& Get();

private:

	static TSharedPtr<ISlateStyle> StylePtr;
	
};
