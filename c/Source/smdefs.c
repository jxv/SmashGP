//#define	__debug__
//char dummy[5000000];

//	#define	__inc_son__
	#ifdef	__inc_son__
		#define	__inc_musique__
	#endif

#define	screen_width	480
#define	screen_width_	479
#define	screen_height	272
#define	screen_height_	271
#define	fond_width		640
#define	fond_height	240

#define	dif_facile		0
#define	dif_normale		1
#define	dif_difficile		2

#define	min_luminosite	0
#define	mid_luminosite	4
#define	max_luminosite	9
#define	min_transparency		0
#define	mid_transparency		4
#define	max_transparency	9

#define	vol_nmusic		0
#define	vol_mini		1
#define	vol_normal		2
#define	vol_maxi		3

#define	max_mapw		100
#define	max_maph		100
#define	max_mapwb	180
#define	max_maphb		150

#define	type_temps		0
#define	type_vies			1
#define	type_survival		2

#define	max_long_jeu		5
#define	max_vies_jeu		6

#define	max_objets			100
#define	max_projects		100
#define	max_explos			200

#define	max_joueurs		4
#define	max_persos			32
#define	max_persos_		31
#define	max_bmps			10000
#define	max_paks			500
//#define	max_sons			200
#define	max_explosion		195
#define	max_perss			10000
#define	max_objes			500
#define	max_moves			5000
#define	max_clists			500
#define	max_defpersos		100
#define	max_defobjets		20
#define	max_defprojects		120
#define	max_areas			100
#define	zoommin			768

#define	max_gravite		10000
#define	max_poids			200


//#define	nb_explosions_		205
//
#define	nb_explos_protect	7
#define	explos_protectr		0
#define	explos_protectv		7
#define	explos_protectb		14
#define	explos_protectj		21
#define	nb_explos_rond		3
#define	explos_rondr		28
#define	explos_rondv		31
#define	explos_rondb		34
#define	explos_rondj		37
#define	nb_explos_fleche		8
#define	explos_fleche		40
#define	explos_cadrehg		48
#define	explos_cadrehd		49
#define	explos_cadrebg		50
#define	explos_cadrebd		51
#define	explos_btna		52
#define	explos_btnb		53
#define	explos_btndir		54
#define	explos_btnsel		55
#define	nb_explos_pm1		10
#define	explos_p1			56
#define	explos_m1			66
//
#define	explos_id1p			76
#define	explos_idcpr		77
#define	explos_idcpv		78
#define	explos_id2p			79
#define	explos_idcpb		80
#define	explos_idcpj		81
#define	nb_explos_ondechoc	4
#define	explos_ondechoc		82
#define	explos_maino		86
#define	explos_mainf		87
#define	explos_maind		88
//Trou 89
#define	explos_jetonr		90
#define	explos_jetonrcp		91
#define	explos_jetonv		92
#define	explos_jetonv2p		93
#define	explos_jetonb		94
#define	explos_jetonj		95
#define	explos_rtf			96
#define	explos_sd			97
#define	explos_rondclass		98
//Trou 99
#define	explos_cr0			100
#define	explos_cr1			101
#define	explos_cr2			102
#define	explos_cr3			103
#define	explos_cr4			104
#define	explos_cr5			105
//
#define	nb_explos_impact		2
#define	explos_impact1		106
#define	explos_impact2		108
#define	explos_impact3		110
#define	nb_explos_fumee		8
#define	explos_fumee		112
//
#define	nb_explos_mort		7
#define	explos_mortb1		120
#define	explos_mortb2		127
#define	explos_mortg1		134
#define	explos_mortg2		141
#define	explos_mortd1		148
#define	explos_mortd2		155
#define	nb_explos_fsmash	6
#define	explos_fsmash		162
#define	explos_fsmash2		168
#define	nb_explos_feject		5
#define	explos_feject		174
//
#define	nb_explos_explosion	16
#define	explos_explosion		179

