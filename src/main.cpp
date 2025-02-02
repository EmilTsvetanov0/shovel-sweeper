#include <iostream>
#include <random>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <ctime>
#include "../include/mines.h"

void printUsage() {
	std::cout << "Использование: shovel-sweeper.exe -h <height> -w <width> -m <mines> [--auto-scale]\n"
			  << "Параметры:\n"
			  << "  -h <height>       Высота поля\n"
			  << "  -w <width>        Ширина поля\n"
			  << "  -m <mines>        Количество мин\n";
}

int main(int argc, char *argv[]){

	if (argc < 7) {
		std::cerr << "Ошибка: недостаточно аргументов.\n";
		printUsage();
		return 1;
	}

	int height = 0, width = 0, mines = 0;

	try {
		for (int i = 1; i < argc; i += 2) {
			std::string flag = argv[i];
			if (flag == "-h") {
				height = std::stoi(argv[i + 1]);
			} else if (flag == "-w") {
				width = std::stoi(argv[i + 1]);
			} else if (flag == "-m") {
				mines = std::stoi(argv[i + 1]);
			} else {
				std::cerr << "Неизвестный флаг: " << flag << "\n";
				printUsage();
				return 1;
			}
		}
	} catch (const std::invalid_argument& e) {
		std::cerr << "Ошибка: один из параметров не является числом.\n";
		printUsage();
		return 1;
	} catch (const std::out_of_range& e) {
		std::cerr << "Ошибка: один из параметров выходит за пределы допустимого диапазона.\n";
		printUsage();
		return 1;
	}
	if( width <= 0 || height <= 0 || mines < 0 || mines > width * height ) {
		std::cerr << "Ошибка: один из параметров либо отсутствует, либо принимает недопустимое значение.";
		printUsage();
		return 1;
	}
	const unsigned int window_length = width * 36 + 40;
	const unsigned int window_height = height * 36 + 40;
	sf::RenderWindow window(sf::VideoMode(window_length,window_height),"Shovel-sweeper");
	std::mt19937 generator(std::time(nullptr));

	auto rd = [&generator](const int x) {
		return static_cast<int>(generator()%x);
	};

	ms::field playground(window,20,20,"../resources/Minesweeper_LAZARUS_21x21_0.png");
	playground.generate_map(height,width,mines,rd);
	playground.set_scale(36.0f/21.0f, 36.0f/21.0f);
	playground.draw();
	window.display();
	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				int mouse_x = event.mouseButton.x;
				int mouse_y = event.mouseButton.y;
				playground.click(mouse_x, mouse_y);
				window.clear();
				playground.draw();
				window.display();
			} else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
				int mouse_x = event.mouseButton.x;
				int mouse_y = event.mouseButton.y;
				playground.flag(mouse_x, mouse_y);
				window.clear();
				playground.draw();
				window.display();
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
			int mouse_x = mouse_position.x;
			int mouse_y = mouse_position.y;

			window.clear();
			playground.draw();
			playground.hold(mouse_x, mouse_y);
			window.display();
		}
	}

	return 0;
}