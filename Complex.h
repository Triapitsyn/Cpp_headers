#include <cmath>
#include <tuple>

class Complex {
private:
    double re, im;

public:
    Complex(double re = 0, double im = 0): re(re), im(im) {}

    [[nodiscard]] double Re() const {
        return re;
    }

    [[nodiscard]] double Im() const {
        return im;
    }

    friend bool operator == (const Complex& first, const Complex& second) {
        return std::tie(first.re, first.im) == std::tie(second.re, second.im);
    }

    friend bool operator != (const Complex& first, const Complex& second) {
        return std::tie(first.re, first.im) != std::tie(second.re, second.im);
    }

    Complex operator + () const  {
        return *this;
    }

    Complex operator - () const {
        return {-re, -im};
    }

    friend Complex operator + (const Complex& first, const Complex& second) {
        return {first.re + second.re, first.im + second.im};
    }

    Complex operator += (const Complex& other) {
        return {re + other.re, im + other.im};
    }

    friend Complex operator - (const Complex& first, const Complex& second) {
        return {first.re - second.re, first.im - second.im};;
    }

    Complex operator -= (const Complex& other) {
        return {re - other.re, im - other.im};
    }

    friend Complex operator * (const Complex& first, const Complex& second) {
        return {first.re * second.re - first.im * second.im,
                first.re * second.im + first.im * second.re};
    }

    Complex operator *= (const Complex& other) {
        return *this = *this * other;
    }

    friend Complex operator / (const Complex& first, const Complex& second) {
        double denominator = second.re * second.re + second.im * second.im;
        return first * Complex(second.re, -second.im) * (1 / denominator);
    }

    Complex operator /= (const Complex& other) {
        return *this = *this / other;
    }

    friend double abs(const Complex& a) {
        return sqrt(a.Re() * a.Re() + a.Im() * a.Im());
    }
};