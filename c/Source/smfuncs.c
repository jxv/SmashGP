//#define	div3(nb)		( ( (nb) * 21846) >> 16)
//#define	div5(nb)		( ( (nb) * 13107) >> 16)

#define	mulzoom(nb)	( ( (nb) * zoomf) >> 10)
#define	divzoom(nb)	( ( (nb) * zoominv) >> 10)
#define	mulzoomb(nb)	( ( (nb) * zoomfb) >> 10)
#define	divzoomb(nb)	( ( (nb) * zoominvb) >> 10)

#define	RGB(r, g, b)	( ( ( ( (b) >> 3) & 0x1F) << 10) | ( ( ( (g) >> 3) & 0x1F) << 5) | ( ( ( (r) >> 3) & 0x1F) << 0) | 0x8000)

//#define	COUL_TRANS	0x7c1f
#define	COUL_TRANS	0x4210

char game_path[256] = "ms0:/";
char psp_path[256];

unsigned char bmpbuffer[0x70800] __attribute__ ( (aligned (16) ) ); //Assez pour le fond zoomé :)

static inline void secoueecran(int force) { //Force de 1 à 9
	secoue = 2 * force + 2;
}

//char trouvecoul(char cmpr, char cmpv, char cmpb) {
//	int i, j, k, l;
//	char coul = 0;
//	int a = 195075;

//	for(i = 0; i < 255; i++) { //Recherche couleurs base
//		j = ( (PAL[palette_jeu][i] & 0xF800) >> 8) - cmpr;
//		k = ( (PAL[palette_jeu][i] & 0x7C0) >> 3) - cmpv;
//		l = ( (PAL[palette_jeu][i] & 0x3E) << 2) - cmpb;
//		j = j * j + k * k + l * l;
//		if(j < a) {
//			a = j;
//			coul = i;
//		}
//	}

//	return coul;
//}

void swizzletex(unsigned short *src, unsigned short *dst, const int w, const int h) {
	int i, j;
	int k, l;

	for(j = 0; j < (h >> 3); j++) {
		for(i = 0; i < (w >> 3); i++) {
			for(l = 0; l < 8; l++) {
				for(k = 0; k < 8; k++) {
					if( ( (i * 8 + k) < w) && ( (j * 8 + l) < h) ) {
						*dst++ = *(src + (j * 8 + l) * w + i * 8 + k);
					} else *dst++ = COUL_TRANS;
				}
			}
		}
	}
}

int makesprite(unsigned short *src, unsigned short *dst, const int w, const int h, const int cote) {
	int i, j;
	int w2 = 1;

	while(w2 < w) w2 <<= 1;
	if(w2 > 512) return w2;

	int dech = w2 - w;
	if(cote > 0) {
		j = h;
		while(j--) {
			i = w;
			while(i--) *dst++ = *src++;
			dst += dech;
		}
	} else {
		j = h;
		while(j--) {
			i = w;
			while(i--) *dst++ = *(src + i);
			src += w;
			dst += dech;
		}
	}

	edram_ptr = (unsigned short *)( ( ( (int)dst + 15) >> 4) * 16);
	if( (int)edram_ptr > EDRAM_LIMIT) {
		edram_ptr = (unsigned short *)textbuf_[0];
		sceGuFinish();
		sceGuSync(0, 0);
		sceGuStart(GU_DIRECT, gu_list);
	}
	return w2;
}

int makespritesolid(unsigned short *src, unsigned short *dst, const int w, const int h, const unsigned short coul, const int cote) {
	int i, j;
	int w2 = 1;

	while(w2 < w) w2 <<= 1;
	if(w2 > 512) return w2;

	int dech = w2 - w;
	if(cote > 0) {
		j = h;
		while(j--) {
			i = w;
			while(i--) {
				if(*src++ != COUL_TRANS) {
					*dst++ = coul;
				} else *dst++ = COUL_TRANS;
			}
			dst += dech;
		}
	} else {
		j = h;
		while(j--) {
			i = w;
			while(i--) {
				if(*(src + i) != COUL_TRANS) {
					*dst++ = coul;
				} else *dst++ = COUL_TRANS;
			}
			src += w;
			dst += dech;
		}
	}

	edram_ptr = (unsigned short *)( ( ( (int)dst + 15) >> 4) * 16);
	if( (int)edram_ptr > EDRAM_LIMIT) {
		edram_ptr = (unsigned short *)textbuf_[0];
		sceGuFinish();
		sceGuSync(0, 0);
		sceGuStart(GU_DIRECT, gu_list);
	}
	return w2;
}

