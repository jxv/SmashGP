#define	nb_menus		1
#define	nb_items		8
#define	nb_varitems	8
#define	max_varitem	10

#define	menu_options	0

typedef struct { //Sprite 64*64 / 256*16
	int x, y, w, h;
	unsigned short tmpbmp[4096];
} s_spritebig;

typedef struct {
	int deb, fin;
} s_menu;

typedef struct {
	int prev, next, var;
	int dx, dy;
	char texte[20];
} s_item;

typedef struct {
	char valeur[max_varitem][32];
	int *var;
} s_varitem;

s_spritebig cursmenu[2];

s_menu menu[nb_menus];

s_item item[nb_items];

s_varitem varitem[nb_varitems];

const int xjetini[max_joueurs] = {54, 124, 194, 264};
const int yjetini[max_joueurs] = {135, 135, 135, 135};
int xjet[max_joueurs] = {54, 124, 194, 264};
int yjet[max_joueurs] = {135, 135, 135, 135};

int classm[max_joueurs];
int distjet[max_joueurs];

void main_menu();

void mode_menu();

void choix_persos(int debut);

void choix_arene();

void gere_menus(int id);

void init_moteur();

//Menus

void animwrite(int x, int y, const char *texte) {
	int i;

	for(i = 0; i < strlen(texte); i++) {
		CharTransBlit(db, x, y, texte[i] );
		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
		x = CharTransBlit(db, x, y, texte[i] ); //Double buffer
	}
}

void itemclig() {
	int i;

	for(i = 0; i < 24; i++) {
		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
		wait(30);
	}
}

void itemblur() {
	int i;

	for(i = 0; i < 8; i++) {
		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
	}
}

void waitnokey() {
	while(mp_GetKey() ) {
		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
	}

	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	db = 1 - db;
	sceDisplayWaitVblankStart();
}

void splash_screen() {
	unsigned int exKey;

	mp_FastBlit(db, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (unsigned short *)image_splash);
	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	db = 1 - db;
	sceDisplayWaitVblankStart();
	mp_FastBlit(db, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (unsigned short *)image_splash);

	animwrite(16, 256, "SmashGpsp 0.2b - mATkEUpON");

	while(1) { //Boucle du menu
		exKey = mp_GetKey();
		if(exKey & PSP_CTRL_START) return;

		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
	}

	mp_Srand();
}

void main_menu() {
	int i = 0;
	int j;
	unsigned int exKey;
	unsigned int prevkey = 0;

	waitnokey();
	mp_FastBlit(db, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (unsigned short *)image_main);
	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	db = 1 - db;
	sceDisplayWaitVblankStart();

	while(1) { //Boucle du menu
		mp_FastBlit(db, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (unsigned short *)image_main);
		exKey = mp_GetKey();

		j = i;
		if(!prevkey) {
			if( (exKey & PSP_CTRL_UP) || (mp_keys.Ly < 96) ) { //Haut
				if(i > 0) i--;
			} else {
				if( (exKey & PSP_CTRL_DOWN) || (mp_keys.Ly > 160) ) { //Bas
					if(i < 2) i++;
				}
			}
			if( (exKey & PSP_CTRL_LEFT) || (mp_keys.Lx < 96) ) { //Gauche
				if(i < 2) i++;
			} else {
				if( (exKey & PSP_CTRL_RIGHT) || (mp_keys.Lx > 160) ) { //Droite
					if(i > 0) i--;
				}
			}

			if(exKey & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE | PSP_CTRL_START) ) { //Bouton X / O / Start
				PlaySound(&sound[son_menu]);
				itemclig();

				switch(i) {
				case 0: //1 player
					mode_menu();
					return;
				case 1: //Options
					gere_menus(menu_options);
					return;
				default: //Reset
					sceGuTerm();

					sceKernelExitGame();
				}
			} else {
				if(exKey & PSP_CTRL_TRIANGLE) { //Bouton Triangle
					splash_screen();
					return;
				}
			}
		}
		prevkey = exKey || ( ( (mp_keys.Lx + 32) >> 6) != 2) || ( ( (mp_keys.Ly + 32) >> 6) != 2);

		switch(i) {
		case 0: //Bouton 1 player
//			mp_FastBlit(db, 235, 72, 122, 26, (unsigned short *)btn_1player);
			mp_FastBlit(db, 225, 69, 159, 40, (unsigned short *)btn_1player);
			break;
		case 1: //Bouton options
//			mp_FastBlit(db, 179, 141, 118, 26, (unsigned short *)btn_options);
			mp_FastBlit(db, 172, 145, 128, 40, (unsigned short *)btn_options);
			break;
		default: //Bouton reset
//			mp_FastBlit(db, 128, 209, 91, 23, (unsigned short *)btn_reset);
			mp_FastBlit(db, 112, 222, 174, 43, (unsigned short *)btn_reset);
		}
		if(j != i) PlaySound(&sound[son_menu]);

		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
	}
}

void mode_menu() {
	int i = type_jeu;
	int j;
	unsigned int exKey;
	unsigned int prevkey = 0;

	waitnokey();
	mp_FastBlit(db, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (unsigned short *)image_mode);
	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	db = 1 - db;
	sceDisplayWaitVblankStart();

	while(1) { //Boucle du menu
		mp_FastBlit(db, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (unsigned short *)image_mode);
		exKey = mp_GetKey();

		j = i;
		if(!prevkey) {
			if( (exKey & PSP_CTRL_UP) || (mp_keys.Ly < 96) ) { //Haut
				if(i > 0) i--;
			} else {
				if( (exKey & PSP_CTRL_DOWN) || (mp_keys.Ly > 160) ) { //Bas
					if(i < 2) i++;
				}
			}
			if( (exKey & PSP_CTRL_LEFT) || (mp_keys.Lx < 96) ) { //Gauche
				if(i > 0) i--;
			} else {
				if( (exKey & PSP_CTRL_RIGHT) || (mp_keys.Lx > 160) ) { //Droite
					if(i < 2) i++;
				}
			}

			if(exKey & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE) ) { //Bouton X / O
				PlaySound(&sound[son_menu]);
				itemclig();

				type_jeu = i;
				choix_persos(0);
				return;
			} else {
				if(exKey & PSP_CTRL_TRIANGLE) { //Bouton Triangle
					return;
				} else {
					if(exKey & PSP_CTRL_START) { //Start
						PlaySound(&sound[son_menu]);
						itemclig();

						type_jeu = i;
						choix_persos(0);
						return;
					}
				}
			}
		}
		prevkey = exKey || ( ( (mp_keys.Lx + 32) >> 6) != 2) || ( ( (mp_keys.Ly + 32) >> 6) != 2);

		switch(i) {
		case 0: //Bouton time
//			mp_FastBlit(db, 101, 72, 192, 88, (unsigned short *)btn_time);
			mp_FastBlit(db, 34, 66, 397, 100, (unsigned short *)btn_time);
			break;
		case 1: //Bouton lives
//			mp_FastBlit(db, 191, 115, 187, 82, (unsigned short *)btn_lives);
			mp_FastBlit(db, 116, 122, 331, 87, (unsigned short *)btn_lives);
			break;
		default: //Bouton survival
//			mp_FastBlit(db, 101, 162, 242, 86, (unsigned short *)btn_survival);
			mp_FastBlit(db, 34, 177, 383, 86, (unsigned short *)btn_survival);
		}
		if(j != i) {
			PlaySound(&sound[son_menu]);
			itemblur(); //Changement
		}

		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
	}
}

