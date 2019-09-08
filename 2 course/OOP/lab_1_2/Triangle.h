#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <sstream>
#include <string>

class Triangle
{
    double A, B, C;

    bool Check(const double &a_in, const double &b_in, const double &c_in) const;

    public:
        Triangle(){};
        Triangle(const double &a_in, const double &b_in, const double &c_in)
        {
            if (Check(a_in, b_in, c_in))
            {
                A = a_in;
                B = b_in;
                C = c_in;
            }
        }

        std::string toString() const
        {
            std::stringstream ss;
            ss << "a - " << A << ", b - " << B << ", c - " << C;
            std::string s = ss.str();
            return s;
        }
        void Display() const;
        void Read();
        double FindPerimeter() const;
        double FindSquare() const;
        void FindHeight() const;
        void FindAngle(double &Angle_ab, double &Angle_ac, double &Angle_bc) const;
        void DisplayAngles() const;
        void TriangleKind() const;
        void CompareTwoTriangles(const Triangle &second) const;
        void Podobie(const Triangle &second) const;



};

#endif // TRIANGLE_H
