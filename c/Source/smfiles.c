#define	LOAD_MEM_	-2
#define	LOAD_OK_		-1
#define	LOAD_ABS_		0

void effaceperso(int id);

char tmpbyte[256]; //Buffers
short tmpshort[256];
int tmpint[256];

int bmpheader[5];

int isdir(const char *name) {
	SceIoStat fattr;
char tmpname[256];

	fattr.st_mode = 0;

	strcpy(tmpname, psp_path);
	strcat(tmpname, name);
	
	sceIoGetstat(tmpname, &fattr);
	return(!(fattr.st_mode & FIO_S_IFDIR) );
}

int rtrim(char *texte, const int len) {
	int i = len - 1;

	while( ( (texte[i] == 0x20) || (texte[i] == 0x0) ) && (i > 0) ) i--;

	if(i) {
		texte[i + 1] = 0x0;
	} else texte[0] = 0x0;

	return i;
}

int loadprefs() {
	int len;
	int handle = -1;
char tmpname[256];

	strcpy(tmpname, psp_path);
	strcat(tmpname, "smprefs.dat");

	handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
	if(handle < 0) return false;

	sceIoRead(handle, tmpbyte, 3);
	difficulte = tmpbyte[0];
//	luminosite = tmpbyte[1];
	volmusique = tmpbyte[2];
	sceIoRead(handle, tmpint, 4);
	maxsurvival = tmpint[0];
	len = sceIoRead(handle, tmpbyte, 1);
	if(len > 0) {
		transparency = tmpbyte[0];
	} else transparency = mid_transparency;

	sceIoClose(handle);

	return true;
}

