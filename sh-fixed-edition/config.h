#pragma once

enum class TDarkChaosEme {
	Untouched,
	Console,
	SA2
};

struct ConfigData {
	bool TitleScr = true;
	bool DemoMode = true;
	bool ADV_PL_Cheese = true;
	TDarkChaosEme TDarkChaosEmeEnum = TDarkChaosEme::Untouched;
	bool IndirectOFF = false;
	bool Exit = true;
};
