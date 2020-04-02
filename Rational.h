#include <tuple>
#include <utility>

class Rational {
private:
    int _numerator, _denominator;

    static int _gcd(int a, int b) {
        while (b) {
            a %= b;
            std::swap(a, b);
        }
        return a;
    }

    void normalize() {
        int gcd = _gcd(_numerator, _denominator);
        _numerator /= gcd;
        _denominator /= gcd;
        if (_denominator < 0) {
            _numerator = -_numerator;
            _denominator = -_denominator;
        }
    }

public:
    Rational(int num = 0, int den = 1) : _numerator(num), _denominator(den) {
        normalize();
    }

    [[nodiscard]] int numerator() const {
        return _numerator;
    }

    [[nodiscard]] int denominator() const {
        return _denominator;
    }

    Rational operator + () const  {
        return *this;
    }

    Rational operator - () const {
        return {-_numerator, _denominator};
    }

    Rational& operator = (const Rational& other) {
        if (this != &other) {
            _numerator = other._numerator;
            _denominator = other._denominator;
        }
        return *this;
    }

    Rational& operator += (const Rational other) {
        _numerator = _numerator * other._denominator + other._numerator * _denominator;
        _denominator *= other._denominator;
        normalize();
        return *this;
    }

    Rational& operator -= (const Rational other) {
        *this += -other;
        return *this;
    }

    Rational& operator *= (const Rational other) {
        _numerator *= other._numerator;
        _denominator *= other._denominator;
        normalize();
        return *this;
    }

    Rational& operator /= (const Rational other) {
        _numerator *= other._denominator;
        _denominator *= other._numerator;
        normalize();
        return *this;
    }

    friend bool operator == (const Rational& first, const Rational& second) {
        return (std::tie(first._numerator, first._denominator) ==
                std::tie(second._numerator, second._denominator));
    }

    friend bool operator != (const Rational& first, const Rational& second) {
        return (std::tie(first._numerator, first._denominator) !=
                std::tie(second._numerator, second._denominator));
    }

    friend Rational operator + (const Rational& first, const Rational& second) {
        return Rational(first) += second;
    }

    friend Rational operator - (const Rational& first, const Rational& second) {
        return Rational(first) -= second;
    }

    friend Rational operator * (const Rational& first, const Rational& second) {
        return Rational(first) *= second;
    }

    friend Rational operator / (const Rational& first, const Rational& second) {
        return Rational(first) /= second;
    }

    Rational operator++ (int) {
        Rational tmp(*this);
        *this += 1;
        return tmp;
    }

    Rational& operator++ () {
        *this += 1;
        return *this;
    }

    Rational operator-- (int) {
        Rational tmp(*this);
        *this -= 1;
        return tmp;
    }

    Rational& operator-- () {
        *this -= 1;
        return *this;
    }
};
