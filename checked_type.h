
#ifndef CHECKEDTP_CHECKED_TYPE_H
#define CHECKEDTP_CHECKED_TYPE_H

#include <limits>
#include <type_traits>
#include <stdexcept>

struct arithm_error : public std::logic_error {
    arithm_error() : d("Arithmetic error"), std::logic_error("Arithmetic error") {};
    arithm_error(const char* d) : d(d), std::logic_error(d) {};

private:
    std::string d;
};

// forward-declaration for safe template friending
template<class T, class limits = std::numeric_limits<T>>
struct checked_type;

template<class T, class limits = std::numeric_limits<T>>
checked_type<T, limits> operator+(const checked_type<T,limits>& lh, const checked_type<T,limits>& rh) {
    if (((rh.get() > 0) && (lh.get() > (limits::max() - rh.get()))) ||
        ((rh.get() < 0) && (lh.get() < (limits::min() - rh.get())))) {
        throw arithm_error();
    }
    return checked_type<T, limits>(lh.get() + rh.get());
};

template<class T, class limits = std::numeric_limits<T>>
checked_type<T, limits> operator-(const checked_type<T,limits>& lh, const checked_type<T,limits>& rh) {
    if ((rh.get() > 0 && lh.get() < limits::max() + rh.get()) ||
        (rh.get() < 0 && lh.get() > limits::min() + rh.get())) {
        throw arithm_error();
    }

    return checked_type<T, limits>(lh.get() - rh.get());
};

template<class T, class limits = std::numeric_limits<T>>
checked_type<T, limits> operator*(const checked_type<T, limits>& lh, const checked_type<T, limits>& rh) {
    if (lh.get() > 0) {
        if (rh.get() > 0) {
            if (lh.get() > (limits::max() / rh.get())) {
                throw arithm_error();
            }
        } else {
            if (rh.get() < (limits::min() / lh.get())) {
                throw arithm_error();
            }
        }
    } else {
        if (rh.get() > 0) {
            if (lh.get() < (limits::min() / rh.get())) {
                throw arithm_error();
            }
        } else {
            if ((lh.get() != 0) && (rh.get() < (limits::max() / lh.get()))) {
                throw arithm_error();
            }
        }
    }

    return checked_type<T, limits>(lh.get() * rh.get());
};

template<class T, class limits>
checked_type<T, limits> operator/(const checked_type<T,limits>& lh, const checked_type<T,limits>& rh) {
    if ((rh.get() == 0) || ((lh.get() == limits::min()) && (rh.get() == -1))) {
        throw arithm_error();
    }
    return checked_type<T, limits>(lh.get() / rh.get());
};

template<class T, class limits>
checked_type<T, limits> operator-(const checked_type<T,limits>& lh) {
    if (lh.get() == limits::min()) {
        throw arithm_error();
    }
    return checked_type<T, limits>(-lh.get());
};

template<class T, class limits>
struct checked_type {
    checked_type(T value) : value(value) {}
    checked_type() : value(T()) {}

    // safe friends, specialized over T : each checked_type<T, limits> befriends only checked_type<T, limits>-operators
    friend checked_type operator+ <> (const checked_type& lh, const checked_type& rh);
    friend checked_type operator- <> (const checked_type& lh, const checked_type& rh);
    friend checked_type operator* <> (const checked_type& lh, const checked_type& rh);
    friend checked_type operator/ <> (const checked_type& lh, const checked_type& rh);
    friend checked_type operator- <> (const checked_type& lh);

    const T& get() const {return value;};

    // unsure about typecast return type, supposedly T, not T&
    operator T() const {
        return value;
    }

private:
    T value;
};


#endif //CHECKEDTP_CHECKED_TYPE_H