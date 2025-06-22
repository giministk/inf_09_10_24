#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <set>
#include <algorithm>
#include <random>
#include <string>

using namespace std;
using namespace sf;

Font font;

int GRID_SIZE = 10; // размер сетки
int CELL_SIZE = 75; // размер клетки
const int WINDOW_SIZE = 750;

// Типы ячеек
enum CellType {
    EMPTY,
    OBSTACLE,
    START,
    END,
    PATH,
    VISITED,
    QUEUED
};

// отдельные цвета для визуализации
Color Orange(229, 135, 34, 255);
Color Purple(90, 34, 139, 255);

// Структура для представления ячейки
struct Cell {
    int x, y;
    CellType type;
    int f, g, h, weight; // Для алгоритма A*
    Cell* parent;

    Cell(int x, int y) : x(x), y(y), type(EMPTY), f(0), g(INT_MAX), h(0), weight(0), parent(nullptr) {}
    /* g ставим как INT_MAX т.к.ещё неизвестно можно ли вообще добраться, а не изначально рассчитываем как с конечной точкой,
    иначе результаты могут быть спорными */

    // Для сравнения ячеек
    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
    void updateF() {
        f = g + h;
    }
};

// Функция для проверки, что мы не выходим за пределы сетки
bool isValid(int x, int y) {
    return (x >= 0) and (x < GRID_SIZE) and (y >= 0) and (y < GRID_SIZE);
}