int convbmp(unsigned short *img, const int w, const int h) {
	int h2 = ( (h + 3) >> 2) * 4;
	int i, j;
	unsigned char *readbuf = (unsigned char *)bmpbuffer;

	if( (img == dumimage) || (img == 0) ) return false;

	memcpy(bmpbuffer, img, w * h2 * 2); //Sauvegarde de l'original

	for(j = 0; j < h; j++) { //Conversion
		for(i = 0; i < w; i++) *img++ = pal[readbuf[(i * h2 + h - j - 1) * 2]];
	}

	return true;
}

int issized(const int id, const int numframe) {
	int i, j;
	int l;

	for(i = 0; i < 90; i++) {
		if(move[defperso[id].idcoups + i].nani >= 0) { //Images mouvement
			for(j = 0; j <= move[defperso[id].idcoups + i].nani; j++) {
				l = move[defperso[id].idcoups + i].idim + j;
				if( (pers[l].image == pers[numframe].image) && pers[l].sized) return true;
			}
		}
	}

	return false;
}

int issized_p(const int id, const int numframe) {
	int i;

	for(i = 0; i <= defproject[id].nani; i++) {
		if( (pers[defproject[id].idim + i].image == pers[numframe].image) && pers[defproject[id].idim + i].sized) return true;
	}

	return false;
}

int issized_e(const int numframe) {
	int i;

	for(i = 0; i < numframe; i++) {
		if(expl__[i].image == expl__[numframe].image) return true;
	}

	return false;
}

int issized_o(const int numframe) {
	int i;

	for(i = 0; i < numframe; i++) {
		if(obje[i].image == obje[numframe].image) return true;
	}

	return false;
}

void wait(long time);

void construit_carte();

void mortperso(int i);

void creeexplos(int x, int y, int imag, int imag2, int vite);

void creeexplosdg(int x, int y, int atta, int prop);

void creeproject(int x, int y, int dx, int dy, int id, int prop);

void baisseenerobjet(int i, int atta);

void freineperso(int i, int cote, int facteur);

void assommeperso(int i, int duree);

int testeobjet(int i);

int testeutilobjet(int i, int x, int ax, int ay);

int testecoup(int i, int idcoup, int smash);

int testecolperso(int exc, int x, int y);

int degatperso(int i, int x, int y, int ax, int ay, int atta, int smash);

int ajoute_objet(int id, int x, int y);

int animperso(int i);

//void logwrite(const char *entry);

//

#include "smsound.c"
#include "sminput.c"
#include "smia.c"
#include "smgraph.c"
#include "smfiles.c"
#include "smmenus.c"
#include "smgestion.c"
#include "sminit.c"

int ylog = 0;

void logwrite(const char *entry) {
	StringTransBlit(0, 2, ylog, (char *)entry);
	StringTransBlit(1, 2, ylog, (char *)entry);
	StringTransBlit(2, 2, ylog, (char *)entry);

	if(ylog > 240) { //Scroll
		int i;

		mp_FontBlit(0, 0, 0, 480, 252, (unsigned short *)pg_vramtop + (FRAMESIZE >> 1) + 12 * 512, 512);
		for(i = 0; i < 12; i++) {
			hline(0, 0, 252 + i, 480, 0x0000);
		}
		mp_FontBlit(1, 0, 0, 480, 272, (unsigned short *)pg_vramtop, 512);
		mp_FontBlit(2, 0, 0, 480, 272, (unsigned short *)pg_vramtop, 512);
	} else ylog += 12;
}

void creeexplos(int x, int y, int imag, int imag2, int vite) {
//	register i = 0;
	int i = 0;

	while(explos[i].imag) i++;

	if(i < max_explos) {
		explos[i].id = imag + imag2;
		explos[i].x = x;
		explos[i].y = y;
		explos[i].vite = vite;
		explos[i].imag = -imag2;
		explos[i].zoom = true;

		if(i >= nb_explos) nb_explos = i + 1;
	}
}

