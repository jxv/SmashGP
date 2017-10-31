void wait(long time) {
//	time += GpTickCountGet();
//	while(time > GpTickCountGet() );
	sceKernelDelayThread(time * 1000);
}

void changecamera(int xs, int ys, int zf) {
	int dx = scrw;
	int dy = scrh;

	if(zf <= zoommax) {
		if(zf >= zoommin) {
			zoomf = zf;
		} else zoomf = zoommin;
	} else zoomf = zoommax;
	zoominv = 1048576 / zoomf + 1;
	scrw = mulzoom(screen_width);
	scrh = mulzoom(screen_height);
	map_xmax = (area[num_area].w - 2) * 16 - scrw - 1;
	map_ymax = (area[num_area].h - 2) * 16 - scrh - 1;
	disp_width = (scrw >> 4) + 2;
	disp_height = (scrh >> 4) + 2;

	if(xs) {
		xscreen = xs;
		yscreen = ys;
	} else {
		dx = (scrw - dx) >> 1;
		dy = (scrh - dy) >> 1;
		xscreen -= dx;
		yscreen -= dy;
	}
	if(xscreen < 16) {
		xscreen = 16;
	} else {
		if(xscreen > map_xmax) xscreen = map_xmax;
	}
	if(yscreen < 16) {
		yscreen = 16;
	} else {
		if(yscreen > map_ymax) yscreen = map_ymax;
	}
	xmap = xscreen >> 4;
	ymap = yscreen >> 4;

	zoomfb = (zoomf * 3) >> 1;
	zoominvb = 1048576 / zoomfb + 1;
	xscreenb = xscreen + 350;
	yscreenb = yscreen + 350;
	xmapb = xscreenb >> 4;
	ymapb = yscreenb >> 4;
	scrwb = mulzoomb(screen_width);
	scrhb = mulzoomb(screen_height);
	disp_widthb = (scrwb >> 4) + 2;
	disp_heightb = (scrhb >> 4) + 2;
}

void sautperso(int i) {
	int j = defperso[perso[i].id].idcoups + perso[i].acti;

	if(perso[i].acti >= 0) {
		if( (perso[i].acti == 21) || (perso[i].acti == 23) ) { //On se relève
			perso[i].acti = 22;
			perso[i].stat = 0;
			perso[i].stata = 0;
		} else {
			if(perso[i].nsauts > 0) {
				if( (perso[i].acti < 20) || ( (move[j].type == move_deplace) && (perso[i].stat == move[j].nani) ) ) {
					if(perso[i].nsauts > 1) { //Saut 1
						perso[i].acti = 4;
					} else perso[i].acti = 5; //Saut 2
					perso[i].stat = 0;
					perso[i].stata = 0;
					j = defperso[perso[i].id].idcoups + perso[i].acti;
					perso[i].dy += move[j].ay;
					if(perso[i].dy < 0) { //Ca suffit
						perso[i].dy = move[j].ay;
					} else perso[i].dy = move[j].ay >> 1;
					perso[i].nsauts--;

					if(move[defperso[perso[i].id].idcoups + perso[i].acti].idson) PlaySoundFX(&defperso[perso[i].id].son[move[defperso[perso[i].id].idcoups + perso[i].acti].idson], 0);
				}
			}
		}
	}
}

