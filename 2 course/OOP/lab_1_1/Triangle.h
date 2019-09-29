#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <sstream>
#include <string>

class Triangle
{
    double A, B, C;

    constexpr static double eps = 0.0001; // точность для след. трех функций

    bool CheckForDopustimost (const double &a_in, const double &b_in, const double &c_in) const noexcept;
    bool CheckForTriangle (const double &a_in, const double &b_in, const double &c_in) const noexcept;
    void CallCheck(const double &a_in, const double &b_in, const double &c_in) const;

    double FindHeight(const double &side) const noexcept; // закрытый метод поиска высоты по стороне

    double FindAngle(const double &A, const double &B, const double &C) const noexcept; // закрытый метод поиска угла через теорему косинусов

    public:
        enum TriangleKind {pryamougolniy, typougolniy, ostrougolniy, ravnobedreniy, ravnostoroniy};

        void Init(const double &a_in, const double &b_in, const double &c_in)
        {
            CallCheck(a_in, b_in, c_in); // вызов проверки
            A = a_in;
            B = b_in;
            C = c_in;
        }

        void Init(const Triangle &t)
        {
            *this = t;
        }

        std::string toString() const noexcept
        {
            std::stringstream ss;
            ss << "a - " << A << ", b - " << B << ", c - " << C;
            std::string s = ss.str();
            return s;
        }

        void Display() const noexcept;
        void Read();
        // поиск периметра и площади
        double FindPerimeter() const noexcept;
        double FindSquare() const noexcept;
        // поиск высот
        double FindHeightA() const noexcept;
        double FindHeightB() const noexcept;
        double FindHeightC() const noexcept;
        // поиск углов
        double FindAngleAB() const noexcept;
        double FindAngleAC() const noexcept;
        double FindAngleBC() const noexcept;

        int TriangleKind() const noexcept;
        int Compare_square(const Triangle &second) const noexcept;
        int Compare_perimeter(const Triangle &second) const noexcept;
        bool Podobie(const Triangle &second) const;


};

#endif // TRIANGLE_H
