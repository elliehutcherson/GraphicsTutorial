#pragma once

namespace GameEngine {
	class FpsLimiter {
	public:
		FpsLimiter();
		void init(float maxFPS );
	
		void setMaxFPS(float maxFPS);

		void begin();

		// end will return the current FPS
		float end();
	private:
		void calculateFPS();
		float _fps;
		float _frameTime;
		float _maxFPS;
		unsigned int _startTicks;
	};
}