const char tabduree[max_long_jeu + 1][3] = {"3", "5", "10", "15", "20", "30"};

void affichejeton(int i) {
	int a;

	switch(i) {
	case 0:
		if(joueur[0].cpu) {
			a = explos_jetonrcp;
		} else a = explos_jetonr;
		break;
	case 1:
//		if(joueur[1].cpu) {
			a = explos_jetonv;
//		} else a = explos_jetonv2p;
		break;
	case 2:
		a = explos_jetonb;
		break;
	default:
		a = explos_jetonj;
	}

	FastTransBlit(db, xjet[i] - expl__[explos_jetonr].dx + 80, yjet[i] - expl__[explos_jetonr].dy + 16, expl__[explos_jetonr].w, expl__[explos_jetonr].h, (unsigned short *)expl__[a].image, COUL_TRANS);
}

void choix_persos(int debut) {
	int xc = 160 << 4;
	int yc = 120 << 4;
	int dxc = 0;
	int dyc = 0;
	int fc = explos_maino;
	int jetonp = -1;
	int i;
	int j = 23;
	int k = 40;
	int l;
	unsigned int exKey;
	unsigned int prevkey = 0;
	int tm = 0;
	int rtf = false;

	waitnokey();
	mp_ClrScr(2, 0x0000);
	mp_FastBlit(2, 80, 16, 320, 240, (unsigned short *)image_char);

	mp_InitGu(db);

	zoomf = 1260;
	zoominv = 832;

	sceGuStart(GU_DIRECT, gu_list);
	sceGuDrawBufferList(GU_PSM_5551, (void *)(FRAMESIZE * 2), 512);	
//	sceGuDrawBuffer(GU_PSM_5551, (void *)(FRAMESIZE * 2), 512);	
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexMode(GU_PSM_5551, 0, 0, 0); //Les photos sont en mode normal
//	sceGuDisable(GU_BLEND);

	for(i = 0; i < 12; i++) {
		if(i + debut >= nb_defpersos) break;

		ZoomTransBlit(2, mulzoom(j + 1 + 80), mulzoom(k + 1 + 16), 48, 48, (unsigned short *)bmps[defperso[i + debut].idphoto].image, COUL_TRANS, 1);

		j += 47;
		if(j > 258) {
			j = 23;
			k += 47;
		}
	}

	sceGuFinish();

	sceGuSync(0, 0);
	edram_ptr = (unsigned short *)textbuf_[0];

	j = 23;
	k = 40;
	for(i = 0; i < 12; i++) {
		if(i + debut >= nb_defpersos) break;

		StringTransBlit(2, j + 19 - strlen(defperso[i + debut].name) * 3 + 80, k + 16, defperso[i + debut].name);
		j += 47;
		if(j > 258) {
			j = 23;
			k += 47;
		}
	}

	if(type_jeu != type_survival) {
		for(i = 1; i < 4; i++) { //Au cas où on revienne du mode survival
/*			if( (xjet[i] != xjetini[i]) || (yjet[i] != yjetini[i]) ) {
				j = (xjet[i] - 19) / 47;
				if(j > 5) j = 5;
				k = (yjet[i] - 36) / 47;
				l = k * 6 + j;
				if( (l >= 0) && (l + debut < nb_defpersos) ) perso[i].id = l + debut;
			}*/
			if(perso[i].id >= 0) {
				l = (perso[i].id / 12) * 12;
				k = (perso[i].id - l) / 6;
				j = (perso[i].id - l) - k * 6;
				if( (i >> 1) * 2 == i) { //A gauche
					xjet[i] = j * 47 + 33;
				} else xjet[i] = j * 47 + 53;
				yjet[i] = k * 47 + 50 + (i >> 1) * 20;
			}
		}

		if(type_jeu == type_temps) { //Bouton temps
			FastTransBlit(2, 53 + 80, 4 + 16, 73, 12, (unsigned short *)btn_pmtime, COUL_TRANS);
		} else FastTransBlit(2, 53 + 80, 4 + 16, 73, 12, (unsigned short *)btn_pmstock, COUL_TRANS); //Bouton stock

		FastTransBlit(2, 147 + 80, 4 + 16, 73, 12, (unsigned short *)btn_pmplayers, COUL_TRANS);
	} else {
		nb_joueurs = 1;
		vies_jeu = 1;
		joueur[0].cpu = false;
	}
	FastTransBlit(2, 249 + 80, 4 + 16, 51, 12, (unsigned short *)btn_back, COUL_TRANS);

	if(debut) FastTransBlit(2, 0 + 80, 0 + 16, 15, 30, (unsigned short *)btn_left, COUL_TRANS); //Bouton left
	if(nb_defpersos > debut + 12) FastTransBlit(2, 305 + 80, 0 + 16, 15, 30, (unsigned short *)btn_right, COUL_TRANS); //Bouton right

	for(i = 0; i < max_persos; i++) {
		perso[i].acti = mp_Rand(90);
		perso[i].stat = 0;
		perso[i].stata = 0;
	}

	while(1) { //Boucle du menu
		tm++;
		if(tm > 24) tm = 0;
		exKey = mp_GetKey();

		if( ( (mp_keys.Ly + 16) >> 5) != 4) { //Analogique en priorité
			dyc = (mp_keys.Ly - 128) >> 1;
		} else {
			if(exKey & PSP_CTRL_UP) { //Haut
				dyc -= 16;
				if(dyc < -(6 << 3) ) dyc = -(6 << 3);
			} else {
				if(exKey & PSP_CTRL_DOWN) { //Bas
					dyc += 16;
					if(dyc > (6 << 3) ) dyc = (6 << 3);
				} else {
					dyc /= 2;
				}
			}
		}
		yc += dyc;
		if(yc < 0) {
			yc = 0;
		} else {
			if(yc > (screen_height_ << 4) ) yc = (screen_height_ << 4);
		}

		if( ( (mp_keys.Lx + 16) >> 5) != 4) { //Analogique en priorité
			dxc = (mp_keys.Lx - 128) >> 1;
		} else {
			if(exKey & PSP_CTRL_LEFT) { //Gauche
				dxc -= 16;
				if(dxc < -(6 << 3) ) dxc = -(6 << 3);
			} else {
				if(exKey & PSP_CTRL_RIGHT) { //Droite
					dxc += 16;
					if(dxc > (6 << 3) ) dxc = (6 << 3);
				} else {
					dxc /= 2;
				}
			}
		}
		xc += dxc;
		if(xc < 0) {
			xc = 0;
		} else {
			if(xc > (screen_width_ << 4) ) xc = (screen_width_ << 4);
		}

		if(!prevkey) {
			if(exKey & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE) ) { //Bouton X / O
				if( (yc < (37 << 4) ) && (jetonp < 0) ) { //En haut - test boutons
					if(xc < (20 << 4) ) { //Left
						if(debut) {
							PlaySound(&sound[son_menu]);

							choix_persos(debut - 12);
							return;
						}
					} else {
						if(xc < (89 << 4) ) { //- stock / temps
							if(type_jeu != type_survival) {
								PlaySound(&sound[son_menu]);

								if(type_jeu == type_temps) {
									if(long_jeu > 0) long_jeu--;
								} else {
									if(vies_jeu > 1) vies_jeu--;
								}
							}
						} else {
							if(xc < (135 << 4) ) { //+ stock / temps
								if(type_jeu != type_survival) {
									PlaySound(&sound[son_menu]);

									if(type_jeu == type_temps) {
										if(long_jeu < max_long_jeu) long_jeu++;
									} else {
										if(vies_jeu < max_vies_jeu) vies_jeu++;
									}
								}
							} else {
								if(xc < (183 << 4) ) { //- players
									if(type_jeu != type_survival) {
										PlaySound(&sound[son_menu]);

										if(nb_joueurs > 2) {
											nb_joueurs--;

											if(perso[nb_joueurs].id >= 0) {
												j = true;
												for(i = 0; i < nb_joueurs; i++) {
													if(perso[nb_joueurs].id == perso[i].id) j = false;
												}
												if(j) defperso[perso[nb_joueurs].id].required = false;
											}
										}
									}
								} else {
									if(xc < (234 << 4) ) { //+ players
										if(type_jeu != type_survival) {
											PlaySound(&sound[son_menu]);

											if(nb_joueurs < max_joueurs) {
												if(perso[nb_joueurs].id >= 0) defperso[perso[nb_joueurs].id].required = true;

												nb_joueurs++;
											}
										}
									} else { //Back
										if(xc < (300 << 4) ) { //Bouton back
											PlaySound(&sound[son_menu]);

											mode_menu();
											return;
										} else { //Right
											if(nb_defpersos > debut + 12) {
												PlaySound(&sound[son_menu]);

												choix_persos(debut + 12);
												return;
											}
										}
									}
								}
							}
						}
					}
				} else {
					if( (type_jeu != type_survival) && (yc > (140 << 4) ) && (yc < (154 << 4) ) && (jetonp < 0) ) { //En bas - test boutons joueurs
						if( (perso[0].id >= 0) && (xc > (57 << 4) ) && (xc < (86 << 4) ) ) { //J1
							PlaySound(&sound[son_menu]);

							if(joueur[0].cpu) {
								joueur[0].cpu = false;
							} else joueur[0].cpu = true;
/*						} else {
							if(chatboard && (perso[1].id >= 0) && (xc > 127) && (xc < 156) ) { //J2
								PlaySound(&sound[son_menu]);

								if(joueur[1].cpu) {
									joueur[1].cpu = false;
								} else joueur[1].cpu = true;
							}*/
						}
					} else {
						if(jetonp < 0) { //Aucun jeton pris
							for(i = 0; i < nb_joueurs; i++) {
								j = (xc >> 4) - xjet[i];
								k = (yc >> 4) - yjet[i];
								distjet[i] = j * j + k * k;
							}
							j = 0;
							for(i = 1; i < nb_joueurs; i++) {
								if(distjet[i] < distjet[j] ) j = i;
							}
							if(distjet[j] < 400) {
								jetonp = j;
								if(perso[j].id >= 0) defperso[perso[j].id].required = false;
							}
						} else {
							j = ( (xc >> 4) - 19) / 47;
							if(j > 5) j = 5;
							k = ( (yc >> 4) - 36) / 47;
							l = k * 6 + j;
							if( (l >= 0) && (l + debut < nb_defpersos) ) {
								perso[jetonp].id = l + debut;

								defperso[perso[jetonp].id].required = true;
								testeperso(perso[jetonp].id); //Chargement dynamique perso

								i = jetonp >> 1;
								if(i * 2 == jetonp) { //A gauche
									xjet[jetonp] = j * 47 + 33;
								} else xjet[jetonp] = j * 47 + 53;
								yjet[jetonp] = k * 47 + 50 + i * 20;
								jetonp = -1;
							}
						}
					}
				}

				prevkey = true;
			} else {
				if(exKey & PSP_CTRL_TRIANGLE) { //Bouton Triangle
					if(jetonp < 0) { //Retour
						mode_menu();
						return;
					} else {
					//	j = (xc - 19) / 47;
					//	k = (yc - 36) / 47;
					//	l = k * 6 + j;
					//	if( (perso[jetonp].id < 0) || (l < 0) || (l + debut >= nb_defpersos) ) { //Pas bon, on remet en position initiale
							perso[jetonp].id = -1;
							xjet[jetonp] = xjetini[jetonp];
							yjet[jetonp] = yjetini[jetonp];
					/*	} else {
							defperso[perso[jetonp].id].required = true;

							k = (perso[jetonp].id - debut) / 6;
							j = (perso[jetonp].id - debut) - k * 6;
							i = jetonp >> 1;
							if(i * 2 == jetonp) { //A gauche
								xjet[jetonp] = j * 47 + 33;
							} else xjet[jetonp] = j * 47 + 53;
							yjet[jetonp] = k * 47 + 50 + i * 20;
						}*/
						jetonp = -1;
					}
				} else {
					if( (exKey & PSP_CTRL_START) && (rtf) ) {
						PlaySound(&sound[son_menu]);

//						db = 1 - db;
						choix_arene();
						return;
					}
				}
			}
		}
		prevkey = exKey & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE | PSP_CTRL_TRIANGLE | PSP_CTRL_START);

		for(i = 0; i < nb_joueurs; i++) {
			if(perso[i].id >= 0) {
				if( (tm >> 1) * 2 == tm) {
					if(animperso(i) || (move[defperso[perso[i].id].idcoups + perso[i].acti].vite < 1) ) {
						perso[i].acti = mp_Rand(90);
						perso[i].stat = 0;
						perso[i].stata = 0;
					}
				}
			}
		}

		if(jetonp >= 0) { //Jeton pris
			xjet[jetonp] = (xc >> 4);
			yjet[jetonp] = (yc >> 4);
			rtf = false;
			fc = explos_mainf;
		} else {
			rtf = true;
			for(i = 0; i < nb_joueurs; i++) {
				if(perso[i].id < 0) rtf = false;
			}
			if( (yc < (37 << 4) ) || (yc > (140 << 4) ) ) {
				fc = explos_maind;
			} else fc = explos_maino;
		}