void creeexplosnz(int x, int y, int imag, int imag2, int vite) {
//	register i = 0;
	int i = 0;

	while(explos[i].imag) i++;

	if(i < max_explos) {
		explos[i].id = imag + imag2;
		explos[i].x = x;
		explos[i].y = y;
		explos[i].vite = vite;
		explos[i].imag = -imag2;
		explos[i].zoom = false;

		if(i >= nb_explos) nb_explos = i + 1;
	}
}

void creeexplosdg(int x, int y, int atta, int prop) {
	int i;
	int a, b, c;
	int cx, cy;

	for(i = 0; i < nb_persos; i++) {
		if(perso[i].enjeu) {
			a = (perso[i].x - x) >> 10;
			if(a > 0) {
				cx = 1;
			} else cx = -1;
			a *= a;
			b = (perso[i].y - y) >> 10;
			if(b > 0) {
				cy = 1;
			} else cy = -1;
			b *= b;
			c = a + b;

			if(c < atta) { //A portée
				degatperso(i, (perso[i].x + x) >> 1, (perso[i].y + y) >> 1, (atta - b) * cx, (atta - a) * cy, atta - c, true);
				if(prop >= 0) {
					perso[i].invinc = 1;
					perso[i].tape = prop;
				}
//				sprintf(g_string, "%d %d %d", (atta - b) * cx, (atta - a) * cy, atta - (c >> 1) );
//				if(db == 0) StringTransBlit(2, 20, 20, g_string);
//				if(db == 1) StringTransBlit(0, 20, 20, g_string);
//				if(db == 2) StringTransBlit(1, 20, 20, g_string);
//				wait(2000);
			}
		}
	}

	a = atta >> 8;
	b = atta >> 7;
	c = b >> 1;

	for(i = 0; i < a; i++) {
//		cx = GpRandN(5);
		cx = mp_Rand(5);
//		creeexplos( (x >> 10) + GpRandN(b) - c, (y >> 10) + GpRandN(b) - c, explos_explosion + cx, nb_explos_explosion - cx, 1);
		creeexplos( (x >> 10) + mp_Rand(b) - c, (y >> 10) + mp_Rand(b) - c, explos_explosion + cx, nb_explos_explosion - cx, 1);
	}

	secoueecran(atta >> 10);

	if(atta < 2500) {
		PlaySoundFX(&sound[son_explos], 0);
	} else PlaySound(&sound[son_explos]);
}

void creeproject(int x, int y, int dx, int dy, int id, int prop) {
//	register i = 0;
	int i = 0;

	while(project[i].port) i++;

	if(i < max_projects) {
		project[i].id = id;
		project[i].x = x;
		project[i].y = y;
		project[i].dx = dx;
		project[i].dy = dy;
		project[i].prop = prop;

		project[i].imag = defproject[id].idim;
		project[i].cote = perso[prop].cote;
		project[i].port = defproject[id].port;

		if(i >= nb_projects) nb_projects = i + 1;
	}
}

int testecolperso(int exc, int x, int y) {
//	register j;
	int j;
	int k, l;

	for(j = 0; j < nb_persos; j++) {
		if( (j != exc) && perso[j].enjeu && (perso[j].invinc < 1) ) {
			if(perso[j].cote > 0) {
				k = perso[j].x - (pers[perso[j].imag].dx << 10);
				l = k + (pers[perso[j].imag].w << 10);
			} else {
				l = perso[j].x + (pers[perso[j].imag].dx << 10);
				k = l - (pers[perso[j].imag].w << 10);
			}

			if( (x >= k) && (x <= l) && (y >= (perso[j].y - (pers[perso[j].imag].h << 10) ) ) && (y <= perso[j].y) ) return j + 1; //Ds le cadre
		}
	}

	return 0;
}

void assommeperso(int i, int duree) {
	perso[i].acti = -duree;
	perso[i].stat = 0;
	perso[i].stata = 0;

	if(perso[i].objet) { //On lache l'objet
		objet[perso[i].objet].dx = perso[i].dx >> 1;
		objet[perso[i].objet].dy = perso[i].dy >> 1;
		objet[perso[i].objet].porte = false;
		objet[perso[i].objet].prop = i;

		perso[i].objet = 0;
	}
}