void saveprefs() {
	int handle = -1;
char tmpname[256];

	strcpy(tmpname, psp_path);
	strcat(tmpname, "smprefs.dat");

	handle = sceIoOpen(tmpname, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	if(handle < 0) return;

	tmpbyte[0] = difficulte;
	tmpbyte[1] = 0; //luminosite;
	tmpbyte[2] = volmusique;
	sceIoWrite(handle, tmpbyte, 3);
	tmpint[0] = maxsurvival;
	sceIoWrite(handle, tmpint, 4);
	tmpbyte[0] = transparency;
	sceIoWrite(handle, tmpbyte, 1);

	sceIoClose(handle);
}

int loadbmp(const char *name) {
	int handle = -1;
	int i, j;
	int k = 1;
	int w2; //, h2;
	int l, l2;
	int a;
	unsigned char *tmpchar = bmpbuffer;
char tmpname[256];

	for(i = 1; i <= nb_bmps; i++) {
		if(strcmp(bmps[i].name, name) == 0) return i;
	}

	while(bmps[k].name[0] && (k <= nb_bmps) ) k++;
	if(k < max_bmps) {
		strcpy(tmpname, psp_path);
		strcat(tmpname, name);

		handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
		if(handle < 0) return LOAD_ABS_;

		sceIoLseek(handle, 10, PSP_SEEK_SET);
		sceIoRead(handle, bmpheader, 20);

		l2 = bmpheader[2] * bmpheader[3] * 2;

		bmps[k].image = (unsigned short *)malloc(l2);
		if(bmps[k].image == 0) {
			sceIoClose(handle);

			return LOAD_MEM_;
		}
		strcpy(bmps[k].name, name);

		a = bmpheader[2] * (bmpheader[3] - 1);

		unsigned short *type = (unsigned short *)bmpheader + 9;
		switch(*type) {
		case 8:
			w2 = ( (bmpheader[2] + 3) >> 2) * 4;
			l = w2 * bmpheader[3];
			w2 -= bmpheader[2];

			sceIoLseek(handle, bmpheader[0], PSP_SEEK_SET);

			sceIoRead(handle, bmpbuffer, l);
			for(j = 0; j < bmpheader[3]; j++) {
				for(i = 0; i < bmpheader[2]; i++) bmps[k].image[a + i - j * bmpheader[2]] = pal[*(tmpchar++)];
				tmpchar += w2;
			}
			break;
		default:
			w2 = ( (bmpheader[2] * 3 + 3) >> 2) * 4;
			l = w2 * bmpheader[3];
			w2 -= (bmpheader[2] * 3);

			sceIoLseek(handle, bmpheader[0], PSP_SEEK_SET);

			sceIoRead(handle, bmpbuffer, l);
			for(j = 0; j < bmpheader[3]; j++) {
				for(i = 0; i < bmpheader[2]; i++) {
					bmps[k].image[a + i - j * bmpheader[2]] = RGB( *(tmpchar + 2), *(tmpchar + 1), *tmpchar);
					tmpchar += 3;
				}
				tmpchar += w2;
			}
		}

		sceIoClose(handle);
		if(k > nb_bmps) nb_bmps = k;
		return k;
	} else return 0;
}

void freebmp(int id, int freemem) {
	if(freemem) free(bmps[id].image);
	bmps[id].name[0] = 0;
	if(id == nb_bmps) nb_bmps--;
}

void freebmpadr(void *bmpadr, int freemem) {
	int i;

	for(i = 1; i <= nb_bmps; i++) {
		if(bmps[i].name[0] && (bmps[i].image == bmpadr) ) freebmp(i, freemem);
	}
}

int loadwave(const char *name, s_sound *son, int id) {
#ifdef	__inc_son__
	int handle = -1;
	int i;
char tmpname[256];

	if(id < 0) {
		for(i = 0; i < nb_sons; i++) {
			if(strcmp(sndname[i], name) == 0) {
				son->length = sound[i].length;
				son->sound = sound[i].sound;
				return LOAD_OK_;
			}
		}
	}

	strcpy(tmpname, psp_path);
	strcat(tmpname, name);

	handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
	if(handle < 0) {
		son->length = 0;
		son->sound = (short *)dumsound;
		return LOAD_ABS_;
	}

	sceIoLseek(handle, 16, PSP_SEEK_SET);
	sceIoRead(handle, tmpint, 4);
	if( (tmpint[0] != 16) && (tmpint[0] != 18) ) {
		son->length = 0;
		son->sound = (short *)dumsound;
		sceIoClose(handle);
		return LOAD_ABS_;
	}
	sceIoLseek(handle, tmpint[0] + 4, PSP_SEEK_CUR);

	sceIoRead(handle, tmpint, 4);

	short *sndbuf = (short *)malloc(tmpint[0]);
	if(sndbuf == 0) {
		son->length = 0;
		son->sound = (short *)dumsound;
		sceIoClose(handle);
		return LOAD_MEM_;
	}

	sceIoRead(handle, sndbuf, tmpint[0]);

	son->length = tmpint[0];
	son->sound = sndbuf;

	if(id >= 0) strcpy(sndname[id], name); //On enregistre le nom

	sceIoClose(handle);
	return LOAD_OK_;
#else
	return LOAD_OK_;
#endif
}

#ifdef	__inc_son__
static inline void freewave(s_sound *son) {
	if(son->length > 0) {
		free(son->sound);
		son->length = 0;
	}
}
#endif

int loadpak(const char *name) {
	int handle = -1;
	int i, j, nbb;
	int k = 0;
	int taille;
	SceIoStat fattr;
char tmpname[256];
	int is16b = false;

	while(pak[k].id && (k < nb_paks) ) k++;
	if(k < max_paks) {
		strcpy(tmpname, psp_path);
		strcat(tmpname, name);

		sceIoGetstat(tmpname, &fattr);

		char *pnom = (char *)tmpint;
		unsigned short *bmpbuf;
		unsigned char *readbuf;
		unsigned char *writebuf;

		handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
		if(handle < 0) return LOAD_ABS_;

		sceIoRead(handle, tmpshort, 2);
		if(tmpshort[0] == 0x3631) {
			sceIoRead(handle, tmpshort, 4);
			if(tmpshort[0] == 0x5042) { //Header 16BP
				nbb = tmpshort[1];
				is16b = true;
			} else { //Ouais c pas gagné ça...
				nbb = 13873;
				sceIoLseek(handle, 2, PSP_SEEK_SET);
			}
		} else nbb = tmpshort[0];

		taille = fattr.st_size - nbb * 20 - 2;
		if(is16b) {
			bmpbuf = (unsigned short *)malloc(taille);
		} else bmpbuf = (unsigned short *)malloc(taille * 2);
		if(bmpbuf == 0) {
			sceIoClose(handle);
			return LOAD_MEM_;
		}
		pak[k].pakadr = bmpbuf;
		pak[k].id = true;

		for(i = 0; i < nbb; i++) {
			sceIoRead(handle, tmpint, 20);

			j = 1;
			while(bmps[j].name[0] && (j <= nb_bmps) ) j++;
			if(j < max_bmps) {
				rtrim(pnom, 15);
				strcpy(bmps[j].name, pnom);
				bmps[j].image = bmpbuf;

				if(j > nb_bmps) nb_bmps = j;
			}
			if(is16b) {
				bmpbuf += (tmpint[4] >> 1);
			} else bmpbuf += tmpint[4];
		}
		if(is16b) {
			sceIoRead(handle, pak[k].pakadr, taille);
		} else {
			readbuf = (unsigned char *)pak[k].pakadr + taille;
			writebuf = (unsigned char *)pak[k].pakadr;
			sceIoRead(handle, readbuf, taille);
			for(i = 0; i < taille; i++) *(writebuf + i * 2) = *(readbuf + i);
		}

		if(k >= nb_paks) nb_paks++;
		sceIoClose(handle);
		return nbb;
	} else return 0;
}

int loadfrompak(const char *name, const char *bmpname) {
	int handle = -1;
	int i, nbb;
	int k = 1;
	int pos = 0;
char tmpname[256];
	int is16b = false;

	while(bmps[k].name[0] && (k <= nb_bmps) ) k++;
	if(k < max_bmps) {
		strcpy(tmpname, psp_path);
		strcat(tmpname, name);

		char *pnom = (char *)tmpint;
		unsigned short *bmpbuf;
		unsigned char *readbuf;
		unsigned char *writebuf;

		handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
		if(handle < 0) return LOAD_ABS_;

		sceIoRead(handle, tmpshort, 2);
		if(tmpshort[0] == 0x3631) {
			sceIoRead(handle, tmpshort, 4);
			if(tmpshort[0] == 0x5042) { //Header 16BP
				nbb = tmpshort[1];
				is16b = true;
			} else { //Ouais c pas gagné ça...
				nbb = 13873;
				sceIoLseek(handle, 2, PSP_SEEK_SET);
			}
		} else nbb = tmpshort[0];

		for(i = 0; i < nbb; i++) {
			sceIoRead(handle, tmpint, 20);

			rtrim(pnom, 15);
			if(strcmp(pnom, bmpname) == 0) { //Trouvé
				if(is16b) {
					bmpbuf = (unsigned short *)malloc(tmpint[4]);
				} else bmpbuf = (unsigned short *)malloc(tmpint[4] * 2);
				if(bmpbuf == 0) {
					sceIoClose(handle);
					return LOAD_MEM_;
				}
				strcpy(bmps[k].name, pnom);
				bmps[k].image = bmpbuf;

				sceIoLseek(handle, nbb * 20 + pos + 2, PSP_SEEK_SET);
				if(is16b) {
					sceIoRead(handle, bmpbuf, tmpint[4]);
				} else {
					readbuf = (unsigned char *)bmpbuf + tmpint[4];
					writebuf = (unsigned char *)bmpbuf;
					sceIoRead(handle, readbuf, tmpint[4]);
					for(i = 0; i < tmpint[4]; i++) *(writebuf + i * 2) = *(readbuf + i);
				}

				sceIoClose(handle);
				if(k > nb_bmps) nb_bmps = k;
				return k;
			} else {
				if(is16b) {
					pos += (tmpint[4] >> 1);
				} else pos += tmpint[4];
			}
		}

		sceIoClose(handle);
	}
	return 0;
}

void freepak(int numpak) {
	free(pak[numpak].pakadr);
	pak[numpak].id = false;

	if(numpak == nb_paks - 1) nb_paks--;
}

int loadpal(const char *name, const int mode) {
	int handle = -1;
	int i;
	unsigned char *tmppal = (unsigned char *)tmpint;
char tmpname[256];

	strcpy(tmpname, psp_path);
	strcat(tmpname, name);

	handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
	if(handle < 0) return LOAD_ABS_;

	sceIoRead(handle, tmppal, 768);

	for(i = 0; i < 255; i++) pal[i] = RGB(tmppal[i * 3], tmppal[i * 3 + 1], tmppal[i * 3 + 2]);
	if(mode) pal[255] = COUL_TRANS; //Couleur transparente - chargement sélectif

	sceIoClose(handle);
	return LOAD_OK_;
}

int loaddefperso(const char *name) {
	int len;
	int handle = -1;
	int i, j;
	char *movepos;
char tmpname[256];

	strcpy(tmpname, psp_path);
	strcat(tmpname, name);

	handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
	if(handle < 0) return LOAD_ABS_;

	sceIoRead(handle, &defperso[nb_defpersos], 35);
	rtrim(defperso[nb_defpersos].name, 31);

	i = strlen(name);
	strncpy(tmpname, name, i - 4);
	tmpname[i - 4] = 0;
	strcpy(defperso[nb_defpersos].pakname, tmpname);
	strcat(defperso[nb_defpersos].pakname, ".pak");

	sceIoLseek(handle, 15, PSP_SEEK_CUR);

	sceIoRead(handle, tmpbyte, 15);
	if(rtrim(tmpbyte, 15) ) {
		defperso[nb_defpersos].idphoto = loadfrompak(defperso[nb_defpersos].pakname, tmpbyte);

		if(defperso[nb_defpersos].idphoto == LOAD_ABS_) {
			strcat(psp_path, tmpname);
			strcat(psp_path, "/");

			defperso[nb_defpersos].idphoto = loadfrompak(defperso[nb_defpersos].pakname, tmpbyte);

			strcpy(psp_path, game_path);
		}

		convbmp(bmps[defperso[nb_defpersos].idphoto].image, 48, 48);
	}

	defperso[nb_defpersos].idcoups = nb_moves;

	for(i = 0; i < 90; i++) { //Mouvements
		movepos = (char *)&move[nb_moves + i] + 2;
		move[nb_moves + i].idim = nb_perss;
		sceIoRead(handle, movepos, 24);

		if(move[nb_moves + i].type == move_projectile) { //Projectile
			if(move[nb_moves + i].atta) move[nb_moves + i].atta = nb_defprojects;
		}

		if(move[nb_moves + i].nani >= 0) { //Images mouvement
			for(j = 0; j <= move[nb_moves + i].nani; j++) {
				sceIoLseek(handle, 15, PSP_SEEK_CUR);
				sceIoRead(handle, tmpshort, 8);
				_assignation_perss(nb_perss++, tmpshort[0], tmpshort[1], tmpshort[2], tmpshort[3], 0);
			}
		}
	}

	for(i = 0; i < 90; i++) { //Coordonnées étendues
		if(move[nb_moves + i].type == move_priseec) {
			move[nb_moves + i].dx = nb_clists; //Index vers clist
			for(j = 0; j <= move[nb_moves + i].nani; j++) {
				sceIoRead(handle, clist[nb_clists], 6);
				switch(clist[nb_clists][2] ) {
				case 0: //Droite
					clist[nb_clists++][2] = 1;
					break;
				case 1: //Gauche
					clist[nb_clists++][2] = -1;
					break;
				default: //Hide
					clist[nb_clists++][2] = 0;
				}
			}
		}
	}

	sceIoRead(handle, tmpshort, 2);
	defperso[nb_defpersos].nsons = tmpshort[0];
	sceIoLseek(handle, 15 * tmpshort[0], PSP_SEEK_CUR);

	len = sceIoRead(handle, tmpshort, 8);
	if(len > 0) {
		defproject[nb_defprojects].atta = tmpshort[0];
		defproject[nb_defprojects].smash = tmpshort[1];
		defproject[nb_defprojects].port = tmpshort[2];
		defproject[nb_defprojects].nani = tmpshort[3] - 1;
		defproject[nb_defprojects].idim = nb_perss;

		for(i = 0; i <= defproject[nb_defprojects].nani; i++) {
			sceIoLseek(handle, 15, PSP_SEEK_CUR);
			sceIoRead(handle, tmpshort, 8);
			_assignation_perss(nb_perss++, tmpshort[0], tmpshort[1], tmpshort[2], tmpshort[3], 0);
		}

		nb_defprojects++;
	}

	nb_defpersos++;
	nb_moves += 90;

	strcpy(psp_path, game_path);

	sceIoClose(handle);
	return LOAD_OK_;
}

int loadperso(int id) {
	int len;
	int handle = -1;
	int i, j;
	int k, l;
	int rc = nb_defpersos >> 1;
char tmpname[256];
#ifdef	__inc_son__
	SceIoStat fattr;
#endif

	strncpy(tmpbyte, defperso[id].pakname, strlen(defperso[id].pakname) - 4);
	tmpbyte[strlen(defperso[id].pakname) - 4] = 0;
	strcat(tmpbyte, ".chr");

	strcpy(tmpname, psp_path);
	strcat(tmpname, tmpbyte);

	handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
	if(handle < 0) return LOAD_ABS_;

	strncpy(tmpbyte, defperso[id].pakname, strlen(defperso[id].pakname) - 4); //Lecture pack bmps
	tmpbyte[strlen(defperso[id].pakname) - 4] = 0;

	k = loadpak(defperso[id].pakname);

	if(k == LOAD_ABS_) {
		strcat(psp_path, tmpbyte);
		strcat(psp_path, "/");

		k = loadpak(defperso[id].pakname);
	}
	strcpy(g_string, psp_path);

	while( (k == LOAD_MEM_) && rc) {
		effaceperso(id);
		k = loadpak(defperso[id].pakname);
		rc--;
	}
	defperso[id].idpak = k;

	sceIoLseek(handle, 35, PSP_SEEK_CUR);

	sceIoRead(handle, tmpbyte, 15);
	if(rtrim(tmpbyte, 15) ) {
		k = loadbmp(tmpbyte);
		while( (k == LOAD_MEM_) && rc) {
			effaceperso(id);
			k = loadbmp(tmpbyte);

			rc--;
		}
		defperso[id].idicone = k;

		convbmp(bmps[k].image, 16, 16);
	}

	sceIoLseek(handle, 15, PSP_SEEK_CUR);

	for(i = 0; i < 90; i++) { //Mouvements
		sceIoLseek(handle, 24, PSP_SEEK_CUR);

		if(move[defperso[id].idcoups + i].nani >= 0) { //Images mouvement
			for(j = 0; j <= move[defperso[id].idcoups + i].nani; j++) {
				sceIoRead(handle, tmpbyte, 15);
				sceIoLseek(handle, 8, PSP_SEEK_CUR);
				if(rtrim(tmpbyte, 15) ) {
					k = loadbmp(tmpbyte);
					while( (k == LOAD_MEM_) && rc) {
						effaceperso(id);
						k = loadbmp(tmpbyte);
						rc--;
					}
					l = move[defperso[id].idcoups + i].idim + j;
					_assignation_perss(l, 0, 0, 0, 0, bmps[k].image);

					if(!issized(id, l) ) convbmp(pers[l].image, pers[l].w, pers[l].h);
					pers[l].sized = true;
				}
			}
		}
	}

	for(i = 0; i < 90; i++) { //Coordonnées étendues
		if(move[defperso[id].idcoups + i].type == move_priseec) len = sceIoLseek(handle, 6 * (move[defperso[id].idcoups + i].nani + 1), PSP_SEEK_CUR);
	}

#ifdef	__inc_son__
	sceIoLseek(handle, 2, PSP_SEEK_CUR);

	for(i = 1; i <= defperso[id].nsons; i++) {
		sceIoRead(handle, tmpbyte, 15);
		if(rtrim(tmpbyte, 15) ) {
			strcpy(tmpname, psp_path);
			strcat(tmpname, tmpbyte);

			if(sceIoGetstat(tmpname, &fattr) < 0) {
				strcpy(psp_path, game_path);
				strcat(psp_path, "common/");
			}
			k = loadwave(tmpbyte, &defperso[id].son[i], -1);
			while( (k == LOAD_MEM_) && rc) {
				effaceperso(id);
				k = loadwave(tmpbyte, &defperso[id].son[i], -1);
				rc--;
			}

			strcpy(psp_path, g_string);
		}
	}
#else
	sceIoLseek(handle, 15 * defperso[id].nsons + 2, PSP_SEEK_CUR);
#endif

	len = sceIoLseek(handle, 8, PSP_SEEK_CUR);
	if(len > 0) {
		j = 0;
		for(i = 0; i < 90; i++) {
			if(move[defperso[id].idcoups + i].type == move_projectile) { //Projectile
				j = move[defperso[id].idcoups + i].atta;
				break;
			}
		}

		if(j) {
			for(i = 0; i <= defproject[j].nani; i++) {
				sceIoRead(handle, tmpbyte, 15);
				sceIoLseek(handle, 8, PSP_SEEK_CUR);
				if(rtrim(tmpbyte, 15) ) {
					k = loadbmp(tmpbyte);
					while( (k == LOAD_MEM_) && rc) {
						effaceperso(id);
						k = loadbmp(tmpbyte);
						rc--;
					}
					l = defproject[j].idim + i;
					_assignation_perss(l, 0, 0, 0, 0, bmps[k].image);

					if(!issized_p(j, l) ) convbmp(pers[l].image, pers[l].w, pers[l].h);
					pers[l].sized = true;
				}
			}
		}
	}

	strcpy(psp_path, game_path);

	sceIoClose(handle);
	return LOAD_OK_;
}

void freeperso(int id) {
	int i, j;

#ifdef	__inc_son__
	for(i = defperso[id].nsons - 1; i >= 0; i--) freewave(&defperso[id].son[i]);
#endif

	if(defperso[id].idpak) { //On efface le pak et désalloue les bmps
		for(i = 0; i < 90; i++) {
			if(move[defperso[id].idcoups + i].nani >= 0) { //Images mouvement
				if(move[defperso[id].idcoups + i].type == move_projectile) { //Projectiles
					for(j = 0; j <= defproject[move[defperso[id].idcoups + i].atta].nani; j++) {
						freebmpadr(pers[defproject[move[defperso[id].idcoups + i].atta].idim + j].image, false);
						pers[defproject[move[defperso[id].idcoups + i].atta].idim + j].sized = false;
					}
				}

				for(j = 0; j <= move[defperso[id].idcoups + i].nani; j++) { //Mouvements
					freebmpadr(pers[move[defperso[id].idcoups + i].idim + j].image, false);
					pers[move[defperso[id].idcoups + i].idim + j].sized = false;
				}
			}
		}

		freebmp(defperso[id].idicone, false);

		freepak(defperso[id].idpak);
		defperso[id].idpak = 0;
	} else { //On libère les bmps
		for(i = 89; i >= 0; i--) { //Projectiles
			if(move[defperso[id].idcoups + i].nani >= 0) {
				if(move[defperso[id].idcoups + i].type == move_projectile) {
					for(j = defproject[move[defperso[id].idcoups + i].atta].nani; j >= 0; j--) {
						freebmpadr(pers[defproject[move[defperso[id].idcoups + i].atta].idim + j].image, true);
						pers[defproject[move[defperso[id].idcoups + i].atta].idim + j].sized = false;
					}
				}
			}
		}

		for(i = 89; i >= 0; i--) { //Mouvements
			if(move[defperso[id].idcoups + i].nani >= 0) {
				for(j = move[defperso[id].idcoups + i].nani; j >= 0 ; j--) {
					freebmpadr(pers[move[defperso[id].idcoups + i].idim + j].image, true);
					pers[move[defperso[id].idcoups + i].idim + j].sized = false;
				}
			}
		}

		freebmp(defperso[id].idicone, true);
	}

	defperso[id].idicone = 0;
}

void effaceperso(int id) { //On libère de la mémoire
	int j = nb_defpersos - 1;

	while(j >= 0) {
		if( (j != id) && !defperso[j].required && defperso[j].idicone) {
			freeperso(j);
			return;
		}
		j--;
	}
}

static inline void testeperso(int id) {
	if(defperso[id].idicone == 0) loadperso(id); //Perso non chargé
}

void testefond(int idarn) {
	if(area[idarn].idfond <= 0) { //Fond pas chargé
		int i = loadbmp(area[idarn].backname);
		int j = nb_areas - 1;

		while( (i == LOAD_MEM_) && (j >= 0) ) {
//			if(j != idarn) { //On libère de la mémoire
				if(!area[j].required && area[j].idfond) {
					freebmp(area[j].idfond, true);
					area[j].idfond = 0;
					i = loadbmp(area[idarn].backname);
				}
//			}
			j--;
		}

		j = nb_defpersos >> 1;
		while( (i == LOAD_MEM_) && j) {
			effaceperso(-1);
			i = loadbmp(area[idarn].backname);
			j--;
		}

		if(i > 0) area[idarn].idfond = i;
	}
}

int loaddefarena(const char *name) {
	int handle = -1;
	int i;
char tmpname[256];

	strcpy(tmpname, psp_path);
	strcat(tmpname, name);

	handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
	if(handle < 0) return LOAD_ABS_;

	strcpy(area[nb_areas].fname, name);
	area[nb_areas].idfond = 0;

	sceIoRead(handle, &area[nb_areas], 20);

	sceIoRead(handle, tmpshort, 4);
	for(i = 0; i < max_joueurs; i++) {
		area[nb_areas].coordj[i][0] = tmpshort[0];
		area[nb_areas].coordj[i][1] = tmpshort[1];
		tmpshort[0] += 4;
	}

	sceIoRead(handle, area[nb_areas].backname, 15);
	rtrim(area[nb_areas].backname, 15);
	sceIoRead(handle, area[nb_areas].modname, 15);
	rtrim(area[nb_areas].modname, 15);
	sceIoRead(handle, area[nb_areas].name, 31);
	rtrim(area[nb_areas].name, 31);

	sceIoRead(handle, tmpshort, 2);
	area[nb_areas].nb_zones = tmpshort[0];
	sceIoRead(handle, &area[nb_areas].zone[0], tmpshort[0] * 6);

	sceIoClose(handle);

	return nb_areas++;
}

int loadarena(int id) {
	int handle = -1;
	int i;
char tmpname[256];

	strcpy(tmpname, psp_path);
	strcat(tmpname, area[id].fname);

	handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
	if(handle < 0) return LOAD_ABS_;

#ifdef	__inc_musique__
//	MODPlay_Stop(&mp);
//	MODPlay_Unload(&mp);
	Player_Stop();
	MikMod_FreeSong(mf);
#endif

	testefond(id);

	sceIoLseek(handle, 85, PSP_SEEK_SET);
	sceIoRead(handle, tmpshort, 2);
	sceIoLseek(handle, tmpshort[0] * 6, PSP_SEEK_CUR);

	for(i = 0; i < area[id].w; i++) { //Tableau foreground
		sceIoRead(handle, &tabl[i][0], area[id].h * 2);
	}
	sceIoRead(handle, tablb, 54000);

#ifdef	__inc_musique__
	if(strlen(area[id].modname) ) {
		strcpy(tmpname, psp_path);
		strcat(tmpname, area[id].modname);
		mf = MikMod_LoadSong(tmpname, 128);
		if(mf != 0) {
			Player_Start(mf);

			mf->volume = volmusique * 24;
		}
	}
#endif

	i = ( ( (area[id].w - 2) << 14) * 3) / (screen_width * 4);
	zoommax = ( ( (area[id].h - 2) << 14) * 3) / (screen_height * 4);
	if(i < zoommax) zoommax = i;
	if(zoommax > 3500) zoommax = 3500;
	xmaxm = (area[id].w - 1) << 14;
	ymaxm = (area[id].h - 1) << 14;
//	decbg = (200 - (200 * area[id].w) / 100) * screen_height;
	decbg = 200 - area[id].w * 2;

	sceIoClose(handle);

	return true;
}