//		memcpy( (unsigned short *)pg_vramtop + db * (FRAMESIZE >> 1), (unsigned short *)pg_vramtop + 2 * (FRAMESIZE >> 1), FRAMESIZE);

		sceGuStart(GU_DIRECT, gu_list);
		sceGuCopyImage(GU_PSM_5551, 0, 0, 480, 272, 512, (unsigned short *)pg_vramtop + 2 * (FRAMESIZE >> 1), 0, 0, 512, (unsigned short *)pg_vramtop + db * (FRAMESIZE >> 1) );
		sceGuFinish();

		sceGuSync(0, 0);

		if(type_jeu != type_survival) {
			if(type_jeu != type_temps) {
				sprintf(g_string, "%d", vies_jeu);
				StringGameTransBlit(db, 100 + 80, 0 + 16, g_string);
			} else StringGameTransBlit(db, 89 + 80, 0 + 16, tabduree[long_jeu] );
			sprintf(g_string, "%d", nb_joueurs);
			StringGameTransBlit(db, 194 + 80, 0 + 16, g_string);
		}

		for(i = 0; i < nb_joueurs; i++) {
			if(perso[i].id >= 0) {
				if(joueur[i].cpu) {
					FastTransBlit(db, i * 70 + 58 + 80, 141 + 16, 27, 12, (unsigned short *)btn_jcpu, COUL_TRANS);
				} else FastTransBlit(db, i * 70 + 58 + 80, 141 + 16, 27, 12, (unsigned short *)btn_jhmn, COUL_TRANS);

		//		FastTransBlit(db, i * 70 + 54 - pers[perso[i].imag].dx + 80, 210 - pers[perso[i].imag].dy + 16, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS);
				if( (perso[i].imag > 0) && (perso[i].imag < nb_perss) ) FastTransBlit(db, i * 70 + 54 - pers[perso[i].imag].dx + 80, 210 - pers[perso[i].imag].dy + 16, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS);

				if( ( (perso[i].id >= debut) && (perso[i].id < debut + 12) ) || (jetonp == i) ) affichejeton(i);
			} else {
				FastTransBlit(db, i * 70 + 58 + 80, 141 + 16, 27, 12, (unsigned short *)btn_jna, COUL_TRANS);

				affichejeton(i);
			}
		}

		temps += 19; //Pour les nombres aléatoires

		FastTransBlit(db, (xc >> 4) - expl__[fc].dx + 80, (yc >> 4) - expl__[fc].dy + 16, expl__[fc].w, expl__[fc].h, (unsigned short *)expl__[fc].image, COUL_TRANS);
		if(rtf && (tm < 10) ) FastTransBlit(db, 0 + 80, 104 + 16, expl__[explos_rtf].w, expl__[explos_rtf].h, (unsigned short *)expl__[explos_rtf].image, COUL_TRANS);

		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1); //Flip
		db = 1 - db;
		sceDisplayWaitVblankStart();
	}
}

