#include "Map.h"
#include "Engine.h"
#include <list>
#include <algorithm>

GameMap::GameMap(unsigned width, unsigned height)
{
        resize(width, height);
}

GameMap::GameMap(std::vector<std::string> data)
        : width(0), height(0)
{
        this->orig = data;
        restore();
}

GameMap::~GameMap()
{
        delete_objs();
}

void GameMap::add_game_obj(char c, juce::Point<int> pos)
{
        if (!map[pos.y][pos.x])
                set_tile(pos.y, pos.x, new GameMap::Tile());

        GameMap::Tile* tile = map[pos.y][pos.x];
        if (c == '#')
                tile->static_obj = new game_object::Wall(pos);
        else if (c == '$')
                tile->static_obj = new game_object::BreakableWall(pos);
        else if (c == '.')
                tile->static_obj = new game_object::Ground(pos);
        else if (c == 'z')
                add_dyn_obj(new game_object::Zombie(pos));
        else if (c == 's')
                add_dyn_obj(new game_object::Skeleton(pos));
        else if (c == 'd')
                add_dyn_obj(new game_object::Demon(pos));
        else if (c == 'D')
                add_dyn_obj(new game_object::Dragon(pos));
        else if (c == 'k')
        {
                game_object::Knight* player = new game_object::Knight(pos);
                set_player(player);
                add_dyn_obj(player);
        }
        else if (c == 'p')
                add_dyn_obj(new game_object::Princess(pos));
        else if (c >= '0' && c <= '9')
        {
                c -= '0';
                game_object::Portal* portal = new game_object::Portal(pos);
                tile->temp_objs.push_back(portal);
                tile->static_obj = new game_object::Ground(pos);
                if (!portals[c])
                        portals[c] = portal;
                else
                        portals[c]->bind(portal);
        }
        else
                return;
}

void GameMap::add_dyn_obj(game_object::DynamicObject * obj)
{
        juce::Point<int> pos = obj->get_pos();
        get_tile(pos)->static_obj = new game_object::Ground(pos);
        get_tile(pos)->dynamic_obj = obj;
}

void GameMap::clear_portals()
{
        portals.resize(10);
        for (int i = 0; i < 10; i++)
                portals[i] = nullptr;
}

void GameMap::resize(unsigned width, unsigned height)
{
        set_height(height);
        set_width(width);
        for (unsigned i = 0; i < height; i++)
                for (unsigned j = 0; j < width; j++)
                        map[i][j] = nullptr;
}

void GameMap::delete_objs()
{
        for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                {
                        Tile* tile = map[i][j];
                        delete tile->animations;
                        delete tile->static_obj;
                        delete tile->dynamic_obj;
                        delete tile;
                        map[i][j] = nullptr;
                }
        map.resize(0);
}

void GameMap::set_width(unsigned width)
{
        for (unsigned i = 0; i < map.size(); i++)
                map[i].resize(width);
        this->width = width;
}

void GameMap::set_height(unsigned height)
{
        map.resize(height);
        this->height = height;
}

void GameMap::restore()
{
        delete_objs();
        clear_portals();
        size_t width = 0;
        size_t height = orig.size();
        for (int i = 0; i < height; i++)
                width = std::max(width, orig[i].size());
        for (int i = 0; i < height; i++)
        {
                int size = orig[i].size();
                for (int j = 0; j < width - size; j++)
                        orig[i].push_back('#');
        }
        resize(width, height);
        for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                        add_game_obj(orig[i][j], juce::Point<int>(j, i));
}

unsigned GameMap::get_width() const
{
        return this->width;
}

unsigned GameMap::get_height() const
{
        return this->height;
}

