#include <iostream>
#include <vector>
#include <mpi.h>
#include <algorithm>

// Функция для выполнения локальной фильтрации медианным фильтром
void medianFilter(std::vector<int>& image, int width, int height) {
    std::vector<int> filteredImage(image.size());

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            std::vector<int> neighborhood = {
                image[(y - 1) * width + x - 1], image[(y - 1) * width + x], image[(y - 1) * width + x + 1],
                image[y * width + x - 1], image[y * width + x], image[y * width + x + 1],
                image[(y + 1) * width + x - 1], image[(y + 1) * width + x], image[(y + 1) * width + x + 1]
            };

            sort(neighborhood.begin(), neighborhood.end());
            filteredImage[y * width + x] = neighborhood[4];  // Значение медианы
        }
    }

    // Заменяем исходное изображение отфильтрованным изображением
    image = filteredImage;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int width = 10;    // Ширина изображения
    int height = 10;   // Высота изображения

    // Генерируем растровое изображение на процессе с рангом 0
    std::vector<int> image(width * height);
    if (rank == 0) {
        for (int i = 0; i < width * height; ++i) {
            image[i] = rand() % 256;  // Случайное значение пикселя
        }
    }

    // Отправляем изображение с процесса с рангом 0 на все остальные процессы
    MPI_Bcast(image.data(), width * height, MPI_INT, 0, MPI_COMM_WORLD);

    // Засекаем время выполнения локальной фильтрации медианным фильтром
    double start_time = MPI_Wtime();

    // Выполняем локальную фильтрацию медианным фильтром на всех процессах
    medianFilter(image, width, height);

    // Собираем отфильтрованные изображения со всех процессов на процессе с рангом 0
    std::vector<int> filteredImage(width * height);
    MPI_Gather(image.data(), width * height / size, MPI_INT,
        filteredImage.data(), width * height / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Выводим время выполнения итоговой фильтрации на процессе с рангом 0
    if (rank == 0) {
        double end_time = MPI_Wtime();
        std::cout << "Time: " << end_time - start_time << std::endl;
    }

    MPI_Finalize();
    return 0;
}
