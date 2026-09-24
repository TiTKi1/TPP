#include "shared_types.h"

#include <iostream>
#include <memory>

std::unique_ptr<Result> calculateA(
    std::shared_ptr<const InputData> data);

std::unique_ptr<Result> calculateB(
    std::shared_ptr<const InputData> data);

int main()
{
    auto data = std::make_shared<const InputData>(
        InputData{
            {{1, 2}, {2, 4}, {3, 9}},
            2.5
        }
    );


    auto resultA = calculateA(data);

    auto [valueA, errorA] = *resultA;

    std::cout << "Lagrange interpolation: " << valueA << '\n';
    std::cout << "Error: " << errorA << '\n';

    auto resultB = calculateB(data);
    auto [valueB, errorB] = *resultB;

    std::cout << "Cubic spline interpolation: " << valueB << '\n';
    std::cout << "Cubic spline error: " << errorB << '\n';


    return 0;
}