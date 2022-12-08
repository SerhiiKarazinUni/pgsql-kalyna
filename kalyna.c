#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "../kalyna/main.c"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(kalyna_encrypt);

Datum
kalyna_encrypt(PG_FUNCTION_ARGS)
{
    bool isnull, isnull2;

    isnull = PG_ARGISNULL(0);
    isnull2 = PG_ARGISNULL(1);
    if (isnull || isnull2)
      ereport( ERROR,
               ( errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
               errmsg("two and only two text values are required as input")));

    text* message = PG_GETARG_TEXT_PP(0);
    text* key = PG_GETARG_TEXT_PP(1);
    
    kalyna_t* ctx22_e = KalynaInit(128, 128);
    
    int num_blocks = ceil((VARSIZE_ANY_EXHDR(message)/(float)sizeof(uint64_t)/2.0f));
    
    uint8_t* ciphertext = malloc(num_blocks*sizeof(uint64_t)*2);
    
    int code = kencrypt(message->vl_dat, VARSIZE_ANY_EXHDR(message), key->vl_dat, VARSIZE_ANY_EXHDR(key), ctx22_e, ciphertext);
    
    if(code>0){
        char* reason = "unknown error";
        switch(code){
            case 1:
                reason = "incorrect key size";
                break;
        }
        ereport( ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED), errmsg(reason)));
    }
    
    bytea* result;
    result = palloc(num_blocks*sizeof(uint64_t)*2 + VARHDRSZ);
    SET_VARSIZE(result, num_blocks*sizeof(uint64_t)*2 + VARHDRSZ);
    memcpy(VARDATA(result), ciphertext, num_blocks*sizeof(uint64_t)*2);
    free(ciphertext);
    PG_RETURN_BYTEA_P(result);

    KalynaDelete(ctx22_e);
}
