typedef struct {
	int phase, stat;
	int lock;
	int cible;
	int ok;
	int air;
	int tmsaut;
} s_ia;

s_ia ia[max_persos];

int testepf(int i, int zone) {
	int x = perso[i].x >> 14;
	int a, b;

	if( (x >= area[num_area].zone[zone].xmin) && (x <= area[num_area].zone[zone].xmax) && (area[num_area].zone[zone].y > (perso[i].y >> 14) ) ) return true;

	a = x - area[num_area].zone[zone].xmin;
	a *= a;
	b = x - area[num_area].zone[zone].xmax;
	b *= b;
	if(b < a) a = b;
	a *= 3;
	b = area[num_area].zone[zone].y - (perso[i].y >> 14);
	if( (a < b * b) && (b > 0) ) {
		return true;
	} else return false;
}

int cherchepf(int i, int xc, int yc, int pdest) {
	int j;
	int tabcible[16];
	int nbcibles = 0;
	int x = perso[i].x >> 14;
	int y = perso[i].y >> 14;
	int a, b, c;
	int tst = pdest && !perso[i].sc;

	for(j = 0; j < area[num_area].nb_zones; j++) { //Recherche de la zone
		if( (x >= area[num_area].zone[j].xmin) && (x <= area[num_area].zone[j].xmax) && (y < area[num_area].zone[j].y) ) {
			a = 0;
		} else {
			a = x - area[num_area].zone[j].xmin;
			a *= a;
			b = x - area[num_area].zone[j].xmax;
			b *= b;
			if(b < a) a = b;
		}
		if(y > area[num_area].zone[j].y) { //Au-dessous
			b = y - area[num_area].zone[j].y;
			if( (x >= area[num_area].zone[j].xmin) && (x <= area[num_area].zone[j].xmax) ) {
				a += 200;
			} else a += (b * b);
		}
		if(tst) { //Priorité cible
			b = xc - ( (area[num_area].zone[j].xmin + area[num_area].zone[j].xmax) >> 1);
			c = yc - area[num_area].zone[j].y;
			b *= b;
			c *= c;
			a += (b * b + c * c);
		} else {
			if(!testepf(i, j) ) {
				if(y < area[num_area].zone[j].y) {
					b = area[num_area].zone[j].y - y;
					b *= b;
					if(a > b) a *= 5;
				}
			}
		}
		if(perso[i].dx > 50) {
			if(area[num_area].zone[j].xmax > x) a >>= 2;
		} else {
			if(perso[i].dx < -50) {
				if(area[num_area].zone[j].xmin < x) a >>= 2;
			}
		}

		tabcible[nbcibles++] = a;
	}

	a = 0;
	for(j = 1; j < nbcibles; j++) {
		if(tabcible[j] < tabcible[a] ) a = j;
	}
	return a;
}

