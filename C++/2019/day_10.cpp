#ifndef DAY_10
#define DAY_10 2

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <set>
#include <string>
#include <utility>
#include <vector>

int day_10_main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    std::ifstream reader(argv[1]);
    std::string line;

    std::size_t y = 0;
    std::vector<std::pair<std::size_t, std::size_t>> coords;
    std::size_t grid_size_x;

    while (std::getline(reader, line)) {
        grid_size_x = line.size();
        static const char ASTEROID = '#';
        for (std::size_t x = 0; x < line.size(); ++x)
            if (line[x] == ASTEROID)
                coords.emplace_back(x, y);
        ++y;
    }

    std::vector<std::size_t> viewable;

    for (const std::pair<std::size_t, std::size_t>& curr : coords) {
        std::vector<std::vector<bool>> processed(grid_size_x, std::vector<bool>(y, false));
        processed[curr.first][curr.second] = true;
        std::size_t num_viewed = 0;

        for (const std::pair<std::size_t, std::size_t>& target : coords) {
            // 在一个线上的小行星只能看见一个，但看到的是哪一个不重要
            // 因此可以随便挑一个小行星，然后“去掉”跟它在一个线上的
            if (!processed[target.first][target.second]) {
                ++num_viewed;
                processed[target.first][target.second] = true;
                int trajectoryX = target.first - curr.first;
                int trajectoryY = target.second - curr.second;
                if (trajectoryX == 0) {
                    trajectoryY = (trajectoryY < 0) ? -1 : 1;
                } else if (trajectoryY == 0) {
                    trajectoryX = (trajectoryX < 0) ? -1 : 1;
                } else {
                    int divisor = std::gcd(trajectoryX, trajectoryY);
                    trajectoryX /= divisor;
                    trajectoryY /= divisor;
                }

                int curr_x = curr.first, curr_y = curr.second;
                while (curr_x >= 0 && curr_y >= 0 && curr_x < grid_size_x
                       && curr_y < processed[curr_x].size()) {
                    processed[curr_x][curr_y] = true;
                    curr_x += trajectoryX;
                    curr_y += trajectoryY;
                }
            }
        }

        viewable.push_back(num_viewed);
    }

    auto it = std::max_element(viewable.begin(), viewable.end());
    int pos = it - viewable.begin();

#if DAY_10 == 1
    std::cout << *it << std::endl;
#elif DAY_10 == 2
    std::pair<std::size_t, std::size_t> optimal_coords = coords[pos];
    coords.erase(coords.begin() + pos); // 不能消掉中心

    // 换成相对位移矢量
    std::vector<std::pair<int, int>> trajectories(coords.size());
    std::transform(coords.begin(), coords.end(), trajectories.begin(),
                   [&optimal_coords](const std::pair<std::size_t, std::size_t>& x) {
                       return std::make_pair<int, int>(x.first - optimal_coords.first,
                                                       x.second - optimal_coords.second);
                   });

    auto same_trajectory = [](const std::pair<int, int>& x, const std::pair<int, int>& y) {
        return (x.first < 0) == (y.first < 0) && (x.second < 0) == (y.second < 0)
            && x.first * y.second == x.second * y.first;
    };

    // 按照角度排列
    std::sort(trajectories.begin(), trajectories.end(),
              [&same_trajectory](const std::pair<int, int>& x, const std::pair<int, int>& y) {
                  if (same_trajectory(x, y))
                      return false;

                  return std::atan2(-x.first, x.second) < std::atan2(-y.first, y.second);
              });

    auto dist_compare = [](const std::pair<int, int>& x, const std::pair<int, int>& y) {
        return (x.first * x.first + x.second * x.second)
             < (y.first * y.first + y.second * y.second);
    };

    // 相等角度的形成组
    std::list<std::set<std::pair<int, int>, decltype(dist_compare)>> grouped_trajectories;
    for (std::pair<int, int> trajectory : trajectories) {
        if (grouped_trajectories.empty()
            || !same_trajectory(trajectory, *grouped_trajectories.back().begin())) {
            std::set<std::pair<int, int>, decltype(dist_compare)> next_group(dist_compare);
            next_group.insert(trajectory);
            grouped_trajectories.push_back(next_group);
        } else {
            grouped_trajectories.back().insert(trajectory);
        }
    }

    // 以上排序除了启点是对的，把启点移到前面
    if (grouped_trajectories.back().begin()->first == 0) {
        grouped_trajectories.push_front(grouped_trajectories.back());
        grouped_trajectories.pop_back();
    }

    std::size_t removed = 0;
    while (!grouped_trajectories.empty()) {
        for (auto it = grouped_trajectories.begin(); it != grouped_trajectories.end();) {
            // 相等角度组里每一圈只去掉一个
            ++removed;
            std::cout << '#' << removed << ':';
            auto it2 = it->begin();
            std::cout << " (" << optimal_coords.first + it2->first << ','
                      << optimal_coords.second + it2->second << ")\n";
            it->erase(it2);
            if (it->empty())
                it = grouped_trajectories.erase(it);
            else
                ++it;
        }
    }

    std::cout << std::endl;
#endif

    return 0;
}

#endif // DAY_10