void previewarena(int idarn) {
	int xmin, ymin;
	int xz, xzn, yz, yzn;
	int i, j;
	int k;
//	int xminb, yminb;
	int xmax, ymax;
char tmpname[256];

	int handle = -1;

	strcpy(tmpname, psp_path);
	strcat(tmpname, area[idarn].fname);

	handle = sceIoOpen(tmpname, PSP_O_RDONLY, 0777);
	if(handle >= 0) {
		sceIoLseek(handle, 85, PSP_SEEK_SET);
		sceIoRead(handle, tmpshort, 2);
		sceIoLseek(handle, tmpshort[0] * 6, PSP_SEEK_CUR);

		for(i = 0; i < area[idarn].w; i++) { //Tableau foreground
			sceIoRead(handle, &tabl[i][0], area[idarn].h * 2);
		}
		sceIoRead(handle, tablb, 54000);

		sceIoClose(handle);
	}

//	mp_InitGu(db);

	zoomf = 2183;
	zoominv = 480;

	sceGuStart(GU_DIRECT, gu_list);
	sceGuDrawBufferList(GU_PSM_5551, (void *)(FRAMESIZE * 2), 512);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
	sceGuTexMode(GU_PSM_5551, 0, 0, GU_TRUE); //textures 16-bit, swizzled
	sceGuDisable(GU_BLEND);

	i = divzoom(22 + mulzoom(80) );
	j = divzoom(251 + mulzoom(16) );
	sceGuTexImage(0, 512, 256, 512, (unsigned short *)texturefond_);
	struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
	vertices[0].u = 0;
	vertices[0].v = 0;
	vertices[0].z = 0;
	vertices[1].u = 512;
	vertices[1].v = 256;
	vertices[1].z = 0;
	vertices[0].x = i;
	vertices[0].y = j;
	vertices[1].x = i + divzoom(640);
	vertices[1].y = j + divzoom(240) + 1;
	sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);

	xmax = (area[idarn].w * 9) / 5;
	ymax = (area[idarn].h * 3) >> 1;
	zoomf = ( (area[idarn].w - 2) << 14) / 300;
	i = ( (area[idarn].h - 2) << 14) / 113;
	if(i > zoomf) { //Plus haut que large
		zoomf = i;
		zoominv = 1048576 / zoomf + 1;

		zoomfb = ( ( (area[idarn].h * 3) << 14) / 113) >> 1;
		zoominvb = 1048576 / zoomfb + 1;

		xmin = 10 + ( (300 - (area[idarn].w << 14) / zoomf) >> 1) + 80;
		ymin = 116 + 16;

//		xminb = 10 + ( (300 - (area[idarn].w << 14) / zoomf) >> 1) + 80;
//		yminb = 116 + 16;
	} else {
		zoominv = 1048576 / zoomf + 1;

		zoomfb = ( ( (area[idarn].w * 3) << 14) / 300) >> 1;
		zoominvb = 1048576 / zoomfb + 1;

		xmin = 10 + 80;
		ymin = 116 + ( (113 - (area[idarn].h << 14) / zoomf) >> 1) + 16;

//		xminb = 10 + 80;
//		yminb = 116 + ( (113 - (area[idarn].h << 14) / zoomf) >> 1) + 16;
	}

	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x909090, 0x6f6f6f);
	sceGuAlphaFunc(GU_GREATER, 0x80, 0xff);
	sceGuEnable(GU_ALPHA_TEST);

	yzn = ymin; //Background
	for(j = 0; j < ymax; j++) {
		xzn = xmin;
		yz = yzn;
		yzn = divzoomb( (j + 1) * 16) + ymin;

		for(i = 0; i < xmax; i++) {
			xz = xzn;
			xzn = divzoomb( (i + 1) * 16) + xmin;

			k = tablb[i][j];
			if(k) { //Décor à afficher
//				sceGuTexImage(0, 16, 16, decors_width, (unsigned short *)(decorsb_ + (k - 1) * 128) );
				sceGuTexImage(0, 16, 16, decors_width, (unsigned short *)(decors_ + (k - 1) * 128) );
				struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
				vertices[0].u = 0;
				vertices[0].v = 0;
				vertices[0].z = 0;
				vertices[1].u = 16;
				vertices[1].v = 16;
				vertices[1].z = 0;
				vertices[0].x = xz;
				vertices[0].y = yz;
				vertices[1].x = xzn;
				vertices[1].y = yzn;
				sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
			}
		}
	}

	sceGuDisable(GU_ALPHA_TEST);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	yzn = divzoom(16) + ymin; //Foreground
	for(j = 1; j < (area[idarn].h - 2); j++) {
		xzn = divzoom(16) + xmin;
		yz = yzn;
		yzn = divzoom( (j + 1) * 16) + ymin;

		for(i = 1; i < (area[idarn].w - 2); i++) {
			xz = xzn;
			xzn = divzoom( (i + 1) * 16) + xmin;

			k = tabl[i][j];
			if(k) { //Décor à afficher
				sceGuTexImage(0, 16, 16, decors_width, (unsigned short *)(decors_ + (k - 1) * 128) );
				struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
				vertices[0].u = 0;
				vertices[0].v = 0;
				vertices[0].z = 0;
				vertices[1].u = 16;
				vertices[1].v = 16;
				vertices[1].z = 0;
				vertices[0].x = xz;
				vertices[0].y = yz;
				vertices[1].x = xzn;
				vertices[1].y = yzn;
				sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
			}
		}
	}
