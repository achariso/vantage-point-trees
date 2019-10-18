#include <cstddef>
#include <cmath>
#include "gtest/gtest.h"
extern "C" {
    #include "conf.h"
    #include "utils.h"
    #include <sodium.h>
}

double array[9];
const uint32_t N = sizeof( array ) / sizeof( double );

/* array of indices (to keep track of VP's original index */
uint32_t* DATASET_INDEX;

class QuickSelectTest : public ::testing::Test {

protected:

    /// \brief Tears up each test in fixture
    void SetUp() override
    {
//        for (double &j : array)
//        {
//            j = randombytes_uniform( DATASET_N ) / (double) DATASET_N;
//            j *= randombytes_uniform( DATASET_N ) / (double) DATASET_N;
//            j /= randombytes_uniform( DATASET_N ) / (double) DATASET_N;
//            j += randombytes_uniform( DATASET_N ) / (double) DATASET_N;
//            j -= randombytes_uniform( DATASET_N ) / (double) DATASET_N;
//        }

        for ( uint32_t i = 1; i <= N; i++ )
        {
            array[i] = 10 - i;
//            fprintf( stdout, "array[%u] = %lf\n", i, array[i] );
        }
    }

public:

    /// \brief Tears down entire test suite (fixture)
    void TearDown( ) override
    {

    }

};


//------------------------------------------------------------------------------------------------


/// \brief Tests quickSelect()
TEST_F(QuickSelectTest, QuickSelect)
{
    vptree vpTree;
    vpTree.dataset_left = 0;
    vpTree.dataset_right = N - 1;
    vpTree.dataset_rows = N;
    vpTree.dataset_dims = 1;
    vpTree.dataset = NULL;
    vpTree.inner = NULL;
    vpTree.outer = NULL;

    vpTree.vp = (double *) malloc( sizeof( double ) );
    vpTree.vp[0] = 9.0;

    vpTree.md = vpMedianDistanceQuickSelect( &vpTree, array );

    fprintf( stdout, "\n" );
    inspect_vpt( &vpTree, 0 );

    // check index
    EXPECT_EQ( vpTree.md_index,4 );
    EXPECT_EQ( vpTree.md,5);

    // check value
    uint32_t i, countLT, countGT;
    //  - check all lower (or equal)
    for ( i = 0, countLT = 0; i < vpTree.md_index; i++, countLT++ )
    {
        EXPECT_LT( array[i], vpTree.md );
    }
    //  - check all higher (or equal)
    for ( i = vpTree.md_index + 1, countGT = 0; i < N; i++, countGT++ )
    {
        EXPECT_GT( array[i], vpTree.md );
    }
    //  - check counts (supp. that no duplicate elements exist)
    EXPECT_EQ(countLT, countGT );
}