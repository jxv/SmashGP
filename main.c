#include	<pspkernel.h>
#include	<pspctrl.h>
#include	<pspdisplay.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>

#include	<pspgu.h>

#include	"mp_sys.c"
#include	"mp_key.c"

#include	"c/Source/smdefs.c"
#include	"c/Source/smfuncs.c"

/* Define the module info section */
PSP_MODULE_INFO("SMASHGPSP 0.2B", 0, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

void moteur_jeu();

void default_settings() { //Réglages par défaut
	difficulte = dif_normale;
//	luminosite = min_luminosite;
	volmusique = vol_normal;
	maxsurvival = 0;
	transparency = mid_transparency;
}

void ajoutepersosurvival() {
	int a = mp_Rand(nb_defpersos);
	int rc = 20;

	while( (defperso[a].idicone == 0) && rc--) a = mp_Rand(nb_defpersos);

	perso[nb_persos].id = a;
	perso[nb_persos].coul = mp_Rand(3) + 1;
	perso[nb_persos].enjeu = false;

	ia[nb_persos].phase = 0;
	ia[nb_persos].stat = 0;
	ia[nb_persos].cible = -1;
	ia[nb_persos].ok = true;
	ia[nb_persos].air = false;
	ia[nb_persos++].tmsaut = 0;

	if(perso[0].id == a) afficheid = true;
}

void init_moteur() { //Initialisation du moteur, et chargement du jeu
	int i;

	pause = false;
	secoue = 0;

	i = ( (0xff * transparency) / 10) & 0xff;
	trans24 = ( (0xff - i) << 16) | ( (0xff - i) << 8) | (0xff - i);
	omtrans24 = (i << 16) | (i << 8) | i;

	for(i = 0; i < nb_explos; i++) explos[i].imag = 0;
	nb_explos = 0;

	for(i = 0; i < nb_projects; i++) project[i].port = 0;
	nb_projects = 0;

	for(i = 1; i <= nb_objets; i++) objet[i].ener = 0;
	nb_objets = 0;

	afficheid = false;
	for(i = 0; i < nb_joueurs; i++) {
		if( (type_jeu == type_temps) || ( (type_jeu == type_survival) && (i != 0) ) ) {
			joueur[i].vies = 1;
		} else joueur[i].vies = vies_jeu;
		joueur[i].kills = 0;
		joueur[i].morts = 0;
		joueur[i].scds = 0;
		joueur[i].score = 0;
		joueur[i].anim = 0;
		joueur[i].statappar = 0;

		perso[i].coul = i;
		perso[i].enjeu = false;
		perso[i].acti = -20;

		if(!joueur[0].cpu && (i != 0) && (perso[0].id == perso[i].id) ) afficheid = true;
		if(!joueur[1].cpu && (i != 1) && (perso[1].id == perso[i].id) ) afficheid = true;

		ia[i].phase = 0;
		ia[i].stat = 0;
		ia[i].cible = -1;
		ia[i].ok = true;
		ia[i].air = false;
		ia[i].tmsaut = 0;
	}

	loadarena(num_area);
	ReInitSound();

	changecamera(256, 392, zoommax); //Zoom maxi en entrée

	nb_persos = nb_joueurs;
	if(type_jeu == type_temps) {
		switch(long_jeu) {
		case 0: //3mn
			cond_timer = 10800;
			break;
		case 1: //5mn
			cond_timer = 18000;
			break;
		case 2: //10mn
			cond_timer = 36000;
			break;
		case 3: //15mn
			cond_timer = 54000;
			break;
		case 4: //20mn
			cond_timer = 72000;
			break;
		default: //30mn
			cond_timer = 108000;
		}
	} else {
		int j = 0;
		int rc = 20;

		cond_timer = 60;
		if(type_jeu == type_survival) {
			_bakdif_ = difficulte;
			difficulte = dif_difficile;

			for(i = 0; i < nb_defpersos; i++) {
				if(defperso[i].idicone != 0) j++;
			}
			while( (j < 5) && (j < nb_defpersos) ) { //Pas assez de persos chargés
				i = mp_Rand(nb_defpersos);

				while( (defperso[i].idicone != 0) && rc--) i = mp_Rand(nb_defpersos);

				testeperso(i);

				j++;
				rc = 20;
			}

			ajoutepersosurvival();
		}
	}

	for(i = 0; i < 5; i++) {
		cptreb[i] = true;
	}

	mort_subite = false;

//	db = 0;
//	db2 = 1;
	moteur_jeu();
}

void conditions_sortie() {
	int i, j, k;

	if(mort_subite) {
		j = nb_joueurs;

		for(i = 0; i < nb_joueurs; i++) {
			if(joueur[i].vies < 1) j--;
		}

		if( (j < 2) && cond_timer) cond_timer--;
	} else {
		switch(type_jeu) {
		case type_vies:
			j = nb_joueurs;

			for(i = 0; i < nb_joueurs; i++) {
				if(joueur[i].vies < 1) j--;
			}

			if( (j < 2) && cond_timer) cond_timer--;
			break;
		case type_temps:
			if(cond_timer) cond_timer--;
			break;
		case type_survival:
			if( (joueur[0].vies < 1) && cond_timer) cond_timer--;

			if(nb_persos < max_persos_) { //Ajout ennemi ??
				if( (mp_Rand(50) == 12) && (mp_Rand(30) == 17) ) ajoutepersosurvival(); //Oui
			}
		}

		if( (cond_timer < 1) && (type_jeu != type_survival) ) { //Fin du jeu, test mort subite
			j = false;
			k = 0;
			for(i = 1; i < nb_joueurs; i++) { //Recherche meilleur score
				if(joueur[i].score > joueur[k].score) k = i;
			}

			for(i = 0; i < nb_joueurs; i++) { //Recherche égalité
				if( (i != k) && (joueur[i].score == joueur[k].score) ) j = true;
			}
			if(j) { //On active la mort subite, tout ceux qui ont le même score que k
				for(i = 0; i < nb_joueurs; i++) {
					if(joueur[i].score == joueur[k].score) {
						joueur[i].vies = 1;

						perso[i].acti = -50;
					} else joueur[i].vies = 0;

					perso[i].enjeu = false;
				}

				cond_timer = 5;
				mort_subite = true;

				PlaySound(&sound[son_sdeath]);
			}
		}
	}
}

void jeu() {
	int i;

	timer3++; //Timers******************************************************************************************
	if(timer3 > 2) timer3 = 0;
	timer5++;
	if(timer5 > 4) timer5 = 0;

	for(i = 0; i < nb_explos; i++) { //Gestion des explosions
		if(explos[i].imag) {
			if(timer3 < explos[i].vite) explos[i].imag++;
		}
	}

	for(i = 0; i < nb_projects; i++) { //Gestion des projectiles
		if(project[i].port) gere_proj(i);
	}

	for(i = 1; i <= nb_objets; i++) { //Gestion des objets
		if( (objet[i].ener > 0) && !objet[i].porte) gere_objet(i);
	}

	for(i = 0; i < nb_persos; i++) { //Gestion des persos
		if(!perso[i].enjeu) { //Mort, attend de réapparaitre
			if(i < nb_joueurs) { //Joueurs
				if(joueur[i].vies > 0) {
					if(perso[i].acti > 0) { //Réapparait
						ajoute_perso(i, area[num_area].coordj[i][0], area[num_area].coordj[i][1] );
					} else perso[i].acti++;
				}
			} else { //Autres persos
				int j = mp_Rand(3) + 1;
				ajoute_perso(i, area[num_area].coordj[j][0], area[num_area].coordj[j][1] );
			}
		} else gere_perso(i);

		if(i < nb_joueurs) {
			if(joueur[i].statappar) majpfnuage(i);
		}
	}

	if(secoue) secoue--; //Screen shake

	gere_camera();

	if( (timer3 == 2) && (mp_Rand(50) == 46) ) { //Apparition objet
		i = mp_Rand(area[num_area].nb_zones);
		if(area[num_area].zone[i].xmax > area[num_area].zone[i].xmin) ajoute_objet(mp_Rand(nb_defobjets), area[num_area].zone[i].xmin + mp_Rand(area[num_area].zone[i].xmax - area[num_area].zone[i].xmin), area[num_area].zone[i].y - 5);
	}

	conditions_sortie();
}

static inline void main_flip() {
//	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	sceGuSwapBuffers();

	db = 1 - db;
}

void moteur_jeu() {
	area[num_area].zfbs = (area[num_area].zfbs * 904) >> 10;
	area[num_area].xbs = (area[num_area].xbs << 10) / 904;
	area[num_area].ybs = (area[num_area].ybs << 10) / 904;
	int wbs = 327680 / area[num_area].zfbs;
	int hbs = 245760 / area[num_area].zfbs;
	int a;

	PlaySound(&sound[son_applau02]);

//	db = 0;
	mp_InitGu(db);

	PreZoom( (unsigned short *)bmps[area[num_area].idfond].image);
	swizzletex( (unsigned short *)bmpbuffer, texturefond_, 512, 256);

	sceDisplayWaitVblankStart();
	temps = mp_TickCountGet();

	sceGuDisplay(GU_TRUE);

//	main_flip();

	while(cond_timer) { //Boucle principale
		if(timer5 == 0) { //1/60ème de seconde alloué
			temps = mp_TickCountGet() + framelength;
		} else temps += framelength;

		gestion_touches();

		a = xscreen / 5 + decbg;

		sceGuStart(GU_DIRECT, gu_list);
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
		sceGuTexMode(GU_PSM_5551, 0, 0, GU_TRUE); //L'image de fond est en mode swizzle
		sceGuDisable(GU_BLEND);

		sceGuTexImage(0, 512, 256, 512, (unsigned short *)texturefond_);
		struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
		vertices[0].u = 0;
		vertices[0].v = 0;
		vertices[0].x = -a;
		vertices[0].y = 0;
		vertices[0].z = 0;
		vertices[1].u = 512;
		vertices[1].v = 256;
		vertices[1].x = 724 - a;
		vertices[1].y = 272;
		vertices[1].z = 0;
		sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);

		if(area[num_area].xbs) {
			sceGuTexMode(GU_PSM_5551, 0, 0, 0); //Pas de swizzle pour l'écran :)

			sceGuTexImage(0, 512, 512, 512, (unsigned short *)pg_vramtop + (1 - db) * (FRAMESIZE >> 1) );
			struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
			vertices[0].u = 0;
			vertices[0].v = 0;
			vertices[0].x = area[num_area].xbs - a;
			vertices[0].y = area[num_area].ybs;
			vertices[0].z = 0;
			vertices[1].u = 480;
			vertices[1].v = 272;
			vertices[1].x = area[num_area].xbs - a + wbs;
			vertices[1].y = area[num_area].ybs + hbs;
			vertices[1].z = 0;
			sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
		}

		if(!pause) jeu();

		affichage();

		if(temps < mp_TickCountGet() ) { //Ouh la, frameskip
			if(!pause) jeu(); //2ème traitement

			gestion_touches(); //2ème deplacement

			temps += framelength; //Du coup, on synchronise sur une autre frame
		}

		sceDisplayWaitVblankStart(); //VWait

		main_flip(); //Flip***********************************************************************
	}

	if( (type_jeu == type_temps) && !mort_subite) {
		PlaySound(&sound[son_timeup]);
	} else {
		PlaySound(&sound[son_gameset]);

		if(type_jeu == type_survival) {
			difficulte = _bakdif_;
			nb_joueurs = 2;
		}
	}

//	while(temps + 50000 > mp_TickCountGet() ) updatesound();
	wait(5000);

	sceGuDisplay(GU_FALSE);

	ReInitSound();
#ifdef	__inc_musique__
	Player_Stop();
	MikMod_FreeSong(mf);

	strcpy(psp_path, game_path);
	strcat(psp_path, "menu.mod");
	mf = MikMod_LoadSong(psp_path, 128);
	if(mf != 0) {
		Player_Start(mf);

		mf->volume = volmusique * 24;
	}
	strcpy(psp_path, game_path);
#endif

//	db = 0;
	fin_jeu();
}