//	sceGuDrawBufferList(GU_PSM_5551, (void *)(db * FRAMESIZE), 512);

	sceGuFinish();

	sceGuSync(0, 0);

	zoomf = 8192;
	zoominv = 128;
}

void choix_arene() {
	unsigned int exKey;
	unsigned int prevkey = 0;
	int stat = 0;
	int dire = 0;
	int i, j;
	int changed = true;
	int rptime = 20;

	strcpy(psp_path, game_path);

	waitnokey();
	mp_ClrScr(2, 0x0000);
	mp_FastBlit(2, 80, 16, 320, 240, (unsigned short *)image_area);
	testefond(num_area);
	if(num_area > 0) testefond(num_area - 1);
	if(num_area < nb_areas - 1) testefond(num_area + 1);

	mp_InitGu(db);

	PreZoom( (unsigned short *)bmps[area[num_area].idfond].image);
	swizzletex( (unsigned short *)bmpbuffer, texturefond_, 512, 256);
	previewarena(num_area);
	sceGuDisplay(GU_TRUE);

	while(1) { //Boucle du menu
		exKey = mp_GetKey();

		if( (!prevkey) || (rptime == 0) ) {
			if( (exKey & PSP_CTRL_LEFT) || (mp_keys.Lx < 96) ) { //Gauche
				if(num_area > 0) {
					PlaySound(&sound[son_menu]);

					stat = 40;
					num_area--;
					dire = -1;

					if(num_area < nb_areas - 2) area[num_area + 2].required = false;
					if(num_area > 0) {
						testefond(num_area - 1);
						area[num_area - 1].required = true;
					}

					changed = true;
					rptime++;
				}
			} else {
				if( (exKey & PSP_CTRL_RIGHT) || (mp_keys.Lx > 160) ) { //Droite
					if(num_area < nb_areas - 1) {
						PlaySound(&sound[son_menu]);

						stat = 40;
						num_area++;
						dire = 1;

						if(num_area > 1) area[num_area - 2].required = false;
						if(num_area < nb_areas - 1) {
							testefond(num_area + 1);
							area[num_area + 1].required = true;
						}

						changed = true;
						rptime++;
					}
				} else rptime = 20;
			}
		} else {
			if(prevkey) rptime--;
		}

		if(!prevkey) {
			if(exKey & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE | PSP_CTRL_START) ) { //Bouton X / O / Start
				PlaySound(&sound[son_menu]);

				sceGuDisplay(GU_FALSE);

				init_moteur();
				return;
			} else {
				if(exKey & PSP_CTRL_TRIANGLE) { //Bouton Triangle
					choix_persos(0);

					sceGuDisplay(GU_FALSE);

					return;
				}
			}
		}
		prevkey = exKey || ( ( (mp_keys.Lx + 32) >> 6) != 2);

		if(changed) {
			if(num_area > 0) { //gauche
				PreZoom( (unsigned short *)bmps[area[num_area - 1].idfond].image);
				swizzletex( (unsigned short *)bmpbuffer, textbuf_[0], 512, 256);
			}

			if(num_area < nb_areas - 1) { //droite
				PreZoom( (unsigned short *)bmps[area[num_area + 1].idfond].image);
				swizzletex( (unsigned short *)bmpbuffer, textbuf_[1], 512, 256);
			}

			PreZoom( (unsigned short *)bmps[area[num_area].idfond].image);
			swizzletex( (unsigned short *)bmpbuffer, texturefond_, 512, 256);

			changed = false;
		}

//		memcpy( (unsigned short *)pg_vramtop + db * (FRAMESIZE >> 1), (unsigned short *)pg_vramtop + 2 * (FRAMESIZE >> 1), FRAMESIZE);

		sceGuStart(GU_DIRECT, gu_list);

		sceGuCopyImage(GU_PSM_5551, 0, 0, 480, 272, 512, (unsigned short *)pg_vramtop + 2 * (FRAMESIZE >> 1), 0, 0, 512, (unsigned short *)pg_vramtop + db * (FRAMESIZE >> 1) );

		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
		sceGuDisable(GU_BLEND);

		if(num_area > 0) { //gauche
			i = divzoom(160 + (dire * stat * 16) + 640);
			j = divzoom(360 + 128);
			sceGuTexImage(0, 512, 256, 512, (unsigned short *)textbuf_[0]);
			struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
			vertices[0].u = 0;
			vertices[0].v = 0;
			vertices[0].z = 0;
			vertices[1].u = 512;
			vertices[1].v = 256;
			vertices[1].z = 0;
			vertices[0].x = i;
			vertices[0].y = j;
			vertices[1].x = i + divzoom(640);
			vertices[1].y = j + divzoom(240);
			sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
		}

		i = divzoom(960 + (dire * stat * 16) + 640);
		j = divzoom(360 + 128);
		sceGuTexImage(0, 512, 256, 512, (unsigned short *)texturefond_);
		struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
		vertices[0].u = 0;
		vertices[0].v = 0;
		vertices[0].z = 0;
		vertices[1].u = 512;
		vertices[1].v = 256;
		vertices[1].z = 0;
		vertices[0].x = i;
		vertices[0].y = j;
		vertices[1].x = i + divzoom(640);
		vertices[1].y = j + divzoom(240);
		sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);

		if(num_area < nb_areas - 1) { //droite
			i = divzoom(1760 + (dire * stat * 16) + 640);
			j = divzoom(360 + 128);
			sceGuTexImage(0, 512, 256, 512, (unsigned short *)textbuf_[1]);
			struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
			vertices[0].u = 0;
			vertices[0].v = 0;
			vertices[0].z = 0;
			vertices[1].u = 512;
			vertices[1].v = 256;
			vertices[1].z = 0;
			vertices[0].x = i;
			vertices[0].y = j;
			vertices[1].x = i + divzoom(640);
			vertices[1].y = j + divzoom(240);
			sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
		}
		sceGuEnable(GU_BLEND);

		sceGuFinish();

		sceGuSync(0, 0);

		if(stat == 0) {
			if(num_area > 0) StringTransBlit(db, 60 - strlen(area[num_area - 1].name) * 3 + 80, 63 + 16, area[num_area - 1].name);
			StringTransBlit(db, 160 - strlen(area[num_area].name) * 3 + 80, 63 + 16, area[num_area].name);
			if(num_area < nb_areas - 1) StringTransBlit(db, 260 - strlen(area[num_area + 1].name) * 3 + 80, 63 + 16, area[num_area + 1].name);
		}

		if(stat > 20) {
			stat -= 3;
		} else {
			if(stat > 10) {
				stat -= 2;
			} else {
				if(stat > 0) {
					stat--;
					if(stat == 0) previewarena(num_area);
				}
			}
		}

