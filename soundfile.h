#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <sndfile.hh>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

class SoundFile {
    const char *soundFileName;
    int nChannels;
    int sampleRate;
    int format;
    SndfileHandle sndfd;
public:
    SoundFile();
    void create_file(const char *fname, int channels, int srate, int fileFormat);
    void write_file();
    void read_file();

};

#endif // SOUNDFILE_H

