#include "parts.h"

#include "part.h"

void Parts::bind_texture(size_t index)
{
    _parts[index]->bind_texture();
}

void Parts::add(Part* part)
{
    part->_lay_index = (int)_parts.size();
    _parts.push_back(part);
    _part_is_draw.push_back(false);
}

void Parts::remove(size_t index)
{
    _parts.erase(_parts.begin() + index);
    _part_is_draw.erase(_part_is_draw.begin() + index);
    for (; index < _parts.size(); ++index) {
        _parts[index]->_lay_index = index;
    }
}

void Parts::insert(size_t index, Part* part)
{
    _parts.insert(_parts.begin() + index, part);
    _part_is_draw.insert(_part_is_draw.begin() + index, false);
    for (; index < _parts.size(); ++index) {
        _parts[index]->_lay_index = index;
    }
}

void Parts::insert_from_to(size_t from, size_t to)
{
    Part* tmp = _parts[from];
    if (from < to) {
        --to;
        std::move(_parts.begin() + from + 1, _parts.begin() + to + 1, _parts.begin() + from);
        _parts[to] = tmp;        // 放入目标
        for (; from <= to; ++from) {
            _parts[from]->_lay_index = from;
        }
    }
    else {
        std::move(_parts.begin() + to, _parts.begin() + from, _parts.begin() + to + 1);
        _parts[to] = tmp;        // 放入目标
        for (; to <= from; ++to) {
            _parts[to]->_lay_index = to;
        }
    }
}

void Parts::swap(size_t index1, size_t index2)
{
    _parts[index1]->_lay_index = (int)index2;
    _parts[index2]->_lay_index = (int)index1;

    Part* tmp = _parts[index1];
    _parts[index1] = _parts[index2];
    _parts[index2] = tmp;

    bool tmpBool = _part_is_draw[index1];
    _part_is_draw[index1] = _part_is_draw[index2];
    _part_is_draw[index2] = tmpBool;
}

void Parts::update_scale(size_t index)
{
    _parts[index]->update_scale();
}

void Parts::add_single_to_draw(size_t index)
{
    _part_is_draw.assign(_part_is_draw.size(), false);

    add_part_to_is_draw(_parts[index]);
}

void Parts::add_single_to_draw(Part* part)
{
    _part_is_draw.assign(_part_is_draw.size(), false);

    add_part_to_is_draw(part);
}

void Parts::add_to_draw_by_piying(Part* part)
{
    _part_is_draw.assign(_part_is_draw.size(), false);

    if (!part) {
        for (int i = 0; i < _parts.size(); i++) {
            if (_parts[i]->is_root()) {
                add_part_to_is_draw(_parts[i]);
            }
        }
    }
}

void Parts::output_root_to_show(std::vector<Part*>& o_parts) const
{
    o_parts.clear();

    for (auto part : _parts) {
        if (part->is_root()) {
            o_parts.push_back(part);
        }
    }
}

bool Parts::is_empty() const
{
    return _parts.empty();
}

bool Parts::part_is_draw(size_t index) const
{
    return _part_is_draw[index];
}

size_t Parts::size() const
{
    return _parts.size();
}

unsigned int Parts::get_vao_piying(size_t index) const
{
    return _parts[index]->vao_piying();
}

Part* Parts::get_part(size_t index) const
{
    return _parts[index];
}

void Parts::add_part_to_is_draw(Part* part)
{
    _part_is_draw[part->_lay_index] = true;

    size_t n_children = part->n_children();

    for (size_t i = 0; i < n_children; ++i) {
        add_part_to_is_draw(part->get_child(i));
    }
}
