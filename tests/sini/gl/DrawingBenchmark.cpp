#include "sini/gl/Camera.hpp"
#include "sini/gl/SimpleRenderer.hpp"
#include "sini/math/Matrix.hpp"
#include "sini/sdl/SubsystemInitializer.hpp"
#include "sini/sdl/Window.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>


using namespace sini;
using gl::Camera;
using gl::SimpleRenderer;


template<uint32_t TERRAIN_SIZE, typename RandEngine>
Matrix<float,TERRAIN_SIZE,TERRAIN_SIZE> generateFractalTerrain(RandEngine random_engine)
{
    std::uniform_real_distribution uniform_dist{ -0.5f, 0.5f };
    Matrix<float, TERRAIN_SIZE, TERRAIN_SIZE> terrain = 0;
    const int max_divs = [](){
                             int temp = 0;
                             for (int i = (TERRAIN_SIZE-1)/2; i > 0; i /= 2, ++temp);
                             return temp;
                         }();


    // Initialize corners
    // ------------------
    terrain(0, 0)                           = 0.5f + uniform_dist(random_engine);
    terrain(TERRAIN_SIZE-1, TERRAIN_SIZE-1) = 0.5f + uniform_dist(random_engine);
    terrain(0, TERRAIN_SIZE-1)              = 0.5f + uniform_dist(random_engine);
    terrain(TERRAIN_SIZE -1, 0)             = 0.5f + uniform_dist(random_engine);


    // Generate fractal terrain using the diamond-square algorithm
    // ( https://en.wikipedia.org/wiki/Diamond-square_algorithm )
    // -----------------------------------------------------------
    int resolution = (TERRAIN_SIZE - 1)/ 2;
    for (int iteration = 1; iteration < max_divs; iteration++) {
        float noise_divisor = pow(2.0f, static_cast<float>(iteration));
        resolution /= 2;
        for (int i = 1, x = i*resolution; x+1 < TERRAIN_SIZE; i += 2, x = i*resolution) {
            for (int j = 1, y = j*resolution; y+1 < TERRAIN_SIZE; j += 2, y = j*resolution) {
                // diamond
                terrain(x, y) =
                    (terrain(x-resolution, y-resolution)
                        + terrain(x-resolution, y+resolution)
                        + terrain(x+resolution, y-resolution)
                        + terrain(x+resolution, y+resolution)) / 4.0f
                    + uniform_dist(random_engine) / noise_divisor;

                // square
                if (x-resolution > 0)
                    terrain(x-resolution, y) =
                        (terrain(x-2*resolution,y) + terrain(x, y)
                            + terrain(x-resolution, y-resolution)
                            + terrain(x-resolution, y+resolution)) / 4.0f
                        + uniform_dist(random_engine) / noise_divisor;
                else
                    terrain(x-resolution, y) =
                        (terrain(x, y) + terrain(x-resolution, y-resolution)
                            + terrain(x-resolution, y+resolution)) / 3.0f
                        + uniform_dist(random_engine) / noise_divisor;

                if (x+resolution < TERRAIN_SIZE-1)
                    terrain(x+resolution, y) =
                        (terrain(x+2*resolution,y) + terrain(x,y)
                            + terrain(x+resolution, y-resolution)
                            + terrain(x+resolution, y+resolution)) / 4.0f
                        + uniform_dist(random_engine) / noise_divisor;
                else
                    terrain(x+resolution, y) =
                        (terrain(x, y) + terrain(x+resolution, y-resolution)
                            + terrain(x+resolution, y+resolution)) / 3.0f
                        + uniform_dist(random_engine) / noise_divisor;

                if (y-resolution > 0)
                    terrain(x, y-resolution) =
                        (terrain(x, y-2*resolution) + terrain(x, y)
                            + terrain(x-resolution, y-resolution)
                            + terrain(x+resolution, y+resolution)) / 4.0f
                        + uniform_dist(random_engine) / noise_divisor;
                else
                    terrain(x, y-resolution) =
                        (terrain(x, y) + terrain(x-resolution, y-resolution)
                            + terrain(x+resolution, y-resolution)) / 3.0f
                        + uniform_dist(random_engine) / noise_divisor;

                if (y+resolution < TERRAIN_SIZE-1)
                    terrain(x, y+resolution) =
                        (terrain(x, y+2*resolution) + terrain(x, y)
                            + terrain(x-resolution, y+resolution)
                            + terrain(x+resolution, y+resolution)) / 4.0f
                        + uniform_dist(random_engine) / noise_divisor;
                else
                    terrain(x, y+resolution) =
                        (terrain(x, y) + terrain(x-resolution, y+resolution)
                            + terrain(x+resolution, y+resolution)) / 3.0f
                        + uniform_dist(random_engine) / noise_divisor;
            }
        }
    }

    return terrain;
}


