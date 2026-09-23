#include "shared_types.h"

#include <memory>

std::unique_ptr<Result> calculateA(
    std::shared_ptr<const InputData> data)
{
    return std::make_unique<Result>(Result{0.0, 0.0});
}