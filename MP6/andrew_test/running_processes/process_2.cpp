/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP6/cpu_bound_processes/cpu_bound_2.cpp                             */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */


#include<iostream>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<vector>
#include<time.h>
#include<climits>


class Matrix
{
public:
    /* Constructor */
    Matrix(int num_rows, int num_columns);

    /* Random Utility Functions */
    void resize_matrix(int new_num_rows, int new_num_columns);
    void random_populate();

    /* Operations */
    static Matrix multiply(Matrix m1, Matrix m2);

    /* Data Getter Functions */
    int get_num_rows();
    int get_num_columns();
    int get_element(int row_pos, int column_pos);

    /* Data Setter Functions */
    void set_element(int row_pos, int column_pos, int value);

private:
    std::vector<std::vector<int> > matrix_elements;
    int m_num_rows;
    int m_num_columns;
};

Matrix::Matrix(int num_rows, int num_columns)
{
    m_num_rows = num_rows;
    m_num_columns = num_columns;

    for(unsigned int i=0; i<num_rows; i++)
    {
        matrix_elements.push_back(std::vector<int>());

        for(unsigned int j=0; j<num_columns; j++)
        {
            matrix_elements[i].push_back(0);
        }
    }
}

void Matrix::resize_matrix(int new_num_rows, int new_num_columns)
{
    // Can't reduce the size of a matrix to be less than 0
    if(new_num_rows < 0 || new_num_columns < 0)
    {
        return;
    }

    // Deal with changed rows
    if(new_num_rows > matrix_elements.size())
    {
        int new_rows = new_num_rows - m_num_rows;
        for(unsigned int i=0; i<new_rows; i++)
        {
            matrix_elements.push_back(std::vector<int>());

            for(int j=0; j<m_num_columns; j++)
            {
                matrix_elements[m_num_rows + 1 + i].push_back(0);
            }
        }
    }
    else if(new_num_rows < matrix_elements.size())
    {
        while(new_num_rows < matrix_elements.size())
        {
            matrix_elements.pop_back();      
        }
    }

    // Deal with changed columns
    if(new_num_columns > m_num_columns)
    {
        int column_difference = new_num_columns - m_num_columns;
        for(unsigned short int i=0; i<matrix_elements.size(); i++)
        {
            for(unsigned short int j=0; j<column_difference; j++)
            {
                matrix_elements[i].push_back(0);
            }
        }
    }
    else if(new_num_columns < m_num_columns)
    {
        int column_difference = new_num_columns - m_num_columns;
        for(unsigned short int i=0; i<matrix_elements.size(); i++)
        {
            for(unsigned short int j=0; j<column_difference; j++)
            {
                matrix_elements[i].pop_back();
            }
        }
    }

    // Final assignments
    m_num_rows = new_num_rows;
    m_num_columns = new_num_columns;
}


void Matrix::random_populate()
{
    for(unsigned int i=0; i<matrix_elements.size(); i++)
    {
        for(unsigned int j=0; j<matrix_elements[i].size(); j++)
        {
            matrix_elements[i][j] = rand() % INT_MAX;
        }
    }
}


Matrix Matrix::multiply(Matrix m1, Matrix m2)
{
    /* Matrix element to hold the result matrix */
    int m3_rows = m1.get_num_rows();
    int m3_columns = m2.get_num_columns();
    Matrix m3(m3_rows, m3_columns);

    /* Check to see whether or not the two matries can even be multiplied */
    if(m1.get_num_columns() != m2.get_num_rows())
    {
        return Matrix(0, 0);
    }

    for(unsigned int i=0; i<m1.get_num_rows(); i++)
    {
        for(unsigned int j=0; j<m2.get_num_columns(); j++)
        {
            for(unsigned int k=0; k<m1.get_num_rows(); k++)
            {
                int old_value_m1 = m1.get_element(i, k);
                int old_value_m2 = m2.get_element(k, j);
                int old_value_m3 = m3.get_element(i, j);
                int result = (old_value_m1 * old_value_m2) + old_value_m3;

                m3.set_element(i, j, result);
            }
        }
    }

    return m3;
}


/* Data Getter Functions */
int Matrix::get_num_rows()
{
    return m_num_rows;
}


int Matrix::get_num_columns()
{
    return m_num_columns;
}

int Matrix::get_element(int row_pos, int column_pos)
{
    return matrix_elements[row_pos][column_pos];
}


/* Data Setter Functions */
void Matrix::set_element(int row_pos, int column_pos, int value)
{
    matrix_elements[row_pos][column_pos] = value;
}


int main()
{
    kill(getpid(), SIGSTOP);
    
    Matrix m1(700, 700);
    Matrix m2(700, 700);
    m1.random_populate();
    m2.random_populate();
    
    Matrix::multiply(m1, m2);

    exit(0);
}
