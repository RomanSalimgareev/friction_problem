#include "class_matrix.h"
#include "error_handling.h"

using namespace MFE;

std::string messageMatrixEmpty()
{
	return "The matrix is empty. ";
}

std::string messageInvalidArgument()
{
	return "Invalid argument. ";
}

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
void Matrix<T>::eraseRow(const UnsignedType& index)
{
	if (m_matrix.empty())
		ERROR(messageMatrixEmpty());
	else
	{
		if (index >= m_matrix.size())
			ERROR(messageInvalidArgument());
		else
			m_matrix.erase(m_matrix.begin() + index);
	}

}

template <typename T>
void Matrix<T>::eraseColumn(const UnsignedType& index)
{
	if (m_matrix.empty())
		ERROR(messageMatrixEmpty());
	else
	{
		for (UnsignedType rowNum = 0; rowNum < m_matrix.size(); ++rowNum)
		{
			if (index >= m_matrix[rowNum].size())
				ERROR(messageInvalidArgument());
			else
				m_matrix[rowNum].erase(m_matrix[rowNum].begin() + index);
		}
	}
}

template <typename T>
bool Matrix<T>::empty() const
{
	return m_matrix.empty();
}

template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;