class Matrix {
    const size_t columns;
    const size_t rows;
    int *mass;
    
    class Proxy
    {
    private:
        int *off_mass;
        size_t columns;
        
    public:
        Proxy(int *off_, size_t columns_)
        :off_mass(off_),
        columns(columns_)
        {}
        
        int &operator [] (int i)
        {
            if (i >= columns)
                throw std::out_of_range("number of rows more than it is");
            return off_mass[i];
        }
        
        const int &operator [] (int i) const
        {
            if (i >= columns)
                throw std::out_of_range("number of rows more than it is");
            return off_mass[i];
        }
        
    };
    
public:
    Matrix(size_t count_rows, size_t count_columns) :
    rows(count_rows),
    columns(count_columns),
    mass(new int[rows * columns]())
    {}
    
    ~Matrix() {
        delete []mass;
    }
    
    bool operator == (const Matrix &other) const
    {
        if (this == &other)
            return true;
        
        if (columns != other.columns || rows != other.rows)
            return false;
        
        for (int i = 0; i < columns * rows; ++i)
            if (mass[i] != other.mass[i])
                return false;
        
        return true;
    }
    bool operator != (const Matrix &other) const
    {
        return !(*this == other);
    }
    
    Matrix &operator *= (int factor)
    {
        for (int i = 0; i < columns * rows; ++i)
            mass[i] *= factor;
        return *this;
    }
    
    const size_t getRows(void) const
    {
        return rows;
    }
    
    const size_t getColumns(void) const
    {
        return columns;
    }
    
    const Proxy operator [] (size_t i) const
    {
        if (i >= rows)
            throw std::out_of_range("number of rows more than it is");
        return Proxy(mass + columns * i, columns);
    }
    
    Proxy operator [] (size_t i)
    {
        if (i >= rows)
            throw std::out_of_range("number of rows more than it is");
        return Proxy(mass + columns * i, columns);
    }
    
};
