#ifdef	__inc_son__
//	#include "gpmm.h"
	#include <pspaudio.h>
	#include <pspaudiolib.h>

	#ifdef	__inc_musique__
		#include	"mikmod.h"

//		#include "C:\SrcSmash\c\Source\modplay\gpmodplay.h"
	#endif
#endif

#define	nb_mix		16
//#define	soundfreq		21484
//#define	sbufsize		4096
//#define	sbufmid		2048
//#define	sbufsize		2048
//#define	sbufmid		1024

#ifdef	__inc_son__
typedef struct {
	short *son;
	short *finson;
//	int nloops;
//	short *adrloop;
} s_mix;

s_mix mix[nb_mix];

//int audiohandle = -1;
//int audiothandle = -1;

//static short sbuffer[2][sbufmid];
//static short sbuffer[sbufmid];

	#ifdef	__inc_musique__
//MODPlay mp;
UNIMOD *mf = NULL;

void *__psp_argv_0;
	#endif

pspAudioCallback_t updatesound(unsigned short *buf, unsigned int reqn, void *pdata) {
	int i, voix;
	int sample;
	int *buf32 = (int *)( (int)buf | 0x40000000);

	for(i = 0; i < (reqn >> 1); i++) {
		if(mix[0].son < mix[0].finson) {
			sample = *mix[0].son++;
		} else sample = 0;
		for(voix = 1; voix < nb_mix; voix++) {
			if(mix[voix].son < mix[voix].finson) sample += *mix[voix].son++;
		}

		sample >>= 2;
		if(sample > 32767) sample = 32767;
		if(sample < -32768) sample = -32768;
		sample |= (sample << 16);
		*buf32++ = sample;
		*buf32++ = sample;
	}

	return 0;
}

pspAudioCallback_t updatesounddummy(unsigned short *buf, unsigned int reqn, void *pdata) { //Gain de temps + support fonctions sdk
	return 0;
}
/*
static void audiothread(void) {
	volatile int bufidx = 0;
	void *bufptr;
	
	for(;;) {
		bufptr = &sbuffer[bufidx];

		updatesound(bufptr, sbufmid);

		sceAudioOutputBlocking(audiohandle, 0x8000, bufptr);

		bufidx = 1 - bufidx;
	}

	sceKernelExitThread(0);
	return;
}
*/
	#ifdef	__inc_musique__
extern int _mm_errno;
extern BOOL _mm_critical;
extern char *_mm_errmsg[];

extern UWORD md_mode;
extern UBYTE md_reverb;
extern UBYTE md_pansep;

//void my_error_handler(void) {
//}
	#endif

void InitSound(void) {
	int i;

	#ifdef	__inc_musique__
//	MODPlay_Init2(&mp);
	//_mm_RegisterErrorHandler(my_error_handler);
	MikMod_RegisterAllLoaders();
	MikMod_RegisterAllDrivers();
	md_mode = DMODE_16BITS | DMODE_STEREO | DMODE_SOFT_SNDFX | DMODE_SOFT_MUSIC; 
	md_reverb = 0;
	md_pansep = 128;
	MikMod_Init();
	MikMod_SetNumVoices(-1, 8);

	pspAudioSetChannelCallback(1, (pspAudioCallback_t)updatesound, 0);

	for(i = 2; i < 8; i++) pspAudioSetChannelCallback(i, (pspAudioCallback_t)updatesounddummy, 0);
	#else
	pspAudioInit();
	pspAudioSetChannelCallback(0, (pspAudioCallback_t)updatesound, 0);

	for(i = 1; i < 8; i++) pspAudioSetChannelCallback(i, (pspAudioCallback_t)updatesounddummy, 0);
/*
	audiohandle = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, sbufmid, PSP_AUDIO_FORMAT_MONO);
	if(audiohandle < 0) return;

	audiothandle = sceKernelCreateThread("audiot01", (void*)&audiothread, 0x12, 0x10000, 0, NULL);
	if(audiothandle < 0) return;

	int a = sceKernelStartThread(audiothandle, 0, NULL);
	if(a != 0) {
		sceKernelDeleteThread(audiothandle);
		audiothandle = -1;
	}
*/
	#endif
}

void ReInitSound(void) {
//	short *sbuf16 = sbuffer[0];
//	while(sbuf16 < (sbuffer[0] + sbufsize) ) *sbuf16++ = 0;

	/* start playing buffer */
//	GpPcmPlay( (unsigned short *)sbuffer, sbufsize * 2, 1);
//	GpPcmLock( (unsigned short *)sbuffer, (int *)&sbufidx, (unsigned int *)&sbufpos);
}

void PlaySoundFX(s_sound *son, int nb) { //Son mixé
//	register i;
	int i;

	for(i = 0; i < nb_mix; i++) {
		if(mix[i].son >= mix[i].finson) { //Emplacement vide
			mix[i].son = (short *)son->sound;
			mix[i].finson = (short *)son->sound + (son->length >> 1);
//			if(nb > 0) {
//				mix[i].nloops = nb;
//				mix[i].adrloop = (unsigned short *)son->sound;
//			} else {
//				mix[i].nloops = 0;
//				mix[i].adrloop = 0;
//			}

			return;
		}
	}

//	PlaySound(son);
}

static inline void PlaySound(s_sound *son) { //Son joué par le SDK
//	GpPcmPlay( (unsigned short *)son->sound, son->length, 0);
	PlaySoundFX(son, 0);
}
#else
void updatesound(void) {
}

void InitSound(void) {
}

void ReInitSound(void) {
}

void PlaySound(s_sound *son) { //Son joué par le SDK
}

void PlaySoundFX(s_sound *son, int nb) { //Son mixé
}
#endif