#define	nb_perss_			10
//
#define	explos_etoile		0
#define	explos_bulscope		8
#define	explos_buraygun		9


#define	perso_rouge		0
#define	perso_vert			1
#define	perso_bleu			2
#define	perso_jaune		3

#define	move_normal		1
#define	move_smash		2
#define	move_prise			3
#define	move_priseec		4
#define	move_projectile		5
#define	move_tremblement	6
#define	move_eclair		7
//#define	move_deplacepad	62
#define	move_deplace		63
#define	move_combo		64

#define	objet_batonetoile		0
#define	objet_batte		1
#define	objet_marteau		2
#define	objet_pill			3
#define	objet_coeur		4
#define	objet_maximt		5
#define	objet_oeuf			6
#define	objet_etoile		7
#define	objet_raygun		8
#define	objet_scope		9
#define	objet_carapr		10
#define	objet_carapv		11
#define	objet_pillexp		12
#define	objet_oeufexp		13
#define	objet_mine			14
#define	objet_bobomb		15

#define	role_aucun			0
#define	role_explose		1
#define	role_boite			2
#define	role_caisse			3
#define	role_pokemon		4
#define	role_bumper		5
#define	role_carapr			6
#define	role_carapv			7
#define	role_bobomb		8
#define	role_mine			9
#define	role_batte			20
#define	role_batonetoile		21
#define	role_marteau		30
#define	role_pistolet		40
#define	role_fleur			50
#define	role_etoile			60
#define	role_coeur			61

#define	proj_etoile			0
#define	proj_scope			1
#define	proj_raygun		2

#include "..\autre.c"
#include "..\decors_.c"
#include "..\fonts.c"
//#include "c\loading.c"

#define	nb_sons			33
#define	son_cpasse			0
#define	son_smpasse		1
#define	son_cbloque		2
#define	son_cvent			3
#define	son_smvent		4
#define	son_protect		5
#define	son_protcass		6
#define	son_objpris			7
#define	son_objlance		8
#define	son_cbatet			9
#define	son_smbatet		10
#define	son_batte			11
#define	son_tombe			12
#define	son_mort			13
#define	son_menu			14
#define	son_0_			15
#define	son_1			16
#define	son_2			17
#define	son_3			18
#define	son_4			19
#define	son_5			20
#define	son_sdeath			21
#define	son_timeup			22
#define	son_gameset		23
#define	son_applau01		24
#define	son_applau02		25
#define	son_ohhh01		26
#define	son_ohhh02		27
#define	son_ohhh03		28
#define	son_carap			29
#define	son_explos			30
#define	son_raygun			31
#define	son_mine			32

//#define	palette_splash		0
//#define	palette_menus		1
//#define	palette_jeu			2

static unsigned int gu_list[262144] __attribute__ ( (aligned(64) ) );

struct Vertex {
	float u, v;
	float x, y, z;
};

#define	EDRAM_LIMIT		(0x44000000 + 0x180000)

unsigned short *edram_ptr; //En VRAM

unsigned short *texturefond_;
unsigned short *textbuf_[2];
//int dbt_ = 0;

unsigned short *decors_;
//unsigned short *decorsb_;

int convbmp(unsigned short *img, const int w, const int h);

int issized_e(const int numframe);

int issized_o(const int numframe);

//Structures*******************************************************************************************************************************************************************

typedef struct {
	int w, h, dx, dy;
	unsigned short *image;
} s_image;

typedef struct {
	int w, h, dx, dy;
	unsigned short *image;
	int sized;
} s_image_mp;

typedef struct {
	char name[16];
	unsigned short *image;
} s_bmp;

typedef struct {
	int x, y;
	int id, vite;
	int imag, zoom;
} s_explos;

typedef struct {
	int length;
	short *sound;
} s_sound;

