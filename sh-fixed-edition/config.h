#pragma once

enum class TDarkChaosEme {
	Untouched,
	Console,
	SA2
};

struct Config {
	bool TitleScr = false;
	bool DemoMode = false;
	TDarkChaosEme TDarkChaosEmeEnum = TDarkChaosEme::Untouched;
	bool Exit = false;
	bool IndirectOFF = false;
};
