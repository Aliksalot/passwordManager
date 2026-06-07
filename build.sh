g++ -Wall -Wextra -fsanitize=address,undefined $(find . -name "*.cpp") -o pm.out && ./pm.out
