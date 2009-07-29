#include "logcore.h"
#include "log.h"
#include <QCoreApplication>
#include <QThread>

#ifdef _WIN32
    #include <atlbase.h>
#endif

// LogCore statics 

LogCore *LogCore::m_LogCore = NULL;
QMutex LogCore::m_LogMutex;
int LogCore::m_nRefCount = 0;
bool LogCore::m_bLog = false;
QMap<QThread *,int> LogCore::m_ThreadMap;


LogCore::LogCore()
{
}


void LogCore::Init()
{
    QMutexLocker lock( &m_LogMutex );
    if ( m_LogCore == NULL )
        m_LogCore = new LogCore();
}


static const char *toString(LogTypes::LogTypes type)
{
    switch ( type )
    {
        case LogTypes::Error:        return "Error";
        case LogTypes::Warning:      return "Warning";
        case LogTypes::Info:         return "Info";
        case LogTypes::Debug:        return "Debug";
        case LogTypes::Audit:        return "Audit";
        case LogTypes::FuncEnter:    return "FuncEnter";
        case LogTypes::FuncExit:     return "FuncExit";
        default:                     return "";
    }
}

// Qt doesn't give a very presentable thread id, so, as the id doesn't mean anything,
// and all we want is to distinguish between threads, we map the thread pointer to an integer.
int LogCore::GetThreadId()
{
    QMap<QThread *,int>::iterator it = m_ThreadMap.find( QThread::currentThread() );

    // Already there.
    if ( it != m_ThreadMap.end() )
	return it.value();

    // Create new id, starting at 1
    m_ThreadMap.insert( QThread::currentThread(), m_ThreadMap.count()+1 );
    return m_ThreadMap.count();
}

void LogCore::LogMsg( LogTypes::LogTypes type, QDateTime timeStamp, const QString &sModule, const QString &sComponent, const QString &sFile, int nLine, const QString &sMsg )
{
    QString s = QString("%1,%2,%3,%4:%5,%6,%7,%8,%9\n").arg(timeStamp.toString("yyyy/MM/dd hh:mm:ss.zzz")).arg(QCoreApplication::applicationPid()).arg(GetThreadId()).arg(sFile).arg(nLine).arg(toString(type)).arg(sModule).arg(sComponent).arg(sMsg);
    // Make sure we only have 1 \n
    while ( s.length() > 1 && s[s.length()-2] == QChar('\n') )
        s.remove( s.length()-1, 1 );

    #ifdef _WIN32
        ATLTRACE2( s.toAscii().constData() ); 
    #else
        fputs( s.toAscii().constData(), stderr );
        fflush( stderr );
    #endif
}


bool LogCore::WillLog( const QString &/*sModule*/, const QString /*sComponent*/, LogTypes::LogTypes type )
{
    if ( type == LogTypes::Error || type == LogTypes::Warning || type == LogTypes::Audit )
        return true;
    return m_bLog;  // todo
}


