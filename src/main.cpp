#include "main.h"

#include "PisteEngine.h"

#include "settings.h"
#include "graphics.h"
#include "language.h"

#include "game.h"
#include "manu.h"
//#include
#include "scenes.h"

#include "controls.h" // mouse

#define GAME_NAME   "Pekka Kana 2"
#define PK2_VERSION "split"

void FirstStart(){
	strcpy(pelaajan_nimi,tekstit->Hae_Teksti(txt_player_default_name));
	srand((unsigned)time(NULL));
	strcpy(viesti,"no message");
	if(!test_level){
		strcpy(episodi,"");
		strcpy(seuraava_kartta,"untitle1.map");
	}

	jakso = 1;

	if (!taulut_laskettu){
		//PisteLog_Kirjoita("  - Precalculating... \n");
		PK_Laske_Taulut();

		PK2Kartta_Cos_Sin(cos_table, sin_table);
		taulut_laskettu = true;
	}

	PK_Alusta_Fadetekstit();

	if (KARTANPIIRTO_LEVEYS > RUUDUN_LEVEYS)
		KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;

	if (KARTANPIIRTO_KORKEUS > RUUDUN_KORKEUS)
		KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;

	if (!RAJAA_KARTANPIIRTOALUE){

		KARTANPIIRTO_LEVEYS = RUUDUN_LEVEYS;
		KARTANPIIRTO_KORKEUS = RUUDUN_KORKEUS;
	}

	PK2Kartta_Aseta_Ruudun_Mitat(KARTANPIIRTO_LEVEYS,KARTANPIIRTO_KORKEUS);

	kartta = new PK2Kartta();

	if (!settings.isFiltered)
		PisteDraw2_SetFilter(PD_FILTER_NEAREST);
	if (settings.isFiltered)
		PisteDraw2_SetFilter(PD_FILTER_BILINEAR);
	PisteDraw2_FitScreen(settings.isFit);
	PisteDraw2_FullScreen(settings.isFullScreen);

	PisteDraw2_Image_Delete(kuva_peli); //Delete if there is a image allocated
	kuva_peli = PisteDraw2_Image_Load("gfx/pk2stuff.bmp", false);

	PisteDraw2_Image_Delete(kuva_peli_sysmem);
	kuva_peli_sysmem = PisteDraw2_Image_Load("gfx/pk2stuff.bmp", false);

	int ind_font = 0,
		ind_path = 0;

	ind_path = tekstit->Hae_Indeksi("font path");

	ind_font = tekstit->Hae_Indeksi("font small font");
	if (ind_path == -1 || ind_font == -1) {
		if ((fontti1 = PisteDraw2_Font_Create("language/fonts/","ScandicSmall.txt")) == -1){
			PK2_virhe = true;
		}
	}
	else {
		if ((fontti1 = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == -1){
			PK2_virhe = true;
		}
	}

	ind_font = tekstit->Hae_Indeksi("font big font normal");
	if (ind_path == -1 || ind_font == -1) {
		if ((fontti2 = PisteDraw2_Font_Create("language/fonts/","ScandicBig1.txt")) == -1){
		PK2_virhe = true;
		}
	}
	else {
		if ((fontti2 = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == -1){
		PK2_virhe = true;
		//PisteLog_Kirjoita("    - Loading font ");
		//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
		//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
		//PisteLog_Kirjoita(" failed!\n");
		}
	}

	ind_font = tekstit->Hae_Indeksi("font big font hilite");
	if (ind_path == -1 || ind_font == -1) {
		if ((fontti3 = PisteDraw2_Font_Create("language/fonts/","ScandicBig2.txt")) == -1){
		PK2_virhe = true;
		}
	}
	else {
		if ((fontti3 = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == -1){
		PK2_virhe = true;
		//PisteLog_Kirjoita("    - Loading font ");
		//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
		//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
		//PisteLog_Kirjoita(" failed!\n");
		}
	}

	ind_font = tekstit->Hae_Indeksi("font big font shadow");
	if (ind_path == -1 || ind_font == -1) {
		if ((fontti4 = PisteDraw2_Font_Create("language/fonts/","ScandicBig3.txt")) == -1){
		PK2_virhe = true;
		}
	}
	else {
		if ((fontti4 = PisteDraw2_Font_Create(tekstit->Hae_Teksti(ind_path),tekstit->Hae_Teksti(ind_font))) == -1){
		PK2_virhe = true;
		//PisteLog_Kirjoita("    - Loading font ");
		//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_path));
		//PisteLog_Kirjoita(tekstit->Hae_Teksti(ind_font));
		//PisteLog_Kirjoita(" failed!\n");
		}
	}

	if ((fontti2 = PisteDraw2_Font_Create("language/fonts/","ScandicBig1.txt")) == -1){
		PK2_virhe = true;
	}

	if ((fontti3 = PisteDraw2_Font_Create("language/fonts/","ScandicBig2.txt")) == -1){
		PK2_virhe = true;
	}

	if ((fontti4 = PisteDraw2_Font_Create("language/fonts/","ScandicBig3.txt")) == -1){
		PK2_virhe = true;
	}

	PK_Sprite_Tyhjenna();

	PK_Episodit_Hae();
	PK_Jaksot_Alusta();
	PK_Jaksot_Hae();

	PisteDraw2_ScreenFill(0);

	//PisteLog_Kirjoita("  - Loading basic sound fx \n");

	if ((kytkin_aani = PisteSound_LoadSFX("sfx/switch3.wav"))==-1)
		PK2_virhe = true;

	if ((hyppy_aani  = PisteSound_LoadSFX("sfx/jump4.wav"))==-1)
		PK2_virhe = true;

	if ((loiskahdus_aani  = PisteSound_LoadSFX("sfx/splash.wav"))==-1)
		PK2_virhe = true;

	if ((avaa_lukko_aani  = PisteSound_LoadSFX("sfx/openlock.wav"))==-1)
		PK2_virhe = true;

	if ((menu_aani  = PisteSound_LoadSFX("sfx/menu2.wav"))==-1)
		PK2_virhe = true;

	if ((ammuu_aani  = PisteSound_LoadSFX("sfx/moo.wav"))==-1)
		PK2_virhe = true;

	if ((kieku_aani  = PisteSound_LoadSFX("sfx/doodle.wav"))==-1)
		PK2_virhe = true;

	if ((tomahdys_aani  = PisteSound_LoadSFX("sfx/pump.wav"))==-1)
		PK2_virhe = true;

	if ((pistelaskuri_aani = PisteSound_LoadSFX("sfx/counter.wav"))==-1)
		PK2_virhe = true;

	PisteDraw2_FadeIn(PD_FADE_SLOW);

	//PisteLog_Kirjoita("  - Calculating tiles. \n");
	PK_Palikka_Laske_Palikat();

	PK_Esineet_Alusta();

	//PisteLog_Kirjoita("  - Loading background picture \n");
	PisteDraw2_Image_Delete(kuva_tausta);
	kuva_tausta = PisteDraw2_Image_Load("gfx/menu.bmp",true);

	PK_Tallennukset_Tyhjenna();

	//PisteLog_Kirjoita("  - Loading saves \n");
	PK_Tallennukset_Hae_Kaikki("data/saves.dat");

	//PisteLog_Kirjoita("  - PisteSound sounds on \n");
	//PisteSound_Aanet_Paalla(settings.aanet);

	//PisteLog_Kirjoita("- Initializing basic stuff completed \n");
}
void StartTest(const char* arg){
	if (arg == NULL) return;

	char buffer[_MAX_PATH];
	int sepindex;

	strcpy(buffer, arg);
	for (sepindex = 0; sepindex < _MAX_PATH; sepindex++)
		if(buffer[sepindex]=='/') break;

	strcpy(episodi, buffer); episodi[sepindex] = '\0';
	strcpy(seuraava_kartta, buffer + sepindex + 1);

	printf("PK2    - testing episode '%s' level '%s'\n", episodi, seuraava_kartta);

	PK_Lataa_Lisainfot();
	peli_kesken = false;
	PK_Uusi_Peli();

	siirry_kartasta_peliin = true;
	musiikin_voimakkuus = 0;
	peli_kesken = false;
}
int UpdateScreen(){
	if (pelin_seuraava_tila != pelin_tila){

		PisteDraw2_FadeIn(PD_FADE_NORMAL);

		switch (pelin_seuraava_tila){
			case TILA_PERUSALUSTUS: FirstStart();       break;
			case TILA_KARTTA:       PK_Start_Map();     break;
			case TILA_MENUT:        PK_Start_Menu();    break;
			case TILA_PELI:         PK_Start_Game();    break;
			case TILA_PISTELASKU:   PK_Start_Scores();  break;
			case TILA_INTRO:        PK_Start_Intro();   break;
			case TILA_LOPPU:        PK_Start_EndGame(); break;
			default: break;
		}

		pelin_tila = pelin_seuraava_tila;
	}
	return 0;
}
int Unload(){
	if (!unload){
		PisteSound_StopMusic();
		delete kartta;
		delete tekstit;
		unload = true;
	}
	return 0;
}

void PK_Quit(){
	if(!lopeta_peli) PisteDraw2_FadeOut(PD_FADE_FAST);
	lopeta_peli = true;
	musiikin_voimakkuus = 0;
}

int MainLoop(){
	static bool window_activated = true;
    bool window_closed = false;

	UpdateScreen();

	if (window_closed){
		running = false;
		return 0;
	}

	// Update mouse
	if (window_activated) {
		MOUSE hiiri = PisteInput_UpdateMouse(pelin_tila == TILA_KARTTA);
		if (hiiri.x < 0) hiiri.x = 0;
		if (hiiri.y < 0) hiiri.y = 0;
		if (hiiri.x > 640-19) hiiri.x = 640-19;
		if (hiiri.y > 480-19) hiiri.y = 480-19;

		hiiri_x = hiiri.x;
		hiiri_y = hiiri.y;
	}
	else {
		hiiri_x = -30;
		hiiri_y = -30;
		if (PisteInput_Hiiri_Oikea() || PisteInput_Hiiri_Vasen()) {
			PisteInput_ActivateWindow(true);
			window_activated = true;
			menu_valittu_id = 0;
		}
	}

	switch (pelin_tila){
		case TILA_PELI       : PK_Main_Peli();       break;
		case TILA_MENUT      : PK_Main_Menut();      break;
		case TILA_KARTTA     : PK_Main_Kartta();     break;
		case TILA_PISTELASKU : PK_Main_Pistelasku(); break;
		case TILA_INTRO      : PK_Main_Intro();      break;
		case TILA_LOPPU      : PK_Main_Loppu();      break;
		default              : lopeta_peli = true;   break;
	}

	// Update Sounds
	bool changed = false;

	if (musiikin_voimakkuus != musiikin_voimakkuus_nyt)
		changed = true;

	if (musiikin_max_voimakkuus > 64)
		musiikin_max_voimakkuus = 64;

	if (musiikin_max_voimakkuus < 0)
		musiikin_max_voimakkuus = 0;

	if (musiikin_voimakkuus > musiikin_max_voimakkuus)
		musiikin_voimakkuus = musiikin_max_voimakkuus;

	if (musiikin_voimakkuus_nyt < musiikin_voimakkuus)
		musiikin_voimakkuus_nyt++;

	if (musiikin_voimakkuus_nyt > musiikin_voimakkuus)
		musiikin_voimakkuus_nyt--;

	if (musiikin_voimakkuus_nyt > 64)
		musiikin_voimakkuus_nyt = 64;

	if (musiikin_voimakkuus_nyt < 0)
		musiikin_voimakkuus_nyt = 0;

	if (changed && settings.musiikki)
		PisteSound_SetMusicVolume(musiikin_voimakkuus_nyt);

	static bool wasPressed = false;
	bool skipped = !skip_frame && doublespeed; // If is in double speed and don't skip this frame, so the last frame was skipped, and it wasn't drawn

	if (PisteInput_Keydown(PI_ESCAPE) && key_delay == 0 && !skipped){ //Don't activate menu whith a not drawn screen
		if(test_level)
			PK_Quit();
		else{
			if (menu_nyt != MENU_PAAVALIKKO || pelin_tila != TILA_MENUT){
				pelin_seuraava_tila = TILA_MENUT;
				menu_nyt = MENU_PAAVALIKKO;
				degree_temp = degree;
			}
			else if (pelin_tila == TILA_MENUT && !wasPressed && PisteInput_Keydown(PI_ESCAPE) && menu_lue_kontrollit == 0){ // Just pressed escape in menu
				if(menu_valittu_id == menu_valinta_id-1)
					PK_Quit();
				else {
					menu_valittu_id = menu_valinta_id-1; // Set to "exit" option
					window_activated = false;
					PisteInput_ActivateWindow(false);
				}
			}
		}
	}

	wasPressed = PisteInput_Keydown(PI_ESCAPE);

	if (lopeta_peli && !PisteDraw2_IsFading())
		window_closed = true;
	running = !PK2_virhe;
	return 0;
}
int main(int argc, char *argv[]){
	char* test_path = NULL;

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "dev") == 0) {
			dev_mode = true;
			Piste_SetDebug(true);
		}
		if (strcmp(argv[i], "nolimits") == 0) RAJAA_KARTANPIIRTOALUE = false;
		if (strcmp(argv[i], "test") == 0) {
			if (argc <= i + 1) PK2_virhe = true;
			else {
				test_level = true;
				test_path = argv[i + 1];
			}
		}
		if (strcmp(argv[i], "version") == 0) {
			printf(PK2_VERSION);
			printf("\n");
			exit(0);
		}
	}

	if (Piste_Init(RUUDUN_LEVEYS, RUUDUN_KORKEUS, GAME_NAME) < 0) {
		printf("PK2    - Failed to init PisteEngine.\n");
		return 0;
	}
	//Piste_SetFPS(60);

	//Game works in ../res
	chdir("../res");
	strcpy(tyohakemisto,".");

	PK_Asetukset_Lataa("data/settings.ini");

	tekstit = new PisteLanguage();

	if (!PK_Lataa_Kieli()){
		printf("PK2    - Could not find %s!\n",settings.kieli);
		strcpy(settings.kieli,"english.txt");
		if(!PK_Lataa_Kieli()){
			printf("PK2    - Could not find the default language file!\n");
			return 0;
		}
	}

	UpdateScreen(); // call FirstStart?

	pelin_seuraava_tila = TILA_INTRO;
	if (dev_mode)
		pelin_seuraava_tila = TILA_MENUT;
	if (test_level) {
		pelin_seuraava_tila = TILA_PELI;
		StartTest(test_path);
	}

	Piste_Loop(running, *MainLoop);

	if(PK2_virhe)
		printf("PK2    - Error!\n");

	PK_Asetukset_Tallenna("data/settings.ini");

	Unload();
	Piste_Quit();

	return 0;
}