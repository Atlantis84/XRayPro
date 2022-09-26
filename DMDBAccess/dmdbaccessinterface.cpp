#include "dmdbaccessinterface.h"
#include "dbaccessimpl.h"
static DBAccessImpl* m_pGDBObject = NULL;
DMDBAccessInterface::DMDBAccessInterface()
{
}

DMDBAccessInterface *DMDBAccessInterface::GetInterface()
{
    if(m_pGDBObject == NULL)
        m_pGDBObject = new DBAccessImpl();
    return (DMDBAccessInterface *)m_pGDBObject;
}
