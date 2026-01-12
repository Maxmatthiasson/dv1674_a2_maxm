/*
Author: David Holmqvist <daae19@student.bth.se>
*/

/*
Optimizations made in v2:
- Pre-computing Gaussian weights at the start of the blur function.
- Removed calls to Gauss:get_weights inside of the loops.
- Re-using the Gaussion weights that were calculated in the beginning.

Optimizations made in v3:
- Caching images x and y size to avoid repeated function calls inside the loops.
- Instead of calling get_y_size() and get_x_size() in the for-loops and 
  if-statements, I'm re-using the the cached values.

Parallelization:
- Each thread gets a row to process.
- Synchronize by using pthread_join() after each pass.
*/

#include "filters_par.hpp"
#include "matrix.hpp"
#include "ppm.hpp"
#include <cmath>
#include <pthread.h>

struct ThreadData {
    Matrix* src;
    Matrix* dst;
    double* w;
    int radius;
    int x_size;
    int y_size;
    int y_start;
    int y_end;
};

void* blur_horizontal_worker(void* arg)
{
    ThreadData* d = static_cast<ThreadData*>(arg);

    for (int x = 0; x < d->x_size; x++)
    {
        for (int y = d->y_start; y < d->y_end; y++)
        {
            auto r = d->w[0] * d->src->r(x, y);
            auto g = d->w[0] * d->src->g(x, y);
            auto b = d->w[0] * d->src->b(x, y);
            auto n = d->w[0];

            for (int wi = 1; wi <= d->radius; wi++)
            {
                auto wc = d->w[wi];

                int x2 = x - wi;
                if (x2 >= 0) {
                    r += wc * d->src->r(x2, y);
                    g += wc * d->src->g(x2, y);
                    b += wc * d->src->b(x2, y);
                    n += wc;
                }

                x2 = x + wi;
                if (x2 < d->x_size) {
                    r += wc * d->src->r(x2, y);
                    g += wc * d->src->g(x2, y);
                    b += wc * d->src->b(x2, y);
                    n += wc;
                }
            }

            d->dst->r(x, y) = r / n;
            d->dst->g(x, y) = g / n;
            d->dst->b(x, y) = b / n;
        }
    }

    return nullptr;
}

void* blur_vertical_worker(void* arg)
{
    ThreadData* d = static_cast<ThreadData*>(arg);

    for (int x = 0; x < d->x_size; x++)
    {
        for (int y = d->y_start; y < d->y_end; y++)
        {
            auto r = d->w[0] * d->src->r(x, y);
            auto g = d->w[0] * d->src->g(x, y);
            auto b = d->w[0] * d->src->b(x, y);
            auto n = d->w[0];

            for (int wi = 1; wi <= d->radius; wi++)
            {
                auto wc = d->w[wi];

                int y2 = y - wi;
                if (y2 >= 0) {
                    r += wc * d->src->r(x, y2);
                    g += wc * d->src->g(x, y2);
                    b += wc * d->src->b(x, y2);
                    n += wc;
                }

                y2 = y + wi;
                if (y2 < d->y_size) {
                    r += wc * d->src->r(x, y2);
                    g += wc * d->src->g(x, y2);
                    b += wc * d->src->b(x, y2);
                    n += wc;
                }
            }

            d->dst->r(x, y) = r / n;
            d->dst->g(x, y) = g / n;
            d->dst->b(x, y) = b / n;
        }
    }

    return nullptr;
}

namespace Filter
{
    namespace Gauss
    {
        void get_weights(int n, double *weights_out)
        {
            for (auto i = 0; i <= n; i++)
            {
                double x = static_cast<double>(i) * max_x / n;
                weights_out[i] = exp(-x * x * pi);
            }
        }
    }

    Matrix blur(Matrix m, int radius, int num_threads)
    {
        auto dst = m;
        Matrix scratch{PPM::max_dimension};

        // Precomputing Gaussian weights
        double w[Gauss::max_radius]{};
        Gauss::get_weights(radius, w);

        // Caching image x and y size
        int x_size = dst.get_x_size();
        int y_size = dst.get_y_size();

        pthread_t threads[num_threads];
        ThreadData data[num_threads];
        int rows_per_thread = y_size / num_threads;

        // Horizontal pass
        for (int t = 0; t < num_threads; t++)
        {
            data[t] = {&dst, &scratch, w, radius, x_size, y_size,
                       t * rows_per_thread,
                       (t == num_threads - 1) ? y_size : (t + 1) * rows_per_thread};

            pthread_create(&threads[t], nullptr, blur_horizontal_worker, &data[t]);
        }
        for (int t = 0; t < num_threads; t++)
            pthread_join(threads[t], nullptr);

        // Vertical pass
        for (int t = 0; t < num_threads; t++)
        {
            data[t].src = &scratch;
            data[t].dst = &dst;

            pthread_create(&threads[t], nullptr, blur_vertical_worker, &data[t]);
        }
        for (int t = 0; t < num_threads; t++)
            pthread_join(threads[t], nullptr);

        return dst;
    }
}


