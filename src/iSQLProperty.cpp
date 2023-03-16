
/***********************************************************************
 * $Id: iSQLProperty.cpp 10083 2004-12-21 06:08:18Z hssohn $
 **********************************************************************/

#include <idl.h>
#include <utString.h>
#include <utISPApi.h>
#include <iSQLSpool.h>
#include <iSQLProperty.h>

extern utString    gString;
extern iSQLSpool * gSpool;

iSQLProperty::iSQLProperty()
{ 
    m_LineSize  = 80;
    m_PageSize  = 0;
    m_Term      = ID_TRUE;
    m_Timing    = ID_FALSE;
    m_Heading   = ID_TRUE;
    m_Verbose   = ID_TRUE;
    m_TimeScale = iSQL_SEC;
    m_IsDisplayComment = ID_FALSE;
    m_ShowForeignKeys = ID_FALSE;
    m_PlanCommit = ID_FALSE;
    m_QueryLogging = ID_FALSE;
    idlOS::memset(m_UserName, 0x00, sizeof(m_UserName));

    SetEnv();
}

iSQLProperty::~iSQLProperty()
{ 
}

/* ============================================
 * iSQL ���� ȯ�溯���� �о ����
 * ============================================ */
void
iSQLProperty::SetEnv()
{
    /* ============================================
     * ISQL_BUFFER_SIZE, default : 64K 
     * ============================================ */
    if (idlOS::getenv("ISQL_BUFFER_SIZE"))              
    {       
        m_CommandLen = atoi(idlOS::getenv("ISQL_BUFFER_SIZE"));
    }       
    else
    {           
        m_CommandLen = COMMAND_LEN;
    }
            
    /* ============================================
     * ISQL_EDITOR, default : /usr/bin/vi 
     * ============================================ */
    if (idlOS::getenv("ISQL_EDITOR"))
    {
        idlOS::strcpy(m_Editor, idlOS::getenv("ISQL_EDITOR"));
    }   
    else    
    {       
        idlOS::strcpy(m_Editor, ISQL_EDITOR);
    }

    /* ============================================
     * ISQL_CONNECTION, default : TCP 
     * ============================================ */
    if (idlOS::getenv("ISQL_CONNECTION"))
    {
        idlOS::strcpy(m_Conntype, idlOS::getenv("ISQL_CONNECTION"));
    }   
    else    
    {       
        idlOS::strcpy(m_Conntype, "TCP");
    }
}

/* ============================================
 * Set LineSize 
 * Display�Ǵ� �� ������ ����.
 * ============================================ */
void 
iSQLProperty::SetLineSize( SChar * a_CommandStr, 
                           SInt    a_LineSize )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    if ( (a_LineSize < 10) || (a_LineSize > 32767) )
    {
        idlOS::sprintf(gSpool->m_Buf, "%s", 
                (SChar*)"LineSize option value out of range(10 - 32767).\n");
        gSpool->Print();
    }
    else
    {
        m_LineSize = a_LineSize;
    }
}

/* ============================================
 * Set PageSize 
 * ���ڵ带 �� �� ������ ������ ���ΰ�.
 * ============================================ */
void 
iSQLProperty::SetPageSize( SChar * a_CommandStr, 
                           SInt    a_PageSize )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    if ( a_PageSize < 0 || a_PageSize > 50000 )
    {
        idlOS::sprintf(gSpool->m_Buf, "%s", 
                (SChar*)"PageSize option value out of range(0 - 50000).\n");
        gSpool->Print();
    }
    else
    {
        m_PageSize = a_PageSize;
    }
}

/* ============================================
 * Set Term 
 * �ܼ� ȭ�������� ����� �� ���ΰ� �� ���ΰ�.
 * ============================================ */
void 
iSQLProperty::SetTerm( SChar * a_CommandStr, 
                       idBool  a_IsTerm )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    m_Term = a_IsTerm;
}

/* ============================================
 * Set Timing 
 * ���� ���� �ð��� ������ ���ΰ� �� ���ΰ�.
 * ============================================ */
void 
iSQLProperty::SetTiming( SChar * a_CommandStr, 
                         idBool  a_IsTiming )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    m_Timing = a_IsTiming;
}

/* ============================================
 * Set Heading 
 * ���(Column Name)�� ������ ���ΰ� �� ���ΰ�.
 * ============================================ */
