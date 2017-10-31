int animperso(int i) {
	int j = defperso[perso[i].id].idcoups + perso[i].acti;

	perso[i].imag = move[j].idim + perso[i].stat;

	perso[i].stata += move[j].vite;
	if(perso[i].stata > 9) {
		perso[i].stata -= 10;
		perso[i].stat++; //Animation
		if(perso[i].stat > move[j].nani) { //Fin animation
			if(move[j].loop) {
				perso[i].stat = 0;
			} else perso[i].stat = move[j].nani;
			return true;
		}
	}

	return false;
}

void gere_perso(int i) {
//	register j;
	int j;
	int a, b;
	int c = true;
	int k, l;
	int air = false;
	int sgnx, sgny;
	int x;

	if(perso[i].dx > 0) {
		sgnx = 1;
	} else sgnx = -1;
	if(perso[i].dy > 0) {
		sgny = 1;
	} else sgny = -1;

	a = sgnx * perso[i].dx; //Valeurs absolues
	b = sgny * perso[i].dy;
	while(a || b || c) {
		c = false;
		if(a > 8000) {
			perso[i].x += (8000 * sgnx);
			a -= 8000;
		} else {
			perso[i].x += (a * sgnx);
			a = 0;
		}
		if(b > 8000) {
			perso[i].y += (8000 * sgny);
			b -= 8000;
		} else {
			perso[i].y += (b * sgny);
			b = 0;
		}

		if( (perso[i].x > 16384) && (perso[i].x < xmaxm) && (perso[i].y > ( (pers[perso[i].imag].h + 16) << 10) ) && (perso[i].y < ymaxm) ) { //Sur la map
			if(typedecor[tabl[perso[i].x >> 14][perso[i].y >> 14]] < 1) { //Collision avec bloc dessous
				if(perso[i].acti >= 0) { //Ok
					if(perso[i].dy >= 0) {
						b = 0;
						if(move[defperso[i].idcoups + perso[i].acti].type < move_deplace) {
							perso[i].dy = 0;
							perso[i].y = ( (perso[i].y >> 14) << 14) - 1024;
						} else { //S'est jetté (coup mouvement)
							//perso[i].y -= perso[i].dy;
							perso[i].dy = -(perso[i].dy >> 2);
							sgny = -sgny;

							if(perso[i].dy * sgny < 500) { //A terre
								perso[i].dy = 0;
								perso[i].y = ( (perso[i].y >> 14) << 14) - 1024;
							}
						}
					}
				} else { //Assommé
					b = 0;
					if(perso[i].dy >= 0) {
						//perso[i].y -= perso[i].dy;
						perso[i].dy = -(perso[i].dy >> 2);
						sgny = -sgny;
						creeexplos(perso[i].x >> 10, perso[i].y >> 10, explos_ondechoc, nb_explos_ondechoc, 2);
						secoueecran(defperso[perso[i].id].poids >> 5);
					}

					if(perso[i].dy * sgny < 500) { //A terre
						perso[i].dy = 0;
						perso[i].y = ( (perso[i].y >> 14) << 14) - 1024;
						if( (i == 0) && (ddhaut > 4) ) { //Haut au bon moment, on est debout
							perso[i].acti = 22;
							perso[i].stat = 0;
							perso[i].stata = 0;

							PlaySound(&sound[son_ohhh02]);
						} else {
							perso[i].acti = 20;
							perso[i].stat = 0;
							perso[i].stata = 0;

							if(perso[i].damage > 6400) {
								PlaySound(&sound[son_ohhh03]);
							} else PlaySound(&sound[son_ohhh02]);
						}
					}
				}
				air = false;
			} else {
				k = (perso[i].x + sgnx * (pers[perso[i].imag].w << 9) ) >> 14;
				l = perso[i].y >> 14;
				air = 0;
				for(j = 0; j <= (pers[perso[i].imag].h >> 4); j++) {
					if(typedecor[tabl[k][l - j]] < 1) air++; //Collision sur coté
				}
				if(air > 0) { //Collision
					if( (air > 1) && (perso[i].acti >= 0) && perso[i].dx && (perso[i].dy > 100) && (perso[i].cote == sgnx) && (typedecor[tabl[k][l - ( (pers[perso[i].imag].h + 15) >> 4)]] > 0) && (typedecor[tabl[k][l - ( (pers[perso[i].imag].h - 1) >> 4)]] < 1) ) { //On s'accroche
						a = 0;
						perso[i].acti = 8;
						perso[i].stat = 0;
						perso[i].stata = 0;
						perso[i].nsauts = defperso[perso[i].id].nsauts;
						perso[i].x = ( (perso[i].x >> 14) << 14) + 8192;
						perso[i].y = ( (l - ( (pers[perso[i].imag].h + 15) >> 4) ) << 14) + pers[perso[i].imag].h * 1024 + 12288;
						perso[i].dx = 0;
						perso[i].dy = 0;

						PlaySoundFX(&defperso[perso[i].id].son[2], 0);

						PlaySound(&sound[son_ohhh01]);
					} else {
						if(air > 1) { //On tape :)
							a = 0;
							perso[i].x -= perso[i].dx;
							switch(perso[i].acti) {
							case 0:
							case 1:
								perso[i].dx = 0;
								perso[i].acti = 0;
								perso[i].stat = 0;
								perso[i].stata = 0;
								break;
							case 2:
								perso[i].dx = -(perso[i].dx >> 2);
								sgnx = -sgnx;
								perso[i].acti = 14;
								perso[i].stat = 0;
								perso[i].stata = 0;
								break;
							default:
								x = (perso[i].dx * sgnx) >> 4;
								if( (x > 500) && (perso[i].invinc < 1) ) { //Arrive vite - assommé
									creeexplos( (perso[i].x + sgnx * (pers[perso[i].imag].w << 9) ) >> 10, (perso[i].y - (pers[perso[i].imag].h << 9) ) >> 10, explos_impact2, nb_explos_impact, 2);

									perso[i].damage += x;
									assommeperso(i, x >> 6);
								}
								perso[i].dx = -(perso[i].dx >> 1);
								sgnx = -sgnx;
							}
						} else {
							if( (typedecor[tabl[k][l]] < 1) && perso[i].dy) { //Collision un seul bloc, en bas, en l'air
								a = 0;
								perso[i].x -= perso[i].dx;
								perso[i].dx = 0;
							}
						}
					}
				}

				if( ( (perso[i].y >> 14) != ( (perso[i].y + 1024) >> 14) ) && (typedecor[tabl[perso[i].x >> 14][(perso[i].y >> 14) + 1]] < 1) ) { //Pile sur une case
					if(perso[i].nsauts < defperso[perso[i].id].nsauts) { //On etait en l'air
						if( (perso[i].acti >= 0) && (perso[i].acti < 20) ) {
							perso[i].acti = 1;
							perso[i].stat = 0;
							perso[i].stata = 0;
							perso[i].nsauts = defperso[perso[i].id].nsauts;
							perso[i].sc = false;
							perso[i].sontombe = true;
						}
					}
					air = false;
				} else {
					if(perso[i].dy < 0) { //Collision au-dessus
						if( (typedecor[tabl[perso[i].x >> 14][( (perso[i].y >> 10) - pers[perso[i].imag].h) >> 4]] < 1) && (typedecor[tabl[perso[i].x >> 14][( (perso[i].y >> 10) - pers[perso[i].imag].h - 16) >> 4]] < 1) ) { //Boum
							b = 0;
							perso[i].y -= perso[i].dy;
							perso[i].dy = -(perso[i].dy >> 4);
							sgny = -sgny;
						}
					}

					if( (timer5 == 4) && !(a || b) ) { //Fumée
						if(sgnx * perso[i].dx + sgny * perso[i].dy > 12000) creeexplos(perso[i].x >> 10, perso[i].y >> 10, explos_feject, nb_explos_feject, 1);
					}

					air = true;
				}

				if(perso[i].acti < 30) { //Pas de coup
					for(j = 0; j < nb_persos; j++) { //Gestion des collisions
						if( (i != j) && perso[j].enjeu && (perso[j].acti < 0) ) {
							k = (perso[i].x - perso[j].x) >> 10;
							l = (perso[i].y - perso[j].y) >> 10;
							if( (k * k + l * l) < (pers[perso[i].imag].w * pers[perso[j].imag].w + pers[perso[i].imag].h * pers[perso[j].imag].h) ) { //Assez pres
								if(perso[i].x < perso[j].x) {
									k = -k;
									x = -1;
								} else x = 1;
								if(perso[i].y < perso[j].y) l = -l;
								if(k < ( (pers[perso[i].imag].w + pers[perso[j].imag].w) >> 1) ) { //Collision horizontale
									if( (perso[i].dx * sgnx < 200) && (perso[i].acti != 8) ) perso[i].dx += x * 200;
									a = 0;
									l = perso[i].dx;
									if(perso[i].dx * perso[j].dx <= 0) { //Il va ds le sens contraire
										if(perso[i].acti != 8) perso[i].dx -= ( ( (perso[i].dx * defperso[perso[i].id].poids + perso[j].dx * defperso[perso[j].id].poids) / defperso[perso[i].id].poids) >> 1);
										if(perso[j].acti != 8) perso[j].dx += ( ( (l * defperso[perso[i].id].poids + perso[j].dx * defperso[perso[j].id].poids) / defperso[perso[j].id].poids) >> 1);
									} else {
										if(perso[i].acti != 8) perso[i].dx = perso[j].dx;
										if(perso[j].acti != 8) perso[j].dx = l;
									}
									if(perso[i].dx * l < 0) sgnx = -sgnx; //On a changé de sens
									while(k < ( (pers[perso[i].imag].w + pers[perso[j].imag].w) >> 1) ) {
										if(perso[i].acti != 8) perso[i].x += 1024 * x;
										if(perso[j].acti != 8) {
											perso[j].x -= 1024 * x;
										} else {
											if(perso[i].acti == 8) break;
										}
										k = ( (perso[i].x - perso[j].x) >> 10) * x;
									}
								}
							}
						}
					}
				}

				if( (perso[i].y > (ymaxm - (ymaxm >> 4) ) ) && perso[i].sontombe) { //Son chute
					PlaySound(&sound[son_tombe]);
					perso[i].sontombe = false;
				}
			}
		} else { //Hors de la map
			if( (perso[i].x < -area[num_area].oversize) || (perso[i].x > xmaxm + area[num_area].oversize) || (perso[i].y < -area[num_area].oversize) || (perso[i].y > ymaxm + area[num_area].oversize) ) { //Mort perso
				a = 0;
				b = 0;
				mortperso(i);
				air = false;
			} else air = true;
		}
	}

	if(air) {
		if( (perso[i].acti != 8) && (perso[i].pecho < 0) ) { //Pas qd accroché
			if(perso[i].nsauts == defperso[perso[i].id].nsauts) perso[i].nsauts--;
			perso[i].dy += (area[num_area].gravite + defperso[perso[i].id].poids);
			if(perso[i].dy > max_gravite) perso[i].dy = max_gravite;
		}
		ia[i].air = true;
	} else ia[i].air = false;

	if(perso[i].invinc > 0) {
		if(perso[i].dcoup && (perso[i].tape >= 0) ) { //Attente fin coup
			if(perso[i].dcoup != perso[i].tape * 90 + perso[perso[i].tape].acti) perso[i].dcoup = 0;
		} else {
			perso[i].invinc--;
			perso[i].dcoup = 0;
		}
	}

	if(perso[i].acti >= 0) { //Pas assommé
		switch(perso[i].acti) {
		case 0: //Rien
		case 6: //Déséquilibre
			if(!perso[i].protect) {
				if(perso[i].dx) {
					perso[i].acti = 1;
					perso[i].stat = 0;
					perso[i].stata = 0;
				} else {
					if(typedecor[tabl[(perso[i].x + perso[i].cote * (pers[perso[i].imag].w << 9) ) >> 14][(perso[i].y >> 14) + 1]] > 0) { //Rien sur le côté
						perso[i].acti = 6;
					} else perso[i].acti = 0;
				}

				j = ( (pers[move[defperso[perso[i].id].idcoups].idim].w * pers[move[defperso[perso[i].id].idcoups].idim].h) >> 10) * 2048 + 3071;

				perso[i].shield += 35;
				if(perso[i].shield > j) perso[i].shield = j;
			} else { //Vérification shield
				if(perso[i].dx) freineperso(i, sgnx, 5); //Freinage

				perso[i].shield -= 5;
				if(perso[i].shield < 1) perso[i].protect = false;
			}

			if(air) { //Est en l'air
				perso[i].acti = 5;
				perso[i].stat = move[defperso[perso[i].id].idcoups + 5].nani;
				perso[i].stata = 0;
			} else perso[i].tape = -1;

			animperso(i);
			break;
		case 7: //Descente étage
			if(animperso(i) ) {
				perso[i].acti = 5;
				perso[i].stat = move[defperso[perso[i].id].idcoups + 5].nani;
				perso[i].stata = 0;
			}
			break;
		case 1: //Marche
		case 2: //Course
			if(!perso[i].bouge) {
				if(perso[i].dx) { //Freinage
					freineperso(i, sgnx, 4);
				} else {
					perso[i].acti = 0;
					perso[i].stat = 0;
					perso[i].stata = 0;
				}
			}

			if(air) { //Plus au sol
				perso[i].acti = 5;
				perso[i].stat = move[defperso[perso[i].id].idcoups + 5].nani;
				perso[i].stata = 0;
			}

			animperso(i);
			break;
		case 3: //Baissé
			if(perso[i].dx) freineperso(i, sgnx, 5);

			if(!perso[i].protect) {
				j = ( (pers[move[defperso[perso[i].id].idcoups].idim].w * pers[move[defperso[perso[i].id].idcoups].idim].h) >> 10) * 2048 + 3071;

				perso[i].shield += 35;
				if(perso[i].shield > j) perso[i].shield = j;
			} else { //Vérification shield
				perso[i].shield -= 5;
				if(perso[i].shield < 1) perso[i].protect = false;
			}

			animperso(i);
			break;
		case 4: //Saut 1
		case 5: //Saut 2
			if(perso[i].dx) freineperso(i, sgnx, 1);

			if(!air) { //Plus en l'air
				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}

			animperso(i);
			break;
		case 8: //Accroché
			if(perso[i].dx) {
				perso[i].acti = 5;
				perso[i].stat = move[defperso[perso[i].id].idcoups + 5].nani;
				perso[i].stata = 0;
			}

			animperso(i);
			break;
		case 9: //Roulade
			if(animperso(i) ) {
				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
				perso[i].cote = -perso[i].cote;
				perso[i].dx >>= 1;
			} else {
				perso[i].invinc = 1;

				creeexplos(perso[i].x >> 10, perso[i].y >> 10, explos_fumee, nb_explos_fumee, 2);
			}
			break;
		case 10: //Freine
			if(perso[i].dx) freineperso(i, sgnx, 3); //Freinage

			creeexplos(perso[i].x >> 10, perso[i].y >> 10, explos_fumee, nb_explos_fumee, 1);

			if(animperso(i) ) {
				perso[i].acti = 2;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}
			break;
		case 13: //Se retourne
		case 14: //Ds le mur
			if(perso[i].dx) freineperso(i, sgnx, 3); //Freinage

			if(animperso(i) ) {
				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}
			break;
		case 20: //Tombe par-terre
			if(perso[i].dx) freineperso(i, sgnx, 4); //Freinage

//			creeexplos(perso[i].x >> 10 + GpRandN(16) - 8, perso[i].y >> 10 + timer3, explos_fumee, nb_explos_fumee, 1);
			creeexplos( (perso[i].x >> 10) + mp_Rand(16) - 8, (perso[i].y >> 10) + timer3, explos_fumee, nb_explos_fumee, 1);

			if(animperso(i) ) {
				perso[i].acti = 21;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}
			break;
		case 21: //Par-terre
			if(perso[i].dx) freineperso(i, sgnx, 4); //Freinage

			if(air) { //Plus au sol
				perso[i].acti = 23;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}

			animperso(i);
			break;
		case 22: //Se relève
			if(perso[i].dx) freineperso(i, sgnx, 3); //Freinage

			if(animperso(i) ) {
				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}
			break;
		case 23: //Tourne comme qd assommé
			if(perso[i].dx) freineperso(i, sgnx, 1);

			if(!air) { //Plus en l'air
				perso[i].acti = 20;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}

			animperso(i);
			break;
		case 80: //Prise objet
			if(perso[i].dx) freineperso(i, sgnx, 4); //Freinage

			if(animperso(i) ) {
				if( (k = testeobjet(i) ) ) { //On prend l'objet
					if(defobjet[objet[k].id].role < role_etoile) { //On prend l'objet
						perso[i].objet = k;
						objet[k].porte = true;
						objet[k].time += 100;

						if( (defobjet[objet[k].id].role == role_carapr) || (defobjet[objet[k].id].role == role_carapv) || (defobjet[objet[k].id].role == role_mine) ) objet[k].stat = 1;
					} else { //Bonus effet immediat
						switch(defobjet[objet[k].id].role) {
						case role_coeur:
							perso[i].invinc = 7;
							if(defobjet[objet[k].id].atta) {
								perso[i].damage = 0;
							} else perso[i].damage -= ( (perso[i].damage * 5) >> 3);
						}

						objet[k].ener = 0;
					}
				}

				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}
			break;
		case 82: //Lancer horizontal
		case 83: //Lancer en haut
		case 84: //Lancer en bas
			if(perso[i].dx) freineperso(i, sgnx, 3); //Freinage

			if(animperso(i) ) { //Lancer
				k = perso[i].objet;
				if(k) {
					x = defperso[perso[i].id].idcoups + perso[i].acti;
					perso[i].objet = 0;
					objet[k].porte = false;
					objet[k].prop = i;

					objet[k].x = perso[i].x + perso[i].cote * (move[x].dx << 10);
					objet[k].y = perso[i].y - (move[x].dy << 10);
					objet[k].dx = perso[i].cote * move[x].ax;
					objet[k].dy = move[x].ay;
				}

				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}
			break;
		case 85: //Coup de batte
		case 89: //Smash batte
			x = defperso[perso[i].id].idcoups + perso[i].acti;

			if(perso[i].dx) freineperso(i, sgnx, 3); //Freinage

			if(animperso(i) ) {
				if( (perso[i].acti == 89) && (defobjet[objet[perso[i].objet].id].role == role_batonetoile) ) {
					creeproject(objet[perso[i].objet].x, objet[perso[i].objet].y, perso[i].cote * 6000, 0, proj_etoile, i);
				}

				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			} else {
				if(perso[i].stat == move[x].nani) { //Test collision
					if(testeutilobjet(i, perso[i].objet, perso[i].cote * move[x].ax, move[x].ay) ) {
						if( (perso[i].acti == 89) && (defobjet[objet[perso[i].objet].id].role == role_batte) ) PlaySoundFX(&sound[son_batte], 0);
					}
				}
			}

			if(perso[i].stat == move[x].nani) {
				objet[perso[i].objet].x = perso[i].x + perso[i].cote * (move[defperso[perso[i].id].idcoups + perso[i].acti].dx << 10);
				objet[perso[i].objet].y = perso[i].y - (move[defperso[perso[i].id].idcoups + perso[i].acti].dy << 10);

				objet[perso[i].objet].imag = defobjet[objet[perso[i].objet].id].idim + defobjet[objet[perso[i].objet].id].nrot + 3; //On affiche l'objet
				objet[perso[i].objet].disp = true;
			}
			break;
		case 86: //Marteau
			x = defperso[perso[i].id].idcoups + perso[i].acti;

			if(perso[i].dx) freineperso(i, sgnx, 3); //Freinage

			if(animperso(i) ) {
				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			} else {
				if(perso[i].stat == move[x].nani) { //Test collision
					testeutilobjet(i, perso[i].objet, perso[i].cote * move[x].ax, move[x].ay);
				}
			}

			objet[perso[i].objet].x = perso[i].x + perso[i].cote * (clist[move[x].dx + perso[i].stat][0] << 10);
			objet[perso[i].objet].y = perso[i].y - (clist[move[x].dx + perso[i].stat][1] << 10);

			objet[perso[i].objet].imag = defobjet[objet[perso[i].objet].id].idim + defobjet[objet[perso[i].objet].id].nrot + 3 + perso[i].stat; //On affiche l'objet
			objet[perso[i].objet].disp = true;
			break;
		case 87: //Pistolet
			x = defperso[perso[i].id].idcoups + perso[i].acti;

			if(perso[i].dx) freineperso(i, sgnx, 3); //Freinage

			k = perso[i].objet;
			if(animperso(i) ) {
				if(k) {
					if(objet[k].stat > 0) { //Munitions
						if(defobjet[objet[k].id].atta < 500) { //Scope
							creeproject(objet[k].x, objet[k].y, perso[i].cote * 7000, 0, proj_scope, i);
						} else creeproject(objet[k].x, objet[k].y, perso[i].cote * 8000, 0, proj_raygun, i); //Raygun
						objet[k].stat--;

						PlaySoundFX(&sound[son_raygun], 0);
					} else PlaySoundFX(&sound[son_mine], 0); //Vide
				}

				perso[i].acti = 1;
				perso[i].stat = 0;
				perso[i].stata = 0;
			}

			if(k && (perso[i].stat == move[x].nani) ) {
				objet[k].x = perso[i].x + perso[i].cote * (move[x].dx << 10);
				objet[k].y = perso[i].y - (move[x].dy << 10);

				objet[k].imag = defobjet[objet[k].id].idim + defobjet[objet[k].id].nrot + 3; //On affiche l'objet
				objet[k].disp = true;
			}
			break;
		default: //Coups
			k = defperso[perso[i].id].idcoups + perso[i].acti;

			if(perso[i].dx) freineperso(i, sgnx, 3); //Freinage

			switch(move[k].type) {
			case move_normal: //Coup simple
				if(animperso(i) ) {
					testecoup(i, k, false);
					perso[i].acti = 1;
					perso[i].stat = 0;
					perso[i].stata = 0;
				}
				break;
			case move_smash: //Coup smash
				if( (perso[i].stat == 0) && (perso[i].stata == 0) ) {
					if(perso[i].cote > 0) {
						creeexplos(perso[i].x >> 10, perso[i].y >> 10, explos_fsmash, nb_explos_fsmash, 2);
					} else creeexplos(perso[i].x >> 10, perso[i].y >> 10, explos_fsmash2, nb_explos_fsmash, 2);
				}

				if(animperso(i) ) {
					perso[i].acti = 1;
					perso[i].stat = 0;
					perso[i].stata = 0;
				} else {
					if(perso[i].stat == move[k].nani) testecoup(i, k, true);
				}
				break;
			case move_prise: //Tentative prise
				if(animperso(i) ) {
					if( (j = testecolperso(i, perso[i].x + perso[i].cote * (move[k].dx << 10), perso[i].y - (move[k].dy << 10) ) ) ) { //Attrapé
						perso[i].acti = move[k].atta;
						perso[i].stat = 0;
						perso[i].stata = 0;
						perso[i].pecho = j - 1;

						perso[j - 1].acti = -1;
						perso[j - 1].stat = 0;
						perso[j - 1].stata = 0;
						perso[j - 1].dx = 0;
						perso[j - 1].dy = 0;
						perso[j - 1].pecho = i;
						perso[j - 1].tape = i;
						perso[j - 1].protect = false;
					} else {
						perso[i].acti = 1;
						perso[i].stat = 0;
						perso[i].stata = 0;
					}
				}
				break;
			case move_priseec: //Prise en cours
				if(animperso(i) ) {
					perso[i].acti = 1;
					perso[i].stat = 0;
					perso[i].stata = 0;

					perso[perso[i].pecho].acti = 0;
					perso[perso[i].pecho].stat = 0;
					perso[perso[i].pecho].stata = 0;
					degatperso(perso[i].pecho, perso[perso[i].pecho].x, perso[perso[i].pecho].y, perso[i].cote * move[k].ax, move[k].ay, move[k].atta, true);

					perso[perso[i].pecho].pecho = -1;
					perso[i].pecho = -1;
				} else {
					perso[perso[i].pecho].dx = 0;
					perso[perso[i].pecho].dy = 0;
					perso[perso[i].pecho].x = perso[i].x + perso[i].cote * (clist[move[k].dx + perso[i].stat][0] << 10);
					perso[perso[i].pecho].y = perso[i].y - (clist[move[k].dx + perso[i].stat][1] << 10);
					perso[perso[i].pecho].cote = -perso[i].cote * clist[move[k].dx + perso[i].stat][2];
				}
				break;
			case move_projectile: //Projectile
				if(animperso(i) ) {
					perso[i].acti = 1;
					perso[i].stat = 0;
					perso[i].stata = 0;
					creeproject(perso[i].x + perso[i].cote * (move[k].dx << 10), perso[i].y - (move[k].dy << 10), perso[i].cote * move[k].ax, move[k].ay, move[k].atta, i);
				}
				break;
			default: //Coup tackle - plongeon
				if( (perso[i].stat < 1) && (perso[i].stata < 2) ) { //Accélération
					perso[i].dx += perso[i].cote * move[k].ax;
					perso[i].dy += move[k].ay;
					if(move[k].ay < 0) { //Doit monter
						if(perso[i].dy < 0) {
							perso[i].dy = move[k].ay;
						} else perso[i].dy = move[k].ay >> 1;
					}
				} else {
					if(testecoup(i, k, true) ) perso[i].cpasse = true;
				}

				if(animperso(i) ) {
					if(move[k].type < 64) { //Dernier mouvement
						if(perso[i].dx * sgnx < 100) {
							perso[i].acti = 1;
							perso[i].stat = 0;
							perso[i].stata = 0;
						}
					} else {
						switch(move[k].dx2) { //Combo manette
						case 1: //Toutes directions
							if(ddhaut || ddgauche || ddbas || dddroite) { //Ok, activé
								if(ddhaut) perso[i].acti = move[k].type;
								if(ddgauche) perso[i].acti = move[k].type + 1;
								if(ddbas) perso[i].acti = move[k].type + 2;
								if(dddroite) perso[i].acti = move[k].type + 3;
							} else perso[i].acti = 1;
							break;
						case 2: //Haut
							if(ddhaut) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 3: //Gauche
							if(ddgauche) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 4: //Bas
							if(ddbas) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 5: //Droite
							if(dddroite) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 6: //A
							if(ba) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 7: //A haut
							if(ba && ddhaut) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 8: //A gauche
							if(ba && ddgauche) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 9: //A bas
							if(ba && ddbas) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 10: //A droite
							if(ba && dddroite) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 11: //B
							if(bb) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 12: //B haut
							if(bb && ddhaut) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 13: //B gauche
							if(bb && ddgauche) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 14: //B bas
							if(bb && ddbas) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						case 15: //B droite
							if(bb && dddroite) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
							break;
						default: //Enchainement auto (si 1er passe)
							if(perso[i].cpasse) {
								perso[i].acti = move[k].type;
							} else perso[i].acti = 1;
						}
						perso[i].stat = 0;
						perso[i].stata = 0;
					}
					perso[i].cpasse = false;
				}
			}
		}
	} else { //Assommé
		if(perso[i].pecho >= 0) { //On s'est fait pecho, on subit
			if(perso[perso[i].pecho].acti < 30) {
				perso[i].acti = 23;
				perso[i].stat = 0;
				perso[i].stata = 0;
				perso[i].pecho = -1;
			}

			j = defperso[perso[i].id].idcoups + 25;
		} else {
			if(timer3 == 0) perso[i].acti++;

			if( ( (perso[i].y >> 14) != ( (perso[i].y + 1024) >> 14) ) && (typedecor[tabl[perso[i].x >> 14][(perso[i].y >> 14) + 1]] < 1) ) { //Au sol
				if(perso[i].dx) freineperso(i, sgnx, 4); //Freinage

				j = defperso[perso[i].id].idcoups + 24;
			} else {
//			if(perso[i].dy) { //On est en l'air, donc on tourne
				if(perso[i].dx) freineperso(i, sgnx, 1);

				if(perso[i].acti >= 0) { //On se rattrappe en l'air
					perso[i].acti = 5;
					perso[i].stat = 0;
					perso[i].stata = 0;
				} else { //Limite de durée assommé en l'air
					if(perso[i].acti < -50) perso[i].acti = -50;
				}

				j = defperso[perso[i].id].idcoups + 23;
			}
		}

		perso[i].imag = move[j].idim + perso[i].stat;

		perso[i].stata += move[j].vite;
		if(perso[i].stata > 9) {
			perso[i].stata -= 10;
			perso[i].stat++; //Animation
			if(perso[i].stat > move[j].nani) { //Fin animation
				if(move[j].loop) {
					perso[i].stat = 0;
				} else perso[i].stat = move[j].nani;
			}
		}
	}

	j = perso[i].objet;
	if(j) { //Coordonnées objet
		if(objet[j].ener > 0) {
			if(perso[i].acti < 80) {
				objet[j].x = perso[i].x + perso[i].cote * (move[defperso[perso[i].id].idcoups].dx << 10);
				objet[j].y = perso[i].y - (move[defperso[perso[i].id].idcoups].dy << 10);
				objet[j].disp = true;
				objet[j].imag = defobjet[objet[j].id].idim + defobjet[objet[j].id].nrot + 2;
			}
			objet[j].cote = perso[i].cote;
		} else perso[i].objet = 0;
	}
	perso[i].bouge = false;

	if(i < nb_joueurs) {
		if(joueur[i].cpu) gere_ia(i);
	} else gere_ia(i);
}

