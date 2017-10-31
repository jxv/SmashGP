#define		SCREEN_WIDTH	480
#define		SCREEN_HEIGHT	272

unsigned short *pg_vramtop = (unsigned short *)0x44000000;

// PSP
void PreZoom(const unsigned short *src) { //Mériterait une routine de bilinear resize
	unsigned short *tmpa;
	unsigned short *dst = (unsigned short *)bmpbuffer + 0x20000; //512 x 256
	//unsigned short *dst = (unsigned short *)pg_vramtop + 2 * (FRAMESIZE >> 1) + 0x20000; //512 x 256
	int xmin;
	int ymax = 256;

	while(ymax--) {
		tmpa = (unsigned short *)(src + ( ( (ymax) * 960) >> 10) * 640);
		xmin = 512;
		while(xmin--) *dst-- = *(tmpa + ( ( (xmin) * 1280) >> 10) );
	}
}

void mp_InitGu(int numsurface) {
//	sceGuInit();
	sceGuStart(GU_DIRECT, gu_list);

	sceGuDrawBuffer(GU_PSM_5551, (void*)(FRAMESIZE * numsurface), 512);
	sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, (void*)(FRAMESIZE * (1 - numsurface) ), 512);

	sceGuOffset(2048 - (SCREEN_WIDTH >> 1), 2048 - (SCREEN_HEIGHT >> 1) );
	sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	sceGuTexFilter(GU_LINEAR, GU_LINEAR);
	sceGuTexScale(1.0f, 1.0f);
	sceGuTexOffset(0.0f, 0.0f);
	sceGuTexWrap(GU_CLAMP, GU_CLAMP);

	sceGuFinish();
	sceGuSync(0, 0);
}

// PSP
void ZoomTransBlit(const int numsurface, const int dx, const int dy, const int width, const int height, const unsigned short *src, const unsigned short trans, const int cote) {
	int x2 = divzoom(dx);
	int y2 = divzoom(dy);
	int w2 = divzoom(width);
	int h2 = divzoom(height);
	if( (w2 < 1) || (h2 < 1) ) return;
//	int ws = makesprite( (unsigned short *)src, (unsigned short *)textbuf_[dbt_], width, height, cote);
	unsigned short *tmp_ptr = edram_ptr;
	int ws = makesprite( (unsigned short *)src, (unsigned short *)tmp_ptr, width, height, cote);
	int hs = 1;

	while(hs < height) hs <<= 1;
	if( (ws > 512) || (hs > 512) ) return;

//	sceGuTexImage(0, ws, hs, ws, (unsigned short *)textbuf_[dbt_]);
	sceGuTexImage(0, ws, hs, ws, (unsigned short *)tmp_ptr);
//	dbt_ = 1 - dbt_;

	struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
	vertices[0].u = 0;
	vertices[0].v = 0;
	vertices[0].x = x2;
	vertices[0].y = y2;
	vertices[0].z = 0;
	vertices[1].u = width;
	vertices[1].v = height;
	vertices[1].x = x2 + w2;
	vertices[1].y = y2 + h2;
	vertices[1].z = 0;
	sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
}

// PSP
void ZoomSolidBlit(const int numsurface, const int dx, const int dy, const int width, const int height, const unsigned short *src, const unsigned short trans, const unsigned short coul, const int cote) {
	int x2 = divzoom(dx);
	int y2 = divzoom(dy);
	int w2 = divzoom(width);
	int h2 = divzoom(height);
	if( (w2 < 1) || (h2 < 1) ) return;
//	int ws = makespritesolid( (unsigned short *)src, (unsigned short *)textbuf_[dbt_], width, height, coul, cote);
	unsigned short *tmp_ptr = edram_ptr;
	int ws = makespritesolid( (unsigned short *)src, (unsigned short *)tmp_ptr, width, height, coul, cote);
	int hs = 1;

	while(hs < height) hs <<= 1;
	if( (ws > 512) || (hs > 512) ) return;

//	sceGuTexImage(0, ws, hs, ws, (unsigned short *)textbuf_[dbt_]);
	sceGuTexImage(0, ws, hs, ws, (unsigned short *)tmp_ptr);
//	dbt_ = 1 - dbt_;

	struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex) );
	vertices[0].u = 0;
	vertices[0].v = 0;
	vertices[0].x = x2;
	vertices[0].y = y2;
	vertices[0].z = 0;
	vertices[1].u = width;
	vertices[1].v = height;
	vertices[1].x = x2 + w2;
	vertices[1].y = y2 + h2;
	vertices[1].z = 0;
	sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
}