// Манхэттенское расстояние
int heuristic(const Cell& a, const Cell& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

pair<pair<int, int>, pair<int, int>> make_grid(vector<vector<Cell>>& grid) {
    if (grid.empty()) { // для варианта
        int start_x = 0;
        int start_y = 4;
        int end_x = 9;
        int end_y = 9;
        vector<vector<int>> gridtempl = {
            {0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
            {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 1, 1, 0, 1, 0, 1, 0, 0},
            {1, 0, 0, 1, 0, 1, 1, 0, 0, 0},//поле
            {1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
        for (int x = 0; x < GRID_SIZE; x++) {
            vector<Cell> row;
            for (int y = 0; y < GRID_SIZE; y++) {
                Cell cell(x, y);
                cell.h = abs(x - end_x) + abs(y - end_y);
                cell.updateF();
                if (gridtempl[y][x]) cell.type = OBSTACLE;
                row.push_back(cell);
            }
            grid.push_back(row);
        }
        Cell* start = &grid[start_x][start_y];
        Cell* end = &grid[end_x][end_y];
        start->type = START;
        start->g = 0;
        end->type = END;
        pair<int, int> startp(start_x, start_y);
        pair<int, int> endp(end_x, end_y);
        return make_pair(startp, endp);
    }
    else {
        grid.clear();
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, GRID_SIZE - 1);
        int start_x = distrib(gen);
        int start_y = distrib(gen);
        int end_x = distrib(gen);
        int end_y = distrib(gen);
        while (start_x == end_x and start_y == end_y) {
            end_x = distrib(gen);
            end_y = distrib(gen);
        }

        for (int x = 0; x < GRID_SIZE; x++) {
            vector<Cell> row;
            for (int y = 0; y < GRID_SIZE; y++) {
                Cell cell(x, y);
                cell.h = abs(x - end_x) + abs(y - end_y);
                cell.updateF();
                row.push_back(cell);
            }
            grid.push_back(row);
        }

        Cell* start = &grid[start_x][start_y];
        Cell* end = &grid[end_x][end_y];
        start->type = START;
        start->g = 0;
        end->type = END;

        int obstacle_count = GRID_SIZE * GRID_SIZE * 0.2;
        for (int i = 0; i < obstacle_count; i++) {
            int x = distrib(gen);
            int y = distrib(gen);
            if (not (grid[x][y] == *start or grid[x][y] == *end)) {
                Cell* cell = &grid[x][y];
                cell->type = OBSTACLE;
                cell->weight = 0;
            }
        }
        pair<int, int> startp(start_x, start_y);
        pair<int, int> endp(end_x, end_y);
        return make_pair(startp, endp);
    }
}

void draw_grid(RenderWindow& window, vector<vector<Cell>>& grid) {
    for (int x = 0; x < grid.size(); x++) {
        for (int y = 0; y < grid.size(); y++) {
            RectangleShape cell(Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            switch (grid[x][y].type) {
            case EMPTY: cell.setFillColor(Color::White); break;
            case OBSTACLE: cell.setFillColor(Color::Black); break;
            case START: cell.setFillColor(Orange); break;
            case END: cell.setFillColor(Color::Cyan); break;
            case PATH: cell.setFillColor(Purple); break;
            case VISITED: cell.setFillColor(Color::Red); break;
            case QUEUED: cell.setFillColor(Color::Green); break;
            }
            cell.setOutlineColor(Color::Black);
            cell.setOutlineThickness(1);
            window.draw(cell);
            if (grid[x][y].weight != 0) {
                Text wText;
                wText.setFont(font);
                wText.setCharacterSize(12);
                wText.setFillColor(sf::Color::Black);
                wText.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                wText.setString(to_string(grid[x][y].weight));
                window.draw(wText);
            }
        }
    }
}

void reset_grid(vector<vector<Cell>>& grid, pair<pair<int, int>, pair<int, int>> st_end_pos) {
    pair<int, int> endp = st_end_pos.second;
    for (int x = 0; x < grid.size(); x++) {
        for (int y = 0; y < grid.size(); y++) {
            Cell* cell = &grid[x][y];
            if (cell->type != START and cell->type != OBSTACLE) cell->g = INT_MAX;
            cell->h = abs(x - endp.first) + abs(y - endp.second);
            cell->updateF();
            if (cell->type != END and cell->type != START and cell->type != OBSTACLE) cell->type = EMPTY;
            cell->parent = nullptr;
        }
    }
}

int A_star(vector<vector<Cell>>& grid, RenderWindow& window, pair<pair<int, int>, pair<int, int>> st_end_pos) {
    auto cmp = [](Cell* a, Cell* b) {
        return (a->f > b->f); 
        };
    priority_queue<Cell*, vector<Cell*>, decltype(cmp)> open_list(cmp);
    set<pair<int, int>> open_list_hash;
    set<pair<int, int>> closed_list;
    pair<int, int> startp = st_end_pos.first;
    pair<int, int> endp = st_end_pos.second;
    Cell* start = &grid[startp.first][startp.second];
    Cell* end = &grid[endp.first][endp.second];
    open_list.push(start);
    open_list_hash.insert(make_pair(start->x, start->y));
    vector<pair<int, int>> destinations = {
        make_pair(-1,0),
        make_pair(1, 0),
        make_pair(0, -1),
        make_pair(0, 1)
    };
    while (!open_list.empty()) {
        Cell* current = open_list.top();
        open_list.pop();
        if (current->type == END) {
            vector<Cell*> path;
            int weight = 0;
            while (current != nullptr) {
                path.push_back(&grid[current->x][current->y]);
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            for (int i = 0; i < path.size() - 1; i++) {
                Cell* cell = path[i];
                if (cell->type != START) cell->type = PATH;
                weight += cell->weight;
                window.clear(Color::White);
                draw_grid(window, grid);
                window.display();
            }
            cout << "length of shortest path (excluding start and end points): " << path.size() - 2 << endl << endl;
            return 0;
        }
        if (current->type != START) current->type = VISITED;
        closed_list.insert(make_pair(current->x, current->y));
        for (int i = 0; i < destinations.size(); i++) {
            int nx = current->x + destinations[i].first;
            int ny = current->y + destinations[i].second;
            if (isValid(nx, ny)) {
                Cell* neighbor = &grid[nx][ny];
                auto g = current->g + 1 + neighbor->weight;
                if (neighbor->type != OBSTACLE and neighbor->type != START) {
                    if (closed_list.find(make_pair(nx, ny)) == closed_list.end()) {
                        if (g < neighbor->g) {
                            neighbor->parent = current;
                            neighbor->g = g;
                            neighbor->h = heuristic(*neighbor, *end);
                            neighbor->updateF();
                            if (open_list_hash.find(make_pair(nx, ny)) == open_list_hash.end()) {
                                if (neighbor->type != END) neighbor->type = QUEUED;
                                open_list.push(neighbor);
                                open_list_hash.insert(make_pair(nx, ny));
                            }
                        }
                    }
                }
            }
        }
        window.clear(Color::White);
        draw_grid(window, grid);
        window.display();
    }
}

int main() {
    cout << "  press R to reset grid" << endl <<
        "  press SPACEb to launch A* pathfinder" << endl <<
        "  press Q to show controls" << endl;
    vector<vector<Cell>> grid;
    pair<pair<int, int>, pair<int, int>> st_end_pos;
    st_end_pos = make_grid(grid);
    bool pathfinderStarted = false;
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "A* algorithm showcase");
    window.setKeyRepeatEnabled(false);  // отключение зажима кнопок
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (event.type == Event::KeyPressed) {

                if (event.key.scancode == Keyboard::Scan::R) {
                    if (GRID_SIZE != 10) {
                        GRID_SIZE = 10;
                        CELL_SIZE = 75;
                        grid.clear();
                        st_end_pos = make_grid(grid);
                    }
                }

                else if (event.key.scancode == Keyboard::Scan::Q)
                    cout << "  press R to reset grid" << endl <<
                    "  press SPACEb to launch A* pathfinder" << endl <<
                    "  press Q to show controls" << endl;

                else if (event.key.scancode == Keyboard::Scan::Space and not pathfinderStarted) {
                    reset_grid(grid, st_end_pos);
                    pathfinderStarted = true;
                    A_star(grid, window, st_end_pos);
                    pathfinderStarted = false;
                }
            }
        }
        window.clear(Color::White);
        draw_grid(window, grid);
        window.display();
    }
    return 0;
}