int degatperso(int i, int x, int y, int ax, int ay, int atta, int smash) {
	if(perso[i].invinc) return true;

	int touche = false;
	int j = ( (atta * ( (1 << 14) + perso[i].damage) ) >> 14) - defperso[perso[i].id].poids;
	if(j < (atta >> 2) ) j = atta >> 2;
	int k = j >> 10;
	if(k > 2) k = 2;

	if(perso[i].protect) { //Baisse de bouclier
		perso[i].shield -= (j >> 2);
		if(perso[i].shield < 1) { //Cassage de bouclier
			perso[i].protect = 0;
			assommeperso(i, j >> 3);

			perso[i].damage += atta;
			perso[i].dx += ax;

			PlaySoundFX(&sound[son_protcass], 0);
			return true;
		} else PlaySoundFX(&sound[son_cbloque], 0);

		creeexplos(x >> 10, y >> 10, explos_impact1 + k * 2, nb_explos_impact, 2);
	} else { //Test sur l'image du perso
		touche = true;
		creeexplos(x >> 10, y >> 10, explos_impact1 + k * 2, nb_explos_impact, 2);

		if(smash) {
			PlaySoundFX(&sound[son_smpasse], 0);
			PlaySoundFX(&defperso[perso[i].id].son[2], 0);
		} else {
			PlaySoundFX(&sound[son_cpasse], 0);
			PlaySoundFX(&defperso[perso[i].id].son[1], 0);
		}
	}

	if(touche) {
		perso[i].damage += atta;
		if( (j > 1000) || smash) { //C'est fort, on part
			perso[i].dx += (ax * (j >> 10) ); /// atta);
			perso[i].dy += (ay * (j >> 10) ); /// atta);

			if(j > 2000) PlaySoundFX(&sound[son_ohhh01], 0);

//			if(perso[i].acti >= 0) {
//				perso[i].stat = 0;
//				perso[i].stata = 0;
//			}

			assommeperso(i, j >> 7); //8);
		} else {
			perso[i].dx += ax; //Pas très fort

//		if(perso[i].acti >= 0) { //Arrêt action
			perso[i].acti = 15;
			perso[i].stat = 0;
			perso[i].stata = 0;
		}

		return true;
	} else return false;
}

int testecoup(int i, int idcoup, int smash) {
	int j;
	int x = perso[i].x + perso[i].cote * (move[idcoup].dx << 10);
	int y = perso[i].y - (move[idcoup].dy << 10);
	int touche = false;

	if( (j = testecolperso(i, x, y) ) ) {
		if(perso[j - 1].dcoup != i * 90 + idcoup) {
			if(degatperso(j - 1, x, y, perso[i].cote * move[idcoup].ax, move[idcoup].ay, move[idcoup].atta, smash) ) {
				perso[j - 1].invinc = 1;
				perso[j - 1].tape = i;
				perso[j - 1].dcoup = i * 90 + idcoup;
				touche = true;
			}
		}
	}

	if(move[idcoup].type < move_combo) {
		if(move[idcoup].dx2 || move[idcoup].dy2) { //2ème point
			x = perso[i].x + perso[i].cote * (move[idcoup].dx2 << 10);
			y = perso[i].y - (move[idcoup].dy2 << 10);
			if( (j = testecolperso(i, x, y) ) ) {
				if(perso[j - 1].dcoup != i * 90 + idcoup) {
					if(degatperso(j - 1, x, y, perso[i].cote * move[idcoup].ax, move[idcoup].ay, move[idcoup].atta, smash) ) {
						perso[j - 1].invinc = 1;
						perso[j - 1].tape = i;
						perso[j - 1].dcoup = i * 90 + idcoup;
						touche = true;
					}
				}
			}
		}
	}

	return touche;
}

int testeobjet(int i) {
//	register j;
	int j;
	int k; //, l;

	for(j = 1; j <= nb_objets; j++) {
		if( (objet[j].ener > 0) && (objet[j].porte == 0) ) {
			if( (perso[i].y >> 14) == (objet[j].y >> 14) ) { //Même ligne
				k = (perso[i].x - objet[j].x) >> 10;
//				l = obje[objet[j].imag].w >> 1;
				if(k * k < 256) return j; //Assez près
			}
		}
	}

	return 0;
}

