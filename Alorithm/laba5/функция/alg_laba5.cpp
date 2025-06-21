#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>
#include <windows.h>
#include <chrono>
#include "tinyxml2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
typedef pair<double, double> Coord;
typedef tuple<Coord, Coord, string> EdgeItem;
typedef map<Coord, vector<pair<Coord, double>>> Graph;

// Функция haversine: вычисляет расстояние между двумя точками на поверхности Земли (в километрах)
double haversine(const Coord& coord1, const Coord& coord2) {
    // Вычисляем расстояние между точками на поверхности Земли (в километрах)
    double lon1 = coord1.first, lat1 = coord1.second;
    double lon2 = coord2.first, lat2 = coord2.second;
    double R = 6371.0;  // Радиус Земли в км

    double phi1 = atan2(sin(lat1 * M_PI / 180.0), cos(lat1 * M_PI / 180.0));
    double phi2 = atan2(sin(lat2 * M_PI / 180.0), cos(lat2 * M_PI / 180.0));
    // Альтернативный вариант расчёта углов:
    phi1 = lat1 * M_PI / 180.0;
    phi2 = lat2 * M_PI / 180.0;
    double dphi = (lat2 - lat1) * M_PI / 180.0;
    double dlambda = (lon2 - lon1) * M_PI / 180.0;

    double a = sin(dphi / 2.0) * sin(dphi / 2.0) +
        cos(phi1) * cos(phi2) *
        sin(dlambda / 2.0) * sin(dlambda / 2.0);
    return 2 * R * atan2(sqrt(a), sqrt(1 - a));
}

// Функция dijkstra: поиск кратчайшего пути в графе
// graph: Граф представленный в виде словаря: {координата: список {(соседняя координата, расстояние)}}
// start: стартовая координата
// end: конечная координата
// Возвращает кортеж (путь, общее расстояние) ps - т.к. список улиц все равно возвращался пустым, убрал его из функции
tuple<vector<Coord>, double> dijkstra(const Graph& graph, const Coord& start, const Coord& end) {
    auto start_time = chrono::high_resolution_clock::now();

    // Приоритетная очередь для хранения (расстояние, узел)
    typedef pair<double, Coord> QueueItem;
    auto cmp = [](const QueueItem& left, const QueueItem& right) {
        return left.first > right.first;
        };
    std::priority_queue<QueueItem, vector<QueueItem>, decltype(cmp)> priority_queue(cmp);
    priority_queue.push(make_pair(0.0, start));

    // Словарь для хранения кратчайшего расстояния до каждого узла и предыдущего узла
    map<Coord, pair<Coord, double>> shortest_paths;
    shortest_paths[start] = make_pair(Coord(0, 0), 0.0);

    // Множество посещённых узлов
    vector<Coord> visited;

    while (!priority_queue.empty()) {
        double current_distance = priority_queue.top().first;
        Coord current_node = priority_queue.top().second;
        priority_queue.pop();

        // Если достигли конечного узла, прерываем поиск
        if (current_node == end) {
            break;
        }

        // Если узел уже посещён, пропускаем
        if (std::find(visited.begin(), visited.end(), current_node) != visited.end()) {
            continue;
        }
        visited.push_back(current_node);

        // Получаем список соседей, если он существует
        auto it = graph.find(current_node);
        if (it != graph.end()) {
            for (auto& neighbor_pair : it->second) {
                Coord neighbor = neighbor_pair.first;
                double distance = neighbor_pair.second;
                double total_distance = current_distance + distance;

                if (shortest_paths.find(neighbor) == shortest_paths.end() ||
                    total_distance < shortest_paths[neighbor].second) {
                    shortest_paths[neighbor] = make_pair(current_node, total_distance);
                    priority_queue.push(make_pair(total_distance, neighbor));
                }
            }
        }
    }

    // Восстановление пути
    vector<Coord> path;
    if (shortest_paths.find(end) == shortest_paths.end()) {
        return make_tuple(path, std::numeric_limits<double>::infinity());
    }

    // Реконструкция пути от конца к началу
    Coord current_node = end;
    while (true) {
        path.push_back(current_node);
        auto pred = shortest_paths[current_node].first;
        // Если предшественник равен (0,0) и расстояние равно 0, то это стартовый узел
        if (current_node == start)
            break;
        current_node = pred;
    }
    std::reverse(path.begin(), path.end());

    // Общее расстояние от начала до конца
    double total_distance = shortest_paths[end].second;

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Время работы алгоритма (мс): " << duration.count() << endl;

    return make_tuple(path, total_distance);
}