typedef struct {
	short poids, nsauts;
	char name[32];
	short idcoups, idphoto, idicone;
	//
	s_sound son[128];
	short nsons;
	char pakname[16];
	short idpak;
	short required;
} s_defperso;

typedef struct {
	short poids, ener, atta, role;
	short idim, nrot, nani, idphoto;
	char name[32];
} s_defobjet;

typedef struct {
	short atta, smash, port;
	short idim, nani;
} s_defproject;

typedef struct {
	unsigned short *pakadr;
	short id;
} s_pak;

typedef struct {
	int x, y, dx, dy;
	int id, imag, cote;
	int coul;
	int acti, stat, bouge;
	int objet;
	int vite, stata;
	int sc, sontombe;
	int protect, invinc;
	int damage, shield;
	int enjeu, nsauts;
	int tape, pecho, dcoup, cpasse;
} s_perso;

typedef struct {
	int vies, kills, morts, scds, score;
	int anim, stata;
	int xappar, yappar, statappar;
	int cpu;
} s_joueur;

typedef struct {
	int x, y, dx, dy;
	int id, imag, cote;
	int stat, disp;
	int porte, prop;
	int ener, time;
} s_objet;

typedef struct {
	int x, y, dx, dy;
	int id, imag, cote;
	int prop, port;
} s_project;

typedef struct {
	short idim, nani;
	short loop, vite;
	short dx, dy;
	short dx2, dy2;
	short ax, ay;
	short type, atta;
	short idson;
} s_move;

typedef struct {
	short xmin, xmax;
	short y;
} s_zone;

typedef struct {
	short xbs, ybs, zfbs;
	short gravite;
	int oversize, w, h;
	short idfond;
	short coordj[max_joueurs][2];
	char backname[16];
	char modname[16];
	char fname[16];
	char name[32];
	int nb_zones;
	s_zone zone[16];
	//
	short required;
} s_area;

//Variables globales*******************************************************************************************************************************************************************

char g_string[256];

//GPDRAWSURFACE gpDraw[3]; //Hardware
int db = 0;
//int db2;

int timer3 = 0; //0 - 2 Jeu
//int timer5 = 0; //0 - 4
int pause = false;

int difficulte; //Réglages
int _bakdif_;
//int luminosite;
int transparency;
int trans24;
int omtrans24;
int volmusique;
int maxsurvival;

unsigned short *decors;
unsigned short *decorsb;
//short typedecor[max_decors];

unsigned short *font01;
unsigned short *font02;
unsigned short *font03;

unsigned short *image_splash;
unsigned short *image_main;
unsigned short *btn_1player;
unsigned short *btn_options;
unsigned short *btn_reset;
unsigned short *image_mode;
unsigned short *btn_time;
unsigned short *btn_lives;
unsigned short *btn_survival;
unsigned short *image_char;
unsigned short *image_area;
unsigned short *image_autre;
unsigned short *tit_options;
unsigned short *tit_results;
unsigned short *tit_credits;
unsigned short *btn_pmtime;
unsigned short *btn_pmstock;
unsigned short *btn_pmplayers;
unsigned short *btn_back;
unsigned short *btn_jhmn;
unsigned short *btn_jcpu;
unsigned short *btn_jna;
unsigned short *btn_left;
unsigned short *btn_right;

//char coul_blanc, coul_noir;
#define	coul_blanc		0xffff
#define	coul_noir		0x0000 | 0x8000
//char coul_jaunec;//, coul_rougesel;
#define	coul_jaunec	0x03ff | 0x8000

//long temps; //Temps avec sceKernelLibcGettimeofday()
//long framelength = 166;
#define	framelength	166

int type_jeu = type_temps;
int vies_jeu = 3;
int long_jeu = 0;
int cond_timer;
int mort_subite;

char cptreb[5];

int xscreen, yscreen;
int xmap, ymap;
int map_xmax, map_ymax;
int xscreenb, yscreenb;
int xmapb, ymapb;
int dxcam, dycam, dzoom;

