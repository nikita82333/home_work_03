#include <iostream>
#include <map>

#include "BlockAllocator.hpp"
#include "Container.hpp"

int main(int, char *[]) {

    try {

        auto factorial = [](int value){
            int result = 1;
            for (int i = 1; i <= value; ++i) {
                result *= i;
            }
            return result;
        };

        std::cout << "--- std::map + std::allocator ---" << std::endl;
        auto std_map = std::map<int, int>{};
        for (int i = 0; i < 10; ++i) {
            std_map[i] = factorial(i);
        }
        for (auto [key, value] : std_map) {
            std::cout << key << " " << value << std::endl;
        }

        std::cout << "--- std::map + custom allocator ---" << std::endl;
        auto custom_map = std::map<
                int, int, std::less<>,
                BlockAllocator<std::pair<const int, int>, 5>>{};

        for (int i = 0; i < 10; ++i) {
            custom_map[i] = factorial(i);
        }
        for (auto [key, value] : custom_map) {
            std::cout << key << " " << value << std::endl;
        }

        std::cout << "--- container (custom vector) + std::allocator ---" << std::endl;
        auto container = Container<int>{};
        for (int i = 0; i < 10; ++i) {
            container.add(i);
        }
        for (int i : container) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::cout << "--- container (custom vector) + custom allocator ---" << std::endl;
        auto custom_container = Container<int, BlockAllocator<int, 10>>{};
        //custom_container.resize(10);
        for (int i = 0; i < 10; ++i) {
            custom_container.add(i);
        }
        for (int i : custom_container) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::cout << "--- free memory ---" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
