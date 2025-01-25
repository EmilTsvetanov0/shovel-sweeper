//
// Created by tsvet on 23.01.2025.
//

#ifndef MINES_H
#define MINES_H

//
// Created by tsvet on 23.01.2025.
//

#include <vector>
#include <iostream>
#include <functional>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

namespace ms {
struct tile_template {
private:
	sf::Sprite square;
	sf::Texture texture;
	std::string base_path;
	int tile_width, tile_height;
	float scale_x=1, scale_y=1;
	size_t current_tile=0;
	sf::RenderWindow &window;

public:
	explicit tile_template(sf::RenderWindow &window, std::string base_path_init);

	void set_scale(const float scale_x_init, const float scale_y);

	void set_source(std::string base_path_init);

	std::pair<int,int> get_dimensions();

	void draw_open(const float pos_x, const float pos_y, const size_t value);

	void draw_closed(const float pos_x, const float pos_y);
};

struct field {
private:
	std::vector<std::vector<size_t>> grid;
	std::vector<std::vector<bool>> used;
	std::vector<std::vector<bool>> flagged;
	tile_template tile;
	int tile_width, tile_height;
	float scale_x=1, scale_y=1;
	int field_x, field_y;
	sf::RenderWindow &window;

	const std::vector<std::pair<int, int>> directions = {
		{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1},          {0, 1},
		{1, -1}, {1, 0},  {1, 1}
	};

	void dfs_zero(int x, int y);

	void click_internal(int x, int y);

public:
	explicit field(sf::RenderWindow &window, int field_x, int field_y, std::string source_folder_init);

	void clear(size_t field_width, size_t field_height);

	std::pair<int,int> get_dimensions();

	void set_scale(float scale_x_init, float scale_y_init);

	void set_source(std::string source_folder_init);

	void click(int mouse_pos_x, int mouse_pos_y);

	void flag(int mouse_pos_x, int mouse_pos_y);

	void generate_map(int rows, int cols, int mines, const std::function<int(int)>& random_generator);

	void draw();
};
}


#endif //MINES_H
