class row{
    int *el;
    size_t size;
    public:
    row():el(nullptr),size(0){}
    void connectRow(int *arr,size_t col){
        el = arr;
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
    int **matr;
public:
    
    Matrix(size_t numrows, size_t numcols):quantCols(numcols),quantRows(numrows){
        rows = new row[numrows];
        matr = new int*[numrows];
        for(int i = 0; i < numrows; i++){
            matr[i] = new int[numcols];
            rows[i].connectRow(matr[i], numcols);
        }
    }
    const row& operator[](size_t i)const{
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
        delete []matr;
    }
    void operator *=(int mn){
        for(int i = 0; i < quantRows; i++)
            for(int j = 0; j < quantCols; j++)
                matr[i][j] *= mn;
    }
    bool operator ==(const Matrix &mat)const{
        if((quantCols != mat.getColumns()) || (quantRows != mat.getRows()))
            return false;
        for(int i = 0; i < quantRows; i++)
            for(int j = 0; j < quantCols; j++)
                if(matr[i][j] != mat[i][j])
                    return false;
        return true;
    }
    bool operator !=(const Matrix &mat)const{
        if((quantCols != mat.getColumns()) || (quantRows != mat.getRows()))
            return true;
        for(int i = 0; i < quantRows; i++)
            for(int j = 0; j < quantCols; j++)
                if(matr[i][j] != mat[i][j])
                    return true;
        return false;
    }
};
