#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <limits>
#include <array>


class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string getname() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual ~Shape() = default;

    friend std::ostream& operator<<(std::ostream& os, const Shape& shape) {
        shape.print(os);
        return os;
    }
};

template <typename T>
class Point : public Shape {
public:
    std::string name;
    T x, y, z;

    Point(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}
    Point(std::string name, T x = 0, T y = 0, T z = 0) : name(name), x(x), y(y), z(z) {}

    Point vector_by_two_points(const Point& other) const {
        return Point(x - other.x, y - other.y, z - other.z);
    }

    Point vector_multiply(const Point& other) const {
        return Point(y * other.z - z * other.y,
                     z * other.x - x * other.z,
                     x * other.y - y * other.x);
    }

    T length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    double area() const override {
        return 0;
    }

    double perimeter() const override {
        return 0;
    }

    std::string getname() const override {
        return name.empty() ? "Пустое имя" : name;
    }

    void print(std::ostream& os) const override {
        os << name <<"(" << x << ", " << y << ", " << z << ")";
    }
};

template <typename T>
class Plane : public Shape {
private:
    T A, B, C, D;
    std::string name;
public:
    Plane(T A, T B, T C, T D) : A(A), B(B), C(C), D(D) {}
    Plane(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3) {
        Point<T> v1 = p2.vector_by_two_points(p1);
        Point<T> v2 = p3.vector_by_two_points(p1);
        Point<T> normal = v1.vector_multiply(v2);
        A = normal.x;
        B = normal.y;
        C = normal.z;
        D = -(A * p1.x + B * p1.y + C * p1.z);
    }

    double area() const override {
        return std::numeric_limits<double>::infinity();
    }

    double perimeter() const override {
        return std::numeric_limits<double>::infinity();
    }

    std::string getname() const override {
        return name.empty() ? "Плоскость без имени" : name;
    }

    void print(std::ostream& os) const override {
        os << "Плоскость: " << name << "\n";
        os << A << "x + " << B << "y + " << C << "z + " << D << " = 0";
    }
};

template <typename T>
class Triangle : public Shape {
private:
    Point<T> p1, p2, p3;
    std::string name;
public:
    Triangle(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, std::string name = "")
        : p1(p1), p2(p2), p3(p3), name(name) {}

    double area() const override {

        Point<T> v1 = p2.vector_by_two_points(p1);
        Point<T> v2 = p3.vector_by_two_points(p1);
        return v1.vector_multiply(v2).length() / 2;
    }

    double perimeter() const override {

        return p1.vector_by_two_points(p2).length() +
               p2.vector_by_two_points(p3).length() +
               p3.vector_by_two_points(p1).length();
    }

    std::string getname() const override {
        return name.empty() ? "Треугольник без имени" : name;
    }

    void print(std::ostream& os) const override {
        os << "Треугольник: " << getname() << "\n";
        os << "Площадь: " << area() << "\n";
        os << "Периметр: " << perimeter();
    }
};

template <typename T>
class Square : public Shape {
private:
    Point<T> p1, p2, p3, p4;
    std::string name;
public:
    Square(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, const Point<T>& p4, std::string name = "")
        : p1(p1), p2(p2), p3(p3), p4(p4), name(name) {}

    double area() const override {
        T sideLength = p1.vector_by_two_points(p2).length();
        return sideLength * sideLength;
    }

    double perimeter() const override {
        return 4 * p1.vector_by_two_points(p2).length();
    }

    std::string getname() const override {
        return name.empty() ? "Квадрат без имени" : name;
    }

    void print(std::ostream& os) const override {
        os << "Квадрат: " << name << "со сторонами:\n";
        os << p1 << "\n" << p2 << "\n" << p3 << "\n" << p4 << "\n";
        os << "Площадь: " << area() << "\n";
        os << "Периметр: " << perimeter();
    }
};

template<typename T>
class Rectangle : public Shape {
public:
    std::string name;
    Point<T> p1, p2, p3, p4;

    Rectangle(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, const Point<T>& p4) 
        : p1(p1), p2(p2), p3(p3), p4(p4) {}
    
    Rectangle(std::string name, const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, const Point<T>& p4) 
        : name(name), p1(p1), p2(p2), p3(p3), p4(p4) {}

    double area() const override {
        T v1 = p2.vector_by_two_points(p1).length();
        T v2 = p4.vector_by_two_points(p1).length();
        return abs(v1*v2);
    }

    double perimeter() const override {
        return p1.vector_by_two_points(p2).length() +
               p2.vector_by_two_points(p3).length() +
               p3.vector_by_two_points(p4).length() +
               p4.vector_by_two_points(p1).length();
    }

    std::string getname() const override {
        return name.empty() ? "Прямоугольник без имени" : name;
    }

    void print(std::ostream& os) const override {
        os << "Прямоугольник: " << name << "\n";
    }
};


