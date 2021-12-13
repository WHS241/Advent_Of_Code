#ifndef DAY_23
#define DAY_23

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <unordered_set>

#include "intcode.cpp"

static constexpr std::size_t NUM_NIC = 50;
std::mutex locks[NUM_NIC + 1];
std::thread comps[NUM_NIC];
std::list<long> queues[NUM_NIC + 1];

char* file_path;

void run_NIC(std::size_t index) {
    bool parameter_given = false;

    auto input = [&parameter_given, &index]() -> long {
        if (!parameter_given) {
            parameter_given = true;
            return index;
        }

        std::lock_guard<std::mutex> read_guard(locks[index]);
        if (queues[index].empty()) {
            return -1;
        } else {
            long next = queues[index].front();
            queues[index].pop_front();
            return next;
        }
    };

    std::size_t next_out = 0;
    std::list<long>* output_queue;
    std::mutex* write_mutex;

    auto output = [&](long out) {
        std::size_t curr_mode = next_out;
        next_out = (next_out + 1) % 3;
        if (curr_mode == 0) {
            std::size_t address = std::min((std::size_t) out, NUM_NIC);
            write_mutex = locks + address;
            output_queue = queues + address;

            if (address != (std::size_t) out) {
                write_mutex->lock();
                output_queue->push_back(out);
            }
        } else if (curr_mode == 1) {
            if (write_mutex - locks < NUM_NIC)
                write_mutex->lock();
            output_queue->push_back(out);
        } else if (curr_mode == 2) {
            output_queue->push_back(out);
            write_mutex->unlock();
        }
    };

    try {
        intcode_computer runner(file_path, input, output);
        runner.run();
    } catch (std::runtime_error&) {}
}

int day_23_main(int argc, char** argv) {
    if (argc < 2)
        return 1;
    file_path = argv[1];

    for (std::size_t i = 0; i < NUM_NIC; ++i)
        comps[i] = std::thread(run_NIC, i);

    constexpr std::size_t TARGET = 255;
    bool to_target = false;
    std::size_t output_mode = 0;

    long last_sent_Y = -1;
    bool received = false;
    std::pair<long, long> last_NAT_packet;
    long last_received_Y = -1;
    std::size_t num_idle_cycles = 0;
    std::size_t THRESHOLD = 2;
    while (true) {
        if (!queues[NUM_NIC].empty()) {
            std::lock_guard<std::mutex> read_guard(locks[NUM_NIC]);
            std::size_t current_mode = output_mode++;
            output_mode %= 3;

            if (current_mode == 0) {
                to_target = (queues[NUM_NIC].front() == TARGET);
                received = true;
            } else if (current_mode == 1 && to_target) {
                last_NAT_packet.first = queues[NUM_NIC].front();
            } else if (current_mode == 2 && to_target) {
                last_NAT_packet.second = queues[NUM_NIC].front();
            }
            queues[NUM_NIC].pop_front();
        }

        if (output_mode == 0 && received) {
            for (std::size_t i = 0; i < NUM_NIC; ++i)
                locks[i].lock();
            if (last_received_Y != last_NAT_packet.second) {
                num_idle_cycles = 0;
            } else if (std::all_of(queues, queues + NUM_NIC,
                                   [](const std::list<long>& l) { return l.empty(); })) {
                ++num_idle_cycles;
            }

            last_received_Y = last_NAT_packet.second;

            if (num_idle_cycles > THRESHOLD) {
                if (last_NAT_packet.second == last_sent_Y) {
                    std::cout << "Duplicate Y: (" << last_NAT_packet.first << ","
                              << last_NAT_packet.second << ')' << std::endl;
                    std::terminate();
                }
                std::cout << "Sending (" << last_NAT_packet.first << ',' << last_NAT_packet.second
                          << ')' << std::endl;
                last_sent_Y = last_NAT_packet.second;
                queues[0].push_back(last_NAT_packet.first);
                queues[0].push_back(last_NAT_packet.second);
                num_idle_cycles = 0;
            }
            for (std::size_t i = 0; i < NUM_NIC; ++i)
                locks[i].unlock();
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(0.5s);
    }

    return 0;
}

#endif // DAY_23
