#pragma once

enum class TDarkChaosEme {
	Untouched,
	Console,
	SA2
};

struct ConfigData {
	bool TitleScr;
	bool DemoMode;
	bool ADV_PL_Cheese;
	TDarkChaosEme TDarkChaosEmeEnum = TDarkChaosEme::Untouched;
	bool IndirectOFF;
	bool Exit;
};
