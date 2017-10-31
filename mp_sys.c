#define		FRAMESIZE		0x44000

#define		true			-1
#define		false			0

int decrand = 35;
long temps; //Temps avec sceKernelLibcGettimeofday()
int timer5 = 0; //0 - 4

struct timeval tempsstr;

int exit_callback(void) { //Exit callback
	sceKernelExitGame();

	return 0;
}

int mp_CallbackThread(SceSize args, void *argp) { //Callback thread
	int cbid = sceKernelCreateCallback("Exit Callback", (SceKernelCallbackFunction)exit_callback, NULL);

	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

int mp_SetupCallbacks(void) {
	int thid = sceKernelCreateThread("update_thread", mp_CallbackThread, 0x11, 0xFA0, 0, 0);

	if(thid >= 0) sceKernelStartThread(thid, 0, 0);

	return thid;
}

long mp_TickCountGet(void) {
	sceKernelLibcGettimeofday(&tempsstr, 0);
	return (tempsstr.tv_sec * 10000 + tempsstr.tv_usec / 100);
}

void mp_Srand(void) {
	decrand = mp_TickCountGet() & 0xff;
}

int mp_Rand(int max) { //Générateur pseudo-aléatoire (c) 2004 mATkEUpON
	if(max <= 0) return 0;
	int a = (temps & decrand) + decrand + max;
	decrand += max;

	while( (a >> 2) > max) a >>= 2;
	while(a >= max) a -= max;
	if(decrand > (timer5 + 169) ) decrand = max - a;
	return a;
}
