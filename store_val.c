#include "stplugin.h"

STDLL stata_call(int argc, char *argv[])
{
	int i ;

	// ST_int var_num obs_num;
	// ST_double obs_val;

	ST_int var_num = 1;
	ST_int obs_num = 1;
	ST_double obs_val = 1234.0;

	SF_vstore(var_num, obs_num, obs_val);

	// for(i=0;i < argc; i++) {
	// 	SF_display(argv[i]) ;
	// 	SF_display("\n") ;
	// }
	return(0) ;
}