void 
iSQLProperty::SetHeading( SChar * a_CommandStr, 
                          idBool  a_IsHeading )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    m_Heading = a_IsHeading;
}

/* ============================================
 * Set TimeScale 
 * ���� ���� �ð��� ����  
 * ============================================ */
void 
iSQLProperty::SetTimeScale( SChar         * a_CommandStr, 
                            iSQLTimeScale   a_Timescale )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    m_TimeScale = a_Timescale;
}

/* ============================================
 * Set Verbose 
 * ��� ���Ͽ� ���ɾ ����� ���ΰ� �� ���ΰ�.
 * ============================================ */
void 
iSQLProperty::SetVerbose( SChar * a_CommandStr, 
                          idBool  a_IsVerbose )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    m_Verbose = a_IsVerbose;
}

/* ============================================
 * Set Comment 
 * ��� ���Ͽ� �ּ��� ����� ���ΰ� �� ���ΰ�.
 * ============================================ */
void 
iSQLProperty::SetComment( SChar * a_CommandStr, 
                          idBool  a_IsDisplayComment )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    m_IsDisplayComment = a_IsDisplayComment;
}

/* ============================================
 * Set User 
 * Connect�� ������ ������ ������ �����Ѵ�.
 * ============================================ */
void 
iSQLProperty::SetUserName( SChar * a_UserName )
{ 
    gString.toUpper(a_UserName);
    idlOS::strcpy(m_UserName, a_UserName);
}

/* ============================================
 * Set ForeignKeys 
 * desc ����� foreign key ������ ������ ������ 
 * ============================================ */
void 
iSQLProperty::SetForeignKeys( SChar * a_CommandStr, 
                              idBool  a_ShowForeignKeys )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    m_ShowForeignKeys = a_ShowForeignKeys;
}

/* ============================================
 * Set PlanCommit
 * autocommit mode false �� ���ǿ��� explain plan ��
 * on �Ǵ� only �� ���� ��, desc, select * From tab;
 * ���v ���ɾ ����ϰ� �Ǹ�, ������ ��������
 * Ʈ������� ������ ��쿡 ������ �߻��ϰ� �ȴ�.
 * error -> The transaction is already active.
 * �̸� �����ϱ� ���ؼ� �������� commit �� �ڵ�����
 * �����ϵ��� �ϴ� �ɼ��� �� �� �ִ�.
 * ============================================ */
void 
iSQLProperty::SetPlanCommit( SChar * a_CommandStr, 
                             idBool  a_Commit )
{
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    m_PlanCommit = a_Commit;
}

void 
iSQLProperty::SetQueryLogging( SChar * a_CommandStr, 
                               idBool  a_Logging ) 
{ 
    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr); 
    gSpool->PrintCommand(); 

    m_QueryLogging = a_Logging; 
} 

/* ============================================
 * ������ iSQL Option�� �����ش�. 
 * ============================================ */
