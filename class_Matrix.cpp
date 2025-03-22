#include "class_Matrix.h"

template <typename T>
Matrix<T>::Matrix() = default;

template <typename T>
Matrix<T>::Matrix(const UnsignedType& r, const UnsignedType& c)
{
	m_matrix.resize(r);
	for (UnsignedType i = 0; i < r; i++)
	{
		m_matrix[i].resize(c);
	}
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::vector<T>> values) :
	m_matrix(values) {}

template <typename T>
const std::vector<T>& Matrix<T>::operator[] (const UnsignedType& i) const
{
	return m_matrix.at(i);
}

template <typename T>
std::vector<T>& Matrix<T>::operator[] (const UnsignedType& i)
{
	return m_matrix.at(i);
}

template <typename T>
UnsignedType Matrix<T>::sizeRows() const
{
	return m_matrix.size();
}

template <typename T>
UnsignedType Matrix<T>::sizeColumns() const
{
	return m_matrix[0].size();
}

template <typename T>
void Matrix<T>::eraseRowMatrix(const UnsignedType& i)
{
	m_matrix.erase(m_matrix.begin() + i);
}

template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;