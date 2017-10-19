#include "soundfile.h"

SoundFile::SoundFile()
{

}

//Last argument: Format | Subtype
//rf64File.create_file("Testfile", 2, 44100, SF_FORMAT_WAV | SF_FORMAT_PCM_16);
void SoundFile::create_file(const char *fname, int channels, int srate, int fileFormat){
    soundFileName = fname;
    nChannels = channels;
    sampleRate = srate;
    format = fileFormat;

    cout<<"Creating sound file with the name: "<< fname <<"\n";

    sndfd = SndfileHandle (soundFileName, SFM_WRITE, format, nChannels, sampleRate) ;
    if (sndfd != NULL) {
        cout<<"File created. \n ";
    }
    else {
        printf("%s\n",sf_strerror(NULL));
        exit(1);
    }
}