void gere_objet(int i) {
//	register j;
	int j;
	int air = false;
	int sgnx, sgny;

	if( (objet[i].x > 16384) && (objet[i].x < xmaxm) && (objet[i].y > 16384) && (objet[i].y < ymaxm) ) { //A l'écran
		int c = true;

		if(objet[i].dx > 0) {
			sgnx = 1;
		} else sgnx = -1;
		if(objet[i].dy > 0) {
			sgny = 1;
		} else sgny = -1;

		int a = sgnx * objet[i].dx; //Valeurs absolues
		int b = sgny * objet[i].dy;
		while(a || b || c) {
			c = false;
			if(a > 8000) {
				objet[i].x += (8000 * sgnx);
				a -= 8000;
			} else {
				objet[i].x += (a * sgnx);
				a = 0;
			}
			if(b > 8000) {
				objet[i].y += (8000 * sgny);
				b -= 8000;
			} else {
				objet[i].y += (b * sgny);
				b = 0;
			}

			if(objet[i].dx || objet[i].dy) { //Collisions avec joueurs
				if(defobjet[objet[i].id].role == role_etoile) { //Etoile
					if( (j = testecolperso(-1, objet[i].x, objet[i].y) ) ) {
						perso[j - 1].invinc = 750;
						baisseenerobjet(i, 30000);

						PlaySoundFX(&sound[son_smbatet], 0);
					}
				}

				if(objet[i].prop >= 0) {
					if( (j = testecolperso(objet[i].prop, objet[i].x, objet[i].y) ) ) {
						if( (defobjet[objet[i].id].role == role_explose) || (defobjet[objet[i].id].role == role_bobomb) ) { //Explosion
							creeexplosdg(objet[i].x, objet[i].y, defobjet[objet[i].id].atta, objet[i].prop);
							baisseenerobjet(i, 30000);
						} else {
							if(degatperso(j - 1, objet[i].x, objet[i].y, objet[i].dx >> 2, objet[i].dy - 4000, defobjet[objet[i].id].atta, true) ) {
								perso[j - 1].invinc = 1;
								perso[j - 1].tape = objet[i].prop;
								baisseenerobjet(i, (objet[i].dx * sgnx) >> 3);

								objet[i].dy = -(objet[i].dx * sgnx) >> 2;
//								objet[i].dx = GpRandN(3000) - 1500;
								objet[i].dx = mp_Rand(3000) - 1500;
								objet[i].prop = -1;
							}
						}

						PlaySoundFX(&sound[son_applau01], 0);
					}
				}
			}

			air = false;
			if(typedecor[tabl[objet[i].x >> 14][objet[i].y >> 14]] < 1) { //Collision avec bloc dessous
				b = 0;
				baisseenerobjet(i, (objet[i].dy * sgny) >> 3);

				objet[i].y = ( (objet[i].y >> 14) << 14) - 1024;
				if(defobjet[objet[i].id].role == role_mine) {
					objet[i].dx >>= 8;
					objet[i].dy = -(objet[i].dy >> 8);
				} else objet[i].dy = -(objet[i].dy >> 2);
				sgny = -sgny;

				if(objet[i].dy * sgny < 500) {
					objet[i].dy = 0; //A terre
				} else air = true;
			} else {
				if( ( (objet[i].y >> 14) != ( (objet[i].y + 1024) >> 14) ) && (typedecor[tabl[objet[i].x >> 14][(objet[i].y >> 14) + 1]] < 1) ) { //Pile sur une case
					objet[i].imag = defobjet[objet[i].id].idim + defobjet[objet[i].id].nrot + 1;
				} else {
					if(objet[i].dy < 0) { //Collision au-dessus
						if(typedecor[tabl[objet[i].x >> 14][( (objet[i].y >> 10) - obje[objet[i].imag].h) >> 4]] < 1) { //Boum
							b = 0;
							objet[i].dy = -(objet[i].dy >> 4);
							sgny = -sgny;
						}
					}
					air = true;
				}
			}

			if(typedecor[tabl[(objet[i].x + sgnx * (obje[objet[i].imag].w << 9) ) >> 14][objet[i].y >> 14]] < 1) { //Collision sur coté
				a = 0;
				objet[i].x -= objet[i].dx;
				baisseenerobjet(i, (objet[i].dx * sgnx) >> 3);

				objet[i].dx = -(objet[i].dx >> 1);
				sgnx = -sgnx;

				if(objet[i].stat) objet[i].cote = -objet[i].cote;
			}
		}

		if(air) {
			objet[i].dy += (area[num_area].gravite + defobjet[objet[i].id].poids);
			if(objet[i].dx) {
				objet[i].dx -= (sgnx * defobjet[objet[i].id].poids);
				if(sgnx * objet[i].dx < 0) objet[i].dx = 0;
			}

			if(timer3 == 2) objet[i].imag++;
			if(objet[i].imag > (defobjet[objet[i].id].idim + defobjet[objet[i].id].nrot) ) objet[i].imag = defobjet[objet[i].id].idim;
		} else {
			if(objet[i].dx) {
				objet[i].dx -= (sgnx * defobjet[objet[i].id].poids * 2);
				if(sgnx * objet[i].dx < 0) objet[i].dx = 0;
			} else {
				if( (objet[i].stat == 0) || (defobjet[objet[i].id].role == role_pistolet) ) {
					objet[i].prop = -1;

					objet[i].time--;
					if( (objet[i].time < 100) && (defobjet[objet[i].id].role == role_bobomb) ) { //Activation bob-omb
						if(objet[i].stat) { //Déja activée
							if(objet[i].stat < 5) { //Clignote 4 secondes
								objet[i].time = 300;
								objet[i].stat = 5;
							} else objet[i].stat = 7; //Boum
						} else { //Va marcher, 10 secondes
							objet[i].time = 600;
							objet[i].stat = 1;
						}
					}

					if(objet[i].time < 1) objet[i].ener = 0; //Timeout
				}
			}
		}
	} else objet[i].ener = 0;

	switch(defobjet[objet[i].id].role) { //Gestion spéciale
	case role_bumper:
		if(!air) {
			
		}
		break;
	case role_carapr:
		if(!air && objet[i].stat) { //On bouge
			int tgt = -1;
			int a = 100000;
			int b, c;

			for(j = 0; j < nb_persos; j++) { //Recherche cible + proche
				if(perso[j].enjeu) {
					b = (objet[i].x - perso[j].x) >> 10;
					c = (objet[i].y - perso[j].y) >> 9;
					b = b * b + c * c;
					if(b < a) {
						a = b;
						tgt = j;
					}
				}
			}

			if(tgt >= 0) {
				if(perso[tgt].x > objet[i].x) { //Droite
					objet[i].cote = 1;
				} else objet[i].cote = -1; //Gauche

				if(objet[i].dx * objet[i].cote < defobjet[objet[i].id].atta * 3) { //Pas assez vite
					objet[i].dx += objet[i].cote * 320;
				}
			}

			objet[i].imag = defobjet[objet[i].id].idim + objet[i].stat - 1;
			if(timer3 == 2) { //Animation
				objet[i].stat++;
				if(objet[i].stat > defobjet[objet[i].id].nrot + 1) objet[i].stat = 1;
			}

			if(testeutilobjet(objet[i].prop, i, objet[i].cote * 5000, -4000) ) {
				baisseenerobjet(i, 1000);

				PlaySound(&sound[son_carap]);
			} else {
				if(testeutilobjet(-1, i, objet[i].cote * 5000, -4000) ) {
					baisseenerobjet(i, 1000);

					PlaySound(&sound[son_carap]);
				}
			}
		}
		break;
	case role_carapv:
		if(!air && objet[i].stat) { //On bouge
			if(objet[i].dx * objet[i].cote < defobjet[objet[i].id].atta * 2) { //Pas assez vite
				objet[i].dx += objet[i].cote * 200;
			}

			objet[i].imag = defobjet[objet[i].id].idim + objet[i].stat - 1;
			if(timer3 == 2) { //Animation
				objet[i].stat++;
				if(objet[i].stat > defobjet[objet[i].id].nrot + 1) objet[i].stat = 1;
			}

			if(testeutilobjet(objet[i].prop, i, objet[i].cote * 4000, -3000) ) {
				baisseenerobjet(i, 1000);

				PlaySound(&sound[son_carap]);
			} else {
				if(testeutilobjet(-1, i, objet[i].cote * 4000, -3000) ) {
					baisseenerobjet(i, 1000);

					PlaySound(&sound[son_carap]);
				}
			}
		}
		break;
	case role_bobomb:
		if(!air && objet[i].stat) {
			if(objet[i].stat < 5) { //Marche
				if(objet[i].dx * objet[i].cote < 1024) { //Pas assez vite
					objet[i].dx += objet[i].cote * 200;
				}
	
				objet[i].imag = defobjet[objet[i].id].idim + defobjet[objet[i].id].nrot + 2 + objet[i].stat;
				if(timer3 == 2) { //Animation
					objet[i].stat++;
					if(objet[i].stat > 4) objet[i].stat = 1;
				}
			} else {
				if(objet[i].stat < 7) { //Clignote
					objet[i].imag = defobjet[objet[i].id].idim + defobjet[objet[i].id].nrot + 2 + objet[i].stat;
					if(timer3 == 2) { //Animation
						objet[i].stat++;
						if(objet[i].stat > 6) objet[i].stat = 5;
					}
				} else { //Boum
					creeexplosdg(objet[i].x, objet[i].y, defobjet[objet[i].id].atta, -1);
					baisseenerobjet(i, 30000);
				}
			}

			testeutilobjet(-1, i, 0, 0);
//			if(testeutilobjet(-1, i, 0, 0) ) {
//				creeexplosdg(objet[i].x, objet[i].y, defobjet[objet[i].id].atta);
//				baisseenerobjet(i, 30000);
//			}
		}
		break;
	case role_mine:
		if(!air && objet[i].stat) { //Activée
			if(objet[i].stat == 1) { //Clic :)
				PlaySoundFX(&sound[son_mine], 0);
				objet[i].stat = 2;
			}
			objet[i].imag = defobjet[objet[i].id].idim + defobjet[objet[i].id].nrot + 3;

//			testeutilobjet(-1, i, 0, 0);
			if(testeutilobjet(-1, i, 0, 0) ) {
				creeexplosdg(objet[i].x, objet[i].y, defobjet[objet[i].id].atta, objet[i].prop);
				baisseenerobjet(i, 30000);
			}
		}
		break;
	case role_etoile:
//		if(objet[i].stat == 0) objet[i].stat = GpRandN(2) * 2 - 1; //Choix direction
		if(objet[i].stat == 0) objet[i].stat = mp_Rand(2) * 2 - 1; //Choix direction
		if(objet[i].dx * objet[i].stat < 1500) objet[i].dx += objet[i].stat * 200;

		if(!air) objet[i].dy -= 5000;
		break;
	}
}