void baisseenerobjet(int i, int atta) {
	int j;

	objet[i].ener -= atta;
	if(objet[i].ener < 1) { //Disparition objet
		for(j = 0; j < 31; j += 3) { //Fumée
//			creeexplos( (objet[i].x >> 10) + j - 15, (objet[i].y >> 10) + GpRandN(30) - 15, explos_fumee, nb_explos_fumee, 1);
			creeexplos( (objet[i].x >> 10) + j - 15, (objet[i].y >> 10) + mp_Rand(30) - 15, explos_fumee, nb_explos_fumee, 1);
		}
	} else {
		if( (objet[i].prop >= 0) || objet[i].stat) {
			switch(defobjet[objet[i].id].role) {
			case role_explose:
			case role_bobomb:
//			case role_mine:
				creeexplosdg(objet[i].x, objet[i].y, defobjet[objet[i].id].atta, objet[i].prop);

				baisseenerobjet(i, 30000);
				break;
			case role_boite:
			case role_caisse:
//				ajoute_objet(GpRandN(nb_defobjets), objet[i].x >> 14, (objet[i].y >> 14) - 1);
				ajoute_objet(mp_Rand(nb_defobjets), objet[i].x >> 14, (objet[i].y >> 14) - 1);

				baisseenerobjet(i, 30000);
				break;
			case role_pokemon:
				break;
			}
		}
	}
}

void freineperso(int i, int cote, int facteur) {
	perso[i].dx -= ( (cote * (max_poids - defperso[perso[i].id].poids) * facteur) >> 1);
	if(cote * perso[i].dx < 0) perso[i].dx = 0; //Fin
}

void mortperso(int i) {
	int j;
	int xe, ye;
	int dir = 0;

	if(perso[i].tape >= 0) { //On l'a fraggé
		if(perso[i].tape < nb_joueurs) {
			joueur[perso[i].tape].kills++;
			joueur[perso[i].tape].score++;
			joueur[perso[i].tape].anim = explos_p1;
			joueur[perso[i].tape].stata = 0;
		}

		PlaySoundFX(&sound[son_applau01], 0);
	} else { //Suicide
		if(i < nb_joueurs) {
			joueur[i].scds++;
			joueur[i].score--;
		}
	}
	if(i < nb_joueurs) {
		joueur[i].morts++;
		joueur[i].score--;
		joueur[i].anim = explos_m1;
		joueur[i].stata = 0;

//		if(mort_subite || (type_jeu == type_vies) || ( (type_jeu == type_survival) && (i == 0) ) ) joueur[i].vies--;
		if(mort_subite || (type_jeu == type_vies) || (type_jeu == type_survival) ) joueur[i].vies--;
	}

	perso[i].enjeu = false;
	perso[i].acti = -200;

	ye = divzoom( (perso[i].y >> 10) - yscreen);
	if(ye < 11) {
//		ye = 11;
		dir = 0;
	} else {
		if(ye > screen_height) {
			ye = screen_height - 11;
			dir = 1;
		}
	}
	xe = divzoom( (perso[i].x >> 10) - xscreen);
	if(xe < 0) {
		xe = 11;
		dir = 2;
	} else {
		if(xe > screen_width) {
			xe = screen_width - 11;
			dir = 3;
		}
	}

	if(dir) { //Explosions sortie perso
		dir = (dir - 1) * (nb_explos_mort * 2);
		for(j = 0; j < 10; j++) {
//			creeexplosnz(xe + GpRandN(20) - 10, ye + GpRandN(20) - 10, explos_mortb1 + dir + GpRandN(nb_explos_mort), nb_explos_mort, 2);
			creeexplosnz(xe + mp_Rand(20) - 10, ye + mp_Rand(20) - 10, explos_mortb1 + dir + mp_Rand(nb_explos_mort), nb_explos_mort, 2);
		}

		PlaySound(&sound[son_mort]);
		secoueecran(9);
	} else PlaySound(&defperso[perso[i].id].son[3]); //Sortie par en-haut
}

