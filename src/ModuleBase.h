#pragma once
class ModuleBase
{
public:
	ModuleBase();

	void EnableModule();
	void DisableModule();

private:
	bool enabled;
};

