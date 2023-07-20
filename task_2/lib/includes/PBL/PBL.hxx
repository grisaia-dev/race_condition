#pragma once
#include <stdexcept>
#include <iostream>
#include <ostream>
#include <cstddef>
#include <string>
#include <chrono>
#include <atomic>
#include <mutex>

namespace PBL {
    class ProgressBar {
    public:
        // constructor
        explicit ProgressBar(int n);

        // destructor
        ~ProgressBar() = default;

        // Delete copy and =
        ProgressBar(ProgressBar const&) = delete;
        ProgressBar& operator=(ProgressBar const&) = delete;
        ProgressBar(ProgressBar&&) = delete;
        ProgressBar& operator=(ProgressBar&&) = delete;

        // Public Functions
        void tick(); // For update variable in bar
        void set_niter(int iter); // For set how many iterations will be used
        void reset(); // To use bar again
        std::mutex& ret_mutex() { return m_mutex; }

    private:
        int progress = 0;
        int last_precent = 0;
        int n_cycles = 0;
        bool tick_call = false;

        const std::string charDone = "\x1b[0;32m#\x1b[0m";
        const std::string charTodo = " ";
        const std::string openBracket = "[";
        const std::string closeBracket = "]";

        std::mutex m_mutex;

        std::ostream& output = std::cout;
    };

    /* Timer class */
    class Timer {
    public:
        // Constructor
        explicit Timer() = default;
        // Public Functions
        void start(std::string name);
        void print() const;
    private:
        using clock_t = std::chrono::high_resolution_clock;
        using second_t = std::chrono::duration<double, std::ratio<1>>;

        std::string m_name;
        std::chrono::time_point<clock_t> m_beg;

        // Private Functions
        std::string set_color_name(std::string& name) const;
        double elapsed() const;
    };
}