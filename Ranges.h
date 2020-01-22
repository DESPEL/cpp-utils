#pragma once

#include <algorithm>
#include <tuple>
#include <functional>
#include <cstdarg>
#include <iostream>

template <typename T>
struct reversed {
    T& val;
    reversed(T& var) : val(var) {}
    reversed(T&& var) : val(var) {}

    constexpr auto begin() { return val.rbegin(); }

    constexpr auto end() { return val.rend(); }
};

template <typename T>
constexpr auto min(const T val1, const T val2) {
    if (val1 < val2)
        return val1;
    return val2;
}

template <typename T, typename... Ts>
constexpr auto min(const T val1, const Ts... vals) {
    return min(val1, min(vals...));
}

template <class... Ts>
struct zip {
    std::tuple<Ts &...> vals;
    int sz;
    zip(Ts&... containers) : vals(containers...) {
        sz = min(containers.size()...);
    }

    class zipIterator {
    public:
        static std::tuple<typename Ts::value_type &...> getAt(int idx,
            Ts&... args) {
            return { args[idx]... };
        }
        int idx;
        std::tuple<Ts &...>& vals;

        zipIterator(int idx, std::tuple<Ts &...>& v) : idx(idx), vals(v) {}

        bool operator==(const zipIterator& v) { return idx == v.idx; }
        bool operator!=(const zipIterator& v) { return idx != v.idx; }
        void operator++() { idx++; }

        std::tuple<typename Ts::value_type &...> operator*() {
            return std::apply(zipIterator::getAt,
                std::tuple_cat(std::make_tuple(idx), vals));
        }
    };

    struct rzipIterator : public zipIterator {
    public:
        rzipIterator(int idx, std::tuple<Ts &...>& v) : zipIterator(idx, v) {};
        void operator++() { this->idx--; };
    };
    constexpr zipIterator begin() { return zipIterator{ 0, vals }; }
    constexpr zipIterator end() { return zipIterator{ sz, vals }; }

    constexpr rzipIterator rbegin() { return rzipIterator{ sz - 1, vals }; }
    constexpr rzipIterator rend() { return rzipIterator{ -1, vals }; }
};

struct range {
    const int min = 0;
    const int max;

    class rangeIterator {
    public:
        int val;

        void operator++() { val++; }

        constexpr bool operator==(const rangeIterator& v) {
            return val == v.val;
        }
        constexpr bool operator!=(const rangeIterator& v) {
            return val != v.val;
        }

        int operator*() { return val; }
    };

    class rrangeIterator : public rangeIterator {
    public:
        rrangeIterator(const int val) : rangeIterator{ val } {}
        constexpr void operator++() { val--; }
    };

    range(const int min, const int max) : min(min), max(max) {};
    range(const int max) : max(max) {};

    rangeIterator begin() const { return { min }; }
    rangeIterator end() const { return { max }; }

    rrangeIterator rbegin() const { return { max - 1 }; }
    rrangeIterator rend() const { return { min - 1 }; }
};

template <typename T>
struct enumerate {
    T& container;

    enumerate(T& val) : container(val) {};
    enumerate(T&& val) : container(val) {};

    class enumerateIterator {
    public:
        int idx;
        int size;
        T& container;
        void operator++() { idx++; }

        bool operator==(const enumerateIterator& v) { return idx == v.idx; }
        bool operator!=(const enumerateIterator& v) { return idx != v.idx; }

        std::tuple<int, typename T::value_type&> operator*() {
            return { idx, container[idx] };
        }
    };

    class renumerateIterator : public enumerateIterator {
    public:
        renumerateIterator(int idx, int sz, T& container)
            : enumerateIterator{ idx, sz, container } {};
        void operator++() { this->idx--; }
    };

    constexpr enumerateIterator begin() const {
        return { 0, static_cast<int>(container.size()), container };
    }

    constexpr enumerateIterator end() const {
        return { static_cast<int>(container.size()),
                static_cast<int>(container.size()),
                container };
    }

    constexpr renumerateIterator rbegin() const {
        return { static_cast<int>(container.size()) - 1,
                static_cast<int>(container.size())
                , container };
    }

    constexpr renumerateIterator rend() const {
        return { -1, static_cast<int>(container.size()),
                container };
    }
};
//
//template <typename T>
//struct filter {
//    typedef std::function<bool(typename T::value_type&)> disFnc;
//    T& container;
//    disFnc discriminator;
//
//    static T value_type;
//    
//    filter(T&cont, disFnc disc)
//        : container(cont), discriminator(disc) {}
//
//    struct iterator {
//        typename T::iterator it;
//        typename T::iterator end;
//        disFnc pred;
//
//        bool operator == (const iterator &filterIt) {
//            return it == filterIt.it;
//        }
//        bool operator != (const iterator &filterIt) {
//            return it != filterIt.it;
//        }
//
//        void operator++ () { 
//            for (++it; it != end && !pred(*it); ++it);
//        }
//
//        typename T::value_type& operator *() { return *it; }
//    };
//
//    constexpr iterator begin() {
//        return { container.begin(), container.end(), discriminator };
//    }
//
//    constexpr iterator end() {
//        return { container.end(), container.end(), discriminator };
//    }
//};

