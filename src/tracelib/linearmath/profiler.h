#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <math.h>


using namespace std;

class Profiler
{
public:
    Profiler();
    Profiler( string message );
    Profiler( string message, bool isMeasurung );
    Profiler( string message, string msgPrefix, string logFileName, bool isMeasuring );
    ~Profiler();

    void start();
    void finish();
    void pause();
    void unpause();
    std::string getResults();
    double getFPS() const;
    inline void setMeasurements( bool value = true );
    inline void setKeepingLog( bool value = true );
    inline bool isMeadurement() const;
    inline bool isKeepingLog()  const;

    inline void setMessage( string message );
    inline void setMeassge( string message , string msgPrefix );
    inline void setMessagePrefix ( string msgPrefix );
//private :
public:
    ofstream _logFile;
    string   _logFileName;
    string   _msgPrefix;
    string   _msg;

    clock_t  _tickFrom;
    clock_t  _tickTo;
    clock_t  _tickStorage;  // This member is using to store gathered values after pause / unpause methods call

    bool isPaused;
    bool isMeasuring;
    bool mustKeepLog;

    string getCurrentTime() const;
    void openLogFile()  ;
    void closeLogFile() ;
    void showResults() ;
    void toLogResults();
    string composeLogMessage() const;
    string getTimeResultString () const;


};
