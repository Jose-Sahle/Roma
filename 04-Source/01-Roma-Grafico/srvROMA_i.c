/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Mar 29 14:19:56 2004
 */
/* Compiler settings for D:\sahle_jnetto\Desenvolvimento\Visual C++\Roma\01-Roma-Grafico\srvROMA.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_SRVROMALib = {0x6055D5B6,0x5F4C,0x4697,{0x9A,0x21,0x66,0xA8,0xCD,0xEC,0xA2,0x20}};


#ifdef __cplusplus
}
#endif

