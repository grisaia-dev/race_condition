#include "PBL/PBL.hxx"

namespace PBL {
    /* ProgressBar class */
    ProgressBar::ProgressBar(int n) : n_cycles(n) { }

    void ProgressBar::reset() {
        progress = 0;
        last_precent = 0;
        tick_call = false;
    }

    void ProgressBar::set_niter(int iter) {
        if (iter <= 0) throw std::invalid_argument("ProgressBar::set_niter -> number of iterations cannot be 0 or less");
        n_cycles = iter;
    }

    void ProgressBar::tick() {
        if (n_cycles <= 0)
            throw std::invalid_argument("ProgressBar::tick -> number of iterations not set or vriable less 0");

        if(!tick_call) {
            output << openBracket;
            for (int i = 0; i < 50; i++) output << charTodo;
            output << closeBracket << " 0%";
        }
        tick_call = true;

        // Compute precetage, if did not change just return
        int precent=0;
        precent = progress*100 / (n_cycles - 1);
        if (precent < last_precent) return;

        // Update precentage each unit
        if (precent == last_precent + 1) {
            // Erease the correct number of characters
            if (precent <= 10) output << "\b\b" << precent << '%';
            else if ((precent > 10) && (precent <= 100)) output << "\b\b\b" << precent << '%';
        }
        // Update bar every 10 units
        if (precent % 2 == 0) {
            // erase closing bracket
            output << std::string(closeBracket.size(), '\b');

            // erase trailing precentage characters
            if (precent  < 10) output << "\b\b\b";
            else if (precent >= 10 && precent < 100) output << "\b\b\b\b";
            else if (precent == 100) output << "\b\b\b\b\b";

            // erase charTodo
            for (int j = 0; j < 50-(precent-1)/2; ++j)
                output << std::string(charTodo.size(), '\b');

            // add one additional 'charDone'
            if (precent == 0) output << charTodo;
            else output << charDone;            

            // refill with 'charDone'
            for (int j = 0; j < 50-(precent-1)/2-1; ++j) output << charTodo;

            // readd trailing percentage characters
            output << closeBracket << ' ' << precent << '%';
        }
        last_precent = precent;
        ++progress;
        output << std::flush;
    }

    /* Timer class */
    void Timer::start(std::string name) {
        m_name = set_color_name(name);
        m_beg = clock_t::now();
    }
    void Timer::print() const { std::cout << m_name << ":\t" << elapsed() * 1000 << " ms" << '\n'; }
    std::string Timer::set_color_name(std::string& name) const {
        name = "\x1b[0;33m" + name + "\x1b[0m";
        return name;
    }
    double Timer::elapsed() const { return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count(); }
}