// Функция build_graph: строит граф из рёбер
// edges: список рёбер формата ((x1, y1), (x2, y2), название_улицы)
Graph build_graph(const vector<EdgeItem>& edges) {
    // Строим граф
    Graph graph;
    for (const auto& edge : edges) {
        Coord start, end;
        string street;
        tie(start, end, street) = edge;
        double dist = haversine(start, end);
        graph[start].push_back(make_pair(end, dist));
        graph[end].push_back(make_pair(start, dist));  // если граф неориентированный
    }
    return graph;
}

// кодировка
string utf8_to_cp1251(const string& utf8_str) {
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    wstring wide_str(wide_len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_len);

    int cp1251_len = WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    string cp1251_str(cp1251_len, '\0');
    WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, &cp1251_str[0], cp1251_len, nullptr, nullptr);

    return cp1251_str;
}

// Функция read_graphml: читает GraphML файл и возвращает узлы и ребра с названиями улиц
// file_path: путь к файлу .graphml
// Возвращает кортеж (nodes, edges), где:
// - nodes: словарь {node_id: (x, y)}
// - edges: список [((x1, y1), (x2, y2), название_улицы), ...]
tuple<map<string, Coord>, vector<EdgeItem>> read_graphml(const string& file_path) {
    map<string, Coord> nodes;
    vector<EdgeItem> edges;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        cerr << "Error loading XML file: " << file_path << endl;
        return make_tuple(nodes, edges);
    }

    tinyxml2::XMLElement* graphml = doc.FirstChildElement("graphml");
    if (!graphml) {
        cerr << "Нет файла" << endl;
        return make_tuple(nodes, edges);
    }

    tinyxml2::XMLElement* graph = graphml->FirstChildElement("graph");
    if (!graph) {
        cerr << "Нет элементов графа" << endl;
        return make_tuple(nodes, edges);
    }


    for (tinyxml2::XMLElement* node = graph->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {
        const char* id = node->Attribute("id");
        if (!id) continue;

        double x = 0, y = 0;
        bool has_coords = false;

        for (tinyxml2::XMLElement* data = node->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (!key) continue;

            if (strcmp(key, "d5") == 0) {
                x = atof(data->GetText());
                has_coords = true;
            }
            else if (strcmp(key, "d4") == 0) {
                y = atof(data->GetText());
                has_coords = true;
            }
        }

        if (has_coords) {
            nodes[id] = make_pair(x, y);
        }
    }


    for (tinyxml2::XMLElement* edge = graph->FirstChildElement("edge"); edge; edge = edge->NextSiblingElement("edge")) {
        const char* source_id = edge->Attribute("source");
        const char* target_id = edge->Attribute("target");
        if (!source_id || !target_id) continue;

        auto source_it = nodes.find(source_id);
        auto target_it = nodes.find(target_id);
        if (source_it == nodes.end() || target_it == nodes.end()) continue;

        string street_name;
        for (tinyxml2::XMLElement* data = edge->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (!key) continue;

            if (strcmp(key, "d12") == 0 && data->GetText()) { // street name
                street_name = utf8_to_cp1251(data->GetText());// нужно для преобразования кодировки
                // cout << "Улицы " << street_name << endl;
            }
        }

        edges.emplace_back(source_it->second, target_it->second, street_name);
    }

    cout << "Количество вершин " << nodes.size() << " ребер " << edges.size() << endl;

    return make_tuple(nodes, edges);
}

string convert_cp1251_to_utf8(const string& input) {
    // 1. CP1251 → UTF-16 (Windows-1251 → wchar_t)
    int wlen = MultiByteToWideChar(1251, 0, input.c_str(), -1, nullptr, 0);
    if (wlen <= 0) return "";
    wstring wstr(wlen, L'\0');
    MultiByteToWideChar(1251, 0, input.c_str(), -1, &wstr[0], wlen);

    // 2. UTF-16 → UTF-8
    int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (ulen <= 0) return "";
    string utf8_str(ulen, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8_str[0], ulen, nullptr, nullptr);

    return utf8_str;
}