// PSP
void mp_FastBlit(const int numsurface, const int dx, const int dy, const int width, const int height, const unsigned short *src) {
	int xmin, ymin, xmax, ymax;

	if(dx < 0) {
		xmin = -dx;
	} else xmin = 0;
	if( (dx + width) > SCREEN_WIDTH) {
		xmax = SCREEN_WIDTH - dx - 1;
	} else xmax = width - 1;
	if(dy < 0) {
		ymin = -dy;
	} else ymin = 0;
	if( (dy + height) > SCREEN_HEIGHT) {
		ymax = SCREEN_HEIGHT - dy - 1;
	} else ymax = height - 1;
	if( (xmin > xmax) || (ymin > ymax) ) return;

	unsigned short *dst = (unsigned short *)pg_vramtop + numsurface * (FRAMESIZE >> 1) + dx + xmax + (dy + ymax) * 512;
	src += (ymax * width + xmax);

	xmax = xmax - xmin + 1;
	ymax = ymax - ymin + 1;
	ymin = 512 - xmax;
	int dech = width - xmax;
	while(ymax--) {
		xmin = xmax;
		while(xmin--) *dst-- = *src--;
		dst -= ymin;
		src -= dech;
	}
}

// PSP
void mp_FontBlit(const int numsurface, const int dx, const int dy, const int width, const int height, const unsigned short *src, const int fontw) {
	int xmin, ymin, xmax, ymax;

	if(dx < 0) {
		xmin = -dx;
	} else xmin = 0;
	if( (dx + width) > SCREEN_WIDTH) {
		xmax = SCREEN_WIDTH - dx - 1;
	} else xmax = width - 1;
	if(dy < 0) {
		ymin = -dy;
	} else ymin = 0;
	if( (dy + height) > SCREEN_HEIGHT) {
		ymax = SCREEN_HEIGHT - dy - 1;
	} else ymax = height - 1;
	if( (xmin > xmax) || (ymin > ymax) ) return;

	unsigned short *dst = (unsigned short *)pg_vramtop + numsurface * (FRAMESIZE >> 1) + dx + xmax + (dy + ymax) * 512;
	src += (ymax * fontw + xmax);

	xmax = xmax - xmin + 1;
	ymax = ymax - ymin + 1;
	ymin = 512 - xmax;
	int dech = fontw - xmax;
	while(ymax--) {
		xmin = xmax;
		while(xmin--) *dst-- = *src--;
		dst -= ymin;
		src -= dech;
	}
}

// PSP
void FastTransBlit(const int numsurface, const int dx, const int dy, const int width, const int height, const unsigned short *src, const unsigned short trans) {
	int xmin, ymin, xmax, ymax;

	if(dx < 0) {
		xmin = -dx;
	} else xmin = 0;
	if( (dx + width) > SCREEN_WIDTH) {
		xmax = SCREEN_WIDTH - dx - 1;
	} else xmax = width - 1;
	if(dy < 0) {
		ymin = -dy;
	} else ymin = 0;
	if( (dy + height) > SCREEN_HEIGHT) {
		ymax = SCREEN_HEIGHT - dy - 1;
	} else ymax = height - 1;
	if( (xmin > xmax) || (ymin > ymax) ) return;

	unsigned short *dst = (unsigned short *)pg_vramtop + numsurface * (FRAMESIZE >> 1) + dx + xmax + (dy + ymax) * 512;
	src += (ymax * width + xmax);
	unsigned short a;

	xmax = xmax - xmin + 1;
	ymax = ymax - ymin + 1;
	ymin = 512 - xmax;
	int dech = width - xmax;
	while(ymax--) {
		xmin = xmax;
		while(xmin--) {
			a = *src--;
			if(a != trans) *dst = a;
			dst--;
		}
		dst -= ymin;
		src -= dech;
	}
}