bool GameMap::in_range(juce::Point<int> pos) const
{
        return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

bool GameMap::has_static_obj(juce::Point<int> pos) const
{
        return in_range(pos) && map[pos.y][pos.x] && map[pos.y][pos.x]->static_obj;
}

bool GameMap::has_dyn_obj(juce::Point<int> pos) const
{
        return in_range(pos) && map[pos.y][pos.x] && map[pos.y][pos.x]->dynamic_obj;
}

bool GameMap::has_temp_obj(juce::Point<int> pos) const
{
        return in_range(pos) && map[pos.y][pos.x] && map[pos.y][pos.x]->temp_objs.size() > 0;
}

bool GameMap::free_to_move(juce::Point<int> pos) const
{
        return !has_dyn_obj(pos) && has_static_obj(pos) && get_static_obj(pos)->passable();
}

void GameMap::set_player(game_object::Knight* player)
{
        this->player = player;
}

void GameMap::add_animation(Animation* anim, juce::Point<int> pos)
{
        if (!in_range(pos))
        {
                delete anim;
                return;
        }
        get_tile(pos)->animations->push(anim);
}

void GameMap::add_temp_obj(game_object::TempObject* temp)
{
        if (!in_range(temp->get_pos()))
        {
                delete temp;
                return;
        }
        get_tile(temp->get_pos())->temp_objs.push_back(temp);
}

void GameMap::set_tile(unsigned i, unsigned j, Tile* cell)
{
        this->map[i][j] = cell;
}

void GameMap::set_princess(game_object::Princess* princess)
{
        this->princess = princess;
}

game_object::Knight* GameMap::get_player() const
{
        return this->player;
}

game_object::Princess* GameMap::get_princess() const
{
        return this->princess;
}

game_object::DynamicObject* GameMap::get_dyn_obj(juce::Point<int> pos) const
{
        if (!in_range(pos))
                return nullptr;
        return get_tile(pos)->dynamic_obj;
}

std::list<GameMap::Tile*> GameMap::get_dyn_obj_tile_list() const
{
        std::list<GameMap::Tile*> result;
        for (unsigned i = 0; i < height; i++)
                for (unsigned j = 0; j < width; j++)
                        if (has_dyn_obj(juce::Point<int>(j, i)))
                                result.push_back(get_tile(juce::Point<int>(j, i)));
        return result;
}

std::list<game_object::DynamicObject*> GameMap::get_visible_dyn_obj_list() const
{
        std::list<game_object::DynamicObject*> result;
        RectIterator<int> it = Engine::get_camera_range();
        while (!it.end())
        {
                juce::Point<int> pos = (*it++);
                if (has_dyn_obj(pos) && player->see(pos))
                        result.push_back(get_dyn_obj(pos));

        }
        return result;
}

std::list<game_object::TempObject*>& GameMap::get_temp_objs(juce::Point<int> pos) const
{
        return get_tile(pos)->temp_objs;
}

game_object::StaticObject* GameMap::get_static_obj(juce::Point<int> pos) const
{
        if (!in_range(pos))
                return nullptr;
        return get_tile(pos)->static_obj;
}

std::vector<juce::Point<int>> GameMap::get_route(juce::Point<int> from, juce::Point<int> to) const
{
        if (!in_range(to))
                return std::vector<juce::Point<int>>();
        std::vector<std::vector<int>> route_data;
        std::vector<std::vector<int>> visited;
        route_data.resize(height);
        visited.resize(height);
        for (unsigned i = 0; i < height; i++)
        {
                route_data[i].resize(width, -1);
                visited[i].resize(width, 0);
        }
        route_data[from.y][from.x] = 0;
        visited[from.y][from.x] = 1;
        std::list<juce::Point<int>> queue;
        queue.push_back(from);
        while (queue.size() > 0)
        {
                juce::Point<int> pos = queue.front();
                queue.pop_front();
                for (int i = 0; i < 4; i++)
                {
                        juce::Point<int> new_pos = pos + directions[i];
                        if ((free_to_move(new_pos) || new_pos == to) && !visited[new_pos.y][new_pos.x])
                        {
                                route_data[new_pos.y][new_pos.x] = route_data[pos.y][pos.x] + 1;
                                visited[new_pos.y][new_pos.x] = 1;
                                queue.push_back(new_pos);
                        }
                }
                if (pos == to)
                        break;
        }
        std::vector<juce::Point<int>> result;
        juce::Point<int> pos = to;
        int distance = route_data[to.y][to.x];
        while (distance > 1)
        {
                for (int i = 0; i < 4; i++)
                {
                        juce::Point<int> new_pos = pos + directions[i];
                        if (in_range(new_pos) && route_data[new_pos.y][new_pos.x] != -1
                                && route_data[new_pos.y][new_pos.x] < route_data[pos.y][pos.x])
                        {
                                pos = new_pos;
                                break;
                        }
                }
                distance = route_data[pos.y][pos.x];
                result.push_back(pos);
        }
        std::reverse(result.begin(), result.end());
        return result;
}

GameMap::Tile* GameMap::get_tile(juce::Point<int> pos) const
{
        if (!in_range(pos))
                return nullptr;
        return map[pos.y][pos.x];
}
