#include <iostream>
#include <memory>
#include <string>

class Expression {
public:
    [[nodiscard]] virtual int Evaluate() const = 0;
    [[nodiscard]] virtual std::string ToString() const = 0;
    virtual ~Expression() = default;
};

using ExpressionPtr = std::shared_ptr<Expression>;

class SumExp: public Expression {
private:
    ExpressionPtr ex1, ex2;

public:
    SumExp(ExpressionPtr ex1, ExpressionPtr ex2): ex1(std::move(ex1)), ex2(std::move(ex2)) {}

    [[nodiscard]] int Evaluate() const override {
        return ex1->Evaluate() + ex2->Evaluate();
    }

    [[nodiscard]] std::string ToString() const override {
        return ex1->ToString() + " + " + ex2->ToString();
    }
};

class ProductExp: public Expression {
private:
    ExpressionPtr ex1, ex2;

public:
    ProductExp(ExpressionPtr ex1, ExpressionPtr ex2): ex1(std::move(ex1)), ex2(std::move(ex2)) {}

    [[nodiscard]] int Evaluate() const override {
        return ex1->Evaluate() * ex2->Evaluate();
    }

    [[nodiscard]] std::string ToString() const override {
        if (isSum(ex1) && isSum(ex2)) {
            return "(" + ex1->ToString() + ") * (" + ex2->ToString() + ")";
        } else if (isSum(ex1)) {
            return "(" + ex1->ToString() + ") * " + ex2->ToString();
        } else if (isSum(ex2)) {
            return ex1->ToString() + " * (" + ex2->ToString() + ")";
        } else {
            return ex1->ToString() + " * " + ex2->ToString();
        }
    }

    // Slow. In the task I was not allowed to change base type Expression or make intermediate
    static bool isSum(const ExpressionPtr& ex) {
        return typeid(*ex) == typeid(SumExp);
    }
};

class ConstExp: public Expression {
private:
    int val;

public:
    explicit ConstExp(int val): val(val) {}

    [[nodiscard]] int Evaluate() const override {
        return val;
    }

    [[nodiscard]] std::string ToString() const override {
        return std::to_string(val);
    }
};

ExpressionPtr Sum(ExpressionPtr ex1, ExpressionPtr ex2) {
    return ExpressionPtr(new SumExp(std::move(ex1), std::move(ex2)));
}

ExpressionPtr Product(ExpressionPtr ex1, ExpressionPtr ex2) {
    return ExpressionPtr(new ProductExp(std::move(ex1), std::move(ex2)));
}

ExpressionPtr Const(int val) {
    return ExpressionPtr(new ConstExp(val));
}

int main() {
    ExpressionPtr ex1 = Sum(Product(Const(3), Const(4)), Const(5));
    std::cout << ex1->ToString() << "\n";  // 3 * 4 + 5
    std::cout << ex1->Evaluate() << "\n";  // 17

    ExpressionPtr ex2 = Product(Const(6), ex1);
    std::cout << ex2->ToString() << "\n";  // 6 * (3 * 4 + 5)
    std::cout << ex2->Evaluate() << "\n";  // 102
}