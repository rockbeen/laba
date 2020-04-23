#pragma once
#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

template <size_t DIM, typename T> //size_t написали, чтобы не указывать все время тип переменной DIM
class vec{};

typedef vec<3, float> Vec3f;// Vec3f - это псевдоним для vec<3, float>

template <typename T>
struct vec<3, T> {//определение  Vec3f
	vec() : x(T()), y(T()), z(T()) {}
	vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
	T& operator[](const size_t i) { assert(i < 3); return i <= 0 ? x : (1 == i ? y : z); }
	const T& operator[](const size_t i) const { assert(i < 3); return i <= 0 ? x : (1 == i ? y : z); }
	float norm() { return std::sqrt(x * x + y * y + z * z); }
	vec<3, T>& normalize(T l = 1) { *this = (*this) * (l / norm()); return *this; }
	T x, y, z;
};

template<size_t DIM, typename T>
T operator*(const vec<DIM, T>& lhs, const vec<DIM, T>& rhs)
{
	T ret = T();
	for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]);
	return ret;
}

template<size_t DIM, typename T>
vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T>& rhs)
{
	for (size_t i = DIM; i--; lhs[i] += rhs[i]);
	return lhs;
}

template<size_t DIM, typename T>vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T>& rhs) {
	for (size_t i = DIM; i--; lhs[i] -= rhs[i]);
	return lhs;
}

template<size_t DIM, typename T, typename U> vec<DIM, T> operator*(const vec<DIM, T>& lhs, const U& rhs) {
	vec<DIM, T> ret;
	for (size_t i = DIM; i--; ret[i] = lhs[i] * rhs);
	return ret;
}
#endif //__GEOMETRY_H__