//		GpSurfaceSet(&gpDraw[db] ); //Flip
	//	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	//	db = 1 - db;
		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();

		db = 1 - db;
	}
}

void fin_jeu() {
	int i, j;
	unsigned int exKey;
	int sauvnj = nb_joueurs;

	waitnokey();
	mp_ClrScr(2, 0x0000);
	mp_FastBlit(2, 80, 16, 320, 240, (unsigned short *)image_autre);
	mp_FastBlit(2, 102, 36, 56, 15, (unsigned short *)tit_results);

	mp_InitGu(db);

	sceGuDisplay(GU_TRUE);

	if(type_jeu != type_survival) {
		for(i = 0; i < nb_joueurs; i++) {
			classm[i] = 0;
			for(j = 0; j < nb_joueurs; j++) {
				if(joueur[j].score > joueur[i].score) classm[i]++;
			}
			if(classm[i] == 0) {
				perso[i].acti = 28;
			} else perso[i].acti = 29;
			perso[i].stat = 0;
			perso[i].stata = 0;
		}
	} else {
		if(joueur[0].kills > maxsurvival) {
			maxsurvival = joueur[0].kills;
			classm[0] = 0;
			saveprefs();
			perso[0].acti = 28;
		} else {
			classm[0] = 1;
			perso[0].acti = 29;
		}
		perso[0].stat = 0;
		perso[0].stata = 0;
		nb_joueurs = 1;
	}

	while(1) { //Boucle du menu
		exKey = mp_GetKey();
		if(exKey) {
			PlaySound(&sound[son_menu]);

			nb_joueurs = sauvnj;

			sceGuDisplay(GU_FALSE);
			return;
		}

		for(i = 0; i < nb_joueurs; i++) {
			if(db == 0) animperso(i);
	//		animperso(i);
		}

//		memcpy( (unsigned short *)pg_vramtop + db * (FRAMESIZE >> 1), (unsigned short *)pg_vramtop + 2 * (FRAMESIZE >> 1), FRAMESIZE);
	//	mp_FontBlit(db, 0, 0, 480, 272, (unsigned short *)pg_vramtop + 2 * (FRAMESIZE >> 1), 512);

		sceGuStart(GU_DIRECT, gu_list);
		sceGuCopyImage(GU_PSM_5551, 0, 0, 480, 272, 512, (unsigned short *)pg_vramtop + 2 * (FRAMESIZE >> 1), 0, 0, 512, (unsigned short *)pg_vramtop + db * (FRAMESIZE >> 1) );
		sceGuFinish();

		sceGuSync(0, 0);

		sceGuStart(GU_DIRECT, gu_list);

		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
		sceGuTexMode(GU_PSM_5551, 0, 0, 0); //textures 16-bit, non swizzled

		for(i = 0; i < nb_joueurs; i++) {
			switch(classm[i] ) {
			case 0: //1er
				zoomf = 512;
//				zoominv = 2048;

//				ZoomTransBlit(db, mulzoom(i * 80 + 40 + 80) - pers[perso[i].imag].dx, mulzoom(160 + 16) - pers[perso[i].imag].dy, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS, 1);
				break;
			case 1: //2ème
				zoomf = 768;
//				zoominv = 1365;

//				ZoomTransBlit(db, mulzoom(i * 80 + 40 + 80) - pers[perso[i].imag].dx, mulzoom(160 + 16) - pers[perso[i].imag].dy, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS, 1);
				break;
			default: //Après
//				FastTransBlit(db, i * 80 + 40 - pers[perso[i].imag].dx + 80, 160 - pers[perso[i].imag].dy + 16, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS);
				zoomf = 1023;
//				zoominv = 1026;

//				ZoomTransBlit(db, mulzoom(i * 80 + 40 + 80) - pers[perso[i].imag].dx, mulzoom(160 + 16) - pers[perso[i].imag].dy, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS, 1);
			}

			zoominv = 1048576 / zoomf + 1;

			ZoomTransBlit(db, mulzoom(i * 80 + 40 + 80) - pers[perso[i].imag].dx, mulzoom(160 + 16) - pers[perso[i].imag].dy, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS, 1);
		}

		sceGuFinish();

		sceGuSync(0, 0);
		edram_ptr = (unsigned short *)textbuf_[0];

		for(i = 0; i < nb_joueurs; i++) {
			sprintf(g_string, "Player %d:", i + 1);
			StringTransBlit(db, i * 80 + 10 + 80, 80 + 16, g_string);
			sprintf(g_string, "Kills: %d", joueur[i].kills);
			StringTransBlit(db, i * 80 + 20 + 80, 95 + 16, g_string);
			if(type_jeu != type_survival) {
				sprintf(g_string, "Deaths: %d", joueur[i].morts);
				StringTransBlit(db, i * 80 + 20 + 80, 110 + 16, g_string);
				sprintf(g_string, "Scds: %d", joueur[i].scds);
				StringTransBlit(db, i * 80 + 20 + 80, 125 + 16, g_string);
				sprintf(g_string, "Score: %d", joueur[i].score);
				StringTransBlit(db, i * 80 + 20 + 80, 140 + 16, g_string);

				FastTransBlit(db, i * 80 + 29 + 80, 165 + 16, expl__[explos_rondclass].w, expl__[explos_rondclass].h, (unsigned short *)expl__[explos_rondclass].image, COUL_TRANS);
				sprintf(g_string, "%d", classm[i] + 1);
				StringGameTransBlit(db, i * 80 + 35 + 80, 165 + 16, g_string);
			}
		}

//		GpSurfaceSet(&gpDraw[db] ); //Flip
	//	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	//	db = 1 - db;
		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();

		db = 1 - db;
	}
}