template<uint32_t N>
void renderTerrain(SimpleRenderer& renderer,
                   const Camera& camera,
                   const Matrix<float,N,N>& terrain)
{
    renderer.clear(0.0f);
    const int terrain_size = static_cast<int>(dimensions(terrain).x);
    const float block_size = camera.width / static_cast<float>(terrain_size);
    for (int row = 0; row < terrain_size; row++) {
        const float y1 = row*block_size,
                    y2 = (row+1)*block_size;
        for (int col = 0; col < terrain_size; col++) {
            const float x1 = col*block_size,
                        x2 = (col+1)*block_size;

            renderer.fillRectangle({ x1, y1 }, { x2, y2 }, vec3(terrain(col, row)), 1.0f);
        }
    }
    renderer.updateScreen();
}


void printReport(const int* sizes, const double* times, int n_entries)
{
    constexpr int col_width = 18;
    std::cout << "Drawing benchmark" << std::endl
              << "-----------------------------" << std::endl
              << std::left << std::setw(col_width) << "terrain size"
              << "time" << std::endl;

    for (int i = 0; i < n_entries; ++i) {
        std::stringstream s;
        s << sizes[i] << "x" << sizes[i];
        std::string s2{ s.str() };
        std::cout << std::left << std::setw(col_width) << s2
                  << times[i] << " ms" << std::endl;
    }
}


#define STR(x) #x

#define BENCHMARK_CASE(SIZE, RESULT_INDEX) {                            \
    const auto terrain = std::make_unique<Matrix<float,SIZE,SIZE>>(     \
        generateFractalTerrain<SIZE>(rand_engine));                     \
    Window window{ "Drawing Benchmark " STR(SIZE),                      \
                   { 800, 800 },                                        \
                   { WindowProperties::OPENGL } };                      \
    Camera camera{ static_cast<float>(SIZE) / 2.0f,                     \
                   1.0f,                                                \
                   static_cast<float>(SIZE) };                          \
    SimpleRenderer renderer{ window, camera };                          \
    const auto start_time = std::chrono::high_resolution_clock::now();  \
    renderTerrain(renderer, camera, *terrain);                          \
    const auto end_time = std::chrono::high_resolution_clock::now();    \
    const std::chrono::duration<double, std::milli> elapsed_time = end_time - start_time; \
    times[RESULT_INDEX] = elapsed_time.count();                         \
    }


int main(int argc, char** argv)
{
    SubsystemInitializer si{ { SubsystemFlags::VIDEO } };

    std::default_random_engine rand_engine{ 10476 };
    // terrain size must be = 2^N + 1 for some integer N
    const int terrain_sizes[] = { 33, 65, 129, 257, 513, 1025 };
    constexpr int n_sizes = sizeof(terrain_sizes) / sizeof(int);
    double times[n_sizes];

    BENCHMARK_CASE(33,   0);
    BENCHMARK_CASE(65,   1);
    BENCHMARK_CASE(129,  2);
    BENCHMARK_CASE(257,  3);
    BENCHMARK_CASE(513,  4);
    BENCHMARK_CASE(1025, 5);

    printReport(terrain_sizes, times, n_sizes);
}
