#include "Graph.h"

Graph::Graph(std::string path) 
{
    std::ifstream input(path);

    if (!input.is_open())
        throw std::runtime_error("Failed to open file");

    input >> num_verteces_ >> num_edges_;

    int v_from, v_to;

    for (int i = 0; i < num_edges_; i++) 
    {
        input >> v_from >> v_to;

        v_from--;
        v_to--;

        if (v_from < 0 || v_from >= num_verteces_ || v_to < 0 || v_to >= num_verteces_) 
            throw std::runtime_error("Invalid vertex index in edge definition");

        edges_.push_back(std::make_pair(v_from, v_to));
    }
    input.close();
};


Vector2d Graph::fRep(double edge_len, Vector2d v1, Vector2d v2)
{
    Vector2d v = v2 - v1; 
    double len = v.len();
    if (len > 20 * edge_len)
        return Vector2d(0, 0);
    if (v.x_ == 0 && v.y_ == 0) 
        return v / len * edge_len/2;

    double scalar = edge_len * edge_len / v.len();
    return v.direction() * (-scalar); 
}

Vector2d Graph::fAttr(double edge_len, Vector2d v1, Vector2d v2)
{
    
    Vector2d v = v2 - v1;
    double scalar = v.len() - edge_len;
    return v.direction() * scalar;
}

void Graph::AlgorithmFrick(int iterations, std::vector<Vector2d>& coordinates, double edge_len)
{
    srand(time(NULL));
    std::vector<Vector2d> movement;

    Vector2d gravity_center(0, 0);
    Vector2d move(0, 0);

    int i, j;
    double damping = 0.1; 
    int t = 0;
    while (iterations > t)
    {
        std::vector<Vector2d>(num_verteces_, Vector2d(0, 0)).swap(movement);

        for (i = 0; i < num_verteces_ - 1; i++)
        {
            for (j = i + 1; j < num_verteces_; j++)
            {
                move = fRep(edge_len, coordinates[i], coordinates[j]);
                movement[i] = movement[i] + move;
                movement[j] = movement[j] - move;
            }
        }

        for (auto& edge : edges_)
        {

            i = edge.first;
            j = edge.second;
            move = fAttr(edge_len, coordinates[i], coordinates[j]);
            movement[i] = movement[i] + move;
            movement[j] = movement[j] - move;
        }
        
        for (i = 0; i < num_verteces_; i++) 
        {
            if (movement[i].len() > 10000)
                movement[i] = movement[i].direction() * 10000;
            coordinates[i] = coordinates[i] + movement[i] * damping;
        }
        
        t++;
    }


    for (i = 0; i < num_verteces_ - 1; i++)
    {
        for (j = i + 1; j < num_verteces_; j++)
        {
            move = fRep(edge_len, coordinates[i], coordinates[j]);
            move = move / 2;
            coordinates[i] = coordinates[i] + move;
            coordinates[j] = coordinates[j] - move;
        }
    }
    
}


std::vector<std::pair<int, int> > GetLine(Vector2d from, Vector2d to) 
{
    std::vector<std::pair<int, int> > linevec;
    int deltaX = abs(to.x_ - from.x_);

    int signX =  -1;
    if (from.x_ < to.x_) signX = 1;

    int deltaY = abs(to.y_ - from.y_);

    int signY = -1;
    if (from.y_ < to.y_) signY = 1;

    std::pair<int, int> pixel;
    int error = deltaX - deltaY;
    int error2;
    pixel = std::make_pair(static_cast<int>(to.x_), static_cast<int>(to.y_));
    linevec.push_back(pixel);
    
    while (from.x_ != to.x_ || from.y_ != to.y_) 
    {
        pixel = std::make_pair(static_cast<int>(from.x_), static_cast<int>(from.y_));

        linevec.push_back(pixel);
        error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            from.x_ += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            from.y_ += signY;
        }
    }
   
    return linevec;
}


void Graph::DrawEdges(std::vector<std::vector<uint8_t> >& vertex_map, std::vector<Vector2d>& coordinates)
{
    std::vector<std::pair<int, int> > line;
    for (auto& edge : edges_) 
    {
        line = GetLine(coordinates[edge.first], coordinates[edge.second]);
        for (auto& p : line) 
            vertex_map[p.second][p.first] = 0;
    }
}

