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
	bool Exit;
	bool IndirectOFF;
};
