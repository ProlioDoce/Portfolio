module;
#define NOMINMAX
#include <Windows.h> 	//stack -> 1,000,000 bytes

export module accumulator;

import std;
import<cstdint>;

void startup_accum()
{
	static auto _ = [] {
		timeBeginPeriod(1);
		std::atexit([]
			{
				timeEndPeriod(1);
			});

		return std::monostate{};
		}();
}

using Clock = std::chrono::steady_clock;
using Duration = std::chrono::duration<double>;

export template<uint16_t UPS>
class Accumulator
{
	static constexpr auto Tick = std::chrono::duration_cast<Clock::duration>(Duration{ 1.0 / UPS });

	struct Iterator
	{
		uint32_t iteration, debt;
		Clock::time_point now, next_update = Clock::now();

		auto operator*()
		{
			return iteration;
		}
		auto operator++()
		{
			++iteration;

			if (debt)
			{
				--debt;
				next_update += Tick;
				return;
			}

			now = Clock::now();

			if (now > next_update - std::chrono::milliseconds(1))
			{
				auto pending_ticks = uint32_t((now - next_update) / Tick + 1);

				if (pending_ticks <= 9)
					debt = pending_ticks - 1;

				else next_update += Tick * pending_ticks;
			}

			std::this_thread::sleep_until(next_update - std::chrono::milliseconds{2});

			while (Clock::now() < next_update)
				std::this_thread::yield();

			next_update += Tick;
			//if (random::chance(25)) std::this_thread::sleep_for(64ms);

			////checker
			//{
			//	static int frames{};
			//	static std::chrono::duration<double> accum_time{};
			//	static double max_ms{};
			//	static double min_ms{ 999999.0 };
			//	auto frame_time = Clock::now() - now;
			//	double ms =
			//		std::chrono::duration<double, std::milli>(frame_time)
			//		.count();
			//	accum_time += frame_time;
			//	max_ms = std::max(max_ms, ms);
			//	min_ms = std::min(min_ms, ms);
			//	++frames;
			//	if (frames >= 120)
			//	{
			//		double avg_ms =
			//			std::chrono::duration<double, std::milli>(accum_time)
			//			.count() / frames;
			//		std::println(
			//			"avg {:.3f}ms | min {:.3f}ms | max {:.3f}ms",
			//			avg_ms,
			//			min_ms,
			//			max_ms
			//		);
			//		frames = 0;
			//		accum_time = std::chrono::seconds(0);
			//		max_ms = 0.0;
			//		min_ms = 999999.0;
			//	}
			//}
		}
		bool operator!=(std::monostate)
		{
			return true;
		}
	};
public:
	Accumulator()
	{
		startup_accum();
	}

	Iterator begin(this auto&& self)
	{
		return {};
	}
	std::monostate end(this auto&& self)
	{
		return {};
	}
};