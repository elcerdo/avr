#include <iostream>
#include <asoundlib.h>
using std::endl;
using std::cout;

int main(int argc,char *argv[]) {
    snd_seq_t *handle = NULL;
    if ( snd_seq_open(&handle,"default",SND_SEQ_OPEN_DUPLEX,0) < 0 ) return 1;
    snd_seq_set_client_name(handle,"Monome");
    cout<<"created alsa client "<<snd_seq_client_id(handle)<<" '"<<snd_seq_name(handle)<<"'"<<endl;

    int synth_out  = snd_seq_create_simple_port(handle,"Synth Midi-Out",SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    int led_out  = snd_seq_create_simple_port(handle,"Led Midi-Out",SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    int key_in = snd_seq_create_simple_port(handle,"Key Midi-In",SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    if ( synth_out < 0  or led_out < 0 or key_in < 0 ) { snd_seq_close(handle); return 1; }
    cout<<"created ports "<<endl;

    snd_seq_event_t *event = new snd_seq_event_t;
    while ( snd_seq_event_input(handle,&event) ) {
        //cout<<"from "<<static_cast<int>(event->source.client)<<","<<static_cast<int>(event->source.port)<<" to "<<static_cast<int>(event->dest.client)<<","<<static_cast<int>(event->dest.port)<<endl;
        if ( event->dest.port == synth_out ) {
            cout<<"[synth] ";
            switch ( event->type ) {
            case SND_SEQ_EVENT_NOTEON:  cout<<"got note on  channel:"<<static_cast<int>(event->data.note.channel)<<" note:"<<static_cast<int>(event->data.note.note)<<" velocity:"<<static_cast<int>(event->data.note.velocity)<<endl; break;
            case SND_SEQ_EVENT_NOTEOFF: cout<<"got note off channel:"<<static_cast<int>(event->data.note.channel)<<" note:"<<static_cast<int>(event->data.note.note)<<" velocity:"<<static_cast<int>(event->data.note.velocity)<<endl; break;
            default: cout<<"unknow event "<<static_cast<int>(event->type)<<endl; break;
            }
        } else if ( event->dest.port == led_out ) {
            cout<<"[led] got event"<<endl;
        } else {
            cout<<"unknow port "<<static_cast<int>(event->dest.port)<<endl;
        }
    }
    delete event;

    snd_seq_close(handle);

	return 0;
}