void deplaceperso(int i, int cote, int ddir) {
	int j;

	if( ( (perso[i].y >> 14) != ( (perso[i].y + 1024) >> 14) ) && (typedecor[tabl[perso[i].x >> 14][(perso[i].y >> 14) + 1]] < 1) ) { //Au sol - contrôle vitesse
		if(!perso[i].protect) {
			switch(perso[i].acti) {
			case 0: //Rien, on marche
			case 6: //Rien, en déséquilibre
				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			case 1: //Marche
				if(perso[i].cote != cote) { //On se retourne
					if(ddir == 0) {
						perso[i].acti = 13;
						perso[i].stat = 0;
						perso[i].stata = 0;
						perso[i].cote = cote;
					}
				} else { //On avance
					perso[i].dx += (move[defperso[perso[i].id].idcoups + 1].ax * cote);
					if(perso[i].dx * cote > (perso[i].vite >> 1) ) perso[i].dx = (perso[i].vite >> 1) * cote;
				}
				break;
			case 2: //Court
				if(perso[i].cote != cote) { //Freinage
					perso[i].acti = 10;
					perso[i].stat = 0;
					perso[i].stata = 0;
					perso[i].cote = cote;
				} else {
					perso[i].dx += (move[defperso[perso[i].id].idcoups + 2].ax * cote);
					if(perso[i].dx * cote > perso[i].vite) perso[i].dx = perso[i].vite * cote;
				}
				break;
			case 3: //Baissé
				perso[i].cote = cote;
			}

			perso[i].bouge = true;
		} else {
			if( (perso[i].acti == 3) && (ddgauche || dddroite) ) { //Baissé et en protection - roulade ??
				for(j = 0; j < nb_joueurs; j++) {
					if( (i != j) && (perso[i].y >> 14) == (perso[j].y >> 14) ) {
						if( ( (perso[j].x - perso[i].x) * cote > 0) && ( (perso[j].x - perso[i].x) * cote < 65536) ) { //Roulade
							perso[i].acti = 9;
							perso[i].stat = 0;
							perso[i].stata = 0;
							perso[i].cote = cote;
							perso[i].dx += (move[defperso[perso[i].id].idcoups + 9].ax * cote);
							return;
						}
					}
				}
			}
		}
	} else {//En l'air
		if(perso[i].acti != 8) { //Pas qd accroché
			if(perso[i].acti >= 0) { //Pas assommé, contrôle de vitesse
				if(move[defperso[perso[i].id].idcoups + perso[i].acti].type < move_deplace) {
					perso[i].dx += (move[defperso[perso[i].id].idcoups + 4].ax * cote);
					if(perso[i].dx * cote > perso[i].vite) perso[i].dx = perso[i].vite * cote;
				}
			} else perso[i].dx += (move[defperso[perso[i].id].idcoups + 5].ax * cote);

			perso[i].bouge = true;
		}
	}
}

void utilobjet(int i, int smash) {
	int ns = true;

	if(smash) { //Smash avec l'objet
		if( (defobjet[objet[perso[i].objet].id].role == role_batte) || (defobjet[objet[perso[i].objet].id].role == role_batonetoile) ) {
			perso[i].acti = 89; //Homerun !
			perso[i].stat = 0;
			perso[i].stata = 0;
			ns = false;

			if(defobjet[objet[perso[i].objet].id].role == role_batonetoile) PlaySoundFX(&sound[son_smbatet], 0);
		}
	}

	if(ns) { //Pas de smash
		switch(defobjet[objet[perso[i].objet].id].role) {
		case role_batonetoile:
			PlaySoundFX(&sound[son_cbatet], 0);
		case role_batte:
			perso[i].acti = 85;
			break;
		case role_marteau:
			perso[i].acti = 86;
			break;
		case role_pistolet:
			perso[i].acti = 87;
			break;
		case role_fleur:
			perso[i].acti = 88;
			break;
//		case role_aucun:
//		case role_explose:
//		case role_boite:
//		case role_caisse:
//		case role_pokemon:
//		case role_bumper:
//		case role_carapr:
//		case role_carapv:
//		case role_bobomb:
		default:
			perso[i].acti = 82; //On les lance

			PlaySoundFX(&sound[son_objlance], 0);
		}

		perso[i].stat = 0;
		perso[i].stata = 0;
	}
}

