#ifndef DAY_7
#define DAY_7
#include <climits>
#include <iterator>
#include <sstream>

#include <list>
#include <vector>

#include <mutex>
#include <thread>

#include "intcode.cpp"

void run_augment(char* file, std::list<int>& input, std::list<int>& output, std::mutex& mutex_in,
                 std::mutex& mutex_out) {
    auto get_input = [&input, &mutex_in]() {
        bool has_input = false;
        while (!has_input) {
            using namespace std::chrono_literals;
            mutex_in.lock();
            has_input = !input.empty();
            mutex_in.unlock();
            if (!has_input)
                std::this_thread::sleep_for(0.1s);
        }
        std::lock_guard<std::mutex> guard(mutex_in);
        int next = input.front();
        input.pop_front();
        return next;
    };

    auto output_val = [&output, &mutex_out](int x) {
        std::lock_guard<std::mutex> guard(mutex_out);
        output.push_back(x);
    };

    intcode_computer runner(file, get_input, output_val);
    runner.run();
}

int day_7_main(int argc, char** argv) {
    if (argc < 3)
        return 1;

    std::vector<int> phases;

    std::string input_phases = argv[2];
    for (char c : input_phases)
        phases.push_back(c - '0');

    std::sort(phases.begin(), phases.end());
    std::vector<int> max_phase_sequence;
    int max_thrust = INT_MIN;

    do {
        std::vector<std::list<int>> inputs;
        std::mutex mutex0, mutex1, mutex2, mutex3, mutex4;
        for (int phase : phases) {
            std::list<int> input_list;
            input_list.push_back(phase);
            inputs.push_back(input_list);
        }
        inputs.front().push_back(0);

        std::thread t0(run_augment, argv[1], std::ref(inputs[0]), std::ref(inputs[1]),
                       std::ref(mutex0), std::ref(mutex1)),
          t1(run_augment, argv[1], std::ref(inputs[1]), std::ref(inputs[2]), std::ref(mutex1),
             std::ref(mutex2)),
          t2(run_augment, argv[1], std::ref(inputs[2]), std::ref(inputs[3]), std::ref(mutex2),
             std::ref(mutex3)),
          t3(run_augment, argv[1], std::ref(inputs[3]), std::ref(inputs[4]), std::ref(mutex3),
             std::ref(mutex4)),
          t4(run_augment, argv[1], std::ref(inputs[4]), std::ref(inputs[0]), std::ref(mutex4),
             std::ref(mutex0));

        t0.join();
        t1.join();
        t2.join();
        t3.join();
        t4.join();

        int output = inputs.front().front();
        if (output > max_thrust) {
            max_phase_sequence = phases;
            max_thrust = output;
        }
    } while (std::next_permutation(phases.begin(), phases.end()));

    std::cout << max_thrust << std::endl;
    std::copy(max_phase_sequence.begin(), max_phase_sequence.end(),
              std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;

    return 0;
}

#endif // DAY_7