void credits() {
	unsigned int exKey;

	waitnokey();
	mp_FastBlit(db, 80, 16, 320, 240, (unsigned short *)image_autre);
	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	db = 1 - db;
	sceDisplayWaitVblankStart();
	mp_FastBlit(db, 80, 16, 320, 240, (unsigned short *)image_autre);

	animwrite(40 + 80, 40 + 16, "Programming: mATkEUpON");
	animwrite(40 + 80, 60 + 16, "In-game gfx: mATkEUpON");
	animwrite(40 + 80, 80 + 16, "Menus gfx: EvilDragon & Solarice");
	animwrite(40 + 80, 100 + 16, "Splash screen: EvilDragon");
//	animwrite(40 + 80, 120 + 16, "Thx to CHN for his modlib");
	animwrite(40 + 80, 120 + 16, "Thx to panpan for the icon");

//	if(chatboard) {
//		animwrite(40 + 80, 145 + 16, "Chatboard detected");
//	} else animwrite(40 + 80, 145 + 16, "No chatboard plugged");
	animwrite(40 + 80, 145 + 16, "Find new characters at www.psparchive.de");

	sprintf(g_string, "Your survival high score: %d kills", maxsurvival);
	animwrite(40 + 80, 170 + 16, g_string);
	int version = sceKernelDevkitVersion();
	if(version == 0x01000300) {
		animwrite(40 + 80, 190 + 16, "You have a PSP version 1.00");
	} else animwrite(40 + 80, 190 + 16, "You have a PSP version 1.50");
//sprintf(g_string, "%d %d %d %d %d %d %d", nb_moves, nb_clists, nb_defpersos, nb_areas, nb_bmps, nb_paks, nb_perss);
//animwrite(40, 205, g_string);

	animwrite(40 + 80, 220 + 16, "Press Start to continue");

	mp_FastBlit(db, 102, 36, 56, 14, (unsigned short *)tit_credits);
	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	db = 1 - db;
	sceDisplayWaitVblankStart();
	mp_FastBlit(db, 102, 36, 56, 14, (unsigned short *)tit_credits);

	while(1) { //Boucle du menu
		exKey = mp_GetKey();
		if(exKey & PSP_CTRL_START) {
			PlaySound(&sound[son_menu]);

			gere_menus(menu_options);
			return;
		}

		sceDisplayWaitVblankStart();
	}
}

void majvariable(int id) {
	if(item[id].var) {
		int x = item[id].dx + strlen(item[id].texte) * font02_w + 10;
		int w = 320 - x;

		mp_FontBlit(db, x + 80, item[id].dy + 16, w, font02_h, (unsigned short *)image_autre + x + item[id].dy * 320, 320);
		mp_FontBlit(1 - db, x + 80, item[id].dy + 16, w, font02_h, (unsigned short *)image_autre + x + item[id].dy * 320, 320);

		StringTransBlit(db, x + 80, item[id].dy + 16, varitem[item[id].var].valeur[*varitem[item[id].var].var] );
		StringTransBlit(1 - db, x + 80, item[id].dy + 16, varitem[item[id].var].valeur[*varitem[item[id].var].var] );
	}
}