void Graph::AddCircle(Vector2d p, std::vector<std::vector<uint8_t> >& vertex_map, unsigned int image_w, unsigned int image_h) {
    unsigned int px, py;
    for (int y = -4; y <= 4; y++) 
    {
        for (int x = -4; x <= 4; x++) 
        {
            if (x * x + y * y <= 16) 
            {
                px = static_cast<unsigned int>(p.x_) + x;
                py = static_cast<unsigned int>(p.y_) + y;
                vertex_map[py][px] = 0;
            }
        }
    }
}

void Graph::AddNumber(int num, Vector2d p, std::vector<std::vector<uint8_t> >& vertex_map)
{
    std::string number = std::to_string(num + 1);
    unsigned int start_x = static_cast<unsigned int>(p.x_) + 2 * 4; 
    unsigned int start_y = static_cast<unsigned int>(p.y_) - 4; 

    std::string filename;
    BMP digit_img;
    for (auto& digit : number) 
    {
        filename = "../digits/" + std::string(1, digit) + ".bmp";
        digit_img.Read(filename);

        unsigned int width = digit_img.info_header_.width;
        unsigned int height = digit_img.info_header_.height;
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) 
                vertex_map[start_y + y][start_x + x] = digit_img.data_[y * width + x];
        }
        start_x += width;
    }
}

void Graph::DrawVertices(std::vector<Vector2d>& coordinates, std::vector<std::vector<uint8_t> >& vertex_map,
                       unsigned int image_w, unsigned int image_h) 
{
    for (size_t i = 0; i < num_verteces_; i++) 
    {
        AddCircle(coordinates[i], vertex_map, image_w, image_h);
        AddNumber(i, coordinates[i], vertex_map);
    }
}


void Graph::SaveTo(std::string& filename, std::vector<std::vector<uint8_t> >& vertex_map) 
{
    BMP graph_img;
    graph_img.Interpret(vertex_map);
    graph_img.Write(filename);
}


void Graph::Print(int weight, int hight, int iterations, std::string& filename)
{
    srand(time(NULL));
    double edge_len = sqrt(weight * hight / num_verteces_);
    if (edge_len < 30)
    {
        edge_len = 30;
        weight = 30 * sqrt(num_verteces_);
        hight = weight;
    }

    std::vector<Vector2d> coordinates;

    int i;

    for (i = 0; i < num_verteces_; i++)
        coordinates.push_back(Vector2d(rand() % (weight), rand() % (hight)));


    AlgorithmFrick(iterations, coordinates, edge_len);

    Vector2d min = coordinates[0];
    Vector2d max = coordinates[0];

    for (auto& point : coordinates)
    {
        if (point.x_ < min.x_) 
            min.x_ = point.x_;

        if (point.x_ > max.x_) 
            max.x_ = point.x_;
        
        if (point.y_ < min.y_)
            min.y_ = point.y_;

        if (point.y_ > max.y_)
            max.y_ = point.y_;
    }

    Vector2d indent(0, 0);

    if (min.x_ > 50) indent.x_ = 0;
    else if (min.x_ > 0) indent.x_ = 50;
    else indent.x_ = 50 - min.x_;

    if (min.y_ > 50) indent.y_ = 0;
    else if (min.y_ > 0) indent.y_ = 50;
    else indent.y_ = 50 - min.y_;


    for (auto& point : coordinates) 
    {
        point.x_ = std::round(point.x_ + indent.x_);
        point.y_ = std::round(point.y_ + indent.y_);
    }

    unsigned int image_w = std::round(max.x_ + indent.x_ + 50);
    image_w += (4 - image_w % 4) % 4;

    unsigned int image_h = std::round(max.y_ + indent.y_ + 50);

    std::vector<std::vector<uint8_t> > vertex_map(image_h, std::vector<uint8_t>(image_w, 255));

    DrawEdges(vertex_map, coordinates);

    DrawVertices(coordinates, vertex_map, image_w, image_h);

    SaveTo(filename, vertex_map);

}