int zoomf, zoominv;
int zoomfb, zoominvb;
int zoommax;
int xmaxm, ymaxm;
int decbg;

int disp_width, disp_height;
int disp_widthb, disp_heightb;
int scrw, scrh;
int scrwb, scrhb;
int secoue;

int afficheid;

int ba; //Boutons
int bb;
int bl, br;
int bsel, bstart;
int dhaut, ddhaut, tmsaut;
int dgauche, ddgauche;
int dbas, ddbas;
int ddroite, dddroite;

//int chatboard = false;
int ba2, dba2; //Boutons chatboard
int bb2, dbb2;
int bl2, br2;
//int bsel2;
int dhaut2, ddhaut2, tmsaut2;
int dgauche2, ddgauche2;
int dbas2, ddbas2;
int ddroite2, dddroite2;

int num_area = 0;

s_joueur joueur[max_joueurs];
int nb_joueurs = 2;
int nb_persos;

s_perso perso[max_persos];
s_objet objet[max_objets];
int nb_objets;

s_project project[max_projects];
int nb_projects;

short tabl[max_mapw][max_maph]; //Objets
short tablb[max_mapwb][max_maphb];

s_explos explos[max_explos];
int nb_explos;

//Définitions d'objets*******************************************************************************************************************************************************************

s_move move[max_moves];
int nb_moves = 0;

short clist[max_clists][3];
int nb_clists = 0;

s_defperso defperso[max_defpersos];
int nb_defpersos = 0;

s_defobjet defobjet[max_defobjets];
int nb_defobjets;// = 0;

s_defproject defproject[max_defprojects];
int nb_defprojects;// = 0;

s_area area[max_areas];
int nb_areas = 0;

s_bmp bmps[max_bmps];
int nb_bmps = 0;

s_pak pak[max_paks];
int nb_paks = 0;

s_image expl__[max_explosion]; //nb_explosions];
//int nb_explosions = nb_explosions_;

s_image_mp pers[max_perss];
int nb_perss = nb_perss_;

s_image obje[max_objes];

//#ifdef	__inc_son__
s_sound sound[nb_sons];
char sndname[nb_sons][16];
//int nb_sons = 1;
//#endif

void _assignation_explosions(int numexplos, int w, int h, int dx, int dy, unsigned short *imgexplos) {
	expl__[numexplos].w = w;
	expl__[numexplos].h = h;
	expl__[numexplos].dx = dx;
	expl__[numexplos].dy = dy;
	expl__[numexplos].image = (unsigned short *)imgexplos;

	if(!issized_e(numexplos) ) convbmp(imgexplos, w, h);
}

void _assignation_explosions_(int numexplos, int w, int h, int dx, int dy, unsigned short *imgexplos) {
	expl__[numexplos].w = w;
	expl__[numexplos].h = h;
	expl__[numexplos].dx = dx;
	expl__[numexplos].dy = dy;
	expl__[numexplos].image = (unsigned short *)imgexplos;
}

void _assignation_perss(int numcaisse, int w, int h, int dx, int dy, unsigned short *imgpers) {
	if(w) {
		pers[numcaisse].w = w;
		pers[numcaisse].h = h;
		pers[numcaisse].dx = dx;
		pers[numcaisse].dy = dy;
	}

	if(imgpers) pers[numcaisse].image = (unsigned short *)imgpers;
}

void _assignation_objes(int numtour, int w, int h, int dx, int dy, unsigned short *imgobje) {
	obje[numtour].w = w;
	obje[numtour].h = h;
	obje[numtour].dx = dx;
	obje[numtour].dy = dy;
	obje[numtour].image = (unsigned short *)imgobje;

	if(!issized_o(numtour) ) convbmp(imgobje, w, h);
}

#ifdef	__inc_son__
//void _assignation_sons(int numson, int length, unsigned short *son) {
//	sound[numson].length = length;
//	sound[numson].sound = (unsigned short *)son;
//}
#endif
