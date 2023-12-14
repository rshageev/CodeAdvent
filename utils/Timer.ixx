export module utils:Timer;

import std;

export
{
	template<class TimerType>
	class TTimer
	{
	public:
		TTimer()
			: start(TimerType::now())
		{}

		auto Get(bool reset = true)
		{
			const auto now = TimerType::now();
			const std::chrono::duration<double> diff = now - start;
			if (reset) {
				start = now;
			}
			return diff;
		}
	private:
		std::chrono::time_point<TimerType> start;
	};

	using Timer = TTimer<std::chrono::high_resolution_clock>;
}