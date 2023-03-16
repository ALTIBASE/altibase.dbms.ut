
/***********************************************************************
 * $Id: iSQLSpool.h 9375 2004-10-21 02:32:24Z bethy $
 **********************************************************************/

#ifndef _O_ISQLSPOOL_H_ 
#define _O_ISQLSPOOL_H_ 1

#include <iSQL.h>

class iSQLSpool
{
public:
    iSQLSpool();
    ~iSQLSpool();
    
    idBool IsSpoolOn()      { return m_bSpoolOn; }
    IDE_RC SetSpoolFile(SChar *a_FileName);
    IDE_RC SpoolOff();
    void   Print();
    void   PrintPrompt();
    void   PrintOutFile();
    void   PrintCommand();
    void   PrintWithDouble(SInt *aPos);
    void   PrintWithFloat(SInt *aPos);

public:
    SChar  * m_Buf;
    SFloat   m_FloatBuf;
    SDouble  m_DoubleBuf;

private:
    idBool   m_bSpoolOn;
    FILE   * m_fpSpool;
    SChar    m_SpoolFileName[WORD_LEN];
};

#endif // _O_ISQLSPOOL_H_