// Функция find_street_index: Возвращает индекс (номер) и название улицы по заданному имени
// edges: список рёбер с названиями улиц
// street_name_query: название улицы для поиска
// Возвращает кортеж (индекс, название_улицы), если найдено, иначе (-1, "")
pair<int, string> find_street_index(const vector<EdgeItem>& edges, const string& street_name_query) {
    for (size_t i = 0; i < edges.size(); i++) {
        Coord start, end;
        string name;
        tie(start, end, name) = edges[i];
        if (!name.empty()) {
            // Приводим к нижнему регистру для сравнения
            string lc_name = name, query = street_name_query;
            transform(lc_name.begin(), lc_name.end(), lc_name.begin(), ::tolower);
            transform(query.begin(), query.end(), query.begin(), ::tolower);
            if (convert_cp1251_to_utf8(lc_name) == convert_cp1251_to_utf8(query)) {
                return make_pair(static_cast<int>(i), name);
            }
        }
    }
    return make_pair(-1, "");
}

// Функция visualize_path_with_network: Визуализация всей дорожной сети + маршрута красным.
// Если передан список street_names, то названия улиц выводятся вдоль маршрута.
// Используется SFML для визуализации
void visualize_path_with_network(const map<string, Coord>& nodes,
    const vector<EdgeItem>& edges,
    const vector<Coord>& path,
    const vector<string>& street_names = vector<string>(),
    const sf::Vector2u& figsize = sf::Vector2u(800, 800)) {
    // Создаём окно с заданным размером
    sf::RenderWindow window(sf::VideoMode(figsize.x, figsize.y), "Shortest path");

    // Определяем границы для масштабирования на основе всех координат
    double minX = numeric_limits<double>::max(), maxX = -numeric_limits<double>::max();
    double minY = numeric_limits<double>::max(), maxY = -numeric_limits<double>::max();
    // Обрабатываем узлы
    for (auto& node : nodes) {
        double x = node.second.first;
        double y = node.second.second;
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }
    // Функция для преобразования координат
    auto transform_coord = [=](const Coord& c) -> sf::Vector2f {
        double scaleX = (figsize.x - 40) / (maxX - minX + 1e-6);
        double scaleY = (figsize.y - 40) / (maxY - minY + 1e-6);
        float x = static_cast<float>((c.first - minX) * scaleX + 20);
        // Инвертируем y для графического отображения
        float y = static_cast<float>(figsize.y - ((c.second - minY) * scaleY + 20));
        return sf::Vector2f(x, y);
        };

    // Основной цикл окна
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Рисуем все рёбра серыми
        for (const auto& edge : edges) {
            Coord start, end;
            string dummy;
            tie(start, end, dummy) = edge;
            sf::Vertex line[] =
            {
                sf::Vertex(transform_coord(start), sf::Color(128,128,128,32)),
                sf::Vertex(transform_coord(end), sf::Color(128,128,128,32))
            };
            window.draw(line, 2, sf::Lines);
        }

        // Рисуем путь красным
        if (!path.empty() && path.size() > 1) {
            for (size_t i = 0; i < path.size() - 1; i++) {
                sf::Vertex line[] =
                {
                    sf::Vertex(transform_coord(path[i]), sf::Color::Red),
                    sf::Vertex(transform_coord(path[i + 1]), sf::Color::Red)
                };
                window.draw(line, 2, sf::Lines);
                // Отображаем названия улиц, если они заданы
                if (!street_names.empty() && i < street_names.size() && !street_names[i].empty()) {
                    sf::Font font;
                    if (!font.loadFromFile("arial.ttf")) {
                        // Если шрифт не найден, пропускаем отображение названия
                    }
                    else {
                        sf::Text text;
                        text.setFont(font);
                        text.setString(street_names[i]);
                        text.setCharacterSize(12);
                        text.setFillColor(sf::Color::Blue);
                        // Вычисляем среднюю точку отрезка
                        float midX = (transform_coord(path[i]).x + transform_coord(path[i + 1]).x) / 2;
                        float midY = (transform_coord(path[i]).y + transform_coord(path[i + 1]).y) / 2;
                        text.setPosition(midX, midY);
                        window.draw(text);
                    }
                }
            }
        }

        window.display();
    }
}

// Дублированная функция visualize_path_with_network (идентичная предыдущей)
// В оригинальном коде функция была определена дважды, поэтому здесь она реализована с другим именем для избежания конфликта
void visualize_path_with_network_duplicate(const map<string, Coord>& nodes,
    const vector<EdgeItem>& edges,
    const vector<Coord>& path,
    const vector<string>& street_names = vector<string>(),
    const sf::Vector2u& figsize = sf::Vector2u(800, 800)) {
    // Вызов оригинальной функции, так как реализация идентична
    visualize_path_with_network(nodes, edges, path, street_names, figsize);
}

