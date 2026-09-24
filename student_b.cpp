#include "shared_types.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>

// Допоміжна структура для зберігання коефіцієнтів відрізка сплайна
struct SplineSegment
{
    double a, b, c, d, x;
};

std::unique_ptr<Result> calculateB(std::shared_ptr<const InputData> data)
{
    // 1. Перевірка на коректність та достатність вхідних даних
    if (!data || data->points.size() < 2)
    {
        return std::make_unique<Result>(Result{0.0, -1.0}); // Помилка даних
    }

    auto pts = data->points;
    // 2. Сортування точок за зростанням координати X (вимога сплайн-інтерполяції)
    std::sort(pts.begin(), pts.end(), [](const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    });

    const size_t n = pts.size();
    std::vector<SplineSegment> splines(n);
    for (size_t i = 0; i < n; ++i)
    {
        splines[i].x = pts[i].x;
        splines[i].a = pts[i].y;
    }

    std::vector<double> h(n - 1);
    double max_h = 0.0;
    for (size_t i = 0; i < n - 1; ++i)
    {
        h[i] = pts[i + 1].x - pts[i].x;
        if (h[i] <= 1e-12) 
        {
            return std::make_unique<Result>(Result{0.0, -1.0}); // Точки за x збігаються
        }
        max_h = std::max(max_h, h[i]);
    }

    // 3. Побудова системи та метод прогонки для коефіцієнтів природного кубічного сплайна
    std::vector<double> alpha(n - 1, 0.0);
    for (size_t i = 1; i < n - 1; ++i) 
    {
        alpha[i] = (3.0 / h[i]) * (splines[i + 1].a - splines[i].a) -
                   (3.0 / h[i - 1]) * (splines[i].a - splines[i - 1].a);
    }

    std::vector<double> l(n, 1.0), mu(n, 0.0), z(n, 0.0);
    for (size_t i = 1; i < n - 1; ++i) 
    {
        l[i] = 2.0 * (pts[i + 1].x - pts[i - 1].x) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    splines[n - 1].c = 0.0;
    for (int j = static_cast<int>(n) - 2; j >= 0; --j) 
    {
        splines[j].c = z[j] - mu[j] * splines[j + 1].c;
        splines[j].b = (splines[j + 1].a - splines[j].a) / h[j] - h[j] * (splines[j + 1].c + 2.0 * splines[j].c) / 3.0;
        splines[j].d = (splines[j + 1].c - splines[j].c) / (3.0 * h[j]);
    }

    // 4. Пошук відповідного інтервалу для шуканої точки x0
    const double x0 = data->x0;
    SplineSegment s;
    if (x0 <= pts.front().x) 
    {
        s = splines.front();
    } 
    else if (x0 >= pts.back().x) 
    {
        s = splines[n - 2];
    } 
    else 
    {
        auto it = std::upper_bound(pts.begin(), pts.end(), x0, [](double val, const Point& p) {
            return val < p.x;
        });
        size_t idx = std::distance(pts.begin(), it) - 1;
        s = splines[idx];
    }

    // 5. Обчислення інтерпольованого значення функції в точці x0
    double dx = x0 - s.x;
    double resultValue = s.a + s.b * dx + s.c * dx * dx + s.d * dx * dx * dx;
    
    // Оцінка теоретичної похибки для кубічного сплайна: O(h^4)
    double estimatedError = std::pow(max_h, 4.0);

    // 6. Повернення результату через std::unique_ptr
    return std::make_unique<Result>(Result{resultValue, estimatedError});
}