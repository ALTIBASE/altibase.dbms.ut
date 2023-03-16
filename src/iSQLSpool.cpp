
/***********************************************************************
 * $Id: iSQLSpool.cpp 10593 2005-02-21 23:47:56Z bethy $
 **********************************************************************/

#include <ida.h>
#include <ideErrorMgr.h>
#include <iSQLProperty.h>
#include <iSQLProgOption.h>
#include <iSQLSpool.h>
#include <iSQLCompiler.h>

extern iSQLProperty    gProperty;
extern iSQLProgOption  gProgOption;
extern iSQLCompiler   *gSQLCompiler;

iSQLSpool::iSQLSpool()
{
    idlOS::memset(m_SpoolFileName, 0x00, sizeof(m_SpoolFileName));
    m_bSpoolOn = ID_FALSE;
    m_fpSpool  = NULL;

    if ( (m_Buf = (SChar*)idlOS::malloc(gProperty.GetCommandLen())) == NULL )
    {
        idlOS::fprintf(stderr, "Memory allocation error!!! --- (%d, %s)\n",
                       __LINE__, __FILE__);
        exit(0);
    }
    idlOS::memset(m_Buf, 0x00, gProperty.GetCommandLen());
}

iSQLSpool::~iSQLSpool()
{
    if ( m_Buf != NULL )
    {
        idlOS::free(m_Buf);
        m_Buf = NULL;
    }
}

