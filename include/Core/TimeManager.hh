//
// Created by kate on 6/11/23.
//

#ifndef RENDERER_TIME_MANAGER_HH
#define RENDERER_TIME_MANAGER_HH

#include <ratio>
#include <chrono>

namespace kT {
    enum class TimeUnit {
        NONE,
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS,
        COUNT,
    };

    class TimeManager {
    public:
        static auto Init() {
            m_InitTimePoint = Clock_T::now();
        }

        static auto UpdateDeltaTime() -> void {
            TimePoint_T now{ Clock_T::now() };
            m_TimeStep = std::chrono::duration_cast<Sec_T>(now - m_LasFrameTime).count();
            m_LasFrameTime = now;
        }

        static auto GetDeltaTime(TimeUnit unit = TimeUnit::SECONDS) -> double {
            switch (unit) {
                case TimeUnit::SECONDS:         return m_TimeStep;
                case TimeUnit::MILLISECONDS:    return m_TimeStep / SEC_TO_MILLI;
                case TimeUnit::MICROSECONDS:    return m_TimeStep / SEC_TO_MICRO;
                case TimeUnit::NANOSECONDS:     return m_TimeStep / SEC_TO_NANO;
            }
        }

        // Returns amount of time between this time_point and the clock's epoch
        static auto GetTime(TimeUnit unit = TimeUnit::SECONDS) -> double {
            switch (unit) {
                case TimeUnit::SECONDS:         return std::chrono::duration_cast<Sec_T>(Clock_T::now() - m_InitTimePoint).count();
                case TimeUnit::MILLISECONDS:    return std::chrono::duration_cast<Milli_T>(Clock_T::now() - m_InitTimePoint).count() / SEC_TO_MILLI;
                case TimeUnit::MICROSECONDS:    return std::chrono::duration_cast<Micro_T>(Clock_T::now() - m_InitTimePoint).count() / SEC_TO_MICRO;
                case TimeUnit::NANOSECONDS:     return std::chrono::duration_cast<Nano_T>(Clock_T::now() - m_InitTimePoint).count() / SEC_TO_NANO;
            }

            return -1;
        }

    private:
        static constexpr double SEC_TO_MILLI{ 1000.0 };
        static constexpr double SEC_TO_MICRO{ 1000'000.0 };
        static constexpr double SEC_TO_NANO{ 1000'000'000.0 };

        using Nano_T = std::chrono::duration<double, std::ratio<1, 1000000000>>;
        using Micro_T = std::chrono::duration<double, std::ratio<1, 1000000>>;
        using Milli_T = std::chrono::duration<double, std::ratio<1, 1000>>;
        using Sec_T = std::chrono::duration<double, std::ratio<1, 1>>;
        using Clock_T = std::chrono::high_resolution_clock;
        using TimePoint_T   =  std::chrono::time_point<Clock_T>;

        // Time in seconds
        inline static double m_TimeStep{};
        inline static TimePoint_T m_LasFrameTime{};
        inline static TimePoint_T m_InitTimePoint{};
    };
}

#endif //RENDERER_TIME_MANAGER_HH
