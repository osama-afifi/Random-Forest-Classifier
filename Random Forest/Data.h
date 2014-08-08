#ifndef __Dataset_h__
#define __Dataset_h__

#include <math.h>
#include <string>
#include <map>
#include <vector>
#include <string.h>
#include <iomanip>

using namespace std;

#define null(type) reinterpret_cast<type*>(0)

class DatasetSplitPair;

class Dataset
{
  public:
    /**
     * Represents a probability map, which is an associative array with the added
     * entropy calculation function.
     */
    class ProbabilityMap : public map<string, double>
    {
      public:
        //Shannon entropy of the probability map in base 10.
        inline double entropy( void ) const
        {
          double entropy = 0.0;
          for (ProbabilityMap::const_iterator iter = this->begin(); iter != this->end(); ++iter )
            if ( iter->second > 0.0 )
              entropy -= iter->second * log10(iter->second) / log10(2.0);
          return entropy;
        }
    };

    typedef map<string, unsigned int> KeyList;
    typedef vector<double> ThresholdVector;

    class IntegerColumn
    {

      private:
        unsigned int rows;  
        unsigned int *data;

      public:

        IntegerColumn( const unsigned int rows ) : rows(rows)
        {
          data = new unsigned int[rows];
          memset( data, 0, sizeof(*data)*rows );
        }

        ~IntegerColumn()
        {
          delete [] data;
        }

        void resize( const unsigned int new_rows )
        {
          // Size changes?
          if ( new_rows != rows )
          {
            unsigned int *new_data = new unsigned int[new_rows];
            unsigned int copy_size = (rows < new_rows) ? rows : new_rows;
            memcpy( new_data, data, sizeof(*data)*copy_size );

            if ( new_rows > rows )
              memset( new_data + rows, 0, sizeof(*new_data)*(new_rows - rows) );           

            delete [] data;
            data = new_data;
            rows = new_rows;
          }
        }

        bool contains( const unsigned int value ) const
        {
          for ( unsigned int row = 0; row < rows; ++row )
            if ( data[row] == value )
              return true;
          return false;
        }

        unsigned int row_count() const
        {
          return rows;
        }

        const unsigned int & operator[]( const unsigned int row ) const
        {
          return data[row];
        }
        unsigned int & operator[]( const unsigned int row )
        {
          return data[row];
        }

    };

    class RealMatrix
    {
		
      private:
        unsigned int rows;    
        unsigned int columns; 
        double * data;       

      public:

        RealMatrix( const unsigned int rows, const unsigned int columns ) : rows(rows), columns(columns)
        {
          data = new double[rows*columns];
          memset( data, 0, sizeof(*data)*rows*columns );
        }

        ~RealMatrix()
        {
          delete [] data;
        }

        unsigned int row_count() const
        {
          return rows;
        }

        unsigned int column_count() const
        {
          return columns;
        }

        const double * const operator[]( const unsigned int row ) const
        {
          return &data[row*columns];
        }

        double * const operator[]( const unsigned int row )
        {
          return &data[row*columns];
        }

    };

  public:

    Dataset( const unsigned int rows, const unsigned int columns );
    Dataset( Dataset & reference, const unsigned int rows );
    ~Dataset();

    /**
     * Enumerates the dataset values into two categories split on the given
     * threshold.
     * @param column The column to evaluate.
     * @param threshold The threshold to split on (<= threshold; > threshold).
     * @return A probability map containing the elements 'LE' for the elements
     *      less than or equal to the threshold and 'G' for the elements greater
     *      than the threshold.
     */
    ProbabilityMap enumerate_threshold(const unsigned int column, const double threshold ) const;

    /**
     * Returns a list of all thresholds to check.
     * @param column Column to evaluate.
     * @return A list of all thresholds.
     */
    ThresholdVector get_thresholds( const unsigned int column ) const;

