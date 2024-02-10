#include <iostream>
#include <vector>
#include <cmath>

// Function to generate Laplacian of Gaussian kernel
std::vector<std::vector<float>> laplacianOfGaussianKernel(int size, float sigma) {
    std::vector<std::vector<float>> laplacianKernel(size, std::vector<float>(size, 0.0));
    int center = size / 2;

    // Compute the Laplacian of Gaussian kernel
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int x = i - center;
            int y = j - center;
            float expo = -(x * x + y * y) / (2 * sigma * sigma);
            laplacianKernel[i][j] = (1 + expo) * exp(expo) / (M_PI * pow(sigma, 4));
        }
    }

    return laplacianKernel;
}

int main() {
    int size = 5; // Size of the kernel (odd)
    float sigma = 1.0; // Standard deviation of the Gaussian

    // Generate Laplacian of Gaussian kernel
    std::vector<std::vector<float>> laplacian = laplacianOfGaussianKernel(size, sigma);

    // Output the Laplacian of Gaussian kernel
    std::cout << "Laplacian of Gaussian Kernel:" << std::endl;
    for (const auto& row : laplacian) {
        for (float value : row) {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
