#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "common.h"

static inline std::string trim(const std::string& s) {
    auto b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    auto e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

TSP TSP::LoadTSPLIB_GPT(std::string filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    int dimension = 0;
    std::string edge_weight_type;
    std::string line;
    bool haveDim = false, haveType = false;

    // читаем заголовок до NODE_COORD_SECTION
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line == "EOF") continue;
        if (line == "NODE_COORD_SECTION") break;
        auto pos = line.find(':');
        if (pos == std::string::npos) continue;
        std::string key = trim(line.substr(0, pos));
        std::string val = trim(line.substr(pos + 1));
        if (key == "DIMENSION") {
            dimension = std::stoi(val);
            haveDim = true;
        } else if (key == "EDGE_WEIGHT_TYPE") {
            edge_weight_type = val;
            haveType = true;
        }
    }

    if (!haveDim || dimension <= 0)
        throw std::runtime_error("Invalid or missing DIMENSION");
    if (!haveType || edge_weight_type != "EUC_2D")
        throw std::runtime_error("Unsupported EDGE_WEIGHT_TYPE: " + edge_weight_type);
    if (dimension >= 5e3)
        throw std::runtime_error("DIMENSION is too large");

    TSP M;
    M.assign(dimension, std::vector<double>(dimension, 0.0));
    M.coords.resize(dimension);

    // читаем ровно dimension строк с id, x, y
    for (int i = 0; i < dimension; ++i) {
        if (!std::getline(in, line))
            throw std::runtime_error("Unexpected EOF in NODE_COORD_SECTION");
        std::istringstream iss(line);
        int id;
        double x, y;
        if (!(iss >> id >> x >> y))
            throw std::runtime_error("Malformed coordinate line: " + line);
        if (id < 1 || id > dimension)
            throw std::runtime_error("Node ID out of range: " + std::to_string(id));
        M.coords[id - 1] = { x, y };
    }

    // заполняем матрицу расстояний
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            if (i == j) continue;
            double dx = M.coords[i].x - M.coords[j].x;
            double dy = M.coords[i].y - M.coords[j].y;
            double d  = std::sqrt(dx*dx + dy*dy);
            M[i][j]   = std::floor(d + 0.5);
        }
    }

    M.n = M.size();
    return M;
}