    /**
     * Split the dataset based on some constraint.
     * @param column The column to split on.
     * @param threshold The value to split on (<= threshold; > threshold).
     * @return An array of two datasets, the first with data <= to the threshold,
     *      the second with data > threshold.
     */
    DatasetSplitPair split(const unsigned int column, const double threshold );

    /**
     * Determines the information gain of a specific column with respect to a
     * given decision column.
     * @param [in] decision_column The decision (classification) column.
     * @param [in] attribute_column The attribute column to determine information
     *      gain on.
     * @param [out] threshold The optimal splitting threshold.
     * @return The information gain of splitting on the given column.
     */
    double information_gain(
      const unsigned int decision_column,
      const unsigned int attribute_column,
      double & threshold );

    /**
     * Generate a bootstrap sample from the dataset (sampling with replacement).
     * @param sample_size The size of the boostrap sample.
     * @return Bootstrap sample.
     */
    Dataset bootstrap_sample( const unsigned int sample_size );

    /**
     * Get the out of bag set. This requires a dataset reference.
     */
    Dataset out_of_bag_set( void );

    /**
     * Return data matrix.
     * @return Data matrix.
     */
    const RealMatrix & data_matrix( void ) const
    {
      return *data;
    }
    RealMatrix & data_matrix( void )
    {
      return *data;
    }

    /**
     * Row accessor.
     * @param row Matrix row.
     * @return Pointer to matrix row.
     */
    const double * const operator[]( const unsigned int row ) const
    {
      unsigned int data_index = data_ref[row];
      return (*data)[data_index];
    }
    double * operator[]( const unsigned int row )
    {
      unsigned int data_index = data_ref[row];
      return (*data)[data_index];
    }

    /**
     * Determines the number of rows in reference dataset.
     * @return Number of rows.
     */
    unsigned int row_count( void ) const
    {
      return data_ref.row_count();
    }

    /**
     * Resize the reference set.
     * @param new_rows New row count.
     * @note This does not affect the actual dataset, just the reference set.
     */
    void resize( const unsigned int rows )
    {
      data_ref.resize( rows );
    }

    /**
     * Returns the data reference array.
     * @return Data reference.
     */
    const IntegerColumn & data_reference( void ) const
    {
      return data_ref;
    }
    IntegerColumn & data_reference( void )
    {
      return data_ref;
    }

    /**
     * Dataset index accessor.
     * @param row Column row.
     * @return Reference to index element.
     */
    const unsigned int & operator()( const unsigned int row ) const
    {
      return data_ref[row];
    }
    unsigned int & operator()( const unsigned int row )
    {
      return data_ref[row];
    }

    /**
     * Key accessor.
     * @return Reference to keys.
     */
    const KeyList & get_keys( void ) const
    {
      return keys;
    }
    KeyList & get_keys( void )
    {
      return keys;
    }

  private:
    KeyList                   keys;     ///< Dataset key list.
    IntegerColumn             data_ref; ///< Dataset reference indices.
    RealMatrix * const        data;     ///< Dataset data.
    const bool                usingRef; ///< Indicates whether or not using reference dataset.

  public:
    // For unit testing...
    friend class ut_Dataset;
};



/**
 * Defines a pair of datasets generated after a split.
 */
class DatasetSplitPair
{
  public:
    /**
     * Constructor.
     * @param max_rows Maximum number of rows the datasets will hold.
     */
    DatasetSplitPair( Dataset & ref_dataset, const unsigned int max_rows ) :
      ds_le( ref_dataset, max_rows ),
      ds_g( ref_dataset, max_rows )
    {
      //
    }

    Dataset ds_le;  ///< Dataset with elements <= split threshold.
    Dataset ds_g;   ///< Dataset with elements > split threshold.
};

/**
 * Define stream output.
 */
std::ostream & operator<<( std::ostream & stream, const Dataset::IntegerColumn & column );
std::ostream & operator<<( std::ostream & stream, const Dataset::RealMatrix & matrix );

#endif