 
////////////////////////////////////////////////////////////////////////
// This file is part of the SndObj library
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
//
// Copyright (c)Victor Lazzarini, 1997-2004
// See License.txt for a disclaimer of all warranties
// and licensing information

/***************************************************/
// SndMidiIn.h: interface of the SndMidiIn          /
// (LINUX/CYGWIN/SGI) class, MIDI input             /
//                                                  /
/***************************************************/


#ifndef _SNDMIDIIN_H
#define _SNDMIDIIN_H



#include "SndMidi.h" 

#ifdef OSS

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#endif

class SndMidiIn : public SndMidi {

protected:

#ifdef SGI

void PutOutput(MDevent* cur);

#endif

#ifdef WIN

HMIDIIN m_handle;
MMRESULT m_result;

inline void PutOutput(MIDI_event*cur);

void WriteBuf(MIDI_event event);
MIDI_event ReadBuf();

private:

friend 
#ifdef VISUALC
static
#endif 
void  
#ifndef SWIGFIX
CALLBACK
#endif
midiInputHandler(HMIDIIN, UINT wMsg, DWORD dwInstance,
    DWORD dwParam1, DWORD dwParam2);

#endif 

public:

SndMidiIn();

#if defined (OSS) || defined (SGI)
SndMidiIn(char* port, int buffsize = 64);
#endif

#ifdef WIN
SndMidiIn(int port, int buffsize = 64);
#endif

~SndMidiIn();

short Read();


char* ErrorMessage();

};


#endif

