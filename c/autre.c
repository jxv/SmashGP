#define	max_secoue	20

//#define	nb_palettes	15
//#define	palette_size	256
//#define	pal_maxi		0

//GP_PALETTEENTRY PAL[nb_palettes][palette_size];
unsigned short pal[256];

//const unsigned short dumsound[1] = {
//	0x8000
//};

const short dumsound[1] = {
	0x0
};

const unsigned short dumimage[1] = {
	0xfc1f
};

const int tabsecouex[max_secoue + 1] = {
	0,-1,0,1,0,-1,0,1,0,-2,-1,1,2,0,-2,2,4,2,-2,-6,2
};

const int tabsecouey[max_secoue + 1] = {
	0,1,0,-2,1,3,0,-4,1,5,0,-6,2,8,-0,-10,2,12,0,-15,10
};

const int tabfleche[8][3][2] = {
	{
		{17, 52},
		{17, 62},
		{17, 72}
	},
	{
		{8, 48},
		{4, 57},
		{-1, 66}
	},
	{
		{-1, 37},
		{-8, 44},
		{-15, 51}
	},
	{
		{-11, 22},
		{-20, 26},
		{-29, 30}
	},
	{
		{-18, 17},
		{-28, 17},
		{-38, 17}
	},
	{
		{-11, 9},
		{-20, 5},
		{-29, 1}
	},
	{
		{-1, -1},
		{-8, -8},
		{-15, -15}
	},
	{
		{8, -11},
		{4, -20},
		{-1, -29}
	}
};