int main(int argc, char *argv[]) {
	sceDisplaySetMode(0, 480, 272);
	sceDisplaySetFrameBuf( (char *)pg_vramtop, 512, 1, 1);
//logwrite("pipo");
//logwrite("molo");
//wait(5000);
	mp_SetupCallbacks();

	texturefond_ = (unsigned short *)(pg_vramtop + (FRAMESIZE >> 1) * 3); //Après le depth_buffer / 3ème buffer (menus)
	decors_ = (unsigned short *)(texturefond_ + 512 * 256); //Après la texture de fond
//	decorsb_ = (unsigned short *)(decors_ + decors_width * 16);
	//textbuf_[0] = (unsigned short *)(decorsb_ + decors_width * 16); //Double espace temporaire textures
	textbuf_[0] = (unsigned short *)(decors_ + decors_width * 16); //Double espace temporaire textures
	textbuf_[1] = (unsigned short *)(textbuf_[0] + 512 * 256);
	edram_ptr = (unsigned short *)textbuf_[0];

	sceGuInit();

	if(argc > 0) strncpy( (char *)game_path, argv[0], strlen(argv[0]) - 9); //Path
	strcpy(psp_path, game_path);

	if(!loadprefs() ) default_settings();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	InitSound();

//	sceDisplaySetMode(0, 480, 272);
//	sceDisplaySetFrameBuf( (char *)pg_vramtop, 512, 1, 1);

#ifdef	__inc_musique__
	strcat(psp_path, "menu.mod");
	mf = MikMod_LoadSong(psp_path, 128);
	if(mf != 0) {
		Player_Start(mf);

		mf->volume = volmusique * 24;
	}
	strcpy(psp_path, game_path);
#endif

	assigne_images();
	assigne_menus();

	splash_screen();

	for(;;) main_menu();
}
