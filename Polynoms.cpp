\\This is a realization of a polynoms class. 

#include <iostream>
#include <cstddef>
#include <vector>
#include <cassert>
#include <algorithm>

template <typename T>
class Polynomial {
public:
    using Iterator = typename std::vector<T>::iterator;
    using ConstIterator = typename std::vector<T>::const_iterator;

    explicit Polynomial(const std::vector<T>& coeff) {
        data_ = std::move(coeff);
        EraseZeroes();
    }

    template <typename Iter>
    Polynomial<T>(Iter begin, Iter end) {
        std::copy(begin, end, std::back_inserter(data_));
        EraseZeroes();
    }

    explicit Polynomial(const T& number = static_cast<T>(0)) {
        data_.push_back(number);
        EraseZeroes();
    }

    bool operator == (const Polynomial<T>& other) const {
        return data_ == other.data_;
    }

    bool operator == (const T& other) const {
        if (other == static_cast<T>(0) && data_.empty()) {
            return true;
        }
        return data_ == Polynomial<T>{other}.data_;
    }

    bool operator != (const Polynomial<T>& other) const {
        return !(data_ == other.data_);
    }

    bool operator != (const T& other) const {
        return !(*this == other);
    }

    size_t GetSize() const {
        return data_.size();
    }

    const T operator[] (int i) const {
        if (i > Degree()) {
            return static_cast<T>(0);
        }
        return data_[i];
    }

    std::vector<T>& GetData() {
        return data_;
    }

    int Degree() const {
        if (data_.empty()) {
            return -1;
        }
        return data_.size() - 1;
    }

    ConstIterator begin() const {
        return data_.begin();
    }

    ConstIterator end() const {
        return data_.end();
    }

    void EraseZeroes() {
        while (!data_.empty() && data_.back() == static_cast<T>(0)) {
            data_.pop_back();
        }
    }

    const T GetCoef(size_t i) {
        Polynomial<T> tmp = *this;
        return tmp[i];
    }

    bool IsEmpty() const {
        return data_.empty();
    }

    void resize(size_t cnt) {
        data_.resize(cnt, T(0));
    }

    Polynomial<T>& operator += (const Polynomial<T>& second) {
        Polynomial<T> tmp;
        tmp.GetData().resize(std::max(GetSize(), second.GetSize()));
        size_t cnt = 0;
        for (size_t i = 0; i < std::max(GetSize(), second.GetSize()); ++i) {
            tmp.data_[i] = GetCoef(i) + second[i];
            ++cnt;
        }
        *this = std::move(tmp);
        EraseZeroes();
        return *this;
    }

    Polynomial<T>& operator += (const T& second) {
        *this += Polynomial<T>{second};
        return *this;
    }

    Polynomial<T>& operator -= (const Polynomial<T>& second) {
        Polynomial<T> tmp;
        tmp.GetData().resize(std::max(GetSize(), second.GetSize()));
        size_t cnt = 0;
        for (size_t i = 0; i < std::max(GetSize(), second.GetSize()); ++i) {
            tmp.data_[i] = GetCoef(i) - second[i];
            ++cnt;
        }
        *this = std::move(tmp);
        EraseZeroes();
        return *this;
    }

    Polynomial<T>& operator -= (const T& second) {
        *this -= Polynomial<T>{second};
        return *this;
    }

    Polynomial<T>& operator *= (const Polynomial<T>& second) {
        Polynomial<T> tmp;
        tmp.data_.resize(GetSize() + second.GetSize());
        for (size_t i = 0; i < GetSize(); ++i) {
            for (size_t j = 0; j < second.GetSize(); ++j) {
                tmp.data_[i+j] += GetCoef(i) * second[j];
            }
        }
        *this = std::move(tmp);
        EraseZeroes();
        return *this;
    }

    Polynomial<T>& operator *= (const T& second) {
        *this *= Polynomial<T>{second};
        return *this;
    }

    const T operator () (const T& value) const {
        T ans = T(0);
        for (int i = Degree(); i >= 0; --i) {
            ans *= value;
            ans += T(data_[i]);
        }
        return ans;
    }

    const Polynomial<T> operator & (const Polynomial<T>& other) const {
        Polynomial<T> ans{};
        for (int i = Degree(); i >= 0; --i) {
            ans *= other;
            ans += T(data_[i]);
        }
        return ans;
    }

    const Polynomial<T> operator / (const Polynomial<T>& other) const {
        Polynomial<T> tmp = *this;
        if (Degree() < other.Degree()) {
            return *this;
        } else if (Degree() == 0 && other.Degree() == 0) {
            return Polynomial<T>{tmp.data_[tmp.Degree()] / other.data_[other.Degree()]};
        }
        std::vector<T> ans(Degree() - other.Degree() + 1);
        while (tmp.Degree() >= other.Degree()) {
            auto coeff = tmp.data_[tmp.Degree()] / other.data_[other.Degree()];
            Polynomial<T> divisor{};
            divisor.data_.resize(tmp.Degree() - other.Degree() + 1);
            divisor.data_[divisor.Degree()] = coeff;
            ans[tmp.Degree() - other.Degree()] = coeff;
            tmp -= (other * divisor);
        }
        Polynomial<T> a{ans};
        a.EraseZeroes();
        return a;
    }

    const Polynomial<T> operator % (const Polynomial<T>& other) const {
        Polynomial<T> tmp = *this;
        return tmp - ((tmp / other) * other);
    }

    const Polynomial<T> operator , (const Polynomial<T>& other) const {
        Polynomial<T> a = *this;
        Polynomial<T> b = other;
        while (b != T(0)) {
            auto r = a % b;
            a = b;
            b = r;
        }
        T divisor = a.data_.back();
        for (auto& x : a.data_) {
            x = x / divisor;
        }
        return a;
    }

private:
    std::vector<T> data_;
};

template <typename T>
const Polynomial<T> operator + (const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> tmp = first;
    tmp += second;
    return tmp;
}

template <typename T>
const Polynomial<T> operator + (const Polynomial<T>& first, const T& second) {
    return first + Polynomial<T>{second};
}

template <typename T>
const Polynomial<T> operator - (const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> tmp = first;
    tmp -= second;
    return tmp;
}

template <typename T>
const Polynomial<T> operator - (const Polynomial<T>& first, const T& second) {
    return first - Polynomial<T>{second};
}

template <typename T>
const Polynomial<T> operator * (const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> tmp = first;
    tmp *= second;
    return tmp;
}

template <typename T>
const Polynomial<T> operator * (const Polynomial<T>& first, const T& second) {
    return first * Polynomial<T>{second};
}

template <typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T>& p) {
    if (p.IsEmpty()) {
        out << T(0);
        return out;
    }
    for (int i = p.Degree(); i > 0; --i) {
        if (p[i] == T(0)) {
            continue;
        }
        if (p[i] < T(0)) {
            if (p[i] == T(-1)) {
                out << '-';
            } else {
                out << p[i] << '*';
            }
            if (i != 1) {
                out << "x^" << i;
            } else {
                out << 'x';
            }
        } else {
            if (i != p.Degree()) {
                out << '+';
            }
            if (p[i] != T(1)) {
                out << p[i] << '*';
            }
            if (i != 1) {
                out << "x^" << i;
            } else {
                out << 'x';
            }
        }
    }
    if (p[0] != T(0)) {
        if (p[0] > T(0)) {
            if (p.Degree() != 0) {
                out << '+';
            }
            out << p[0];
        } else {
            out << p[0];
        }
    }
    return out;
}
