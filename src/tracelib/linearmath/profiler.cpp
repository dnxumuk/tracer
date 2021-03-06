
#include "profiler.h"

Profiler::Profiler()
{
    _logFileName = "";
    _msgPrefix   = "";
    _msg         = "";

    _tickStorage = 0;

    isMeasuring = false;
    mustKeepLog = false;
    isPaused    = false;
}

Profiler::Profiler( string message )
{
    _logFileName = "";
    _msgPrefix   = "";
    _msg         = message;

    _tickStorage = 0;

    isMeasuring = true;
    mustKeepLog = false;
    isPaused    = false;
}

Profiler::Profiler( string message, bool isMeasuring )
{
    _logFileName = "";
    _msgPrefix   = "";
    _msg         = message;

    _tickStorage = 0;

    this->isMeasuring = isMeasuring;
    mustKeepLog = false;
    isPaused    = false;
}

Profiler::Profiler( string message, string msgPrefix, string logFileName, bool isMeasuring )
{
    _logFileName = logFileName;
    _msgPrefix   = msgPrefix;
    _msg         = message;

    _tickStorage = 0;

    this->isMeasuring = isMeasuring;
    this->mustKeepLog = true;
    isPaused          = false;
}

// Clearing timer values if enable measurements after it has been disabed
void Profiler::setMeasurements( bool value )
{
    if ( value && !this->isMeasuring )
        _tickFrom = _tickTo = _tickStorage = 0;
    isMeasuring = value;
}
void Profiler::setKeepingLog( bool value )
{
    this->mustKeepLog = value;
}

bool Profiler::isMeadurement() const
{
    return isMeasuring;
}

bool Profiler::isKeepingLog()  const
{
    return mustKeepLog;
}

void Profiler::setMessage( string message )
{
    _msg = message;
}
void Profiler::setMeassge( string message , string msgPrefix )
{
    _msg = message;
    _msgPrefix = msgPrefix;
}
void Profiler::setMessagePrefix ( string msgPrefix )
{
    _msgPrefix = msgPrefix;
}

string Profiler::getCurrentTime() const
{
    const unsigned int bufferSize = 80;
    char  buffer[ bufferSize ];

    tm            timeInfo;
    const tm      *pTimeInfo = &timeInfo;
    time_t        rawTime;
    const time_t  *pRawTime =&rawTime;
    
    time( &rawTime );
    localtime_s( &timeInfo, pRawTime );

    strftime( buffer, bufferSize, "%Y-%m-%d %I:%M:%S", pTimeInfo );
    return string( buffer );
}


void Profiler::openLogFile()  
{
    if ( mustKeepLog )
        _logFile.open( _logFileName.c_str() , ios::app );
    if ( ! _logFile.is_open() )
        mustKeepLog = false;
};

void Profiler::closeLogFile() 
{
    // _logFile.is_open() && _logFile.close()
    if ( _logFile.is_open() )
        _logFile.close();
};

void Profiler::showResults()
{
    cout << composeLogMessage();
    if ( mustKeepLog )
        toLogResults();
}

void Profiler::toLogResults()   
{
    openLogFile();
    if ( mustKeepLog )
        _logFile << composeLogMessage();  
}

string Profiler::composeLogMessage() const
{
    // Here we're composing a log message in the following format:
    // [ YYYY-mm-dd HH:MM:SS ] %Result %result_dimention fps: %fps . ( %Message_prefix ) %Message .
	if (!isMeasuring)
		return _msg + "\n";
	
	string message("[");
    message += ( getCurrentTime() + "] " + getTimeResultString() + " => " );
    if ( _msgPrefix.length() > 0 )
        message +=  "(" + _msgPrefix + ") " + _msg ;
    else
        message += _msg ;
	message += "\n";
    return message; 
}

double Profiler::getFPS() const {
  long   tick = _tickStorage + _tickTo - _tickFrom;
  double seconds = tick / (double)CLOCKS_PER_SEC;
  return 1.0 / seconds;

}

string Profiler::getTimeResultString() const
{
    long   tick = _tickStorage + _tickTo - _tickFrom;
    double seconds = tick / (double)CLOCKS_PER_SEC;
    double fps = 1 / seconds;

    const unsigned int bufferSize = 128;
    char  buffer[ bufferSize ];
    if ( tick == 0 )
    {
        sprintf_s( &buffer[0], bufferSize, "%.4f ms fps: inf . ", seconds * 1000 );
        return string( buffer );
    }
    if ( seconds < 1 )
    {
        // mSec
        sprintf_s( &buffer[0], bufferSize, "%.4f ms fps: %.2f.", seconds * 1000 , fps );
        return string( buffer );
    }
    if ( seconds < 60 )
    {
        // Secs
        sprintf_s( &buffer[0], bufferSize, "%.2f s  fps: %.2f.", seconds , fps  );
        return string( buffer );
    }
    if ( seconds < 60*60 )
    {
        // Minutes 
        unsigned int minutes = (unsigned int)floor( seconds/60 );
        double       sec     = seconds - minutes * 60 ;
        sprintf_s( &buffer[0], bufferSize, "%dm %.1fs  fpm: %.2f.", minutes, sec , fps*60  );
        return string( buffer );
    }
    if ( seconds < 60*60*60 )
    {
        // Hours
        double rest;
        unsigned int hours   = (unsigned int)floor( seconds/(60*60) );
        rest = seconds - hours*60*60;
        unsigned int minutes = (unsigned int)floor( rest/60 );
        rest = rest - minutes*60;
        double sec  = rest ;
        sprintf_s( &buffer[0], bufferSize, "%dh %dm %.1fs fpm: %.2f.", hours, minutes, sec , fps*60  );
        return string( buffer );
    } else {
      return std::string(buffer);
    }

}


void Profiler::start()
{
    _tickFrom = clock();
    _tickTo = _tickStorage = 0;
    isPaused = false;
}

void Profiler::finish()
{
    _tickTo = clock();
    showResults();
}

void Profiler::pause()
{
    if ( isPaused )
        return;
    _tickTo = clock();
    _tickStorage += _tickTo - _tickFrom;
    _tickTo = _tickFrom = 0;
    isPaused = true;
}

void Profiler::unpause()
{
    if ( ! isPaused )
        return;
    _tickFrom = clock();
}

std::string Profiler::getResults()
{
	return string( composeLogMessage() );
}


Profiler::~Profiler()
{
    closeLogFile();
}