// PSP
void FontTransBlit(const int numsurface, const int dx, const int dy, const int width, const int height, const unsigned short *src, const unsigned short trans, const int fontw) {
	int xmin, ymin, xmax, ymax;

	if(dx < 0) {
		xmin = -dx;
	} else xmin = 0;
	if( (dx + width) > SCREEN_WIDTH) {
		xmax = SCREEN_WIDTH - dx - 1;
	} else xmax = width - 1;
	if(dy < 0) {
		ymin = -dy;
	} else ymin = 0;
	if( (dy + height) > SCREEN_HEIGHT) {
		ymax = SCREEN_HEIGHT - dy - 1;
	} else ymax = height - 1;
	if( (xmin > xmax) || (ymin > ymax) ) return;

	unsigned short *dst = (unsigned short *)pg_vramtop + numsurface * (FRAMESIZE >> 1) + dx + xmax + (dy + ymax) * 512;
	src += (ymax * fontw + xmax);
	unsigned short a;

	xmax = xmax - xmin + 1;
	ymax = ymax - ymin + 1;
	ymin = 512 - xmax;
	int dech = fontw - xmax;
	while(ymax--) {
		xmin = xmax;
		while(xmin--) {
			a = *src--;
			if(a != trans) *dst = a;
			dst--;
		}
		dst -= ymin;
		src -= dech;
	}
}

// PSP
void SaveBitmap(const int numsurface, const int dx, const int dy, const int width, const int height, unsigned short *dest) { //Sur l'écran
	int xmin, ymin, xmax, ymax;

	if(dx < 0) {
		xmin = -dx;
	} else xmin = 0;
	if( (dx + width) > SCREEN_WIDTH) {
		xmax = SCREEN_WIDTH - dx - 1;
	} else xmax = width - 1;
	if(dy < 0) {
		ymin = -dy;
	} else ymin = 0;
	if( (dy + height) > SCREEN_HEIGHT) {
		ymax = SCREEN_HEIGHT - dy - 1;
	} else ymax = height - 1;
	if( (xmin > xmax) || (ymin > ymax) ) return;

	unsigned short *src = (unsigned short *)pg_vramtop + numsurface * (FRAMESIZE >> 1) + dx + xmax + (dy + ymax) * 512;
	dest += (ymax * width + xmax);

	xmax = xmax - xmin + 1;
	ymax = ymax - ymin + 1;
	ymin = 512 - xmax;
	int dech = width - xmax;
	while(ymax-- > 0) {
		xmin = xmax;
		while(xmin-- > 0) *dest-- = *src--;
		src -= ymin;
		dest -= dech;
	}
}

void hline(const int numsurface, const int x, const int y, int n, const short coul) {
	unsigned short *dst = (unsigned short *)pg_vramtop + numsurface * (FRAMESIZE >> 1) + x + y * 512;

	while(n--) *dst++ = coul;
}

void vline(const int numsurface, const int x, const int y, int n, const short coul) {
	unsigned short *dst = (unsigned short *)pg_vramtop + numsurface * (FRAMESIZE >> 1) + x + y * 512;

	while(n--) {
		*dst = coul;
		dst += 512;
	}
}

// PSP
void StringGameTransBlit(const int numsurface, int dx, const int dy, const char *text) { //Transparent
	int i = 0;
	int a;

	while(text[i]) {
		a = text[i++] - 32; //On recale sur nos fontes
		if(a > 0) a -= 4;
		if(a > 1) a -= 10;

		FontTransBlit(numsurface, dx, dy, font01x[a + 1] - font01x[a], font01_h, (unsigned short *)font01 + font01x[a], COUL_TRANS, font01_w);
		dx += (font01x[a + 1] - font01x[a] + 1);
	}
}

// PSP
void StringTimeTransBlit(const int numsurface, int dx, const int dy, const char *text) { //Transparent
	int i = strlen(text);
	int a;

	while(i > 0) {
		a = text[--i] - 32; //On recale sur nos fontes
		if(a > 0) a -= 15;

		dx -= (font03x[a + 1] - font03x[a] + 1);
		FontTransBlit(numsurface, dx, dy, font03x[a + 1] - font03x[a], font03_h, (unsigned short *)font03 + font03x[a], COUL_TRANS, font03_w);
	}
}

// PSP
void StringTransBlit(const int numsurface, int dx, const int dy, const char *text) { //Transparent
	int i = 0;
	int a;

	while(text[i] ) {
		a = text[i++] - 32; //On recale sur nos fontes

		FontTransBlit(numsurface, dx, dy, font02_w, font02_h, (unsigned short *)font02 + a * font02_w, COUL_TRANS, font02_width);
		dx += font02_w;
	}
}

