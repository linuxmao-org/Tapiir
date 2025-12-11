#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "MTD.hxx"
#include "MTDdecl.hxx"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
#include <jack/jack.h>
}

jack_port_t *input_port[2];
jack_port_t *output_port[2];

int
process (jack_nframes_t nframes, void *arg)
{
	jack_default_audio_sample_t  *in[2],*out[2];
	jack_nframes_t i;
	
	out[0] = (jack_default_audio_sample_t *) 
		jack_port_get_buffer (output_port[0], nframes);
	out[1] = (jack_default_audio_sample_t *)
		jack_port_get_buffer (output_port[1], nframes);

	in[0] = (jack_default_audio_sample_t *)
		jack_port_get_buffer (input_port[0], nframes);
	in[1] = (jack_default_audio_sample_t *)
		jack_port_get_buffer (input_port[1], nframes);

 ((MTD<float,float>*)mtd)->
		ApplyNonInterleaved(in,out,nframes);

	return 0;      
}

int
bufsize (jack_nframes_t nframes, void *arg)

{
	printf ("the maximum buffer size is now %lu\n", nframes);
	return 0;
}

int
srate (jack_nframes_t nframes, void *arg)

{
	printf ("the sample rate is now %lu/sec\n", nframes);
	return 0;
}

extern void run_gui(void);

static void help(char* program)
{
	int k;
	printf(
"Usage: %s [OPTION]...                                                      \n"
"-h,--help                   help                                           \n"
"-C,--cconnect <LEFT,RIGHT>  jack-connect to capture client   default none  \n"
"-P,--pconnect <LEFT,RIGHT>  jack-connect to playback client  default none  \n"
"-a,--alsaconnect            jack-connect to alsa_pcm                       \n",
program
);
}

int run_jack(int argc, char *argv[])
{
	jack_client_t *client;
	int morehelp = 0;
	char* pclientL = NULL;
	char* pclientR = NULL;
	char* cclientL = NULL;
	char* cclientR = NULL;

	struct option long_option[] =
	{
		{"help", 0, NULL, 'h'},
		{"pconnect", 1, NULL, 'P'},
		{"cconnect", 1, NULL, 'C'},
		{"alsaconnect", 0, NULL, 'a'},
		{NULL, 0, NULL, 0},
	};

	while (1) {
		int c;
		if ((c = getopt_long(argc, argv, "hP:C:a", long_option, NULL)) < 0)
			break;
		switch (c) {
		case 'h':
			morehelp++;
			break;
		case 'P':
			pclientL = strdup(optarg);
			pclientR = strstr(pclientL,",");
			if (pclientR) *pclientR++=0;
			else pclientR = pclientL;
			break;
		case 'C':
			cclientL = strdup(optarg);
			cclientR = strstr(cclientL,",");
			if (cclientR) *cclientR++=0;
			else cclientR = cclientL;
			break;
		case 'a':
			cclientL = "alsa_pcm:capture_1";
			cclientR = "alsa_pcm:capture_2";
			pclientL = "alsa_pcm:playback_1";
			pclientR = "alsa_pcm:playback_2";
			break;
		}
	}

	if (morehelp) {
		if (!strcmp(argv[0],"--jack"))
		{
			help("tapiir --jack");
		}else{
			help("tapiir_jack");
		}
		return 0;
	}

	int size = 0x01000000; // 16 mb

	try {
		mtd = new MTD<float,float>(size,2,6);
	} catch (...) {
		printf(
		"Error: Not enough memory to create a %d samples multitap delay\n",size);
		exit(0);
	}
	
	mtd->SetGain(mtd->ChannelID(0),mtd->ChannelID(0),1);
	mtd->SetGain(mtd->ChannelID(0),mtd->OverallID(),1);

	mtd->SetGain(mtd->ChannelID(1),mtd->ChannelID(1),1);
	mtd->SetGain(mtd->ChannelID(1),mtd->OverallID(),1);

	if ((client = jack_client_new ("tapiir")) == 0) {
		fprintf (stderr, "jack server not running?\n");
		return 1;
	}

	jack_set_process_callback (client, process, 0);
	jack_set_buffer_size_callback (client, bufsize, 0);
	jack_set_sample_rate_callback (client, srate, 0);

	printf ("engine sample rate: %lu\n", jack_get_sample_rate (client));

	input_port[0] = jack_port_register (client, "input_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	input_port[1] = jack_port_register (client, "input_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	output_port[0] = jack_port_register (client, "output_1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	output_port[1] = jack_port_register (client, "output_2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
	}

	printf ("client activated\n");

	if (cclientL &&
		jack_connect (client, cclientL, jack_port_name (input_port[0]))) {
		fprintf (stderr, "cannot connect left input port\n");
	} 
	if (cclientR &&
		jack_connect (client, cclientR, jack_port_name (input_port[1]))) {
		fprintf (stderr, "cannot connect right input ports\n");
	} 

	if (pclientL &&
		jack_connect (client, jack_port_name (output_port[0]), pclientL)) {
		fprintf (stderr, "cannot connect left output ports\n");
	} 
	if (pclientR &&
		jack_connect (client, jack_port_name (output_port[1]), pclientR)) {
		fprintf (stderr, "cannot connect right output ports\n");
	} 

	run_gui();

	jack_client_close (client);
	exit (0);
}

