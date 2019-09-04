#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <sstream>
#include <string>

class Triangle
{
    double A, B, C;
    bool Check(const double &a_in, const double &b_in, const double &c_in);

    public:
        void Init(const double &a_in, const double &b_in, const double &c_in)
        {
            if (Check(a_in, b_in, c_in))
            {
                A = a_in;
                B = b_in;
                C = c_in;
            }
        }
        void Init(const Triangle &t)
        {
            *this = t;
        }
        std::string toString()
        {
            std::stringstream ss;
            ss << "a: " << A << ", b: " << B << ", c: " << C;
            std::string s = ss.str();
            return s;
        }
        void Read();
        void Display();
        double FindSquare() const;
        double FindPerimeter() const;
        void TriangleKind();
        void FindHeight();
        void FindAngle(double &Angle_ab, double &Angle_ac, double &Angle_bc);
        void DisplayAngles(const double &Angle_ab, const double &Angle_ac, const double &Angle_bc);
        void CompareTwoTriangles(const Triangle &second);
        void Podobie(const Triangle &second);



};

#endif // TRIANGLE_H
