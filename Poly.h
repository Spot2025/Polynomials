#ifndef POLY_H
#define POLY_H

#include <map>
#include <vector>
#include <utility>

#include "Matrix.h"

inline std::vector<std::string> convert{"x", "y", "z", "w", "t", "k"};

static std::string converter(const std::vector<size_t>& to_convert) {
    std::string result;
    for (size_t i = 0; i < to_convert.size(); i++) {
        if (to_convert[i] == 0) {
            continue;
        }
        result += convert[i];
        if (to_convert[i] > 1) {
            result += "^" + std::to_string(to_convert[i]);
        }
    }

    return result;
}

template <class T>
T Pow(T value, size_t n) {
    T res(1);
    while (n > 0) {
        if (n % 2 == 1) {
            res *= value;
        }
        value *= value;
        n /= 2;
    }
    return res;
}

template <class T>
Matrix<T> Pow(Matrix<T> value, size_t n) {
    Matrix<T> res(value.Size(), 1);
    while (n > 0) {
        if (n % 2 == 1) {
            res *= value;
        }
        value *= value;
        n /= 2;
    }
    return res;
}

template <class T>
class Polynomial {
    size_t dimension_ = 1;
    std::map<std::vector<size_t>, T> coefficients_;
public:
    template <class Y>
    friend class Polynomial;

    Polynomial() = default;

    // <--- OPERATIONS WITH CONSTANTS --->

    Polynomial(const T& value) {
        coefficients_[std::vector<size_t>(1)] = value;
    }

    template <class Y>
    Polynomial& operator+=(const Y& value) {
        coefficients_[std::vector<size_t>(dimension_)] += value;

        return *this;
    }

    template <class Y>
    friend Polynomial operator+(Polynomial lhs, const Y& rhs) {
        lhs += rhs;
        return lhs;
    };

    template <class Y>
    Polynomial& operator-=(const Y& value) {
        coefficients_[std::vector<size_t>(dimension_)] -= value;

        return *this;
    }

    template <class Y>
    Polynomial operator-(const Y& rhs) {
        Polynomial lhs(*this);
        lhs -= rhs;
        return lhs;
    };

    template <class Y>
    Polynomial& operator*=(const Y& value) {
        for (auto& pair : coefficients_) {
            pair.second *= value;
        }

        return *this;
    }

    template <class Y>
    Polynomial operator*(const Y& rhs) {
        Polynomial lhs(*this);
        lhs *= rhs;
        return lhs;
    }

    template <class Y>
    friend Polynomial operator*(const Y& rhs, Polynomial lhs) {
        lhs *= rhs;
        return lhs;
    }

    // <--- OPERATIONS WITH POLYNOMIALS --->

    template <class Y>
    Polynomial& operator+=(const Polynomial<Y>& rhs) {
        *this = *this + rhs;
        return *this;
    }

    template <class Y>
    Polynomial operator+(const Polynomial<Y>& rhs) {
        Polynomial res;
        res.dimension_ = std::max(dimension_, rhs.GetDimension());
        for (auto& [key, value] : GetCoefficients()) {
            auto right_key = key;
            right_key.resize(res.dimension_);
            res.coefficients_[right_key] += value;
        }
        for (auto& [key, value] : rhs.GetCoefficients()) {
            auto right_key = key;
            right_key.resize(res.dimension_);
            res.coefficients_[right_key] += value;
        }

        return res;
    }

    template <class Y>
    Polynomial& operator-=(const Polynomial<Y>& rhs) {
        *this = *this - rhs;
        return *this;
    }

    template <class Y>
    Polynomial operator-(const Polynomial<Y>& rhs) {
        Polynomial res;
        res.dimension_ = std::max(dimension_, rhs.GetDimension());
        for (auto& [key, value] : GetCoefficients()) {
            auto right_key = key;
            right_key.resize(dimension_);
            res.coefficients_[right_key] += value;
        }
        for (auto& [key, value] : rhs.GetCoefficients()) {
            auto right_key = key;
            right_key.resize(dimension_);
            res.coefficients_[right_key] -= value;
        }

        return res;
    }

    template <class Y>
    Polynomial& operator*=(const Polynomial<Y>& rhs) {
        dimension_ = std::max(dimension_, rhs.GetDimension());

        std::map<std::vector<size_t>, T> new_coefficients;
        for (auto& [key1, value1] : coefficients_) {
            for (auto & [key2, value2] : rhs.GetCoefficients()) {
                std::vector<size_t> new_key(dimension_);
                for (size_t i = 0; i < key1.size(); i++) {
                    new_key[i] += key1[i];
                }
                for (size_t i = 0; i < key2.size(); i++) {
                    new_key[i] += key2[i];
                }

                new_coefficients[new_key] += value1 * value2;
            }
        }

        coefficients_ = std::move(new_coefficients);

        return *this;
    }

    template <class Y>
    Polynomial operator*(const Polynomial<Y>& rhs) {
        Polynomial lhs(*this);
        lhs *= rhs;
        return lhs;
    }

    // <--- HELPFUL FUNCTIONS --->

    template <class Y>
    Polynomial operator()(const Y& value, size_t index = 0) const {
        if (index >= dimension_) {
            throw std::out_of_range("This polynomial has: " + std::to_string(dimension_) + " dimensions");
        }
        Polynomial result;
        for (auto& [key_, value_] : coefficients_) {
            auto new_key = key_;
            size_t pow = key_[index];
            new_key[index] = 0;
            Polynomial now;
            now.dimension_ = new_key.size();
            now.coefficients_[new_key] = value_;
            now *= Pow(value, pow);

            result += now;
        }

        return result;
    }

    template <class Y>
    Matrix<Y> operator()(const Matrix<Y>& value) const {
        if (dimension_ > 1) {
            throw std::logic_error("This method works only for 1-dimension polynomials");
        }
        Matrix<Y> result(value.Size());
        for (auto& [key_, value_] : coefficients_) {
            size_t pow = key_[0];
            result += Pow(value, pow) * value_;
        }

        return result;
    }

    T GetConst() {
        return coefficients_[std::vector<size_t>(dimension_)];
    }

    // <--- INTERFACE FUNCTIONS --->

    [[nodiscard]] size_t GetDimension() const {
        return dimension_;
    }

    [[nodiscard]] const std::map<std::vector<size_t>, T>& GetCoefficients() const {
        return coefficients_;
    }

    friend Polynomial<int> x(size_t index);

    [[nodiscard]] std::string GetPolyRepresentation() const {
        bool first = true;
        std::ostringstream os;
        for (auto& [key, value] : coefficients_) {
            if (first) {
                os << value << converter(key);
            } else {
                if (abs(value) == 1) {
                    if (value < 0) {
                        os << " - " << converter(key);
                    } else {
                        os << " + " << converter(key);
                    }
                } else {
                    if (value < 0) {
                        os << " - " << abs(value) << converter(key);
                    } else {
                        os << " + " << value << converter(key);
                    }
                }
            }
            first = false;
        }
        return os.str();
    }

    void TrashClean() {
        for (auto it = coefficients_.begin(); it != coefficients_.end();) {
            if (it->second == 0) {
                it = coefficients_.erase(it);
            } else {
                ++it;
            }
        }
    }
};

inline Polynomial<int> x(size_t index = 1) {
    Polynomial<int> res;
    res.dimension_ = index;
    std::vector<size_t> needed(index);
    needed.back() = 1;

    res.coefficients_[needed] = 1;
    return res;
}


#endif //POLY_H
