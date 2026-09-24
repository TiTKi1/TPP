#include "shared_types.h"

#include <cmath>
#include <memory>

std::unique_ptr<Result> calculateA(
    std::shared_ptr<const InputData> data)
{
    double value = 0.0;

    for (size_t i = 0; i < data->points.size(); ++i)
    {
        double term = data->points[i].y;

        for (size_t j = 0; j < data->points.size(); ++j)
        {
            if (i != j)
            {
                term *= (data->x0 - data->points[j].x) /
                         (data->points[i].x - data->points[j].x);
            }
        }

        value += term;
    }

    double dividedDifference = 0.0;

    for (size_t i = 0; i < data->points.size(); ++i)
    {
        double term = data->points[i].y;

        for (size_t j = 0; j < data->points.size(); ++j)
        {
            if (i != j)
            {
                term /= data->points[i].x - data->points[j].x;
            }
        }

        dividedDifference += term;
    }

    double product = 1.0;

    for (const auto& point : data->points)
    {
        product *= data->x0 - point.x;
    }

    double error = std::abs(dividedDifference * product);

    return std::make_unique<Result>(Result{value, error});
}