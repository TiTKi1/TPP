#pragma once

#include <vector>

struct Point
{
    double x;
    double y;
};

struct InputData
{
    std::vector<Point> points;
    double x0;
};

struct Result
{
    double value;
    double error;
};