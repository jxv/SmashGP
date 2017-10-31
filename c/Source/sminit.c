void assigne_images() {
//	long listlen = 0;
//	GPDIRENTRY list[256];
	SceIoDirent ename;
	int dhandle = -1;
//	long rcount = 0;
	int i;

	bmps[0].image = (unsigned short *)dumimage;
	strcpy(bmps[0].name, "");

	for(i = 0; i < max_joueurs; i++) perso[i].id = -1;
	joueur[0].cpu = false;
	joueur[1].cpu = true;
	joueur[2].cpu = true;
	joueur[3].cpu = true;

	if(loadpak("menu.pak") > 0) {
		strcat(psp_path, "palette/");

//		loadpal("splash.act", false);

		image_splash = bmps[loadbmp("splash.bmp") ].image;
//		convbmp(image_splash, 320, 240);

//		loadpal("menus.act", false);

		image_main = bmps[loadbmp("mainmenu.bmp") ].image;
//		convbmp(image_main, 320, 240);
		btn_1player = bmps[loadbmp("mn1pla.bmp") ].image;
//		convbmp(btn_1player, 122, 26);
		btn_options = bmps[loadbmp("mnopti.bmp") ].image;
//		convbmp(btn_options, 118, 26);
		btn_reset = bmps[loadbmp("mnrese.bmp") ].image;
//		convbmp(btn_reset, 91, 23);
		image_mode = bmps[loadbmp("modemenu.bmp") ].image;
//		convbmp(image_mode, 320, 240);
		btn_time = bmps[loadbmp("mdtime.bmp") ].image;
//		convbmp(btn_time, 192, 88);
		btn_lives = bmps[loadbmp("mdlive.bmp") ].image;
//		convbmp(btn_lives, 187, 82);
		btn_survival = bmps[loadbmp("mdsurv.bmp") ].image;
//		convbmp(btn_survival, 242, 86);

		loadpal("pal00.act", true);

		image_char = bmps[loadbmp("chsele.bmp") ].image;
//		convbmp(image_char, 320, 240);
		image_area = bmps[loadbmp("arsele.bmp") ].image;
//		convbmp(image_area, 320, 240);
		image_autre = bmps[loadbmp("optimenu.bmp") ].image;
//		convbmp(image_autre, 320, 240);
		tit_options = bmps[loadbmp("auopti.bmp") ].image;
//		convbmp(tit_options, 58, 16);
		tit_results = bmps[loadbmp("auresu.bmp") ].image;
//		convbmp(tit_results, 56, 15);
		tit_credits = bmps[loadbmp("aucred.bmp") ].image;
//		convbmp(tit_credits, 56, 15);
		btn_pmtime = bmps[loadbmp("bttime.bmp") ].image;
//		convbmp(btn_pmtime, 73, 12);
		btn_pmstock = bmps[loadbmp("btstoc.bmp") ].image;
//		convbmp(btn_pmstock, 73, 12);
		btn_pmplayers = bmps[loadbmp("btplay.bmp") ].image;
//		convbmp(btn_pmplayers, 73, 12);
		btn_back = bmps[loadbmp("btback.bmp") ].image;
//		convbmp(btn_back, 51, 12);
		btn_jhmn = bmps[loadbmp("btjhmn.bmp") ].image;
//		convbmp(btn_jhmn, 27, 12);
		btn_jcpu = bmps[loadbmp("btjcpu.bmp") ].image;
//		convbmp(btn_jcpu, 27, 12);
		btn_jna = bmps[loadbmp("btjna.bmp") ].image;
//		convbmp(btn_jna, 27, 12);
		btn_left = bmps[loadbmp("btleft.bmp") ].image;
//		convbmp(btn_left, 15, 30);
		btn_right = bmps[loadbmp("btright.bmp") ].image;
//		convbmp(btn_right, 15, 30);
	}

	strcpy(psp_path, game_path);

	if(loadpak("game.pak") > 0) {
		font01 = bmps[loadbmp("fonte01.bmp") ].image;
		convbmp(font01, font01_w, font01_h);
		font02 = bmps[loadbmp("fonte02.bmp") ].image;
		convbmp(font02, font02_width, font02_h);
		font03 = bmps[loadbmp("fonte03.bmp") ].image;
		convbmp(font03, font03_w, font03_h);

		decors = bmps[loadbmp("tiles.bmp") ].image;
		convbmp(decors, decors_width, 16);
//		decors_ = edram_ptr;
		swizzletex(decors, decors_, decors_width, 16);
	//	decorsb = bmps[loadbmp("tiles2.bmp") ].image;
	//	convbmp(decorsb, decors_width, 16);
//		decorsb_ = edram_ptr;
	//	swizzletex(decorsb, decorsb_, decors_width, 16);
	}

	defobjet[objet_batonetoile].poids = 100;
	defobjet[objet_batonetoile].ener = 15000;
	defobjet[objet_batonetoile].atta = 1500;
	defobjet[objet_batonetoile].role = role_batonetoile;
	defobjet[objet_batonetoile].idim = 0;
	defobjet[objet_batonetoile].nrot = 7;
	defobjet[objet_batonetoile].nani = 0;

	defobjet[objet_batte].poids = 100;
	defobjet[objet_batte].ener = 20000;
	defobjet[objet_batte].atta = 1200;
	defobjet[objet_batte].role = role_batte;
	defobjet[objet_batte].idim = 11;
	defobjet[objet_batte].nrot = 7;
	defobjet[objet_batte].nani = 0;

	defobjet[objet_marteau].poids = 200;
	defobjet[objet_marteau].ener = 20000;
	defobjet[objet_marteau].atta = 2000;
	defobjet[objet_marteau].role = role_marteau;
	defobjet[objet_marteau].idim = 22;
	defobjet[objet_marteau].nrot = 7;
	defobjet[objet_marteau].nani = 4;

	defobjet[objet_pill].poids = 50;
	defobjet[objet_pill].ener = 20000;
	defobjet[objet_pill].atta = 1000;
	defobjet[objet_pill].role = role_boite;
	defobjet[objet_pill].idim = 37;
	defobjet[objet_pill].nrot = 7;
	defobjet[objet_pill].nani = 0;

	defobjet[objet_coeur].poids = 10;
	defobjet[objet_coeur].ener = 10000;
	defobjet[objet_coeur].atta = 1;
	defobjet[objet_coeur].role = role_coeur;
	defobjet[objet_coeur].idim = 48;
	defobjet[objet_coeur].nrot = 0;
	defobjet[objet_coeur].nani = 0;

	defobjet[objet_maximt].poids = 10;
	defobjet[objet_maximt].ener = 10000;
	defobjet[objet_maximt].atta = 0;
	defobjet[objet_maximt].role = role_coeur;
	defobjet[objet_maximt].idim = 50;
	defobjet[objet_maximt].nrot = 0;
	defobjet[objet_maximt].nani = 0;

	defobjet[objet_oeuf].poids = 80;
	defobjet[objet_oeuf].ener = 15000;
	defobjet[objet_oeuf].atta = 1200;
	defobjet[objet_oeuf].role = role_boite;
	defobjet[objet_oeuf].idim = 52;
	defobjet[objet_oeuf].nrot = 7;
	defobjet[objet_oeuf].nani = 0;

	defobjet[objet_etoile].poids = 20;
	defobjet[objet_etoile].ener = 10000;
	defobjet[objet_etoile].atta = 0;
	defobjet[objet_etoile].role = role_etoile;
	defobjet[objet_etoile].idim = 63;
	defobjet[objet_etoile].nrot = 1;
	defobjet[objet_etoile].nani = 0;

	defobjet[objet_raygun].poids = 50;
	defobjet[objet_raygun].ener = 15000;
	defobjet[objet_raygun].atta = 800;
	defobjet[objet_raygun].role = role_pistolet;
	defobjet[objet_raygun].idim = 66;
	defobjet[objet_raygun].nrot = 7;
	defobjet[objet_raygun].nani = 0;

	defobjet[objet_scope].poids = 70;
	defobjet[objet_scope].ener = 10000;
	defobjet[objet_scope].atta = 400;
	defobjet[objet_scope].role = role_pistolet;
	defobjet[objet_scope].idim = 77;
	defobjet[objet_scope].nrot = 7;
	defobjet[objet_scope].nani = 0;

	defobjet[objet_carapr].poids = 90;
	defobjet[objet_carapr].ener = 20000;
	defobjet[objet_carapr].atta = 1500;
	defobjet[objet_carapr].role = role_carapr;
	defobjet[objet_carapr].idim = 88;
	defobjet[objet_carapr].nrot = 3;
	defobjet[objet_carapr].nani = 0;

	defobjet[objet_carapv].poids = 80;
	defobjet[objet_carapv].ener = 15000;
	defobjet[objet_carapv].atta = 1000;
	defobjet[objet_carapv].role = role_carapv;
	defobjet[objet_carapv].idim = 94;
	defobjet[objet_carapv].nrot = 3;
	defobjet[objet_carapv].nani = 0;

	defobjet[objet_pillexp].poids = 50;
	defobjet[objet_pillexp].ener = 20000;
	defobjet[objet_pillexp].atta = 2000;
	defobjet[objet_pillexp].role = role_explose;
	defobjet[objet_pillexp].idim = 37;
	defobjet[objet_pillexp].nrot = 7;
	defobjet[objet_pillexp].nani = 0;

	defobjet[objet_oeufexp].poids = 80;
	defobjet[objet_oeufexp].ener = 15000;
	defobjet[objet_oeufexp].atta = 3000;
	defobjet[objet_oeufexp].role = role_explose;
	defobjet[objet_oeufexp].idim = 52;
	defobjet[objet_oeufexp].nrot = 7;
	defobjet[objet_oeufexp].nani = 0;

	defobjet[objet_mine].poids = 40;
	defobjet[objet_mine].ener = 10000;
	defobjet[objet_mine].atta = 4000;
	defobjet[objet_mine].role = role_mine;
	defobjet[objet_mine].idim = 100;
	defobjet[objet_mine].nrot = 0;
	defobjet[objet_mine].nani = 0;

	defobjet[objet_bobomb].poids = 80;
	defobjet[objet_bobomb].ener = 20000;
	defobjet[objet_bobomb].atta = 5000;
	defobjet[objet_bobomb].role = role_bobomb;
	defobjet[objet_bobomb].idim = 104;
	defobjet[objet_bobomb].nrot = 7;
	defobjet[objet_bobomb].nani = 3;
	nb_defobjets = 16;

	defproject[proj_etoile].atta = 1500;
	defproject[proj_etoile].smash = true;
	defproject[proj_etoile].idim = explos_etoile;
	defproject[proj_etoile].nani = 7;
	defproject[proj_etoile].port = 30;

	defproject[proj_scope].atta = 400;
	defproject[proj_scope].smash = false;
	defproject[proj_scope].idim = explos_bulscope;
	defproject[proj_scope].nani = 0;
	defproject[proj_scope].port = 100;

	defproject[proj_raygun].atta = 800;
	defproject[proj_raygun].smash = true;
	defproject[proj_raygun].idim = explos_buraygun;
	defproject[proj_raygun].nani = 0;
	defproject[proj_raygun].port = 200;
	nb_defprojects = 3;

#ifdef	__inc_son__
//	GpRelativePathSet("gp:\\gpmm\\smashgp\\common");
	strcat(psp_path, "common/");

	loadwave("cpasse.wav", &sound[son_cpasse], son_cpasse);
	loadwave("smpasse.wav", &sound[son_smpasse], son_smpasse);
	loadwave("cbloque.wav", &sound[son_cbloque], son_cbloque);
	loadwave("cvent.wav", &sound[son_cvent], son_cvent);
	loadwave("smvent.wav", &sound[son_smvent], son_smvent);
	loadwave("protect.wav", &sound[son_protect], son_protect);
	loadwave("protcass.wav", &sound[son_protcass], son_protcass);
	loadwave("objpris.wav", &sound[son_objpris], son_objpris);
	loadwave("objlance.wav", &sound[son_objlance], son_objlance);
	loadwave("cbatet.wav", &sound[son_cbatet], son_cbatet);
	loadwave("smbatet.wav", &sound[son_smbatet], son_smbatet);
	loadwave("batte.wav", &sound[son_batte], son_batte);
	loadwave("tombe.wav", &sound[son_tombe], son_tombe);
	loadwave("mort.wav", &sound[son_mort], son_mort);
	loadwave("menu.wav", &sound[son_menu], son_menu);
	loadwave("one.wav", &sound[son_1], son_1);
	loadwave("two.wav", &sound[son_2], son_2);
	loadwave("three.wav", &sound[son_3], son_3);
	loadwave("four.wav", &sound[son_4], son_4);
	loadwave("five.wav", &sound[son_5], son_5);
	loadwave("sdeath.wav", &sound[son_sdeath], son_sdeath);
	loadwave("timeup.wav", &sound[son_timeup], son_timeup);
	loadwave("gameset.wav", &sound[son_gameset], son_gameset);
	loadwave("applau01.wav", &sound[son_applau01], son_applau01);
	loadwave("applau02.wav", &sound[son_applau02], son_applau02);
	loadwave("ohhh01.wav", &sound[son_ohhh01], son_ohhh01);
	loadwave("ohhh02.wav", &sound[son_ohhh02], son_ohhh02);
	loadwave("ohhh03.wav", &sound[son_ohhh03], son_ohhh03);
	loadwave("carap.wav", &sound[son_carap], son_carap);
	loadwave("explos.wav", &sound[son_explos], son_explos);
	loadwave("raygun.wav", &sound[son_raygun], son_raygun);
	loadwave("mine.wav", &sound[son_mine], son_mine);

//	GpRelativePathSet("gp:\\gpmm\\smashgp");
	strcpy(psp_path, game_path);
#endif


	_assignation_objes(0, 9, 26, 4, 13, bmps[loadbmp("batetr01.bmp") ].image); //Bâton étoile

	_assignation_objes(1, 20, 19, 10, 9, bmps[loadbmp("batetr02.bmp") ].image);

	_assignation_objes(2, 26, 9, 13, 4, bmps[loadbmp("batetr03.bmp") ].image);

	_assignation_objes(3, 20, 19, 10, 9, bmps[loadbmp("batetr04.bmp") ].image);

	_assignation_objes(4, 9, 26, 4, 13, bmps[loadbmp("batetr05.bmp") ].image);

	_assignation_objes(5, 19, 20, 9, 10, bmps[loadbmp("batetr06.bmp") ].image);

	_assignation_objes(6, 26, 9, 13, 4, bmps[loadbmp("batetr07.bmp") ].image);

	_assignation_objes(7, 19, 20, 9, 10, bmps[loadbmp("batetr08.bmp") ].image);

	_assignation_objes(8, 25, 9, 12, 8, bmps[loadbmp("batets01.bmp") ].image); //sol

	_assignation_objes(9, 20, 19, 10, 9, bmps[loadbmp("batetr02.bmp") ].image); //porté

	_assignation_objes(10, 26, 9, 2, 4, bmps[loadbmp("batetr03.bmp") ].image); //utilisé


	_assignation_objes(11, 7, 33, 3, 16, bmps[loadbmp("batter01.bmp") ].image); //Batte

	_assignation_objes(12, 26, 26, 13, 13, bmps[loadbmp("batter02.bmp") ].image);

	_assignation_objes(13, 33, 7, 16, 3, bmps[loadbmp("batter03.bmp") ].image);

	_assignation_objes(14, 26, 26, 13, 13, bmps[loadbmp("batter04.bmp") ].image);

	_assignation_objes(15, 7, 33, 3, 16, bmps[loadbmp("batter05.bmp") ].image);

	_assignation_objes(16, 26, 26, 13, 13, bmps[loadbmp("batter06.bmp") ].image);

	_assignation_objes(17, 33, 7, 16, 3, bmps[loadbmp("batter07.bmp") ].image);

	_assignation_objes(18, 26, 26, 13, 13, bmps[loadbmp("batter08.bmp") ].image);

	_assignation_objes(19, 33, 7, 16, 6, bmps[loadbmp("battes01.bmp") ].image); //sol

	_assignation_objes(20, 26, 26, 13, 13, bmps[loadbmp("batter02.bmp") ].image); //portée

	_assignation_objes(21, 33, 7, 5, 3, bmps[loadbmp("batter03.bmp") ].image); //utilisée


	_assignation_objes(22, 14, 30, 7, 15, bmps[loadbmp("martor01.bmp") ].image); //Marteau

	_assignation_objes(23, 24, 25, 12, 12, bmps[loadbmp("martor02.bmp") ].image);

	_assignation_objes(24, 30, 14, 15, 7, bmps[loadbmp("martor03.bmp") ].image);

	_assignation_objes(25, 25, 24, 12, 12, bmps[loadbmp("martor04.bmp") ].image);

	_assignation_objes(26, 14, 30, 7, 15, bmps[loadbmp("martor05.bmp") ].image);

	_assignation_objes(27, 25, 24, 12, 12, bmps[loadbmp("martor06.bmp") ].image);

	_assignation_objes(28, 30, 14, 15, 7, bmps[loadbmp("martor07.bmp") ].image);

	_assignation_objes(29, 24, 25, 12, 12, bmps[loadbmp("martor08.bmp") ].image);

	_assignation_objes(30, 29, 13, 14, 12, bmps[loadbmp("martos01.bmp") ].image); //sol

	_assignation_objes(31, 24, 25, 12, 12, bmps[loadbmp("martor02.bmp") ].image); //porté

	_assignation_objes(32, 30, 14, 24, 6, bmps[loadbmp("martor07.bmp") ].image); //utilisé

	_assignation_objes(33, 24, 25, 18, 19, bmps[loadbmp("martor08.bmp") ].image);

	_assignation_objes(34, 14, 30, 6, 24, bmps[loadbmp("martor01.bmp") ].image);

	_assignation_objes(35, 24, 25, 5, 19, bmps[loadbmp("martor02.bmp") ].image);

	_assignation_objes(36, 30, 14, 5, 6, bmps[loadbmp("martor03.bmp") ].image);


	_assignation_objes(37, 7, 18, 3, 9, bmps[loadbmp("pillr01.bmp") ].image); //Pillule

	_assignation_objes(38, 15, 15, 7, 7, bmps[loadbmp("pillr02.bmp") ].image);

	_assignation_objes(39, 18, 7, 9, 3, bmps[loadbmp("pillr03.bmp") ].image);

	_assignation_objes(40, 15, 15, 7, 7, bmps[loadbmp("pillr04.bmp") ].image);

	_assignation_objes(41, 7, 18, 3, 9, bmps[loadbmp("pillr05.bmp") ].image);

	_assignation_objes(42, 15, 15, 7, 7, bmps[loadbmp("pillr06.bmp") ].image);

	_assignation_objes(43, 18, 7, 9, 3, bmps[loadbmp("pillr07.bmp") ].image);

	_assignation_objes(44, 15, 15, 7, 7, bmps[loadbmp("pillr08.bmp") ].image);

	_assignation_objes(45, 18, 7, 9, 6, bmps[loadbmp("pillr03.bmp") ].image); //sol

	_assignation_objes(46, 15, 15, 7, 7, bmps[loadbmp("pillr02.bmp") ].image); //portée

	_assignation_objes(47, 18, 7, 9, 3, bmps[loadbmp("pillr03.bmp") ].image); //utilisée


	_assignation_objes(48, 15, 13, 7, 12, bmps[loadbmp("heart.bmp") ].image); //Coeur

	_assignation_objes(49, 15, 13, 7, 12, bmps[loadbmp("heart.bmp") ].image); //sol


	_assignation_objes(50, 15, 13, 7, 12, bmps[loadbmp("maximt.bmp") ].image); //Maxim tomato

	_assignation_objes(51, 15, 13, 7, 12, bmps[loadbmp("maximt.bmp") ].image); //sol


	_assignation_objes(52, 14, 15, 7, 7, bmps[loadbmp("eggr01.bmp") ].image); //Oeuf

	_assignation_objes(53, 14, 14, 7, 7, bmps[loadbmp("eggr02.bmp") ].image);

	_assignation_objes(54, 15, 14, 7, 7, bmps[loadbmp("eggr03.bmp") ].image);

	_assignation_objes(55, 14, 14, 7, 7, bmps[loadbmp("eggr04.bmp") ].image);

	_assignation_objes(56, 14, 15, 7, 7, bmps[loadbmp("eggr05.bmp") ].image);

	_assignation_objes(57, 14, 14, 7, 7, bmps[loadbmp("eggr06.bmp") ].image);

	_assignation_objes(58, 15, 14, 7, 7, bmps[loadbmp("eggr07.bmp") ].image);

	_assignation_objes(59, 14, 14, 7, 7, bmps[loadbmp("eggr08.bmp") ].image);

	_assignation_objes(60, 14, 15, 7, 14, bmps[loadbmp("eggr01.bmp") ].image); //sol

	_assignation_objes(61, 14, 14, 7, 7, bmps[loadbmp("eggr02.bmp") ].image); //porté

	_assignation_objes(62, 15, 14, 7, 7, bmps[loadbmp("eggr03.bmp") ].image); //utilisé


	_assignation_objes(63, 17, 17, 8, 8, bmps[loadbmp("starman1.bmp") ].image); //Starman

	_assignation_objes(64, 17, 17, 8, 8, bmps[loadbmp("starman2.bmp") ].image);

	_assignation_objes(65, 17, 17, 8, 8, bmps[loadbmp("starman1.bmp") ].image); //sol


	_assignation_objes(66, 11, 15, 5, 7, bmps[loadbmp("raygur01.bmp") ].image); //Raygun

	_assignation_objes(67, 13, 16, 6, 8, bmps[loadbmp("raygur02.bmp") ].image);

	_assignation_objes(68, 15, 11, 7, 5, bmps[loadbmp("raygur03.bmp") ].image);

	_assignation_objes(69, 16, 13, 8, 6, bmps[loadbmp("raygur04.bmp") ].image);

	_assignation_objes(70, 11, 15, 5, 7, bmps[loadbmp("raygur05.bmp") ].image);

	_assignation_objes(71, 13, 16, 6, 8, bmps[loadbmp("raygur06.bmp") ].image);

	_assignation_objes(72, 15, 11, 7, 5, bmps[loadbmp("raygur07.bmp") ].image);

	_assignation_objes(73, 16, 13, 8, 6, bmps[loadbmp("raygur08.bmp") ].image);

	_assignation_objes(74, 16, 11, 8, 10, bmps[loadbmp("raygus01.bmp") ].image); //sol

	_assignation_objes(75, 13, 16, 2, 8, bmps[loadbmp("raygur02.bmp") ].image); //porté

	_assignation_objes(76, 15, 11, 3, 8, bmps[loadbmp("raygur03.bmp") ].image); //utilisé


	_assignation_objes(77, 12, 29, 6, 14, bmps[loadbmp("scoper01.bmp") ].image); //Scope

	_assignation_objes(78, 21, 24, 10, 12, bmps[loadbmp("scoper02.bmp") ].image);

	_assignation_objes(79, 29, 12, 14, 6, bmps[loadbmp("scoper03.bmp") ].image);

	_assignation_objes(80, 24, 21, 12, 10, bmps[loadbmp("scoper04.bmp") ].image);

	_assignation_objes(81, 12, 29, 6, 14, bmps[loadbmp("scoper05.bmp") ].image);

	_assignation_objes(82, 21, 24, 10, 12, bmps[loadbmp("scoper06.bmp") ].image);

	_assignation_objes(83, 29, 12, 14, 6, bmps[loadbmp("scoper07.bmp") ].image);

	_assignation_objes(84, 24, 21, 12, 10, bmps[loadbmp("scoper08.bmp") ].image);

	_assignation_objes(85, 29, 10, 14, 9, bmps[loadbmp("scopes01.bmp") ].image); //sol

	_assignation_objes(86, 21, 24, 10, 12, bmps[loadbmp("scoper02.bmp") ].image); //porté

	_assignation_objes(87, 29, 12, 14, 6, bmps[loadbmp("scoper03.bmp") ].image); //utilisé


	_assignation_objes(88, 17, 16, 8, 14, bmps[loadbmp("carapr01.bmp") ].image); //Carapace rouge

	_assignation_objes(89, 17, 16, 8, 14, bmps[loadbmp("carapr02.bmp") ].image);

	_assignation_objes(90, 17, 16, 8, 14, bmps[loadbmp("carapr03.bmp") ].image);

	_assignation_objes(91, 17, 16, 8, 14, bmps[loadbmp("carapr04.bmp") ].image);

	_assignation_objes(92, 17, 16, 8, 15, bmps[loadbmp("carapr05.bmp") ].image); //sol

	_assignation_objes(93, 17, 16, 8, 15, bmps[loadbmp("carapr05.bmp") ].image); //portée


	_assignation_objes(94, 17, 16, 8, 14, bmps[loadbmp("carapv01.bmp") ].image); //Carapace verte

	_assignation_objes(95, 17, 16, 8, 14, bmps[loadbmp("carapv02.bmp") ].image);

	_assignation_objes(96, 17, 16, 8, 14, bmps[loadbmp("carapv03.bmp") ].image);

	_assignation_objes(97, 17, 16, 8, 14, bmps[loadbmp("carapv04.bmp") ].image);

	_assignation_objes(98, 17, 16, 8, 15, bmps[loadbmp("carapv05.bmp") ].image); //sol

	_assignation_objes(99, 17, 16, 8, 15, bmps[loadbmp("carapv05.bmp") ].image); //portée


	_assignation_objes(100, 12, 11, 6, 5, bmps[loadbmp("miner01.bmp") ].image); //Mine proximité

	_assignation_objes(101, 12, 11, 6, 10, bmps[loadbmp("miner01.bmp") ].image); //sol

	_assignation_objes(102, 12, 11, 6, 5, bmps[loadbmp("miner01.bmp") ].image); //portée

	_assignation_objes(103, 12, 3, 6, 2, bmps[loadbmp("mines01.bmp") ].image); //utilisée


	_assignation_objes(104, 11, 14, 5, 8, bmps[loadbmp("bobomr01.bmp") ].image); //Bob-omb

	_assignation_objes(105, 11, 14, 5, 7, bmps[loadbmp("bobomr02.bmp") ].image);

	_assignation_objes(106, 14, 11, 6, 5, bmps[loadbmp("bobomr03.bmp") ].image);

	_assignation_objes(107, 14, 11, 7, 5, bmps[loadbmp("bobomr04.bmp") ].image);

	_assignation_objes(108, 11, 14, 5, 6, bmps[loadbmp("bobomr05.bmp") ].image);

	_assignation_objes(109, 11, 14, 5, 7, bmps[loadbmp("bobomr06.bmp") ].image);

	_assignation_objes(110, 14, 11, 8, 5, bmps[loadbmp("bobomr07.bmp") ].image);

	_assignation_objes(111, 14, 11, 7, 5, bmps[loadbmp("bobomr08.bmp") ].image);

	_assignation_objes(112, 11, 14, 5, 13, bmps[loadbmp("bobomr01.bmp") ].image); //sol

	_assignation_objes(113, 11, 14, 5, 8, bmps[loadbmp("bobomr01.bmp") ].image); //portée

	_assignation_objes(114, 10, 15, 5, 14, bmps[loadbmp("boboma01.bmp") ].image); //action

	_assignation_objes(115, 12, 15, 6, 14, bmps[loadbmp("boboma02.bmp") ].image);

	_assignation_objes(116, 10, 15, 5, 14, bmps[loadbmp("boboma01.bmp") ].image);

	_assignation_objes(117, 10, 14, 5, 13, bmps[loadbmp("boboma03.bmp") ].image);

	_assignation_objes(118, 13, 15, 6, 14, bmps[loadbmp("boboma04.bmp") ].image);

	_assignation_objes(119, 13, 15, 6, 14, bmps[loadbmp("boboma05.bmp") ].image);


	_assignation_explosions(explos_protectr, 20, 20, 10, 19, bmps[loadbmp("protr07.bmp") ].image); //Protections

	_assignation_explosions(explos_protectr + 1, 30, 30, 15, 29, bmps[loadbmp("protr06.bmp") ].image);

	_assignation_explosions(explos_protectr + 2, 40, 40, 20, 39, bmps[loadbmp("protr05.bmp") ].image);

	_assignation_explosions(explos_protectr + 3, 50, 50, 25, 49, bmps[loadbmp("protr04.bmp") ].image);

	_assignation_explosions(explos_protectr + 4, 60, 60, 30, 59, bmps[loadbmp("protr03.bmp") ].image);

	_assignation_explosions(explos_protectr + 5, 70, 70, 35, 69, bmps[loadbmp("protr02.bmp") ].image);

	_assignation_explosions(explos_protectr + 6, 80, 80, 40, 79, bmps[loadbmp("protr01.bmp") ].image);

	_assignation_explosions(explos_protectv, 20, 20, 10, 19, bmps[loadbmp("protv07.bmp") ].image);

	_assignation_explosions(explos_protectv + 1, 30, 30, 15, 29, bmps[loadbmp("protv06.bmp") ].image);

	_assignation_explosions(explos_protectv + 2, 40, 40, 20, 39, bmps[loadbmp("protv05.bmp") ].image);

	_assignation_explosions(explos_protectv + 3, 50, 50, 25, 49, bmps[loadbmp("protv04.bmp") ].image);

	_assignation_explosions(explos_protectv + 4, 60, 60, 30, 59, bmps[loadbmp("protv03.bmp") ].image);

	_assignation_explosions(explos_protectv + 5, 70, 70, 35, 69, bmps[loadbmp("protv02.bmp") ].image);

	_assignation_explosions(explos_protectv + 6, 80, 80, 40, 79, bmps[loadbmp("protv01.bmp") ].image);

	_assignation_explosions(explos_protectb, 20, 20, 10, 19, bmps[loadbmp("protb07.bmp") ].image);

	_assignation_explosions(explos_protectb + 1, 30, 30, 15, 29, bmps[loadbmp("protb06.bmp") ].image);

	_assignation_explosions(explos_protectb + 2, 40, 40, 20, 39, bmps[loadbmp("protb05.bmp") ].image);

	_assignation_explosions(explos_protectb + 3, 50, 50, 25, 49, bmps[loadbmp("protb04.bmp") ].image);

	_assignation_explosions(explos_protectb + 4, 60, 60, 30, 59, bmps[loadbmp("protb03.bmp") ].image);

	_assignation_explosions(explos_protectb + 5, 70, 70, 35, 69, bmps[loadbmp("protb02.bmp") ].image);

	_assignation_explosions(explos_protectb + 6, 80, 80, 40, 79, bmps[loadbmp("protb01.bmp") ].image);

	_assignation_explosions(explos_protectj, 20, 20, 10, 19, bmps[loadbmp("protj07.bmp") ].image);

	_assignation_explosions(explos_protectj + 1, 30, 30, 15, 29, bmps[loadbmp("protj06.bmp") ].image);

	_assignation_explosions(explos_protectj + 2, 40, 40, 20, 39, bmps[loadbmp("protj05.bmp") ].image);

	_assignation_explosions(explos_protectj + 3, 50, 50, 25, 49, bmps[loadbmp("protj04.bmp") ].image);

	_assignation_explosions(explos_protectj + 4, 60, 60, 30, 59, bmps[loadbmp("protj03.bmp") ].image);

	_assignation_explosions(explos_protectj + 5, 70, 70, 35, 69, bmps[loadbmp("protj02.bmp") ].image);

	_assignation_explosions(explos_protectj + 6, 80, 80, 40, 79, bmps[loadbmp("protj01.bmp") ].image);


	_assignation_explosions(explos_rondr, 40, 40, 20, 20, bmps[loadbmp("rondr01.bmp") ].image); //Cadres - flèches

	_assignation_explosions(explos_rondr + 1, 60, 60, 30, 30, bmps[loadbmp("rondr02.bmp") ].image);

	_assignation_explosions(explos_rondr + 2, 80, 80, 40, 40, bmps[loadbmp("rondr03.bmp") ].image);

	_assignation_explosions(explos_rondv, 40, 40, 20, 20, bmps[loadbmp("rondv01.bmp") ].image);

	_assignation_explosions(explos_rondv + 1, 60, 60, 30, 30, bmps[loadbmp("rondv02.bmp") ].image);

	_assignation_explosions(explos_rondv + 2, 80, 80, 40, 40, bmps[loadbmp("rondv03.bmp") ].image);

	_assignation_explosions(explos_rondb, 40, 40, 20, 20, bmps[loadbmp("rondb01.bmp") ].image);

	_assignation_explosions(explos_rondb + 1, 60, 60, 30, 30, bmps[loadbmp("rondb02.bmp") ].image);

	_assignation_explosions(explos_rondb + 2, 80, 80, 40, 40, bmps[loadbmp("rondb03.bmp") ].image);

	_assignation_explosions(explos_rondj, 40, 40, 20, 20, bmps[loadbmp("rondj01.bmp") ].image);

	_assignation_explosions(explos_rondj + 1, 60, 60, 30, 30, bmps[loadbmp("rondj02.bmp") ].image);

	_assignation_explosions(explos_rondj + 2, 80, 80, 40, 40, bmps[loadbmp("rondj03.bmp") ].image);

	_assignation_explosions(explos_fleche, 34, 34, 0, 0, bmps[loadbmp("flech01.bmp") ].image);

	_assignation_explosions(explos_fleche + 1, 30, 37, 0, 0, bmps[loadbmp("flech02.bmp") ].image);

	_assignation_explosions(explos_fleche + 2, 35, 36, 0, 0, bmps[loadbmp("flech03.bmp") ].image);

	_assignation_explosions(explos_fleche + 3, 37, 31, 0, 0, bmps[loadbmp("flech04.bmp") ].image);

	_assignation_explosions(explos_fleche + 4, 34, 34, 0, 0, bmps[loadbmp("flech05.bmp") ].image);

	_assignation_explosions(explos_fleche + 5, 37, 31, 0, 0, bmps[loadbmp("flech06.bmp") ].image);

	_assignation_explosions(explos_fleche + 6, 35, 36, 0, 0, bmps[loadbmp("flech07.bmp") ].image);

	_assignation_explosions(explos_fleche + 7, 30, 37, 0, 0, bmps[loadbmp("flech08.bmp") ].image);

	_assignation_explosions(explos_cadrehg, 9, 9, 0, 0, bmps[loadbmp("cadrehg.bmp") ].image);

	_assignation_explosions(explos_cadrehd, 9, 9, 0, 0, bmps[loadbmp("cadrehd.bmp") ].image);

	_assignation_explosions(explos_cadrebg, 9, 9, 0, 0, bmps[loadbmp("cadrebg.bmp") ].image);

	_assignation_explosions(explos_cadrebd, 9, 9, 0, 0, bmps[loadbmp("cadrebd.bmp") ].image);

	_assignation_explosions(explos_btna, 14, 14, 0, 0, bmps[loadbmp("btna.bmp") ].image);

	_assignation_explosions(explos_btnb, 14, 14, 0, 0, bmps[loadbmp("btnb.bmp") ].image);

	_assignation_explosions(explos_btndir, 18, 18, 0, 0, bmps[loadbmp("btndir.bmp") ].image);

	_assignation_explosions(explos_btnsel, 13, 13, 0, 0, bmps[loadbmp("btnsel.bmp") ].image);


	_assignation_explosions(explos_p1, 29, 21, 14, 10, bmps[loadbmp("+1d.bmp") ].image);

	_assignation_explosions(explos_p1 + 1, 29, 21, 14, 10, bmps[loadbmp("+1c.bmp") ].image);

	_assignation_explosions(explos_p1 + 2, 29, 21, 14, 10, bmps[loadbmp("+1b.bmp") ].image);

	_assignation_explosions(explos_p1 + 3, 29, 21, 14, 10, bmps[loadbmp("+1a.bmp") ].image);

	_assignation_explosions(explos_p1 + 4, 29, 21, 14, 10, bmps[loadbmp("+1a.bmp") ].image);

	_assignation_explosions(explos_p1 + 5, 29, 21, 14, 10, bmps[loadbmp("+1a.bmp") ].image);

	_assignation_explosions(explos_p1 + 6, 29, 21, 14, 8, bmps[loadbmp("+1a.bmp") ].image);

	_assignation_explosions(explos_p1 + 7, 29, 21, 14, 4, bmps[loadbmp("+1a.bmp") ].image);

	_assignation_explosions(explos_p1 + 8, 29, 21, 14, -2, bmps[loadbmp("+1a.bmp") ].image);

	_assignation_explosions(explos_p1 + 9, 29, 21, 14, -10, bmps[loadbmp("+1a.bmp") ].image);

	_assignation_explosions(explos_m1, 29, 21, 14, 10, bmps[loadbmp("-1d.bmp") ].image);

	_assignation_explosions(explos_m1 + 1, 29, 21, 14, 10, bmps[loadbmp("-1c.bmp") ].image);

	_assignation_explosions(explos_m1 + 2, 29, 21, 14, 10, bmps[loadbmp("-1b.bmp") ].image);

	_assignation_explosions(explos_m1 + 3, 29, 21, 14, 10, bmps[loadbmp("-1a.bmp") ].image);

	_assignation_explosions(explos_m1 + 4, 29, 21, 14, 10, bmps[loadbmp("-1a.bmp") ].image);

	_assignation_explosions(explos_m1 + 5, 29, 21, 14, 10, bmps[loadbmp("-1a.bmp") ].image);

	_assignation_explosions(explos_m1 + 6, 29, 21, 14, 8, bmps[loadbmp("-1a.bmp") ].image);

	_assignation_explosions(explos_m1 + 7, 29, 21, 14, 4, bmps[loadbmp("-1a.bmp") ].image);

	_assignation_explosions(explos_m1 + 8, 29, 21, 14, -2, bmps[loadbmp("-1a.bmp") ].image);

	_assignation_explosions(explos_m1 + 9, 29, 21, 14, -10, bmps[loadbmp("-1a.bmp") ].image);


	_assignation_explosions(explos_id1p, 16, 13, 7, 12, bmps[loadbmp("1p.bmp") ].image);

	_assignation_explosions(explos_idcpr, 16, 13, 7, 12, bmps[loadbmp("cpr.bmp") ].image);

	_assignation_explosions(explos_idcpv, 16, 13, 7, 12, bmps[loadbmp("cpv.bmp") ].image);

	_assignation_explosions(explos_id2p, 16, 13, 7, 12, bmps[loadbmp("2p.bmp") ].image);

	_assignation_explosions(explos_idcpb, 16, 13, 7, 12, bmps[loadbmp("cpb.bmp") ].image);

	_assignation_explosions(explos_idcpj, 16, 13, 7, 12, bmps[loadbmp("cpj.bmp") ].image);


	_assignation_explosions(explos_ondechoc, 20, 13, 10, 12, bmps[loadbmp("chcsol01.bmp") ].image);

	_assignation_explosions(explos_ondechoc + 1, 30, 11, 15, 10, bmps[loadbmp("chcsol02.bmp") ].image);

	_assignation_explosions(explos_ondechoc + 2, 40, 10, 20, 9, bmps[loadbmp("chcsol03.bmp") ].image);

	_assignation_explosions(explos_ondechoc + 3, 46, 7, 23, 6, bmps[loadbmp("chcsol04.bmp") ].image);


	_assignation_explosions_(explos_maino, 26, 28, 13, 14, bmps[loadbmp("main02.bmp") ].image);

	_assignation_explosions_(explos_mainf, 21, 18, 11, 4, bmps[loadbmp("main01.bmp") ].image);

	_assignation_explosions_(explos_maind, 21, 26, 4, 1, bmps[loadbmp("main03.bmp") ].image);

	_assignation_explosions_(explos_jetonr, 18, 18, 9, 9, bmps[loadbmp("jetr.bmp") ].image);

	_assignation_explosions_(explos_jetonrcp, 18, 18, 9, 9, bmps[loadbmp("jetr2.bmp") ].image);

	_assignation_explosions_(explos_jetonv, 18, 18, 9, 9, bmps[loadbmp("jetv.bmp") ].image);

	_assignation_explosions_(explos_jetonv2p, 18, 18, 9, 9, bmps[loadbmp("jetv2.bmp") ].image);

	_assignation_explosions_(explos_jetonb, 18, 18, 9, 9, bmps[loadbmp("jetb.bmp") ].image);

	_assignation_explosions_(explos_jetonj, 18, 18, 9, 9, bmps[loadbmp("jetj.bmp") ].image);

	_assignation_explosions(explos_rtf, 320, 32, 0, 0, bmps[loadbmp("rtf.bmp") ].image);

	_assignation_explosions(explos_sd, 320, 32, 0, 0, bmps[loadbmp("sd.bmp") ].image);

	_assignation_explosions_(explos_rondclass, 23, 23, 0, 0, bmps[loadbmp("rondclas.bmp") ].image);

	_assignation_explosions(explos_cr1, 46, 86, 23, 43, bmps[loadbmp("cptreb1.bmp") ].image);

	_assignation_explosions(explos_cr2, 60, 86, 30, 43, bmps[loadbmp("cptreb2.bmp") ].image);

	_assignation_explosions(explos_cr3, 61, 87, 30, 43, bmps[loadbmp("cptreb3.bmp") ].image);

	_assignation_explosions(explos_cr4, 63, 85, 31, 44, bmps[loadbmp("cptreb4.bmp") ].image);

	_assignation_explosions(explos_cr5, 61, 86, 30, 43, bmps[loadbmp("cptreb5.bmp") ].image);


	_assignation_explosions(explos_impact1, 19, 22, 10, 10, bmps[loadbmp("impact05.bmp") ].image); //Autres

	_assignation_explosions(explos_impact1 + 1, 9, 9, 4, 4, bmps[loadbmp("impact06.bmp") ].image);

	_assignation_explosions(explos_impact2, 28, 27, 14, 12, bmps[loadbmp("impact03.bmp") ].image);

	_assignation_explosions(explos_impact2 + 1, 11, 12, 5, 5, bmps[loadbmp("impact04.bmp") ].image);

	_assignation_explosions(explos_impact3, 35, 31, 18, 13, bmps[loadbmp("impact01.bmp") ].image);

	_assignation_explosions(explos_impact3 + 1, 17, 14, 8, 6, bmps[loadbmp("impact02.bmp") ].image);


	_assignation_explosions(explos_fumee, 1, 1, 0, 0, bmps[loadbmp("fumee01.bmp") ].image);

	_assignation_explosions(explos_fumee + 1, 3, 3, 1, 2, bmps[loadbmp("fumee02.bmp") ].image);

	_assignation_explosions(explos_fumee + 2, 5, 5, 2, 4, bmps[loadbmp("fumee03.bmp") ].image);

	_assignation_explosions(explos_fumee + 3, 7, 7, 3, 6, bmps[loadbmp("fumee04.bmp") ].image);

	_assignation_explosions(explos_fumee + 4, 9, 9, 4, 8, bmps[loadbmp("fumee05.bmp") ].image);

	_assignation_explosions(explos_fumee + 5, 7, 7, 3, 8, bmps[loadbmp("fumee06.bmp") ].image);

	_assignation_explosions(explos_fumee + 6, 7, 7, 3, 9, bmps[loadbmp("fumee07.bmp") ].image);

	_assignation_explosions(explos_fumee + 7, 7, 7, 3, 10, bmps[loadbmp("fumee08.bmp") ].image);


	_assignation_explosions(explos_mortb1, 40, 20, 20, 19, bmps[loadbmp("expgr01.bmp") ].image);

	_assignation_explosions(explos_mortb1 + 1, 35, 30, 17, 29, bmps[loadbmp("expgr02.bmp") ].image);

	_assignation_explosions(explos_mortb1 + 2, 30, 40, 15, 39, bmps[loadbmp("expgr03.bmp") ].image);

	_assignation_explosions(explos_mortb1 + 3, 25, 50, 12, 49, bmps[loadbmp("expgr04.bmp") ].image);

	_assignation_explosions(explos_mortb1 + 4, 20, 65, 10, 64, bmps[loadbmp("expgr05.bmp") ].image);

	_assignation_explosions(explos_mortb1 + 5, 15, 85, 7, 84, bmps[loadbmp("expgr06.bmp") ].image);

	_assignation_explosions(explos_mortb1 + 6, 10, 115, 5, 114, bmps[loadbmp("expgr07.bmp") ].image);

	_assignation_explosions(explos_mortb2, 40, 20, 20, 19, bmps[loadbmp("expor01.bmp") ].image);

	_assignation_explosions(explos_mortb2 + 1, 35, 30, 17, 29, bmps[loadbmp("expor02.bmp") ].image);

	_assignation_explosions(explos_mortb2 + 2, 30, 40, 15, 39, bmps[loadbmp("expor03.bmp") ].image);

	_assignation_explosions(explos_mortb2 + 3, 25, 50, 12, 49, bmps[loadbmp("expor04.bmp") ].image);

	_assignation_explosions(explos_mortb2 + 4, 20, 65, 10, 64, bmps[loadbmp("expor05.bmp") ].image);

	_assignation_explosions(explos_mortb2 + 5, 15, 85, 7, 84, bmps[loadbmp("expor06.bmp") ].image);

	_assignation_explosions(explos_mortb2 + 6, 10, 115, 5, 114, bmps[loadbmp("expor07.bmp") ].image);

	_assignation_explosions(explos_mortg1, 20, 40, 10, 0, bmps[loadbmp("expbl01.bmp") ].image);

	_assignation_explosions(explos_mortg1 + 1, 30, 35, 15, 0, bmps[loadbmp("expbl02.bmp") ].image);

	_assignation_explosions(explos_mortg1 + 2, 40, 30, 20, 0, bmps[loadbmp("expbl03.bmp") ].image);

	_assignation_explosions(explos_mortg1 + 3, 50, 25, 25, 0, bmps[loadbmp("expbl04.bmp") ].image);

	_assignation_explosions(explos_mortg1 + 4, 65, 20, 32, 0, bmps[loadbmp("expbl05.bmp") ].image);

	_assignation_explosions(explos_mortg1 + 5, 85, 15, 42, 0, bmps[loadbmp("expbl06.bmp") ].image);

	_assignation_explosions(explos_mortg1 + 6, 115, 10, 57, 0, bmps[loadbmp("expbl07.bmp") ].image);

	_assignation_explosions(explos_mortg2, 20, 40, 10, 0, bmps[loadbmp("expve01.bmp") ].image);

	_assignation_explosions(explos_mortg2 + 1, 30, 35, 15, 0, bmps[loadbmp("expve02.bmp") ].image);

	_assignation_explosions(explos_mortg2 + 2, 40, 30, 20, 0, bmps[loadbmp("expve03.bmp") ].image);

	_assignation_explosions(explos_mortg2 + 3, 50, 25, 25, 0, bmps[loadbmp("expve04.bmp") ].image);

	_assignation_explosions(explos_mortg2 + 4, 65, 20, 32, 0, bmps[loadbmp("expve05.bmp") ].image);

	_assignation_explosions(explos_mortg2 + 5, 85, 15, 42, 0, bmps[loadbmp("expve06.bmp") ].image);

	_assignation_explosions(explos_mortg2 + 6, 115, 10, 57, 0, bmps[loadbmp("expve07.bmp") ].image);

	_assignation_explosions(explos_mortd1, 20, 40, 10, 19, bmps[loadbmp("expbl01.bmp") ].image);

	_assignation_explosions(explos_mortd1 + 1, 30, 35, 15, 29, bmps[loadbmp("expbl02.bmp") ].image);

	_assignation_explosions(explos_mortd1 + 2, 40, 30, 20, 39, bmps[loadbmp("expbl03.bmp") ].image);

	_assignation_explosions(explos_mortd1 + 3, 50, 25, 25, 49, bmps[loadbmp("expbl04.bmp") ].image);

	_assignation_explosions(explos_mortd1 + 4, 65, 20, 32, 64, bmps[loadbmp("expbl05.bmp") ].image);

	_assignation_explosions(explos_mortd1 + 5, 85, 15, 42, 84, bmps[loadbmp("expbl06.bmp") ].image);

	_assignation_explosions(explos_mortd1 + 6, 115, 10, 57, 114, bmps[loadbmp("expbl07.bmp") ].image);

	_assignation_explosions(explos_mortd2, 20, 40, 10, 19, bmps[loadbmp("expve01.bmp") ].image);

	_assignation_explosions(explos_mortd2 + 1, 30, 35, 15, 29, bmps[loadbmp("expve02.bmp") ].image);

	_assignation_explosions(explos_mortd2 + 2, 40, 30, 20, 39, bmps[loadbmp("expve03.bmp") ].image);

	_assignation_explosions(explos_mortd2 + 3, 50, 25, 25, 49, bmps[loadbmp("expve04.bmp") ].image);

	_assignation_explosions(explos_mortd2 + 4, 65, 20, 32, 64, bmps[loadbmp("expve05.bmp") ].image);

	_assignation_explosions(explos_mortd2 + 5, 85, 15, 42, 84, bmps[loadbmp("expve06.bmp") ].image);

	_assignation_explosions(explos_mortd2 + 6, 115, 10, 57, 114, bmps[loadbmp("expve07.bmp") ].image);


	_assignation_explosions(explos_fsmash, 16, 12, 15, 11, bmps[loadbmp("fumee10.bmp") ].image);

	_assignation_explosions(explos_fsmash + 1, 26, 18, 28, 20, bmps[loadbmp("fumee11.bmp") ].image);

	_assignation_explosions(explos_fsmash + 2, 29, 21, 34, 26, bmps[loadbmp("fumee12.bmp") ].image);

	_assignation_explosions(explos_fsmash + 3, 19, 16, 46, 33, bmps[loadbmp("fumee13.bmp") ].image);

	_assignation_explosions(explos_fsmash + 4, 13, 12, 53, 38, bmps[loadbmp("fumee14.bmp") ].image);

	_assignation_explosions(explos_fsmash + 5, 6, 7, 60, 43, bmps[loadbmp("fumee15.bmp") ].image);

	_assignation_explosions(explos_fsmash2, 16, 12, 0, 11, bmps[loadbmp("fumee20.bmp") ].image);

	_assignation_explosions(explos_fsmash2 + 1, 26, 18, -3, 20, bmps[loadbmp("fumee21.bmp") ].image);

	_assignation_explosions(explos_fsmash2 + 2, 29, 21, -6, 26, bmps[loadbmp("fumee22.bmp") ].image);

	_assignation_explosions(explos_fsmash2 + 3, 19, 16, -18, 33, bmps[loadbmp("fumee23.bmp") ].image);

	_assignation_explosions(explos_fsmash2 + 4, 13, 12, -27, 38, bmps[loadbmp("fumee24.bmp") ].image);

	_assignation_explosions(explos_fsmash2 + 5, 6, 7, -35, 43, bmps[loadbmp("fumee25.bmp") ].image);

	_assignation_explosions(explos_feject, 20, 15, 10, 7, bmps[loadbmp("fumee30.bmp") ].image);

	_assignation_explosions(explos_feject + 1, 34, 26, 17, 13, bmps[loadbmp("fumee31.bmp") ].image);

	_assignation_explosions(explos_feject + 2, 25, 20, 12, 10, bmps[loadbmp("fumee32.bmp") ].image);

	_assignation_explosions(explos_feject + 3, 26, 23, 13, 11, bmps[loadbmp("fumee33.bmp") ].image);

	_assignation_explosions(explos_feject + 4, 5, 5, 11, 9, bmps[loadbmp("fumee34.bmp") ].image);


	_assignation_explosions(explos_explosion, 4, 3, 2, 1, bmps[loadbmp("explos01.bmp") ].image);

	_assignation_explosions(explos_explosion + 1, 12, 11, 6, 5, bmps[loadbmp("explos02.bmp") ].image);

	_assignation_explosions(explos_explosion + 2, 19, 15, 9, 7, bmps[loadbmp("explos03.bmp") ].image);

	_assignation_explosions(explos_explosion + 3, 25, 18, 12, 9, bmps[loadbmp("explos04.bmp") ].image);

	_assignation_explosions(explos_explosion + 4, 26, 19, 13, 9, bmps[loadbmp("explos05.bmp") ].image);

	_assignation_explosions(explos_explosion + 5, 25, 18, 12, 9, bmps[loadbmp("explos06.bmp") ].image);

	_assignation_explosions(explos_explosion + 6, 27, 18, 13, 9, bmps[loadbmp("explos07.bmp") ].image);

	_assignation_explosions(explos_explosion + 7, 29, 19, 14, 9, bmps[loadbmp("explos08.bmp") ].image);

	_assignation_explosions(explos_explosion + 8, 27, 20, 13, 10, bmps[loadbmp("explos09.bmp") ].image);

	_assignation_explosions(explos_explosion + 9, 24, 19, 12, 9, bmps[loadbmp("explos10.bmp") ].image);

	_assignation_explosions(explos_explosion + 10, 24, 21, 12, 10, bmps[loadbmp("explos11.bmp") ].image);

	_assignation_explosions(explos_explosion + 11, 21, 18, 10, 9, bmps[loadbmp("explos12.bmp") ].image);

	_assignation_explosions(explos_explosion + 12, 19, 17, 9, 8, bmps[loadbmp("explos13.bmp") ].image);

	_assignation_explosions(explos_explosion + 13, 17, 16, 9, 8, bmps[loadbmp("explos14.bmp") ].image);

	_assignation_explosions(explos_explosion + 14, 12, 15, 8, 7, bmps[loadbmp("explos15.bmp") ].image);

	_assignation_explosions(explos_explosion + 15, 3, 3, 7, 6, bmps[loadbmp("explos16.bmp") ].image);


	_assignation_perss(explos_etoile, 9, 10, 5, 5, bmps[loadbmp("projet01.bmp") ].image);
	convbmp(pers[explos_etoile].image, 9, 10);

	_assignation_perss(explos_etoile + 1, 10, 10, 5, 5, bmps[loadbmp("projet02.bmp") ].image);
	convbmp(pers[explos_etoile + 1].image, 10, 10);

	_assignation_perss(explos_etoile + 2, 10, 9, 5, 5, bmps[loadbmp("projet03.bmp") ].image);
	convbmp(pers[explos_etoile + 2].image, 10, 9);

	_assignation_perss(explos_etoile + 3, 10, 10, 5, 5, bmps[loadbmp("projet04.bmp") ].image);
	convbmp(pers[explos_etoile + 3].image, 10, 10);

	_assignation_perss(explos_etoile + 4, 9, 10, 5, 5, bmps[loadbmp("projet05.bmp") ].image);
	convbmp(pers[explos_etoile + 4].image, 9, 10);

	_assignation_perss(explos_etoile + 5, 10, 10, 5, 5, bmps[loadbmp("projet06.bmp") ].image);
	convbmp(pers[explos_etoile + 5].image, 10, 10);

	_assignation_perss(explos_etoile + 6, 10, 9, 5, 5, bmps[loadbmp("projet07.bmp") ].image);
	convbmp(pers[explos_etoile + 6].image, 10, 9);

	_assignation_perss(explos_etoile + 7, 10, 10, 5, 5, bmps[loadbmp("projet08.bmp") ].image);
	convbmp(pers[explos_etoile + 7].image, 10, 10);

	_assignation_perss(explos_bulscope, 7, 7, 3, 3, bmps[loadbmp("bulscope.bmp") ].image);
	convbmp(pers[explos_bulscope].image, 7, 7);

	_assignation_perss(explos_buraygun, 28, 11, 20, 5, bmps[loadbmp("buraygun.bmp") ].image);
	convbmp(pers[explos_buraygun].image, 28, 11);


//int j = 2;
//	if(GpDirEnumNum("gp:\\gpmm\\smashgp", &listlen) == SM_OK) { //Ouverture de tous les fichiers .chr & .arn
	dhandle = sceIoDopen(psp_path);
	if(dhandle >= 0) {
//		GpDirEnumList("gp:\\gpmm\\smashgp", 0, 256, &list[0], &rcount);
//		for(i = 0; i < rcount; i++) {
		for(;;) {
			memset(&ename, 0, sizeof(ename) );
			i = sceIoDread(dhandle, &ename);
			if(isdir(ename.d_name) == 0) { //C'est un fichier
//StringTransBlit(0, 10, j, ename.d_name);
//j += 8;
				if(strstr(ename.d_name, ".chr") || strstr(ename.d_name, ".CHR") ) loaddefperso(ename.d_name); //C'est un fichier .chr
				if(strstr(ename.d_name, ".arn") || strstr(ename.d_name, ".ARN") ) loaddefarena(ename.d_name); //C'est un fichier .arn
			}
			if(i < 1) break;
		}
	}
//StringTransBlit(0, 10, 10, game_path);
//wait(5000);
}
