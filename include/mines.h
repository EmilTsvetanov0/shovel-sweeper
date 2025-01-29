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
	// `base_path_init` should point to a file whose name ends with `0.png`.
	// This file must be in the same folder as other `.png` textures.
	// All files in the folder should have names of the same length as `base_path_init`.
	// The digit in the filename (`0-8.png`) represents the number of mines around the cell,
	// except for `9.png`, which indicates that the cell itself contains a mine.
	// Additionally you should have:
	//   - `c.png` — closed cell,
	//   - `f.png` — flagged cell.
	explicit tile_template(sf::RenderWindow &window, std::string base_path_init);

	// Sets parameters (x,y) for scaling the texture
	// Do not use for scaling the whole grid, it may result in shifting the whole tile tracking logic
	// If this is the case, instead, use field::set_scale
	void set_scale(const float scale_x_init, const float scale_y);

	// Set another source for the tiles textures
	void set_source(std::string base_path_init);

	// Returns pair<int,int> with tile_width and tile_height
	std::pair<int,int> get_dimensions();

	// Renders the tile with the texture determined by `value` in the position (pos_x, pos_y) of the screen
	void draw_open(const float pos_x, const float pos_y, const size_t value);

	// Renders the closed tile in the position (pos_x, pos_y) of the screen
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

	// Internal method needed for recursive opening of the zeros on the field
	void dfs_zero(int x, int y);

	// Method helper for click, takes index coordinates of the cell in the grid and renders it open if it is non zero
	// If it is zero, calls field::dfs_zero
	void click_internal(int x, int y);

public:
	// `base_path_init` should point to a file whose name ends with `0.png`.
	// This file must be in the same folder as other `.png` textures.
	// All files in the folder should have names of the same length as `base_path_init`.
	// The digit in the filename (`0-8.png`) represents the number of mines around the cell,
	// except for `9.png`, which indicates that the cell itself contains a mine.
	// Additionally you should have:
	//   - `c.png` — closed cell,
	//   - `f.png` — flagged cell.
	explicit field(sf::RenderWindow &window, int field_x, int field_y, std::string base_path_init);

	// Resizes and clears field
	void clear(size_t field_width, size_t field_height);

	// Returns width and height of the tile respectively
	std::pair<int,int> get_dimensions();

	// Set scaling parameters for width and height of the texture that will be applied to every tile in the field
	void set_scale(float scale_x_init, float scale_y_init);

	// Sets another source for the textures
	void set_source(std::string source_folder_init);

	// Processing click in the (mouse_pos_x, mouse_pos_y) position of the screen
	void click(int mouse_pos_x, int mouse_pos_y);

	// Just draws the tile at (mouse_pos_x, mouse_pos_y) position of the screen as ...0.png if it is closed
	// You should apply this method only after the field::draw method
	void hold(int mouse_pos_x, int mouse_pos_y);

	// Draws the tile at (mouse_pos_x, mouse_pos_y) position of the screen as a flag
	void flag(int mouse_pos_x, int mouse_pos_y);

	// Cleares the map and generating a random map
	void generate_map(int rows, int cols, int mines, const std::function<int(int)>& random_generator);

	// Renders current state of the field
	void draw();
};
}


#endif //MINES_H