// PSP
int CharTransBlit(const int numsurface, const int dx, const int dy, const char g_char) { //Transparent
	int a = g_char - 32; //On recale sur nos fontes

	FontTransBlit(numsurface, dx, dy, font02_w, font02_h, (unsigned short *)font02 + a * font02_w, COUL_TRANS, font02_width);

	return dx + font02_w;
}

void mp_ClrScr(unsigned int numsurface, unsigned short coul) {
	unsigned short *dst = (unsigned short *)pg_vramtop + numsurface * (FRAMESIZE >> 1);
	int i;

	for(i = 0; i < (FRAMESIZE >> 1); i++) *dst++ = coul;
}

static inline int getafficheid(int i) {
	switch(i) {
	case 0:
		if(joueur[0].cpu) {
			return explos_idcpr;
		} else return explos_id1p;
	case 1:
		if(joueur[1].cpu) {
			return explos_idcpv;
		} else return explos_id2p;
	case 2:
		return explos_idcpb;
	default:
		return explos_idcpj;
	}
}

void affichage() { //Affichage du background
	int xshake = xscreenb + tabsecouex[secoue];
	int yshake = yscreenb + tabsecouey[secoue];
	int xmshake = xshake >> 4;
	int ymshake = yshake >> 4;
	int dxscreen = xshake - xmshake * 16;
	int dyscreen = yshake - ymshake * 16;
	int i, j;
	int a, b;
	int k, l;
	int scrwb = scrw + 20;
	int scrhb = scrh + 20;

	int xz, yz;
	int xzn, yzn;

	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexMode(GU_PSM_5551, 0, 0, GU_TRUE); //Les tiles sont en mode swizzle
	sceGuEnable(GU_BLEND);
	//sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x909090, 0x6f6f6f);
	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, trans24, omtrans24);
	sceGuAlphaFunc(GU_GREATER, 0x80, 0xff);
	sceGuEnable(GU_ALPHA_TEST);

	yzn = -divzoomb(dyscreen); //Background
	for(j = 0; j < disp_heightb; j++) {
		xzn = -divzoomb(dxscreen);
		yz = yzn;
		yzn = divzoomb( (j + 1) * 16 - dyscreen);

		for(i = 0; i < disp_widthb; i++) {
			xz = xzn;
			xzn = divzoomb( (i + 1) * 16 - dxscreen);

			k = tablb[xmshake + i][ymshake + j];
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
	sceGuTexMode(GU_PSM_5551, 0, 0, 0); //Pas de swizzle pour les sprites :)

	xshake = xscreen + tabsecouex[secoue];
	yshake = yscreen + tabsecouey[secoue];
	xmshake = xshake >> 4;
	ymshake = yshake >> 4;
	dxscreen = xshake - xmshake * 16;
	dyscreen = yshake - ymshake * 16;

	for(i = 0; i < nb_persos; i++) { //Joueurs en jeu
		if(perso[i].enjeu) {
			xz = (perso[i].x >> 10) - xshake;
			yz = (perso[i].y >> 10) - yshake;
			if( (xz >= 0) && (xz <= scrw) && (yz >= 0) && (yz <= (scrh + pers[move[defperso[perso[i].id].idcoups].idim].h) ) ) { //En jeu
				k = perso[i].objet;
				if(k) {
					if(objet[k].disp) {
						xzn = (objet[k].x >> 10) - xshake;
						yzn = (objet[k].y >> 10) - yshake;
						if(objet[k].cote > 0) {
							j = obje[objet[k].imag].dx;
						} else j = obje[objet[k].imag].w - obje[objet[k].imag].dx;
						ZoomTransBlit(db, xzn - j, yzn - obje[objet[k].imag].dy, obje[objet[k].imag].w, obje[objet[k].imag].h, (unsigned short *)obje[objet[k].imag].image, COUL_TRANS, objet[k].cote);
						objet[k].disp = false;
					}
				}

				if(perso[i].cote > 0) {
					j = pers[perso[i].imag].dx;
				} else j = pers[perso[i].imag].w - pers[perso[i].imag].dx;
				if( (perso[i].invinc >> 2) * 2 == (perso[i].invinc >> 1) ) { //Pas clignotant
					ZoomTransBlit(db, xz - j, yz - pers[perso[i].imag].dy, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS, perso[i].cote);
				} else ZoomSolidBlit(db, xz - j, yz - pers[perso[i].imag].dy, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS, coul_jaunec, perso[i].cote);
				if(perso[i].protect) {
					k = explos_protectr + (perso[i].shield >> 10) + perso[i].coul * nb_explos_protect;
					ZoomTransBlit(db, xz - expl__[k].dx, yz - expl__[k].dy, expl__[k].w, expl__[k].h, (unsigned short *)expl__[k].image, COUL_TRANS, 1);
				}

				if(afficheid) {
					yz -= (pers[perso[i].imag].h + 2);
					j = getafficheid(perso[i].coul);
					ZoomTransBlit(db, xz - expl__[j].dx, yz - expl__[j].dy, expl__[j].w, expl__[j].h, (unsigned short *)expl__[j].image, COUL_TRANS, 1);
				}
			}
		}
	}

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_TRUE); //Y'a re-swizzle :)

	yzn = -divzoom(dyscreen); //Foreground
	for(j = 0; j < disp_height; j++) {
		xzn = -divzoom(dxscreen);
		yz = yzn;
		yzn = divzoom( (j + 1) * 16 - dyscreen);

		for(i = 0; i < disp_width; i++) {
			xz = xzn;
			xzn = divzoom( (i + 1) * 16 - dxscreen);

			k = tabl[xmshake + i][ymshake + j];
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

	sceGuTexMode(GU_PSM_5551, 0, 0, 0); //Pas de swizzle pour les sprites :)

	for(i = 1; i <= nb_objets; i++) { //Objets
		if( (objet[i].ener > 0) && !objet[i].porte) {
			if( (objet[i].time > 100) || ( (objet[i].time >> 2) * 2 == (objet[i].time >> 1) ) ) {
				xz = (objet[i].x >> 10) - xshake;
				yz = (objet[i].y >> 10) - yshake;
				if( (xz > -20) && (xz < scrwb) && (yz > -20) && (yz < scrhb) ) {
					if(objet[i].cote > 0) {
						j = obje[objet[i].imag].dx;
					} else j = obje[objet[i].imag].w - obje[objet[i].imag].dx;
					ZoomTransBlit(db, xz - j, yz - obje[objet[i].imag].dy, obje[objet[i].imag].w, obje[objet[i].imag].h, (unsigned short *)obje[objet[i].imag].image, COUL_TRANS, objet[i].cote);
				}
			}
		}
	}

	for(i = 0; i < nb_projects; i++) { //Projectiles
		if(project[i].port) {
			xz = (project[i].x >> 10) - xshake;
			yz = (project[i].y >> 10) - yshake;
			if( (xz > -20) && (xz < scrwb) && (yz > -20) && (yz < scrhb) ) {
				if(project[i].cote > 0) {
					j = pers[project[i].imag].dx;
				} else j = pers[project[i].imag].w - pers[project[i].imag].dx;
				ZoomTransBlit(db, xz - j, yz - pers[project[i].imag].dy, pers[project[i].imag].w, pers[project[i].imag].h, (unsigned short *)pers[project[i].imag].image, COUL_TRANS, project[i].cote);
			}
		}
	}

	for(i = 0; i < nb_persos; i++) {
		if(perso[i].enjeu) {
			xz = (perso[i].x >> 10) - xshake + i * 2;
			yz = (perso[i].y >> 10) - yshake + i * 2;
			if( (xz < 0) || (xz > scrw) || (yz < 0) || (yz > (scrh + pers[move[defperso[perso[i].id].idcoups].idim].h) ) ) { //Hors du jeu, dans une bulle
				a = (pers[move[defperso[perso[i].id].idcoups].idim].w * pers[move[defperso[perso[i].id].idcoups].idim].h) >> 10;
				yz -= (pers[perso[i].imag].h >> 1);

				if(xz < (50 + i * 2) ) {
					xz = 50 + i * 2;
				} else {
					if(xz > (scrw - 50 + i * 2) ) xz = scrw - 50 + i * 2;
				}
				if(yz < (50 + i * 2) ) {
					yz = 50 + i * 2;
				} else {
					if(yz > (scrh - 50 + i * 2) ) yz = scrh - 50 + i * 2;
				}
				k = explos_rondr + a + perso[i].coul * nb_explos_rond;
				ZoomTransBlit(db, xz - expl__[k].dx, yz - expl__[k].dy, expl__[k].w, expl__[k].h, (unsigned short *)expl__[k].image, COUL_TRANS, 1);

				xzn = xz - (scrw >> 1); //Direction flèche
				yzn = (scrh >> 1) - yz;
				if(xzn > 0) {
					b = 1;
				} else b = -1;
				l = 0;
				j = 1000;
				if(yzn) j = (xzn * b * 100) / yzn; //Angle
				if( (j > 20) && (j < 61) ) l = 1;
				if( (j > 60) && (j < 141) ) l = 2;
				if( (j > 140) && (j < 401) ) l = 3;
				if( (j > 400) || (j < -400) ) l = 4;
				if( (j < -140) && (j > -401) ) l = 5;
				if( (j < -60) && (j > -141) ) l = 6;
				if( (j < 0) && (j > -61) ) l = 7;
				if( (j == 0) && (yzn < 0) ) l = 7;
				k = explos_fleche + l;
				if(b > 0) {
					j = tabfleche[l][a][0];
				} else j = expl__[k].w - tabfleche[l][a][0];
				ZoomTransBlit(db, xz - j, yz - tabfleche[l][a][1], expl__[k].w, expl__[k].h, (unsigned short *)expl__[k].image, COUL_TRANS, b);

				yz += (pers[perso[i].imag].h >> 1);

				k = perso[i].objet;
				if(k) {
					if(objet[k].disp) {
						xzn = xz + move[defperso[perso[i].id].idcoups].dx; //(objet[k].x >> 10) - xshake;
						yzn = yz - move[defperso[perso[i].id].idcoups].dy; //(objet[k].y >> 10) - yshake;
						if(objet[k].cote > 0) {
							j = obje[objet[k].imag].dx;
						} else j = obje[objet[k].imag].w - obje[objet[k].imag].dx;
						ZoomTransBlit(db, xzn - j, yzn - obje[objet[k].imag].dy, obje[objet[k].imag].w, obje[objet[k].imag].h, (unsigned short *)obje[objet[k].imag].image, COUL_TRANS, objet[k].cote);
						objet[k].disp = false;
					}
				}

				if(perso[i].cote > 0) {
					j = pers[perso[i].imag].dx;
				} else j = pers[perso[i].imag].w - pers[perso[i].imag].dx;
				if( (perso[i].invinc >> 2) * 2 == (perso[i].invinc >> 1) ) { //Pas clignotant
					ZoomTransBlit(db, xz - j, yz - pers[perso[i].imag].h + 1, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS, perso[i].cote);
				} else ZoomSolidBlit(db, xz - j, yz - pers[perso[i].imag].h + 1, pers[perso[i].imag].w, pers[perso[i].imag].h, (unsigned short *)pers[perso[i].imag].image, COUL_TRANS, coul_jaunec, perso[i].cote);
				if(perso[i].protect) {
					k = explos_protectr + (perso[i].shield >> 10) + perso[i].coul * nb_explos_protect;
					ZoomTransBlit(db, xz - expl__[k].dx, yz - expl__[k].dy, expl__[k].w, expl__[k].h, (unsigned short *)expl__[k].image, COUL_TRANS, 1);
				}
			}
		}
	}

	for(i = 0; i < nb_explos; i++) { //Explosions
		if(explos[i].imag) {
			if(explos[i].zoom) {
				xz = explos[i].x - xshake;
				yz = explos[i].y - yshake;
				if( (xz > -20) && (xz < scrwb) && (yz > -20) && (yz < scrhb) ) {
					j = explos[i].id + explos[i].imag;
					ZoomTransBlit(db, xz - expl__[j].dx, yz - expl__[j].dy, expl__[j].w, expl__[j].h, (unsigned short *)expl__[j].image, COUL_TRANS, 1);
				}
			} else {
				j = explos[i].id + explos[i].imag;
				FastTransBlit(db, explos[i].x - expl__[j].dx, explos[i].y - expl__[j].dy, expl__[j].w, expl__[j].h, (unsigned short *)expl__[j].image, COUL_TRANS);
			}
		}
	}
//	sceKernelDcacheWritebackAll();
	sceKernelDcacheWritebackInvalidateAll();

	sceGuFinish();

	sceGuSync(0, 0); //Synchro
	edram_ptr = (unsigned short *)textbuf_[0];

	k = 30;
	for(i = 0; i < nb_joueurs; i++) {
		a = k; //Vies
		l = 240;
		for(j = 0; j < joueur[i].vies; j++) {
			FastTransBlit(db, a, l, 16, 16, (unsigned short *)bmps[defperso[perso[i].id].idicone].image, COUL_TRANS);
			if(j == 2) {
				l += 16;
				a = k;
			} else a += 16;
		}

		sprintf(g_string, "%d%%", perso[i].damage >> 7); //%
		StringGameTransBlit(db, k, 248, g_string);

		if(joueur[i].anim) { //Anims +/- 1
			j = joueur[i].anim + joueur[i].stata;
			FastTransBlit(db, k + 24 - expl__[j].dx, 252 - expl__[j].dy, expl__[j].w, expl__[j].h, (unsigned short *)expl__[j].image, COUL_TRANS);
			if(timer3 == 0) {
				joueur[i].stata++;
				if(joueur[i].stata >= nb_explos_pm1) joueur[i].anim = 0;
			}
		}
		k += 120;
	}

	if( (type_jeu == type_temps) && !mort_subite) { //Affichage temps restant
		i = cond_timer / 60;
		if(i > 5) {
			j = i / 60;
			i -= j * 60;
			sprintf(g_string, "%d:%02d", j, i);
			StringTimeTransBlit(db, 478, 2, g_string);
		} else { //Compte à rebours
			if(i > 0) {
				if(cptreb[i - 1] ) { //Voix compte à rebours
					PlaySound(&sound[son_0_ + i]);

					cptreb[i - 1] = false;
				}
				j = explos_cr0 + i;
				FastTransBlit(db, 240 - expl__[j].dx, 136 - expl__[j].dy, expl__[j].w, expl__[j].h, (unsigned short *)expl__[j].image, COUL_TRANS);
			}
		}
	}

	if(pause) {
		hline(db, 17, 8, 447, coul_blanc);
		hline(db, 17, 9, 447, coul_noir);
		hline(db, 17, 263, 447, coul_blanc);
		hline(db, 17, 264, 447, coul_noir);
		vline(db, 8, 17, 239, coul_blanc);
		vline(db, 9, 17, 239, coul_noir);
		vline(db, 471, 17, 239, coul_blanc);
		vline(db, 472, 17, 239, coul_noir);
		FastTransBlit(db, 8, 8, expl__[explos_cadrehg].w, expl__[explos_cadrehg].h, (unsigned short *)expl__[explos_cadrehg].image, COUL_TRANS);
		FastTransBlit(db, 464, 8, expl__[explos_cadrehd].w, expl__[explos_cadrehd].h, (unsigned short *)expl__[explos_cadrehd].image, COUL_TRANS);
		FastTransBlit(db, 8, 256, expl__[explos_cadrebg].w, expl__[explos_cadrebg].h, (unsigned short *)expl__[explos_cadrebg].image, COUL_TRANS);
		FastTransBlit(db, 464, 256, expl__[explos_cadrebd].w, expl__[explos_cadrebd].h, (unsigned short *)expl__[explos_cadrebd].image, COUL_TRANS);

		FastTransBlit(db, 20, 2, expl__[explos_btna].w, expl__[explos_btna].h, (unsigned short *)expl__[explos_btna].image, COUL_TRANS);
		StringTransBlit(db, 35, 3, "Zoom In");
		FastTransBlit(db, 83, 2, expl__[explos_btnb].w, expl__[explos_btnb].h, (unsigned short *)expl__[explos_btnb].image, COUL_TRANS);
		StringTransBlit(db, 98, 3, "Zoom Out");
		FastTransBlit(db, 150, 0, expl__[explos_btndir].w, expl__[explos_btndir].h, (unsigned short *)expl__[explos_btndir].image, COUL_TRANS);
		StringTransBlit(db, 169, 3, "Move");
		FastTransBlit(db, 199, 2, expl__[explos_btnsel].w, expl__[explos_btnsel].h, (unsigned short *)expl__[explos_btnsel].image, COUL_TRANS);
		StringTransBlit(db, 213, 3, "Sel+Start: Exit");
	}

	if(mort_subite) {
		j = false;

		for(i = 0; i < nb_joueurs; i++) {
			if(joueur[i].vies && !perso[i].enjeu) {
				j = true;
				break;
			}
		}

		if(j) FastTransBlit(db, 80, 136, expl__[explos_sd].w, expl__[explos_sd].h, (unsigned short *)expl__[explos_sd].image, COUL_TRANS);
	}

#ifdef	__debug__
sprintf(g_string, "%d %d %d %d - %d %d %d %d", ia[1].phase, ia[1].cible, ia[1].air, perso[1].acti, ia[2].phase, ia[2].cible, ia[2].air, perso[2].acti);
StringTransBlit(db, 10, 10, g_string);
#endif
}