void posepfnuage(int x, int y) {
	tabl[x][y] = decor_nuage;
	tabl[x - 1][y] = decor_nuage;
	tabl[x + 1][y] = decor_nuage;
}

void majpfnuage(int i) {
	if(joueur[i].statappar-- < 40) { //Clignotement
		if(joueur[i].statappar == (joueur[i].statappar >> 1) * 2) { //Disparait
			tabl[joueur[i].xappar][joueur[i].yappar] = 0;
			tabl[joueur[i].xappar - 1][joueur[i].yappar] = 0;
			tabl[joueur[i].xappar + 1][joueur[i].yappar] = 0;
		} else { //Apparait
			tabl[joueur[i].xappar][joueur[i].yappar] = decor_nuage;
			tabl[joueur[i].xappar - 1][joueur[i].yappar] = decor_nuage;
			tabl[joueur[i].xappar + 1][joueur[i].yappar] = decor_nuage;
		}
	}
}

int ajoute_perso(int idj, int x, int y) {
	if(!perso[idj].enjeu) {
		if(idj < nb_joueurs) {
			joueur[idj].xappar = x;
			joueur[idj].yappar = y;
			joueur[idj].statappar = 200;
			posepfnuage(x, y);
		}

		perso[idj].x = x << 14;
		perso[idj].y = (y - 1) << 14;

		perso[idj].imag = defperso[perso[idj].id].idcoups;
		perso[idj].vite = 4000 - defperso[perso[idj].id].poids * 10;
		perso[idj].nsauts = defperso[perso[idj].id].nsauts;
		perso[idj].imag = move[defperso[perso[idj].id].idcoups].idim;
		perso[idj].shield = ( (pers[perso[idj].imag].w * pers[perso[idj].imag].h) >> 10) * 2048 + 3071;

		perso[idj].dx = 0;
		perso[idj].dy = 0;
		perso[idj].cote = 1;
		perso[idj].acti = 0;
		perso[idj].stat = 0;
		perso[idj].bouge = false;
		perso[idj].objet = 0;
		perso[idj].stata = 0;
		perso[idj].sc = false;
		perso[idj].sontombe = true;
		perso[idj].protect = false;
		perso[idj].invinc = 150;
		if(mort_subite) {
			perso[idj].damage = 64000;
		} else perso[idj].damage = 0;
		perso[idj].tape = -1;
		perso[idj].pecho = -1;
		perso[idj].dcoup = 0;
		perso[idj].cpasse = false;

		perso[idj].enjeu = true;

		return true;
	} else return false;
}

int ajoute_objet(int id, int x, int y) {
	int i = 1;

	while( (objet[i].ener > 0) && (i < max_objets) ) i++;

	if(i < max_objets) {
		objet[i].id = id;
		objet[i].x = x << 14;
		objet[i].y = y << 14;

		objet[i].imag = defobjet[id].idim;
		objet[i].ener = defobjet[id].ener;

		objet[i].dx = 0;
		objet[i].dy = 0;
		objet[i].cote = 1;
		if(defobjet[id].role == role_pistolet) {
			if(defobjet[id].atta < 500) {
//				objet[i].stat = 30 + GpRandN(20);
				objet[i].stat = 30 + mp_Rand(20);
//			} else objet[i].stat = 10 + GpRandN(10);
			} else objet[i].stat = 10 + mp_Rand(10);
		} else objet[i].stat = 0;
		objet[i].disp = true;
		objet[i].porte = false;
		objet[i].prop = -1;
//		objet[i].time = 1000 + GpRandN(10) * 50;
		objet[i].time = 1000 + mp_Rand(10) * 50;

		if(i > nb_objets) nb_objets = i;
		return i;
	} else return 0;
}

