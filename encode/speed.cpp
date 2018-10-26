#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <memory>

#include "config.h"
#include "../gettime.cpp"

#include "benchmark_application.cpp"

class Application final: public BenchmarkApplication<Application> {

    friend class BenchmarkApplication<Application>;

    double reference_time;

public:
    Application(const CommandLine& c)
        : BenchmarkApplication<Application>(c)
        , reference_time(0.0) {}

    int run() {
        
        initialize();

        return run_all();
    }

private:
    template<typename T>
    double measure_impl(const char* name, T callback) {

        printf("%-40s... ", name);
        fflush(stdout);

        unsigned n = iterations;
        double time = -1;
        while (n-- > 0) {

            const auto t1 = get_time();
            callback(input.get(), get_input_size(), output.get());
            const auto t2 = get_time();

            const double t = (t2 - t1)/1000000.0;
            if (time < 0) {
                time = t;
            } else {
                time = std::min(time, t);
            }
        }

        printf("%0.5f", time);

        if (reference_time == 0.0) {
            reference_time = time;
        } else {
            printf(" (speedup %0.2f)", reference_time/time);
        }

        putchar('\n');

        return time;
    }
};


int main(int argc, char* argv[]) {

    CommandLine cmd(argc, argv);
    Application app(cmd);

    return app.run();
}

