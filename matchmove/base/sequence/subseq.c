#include "sequence.h"

void subseq_extrinsic_calibrate( SubSeqNode *s, gsl_matrix *k )
{
 subseq_dlt_calib( s, k );
 ba_set_lm_max_iterations( 30 );
 subseq_extrinsic_ba( s, k );
 subseq_recalibrate( s, 10. );
 ba_set_lm_max_iterations( 30 );
 subseq_extrinsic_ba( s, k );
 subseq_recalibrate( s, 5. );
 ba_set_lm_max_iterations( 30 );
 subseq_extrinsic_ba( s, k );
 subseq_recalibrate( s, 3. );
 ba_set_lm_max_iterations( 50 );
 subseq_extrinsic_ba( s, k );
}