int testeutilobjet(int i, int x, int ax, int ay) {
	int j;
	int k, l;
	int atta = 0;

	if(ax || ay) atta = defobjet[objet[x].id].atta;

	if(objet[x].cote > 0) {
		k = objet[x].x - (obje[objet[x].imag].dx << 10);
		l = k + (obje[objet[x].imag].w << 10);
	} else {
		l = objet[x].x + (obje[objet[x].imag].dx << 10);
		k = l - (obje[objet[x].imag].w << 10);
	}

	if( (j = testecolperso(i, k, objet[x].y) ) ) { //Gauche
		if( (i >= 0) || (perso[j - 1].dcoup != i * 90 + perso[i].acti) ) {
			if(degatperso(j - 1, k, objet[x].y, ax, ay, atta, true) ) {
				if(i >= 0) {
					perso[j - 1].invinc = 1;
					perso[j - 1].tape = i;
					perso[j - 1].dcoup = i * 90 + perso[i].acti;
				}

				baisseenerobjet(x, defobjet[objet[x].id].atta >> 1);
				return true;
			}
		}
	}

	if( (j = testecolperso(i, l, objet[x].y) ) ) { //Droite
		if( (i >= 0) || (perso[j - 1].dcoup != i * 90 + perso[i].acti) ) {
			if(degatperso(j - 1, l, objet[x].y, ax, ay, atta, true) ) {
				if(i >= 0) {
					perso[j - 1].invinc = 1;
					perso[j - 1].tape = i;
					perso[j - 1].dcoup = i * 90 + perso[i].acti;
				}

				baisseenerobjet(x, defobjet[objet[x].id].atta >> 1);
				return true;
			}
		}
	}

	if( (j = testecolperso(i, (k + l) >> 1, objet[x].y) ) ) { //Milieu
		if( (i >= 0) || (perso[j - 1].dcoup != i * 90 + perso[i].acti) ) {
			if(degatperso(j - 1, objet[x].x, objet[x].y, ax, ay, atta, true) ) {
				if(i >= 0) {
					perso[j - 1].invinc = 1;
					perso[j - 1].tape = i;
					perso[j - 1].dcoup = i * 90 + perso[i].acti;
				}

				baisseenerobjet(x, defobjet[objet[x].id].atta >> 1);
				return true;
			}
		}
	}

	return false;
}

void gere_camera() { //Gestion caméra
	int a = xscreen + (scrw >> 1);
	int b = yscreen + (scrh >> 1);
	int c = (scrw >> 1) * (scrh >> 1);// - 4000;
//	register i = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int dmax = 0;
	int td, tx, ty;

	for(i = 0; i < nb_joueurs; i++) {
		if(perso[i].enjeu) {
			if(joueur[i].cpu) {
				j++;
				k += perso[i].x;
				l += perso[i].y;
			} else {
				j = nb_joueurs;
				k = (perso[i].x + perso[i].dx * 3) * nb_joueurs;
				l = (perso[i].y + perso[i].dy * 5) * nb_joueurs;
			}
			tx = a - (perso[i].x >> 10);
			ty = b - (perso[i].y >> 10);
			td = tx * tx + ty * ty;
			if(td > dmax) dmax = td;
		}
	}
	if(j < 1) { //Personne, on va au centre
		k = area[num_area].coordj[1][0] * 16;
		l = area[num_area].coordj[1][1] * 16;
		dmax = c;
	} else {
		k = (k >> 10) / j;
		l = (l >> 10) / j;
	}

	if( ( ( (k - a) * (k - a) + (l - b) * (l - b) ) > 10) || ( (dmax - c) * (dmax - c) > 10000) ) { //On bouge
		if(k > (a + 10) ) {
			dxcam += 2;
			if(dxcam > 20) dxcam = 20;
		} else {
			if(k < (a - 10) ) {
				dxcam -= 2;
				if(dxcam < -20) dxcam = -20;
			} else dxcam >>= 1;
		}
		if(l > (b + 10) ) {
			dycam += 2;
			if(dycam > 20) dycam = 20;
		} else {
			if(l < (b - 10) ) {
				dycam -= 2;
				if(dycam < -20) dycam = -20;
			} else dycam >>= 1;
		}

		if(dmax > (c + 20000) ) {
			if(dzoom < 0) dzoom >>= 1;
			dzoom += 2;
			if(dzoom > 20) dzoom = 20;
		} else {
			if(dmax < (c - 10000) ) {
				if(dzoom > 0) dzoom >>= 1;
				dzoom -= 4;
				if(dzoom < -40) dzoom = -40;
			} else dzoom >>= 1;
		}

		changecamera(xscreen + dxcam, yscreen + dycam, zoomf + dzoom);
	}
}