void gestion_touches() {
//	int exKey;
	unsigned int exKey;
	int ztmp;

//	GpKeyGetEx(&exKey);
	exKey = mp_GetKey();

	if(exKey & (PSP_CTRL_START | PSP_CTRL_HOME) ) { //Bouton Start / Home
		if(bsel) {
			cond_timer = 0;
			mort_subite = true;
			pause = false;
		} else {
			if(!bstart) {
//				pause = !pause;
				if(pause) {
					if(exKey & PSP_CTRL_START) pause = false;
				} else pause = true;

				bstart = true;
			}
		}
	} else bstart = false;

	if(pause) {
		if(exKey & PSP_CTRL_SELECT) { //Bouton Select
			bsel = 1;
		} else bsel = 0;

		if( (exKey & PSP_CTRL_UP) || (mp_keys.Ly < 96) ) { //Haut
			changecamera(xscreen, yscreen - 10, zoomf);
		} else {
			if( (exKey & PSP_CTRL_DOWN) || (mp_keys.Ly > 160) ) { //Bas
				changecamera(xscreen, yscreen + 10, zoomf);
			}
		}
		if( (exKey & PSP_CTRL_LEFT) || (mp_keys.Lx < 96) ) { //Gauche
			changecamera(xscreen - 10, yscreen, zoomf);
		} else {
			if( (exKey & PSP_CTRL_RIGHT) || (mp_keys.Lx > 160) ) { //Droite
				changecamera(xscreen + 10, yscreen, zoomf);
			}
		}

		if(exKey & (PSP_CTRL_CROSS | PSP_CTRL_SQUARE) ) { //Bouton X / Carré
			changecamera(0, 0, zoomf - 32);
		} else {
			if(exKey & (PSP_CTRL_CIRCLE | PSP_CTRL_TRIANGLE) ) { //Bouton O / Triangle
				changecamera(0, 0, zoomf + 32);
			}
		}
	} else {
		if(!joueur[0].cpu) { //********************************************* PSP *************************************************************
			if(exKey & (PSP_CTRL_SQUARE | PSP_CTRL_TRIANGLE) ) { //Bouton Carré / Triangle
				if(bsel < 1) { //Un coup
					sautperso(0);
					bsel = 10;
				} else bsel--;
			} else bsel = 0;

			if(exKey & PSP_CTRL_RTRIGGER) { //Bouton R
				br = true;
			} else br = false;

			if(exKey & PSP_CTRL_LTRIGGER) { //Bouton L
				if(!perso[0].protect) {
					if( ( (perso[0].acti == 0) || (perso[0].acti == 1) || (perso[0].acti == 3) || (perso[0].acti == 6) ) && (perso[0].shield > 0) ) {
						perso[0].protect = true;

						PlaySoundFX(&sound[son_protect], 0);
					}
				}
				bl = true;
			} else {
				bl = false;
				perso[0].protect = false;
			}

			if( (exKey & PSP_CTRL_UP) || (mp_keys.Ly < 32) ) { //Haut
				if(!dhaut) { //Un coup
					if(!ddhaut) ddhaut = 15;
					dhaut = true;
				} else { //Continu
					ddhaut >>= 1;
					if(ddhaut == 0) {
						if(tmsaut < 1) {
							sautperso(0);
							tmsaut = 15;
						} else tmsaut--;
					}
				}
			} else {
				if(dhaut && (ddhaut > 0) ) { //Simple clic dir
					if(!ba && !bb) sautperso(0); //Pas d'action, on saute
				}
				dhaut = false;
				if(ddhaut > 0) ddhaut--;
				tmsaut = 0;

				if( (exKey & PSP_CTRL_DOWN) || (mp_keys.Ly > 224) ) { //Bas
					if(!dbas) { //Un coup
						if(ddbas) { //Double clic
							if( (perso[0].y >> 14) != ( (perso[0].y + 1024) >> 14) ) { //Pile sur une case
								if( (typedecor[tabl[perso[0].x >> 14][(perso[0].y >> 14) + 1]] < 1) && (typedecor[tabl[perso[0].x >> 14][(perso[0].y >> 14) + 2]] > 0) ) { //Au sol - on peut descendre
									perso[0].acti = 7;
									perso[0].stat = 0;
									perso[0].stata = 0;
									perso[0].y += 17409;
								}
							}
						} else {
							if(perso[0].acti == 8) { //On se lache
								perso[0].acti = 0;
								perso[0].stat = 0;
								perso[0].stata = 0;
							}
							ddbas = 15;
						}
						dbas = true;
					} else { //Enfoncé
						if( ( (perso[0].y >> 14) != ( (perso[0].y + 1024) >> 14) ) && (typedecor[tabl[perso[0].x >> 14][(perso[0].y >> 14) + 1]] < 1) ) { //Au sol - on peut se baisser
							if( ( (perso[0].acti > -1) && (perso[0].acti < 3) && (perso[0].dy == 0) ) || (perso[0].acti == 6) ) { //On se baisse
								perso[0].acti = 3;
								perso[0].stat = 0;
								perso[0].stata = 0;
							}
						}
					}
				} else {
					dbas = false;
					if(ddbas > 0) ddbas--;
					if(perso[0].acti == 3) { //On se relève
						perso[0].acti = 1;
						perso[0].stat = 0;
						perso[0].stata = 0;
					}
				}
			}

			if( (exKey & PSP_CTRL_LEFT) || (mp_keys.Lx < 96) ) { //Gauche
				if(!dgauche || (mp_keys.Lx < 32) ) { //Un coup
					if(ddgauche || (mp_keys.Lx < 32) ) { //Double clic / à fond
						if( (perso[0].acti == 1) && (perso[0].cote < 0) ) { //On commence à courir
							perso[0].acti = 2;
							perso[0].stat = 0;
							perso[0].stata = 0;
							perso[0].dx += (move[defperso[perso[0].id].idcoups + 2].ax * -2);
						}
						if(exKey & PSP_CTRL_LEFT) ddgauche = 0;
					} else { //Simple clic
						if( (perso[0].acti == 0) && !perso[0].protect) { //On commence à marcher
							perso[0].acti = 1;
							perso[0].stat = 0;
							perso[0].stata = 0;
						}
						ddgauche = 15;
					}
					dgauche = true;
				} else {
					if(ddgauche > 0) ddgauche--; //else ddgauche >>= 1;
				}
				deplaceperso(0, -1, ddgauche);
				dddroite = 0;
			} else {
				if(dgauche && (ddgauche > 0) ) { //Simple clic dir
					if(!ba && !bb) deplaceperso(0, -1, 0); //Pas d'action, on se retourne
				}
				dgauche = false;
				if(ddgauche > 0) ddgauche--;

				if( (exKey & PSP_CTRL_RIGHT) || (mp_keys.Lx > 160) ) { //Droite
					if(!ddroite || (mp_keys.Lx > 224) ) { //Un coup
						if(dddroite || (mp_keys.Lx > 224) ) { //Double clic / à fond
							if( (perso[0].acti == 1) && (perso[0].cote > 0) ) { //On commence à courir
								perso[0].acti = 2;
								perso[0].stat = 0;
								perso[0].stata = 0;
								perso[0].dx += (move[defperso[perso[0].id].idcoups + 2].ax * 2);
							}
							if(exKey & PSP_CTRL_RIGHT) dddroite = 0;
						} else { //Simple clic
							if( (perso[0].acti == 0) && !perso[0].protect) { //On commence à marcher
								perso[0].acti = 1;
								perso[0].stat = 0;
								perso[0].stata = 0;
							}
							dddroite = 15;
						}
						ddroite = true;
					} else {
						if(dddroite > 0) dddroite--; // else dddroite >>= 1;
					}
					deplaceperso(0, 1, dddroite);
					ddgauche = 0;
				} else {
					if(ddroite && (dddroite > 0) ) { //Simple clic dir
						if(!ba && !bb) deplaceperso(0, 1, 0); //Pas d'action, on se retourne
					}
					ddroite = false;
					if(dddroite > 0) dddroite--;
				}
			}

			if(exKey & PSP_CTRL_CROSS) { //Bouton X
				if(!ba && !perso[0].protect) { //Un coup
					if( (perso[0].acti >= 0) && (perso[0].acti < 8) ) {
						if(perso[0].objet) { //On a un objet
							if(br) { //On le lance
								if(dddroite || ddgauche) { //Sur le côté
									perso[0].acti = 82;
									perso[0].stat = 0;
									perso[0].stata = 0;

									if(dddroite) {
										perso[0].cote = 1;
									} else perso[0].cote = -1;

									PlaySoundFX(&sound[son_objlance], 0);
								} else {
									if(ddhaut) { //En haut
										perso[0].acti = 83;
										perso[0].stat = 0;
										perso[0].stata = 0;

										PlaySoundFX(&sound[son_objlance], 0);
									} else {
										if(ddbas) { //En bas
											perso[0].acti = 84;
											perso[0].stat = 0;
											perso[0].stata = 0;

											PlaySoundFX(&sound[son_objlance], 0);
										}
									}
								}
							} else utilobjet(0, dddroite || ddgauche); //On l'utilise
						} else { //Pas d'objet
							if( ( (perso[0].y >> 14) != ( (perso[0].y + 1024) >> 14) ) && (typedecor[tabl[perso[0].x >> 14][(perso[0].y >> 14) + 1]] < 1) ) { //Au sol - coups normaux
								if(perso[0].objet == 0) {
									if(br) { //Tentative prise
										if(perso[0].acti != 3) {
											perso[0].acti = 50;
											perso[0].stat = 0;
											perso[0].stata = 0;
											return;
										}
									} else {
										if(testeobjet(0) ) { //On ramasse
											perso[0].acti = 80;
											perso[0].stat = 0;
											perso[0].stata = 0;
											ztmp = false;

											PlaySoundFX(&sound[son_objpris], 0);
											return;
										}
									}
								}

								switch(perso[0].acti) {
								case 0: //Rien
								case 1: //Marche
								case 6: //Déséquilibre
									if(dddroite) { //Smash à droite
										if(perso[0].cote > 0) { //Bon côté
											perso[0].acti = 41;
											perso[0].stat = 0;
											perso[0].stata = 0;
										} else { //Retourné
											perso[0].acti = 44;
											perso[0].stat = 0;
											perso[0].stata = 0;
										}
									} else {
										if(ddgauche) { //Smash à gauche
											if(perso[0].cote < 0) { //Bon côté
												perso[0].acti = 41;
												perso[0].stat = 0;
												perso[0].stata = 0;
											} else { //Retourné
												perso[0].acti = 44;
												perso[0].stat = 0;
												perso[0].stata = 0;
											}
										} else {
											if(ddhaut) { //Coup smash haut
												perso[0].acti = 42;
												perso[0].stat = 0;
												perso[0].stata = 0;
											} else { //Coup simple
												perso[0].acti = 40;
												perso[0].stat = 0;
												perso[0].stata = 0;
											}
										}
									}
									break;
								case 2: //Course
									if(dddroite) { //Smash à droite
										if(perso[0].cote > 0) { //Bon côté
											perso[0].acti = 31;
											perso[0].stat = 0;
											perso[0].stata = 0;
										}
									} else {
										if(ddgauche) { //Smash à gauche
											if(perso[0].cote < 0) { //Bon côté
												perso[0].acti = 31;
												perso[0].stat = 0;
												perso[0].stata = 0;
											}
										} else { //Coup simple
											perso[0].acti = 30;
											perso[0].stat = 0;
											perso[0].stata = 0;
										}
									}
									break;
								case 3: //Baissé
									perso[0].acti = 36;
									perso[0].stat = 0;
									perso[0].stata = 0;
								}
							} else { //En l'air
								if(dddroite) { //Smash à droite
									if(perso[0].cote > 0) { //Bon côté
										perso[0].acti = 33;
										perso[0].stat = 0;
										perso[0].stata = 0;
									} else { //Retourné
										perso[0].acti = 38;
										perso[0].stat = 0;
										perso[0].stata = 0;
									}
								} else {
									if(ddgauche) { //Smash à gauche
										if(perso[0].cote < 0) { //Bon côté
											perso[0].acti = 33;
											perso[0].stat = 0;
											perso[0].stata = 0;
										} else { //Retourné
											perso[0].acti = 38;
											perso[0].stat = 0;
											perso[0].stata = 0;
										}
									} else {
										if(ddhaut) { //Coup smash haut
											perso[0].acti = 34;
											perso[0].stat = 0;
											perso[0].stata = 0;
										} else {
											if(ddbas) { //Coup smash bas
												perso[0].acti = 35;
												perso[0].stat = 0;
												perso[0].stata = 0;
											} else { //Coup simple
												perso[0].acti = 32;
												perso[0].stat = 0;
												perso[0].stata = 0;
											}
										}
									}
								}
							}
						}
					}
					ba = true;
				}
			} else {
				ba = false;

				if(exKey & PSP_CTRL_CIRCLE) { //Bouton O
					if(!bb && !perso[0].protect) { //Un coup
						if( (perso[0].acti >= 0) && (perso[0].acti < 8) ) {
							if(dddroite) { //Smash à droite
								perso[0].acti = 61;
								perso[0].stat = 0;
								perso[0].stata = 0;
								perso[0].cote = 1;
							} else {
								if(ddgauche) { //Smash à gauche
									perso[0].acti = 61;
									perso[0].stat = 0;
									perso[0].stata = 0;
									perso[0].cote = -1;
								} else {
									if(ddhaut) { //Coup smash haut
										if(!perso[0].sc) {
											perso[0].acti = 62;
											perso[0].stat = 0;
											perso[0].stata = 0;
											perso[0].sc = true;
										}
									} else {
										if(ddbas) { //Coup smash bas
											perso[0].acti = 63;
											perso[0].stat = 0;
											perso[0].stata = 0;
										} else { //Coup simple
											perso[0].acti = 60;
											perso[0].stat = 0;
											perso[0].stata = 0;
										}
									}
								}
							}
						}
						bb = true;
					}
				} else bb = false;
			}

			if( (perso[0].acti > 29) && (perso[0].stat == 0) && (perso[0].stata == 0) ) {
				if(move[defperso[perso[0].id].idcoups + perso[0].acti].type == move_smash) {
					PlaySoundFX(&sound[son_smvent], 0);
				} else PlaySoundFX(&sound[son_cvent], 0);
				if(move[defperso[perso[0].id].idcoups + perso[0].acti].idson) PlaySoundFX(&defperso[perso[0].id].son[move[defperso[perso[0].id].idcoups + perso[0].acti].idson], 0);
			}
		} else bsel = 0;

/*
		if(!joueur[1].cpu) { // ********************************************* CHATBOARD *************************************************************
			char cbkey = gp_getChatboard();

			if(cbkey == 75) { //Bouton Saut 'k'
				sautperso(1);
			}

			if(cbkey == 76) { //Bouton Jet 'l'
				br2 = 20;
			} else {
				if(br2 > 0) br2--;
			}

			if( (cbkey == 32) || (cbkey == 81) ) { //Bouton Protection ' ' ou 'q'
				if(!bl2) {
					if(!perso[1].protect) {
						if( ( (perso[1].acti == 0) || (perso[1].acti == 1) || (perso[1].acti == 3) || (perso[1].acti == 6) ) && (perso[1].shield > 0) ) {
							perso[1].protect = true;

							PlaySoundFX(&sound[son_protect], 1);
						}
					}
					bl2 = true;
				} else {
					bl2 = false;
					perso[1].protect = false;
				}
			}

			if(cbkey == 87) { //Haut 'w'
				ddhaut2 = 15;
				dhaut2 = true;
			} else {
				if(ddhaut2 > 0) ddhaut2--;
				if(ddhaut2 == 1) { //Simple clic dir
					if(!dba2 && !dbb2) sautperso(1); //Pas d'action, on saute
				}
				dhaut2 = false;
				tmsaut2 = 0;

				if(cbkey == 90) { //Bas 'z'
					if(ddbas2) { //Double clic
						if( (perso[1].y >> 14) != ( (perso[1].y + 1024) >> 14) ) { //Pile sur une case
							if( (typedecor[tabl[perso[1].x >> 14][(perso[1].y >> 14) + 1]] < 1) && (typedecor[tabl[perso[1].x >> 14][(perso[1].y >> 14) + 2]] > 0) ) { //Au sol - on peut descendre
								perso[1].acti = 7;
								perso[1].stat = 0;
								perso[1].stata = 0;
								perso[1].y += 17409;
							}
						}
					} else {
						if(!dbas2) {
							if(perso[1].acti == 8) { //On se lache
								perso[1].acti = 0;
								perso[1].stat = 0;
								perso[1].stata = 0;
							} else { //Lock
								if( ( (perso[1].y >> 14) != ( (perso[1].y + 1024) >> 14) ) && (typedecor[tabl[perso[1].x >> 14][(perso[1].y >> 14) + 1]] < 1) ) { //Au sol - on peut se baisser
									if( ( (perso[1].acti > -1) && (perso[1].acti < 3) && (perso[1].dy == 0) ) || (perso[1].acti == 6) ) { //On se baisse
										perso[1].acti = 3;
										perso[1].stat = 0;
										perso[1].stata = 0;
									}
								}
								dbas2 = true;
							}
							ddbas2 = 20;
						} else { //Release
							if(ddbas2 < 1) {
								dbas2 = false;
								if(perso[1].acti == 3) { //On se relève
									perso[1].acti = 1;
									perso[1].stat = 0;
									perso[1].stata = 0;
								}
							}
						}
					}
				} else {
					if(ddbas2 > 0) ddbas2--;
				}
			}

			if(dgauche2) deplaceperso(1, -1, ddgauche2);
			if(ddroite2) deplaceperso(1, 1, dddroite2);

			if(cbkey == 65) { //Gauche 'a'
				if(ddgauche2) { //Double clic - lock au sol
					if( (perso[1].acti == 1) && (perso[1].cote < 0) ) { //On commence à courir
						perso[1].acti = 2;
						perso[1].stat = 0;
						perso[1].stata = 0;
						perso[1].dx += (move[defperso[perso[1].id].idcoups + 2].ax * -2);
					}
					ddgauche2 = 0;
					if(perso[1].dy == 0) dgauche2 = true;
				} else {
					if(!dgauche2) { //Lock en l'air
						if( (perso[1].acti == 0) && !perso[1].protect) { //On commence à marcher
							perso[1].acti = 1;
							perso[1].stat = 0;
							perso[1].stata = 0;
						}
						ddgauche2 = 10;
						if(perso[1].dy) dgauche2 = true;
					} else dgauche2 = false; //Release
				}
				ddroite2 = false;
				dddroite2 = 0;
			} else {
				if(ddgauche2 > 0) ddgauche2--;
				if(ddgauche2 == 1) { //Simple clic dir
					if(!dba2 && !dbb2) deplaceperso(1, -1, 0); //Pas d'action, on se retourne
				}

				if(cbkey == 83) { //Droite 's'
					if(dddroite2) { //Double clic - lock au sol
						if( (perso[1].acti == 1) && (perso[1].cote > 0) ) { //On commence à courir
							perso[1].acti = 2;
							perso[1].stat = 0;
							perso[1].stata = 0;
							perso[1].dx += (move[defperso[perso[1].id].idcoups + 2].ax * 2);
						}
						dddroite2 = 0;
						if(perso[1].dy == 0) ddroite2 = true;
					} else {
						if(!ddroite2) { //Lock
							if( (perso[1].acti == 0) && !perso[1].protect) { //On commence à marcher
								perso[1].acti = 1;
								perso[1].stat = 0;
								perso[1].stata = 0;
							}
							dddroite2 = 10;
							if(perso[1].dy) ddroite2 = true;
						} else ddroite2 = false; //Release
					}
					dgauche2 = false;
					ddgauche2 = 0;
				} else {
					if(dddroite2 > 0) dddroite2--;
					if(dddroite2 == 1) { //Simple clic dir
						if(!dba2 && !dbb2) deplaceperso(1, 1, 0); //Pas d'action, on se retourne
					}
				}
			}

			if(cbkey == 46) { //Bouton A '.'
				if(!ba2 && !perso[1].protect) { //Un coup
					if( (perso[1].acti >= 0) && (perso[1].acti < 8) ) {
						if(perso[1].objet) { //On a un objet
							if(br2) { //On le lance
								if(dddroite2 || ddgauche2) { //Sur le côté
									perso[1].acti = 82;
									perso[1].stat = 0;
									perso[1].stata = 0;

									if(dddroite2) {
										perso[1].cote = 1;
									} else perso[1].cote = -1;

									PlaySoundFX(&sound[son_objlance], 0);
								} else {
									if(ddhaut2) { //En haut
										perso[1].acti = 83;
										perso[1].stat = 0;
										perso[1].stata = 0;

										PlaySoundFX(&sound[son_objlance], 0);
									} else {
										if(ddbas2) { //En bas
											perso[1].acti = 84;
											perso[1].stat = 0;
											perso[1].stata = 0;

											PlaySoundFX(&sound[son_objlance], 0);
										} else { //Jet auto
											perso[1].acti = 82;
											perso[1].stat = 0;
											perso[1].stata = 0;
		
											PlaySoundFX(&sound[son_objlance], 0);
										}
									}
								}
							} else utilobjet(1, dddroite2 || ddgauche2); //On l'utilise
						} else { //Pas d'objet
							if( ( (perso[1].y >> 14) != ( (perso[1].y + 1024) >> 14) ) && (typedecor[tabl[perso[1].x >> 14][(perso[1].y >> 14) + 1]] < 1) ) { //Au sol - coups normaux
								if(perso[1].objet == 0) {
									if(br2) { //Tentative prise
										if(perso[1].acti != 3) {
											perso[1].acti = 50;
											perso[1].stat = 0;
											perso[1].stata = 0;
											return;
										}
									} else {
										if(testeobjet(1) ) { //On ramasse
											perso[1].acti = 80;
											perso[1].stat = 0;
											perso[1].stata = 0;
											ztmp = false;

											PlaySoundFX(&sound[son_objpris], 0);
											return;
										}
									}
								}

								switch(perso[1].acti) {
								case 0: //Rien
								case 1: //Marche
								case 6: //Déséquilibre
									if(dddroite2) { //Smash à droite
										if(perso[1].cote > 0) { //Bon côté
											perso[1].acti = 41;
											perso[1].stat = 0;
											perso[1].stata = 0;
										} else { //Retourné
											perso[1].acti = 44;
											perso[1].stat = 0;
											perso[1].stata = 0;
										}
									} else {
										if(ddgauche2) { //Smash à gauche
											if(perso[1].cote < 0) { //Bon côté
												perso[1].acti = 41;
												perso[1].stat = 0;
												perso[1].stata = 0;
											} else { //Retourné
												perso[1].acti = 44;
												perso[1].stat = 0;
												perso[1].stata = 0;
											}
										} else {
											if(ddhaut2) { //Coup smash haut
												perso[1].acti = 42;
												perso[1].stat = 0;
												perso[1].stata = 0;
											} else { //Coup simple
												perso[1].acti = 40;
												perso[1].stat = 0;
												perso[1].stata = 0;
											}
										}
									}
									break;
								case 2: //Course
									if(dddroite2) { //Smash à droite
										if(perso[1].cote > 0) { //Bon côté
											perso[1].acti = 31;
											perso[1].stat = 0;
											perso[1].stata = 0;
										}
									} else {
										if(ddgauche2) { //Smash à gauche
											if(perso[1].cote < 0) { //Bon côté
												perso[1].acti = 31;
												perso[1].stat = 0;
												perso[1].stata = 0;
											}
										} else { //Coup simple
											perso[1].acti = 30;
											perso[1].stat = 0;
											perso[1].stata = 0;
										}
									}
									break;
								case 3: //Baissé
									perso[1].acti = 36;
									perso[1].stat = 0;
									perso[1].stata = 0;
								}
							} else { //En l'air
								if(dddroite2) { //Smash à droite
									if(perso[1].cote > 0) { //Bon côté
										perso[1].acti = 33;
										perso[1].stat = 0;
										perso[1].stata = 0;
									} else { //Retourné
										perso[1].acti = 38;
										perso[1].stat = 0;
										perso[1].stata = 0;
									}
								} else {
									if(ddgauche2) { //Smash à gauche
										if(perso[1].cote < 0) { //Bon côté
											perso[1].acti = 33;
											perso[1].stat = 0;
											perso[1].stata = 0;
										} else { //Retourné
											perso[1].acti = 38;
											perso[1].stat = 0;
											perso[1].stata = 0;
										}
									} else {
										if(ddhaut2) { //Coup smash haut
											perso[1].acti = 34;
											perso[1].stat = 0;
											perso[1].stata = 0;
										} else {
											if(ddbas2) { //Coup smash bas
												perso[1].acti = 35;
												perso[1].stat = 0;
												perso[1].stata = 0;
											} else { //Coup simple
												perso[1].acti = 32;
												perso[1].stat = 0;
												perso[1].stata = 0;
											}
										}
									}
								}
							}
						}
					}
					ba2 = true;
					dba2 = 10;
				}
			} else {
				ba2 = false;
				if(dba2 > 0) dba2--;

				if(cbkey == 77) { //Bouton B 'm'
					if(!bb2 && !perso[1].protect) { //Un coup
						if( (perso[1].acti >= 0) && (perso[1].acti < 8) ) {
							if(dddroite2) { //Smash à droite
								perso[1].acti = 61;
								perso[1].stat = 0;
								perso[1].stata = 0;
								perso[1].cote = 1;
							} else {
								if(ddgauche2) { //Smash à gauche
									perso[1].acti = 61;
									perso[1].stat = 0;
									perso[1].stata = 0;
									perso[1].cote = -1;
								} else {
									if(ddhaut2) { //Coup smash haut
										if(!perso[1].sc) {
											perso[1].acti = 62;
											perso[1].stat = 0;
											perso[1].stata = 0;
											perso[1].sc = true;
										}
									} else {
										if(ddbas2) { //Coup smash bas
											perso[1].acti = 63;
											perso[1].stat = 0;
											perso[1].stata = 0;
										} else { //Coup simple
											perso[1].acti = 60;
											perso[1].stat = 0;
											perso[1].stata = 0;
										}
									}
								}
							}
						}
						bb2 = true;
						dbb2 = 10;
					}
				} else {
					bb2 = false;
					if(dbb2 > 0) dbb2--;
				}
			}

			if( (perso[1].acti > 29) && (perso[1].stat == 0) && (perso[1].stata == 0) ) {
				if(move[defperso[perso[1].id].idcoups + perso[1].acti].type == move_smash) {
					PlaySoundFX(&sound[son_smvent], 0);
				} else PlaySoundFX(&sound[son_cvent], 0);
				if(move[defperso[perso[1].id].idcoups + perso[1].acti].idson) PlaySoundFX(&defperso[perso[1].id].son[move[defperso[perso[1].id].idcoups + perso[1].acti].idson], 0);
			}
		}
*/
	}
}