IDE_RC 
iSQLSpool::SetSpoolFile( SChar * a_FileName )
{
    IDE_TEST_RAISE(m_bSpoolOn == ID_TRUE, already_spool_on);

    m_fpSpool = isql_fopen(a_FileName, "r");
    IDE_TEST_RAISE(m_fpSpool != NULL, already_exist_file);

    m_fpSpool = isql_fopen(a_FileName, "wt");
    IDE_TEST_RAISE(m_fpSpool == NULL, fail_open_file); 

    m_bSpoolOn = ID_TRUE;
    idlOS::strcpy(m_SpoolFileName, a_FileName);
    idlOS::sprintf(m_Buf, (SChar*)"Spool start. [%s]\n", a_FileName);
    PrintOutFile();

    return IDE_SUCCESS;

    IDE_EXCEPTION(already_spool_on);
    {
        idlOS::sprintf(m_Buf, (SChar*)"Already spool on state. [%s]\n",
                       m_SpoolFileName);
        PrintOutFile();
    }
    IDE_EXCEPTION(already_exist_file); 
    {
        idlOS::fclose( m_fpSpool );
        idlOS::sprintf(m_Buf, "Already exist file. [%s]\n", a_FileName);
        PrintOutFile();
    }
    IDE_EXCEPTION(fail_open_file); 
    {
        idlOS::sprintf(m_Buf, "Can not open file. [%s]\n", a_FileName);
        PrintOutFile();
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC 
iSQLSpool::SpoolOff()
{
    IDE_TEST_RAISE(m_bSpoolOn == ID_FALSE, not_spool_on);

    IDE_TEST_RAISE(idlOS::fclose(m_fpSpool) != 0, fail_close_file);

    m_bSpoolOn         = ID_FALSE;
    m_fpSpool          = NULL;

    idlOS::sprintf(m_Buf, (SChar*)"Spool Stop\n");
    PrintOutFile();
    
    m_SpoolFileName[0] = '\0';

    return IDE_SUCCESS;

    IDE_EXCEPTION(not_spool_on);
    {
        idlOS::strcpy(m_Buf, (SChar*)"Is not spool on state.\n");
        PrintOutFile();
    }
    IDE_EXCEPTION(fail_close_file);
    {
        m_bSpoolOn         = ID_FALSE;
        m_fpSpool          = NULL;
        idlOS::sprintf(m_Buf, "Fail close file. [%s]", m_SpoolFileName);
        PrintOutFile();
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void 
iSQLSpool::PrintPrompt()
{
    idlOS::fprintf(gProgOption.m_OutFile, "iSQL> ");
    idlOS::fflush(gProgOption.m_OutFile);

    if ( m_bSpoolOn == ID_TRUE && m_fpSpool != NULL )
    {
        idlOS::fprintf(m_fpSpool, "iSQL> ", m_Buf);
        idlOS::fflush(m_fpSpool);
    }
}

void 
iSQLSpool::Print()
{
    if (gProperty.GetTerm() == ID_TRUE ||
        gSQLCompiler->IsFileRead() == ID_FALSE )
    {
        idlOS::fprintf(gProgOption.m_OutFile, "%s", m_Buf);
        idlOS::fflush(gProgOption.m_OutFile);
    }

    if ( m_bSpoolOn == ID_TRUE && m_fpSpool != NULL )
    {
        idlOS::fprintf(m_fpSpool, "%s", m_Buf);
        idlOS::fflush(m_fpSpool);
    }
}

void 
iSQLSpool::PrintOutFile()
{
    idlOS::fprintf(gProgOption.m_OutFile, "%s", m_Buf);
    idlOS::fflush(gProgOption.m_OutFile);
}

void 
iSQLSpool::PrintCommand()
{
/*    if ( gProgOption.IsVerbose() == ID_TRUE )
    {
        idlOS::fprintf(gProgOption.m_OutFile, "%s\n", m_Buf);
        idlOS::fflush(gProgOption.m_OutFile);
    }
*/
    if ( m_bSpoolOn == ID_TRUE && m_fpSpool != NULL )
    {
        idlOS::fprintf(m_fpSpool, "iSQL> %s", m_Buf);
        idlOS::fflush(m_fpSpool);
    }
}

void 
iSQLSpool::PrintWithDouble(SInt *aPos)
{
/***********************************************************************
 *
 * Description :
 *    DOUBLE ���� ������ �������� ���
 *
 * Implementation :
 *    ������ ǥ���� ������ ������� �ʱ� ����,
 *    ������ ���� ������ ������.
 *
 *    DOUBLE --> TNumeric --> Varchar --> Char String
 *
 ***********************************************************************/

    SInt   sPos = 0;
    SChar  sTmp[1024];
    SChar  sVarTmp[1024];
    
    idBool sConverted;

    idaTNumericType  sTNumeric;
    idaVarcharType * sVarchar;

    SInt   sLen;
    
#ifdef VC_WIN32
    SChar * spDest;
    SInt    sResult;
#endif

    sConverted = ID_TRUE;
    
    while ( 1 )
    {
        //------------------------------------------
        // Double --> TNumeric
        //------------------------------------------
        
        idlOS::sprintf(sTmp, "%f", m_DoubleBuf);
        if( idaTNumeric( &sTNumeric, (UChar*) sTmp, idlOS::strlen( sTmp ) )
            != IDE_SUCCESS )
        {
            sConverted = ID_FALSE;
            break;
        }
        else
        {
            // Go Go
        }

        //------------------------------------------
        // TNumeric --> Varchar
        //------------------------------------------
        
        sVarchar = (idaVarcharType *) sVarTmp;
        if (idaVarchar( sVarchar, 1024, & sTNumeric ) != IDE_SUCCESS)
        {
            sConverted = ID_FALSE;
            break;
        }
        else
        {
            // Go Go
        }

        sLen = IDA_GET_LENGTH( sVarchar->length);

        if ( (sLen > 0) && (sLen <= NUMBER_SIZE) )
        {
            sConverted = ID_TRUE;
        }
        else
        {
            sConverted = ID_FALSE;
        }
        
        break;
    }
    
    if ( sConverted == ID_TRUE )
    {
        //------------------------------------------
        // Varchar ��ȯ�� ������ ���
        //------------------------------------------

        if ( sVarchar->value[0] == '.' )
        {
            sTmp[0] = '0';
            idlOS::memcpy(sTmp + 1, sVarchar->value, sLen);
            sTmp[sLen+1] = '\0';
        }
        else if ( idlOS::strncmp( (SChar*) sVarchar->value, "-.", 2) == 0 )
        {
            sTmp[0] = '-';
            sTmp[1] = '0';
            idlOS::memcpy( sTmp + 2, sVarchar->value + 1, sLen - 1);
            sTmp[sLen+1] = '\0';
        }
        else
        {
            idlOS::memcpy( sTmp, sVarchar->value, sLen);
            sTmp[sLen] = '\0';
        }
        
    }
    else
    {
        //------------------------------------------
        // Varchar ��ȯ�� ������ ���
        //------------------------------------------

        idlOS::sprintf(sTmp, "%f", m_DoubleBuf);
        
        if ( idlOS::strlen(sTmp) > NUMBER_SIZE )
        {
            idlOS::sprintf(sTmp, "%11.5E", m_DoubleBuf);
            
#ifdef VC_WIN32 // ex> UNIX:WIN32 = 3.06110e+04:3.06110e+004
            spDest = strstr(sTmp, "+0");
            if (spDest != NULL)
            {
                sResult = (SInt) (spDest - sTmp);
                idlOS::memcpy( sTmp + sResult + 1,
                               sTmp + sResult + 2,
                               idlOS::strlen(sTmp + sResult + 2) + 1);
            }
#endif
        }
    }
    
    sPos = *aPos;
    *aPos += idlOS::sprintf(m_Buf + sPos, "%-11s", sTmp);
    m_Buf[*aPos] = ' ';
}

void 
iSQLSpool::PrintWithFloat(SInt *aPos)
{
/***********************************************************************
 *
 * Description :
 *    REAL ���� ������ �������� ���
 *
 * Implementation :
 *    ������ ǥ���� ������ ������� �ʱ� ����,
 *    ������ ���� ������ ������.
 *
 *    REAL --> ( DOUBLE --> TNumeric --> Varchar --> Char String )
 *
 ***********************************************************************/

    SChar sTmp[512];
    SInt  sPos;

#ifdef VC_WIN32
    SChar * spDest;
    SInt    sResult;
#endif

    // To Fix PR-9702
    // REAL Type�� ��� DOUBLE�� Type Casting�ϰų�,
    // sprintf("%f") ������ ����� ���, ���� ū ���� ���
    // distortion�� �߻��ϰ� �ȴ�.
    //          Ex) REAL'123456789' ==> DOUBLE'123456792'
    // �̴� �ڿ����� ����ӿ��� �ұ��ϰ�,
    // ����� ���忡���� �޾Ƶ��� �� ���� ������ �� �� �ִ�.
    // �̷��� ������ Work-Around ó���ϱ� ���Ͽ�
    // ������ ū �� �̻� ���ؼ��� ������ ǥ������ ����ϰ�,
    // �� �ܿ� ���ؼ��� Numeric������ ��ȯ�� ���� ����Ͽ�
    // ������, �Ǽ���, ������ ������ ǥ���� �� �ֵ��� �Ѵ�.
    if ( (m_FloatBuf > 1000000) || (m_FloatBuf < -1000000) )
    {
        idlOS::sprintf(sTmp, "%f", m_FloatBuf);
        
        if ( idlOS::strlen(sTmp) > NUMBER_SIZE )
        {
            idlOS::sprintf(sTmp, "%11.5E", m_FloatBuf);
            
#ifdef VC_WIN32 // ex> UNIX:WIN32 = 3.06110e+04:3.06110e+004
            spDest = strstr(sTmp, "+0");
            if (spDest != NULL)
            {
                sResult = (SInt) (spDest - sTmp);
                idlOS::memcpy( sTmp + sResult + 1,
                               sTmp + sResult + 2,
                               idlOS::strlen(sTmp + sResult + 2) + 1);
            }
#endif
        }

        sPos = *aPos;
        *aPos += idlOS::sprintf(m_Buf + sPos, "%-11s", sTmp);
        m_Buf[*aPos] = ' ';
    }
    else
    {
        // Double �� ���� ����
        m_DoubleBuf = m_FloatBuf;

        // Double �� ���
        PrintWithDouble(aPos);
    }
}