// Функция visualize_only_path: Визуализирует только маршрут (без остального графа)
void visualize_only_path(const vector<Coord>& path, const sf::Vector2u& figsize = sf::Vector2u(400, 400)) {
    if (path.empty() || path.size() < 2) {
        cout << "Path doesn't exist or is empty" << endl;
        return;
    }

    // Создаём окно
    sf::RenderWindow window(sf::VideoMode(figsize.x, figsize.y), "Shortest path (no graph)");

    // Определяем границы для масштабирования
    double minX = numeric_limits<double>::max(), maxX = -numeric_limits<double>::max();
    double minY = numeric_limits<double>::max(), maxY = -numeric_limits<double>::max();
    for (const auto& c : path) {
        if (c.first < minX) minX = c.first;
        if (c.first > maxX) maxX = c.first;
        if (c.second < minY) minY = c.second;
        if (c.second > maxY) maxY = c.second;
    }
    auto transform_coord = [=](const Coord& c) -> sf::Vector2f {
        double scaleX = (figsize.x - 40) / (maxX - minX + 1e-6);
        double scaleY = (figsize.y - 40) / (maxY - minY + 1e-6);
        float x = static_cast<float>((c.first - minX) * scaleX + 20);
        float y = static_cast<float>(figsize.y - ((c.second - minY) * scaleY + 20));
        return sf::Vector2f(x, y);
        };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::White);

        // Рисуем путь красным
        for (size_t i = 0; i < path.size() - 1; i++) {
            sf::Vertex line[] =
            {
                sf::Vertex(transform_coord(path[i]), sf::Color::Red),
                sf::Vertex(transform_coord(path[i + 1]), sf::Color::Red)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
}

// Функция save_visualization: Сохраняет текущую визуализацию в файл
// filename: имя файла для сохранения
// dpi: разрешение изображения (не используется в данном варианте, так как SFML сохраняет по пикселям)
void save_visualization(const string& filename, int dpi = 300) {
    // Используем RenderTexture для сохранения визуализации
    sf::RenderTexture renderTexture;
    sf::Vector2u size(800, 800);
    if (!renderTexture.create(size.x, size.y)) {
        cerr << "Can't create RenderTexture" << endl;
        return;
    }
    renderTexture.clear(sf::Color::White);
    // Здесь не реализовано рисование, так как функция save_visualization
    // сохраняет текущую визуализацию, которая должна быть вызвана после завершения отрисовки.
    renderTexture.display();

    sf::Texture texture = renderTexture.getTexture();
    sf::Image screenshot = texture.copyToImage();
    if (!screenshot.saveToFile(filename)) {
        cerr << "Can't save image to file " << filename << endl;
    }
}

// Основной блок использования
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // 1. Загрузка данных
    map<string, Coord> nodes;
    vector<EdgeItem> edges;
    tie(nodes, edges) = read_graphml("minsk_road_network.graphml");
    //  print_edges(edges);
      // 2. Задаём названия улиц для начала и конца маршрута
    Coord start, end;
    string name;
    tie(start, end, name) = edges[edges.size() - 1];
    string start_street_query = "Абрыкосавая вуліца";      // Название улицы для старта
    string end_street_query = "вуліца Свярдлова";      // Название улицы для финиша

    //вуліца Каржанеўскага
    //Аэрадромная вуліца
    //вуліца Свярдлова

    // 3. Используем find_street_index для определения нужных рёбер
    pair<int, string> start_result = find_street_index(edges, start_street_query);
    cout << "Поиск пути от " << start_result.second << " до ";
    pair<int, string> end_result = find_street_index(edges, end_street_query);
    cout << end_result.second << endl;
    int start_index = start_result.first;
    int end_index = end_result.first;
    string start_street = start_result.second;
    string end_street = end_result.second;

    if (start_index == -1 || end_index == -1)cout << "Не удалось найти заданную улицу для начала или конца маршрута" << endl;
    else {
        // 4. Определяем стартовый и конечный узлы:
        // Используем первую точку ребра для старта и вторую точку ребра для финиша.
        Coord start_node = get<0>(edges[start_index]);
        Coord end_node = get<1>(edges[end_index]);

        // 5. Строим граф и ищем кратчайший путь
        Graph graph = build_graph(edges);
        vector<Coord> path;
        double distance;
        tie(path, distance) = dijkstra(graph, start_node, end_node);

        if (path.empty()) {
            cout << "Путь не найден" << endl;
        }
        else {
            cout << "Найден путь длиной " << distance << " км" << endl;
            cout << endl;

            // 6. Визуализация маршрута
            visualize_path_with_network(nodes, edges, path);
        }
    }

    return 0;
}