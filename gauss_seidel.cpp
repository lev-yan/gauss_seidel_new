#include<cassert>
#include<limits>  //for numeric_limits
#include<cstddef> //std::size_t
#include<fstream>
#include<iostream> 
#include<iomanip> //for setw()

typedef float elementType;

class Matrix
{
    private:
	size_t mColCount;
	size_t mRowCount;
	elementType * mData;
    public:
	void initialize(const std::size_t r, const std::size_t c)
	{
		mRowCount = r;
		mColCount = c;
		mData = new elementType[r*c];
	}
	void set_element(const std::size_t i, const std::size_t j, const elementType e)
	{
		mData[i* mColCount + j] = e;
	}
	elementType & get_element(const std::size_t i, const std::size_t j) const
	{
		return mData[i * mColCount + j];
	}
	size_t get_col() const
	{
		return mColCount;
	}
	size_t get_row() const
	{
		return mRowCount;
	}
};

void read_matrix(Matrix & matrix, std::ifstream & like_pipe)
{
	assert( like_pipe.is_open() );
	std::size_t r= std::numeric_limits<std::size_t>::max();
	like_pipe >> r;
	assert(r < std::numeric_limits<elementType>::max() );
	assert(r > 0);
	assert( like_pipe.is_open() );
	std::size_t c = std::numeric_limits<std::size_t>::max();
	like_pipe >> c;
	assert(c < std::numeric_limits<elementType>::max() );
	assert(c > 0);
	elementType x = std::numeric_limits<elementType>::max();
	matrix.initialize(r,c);
	for (std::size_t i = 0; i < r; ++i){
		for (std::size_t j=0; j< c; ++j){
			like_pipe>> x; //OK version
			assert( x < std::numeric_limits<elementType>::max());
			matrix.set_element(i,j,x);
		}
	}
}

void write_matrix(const Matrix & matrix , std::ofstream & like_pipe)
{
	assert(like_pipe.good());
	like_pipe<< matrix.get_row() << " ";
	assert(like_pipe.good());
	like_pipe<< matrix.get_col() << "\n";
	for(std::size_t i = 0; i < matrix.get_row(); ++i){
		for( std::size_t j = 0; j< matrix.get_col(); ++j){
			like_pipe<< std::setw(10)
				 << matrix.get_element(i,j)<< " "; 
		}
		like_pipe << std::endl;
	}
}
 
elementType my_round(elementType varm)
{
	elementType value = (int)(varm * 100000);
	return (elementType)value/100000;
}

elementType my_abs(elementType & var)
{
	if( var > 0) 
       	{ return var;
	}else{
	return  (-1)* var;
	}
}
void show_matrix(Matrix & x) //if you need to watch calculation step by step
{
	std::cout << "+++++++++++++++++++++++++++++++++++++++++++TEST++++++\n";
	for (std::size_t i = 0; i < x.get_row(); ++i){
		for (std::size_t j = 0; j < x.get_col(); ++j){
		       	std::cout << std::setw(9) << x.get_element(i,j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void show_vector(Matrix & x) //if you need to watch calculation vector step by step
{
	std::cout << "+++++++++++++++++++++++++++++++++++++++++++STEP++++++\n";
	for (std::size_t i = 0; i < x.get_row(); ++i){
		for (std::size_t j = 0; j < x.get_col(); ++j){
		       	std::cout << std::setw(9) << x.get_element(i,j) << "   ";
		}
	}
	std::cout << std::endl;
}


void augmented_matrix(const Matrix & a, const Matrix & b, Matrix & ab)
{
	std::size_t row = a.get_row();
	std::size_t col = a.get_col();
	for (std::size_t i = 0; i < row; ++i){
		for (std::size_t j = 0; j < col; ++j){
			elementType temp_a = a.get_element(i,j);
			ab.set_element(i,j, temp_a);
			temp_a = b.get_element(i,0);
			ab.set_element(i, j+1, temp_a);
		}
	}
}

void iteration_step(Matrix & ab, Matrix & x_prev, Matrix & x_next)
{
	std::size_t row = ab.get_row();
	std::size_t col = ab.get_col();
	std::size_t single_col = 0;
	for (std::size_t i = 0; i < row; ++i){	
		elementType  element = 0;
		for (std::size_t j = 0; j < col-1; ++j){
			if (i != j) {
					element += x_prev.get_element(j, single_col) * ab.get_element(i, j);
			        }			
		}
		element = ab.get_element(i,col-1) - element; 
		element = element/ab.get_element(i,single_col+i);
		element = my_round(element);
		x_prev.set_element(i, single_col, element); //?
	}
	x_next = x_prev;		
}

void gauss_seidel_method(const  Matrix & a,
		  const  Matrix & b,
			 Matrix &x)
{
	Matrix ab;
	Matrix x_prev;
	Matrix x_new;
	ab.initialize(a.get_row(), a.get_col() +1);
	x_new.initialize(a.get_row(), 1);
	x_prev.initialize(a.get_row(), 1);
	elementType  precision = 0.000001;
	elementType  temp = 0;
	augmented_matrix(a,b,ab);   
	show_matrix(ab);  	   

	for (std::size_t i = 0; i < 10; ++i){
		iteration_step(ab, x_prev, x);
		show_vector(x);
	}

	std::cout << "\ngauss-seidel method ... DONE..."<< std::endl;
	std::cout << "You can find solution of equation into x.txt file"<< std::endl;
}

int main()
{
	Matrix a;
	Matrix b;
	Matrix x;
	std::ifstream A("A.txt"), B("B.txt");
	std::ofstream X("x.txt");
	read_matrix(a,A); 
	read_matrix(b,B);
	x.initialize(a.get_row(),1); 
	gauss_seidel_method( a, b, x);
	write_matrix(x, X);
	return 0;
}
