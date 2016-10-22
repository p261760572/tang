#include "ibdcs.h"

int dcs_load_config(IBDCS_CFG *pIbdcsCfg)
{
    // this function load configuration from file
    // "$FEP_RUNPATH/config/ibcds.conf",and placed
    // all information into the buffer given by caller

    char cfgfile[256];
    int  fd, ret;

    if(u_fabricatefile("config/ibdcs.conf",cfgfile,sizeof(cfgfile)) < 0)
    {
        dcs_log(0,0,"cannot get full path name of 'ibdcs.conf'\n");
        return -1;
    }

    if(0 > (fd = conf_open(cfgfile)) )
    {
        dcs_log(0,0,"open file '%s' failed.\n",cfgfile);
        return -1;
    }

    memset(pIbdcsCfg,0,sizeof(IBDCS_CFG));

    //read items with respect to TCP connection
 
    //stuffes of SHM
    ret=conf_get_first_number(fd,"Shm_Timeout_Rows",&pIbdcsCfg->iTbTRows);
    if(ret < 0)
    {
        dcs_log(0,0,"loading config item 'Shm_Timeout_Rows' failed.\n");
        goto lblFailure;
    }

    //stuffes of timeout control
    ret=conf_get_first_number(fd,"T1",&pIbdcsCfg->iT1);
    if(ret < 0)
    {
        dcs_log(0,0,"loading config item 'T1' failed.\n");
        goto lblFailure;
    }
    ret=conf_get_first_number(fd,"T2",&pIbdcsCfg->iT2);
    if(ret < 0)
    {
        dcs_log(0,0,"loading config item 'T2' failed.\n");
        goto lblFailure;
    }
    ret=conf_get_first_number(fd,"T3",&pIbdcsCfg->iT3);
    if(ret < 0)
    {
        dcs_log(0,0,"loading config item 'T3' failed.\n");
        goto lblFailure;
    }


    //others
    ret=conf_get_first_number(fd,"RunMode",&pIbdcsCfg->iRunMode);
    if(ret < 0)
    {
        dcs_log(0,0,"loading config item 'RunMode' failed.\n");
        goto lblFailure;
    }

    conf_close(fd);
    return 0;

lblFailure:
    conf_close(fd);
    return -1;
}

int dcs_dump_sysconf(FILE *fpOut)
{
    if(fpOut==NULL)
        fpOut = stdout;

    if(g_pIbdcsCfg == NULL)
    {
        fprintf(fpOut,"not attach to shared memory of IBDCS!\n");
        return -1;
    }

    /*共享内存中超时表的记录数*/
    fprintf(fpOut,"RowsInTimeoutTable = %d\n",g_pIbdcsCfg->iTbTRows);

    /*超时时间*/
    fprintf(fpOut,"                T1 = %d\n" ,g_pIbdcsCfg->iT1);
    fprintf(fpOut,"                T2 = %d\n" ,g_pIbdcsCfg->iT2);
    fprintf(fpOut,"                T3 = %d\n",g_pIbdcsCfg->iT3);
    fprintf(fpOut,"       RunningMode = %d\n",g_pIbdcsCfg->iRunMode);
    fflush(fpOut);
    return 0;
}

int dcs_set_sysconf(int which,  int val)
{
    switch(which)
    {
        case TIMEOUT1:
            g_pIbdcsCfg->iT1 = val;break;
        case TIMEOUT2:
            g_pIbdcsCfg->iT2 = val;break;
        case TIMEOUT3:
            g_pIbdcsCfg->iT3 = val;break;
        case RUN_MODE:
            g_pIbdcsCfg->iRunMode = val;break;
        default:
            dcs_log(0,0,"dcs_set_sysconf() encounter illegal"
                    " config item %d.\n",which);
            return -1;
    }//switch

    return 0;
}