void 
iSQLProperty::ShowStmt( SChar          * a_CommandStr, 
                        iSQLOptionKind   a_iSQLOptionKind )
{
    SChar tmp[20];

    idlOS::sprintf(gSpool->m_Buf, "%s", a_CommandStr);  
    gSpool->PrintCommand();

    switch(a_iSQLOptionKind)
    {
    case iSQL_SHOW_ALL :
        idlOS::sprintf(gSpool->m_Buf, "User      : %s\n", m_UserName);
        gSpool->Print();
        idlOS::sprintf(gSpool->m_Buf, "LineSize  : %d\n", m_LineSize);
        gSpool->Print();
        idlOS::sprintf(gSpool->m_Buf, "PageSize  : %d\n", m_PageSize);
        gSpool->Print();
/*        
        if (m_IsDisplayComment == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Comment   : display\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Comment   : not display\n");
        gSpool->Print();
*/
        if (m_TimeScale == iSQL_SEC)
            idlOS::strcpy(tmp, "Second");
        else if (m_TimeScale == iSQL_MILSEC)
            idlOS::strcpy(tmp, "MilliSecond");
        else if (m_TimeScale == iSQL_MICSEC)
            idlOS::strcpy(tmp, "MicroSecond");
        else if (m_TimeScale == iSQL_NANSEC)
            idlOS::strcpy(tmp, "NanoSecond");
        idlOS::sprintf(gSpool->m_Buf, "TimeScale : %s\n", tmp);
        gSpool->Print();

        if (m_Heading == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Heading   : On\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Heading   : Off\n");
        gSpool->Print();

        if (m_Timing == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Timing    : On\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Timing    : Off\n");
        gSpool->Print();
        
        if (m_Verbose == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Verbose   : On\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Verbose   : Off\n");
        gSpool->Print();

        if (m_ShowForeignKeys == ID_TRUE)
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"ForeignKeys : On\n");
        }
        else
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"ForeignKeys : Off\n");
        }
        gSpool->Print();

        if (m_PlanCommit == ID_TRUE)
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"PlanCommit : On\n");
        }
        else
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"PlanCommit : Off\n");
        }
        gSpool->Print();

        if (m_QueryLogging == ID_TRUE) 
        { 
            idlOS::sprintf(gSpool->m_Buf, "%s", 
                           (SChar*)"QueryLogging : On\n"); 
        } 
        else 
        { 
            idlOS::sprintf(gSpool->m_Buf, "%s", 
                           (SChar*)"QueryLogging : Off\n"); 
        } 
        gSpool->Print(); 

        break;
    case iSQL_HEADING :
        if (m_Heading == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Heading : On\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Heading : Off\n");
        gSpool->Print();
        break;
    case iSQL_LINESIZE :
        idlOS::sprintf(gSpool->m_Buf, "LineSize : %d\n", m_LineSize);
        gSpool->Print();
        break;
    case iSQL_PAGESIZE :
        idlOS::sprintf(gSpool->m_Buf, "PageSize : %d\n", m_PageSize);
        gSpool->Print();
        break;
    case iSQL_TERM :
        if (m_Term == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Term : On\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Term : Off\n");
        gSpool->Print();
        break;
    case iSQL_TIMING :
        if (m_Timing == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Timing : On\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Timing : Off\n");
        gSpool->Print();
        break;
    case iSQL_VERBOSE :
        if (m_Verbose == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Verbose : On\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Verbose : Off\n");
        gSpool->Print();
        break;
    case iSQL_FOREIGNKEYS :
        if (m_ShowForeignKeys == ID_TRUE)
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"ForeignKeys : On\n");
        }
        else
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"ForeignKeys : Off\n");
        }
        gSpool->Print();
        break;
    case iSQL_PLANCOMMIT :
        if (m_PlanCommit == ID_TRUE)
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"PlanCommit : On\n");
        }
        else
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"PlanCommit : Off\n");
        }
        gSpool->Print();
        break;
    case iSQL_QUERYLOGGING :
        if (m_QueryLogging  == ID_TRUE)
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"QueryLogging : On\n");
        }
        else
        {
            idlOS::sprintf(gSpool->m_Buf, "%s",
                           (SChar*)"QueryLogging : Off\n");
        }
        gSpool->Print();
        break;
    case iSQL_USER :
        idlOS::sprintf(gSpool->m_Buf, "User : %s\n", m_UserName);
        gSpool->Print();
        break;
    case iSQL_TIMESCALE :
        if (m_TimeScale == iSQL_SEC)
            idlOS::strcpy(tmp, "Second");
        else if (m_TimeScale == iSQL_MILSEC)
            idlOS::strcpy(tmp, "MilliSecond");
        else if (m_TimeScale == iSQL_MICSEC)
            idlOS::strcpy(tmp, "MicroSecond");
        else if (m_TimeScale == iSQL_NANSEC)
            idlOS::strcpy(tmp, "NanoSecond");
        idlOS::sprintf(gSpool->m_Buf, "TimeScale : %s\n", tmp);
        gSpool->Print();
        break;
    case iSQL_COMMENT :
        if (m_IsDisplayComment == ID_TRUE)
            idlOS::sprintf(gSpool->m_Buf, "%s", 
                    (SChar*)"Display comment\n");
        else
            idlOS::sprintf(gSpool->m_Buf, "%s", 
                    (SChar*)"Not display comment\n");
        gSpool->Print();
        break;
    default :
        idlOS::sprintf(gSpool->m_Buf, "%s", (SChar*)"Have no information.\n");
        gSpool->Print();
        break;
    }
}