void gere_ia(const int i) {
	int j;
	int k, l;
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;

	if(ia[i].tmsaut > 0) {
		if(perso[i].dy > 0) {
			ia[i].tmsaut -= 2;
		} else ia[i].tmsaut--;
	}

	if(ia[i].lock < 1) {
		if(perso[i].protect) perso[i].protect = false;
		if(perso[i].acti == 3) {
			perso[i].acti = 0;
			perso[i].stat = 0;
			perso[i].stata = 0;
		}

		if(perso[i].acti >= 0) {
			if(ia[i].stat < 1) { //Durée phase
				ia[i].phase = 0;
				ia[i].cible = -1;
			}

//			if( (perso[i].acti == 21) || (perso[i].acti == 23) ) { //Par terre
//				sautperso(i);
//				return;
//			}

			switch(ia[i].phase) {
			case 1: //Phase attaque
				if(ia[i].cible >= 0) {
					if(perso[ia[i].cible].enjeu) {
						x = perso[ia[i].cible].x >> 14;
						y = perso[ia[i].cible].y >> 14;
						k = x - (perso[i].x >> 14);
//						if(k * k < 5) x += GpRandN(8) - 4;
						if(k * k < 5) x += mp_Rand(8) - 4;
						ia[i].stat--;

						if(perso[i].acti < 8) { //Fait rien
							l = y - (perso[i].y >> 14);
							j = k * k + l * l;
							if(perso[i].objet) {
//								if( (GpRandN(50) == 4) && (j > 10) ) { //On essaie de le lancer
								if( (mp_Rand(50) == 4) && (j > 10) ) { //On essaie de le lancer
									if( (k * k > l * l) && (l * l < 4) ) { //Lancer horizontal
										perso[i].acti = 82;
										perso[i].stat = 0;
										perso[i].stata = 0;

										PlaySoundFX(&sound[son_objlance], 0);
									} else {
										if(k * k < 4) { //Lancer vertical
											if(l > 0) {
												perso[i].acti = 83;
											} else perso[i].acti = 84;
											perso[i].stat = 0;
											perso[i].stata = 0;

											PlaySoundFX(&sound[son_objlance], 0);
										}
									}
								} else { //On l'utilise
//									if( (j < 7) || (GpRandN(8) == 3) ) utilobjet(i, GpRandN(10) == 8);
									if( (j < 7) || (mp_Rand(8) == 3) ) utilobjet(i, mp_Rand(10) == 8);
								}
							} else {
//								if( (j < 15) && (GpRandN(10) == 3) ) { //Coup B
								if( (j < 15) && (mp_Rand(10) == 3) ) { //Coup B
									//if(ia[i].air) {
									//	j = GpRandN(3);
//									j = GpRandN(4);
									j = mp_Rand(4);
									switch(j) {
									case 0:
										perso[i].acti = 60;
										perso[i].stat = 0;
										perso[i].stata = 0;
										break;
									case 1:
										//perso[i].x += (perso[i].cote * move[defperso[perso[i].id].idcoups + 61].ax * 5);
										//if(testepf(i, cherchepf(i, 0, 0, false) ) && perso[i].nsauts) {
										if(perso[i].nsauts) {
											perso[i].acti = 61;
											perso[i].stat = 0;
											perso[i].stata = 0;
										}
										//perso[i].x -= (perso[i].cote * move[defperso[perso[i].id].idcoups + 61].ax * 5);
										break;
									case 2:
										if(!perso[i].sc) {
											perso[i].acti = 62;
											perso[i].stat = 0;
											perso[i].stata = 0;
											perso[i].sc = true;
										}
										break;
									default:
										perso[i].acti = 63;
										perso[i].stat = 0;
										perso[i].stata = 0;
									}
								} else { //Coup A
									if(j < 4) {
//										if( (perso[ia[i].cible].acti < 30) && (GpRandN(6) < (difficulte * 2) + 1) ) {
										if( (perso[ia[i].cible].acti < 30) && (mp_Rand(6) < (difficulte * 2) + 1) ) {
											switch(perso[i].acti) {
											case 0:
											case 1:
											case 6:
//												j = GpRandN(8);
												j = mp_Rand(8);
												switch(j) {
												case 0:
													perso[i].acti = 41;
													break;
												case 1:
													perso[i].acti = 42;
													break;
												case 2:
													perso[i].acti = 44;
													break;
												case 3:
													if(perso[i].objet == 0) perso[i].acti = 50;
													break;
												default:
													perso[i].acti = 40;
												}
												break;
											case 2:
//												if(GpRandN(4) == 3) {
												if(mp_Rand(4) == 3) {
													perso[i].acti = 31;
												} else perso[i].acti = 30;
												break;
											case 3:
												perso[i].acti = 36;
												break;
											case 4:
											case 5:
//												j = GpRandN(8);
												j = mp_Rand(8);
												switch(j) {
												case 0:
													perso[i].acti = 33;
													break;
												case 1:
													perso[i].acti = 34;
													break;
												case 2:
													perso[i].acti = 35;
													break;
												default:
													perso[i].acti = 32;
												}
											}
											perso[i].stat = 0;
											perso[i].stata = 0;
										} else {
											if(!ia[i].air) {
//												if(GpRandN(3) == 1) {
												if(mp_Rand(3) == 1) {
													perso[i].acti = 3;
													perso[i].stat = 0;
													perso[i].stata = 0;
												} else {
													if(perso[i].shield > 0) {
														perso[i].protect = true;

														PlaySoundFX(&sound[son_protect], 0);
													}
												}
//												ia[i].lock = 30 - GpRandN(10) * difficulte;
												ia[i].lock = 30 - mp_Rand(10) * difficulte;
											}
										}
									}
								}
							}

							if( (perso[i].acti > 29) && (perso[i].stat == 0) ) { //Son
								if(move[defperso[perso[i].id].idcoups + perso[i].acti].type == move_smash) {
									PlaySoundFX(&sound[son_smvent], 0);
								} else PlaySoundFX(&sound[son_cvent], 0);
								if(move[defperso[perso[i].id].idcoups + perso[i].acti].idson) PlaySoundFX(&defperso[perso[i].id].son[move[defperso[perso[i].id].idcoups + perso[i].acti].idson], 0);
							}
						}
					} else ia[i].phase = 0;
				} else ia[i].phase = 0;
				break;
			case 2: //Phase objet
				if( (ia[i].cible > 0) && (perso[i].objet == 0) ) {
					if( (objet[ia[i].cible].dx == 0) && (objet[ia[i].cible].ener > 0) ) {
						x = objet[ia[i].cible].x >> 14;
						y = objet[ia[i].cible].y >> 14;
						k = x - (perso[i].x >> 14);
//						l = y - (perso[i].y >> 14);
//						if(k * k < 2) x += GpRandN(3) - 1;
						if(k * k < 2) x += mp_Rand(3) - 1;
						ia[i].stat--;

						if(perso[i].acti < 8) { //Fait rien
							if(testeobjet(i) ) { // == ia[i].cible) { //On le prend
								perso[i].acti = 80;
								perso[i].stat = 0;
								perso[i].stata = 0;
								ia[i].phase = 0;

								PlaySoundFX(&sound[son_objpris], 0);
								return;
							}
						}
					} else ia[i].phase = 0;
				} else ia[i].phase = 0;
				break;
			case 3: //Phase retraite
				k = (perso[i].x >> 14) - area[num_area].coordj[i][0];
				if(k * k > 4) {
					x = area[num_area].coordj[i][0];
					y = area[num_area].coordj[i][1];
					ia[i].stat--;
				} else ia[i].phase = 0;
				break;
			default: //Pas de phase
				if(perso[i].acti < 8) {
					if(perso[i].objet) { //Attaque
						ia[i].phase = 0;
//					} else ia[i].phase = GpRandN(5 + difficulte);
					} else ia[i].phase = mp_Rand(5 + difficulte);

					switch(ia[i].phase) {
					case 0: //Attaque - choix cible
					case 1:
					case 2:
					case 4:
					case 5:
					case 6:
						ia[i].phase = 1;
						if(type_jeu != type_survival) {
//							if(GpRandN(3 - difficulte) < 1) { //Choix tactique
							if(mp_Rand(3 - difficulte) < 1) {
								k = -1;
								for(j = 0; j < nb_joueurs; j++) {
									if( (perso[j].enjeu) && (i != j) ) {
										if(k >= 0) {
											if(perso[j].damage > perso[k].damage) k = j;
										} else k = j;
									}
								}
							} else { //Choix aléatoire
								k = i;
//								while(k == i) k = GpRandN(nb_joueurs);
								while(k == i) k = mp_Rand(nb_joueurs);
							}
							ia[i].cible = k;
						} else ia[i].cible = 0;

						if(perso[ia[i].cible].enjeu) {
//							ia[i].stat = 100 + GpRandN(5) * 50;
							ia[i].stat = 100 + mp_Rand(5) * 50;
						} else ia[i].stat = 5;
						return;
					default: //Objet - choix objet cible
						ia[i].phase = 2;
						if( (nb_objets > 0) && (perso[i].objet == 0) ) {
//							if(GpRandN(3 - difficulte) < 1) { //Choix tactique
							if(mp_Rand(3 - difficulte) < 1) { //Choix tactique
								k = 1;
								for(j = 2; j <= nb_objets; j++) {
									if(defobjet[objet[j].id].atta > defobjet[objet[k].id].atta) k = j;
								}
								ia[i].cible = k;
//							} else k = GpRandN(nb_objets) + 1; //Choix aléatoire
							} else k = mp_Rand(nb_objets) + 1;
							if( (objet[k].dx == 0) && (objet[k].ener > 0) && !objet[k].porte) {
								ia[i].cible = k;
							} else ia[i].phase = 0;
						} else ia[i].phase = 0;
//						ia[i].stat = 200 + GpRandN(10) * 50;
						ia[i].stat = 200 + mp_Rand(10) * 50;
					}
					return;
				}
			}
		} else {
			x = area[num_area].coordj[i][0];
			y = area[num_area].coordj[i][1];
		}

		if( (perso[i].x >> 14) < x) {
			dx = 1;
		} else {
			if( (perso[i].x >> 14) > x) dx = -1;
		}
		if( (perso[i].y >> 14) < y) {
			dy = 1;
		} else {
			if( (perso[i].y >> 14) > y) dy = -1;
		}
		if(ia[i].air) { //En l'air, on controle
			if(perso[i].acti < 0) {
				if( (perso[i].dx > 4000) || (perso[i].dx < -4000) ) { //On est parti, fo revenir
					ia[i].phase = 3;
					ia[i].stat = 50;
				}
				ia[i].ok = false;
			}

			if(perso[i].acti != 8) {
				if( (perso[i].dy < 1000) && (perso[i].nsauts > 0) && ia[i].ok) { //Phase montante
					if(dx) deplaceperso(i, dx, true);

					if(dy < 0) { //Objectif plus haut
						if(ia[i].tmsaut < 1) {
							sautperso(i);
							ia[i].tmsaut = 30;
						}
					}
				} else { //Phase descendante, recherche d'une plate-forme d'atterrissage
					if(ia[i].ok) {
						k = cherchepf(i, x, y, true);
					} else k = cherchepf(i, 0, 0, false);
					l = (area[num_area].zone[k].xmin + area[num_area].zone[k].xmax) >> 1;
					dx = 0;
					if( ( (perso[i].x >> 14) <= l) || (perso[i].dx < -4000) ) {
						dx = 1;
					} else {
						if( ( (perso[i].x >> 14) >= l) || (perso[i].dx > 4000) ) dx = -1;
					}
					if(dx) deplaceperso(i, dx, true);

					if( ( (perso[i].y >> 14) > area[num_area].zone[k].y) || (perso[i].dy > 1000) ) { //Il faut re-sauter
						if(!testepf(i, k) ) { //Pas bon
							if(ia[i].tmsaut < 1) {
								if(perso[i].nsauts < 1) {
									if( (perso[i].acti >= 0) && (perso[i].acti < 30) && !perso[i].sc) {
										perso[i].acti = 62;
										perso[i].stat = 0;
										perso[i].stata = 0;
										perso[i].sc = true;

										if(move[defperso[perso[i].id].idcoups + perso[i].acti].idson) PlaySoundFX(&defperso[perso[i].id].son[move[defperso[perso[i].id].idcoups + perso[i].acti].idson], 0);
										return;
									}
								} else {
									sautperso(i);
									ia[i].tmsaut = 40;
								}
							}
						}
					}
				}
			} else { //Accroché
//				if(GpRandN(20) < difficulte) {
				if(mp_Rand(30) <= difficulte) {
					sautperso(i);
					ia[i].tmsaut = 40;
				}
			}
		} else {
			if(perso[i].acti >= 0) {
				ia[i].ok = true;

				if( (perso[i].acti == 21) || (perso[i].acti == 23) ) { //Par terre
					sautperso(i);
					return;
				}

				if(dx) {
					if(perso[i].acti < 8) { //Fait rien
						l = (perso[i].x >> 14) - x;
						if(l * l > 3) {
							if(perso[i].acti == 1) {
								perso[i].acti = 2;
								perso[i].stat = 0;
								perso[i].stata = 0;
							}
						} else {
							if(perso[i].acti == 2) {
								perso[i].acti = 1;
								perso[i].stat = 0;
								perso[i].stata = 0;
							}
						}
						if( (perso[i].objet == 0) && (testeobjet(i) ) ) {
							perso[i].acti = 80;
							perso[i].stat = 0;
							perso[i].stata = 0;

							PlaySoundFX(&sound[son_objpris], 0);
							return;
						}
					}
					deplaceperso(i, dx, true);

					k = cherchepf(i, 0, 0, false);
					l = perso[i].x >> 14;
					if( ( (l <= area[num_area].zone[k].xmin + 1) && (l > x) ) || ( (l >= area[num_area].zone[k].xmax - 1) && (l < x) ) ) {
						sautperso(i);
						ia[i].tmsaut = 40;
					} else {
						k = (l - x);
						k *= k;
						if(k < 15) { //Assez proche
							if(dy < 0) {
								sautperso(i);
								ia[i].tmsaut = 40;
							} else {
								if( (dy > 0) && (perso[i].acti < 8) ) { //Fait rien - descente étage
									if(typedecor[tabl[perso[i].x >> 14][(perso[i].y >> 14) + 2]] > 0) { //Au sol - on peut descendre
										perso[i].acti = 7;
										perso[i].stat = 0;
										perso[i].stata = 0;
										perso[i].y += 17409;
										return;
									}
								}
							}
						}
					}
				} else {
					if( (perso[i].x >> 14) < x) {
						l = 1;
					} else l = -1;

					if(perso[i].cote == l) {
						if( (perso[i].acti < 8) && (perso[i].objet == 0) ) { //Fait rien
							if(testeobjet(i) ) {
								perso[i].acti = 80;
								perso[i].stat = 0;
								perso[i].stata = 0;

								PlaySoundFX(&sound[son_objpris], 0);
								return;
							}
						}
					} else deplaceperso(i, l, false);
				}
			} //else sautperso(i);
		}
	} else {
		ia[i].lock--;
		if( (perso[i].protect) && (perso[i].shield < 1) ) {
			perso[i].protect = 0;
			ia[i].lock = 0;
		}
	}
}