void gere_menus(int id) {
	int i;
	int j = 0; //Mouvement curseur
	int x, y;
	int dy = 0;
	int w;
	int dw = 0;
	unsigned int exKey;
	unsigned int prevkey = 0;

	cursmenu[0].x = 0;
	cursmenu[1].x = 0;

	waitnokey();
	mp_ClrScr(db, 0x0000);
	mp_FastBlit(db, 80, 16, 320, 240, (unsigned short *)image_autre);
	sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
	db = 1 - db;
	sceDisplayWaitVblankStart();
	mp_ClrScr(db, 0x0000);
	mp_FastBlit(db, 80, 16, 320, 240, (unsigned short *)image_autre);

//	animwrite(54, 50, menu[id].titre); //Titre caractère par caractère

	for(i = menu[id].deb; i <= menu[id].fin; i++) { //Menu lettre par lettre
		animwrite(item[i].dx + 80, item[i].dy + 16, item[i].texte);
	}

	i = menu[id].deb; //Item par défaut
	x = item[i].dx - 2;
	y = item[i].dy - 2;
	w = strlen(item[i].texte) * font02_w + 4;

	switch(id) {
	case menu_options:
		majvariable(0);
		majvariable(1);
		majvariable(2);
		mp_FastBlit(db, 102, 36, 58, 16, (unsigned short *)tit_options);
		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
		mp_FastBlit(db, 102, 36, 58, 16, (unsigned short *)tit_options);
	}

	prevkey = 0;
	while(1) { //Boucle du menu
		exKey = mp_GetKey();

		if(cursmenu[db].x) mp_FastBlit(db, cursmenu[db].x + 80, cursmenu[db].y + 16, cursmenu[db].w, cursmenu[db].h, (unsigned short *)cursmenu[db].tmpbmp);

		if(!prevkey) {
			if( (exKey & PSP_CTRL_UP) || (mp_keys.Ly < 96) ) { //Haut
				i = item[i].prev;
				dy = (item[i].dy - 2 - y) / 6;
				dw = strlen(item[i].texte);
				dw = (dw  * font02_w + 4 - w) / 6;
				j = 7;

				PlaySound(&sound[son_menu]);
			} else {
				if( (exKey & PSP_CTRL_DOWN) || (mp_keys.Ly > 160) ) { //Bas
					i = item[i].next;
					dy = (item[i].dy - 2 - y) / 6;
					dw = strlen(item[i].texte);
					dw = (dw  * font02_w + 4 - w) / 6;
					j = 7;

					PlaySound(&sound[son_menu]);
				}
			}

			if(exKey & (PSP_CTRL_CROSS | PSP_CTRL_CIRCLE) ) { //Bouton X / O
				PlaySound(&sound[son_menu]);
				itemclig();

				switch(i) {
				case 0: //Difficulty
					difficulte++;
					if(difficulte > dif_difficile) difficulte = dif_facile;
					majvariable(0);
					break;
				case 1: //Transparency
					transparency++;
					if(transparency > max_transparency) transparency = min_transparency;
				//	luminosite++;
				//	if(luminosite > max_luminosite) luminosite = min_luminosite;
//					setpalette(PAL[palette_jeu + luminosite] );
					majvariable(1);
					break;
				case 2: //Volume
					volmusique++;
					if(volmusique > vol_maxi) volmusique = vol_nmusic;
#ifdef	__inc_musique__
					mf->volume = volmusique * 24;
#endif
					majvariable(2);
					break;
				case 3: //Save
					saveprefs();
					break;
				case 4: //Credits
					credits();
					return;
				default: //Back
					return;
				}
			} else {
				if(exKey & PSP_CTRL_TRIANGLE) return; //Bouton Triangle
			}
		}
		prevkey = exKey || ( ( (mp_keys.Lx + 32) >> 6) != 2) || ( ( (mp_keys.Ly + 32) >> 6) != 2);

		if(j) { //Déplacement du curseur
			if(j-- > 1) { //Animation
				y += dy;
				w += dw;
			} else { //Arrivé
				y = item[i].dy - 2;
				w = strlen(item[i].texte) * font02_w + 4;
			}
		}

		cursmenu[db].x = x; //On retrace le nouveau
		cursmenu[db].y = y;
		cursmenu[db].w = w;
		cursmenu[db].h = 16;
		SaveBitmap(db, x + 80, y + 16, w, 16, (unsigned short *)cursmenu[db].tmpbmp);
		hline(db, x + 80, y + 16, w - 1, coul_blanc);
		hline(db, x + 80, y + 15 + 16, w - 1, coul_blanc);
		vline(db, x + 80, y + 1 + 16, 14, coul_blanc);
		vline(db, x + w - 1 + 80, y + 16, 16, coul_blanc);

		sceDisplaySetFrameBuf( (char *)pg_vramtop + db * FRAMESIZE, 512, 1, 1);
		db = 1 - db;
		sceDisplayWaitVblankStart();
	}
}

//Assignations

void _assignation_menus(int id, int deb, int fin) { //, const char *texte) {
	menu[id].deb = deb;
	menu[id].fin = fin;
//	strcpy(menu[id].titre, texte);
}

void _assignation_items(int id, int prev, int next, int var, int dx, int dy, const char *texte) {
	item[id].prev = prev;
	item[id].next = next;
	item[id].var = var;
	item[id].dx = dx;
	item[id].dy = dy;
	strcpy(item[id].texte, texte);
}

void _assignation_varitems(int id, int var, const char *texte) {
	strcpy(varitem[id].valeur[var], texte);
}

void assigne_menus() {
	//Menu mode jeu**************************************************

	_assignation_menus(menu_options, 0, 5); //, "Options");

	_assignation_items(0, 5, 1, 1, 44, 100, "Difficulty:");

	_assignation_items(1, 0, 2, 2, 44, 115, "Back transparency:");

	_assignation_items(2, 1, 3, 3, 44, 130, "Music volume:");

	_assignation_items(3, 2, 4, 0, 44, 145, "Save preferences");

	_assignation_items(4, 3, 5, 0, 44, 160, "Credits");

	_assignation_items(5, 4, 0, 0, 44, 205, "Back");

	//Varitem Difficulty**************************************************

	_assignation_varitems(1, 0, "Easy");

	_assignation_varitems(1, 1, "Medium");

	_assignation_varitems(1, 2, "Hard");

	varitem[1].var = &difficulte;

	//Varitem Transparency**************************************************

	_assignation_varitems(2, 0, "0%");

	_assignation_varitems(2, 1, "10%");

	_assignation_varitems(2, 2, "20%");

	_assignation_varitems(2, 3, "30%");

	_assignation_varitems(2, 4, "40%");

	_assignation_varitems(2, 5, "50%");

	_assignation_varitems(2, 6, "60%");

	_assignation_varitems(2, 7, "70%");

	_assignation_varitems(2, 8, "80%");

	_assignation_varitems(2, 9, "90%");

	varitem[2].var = &transparency;

	//Varitem Volume**************************************************

	_assignation_varitems(3, 0, "No music");

	_assignation_varitems(3, 1, "Low");

	_assignation_varitems(3, 2, "Medium");

	_assignation_varitems(3, 3, "High");

	varitem[3].var = &volmusique;
}
