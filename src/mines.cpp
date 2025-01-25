//
// Created by tsvet on 23.01.2025.
//

#include "../include/mines.h"

namespace ms {
	tile_template::tile_template(sf::RenderWindow &window, std::string base_path_init):base_path(std::move(base_path_init)), window(window) {
		try {
			if(!texture.loadFromFile(base_path)) throw std::exception();
			square.setTexture(texture);
		} catch(...) {
			std::cout<<"ms::tile_template:tile_template exception occurred: "<<base_path_init<<" is a not valid source folder. Either it is not a valid path either it doesn't contain 0.png"<<std::endl;
		}

		tile_width = texture.getSize().x * scale_x;
		tile_height = texture.getSize().y * scale_y;
	}

	void tile_template::set_scale(const float scale_x_init, const float scale_y_init) {
		tile_width = tile_width/scale_x * scale_x_init;
		tile_height = tile_height/scale_y * scale_y_init;
		scale_x = scale_x_init;
		scale_y = scale_y_init;
	}

	void tile_template::set_source(std::string base_path_init) {
		try {
			if(!texture.loadFromFile(base_path_init)) throw std::exception();
			square.setTexture(texture);
			base_path=std::move(base_path_init);
		} catch(...) {
			std::cout<<"ms::tile_template::set_base_path exception occurred: "<<base_path_init<<" is a not valid source folder. Either it is not a valid path either it doesn't contain 0.png"<<std::endl;
		}

		tile_width = texture.getSize().x * scale_x;
		tile_height = texture.getSize().y * scale_y;
	}

	std::pair<int,int> tile_template::get_dimensions() {
		return {tile_width, tile_height};
	}

	void tile_template::draw_open(const float pos_x, const float pos_y, const size_t value) {
		if(base_path.size()<5) throw std::invalid_argument("base_path is too small");

		try {
			if(value!=current_tile) {
				base_path[base_path.size()-5] = '0'+value;
				std::cout<<"Opening tile: "<<base_path<<std::endl;
				if(!texture.loadFromFile(base_path)) throw std::exception();
				square.setTexture(texture);
				square.setScale(scale_x, scale_y);
				current_tile=value;
			}
			square.setPosition(pos_x, pos_y);
			window.draw(square);
		} catch(...) {
			std::cout<<"ms::tile_template::draw_open texture load failed"<<std::endl;
			base_path[base_path.size()-5] = '0';
			current_tile=0;
		}
	}

	void tile_template::draw_closed(const float pos_x, const float pos_y) {
		if(base_path.size()<5) throw std::invalid_argument("ms::tile_template::draw_closed base_path is too small");

		try {
			if(current_tile!=51) {

				base_path[base_path.size()-5] = 'c';
				std::cout<<"Closed tile drawing: "<<base_path<<std::endl;

				if(!texture.loadFromFile(base_path)) throw std::exception();

				std::cout<<"Setting texture\n";
				square.setTexture(texture);
				square.setScale(scale_x, scale_y);
				current_tile=51;
			}
			square.setPosition(pos_x, pos_y);
			window.draw(square);
		} catch(...) {
			std::cout<<"ms::tile_template::draw_closed texture load failed"<<std::endl;
			base_path[base_path.size()-5] = '0';
			current_tile=0;
		}
	}

