#pragma once

class Screen
{
public:
	virtual ~Screen() {}

	virtual void init() {}
	virtual void update(float deltaTime) {}
	virtual void render() {};
	int finish() { return m_finishScreen; }
	virtual void unload() {}

protected:
	int m_finishScreen{ 0 };
};