void gere_proj(int i) {
//	register j;
	int j;
	int sgnx, sgny;

	if( (project[i].x > 16384) && (project[i].x < xmaxm) && (project[i].y > 16384) && (project[i].y < ymaxm) ) { //A l'écran
		int c = true;

		if(project[i].dx > 0) {
			sgnx = 1;
		} else sgnx = -1;
		if(project[i].dy > 0) {
			sgny = 1;
		} else sgny = -1;

		int a = sgnx * project[i].dx; //Valeurs absolues
		int b = sgny * project[i].dy;
		while(a || b || c) {
			c = false;
			if(a > 8000) {
				project[i].x += (8000 * sgnx);
				a -= 8000;
			} else {
				project[i].x += (a * sgnx);
				a = 0;
			}
			if(b > 8000) {
				project[i].y += (8000 * sgny);
				b -= 8000;
			} else {
				project[i].y += (b * sgny);
				b = 0;
			}

			if( (j = testecolperso(project[i].prop, project[i].x, project[i].y) ) ) {
				if(degatperso(j - 1, project[i].x, project[i].y, project[i].dx >> 1, project[i].dy - 5000, defproject[project[i].id].atta, defproject[project[i].id].smash) ) {
					perso[j - 1].invinc = 1;
					perso[j - 1].tape = project[i].prop;
				}
				project[i].port = 0;
			} else {
				if(typedecor[tabl[project[i].x >> 14][project[i].y >> 14]] < 1) project[i].port = 0; //Collision bloc
			}
		}

		if(timer3 == 1) {
			project[i].imag++;
			if(project[i].imag > (defproject[project[i].id].idim + defproject[project[i].id].nani) ) project[i].imag = defproject[project[i].id].idim;
		}

		project[i].port--;
		if(project[i].port < 0) project[i].port = 0;
	} else project[i].port = 0;
}