int main() {
    std::vector<Shape*> shapes;
    std::vector<Point<double>> points;
    int choice;

    while (true) {
        std::cout << "Выбери функцию:\n";
        std::cout << "1. Создать треугольник\n";
        std::cout << "2. Создать Плоскость\n";
        std::cout << "3. Создать Квадрат\n";
        std::cout << "4. Создать Прямоугольник\n";
        std::cout << "5. Добавить точку\n";
        std::cout << "6. Показать созданные фигуры для взаимодействия\n";
        std::cout << "7. Выйти из программы\n";

        std::cin >> choice;
        
        while(std::cin.fail() || choice > 7 || choice < 1){
            std::cout << "Выберете корректную функцию" << "\n" ;
            std::cin >> choice;
        }
        
        if (choice == 7) break;

        if (choice == 5) {
            double x, y, z;
            std::string name;

            while (true) {
                std::cout << "Введите координаты точки (x y z): ";
                std::cin >> x >> y >> z;
                std::cout << "Введите имя точки (x y z): ";
                std::cin >> name;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Неверный ввод. Пожалуйста, введите числа." << "\n";
                } else {
                    points.push_back(Point<double>(name, x, y, z));
                    break;
                }
            }
        } else if (choice == 1) {

            int inputMethod;
            std::cout << "Выберите способ создания:\n";
            std::cout << "1. Создать через координат\n";
            std::cout << "2. Использовать созданные точки\n";
            std::cin >> inputMethod;

            if (inputMethod == 1) {
                double x1, y1, z1, x2, y2, z2, x3, y3, z3;
                std::cout << "Введите координаты (x1 y1 z1 x2 y2 z2 x3 y3 z3): ";
                std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
                std::cout << "Введите имя фигуры ";
                std::string name;
                while(std::cin.fail()){
                    std::cout << "Введите имя фигуры " << "\n" ;
                    std::cin >> name;
                }
                shapes.push_back(new Triangle<double>(Point<double>(name + "_p1", x1, y1, z1),
                                                      Point<double>(name + "_p2", x2, y2, z2),
                                                      Point<double>(name + "_p3", x3, y3, z3),
                                                      name));
            } else if (inputMethod == 2) {
                if (points.size() < 3) {
                    std::cout << "Не хватает точек для создания треугольника\n";
                    continue;
                }
                std::string name;
                std::cout << "Доступные точки:\n";
                for (size_t i = 0; i < points.size(); ++i) {
                    std::cout << i + 1 << "  " << points[i].name <<  ": " << points[i] << "\n";
                }
                std::array<int, 3> idx;
                std::cout << "Выберете три точки из них (например 1 2 3): ";
                for (int &i : idx) {
                    std::cin >> i;
                    --i;
                }
                if (idx[0] < 0 || idx[0] >= points.size() ||
                    idx[1] < 0 || idx[1] >= points.size() ||
                    idx[2] < 0 || idx[2] >= points.size()) {
                    std::cout << "Выбраны неправильные точки!\n";
                    continue;
                }
                std::cout << "Введите имя треугольника: ";
                std::cin >> name;
                shapes.push_back(new Triangle<double>(points[idx[0]], points[idx[1]], points[idx[2]], name));
            }
        } else if (choice == 2) {
            int subChoice;
            std::cout << "Выберете тип создания:\n";
            std::cout << "1. Через введенные точки\n";
            std::cout << "2. Через координаты\n";
            std::cout << "3. Back\n";
            std::cin >> subChoice;

            if (subChoice == 1) {
                if (points.size() < 3) {
                    std::cout << "Не хватает точек для создания плоскости.\n";
                    continue;
                }
                std::array<int, 3> idx;
                for (size_t i = 0; i < points.size(); ++i) {
                    std::cout << i + 1 << ": " << points[i] << "\n";
                }
                std::cout << "Выберете три точки (1 2 3): ";
                for (int &i : idx) {
                    std::cin >> i;
                    --i;
                }
                if (idx[0] < 0 || idx[0] >= points.size() ||
                    idx[1] < 0 || idx[1] >= points.size() ||
                    idx[2] < 0 || idx[2] >= points.size()) {
                    std::cout << "Неправильные индексы выбраны!\n";
                    continue;
                }
                shapes.push_back(new Plane<double>(points[idx[0]], points[idx[1]], points[idx[2]]));
            } else if (subChoice == 2) {
                double x1, y1, z1, x2, y2, z2, x3, y3, z3;
                std::cout << "Выберет координаты для плоскости (x1 y1 z1 x2 y2 z2 x3 y3 z3): ";
                std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
                shapes.push_back(new Plane<double>(Point<double>(x1, y1, z1), Point<double>(x2, y2, z2), Point<double>(x3, y3, z3)));
            } else if (subChoice == 3) {
                continue;
            }
        } else if (choice == 3) {
            int inputMethod;
            std::cout << "Выберете способ создания:\n";
            std::cout << "1. Создание через координаты\n";
            std::cout << "2. Через существующие точки\n";
            std::cin >> inputMethod;

            if (inputMethod == 1) {
                double xA, yA, zA, xB, yB, zB, xC, yC, zC, xD, yD, zD;
                std::cout << "Введите 12 координат для квадрата (xA yA zA xB yB zB xC yC zC xD yD zD): ";
                std::cin >> xA >> yA >> zA >> xB >> yB >> zB >> xC >> yC >> zC >> xD >> yD >> zD;
                std::string name;
                std::cout << "Введите имя для квадрата: ";
                std::cin >> name;
                shapes.push_back(new Square<double>(Point<double>(xA, yA, zA),
                                                    Point<double>(xB, yB, zB),
                                                    Point<double>(xC, yC, zC),
                                                    Point<double>(xD, yD, zD),
                                                    name));
            } else if (inputMethod == 2) {
                if (points.size() < 4) {
                    std::cout << "Не хватает точек для создания квадрата.\n";
                    continue;
                }
                std::cout << "Доступные точки:\n";
                for (size_t i = 0; i < points.size(); ++i) {
                    std::cout << i + 1 << ": " << points[i] << "\n";
                }
                std::array<int, 4> idx;
                std::cout << "Выберете 4 точки (1 2 3 4): ";
                for (int &i : idx) {
                    std::cin >> i;
                    --i;
                }
                if (idx[0] < 0 || idx[0] >= points.size() ||
                    idx[1] < 0 || idx[1] >= points.size() ||
                    idx[2] < 0 || idx[2] >= points.size() ||
                    idx[3] < 0 || idx[3] >= points.size()) {
                    std::cout << "Неправильные индексы выбраны!\n";
                    continue;
                }
                std::string name;
                std::cout << "Введите имя для квадрата: ";
                std::cin >> name;
                shapes.push_back(new Square<double>(points[idx[0]], points[idx[1]], points[idx[2]], points[idx[3]], name));
            }
        } else if (choice == 4) {
            int inputMethod;
            std::cout << "Введите тип содзания прямоугольника:\n";
            std::cout << "1. Создание через координаты\n";
            std::cout << "2. Создание через введенные точки\n";
            std::cin >> inputMethod;

            if (inputMethod == 1) {
                double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
                std::string name;

                std::cout << "Введите имя прямоугольника: ";
                std::cin >> name;

                std::cout << "Введите 12 координат";

                std::cin >> x1 >> y1 >> z1;
                std::cin >> x2 >> y2 >> z2;
                std::cin >> x3 >> y3 >> z3;
                std::cin >> x4 >> y4 >> z4;

                Point<double> p1(x1, y1, z1);
                Point<double> p2(x2, y2, z2);
                Point<double> p3(x3, y3, z3);
                Point<double> p4(x4, y4, z4);

                shapes.push_back(new Rectangle<double>(name, p1, p2, p3, p4));
            } else if (inputMethod == 2) {
                if (points.size() < 4) {
                    std::cout << "Не хватает точек для создания прямоугольника.\n";
                    continue;
                }

                std::cout << "Доступные точки:\n";
                for (size_t i = 0; i < points.size(); ++i) {
                    std::cout << i + 1 << ": " << points[i] << "\n";
                }

                int idx[4];
                std::cout << "Выберете 4 точки: ";
                for (int &i : idx) {
                    std::cin >> i;
                    --i;
                    if (i < 0 || i >= points.size()) {
                        std::cout << "Неправильные индексы выбраны!\n";
                        continue;
                    }
                }
                std::string name;
                std::cout << "Введите имя треугольника:" << "\n";
                std::cin >> name;
                shapes.push_back(new Rectangle<double>(name, points[idx[0]], points[idx[1]],points[idx[2]],points[idx[3]]));
            }
            continue;
        }else if (choice == 6) {
            if (shapes.empty()) {
                std::cout << "Нет созданных фигур!\n";
                continue;
            }
            std::cout << "Созданные фигуры:\n";
            for (size_t i = 0; i < shapes.size(); ++i) {
                std::cout << i + 1 << ": " << shapes[i]->getname() << "\n";
            }
            int shapeChoice;
            std::cout << "Выберете фигуру чтобы узнать ифнормацию о ней: ";
            std::cin >> shapeChoice;
            --shapeChoice;
            if (shapeChoice < 0 || shapeChoice >= shapes.size()) {
                std::cout << "Выбран неверный номер!\n";
                continue;
            }
            Shape* selectedShape = shapes[shapeChoice];
            std::cout << "Выбранная фигура: " << selectedShape->getname() << "\n";
            std::cout << "Площадь: " << selectedShape->area() << "\n";
            std::cout << "Периметр: " << selectedShape->perimeter() << "\n";
            std::cout << "\n\n\n";
    }
}

    std::cout << "Созданные фигуры\n";
    for (const auto &shape : shapes) {
        if (shape) {
            std::cout << *shape << "\n";
        }
    }

    for (const auto &shape : shapes) {
        delete shape;
    }

    return 0;
}