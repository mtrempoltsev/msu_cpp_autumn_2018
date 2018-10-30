class row{
    int *el;
    size_t size;
    public:
    row():el(nullptr),size(0){}
    void connectRow(size_t col){
        el = new int[col];
        size = col;
    }
    int& operator[](size_t j)const{
        if(j < size){
            return el[j];
        }
        throw std::out_of_range("");
    }
    ~row(){
        delete []el;
    }
};

class Matrix{
    size_t quantRows;
    size_t quantCols;
    row *rows;
public:
    
    Matrix(size_t numrows, size_t numcols):quantCols(numcols),quantRows(numrows){
        rows = new row[numrows];
        for(int i = 0; i < numrows; i++){
            rows[i].connectRow(numcols);
        }
    }
    row& operator[](size_t i)const{
        if(i < quantRows)
            return rows[i];
        throw std::out_of_range(""); 
    }
    size_t getColumns()const{
        return quantCols;    
    }
    size_t getRows()const{
        return quantRows;
    }
    ~Matrix(){
        delete []rows;
    }
    Matrix& operator *=(int mn){
        for(int i = 0; i < quantRows; i++)
            for(int j = 0; j < quantCols; j++)
                rows[i][j] *= mn;
	return *this;
    }
    bool operator ==(const Matrix &mat)const{
        if((quantCols != mat.getColumns()) || (quantRows != mat.getRows()))
            return false;
        for(int i = 0; i < quantRows; i++)
            for(int j = 0; j < quantCols; j++)
                if(rows[i][j] != mat[i][j])
                    return false;
        return true;
    }
    bool operator !=(const Matrix &mat)const{
        return !(*this == mat);
    }
};
