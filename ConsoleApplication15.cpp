#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

class Shape {
public:
    virtual void Show() const = 0;
    virtual void Save(FILE* file) const = 0;
    virtual void Load(FILE* file) = 0;
    virtual ~Shape() = default;
};

class Square : public Shape {
private:
    int x, y, side;
public:
    Square(int x = 0, int y = 0, int side = 0)
        : x(x), y(y), side(side) {}

    void Show() const override {
        cout << "Square: Top-left (" << x << ", " << y
            << "), Side Length = " << side << endl;
    }

    void Save(FILE* file) const override {
        fprintf(file, "Square %d %d %d\n", x, y, side);
    }

    void Load(FILE* file) override {
        fscanf(file, "%d %d %d", &x, &y, &side);
    }
};

class Rectangle : public Shape {
private:
    int x, y, width, height;
public:
    Rectangle(int x = 0, int y = 0, int width = 0, int height = 0)
        : x(x), y(y), width(width), height(height) {}

    void Show() const override {
        cout << "Rectangle: Top-left (" << x << ", " << y
            << "), Width = " << width << ", Height = " << height << endl;
    }

    void Save(FILE* file) const override {
        fprintf(file, "Rectangle %d %d %d %d\n", x, y, width, height);
    }

    void Load(FILE* file) override {
        fscanf(file, "%d %d %d %d", &x, &y, &width, &height);
    }
};

class Circle : public Shape {
private:
    int cx, cy, radius;
public:
    Circle(int cx = 0, int cy = 0, int radius = 0)
        : cx(cx), cy(cy), radius(radius) {}

    void Show() const override {
        cout << "Circle: Center (" << cx << ", " << cy
            << "), Radius = " << radius << endl;
    }

    void Save(FILE* file) const override {
        fprintf(file, "Circle %d %d %d\n", cx, cy, radius);
    }

    void Load(FILE* file) override {
        fscanf(file, "%d %d %d", &cx, &cy, &radius);
    }
};

class Ellipse : public Shape {
private:
    int x, y, width, height;
public:
    Ellipse(int x = 0, int y = 0, int width = 0, int height = 0)
        : x(x), y(y), width(width), height(height) {}

    void Show() const override {
        cout << "Ellipse: Bounding Box Top-left (" << x << ", " << y
            << "), Width = " << width << ", Height = " << height << endl;
    }

    void Save(FILE* file) const override {
        fprintf(file, "Ellipse %d %d %d %d\n", x, y, width, height);
    }

    void Load(FILE* file) override {
        fscanf(file, "%d %d %d %d", &x, &y, &width, &height);
    }
};

void hufle(Shape* shapes[], int shapeCount, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file) {
        for (int i = 0; i < shapeCount; ++i) {
            shapes[i]->Save(file);
        }
        fclose(file);
    }
    else {
        cerr << "Could not open file for writing: " << filename << endl;
    }
}

void lostu(Shape* shapes[], int& shapeCount, int maxShapes, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        shapeCount = 0;
        char shapeType[20];
        while (fscanf(file, "%s", shapeType) == 1 && shapeCount < maxShapes) {
            Shape* shape = nullptr;
            if (strcmp(shapeType, "Square") == 0) {
                shape = new Square();
            }
            else if (strcmp(shapeType, "Rectangle") == 0) {
                shape = new Rectangle();
            }
            else if (strcmp(shapeType, "Circle") == 0) {
                shape = new Circle();
            }
            else if (strcmp(shapeType, "Ellipse") == 0) {
                shape = new Ellipse();
            }
            if (shape) {
                shape->Load(file);
                shapes[shapeCount++] = shape;
            }
        }
        fclose(file);
    }
    else {
        cerr << "Could not open file for reading: " << filename << endl;
    }
}

int main() {
    const int maxShapes = 10;
    Shape* shapes[maxShapes];
    int shapeCount = 0;
    shapes[shapeCount++] = new Square(0, 0, 10);
    shapes[shapeCount++] = new Rectangle(1, 1, 20, 30);
    shapes[shapeCount++] = new Circle(5, 5, 15);
    shapes[shapeCount++] = new Ellipse(2, 2, 40, 20);

    hufle(shapes, shapeCount, "shapes.txt");

    for (int i = 0; i < shapeCount; ++i) {
        delete shapes[i];
    }
    shapeCount = 0;

    lostu(shapes, shapeCount, maxShapes, "shapes.txt");

    for (int i = 0; i < shapeCount; ++i) {
        shapes[i]->Show();
        delete shapes[i];
    }

    return 0;
}
