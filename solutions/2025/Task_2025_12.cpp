#include "Runner.h"

import std;
import utils;

namespace
{
    struct Task {
        int w, h;
        std::array<int, 6> presents;
    };

    Array2D<char> RotateShape(const Array2D<char>& shape)
    {
        Array2D<char> ret(shape.Area());
        ret[{0, 0}] = shape[{2, 0}];
        ret[{0, 1}] = shape[{1, 0}];
        ret[{0, 2}] = shape[{0, 0}];
        ret[{1, 0}] = shape[{2, 1}];
        ret[{1, 1}] = shape[{1, 1}];
        ret[{1, 2}] = shape[{0, 1}];
        ret[{2, 0}] = shape[{2, 2}];
        ret[{2, 1}] = shape[{1, 2}];
        ret[{2, 2}] = shape[{0, 2}];
        return ret;
    }

    bool ShapeFits(Array2D<char>& dst, Point dst_pos, const Array2D<char>& shape)
    {
        for (Point src_p : to_cell_coords(shape)) {
            Point dst_p = dst_pos + src_p;
            if (shape[src_p] == '#') {
                if (!Contains(dst.Area(), dst_p) || dst[dst_p] == '#')
                    return false;
            }
        }
        return true;
    }

    void PlaceShape(Array2D<char>& dst, Point dst_pos, const Array2D<char>& shape)
    {
        for (Point src_p : to_cell_coords(shape)) {
            Point dst_p = dst_pos + src_p;
            dst[dst_p] = shape[src_p];
        }
    }

    struct Shape {
        std::array<Array2D<char>, 4> variations;
        int area = 0;
    }; 

    auto ReadData(const std::filesystem::path& input)
    {
        std::vector<Shape> shapes;
        std::vector<Task> tasks;

        auto blocks = ReadLineBlocks(input);

        for (std::string_view line : blocks.back()) {
            auto& task = tasks.emplace_back();
            task.w = Read<int>(line);
            Skip(line, "x");
            task.h = Read<int>(line);
            Skip(line, ":");
            for (size_t i = 0; i < 6; ++i) {
                Skip(line, " ");
                task.presents[i] = Read<int>(line);
            }
        }

        blocks.pop_back();

        for (const auto& block : blocks) {
            auto& shape = shapes.emplace_back();
            auto arr = Array2DFromLines(block | stdv::drop(1));
            for (size_t i = 0; i < 4; ++i) {
                shape.variations[i] = arr;
                arr = RotateShape(arr);
            }
            shape.area = (int)std::count(arr.begin(), arr.end(), '#');
        }

        return std::pair(std::move(shapes), std::move(tasks));
    }

    bool PutPresents(std::span<const Shape> shapes, Array2D<char> space, std::array<int, 6> rem_presents)
    {
        int shape_to_place_idx = -1;
        for (int i = 0; i < 6; ++i) {
            int& rem_count = rem_presents[i];
            if (rem_count > 0) {
                shape_to_place_idx = i;
                rem_count--;
                break;
            }  
        }

        if (shape_to_place_idx < 0) {
            return true;
        }

        const auto& shape = shapes[shape_to_place_idx];
        for (const auto& shape_var : shape.variations) {
            for (int x = 0; x <= space.Width() - 3; ++x) {
                for (int y = 0; y <= space.Height() - 3; ++y) {
                    if (ShapeFits(space, { x,y }, shape_var)) {
                        Array2D<char> new_space = space;
                        PlaceShape(new_space, { x,y }, shape_var);
                        if (PutPresents(shapes, new_space, rem_presents)) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool CanPotentiallyFit(std::span<const Shape> shapes, const Task& task)
    {
        int shapes_area = 0;
        for (size_t i = 0; i < task.presents.size(); ++i) {
            shapes_area += (shapes[i].area * task.presents[i]);
        }
        return shapes_area <= (task.w * task.h);
    }

    bool DefinitelyCanFit(const Task& task)
    {
        return (task.w / 3) * (task.h / 3) >= Sum(task.presents);
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        const auto [shapes, tasks] = ReadData(input);

        int count = 0;
        for (const auto& task : tasks) {
            if (DefinitelyCanFit(task))
            {
                ++count;
            }
            else if (CanPotentiallyFit(shapes, task))
            {
                Array2D<char> space(task.w, task.h, '.');
                auto rem_presents = task.presents;
                if (PutPresents(shapes, space, rem_presents)) {
                    ++count;
                }
            }         
        }

        return count;
    }

    REGISTER_SOLUTION(2025, 12, 1, Solve_1);
}