	void field::dfs_zero(const int x, const int y) {
		if(used[y][x]) return;
		used[y][x]=true;

		for (const auto& [dr, dc] : directions) {
			const int nr = y + dr;
			const int nc = x + dc;
			if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size()) {
				tile.draw_open(field_x+nc*tile_width,field_y+nr*tile_height,grid[nr][nc]);
				if(grid[nr][nc]==0) dfs_zero(nc,nr);
				else used[nr][nc]=true;
			}
		}
	}

	void field::click_internal(const int x, const int y) {
		if(used[y][x]) return;
		if(grid[y][x]==0) dfs_zero(x,y);
		else {
			used[y][x]=true;
			tile.draw_open(field_x+x*tile_width,field_y+y*tile_height,grid[y][x]);
		}
	}

	field::field(sf::RenderWindow &window, const int field_x, const int field_y, std::string source_folder_init):
		tile(window,std::move(source_folder_init)),
		field_x(field_x),
		field_y(field_y),
		window(window)
	{
		const auto dimensions = tile.get_dimensions();
		tile_width = dimensions.first;
		tile_height = dimensions.second;
	}

	void field::clear(const size_t field_width, const size_t field_height) {
		grid.resize(field_height, std::vector<size_t>(field_width,0));
		used.resize(field_height, std::vector<bool>(field_width,false));
	}

	std::pair<int,int> field::get_dimensions() {
		return {tile_width, tile_height};
	}

	void field::set_scale(const float scale_x_init, const float scale_y_init) {
		tile_width = tile_width/scale_x * scale_x_init;
		tile_height = tile_height/scale_y * scale_y_init;
		scale_x = scale_x_init;
		scale_y = scale_y_init;
		tile.set_scale(scale_x,scale_y);
	}

	void field::set_source(std::string source_folder_init) {
		tile.set_source(std::move(source_folder_init));
		const auto dimensions = tile.get_dimensions();
		tile_width = dimensions.first;
		tile_height = dimensions.second;
	}

	void field::click(const int mouse_pos_x, const int mouse_pos_y) {
		const int c=(mouse_pos_x-field_x)/tile_width;
		const int r=(mouse_pos_y-field_y)/tile_height;
		const int rows = grid.size();
		const int cols = grid[0].size();

		if (c < 0 || r < 0 || c >= grid[0].size() || r >= grid.size() || flagged[r][c]) return;
		if (used[r][c]) {
			int flags=0;
			for (const auto& [dr, dc] : directions) {
				const int nr = r + dr;
				const int nc = c + dc;
				if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && flagged[nr][nc]) {
					++flags;
				}
			}
			if(flags==grid[r][c]) {
				for (const auto& [dr, dc] : directions) {
					const int nr = r + dr;
					const int nc = c + dc;
					if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !flagged[nr][nc]) {
						click_internal(nc,nr);
					}
				}
			}
			return;
		}

		tile.draw_open(field_x+c*tile_width,field_y+r*tile_height,grid[r][c]);
		if(grid[r][c]==0) dfs_zero(c,r);
		else used[r][c]=true;
		for (const auto& [dr, dc] : directions) {
			const int nr = r + dr;
			const int nc = c + dc;
			if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc]==0) {
				dfs_zero(nc,nr);
			}
		}
	}

	void field::flag(const int mouse_pos_x, const int mouse_pos_y) {
		const int x=(mouse_pos_x-field_x)/tile_width;
		const int y=(mouse_pos_y-field_y)/tile_height;

		if (x < 0 || y < 0 || x >= grid[0].size() || y >= grid.size() || used[y][x]) return;

		if(!flagged[y][x]) {
			flagged[y][x]=true;
			tile.draw_open(field_x+x*tile_width,field_y+y*tile_height,'f'-'0');
		}
		else {
			flagged[y][x]=false;
			tile.draw_closed(field_x+x*tile_width,field_y+y*tile_height);
		}
	}

	void field::generate_map(const int rows, const int cols, const int mines, const std::function<int(int)>& random_generator) {
		if(rows <= 0 || cols <= 0 || mines <= 0 || mines > rows*cols) throw std::invalid_argument("Invalid input");
		grid.resize(rows, std::vector<size_t>(cols, 0));
		used.resize(rows, std::vector<bool>(cols, false));
		flagged.resize(rows, std::vector<bool>(cols, false));
		std::cout<<"tile_width: "<<tile_width<<std::endl;
		std::cout<<"tile_height: "<<tile_height<<std::endl;
		int placed = 0;
		while (placed < mines) {
			const int cell = random_generator(rows * cols);
			const size_t r = cell / cols;
			const size_t c = cell % cols;
			if (grid[r][c] != 9) {
				grid[r][c] = 9;
				++placed;
			}
		}

		for (int r = 0; r < rows; ++r) {
			for (int c = 0; c < cols; ++c) {
				if (grid[r][c] == 9) {
					for (const auto& [dr, dc] : directions) {
						const int nr = r + dr;
						const int nc = c + dc;
						if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != 9) {
							++grid[nr][nc];
						}
					}
				}
			}
		}
	}

	void field::draw() {
		for (size_t y = 0; y < grid.size(); ++y) {
			for (size_t x = 0; x < grid[0].size(); ++x) {
				if (used[y][x]) {
					tile.draw_open(field_x + x * tile_width, field_y + y * tile_height, grid[y][x]);
				} else if(flagged[y][x]) {
					tile.draw_open(field_x + x * tile_width, field_y + y * tile_height, 'f'-'0');
				} else {
					tile.draw_closed(field_x + x * tile_width, field_y + y * tile_height);
				}
			}
		}
	}
}
