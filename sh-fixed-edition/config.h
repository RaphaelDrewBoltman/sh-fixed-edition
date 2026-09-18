#pragma once

enum class TDarkChaosEme {
	Untouched,
	Console,
	SA2
};

struct ConfigData {
	bool TitleScr;
	bool DemoMode;
	TDarkChaosEme TDarkChaosEmeEnum = TDarkChaosEme::Untouched;
	bool IndirectOFF;
	bool Exit;
	bool MenuCheese;
};
