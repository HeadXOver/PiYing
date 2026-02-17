#pragma once

#include <vector>

class Part;

class Parts final
{
	Parts() = default;
	~Parts();

public:
	Parts(const Parts&) = delete;
	Parts& operator=(const Parts&) = delete;

	static Parts& getInstance();

	void bind_texture(size_t index);
	void add(Part* part);
	void remove(size_t index);
	void remove_with_children(size_t index);
	void insert(size_t index, Part* part);
	void insert_from_to(size_t from, size_t to);
	void swap(size_t index1, size_t index2);
	void update_scale(size_t index);
	void add_single_to_draw(size_t index);
	void add_single_to_draw(Part* part);
	void add_to_draw_by_piying(Part* part = nullptr);
	void output_root_to_show(std::vector<Part*>& o_parts) const;
	void output_detail_to_show(std::vector<Part*>& o_parts, Part& part) const;

	bool is_empty() const;
	bool part_is_draw(size_t index) const;

	float get_prescale(size_t index) const;

	size_t size() const;

	unsigned int get_vao_piying(size_t index) const;

	Part* get_part(size_t index) const;

private:
	void add_part_to_is_draw(Part* part);
	void reset_layer();

private:
	static Parts _instance;

private:
	std::vector<Part*> _parts;
	std::vector<char> _part_is_draw;
};