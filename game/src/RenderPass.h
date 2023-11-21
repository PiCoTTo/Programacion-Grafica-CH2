#pragma once

class RenderPass
{
public:
	virtual ~RenderPass();

	virtual void render() = 0;
};

