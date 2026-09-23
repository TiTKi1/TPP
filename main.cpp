#include "shared_types.h"

#include <iostream>
#include <memory>

std::unique_ptr<Result> calculateA(
    std::shared_ptr<const InputData> data);

std::unique_ptr<Result> calculateB(
    std::shared_ptr<const InputData> data);

int main()
{
    std::cout << "Lab1 Group Project\n";

    return 0;
}