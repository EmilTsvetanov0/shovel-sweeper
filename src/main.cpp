#include <iostream>
#include <random>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <ctime>
#include "../include/mines.h"

constexpr unsigned int window_height = 800, window_length = 1500;

void printUsage() {
	std::cout << "Использование: shovel-sweeper.exe -h <height> -w <width> -m <mines> [--auto-scale]\n"
			  << "Параметры:\n"
			  << "  -h <height>       Высота поля\n"
			  << "  -w <width>        Ширина поля\n"
			  << "  -m <mines>        Количество мин\n"
			  << "  --auto-scale      Включить автоматическое масштабирование поля пропорционально размеру окна (опционально)\n";
}

int main(int argc, char *argv[]){

	if (argc < 7) {
		std::cerr << "Ошибка: недостаточно аргументов.\n";
		printUsage();
		return 1;
	}

	int height = 0, width = 0, mines = 0;
	bool autoscale = false;

	try {
		for (int i = 1; i < argc; i += 2) {
			std::string flag = argv[i];
			if (flag == "-h") {
				height = std::stoi(argv[i + 1]);
			} else if (flag == "-w") {
				width = std::stoi(argv[i + 1]);
			} else if (flag == "-m") {
				mines = std::stoi(argv[i + 1]);
			} else if (flag == "--auto-scale") {
				autoscale = true;
				--i;
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

	sf::RenderWindow window(sf::VideoMode(window_length,window_height),"Shovel-sweeper");
	std::mt19937 generator(std::time(nullptr));

	auto rd = [&generator](const int x) {
		return static_cast<int>(generator()%x);
	};

	ms::field playground(window,20,20,"../resources/Minesweeper_LAZARUS_21x21_0.png");
	playground.generate_map(height,width,mines,rd);
	if(autoscale) {
		const int tile_height = playground.get_dimensions().second;
		const float scaler_y = window_height / (tile_height * height);
		playground.set_scale(scaler_y,scaler_y);
	} else {
		playground.set_scale(35.0f/21.0f, 35.0f/21.0f);
	}
	playground.draw();
	window.display();
	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				int mouse_x = event.mouseButton.x;
				int mouse_y = event.mouseButton.y;
				playground.click(mouse_x, mouse_y);
				window.clear();
				playground.draw();
				window.display();
			}
			else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
				int mouse_x = event.mouseButton.x;
				int mouse_y = event.mouseButton.y;
				playground.flag(mouse_x, mouse_y);
				window.clear();
				playground.draw();
				window.display();
			}
		}
	}

	return 0;
}
/*
g++ -c main.cpp -IC:\Users\tsvet\SFML-2.6.1\include
g++ main.o -o main -LC:\Users\tsvet\SFML-2.6.1\lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32
*/