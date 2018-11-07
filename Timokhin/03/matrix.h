#pragma once

#include <cstdint>
#include <stdexcept>

class Matrix
{
public:
    Matrix(uint64_t rowsN, uint64_t colsN) :
            m_rowsN(rowsN), m_colsN(colsN), m_data(new int[rowsN * colsN])
    {}

    uint64_t getRows() const
    {
        return m_rowsN;
    }

    uint64_t getColumns() const
    {
        return m_colsN;
    }

    Matrix& operator *=(int a)
    {
        for (uint64_t i = 0; i < m_rowsN * m_colsN; ++i)
            m_data[i] *= a;
        return *this;
    }

    bool operator==(const Matrix& m) const
    {
        bool equal = (m.m_colsN == m_colsN && m.m_rowsN == m_rowsN);
        for (uint64_t i = 0; i < m_rowsN * m_colsN && equal; ++i)
            equal = equal && (m_data[i] == m.m_data[i]);
        return equal;
    }

    bool operator!=(const Matrix& m) const
    {
        return !operator==(m);
    }

    class Indexer
    {
    public:
        Indexer(int* data, uint64_t offset, uint64_t range) :
                m_data(data), m_offset(offset), m_range(range)
        {}

        int& operator[](uint64_t i)
        {
            if (i < m_range)
                return m_data[m_offset + i];
            else
                throw std::out_of_range("Matrix: wrong column");
        }

        int operator[](uint64_t i) const
        {
            if (i < m_range)
                return m_data[m_offset + i];
            else
                throw std::out_of_range("Matrix: wrong column");
        }

    private:
        int* m_data;
        const uint64_t m_offset;
        const uint64_t m_range;
    };

    Indexer operator[](uint64_t i)
    {
        if (i < m_rowsN)
            return Indexer(m_data, i * m_colsN, m_colsN);
        else
            throw std::out_of_range("Matrix: wrong row");
    }

    const Indexer operator[](uint64_t i) const
    {
        if (i < m_rowsN)
            return Indexer(m_data, i * m_colsN, m_colsN);
        else
            throw std::out_of_range("Matrix: wrong row");
    }

    ~Matrix()
    {
        delete[] m_data;
    }

private:
    const uint64_t m_rowsN;
    const uint64_t m_colsN;
    int* m_data;
};

